/*
Version: 1.5.4
Web-site: http://www.qtrpt.tk
Programmer: Aleksey Osipov
E-mail: aliks-os@ukr.net
Web-site: http://www.aliks-os.tk

Copyright 2012-2015 Aleksey Osipov

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "CommonClasses.h"
//#include <QDebug>

QString double2MoneyUKR(double n, int currency) {
    static QString cap[4][10] =	{
        {
            "",
            "один ",
            "два ",
            "три ",
            "чотири ",
            "п'ять ",
            "шість ",
            "сім ",
            "вісім ",
            "дев'ять "
        },
        {
            "",
            "одна ",
            "дві ",
            "три ",
            "чотири ",
            "п'ять ",
            "шість ",
            "сім ",
            "вісім ",
            "дев'ять "
        },
        {
            "",
            "",
            "двадцять ",
            "тридцять ",
            "сорок ",
            "п'ятдесят ",
            "шістдесят ",
            "сімдесят ",
            "вісімдесят ",
            "дев'яносто "
        },
        {
            "",
            "сто ",
            "двісти ",
            "триста ",
            "чотириста ",
            "п'ятсот ",
            "шістсот ",
            "сімсот ",
            "вісімсот ",
            "дев'ятсот "
        }
    };

    static QString cap2[10] = {
        "десять ",
        "одинадцять ",
        "дванадцять ",
        "тринадцять ",
        "чотирнадцять ",
        "п'ятнадцять ",
        "шістнадцять ",
        "сімнадцять ",
        "вісімнадцять ",
        "дев'ятнадцять "
    };

    static QString cap3[5][3] = {
        {
            "копійка",
            "копійки",
            "копійок"
        },
        {
            "гривня ",
            "гривні ",
            "гривень "
        },
        {
            "тисяча ",
            "тисячі ",
            "тисяч "
        },
        {
            "мілльон ",
            "мильони ",
            "мільонів "
        },
        {
            "мілліард ",
            "мільярди ",
            "мільярдів "
        }
    };

    if (currency == 0) {
        cap3[0][0] = "";
        cap3[0][1] = "";
        cap3[0][2] = "";

        cap3[1][0] = ", ";
        cap3[1][1] = ", ";
        cap3[1][2] = ", ";
    }
    QString s;

    long nn = ((int)(n)*10000 + (int)(n*1000) % 1000 + 5 ) / 10;

    long lo = 0;
    long hi = nn;
    int r[3];
    int rod;

    for ( int nPor = 0; true; ++nPor ) {
        QString olds = s;
        s = "";

        lo = hi%1000;
        hi = hi/1000;

        if ((nPor == 1) && (lo == 0) && (hi == 0)) {
            s = "ноль " + cap3[1][2] + olds;
            break;
        }

        for ( int nRaz = 0; nRaz < 3; ++nRaz ) {
            r[nRaz] = lo%10;
            lo      = lo/10;
        }

        switch ( nPor ) {
            case 0:
            case 1:
            case 2:
                rod = 1;
                break;
            default:
                rod = 0;
        }

        QString capt;
        if (r[1] != 1) {
            if (r[0] == 1)
                capt = cap3[nPor][0];
            else if ((r[0] > 1 ) && (r[0] < 5))
                capt = cap3[nPor][1];
        }
        if (capt.isEmpty())
            capt = cap3[nPor][2];

        if (nPor == 0) {
            s = QString::number(r[1]) + QString::number(r[0]) + " " + capt;
            continue;
        }

        s += cap[2+1][r[2]];
        s += ((r[1] == 1) ? cap2[r[0]] : cap[1+1][r[1]]+cap[0+rod][r[0]])+capt;
        if (hi == 0)  //Если не нужна часть после запятой
            s += olds;

        if (hi == 0 && nPor >= 1)
            break;
    }
    QString up(s[0]);
    s.replace(0,1,up);
    //return s.replace(" ,",",");
    return s.replace(",","");
}

QString double2MoneyRUS(double n, int currency) {
    QString v1,v2,v3,s1,s2,s3;
    switch(currency) {
        case 0: {  //БЕЗ ВАЛЮТЫ
            v1 = QString::fromUtf8(", ");
            v2 = QString::fromUtf8(", ");
            v3 = QString::fromUtf8(", ");
            s1 = QString::fromUtf8("один ");
            s2 = QString::fromUtf8("два ");
            break;
        }
        case 1: {  //ГРИВНА
            v1 = QString::fromUtf8("гривна, ");
            v2 = QString::fromUtf8("гривны, ");
            v3 = QString::fromUtf8("гривен, ");
            s1 = QString::fromUtf8("одна ");
            s2 = QString::fromUtf8("две ");
            break;
        }
        case 2: {  //РУБЛЬ
            v1 = QString::fromUtf8("рубль, ");
            v2 = QString::fromUtf8("рубля, ");
            v3 = QString::fromUtf8("рублей, ");
            s1 = QString::fromUtf8("один ");
            s2 = QString::fromUtf8("два ");
            break;
        }
    }

    if ( n > 999999999999.99 )
        return QString::fromUtf8("Очень много денег ...");

    static QString cap[4][10] = {
        {
            "",
            s1,
            s2,
            QString::fromUtf8("три "),
            QString::fromUtf8("четыре "),
            QString::fromUtf8("пять "),
            QString::fromUtf8("шесть "),
            QString::fromUtf8("семь "),
            QString::fromUtf8("восемь "),
            QString::fromUtf8("девять ")
        },
        {
            "",
            QString::fromUtf8("одна "),
            QString::fromUtf8("две "),
            QString::fromUtf8("три "),
            QString::fromUtf8("четыре "),
            QString::fromUtf8("пять "),
            QString::fromUtf8("шесть "),
            QString::fromUtf8("семь "),
            QString::fromUtf8("восемь "),
            QString::fromUtf8("девять ")
        },
        {
            "",
            "",
            QString::fromUtf8("двадцать "),
            QString::fromUtf8("тридцать "),
            QString::fromUtf8("сорок "),
            QString::fromUtf8("пятьдесят "),
            QString::fromUtf8("шестьдесят "),
            QString::fromUtf8("семьдесят "),
            QString::fromUtf8("восемьдесят "),
            QString::fromUtf8("девяносто ")
        },
        {
            "",
            QString::fromUtf8("сто "),
            QString::fromUtf8("двести "),
            QString::fromUtf8("триста "),
            QString::fromUtf8("четыреста "),
            QString::fromUtf8("пятьсот "),
            QString::fromUtf8("шестьсот "),
            QString::fromUtf8("семьсот "),
            QString::fromUtf8("восемьсот "),
            QString::fromUtf8("девятьсот ")
        }
    };

    static QString cap2[10] = {
        QString::fromUtf8("десять "),
        QString::fromUtf8("одиннадцать "),
        QString::fromUtf8("двенадцать "),
        QString::fromUtf8("тринадцать "),
        QString::fromUtf8("четырнадцать "),
        QString::fromUtf8("пятнадцать "),
        QString::fromUtf8("шестнадцать "),
        QString::fromUtf8("семнадцать "),
        QString::fromUtf8("восемнадцать "),
        QString::fromUtf8("девятнадцать ")
    };

    static QString cap3[5][3] = {
        {
            QString::fromUtf8("копейка "),
            QString::fromUtf8("копейки "),
            QString::fromUtf8("копеек ")
        },
        {
            v1,
            v2,
            v3
        },
        {
            QString::fromUtf8("тысяча "),
            QString::fromUtf8("тысячи "),
            QString::fromUtf8("тысяч ")
        },
        {
            QString::fromUtf8("миллион "),
            QString::fromUtf8("милиона "),
            QString::fromUtf8("миллионов ")
        },
        {
            QString::fromUtf8("миллиард "),
            QString::fromUtf8("миллиарда "),
            QString::fromUtf8("миллиардов ")
        }
    };

    if (currency == 0) {
        cap3[0][0] = "";
        cap3[0][1] = "";
        cap3[0][2] = "";
    }
    QString s;

    qlonglong nn = ((qlonglong)(n)*10000 + (qlonglong)(n*1000) % 1000 + 5 ) / 10;

    qlonglong lo = 0;
    qlonglong hi = nn;
    int r[3];
    int rod;

    for ( int nPor = 0; true; ++nPor ) {
        QString olds = s;
        s = "";

        lo = hi%1000;
        hi = hi/1000;

        if ((nPor == 1) && (lo == 0) && (hi == 0)) {
            s = QString::fromUtf8("ноль ") + cap3[1][2] + olds;
            break;
        }

        for ( int nRaz = 0; nRaz < 3; ++nRaz ) {
            r[nRaz] = lo%10;
            lo      = lo/10;
        }

        switch ( nPor ) {
            case 0:
            case 1:
            case 2:
                rod = 1;
                break;
            default:
                rod = 0;
        }

        QString capt;
        if (r[1] != 1) {
            if (r[0] == 1)
                capt = cap3[nPor][0];
            else if ((r[0] > 1 ) && (r[0] < 5))
                capt = cap3[nPor][1];
        }
        if (capt.isEmpty())
            capt = cap3[nPor][2];

        if (nPor == 0) {
            s = QString::number(r[1]) + QString::number(r[0]) + " " + capt;
            continue;
        }
        if (nPor == 1)
            rod = 0;

        s += cap[2+1][r[2]];
        s += ((r[1] == 1) ? cap2[r[0]] : cap[1+1][r[1]]+cap[0+rod][r[0]])+capt;
        s += olds;

        if ((hi == 0) && (nPor >= 1))
            break;
    }
    QString up(s[0]);
    s.replace(0,1,up);
    if ( n < 0 )
        return QString::fromUtf8("Минус ")+s;
    return s.replace(" ,",",");
}


//Based on https://github.com/icelander/number_to_text
//No license encumbrance.
QString double2MoneyENG(double number) {
    static QMap<double, QString> numbers;

    //Only initialize once
    if (numbers.isEmpty()) {
        numbers[0] = "zero";
        numbers[1] = "one";
        numbers[2] = "two";
        numbers[3] = "three";
        numbers[4] = "four";
        numbers[5] = "five";
        numbers[6] = "six";
        numbers[7] = "seven";
        numbers[8] = "eight";
        numbers[9] = "nine";
        numbers[10] = "ten";
        numbers[11] = "eleven";
        numbers[12] = "twelve";
        numbers[13] = "thirteen";
        numbers[14] = "fourteen";
        numbers[15] = "fifteen";
        numbers[16] = "sixteen";
        numbers[17] = "seventeen";
        numbers[18] = "eighteen";
        numbers[19] = "nineteen";
        numbers[20] = "twenty";
        numbers[30] = "thirty";
        numbers[40] = "forty";
        numbers[50] = "fifty";
        numbers[60] = "sixty";
        numbers[70] = "seventy";
        numbers[80] = "eighty";
        numbers[90] = "ninety";
    }

    static QMap<uint, QString> powers;

    //Only initialize once
    if (powers.isEmpty()) {
        powers[2] = "hundred";
        powers[3] = "thousand";
        powers[6] = "million";
        powers[9] = "billion";
    }

    QString output;

    if (number < 21) {
        output = numbers[number];
    } else if (number < 100) {
        output = numbers[10 * qFloor(number / 10)];
        int remainder = (int)number % 10;

        if (remainder > 0)
            output += "-" + double2MoneyENG(remainder);
    } else {
        uint power = 2;
        uint place = 0;
        QString powerString;

        //QMap::keys is ordered
        foreach (uint pow, powers.keys()) {
            uint place_value = qPow(10, pow);
            uint tmp_place = qFloor(number / place_value);
            if (tmp_place < 1)
                break;

            place = tmp_place;
            power = pow;
            powerString = powers[pow];
        }

        if (power > 0) {
            output = double2MoneyENG(place) + " " + powerString;
            double remainder = (int)number % (int)double(qPow(10, power));

            if (remainder > 0)
            output += " " + double2MoneyENG(remainder);
        }
    }

    return output;
}

//Thank you to Norbert Schlia
QString double2MoneyGER(double number, bool bAdditional /*= false*/) {
    static QMap<double, QString> numbers;

    //Only initialize once
    if (numbers.isEmpty()) {
        numbers[0] = "null";
        numbers[1] = "ein";
        numbers[2] = "zwei";
        numbers[3] = "drei";
        numbers[4] = "vier";
        numbers[5] = "fünf";
        numbers[6] = "sechs";
        numbers[7] = "sieben";
        numbers[8] = "acht";
        numbers[9] = "neun";
        numbers[10] = "zehn";
        numbers[11] = "elf";
        numbers[12] = "zwölf";
        numbers[13] = "dreizehn";
        numbers[14] = "vierzehn";
        numbers[15] = "fünfzehn";
        numbers[16] = "sechzehn";
        numbers[17] = "siebzehn";
        numbers[18] = "achtzehn";
        numbers[19] = "neunzehn";
        numbers[20] = "zwanzig";
        numbers[30] = "dreissig";
        numbers[40] = "vierzig";
        numbers[50] = "fünfzig";
        numbers[60] = "sechszig";
        numbers[70] = "siebzig";
        numbers[80] = "achtzig";
        numbers[90] = "neunzig";
    }

    static QMap<uint, QString> powers;

    //Only initialize once
    if (powers.isEmpty()) {
        powers[2] = "hundert";
        powers[3] = "tausend";
        powers[6] = "millionen";
        powers[9] = "milliarden";
    }

    QString output;

    if ((long long)number == 1 && !bAdditional) {
        output = "eins";
    }
    else if (number < 21) {
        output = numbers[(long long)number];
    } else if (number < 100) {
        output = numbers[10 * qFloor(number / 10)];
        int remainder = (long long)number % 10;

        if (remainder > 0)
            output = double2MoneyGER(remainder, true) + "und" + output;
    } else {
        uint power = 2;
        uint place = 0;
        QString powerString;

        //QMap::keys is ordered
        foreach (uint pow, powers.keys()) {
            uint place_value = qPow(10, pow);
            uint tmp_place = qFloor(number / place_value);
            if (tmp_place < 1)
                break;

            place = tmp_place;
            power = pow;

            if (pow == 6 && number < 2E6) {
                powerString = "emillion";
            }
            else if (pow == 9 && number < 2E9) {
                powerString = "emilliarde";
            }
            else {
                powerString = powers[pow];
            }
        }

        if (power > 0) {
            output = double2MoneyGER(place, true) + powerString;
            double remainder = (long long)number % (long long)double(qPow(10, power));

            if (remainder > 0)
                output += double2MoneyGER(remainder, true);
        }
    }

    return output;
}

QString double2Money(double n, QString lang) {
    if (lang == "UKR")
        return double2MoneyUKR(n,0);
    else if (lang == "RUS")
        return double2MoneyRUS(n);
    else if (lang == "GER")
        return double2MoneyGER(n);
    else if (lang == "ENG")
        return double2MoneyENG(n);
    else
        return double2MoneyENG(n);
}

QString colorToString(QColor color) {
    QString str("rgba("+
                QString::number(color.red())+","+
                QString::number(color.green())+","+
                QString::number(color.blue())+","+
                QString::number(color.alpha())+")");
    return str;
}

QColor colorFromString(QString value) {
    QColor color;
    if (value.isEmpty()) return color;
    int start; int end;
    start = value.indexOf("(",0,Qt::CaseInsensitive);
    end =   value.indexOf(")",start+1,Qt::CaseInsensitive);
    QString tmp = value.mid(start+1,end-start-1);
    int v = tmp.section(",",0,0).toInt();
    color.setRed(v);
    v = tmp.section(",",1,1).toInt();
    color.setGreen(v);
    v = tmp.section(",",2,2).toInt();
    color.setBlue(v);
    v = tmp.section(",",3,3).toInt();
    color.setAlpha(v);
    return color;
}

QString eventType(QEvent *ev) {
    static int eventEnumIndex = QEvent::staticMetaObject.indexOfEnumerator("Type");
    QString name = QEvent::staticMetaObject.enumerator(eventEnumIndex).valueToKey(ev->type());

    if (!name.isEmpty()) return name; else return (QString)ev->type();
}
