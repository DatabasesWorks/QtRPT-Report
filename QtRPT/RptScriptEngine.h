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

#pragma once

#include <QScriptEngine>

struct AggregateValues
{
    QString paramName;
    QVariant paramValue;
    int lnNo;
    int pageReport;
};

extern QList<AggregateValues> listOfPair;
extern QList<int> listIdxOfGroup;

//--------------------------------------------------------

class RptScriptEngine : public QScriptEngine
{
public:
    RptScriptEngine(QObject *parent = nullptr);
    void addObject(QObject *object);
    QScriptValue evaluate(const QString &program, const QString &fileName = QString(), int lineNumber = 1);
};

//--------------------------------------------------------

QScriptValue qcolorValue(QScriptContext *context, QScriptEngine *engine);
QScriptValue funcReplace(QScriptContext *context, QScriptEngine *engine);
QScriptValue funcToUpper(QScriptContext *context, QScriptEngine *engine);
QScriptValue funcToLower(QScriptContext *context, QScriptEngine *engine);
QScriptValue funcFrac(QScriptContext *context, QScriptEngine *engine);
QScriptValue funcFloor(QScriptContext *context, QScriptEngine *engine);
QScriptValue funcCeil(QScriptContext *context, QScriptEngine *engine);
QScriptValue funcRound(QScriptContext *context, QScriptEngine *engine);
QScriptValue funcDebug(QScriptContext *context, QScriptEngine *engine);
QScriptValue funcNumberToWords(QScriptContext *context, QScriptEngine *engine);
QScriptValue funcAggregate(QScriptContext *context, QScriptEngine *engine);

