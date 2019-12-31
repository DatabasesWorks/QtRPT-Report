/*
Name: QtRpt
Version: 2.1.0
Web-site: http://www.qtrpt.tk
Programmer: Aleksey Osipov
E-mail: aliks-os@ukr.net
Web-site: http://www.aliks-os.tk

Copyright 2012-2020 Aleksey Osipov

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

#include "RptScriptEngine.h"
#include <QDebug>
#include <QtMath>
#include "CommonClasses.h"
#include "qtrpt.h"

Q_DECLARE_METATYPE(QColor);

RptScriptEngine::RptScriptEngine(QObject *parent)
: QScriptEngine(parent)
{
    QScriptValue fun = this->newFunction(funcAggregate);
    this->globalObject().setProperty("SUM", fun);

    fun = this->newFunction(funcReplace);
    this->globalObject().setProperty("Replace", fun);

    fun = this->newFunction(funcToUpper);
    this->globalObject().setProperty("ToUpper", fun);

    fun = this->newFunction(funcToLower);
    this->globalObject().setProperty("ToLower", fun);

    fun = this->newFunction(funcNumberToWords);
    this->globalObject().setProperty("NumberToWords", fun);

    fun = this->newFunction(funcFrac);
    this->globalObject().setProperty("Frac", fun);

    fun = this->newFunction(funcFloor);
    this->globalObject().setProperty("Floor", fun);

    fun = this->newFunction(funcCeil);
    this->globalObject().setProperty("Ceil", fun);

    fun = this->newFunction(funcRound);
    this->globalObject().setProperty("Round", fun);

    fun = this->newFunction(funcDebug);
    this->globalObject().setProperty("debug", fun);

    fun = this->newFunction(qcolorValue);
    this->globalObject().setProperty("QColor", fun);

    addObject(parent);
}

void RptScriptEngine::addObject(QObject *object)
{
    QScriptValue scriptObject = this->newQObject(object);

    auto qtrpt = qobject_cast<QtRPT*>(object);
    if (qtrpt)
        this->globalObject().setProperty("QtRPT", scriptObject);
    else
        this->globalObject().setProperty(object->objectName(), scriptObject);
}

QScriptValue RptScriptEngine::evaluate(const QString &program, const QString &fileName, int lineNumber)
{
    Q_UNUSED(fileName);
    Q_UNUSED(lineNumber);

    QScriptValue result = QScriptEngine::evaluate(program);

    //qDebug() << program;

    if (this->hasUncaughtException())
    {
          int line = this->uncaughtExceptionLineNumber();
          qDebug() << "formulaStr: " << program;
          qDebug() << "uncaught exception at line" << line << ":" << result.toString();
    }

    return result;
}

//--------------------------------------------------------

QScriptValue qcolorValue(QScriptContext *context, QScriptEngine *engine)
{
    int r = context->argument(0).toInt32();
    int g = context->argument(1).toInt32();
    int b = context->argument(2).toInt32();
    return engine->toScriptValue(QColor(r,g,b,255));
}

QScriptValue funcReplace(QScriptContext *context, QScriptEngine *engine)
{
    Q_UNUSED(engine);

    QString param = context->argument(0).toString();
    QString strBefore = context->argument(1).toString();
    QString strAfter = context->argument(2).toString();
    return param.replace(strBefore, strAfter);
}

QScriptValue funcToUpper(QScriptContext *context, QScriptEngine *engine)
{
    Q_UNUSED(engine);

    QString param = context->argument(0).toString();
    return param.toUpper();
}

QScriptValue funcToLower(QScriptContext *context, QScriptEngine *engine)
{
    Q_UNUSED(engine);

    QString param = context->argument(0).toString();
    return param.toLower();
}

QScriptValue funcFrac(QScriptContext *context, QScriptEngine *engine)
{
    Q_UNUSED(engine);

    double value = context->argument(0).toString().toDouble();
    int b = qFloor(value);
    b = (value-b) * 100 + 0.5;
    return b;
}

QScriptValue funcFloor(QScriptContext *context, QScriptEngine *engine)
{
    Q_UNUSED(engine);

    double value = context->argument(0).toString().toDouble();
    return qFloor(value);
}

QScriptValue funcCeil(QScriptContext *context, QScriptEngine *engine)
{
    Q_UNUSED(engine);

    double value = context->argument(0).toString().toDouble();
    return qCeil(value);
}

QScriptValue funcRound(QScriptContext *context, QScriptEngine *engine)
{
    Q_UNUSED(engine);

    double value = context->argument(0).toString().toDouble();
    return qRound(value);
}

QScriptValue funcDebug(QScriptContext *context, QScriptEngine *engine)
{
    Q_UNUSED(engine);

    qDebug() << context->argument(0).toString();
    return "";
}

QScriptValue funcNumberToWords(QScriptContext *context, QScriptEngine *engine)
{
    Q_UNUSED(engine);

    QString paramLanguage = context->argument(0).toString();
    double value = context->argument(1).toString().toDouble();
    return double2Money(value, paramLanguage);
}

QScriptValue funcAggregate(QScriptContext *context, QScriptEngine *engine)
{
    Q_UNUSED(engine);

    QScriptValue self = context->thisObject();
    int funcMode = context->argument(0).toInteger();
    QString paramName = context->argument(1).toString();
    paramName = paramName.replace("'", "");

    double total = 0;
    double min = 0;
    double max = 0;
    int count = 0;

    if (!listOfPair.isEmpty())
        min = listOfPair.first().paramValue.toDouble();  //set initial value for Min

    for (const auto &aggValues : listOfPair) {
        if (aggValues.paramName == paramName) {
            if (!listIdxOfGroup.isEmpty() && self.property("showInGroup").toBool() == true) {
                for (auto &grpIdx : listIdxOfGroup) {
                    if (grpIdx == aggValues.lnNo) {
                        total += aggValues.paramValue.toDouble();
                        count += 1;
                        if (max < aggValues.paramValue.toDouble())
                            max = aggValues.paramValue.toDouble();
                        if (min > aggValues.paramValue.toDouble())
                            min = aggValues.paramValue.toDouble();
                    }
                }
            } else {
                if (!aggValues.paramValue.toString().isEmpty()) {
                    total += aggValues.paramValue.toDouble();
                    count += 1;
                    if (max < aggValues.paramValue.toDouble())
                        max = aggValues.paramValue.toDouble();
                    if (min > aggValues.paramValue.toDouble())
                        min = aggValues.paramValue.toDouble();
                }
            }
        }
    }

    switch (funcMode) {
    case 0:  //SUM
        return total;
    case 1:  //AVG
        if (count > 0)
            return total/count;
        else
            return 0;
    case 2:  //COUNT
        return count;
    case 3:  //MAX
        return max;
    case 4:  //MIN
        return min;
    default: return 0;
    }
    return 0;
}
