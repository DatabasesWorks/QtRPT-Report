/*
Name: CommonFiles
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

#ifndef QTEXTEDITEX_H
#define QTEXTEDITEX_H

#include <QWidget>
#include <QTextEdit>
#include <QTextList>

namespace Ui {
    class QTextEditEx;
}

class QTextEditEx : public QWidget {
    Q_OBJECT
public:
    QTextEditEx(QWidget *parent = 0);
    ~QTextEditEx();
    QTextEdit *textEdit;

protected:

private:
    Ui::QTextEditEx *m_ui;
    void setupTextActions();
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void alignmentChanged(Qt::Alignment a);
    void fontChanged(const QFont &f);

private slots:
    void textSize(const QString &p);
    void textFamily(const QString &f);
    void textStyle(int styleIndex);
    void textBold();
    void textItalic();
    void textUnderline();
    void textAlign();
    void textColor();
    void currentCharFormatChanged(const QTextCharFormat &format);
    void cursorPositionChanged();
    void textDirection();
};

#endif // QTEXTEDITEX_H
