#include "RptScriptEngine.h"
#include <QDebug>
#include <QtMath>
#include "CommonClasses.h"

RptScriptEngine::RptScriptEngine(QObject *parent)
: QScriptEngine(parent)
{
    QScriptValue fun = this->newFunction(funcAggregate);
    this->globalObject().setProperty("Sum", fun);

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
}

QScriptValue RptScriptEngine::evaluate(const QString &program, const QString &fileName, int lineNumber)
{
    Q_UNUSED(fileName);
    Q_UNUSED(lineNumber);

    QScriptValue result = QScriptEngine::evaluate(program);

    qDebug() << program;

    if (this->hasUncaughtException())
    {
          int line = this->uncaughtExceptionLineNumber();
          qDebug() << "formulaStr: " << program;
          qDebug() << "uncaught exception at line" << line << ":" << result.toString();
    }

    return result;
}
//--------------------------------------------------------

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
