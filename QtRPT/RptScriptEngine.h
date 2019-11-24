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
};

//--------------------------------------------------------

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

