/*
Name: QtRpt
Version: 2.0.0
Web-site: http://www.qtrpt.tk
Programmer: Aleksey Osipov
E-mail: aliks-os@ukr.net
Web-site: http://www.aliks-os.tk

Copyright 2012-2016 Aleksey Osipov

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

#ifndef RPTCROSSTABOBJECT_H
#define RPTCROSSTABOBJECT_H

#include <QDebug>
#include <QMetaType>
#include <RptFieldObject.h>

using namespace QtRptName;

class RptFieldObject;

struct RptTabElement
{
    RptFieldObject *fieldObject;
    QVariant value;
    unsigned left;
    unsigned top;
    unsigned height;
    unsigned width;
    unsigned corrLeft;
    unsigned corrTop;
};

typedef QVector<RptTabElement> VectorRptTabElement;

class RptCrossTabObject
{
public:
    RptCrossTabObject();
    ~RptCrossTabObject();
    QString name;
    QRect rect;
    int colCount() const;
    void setColCount(int value);
    int rowCount() const;
    void setRowCount(int value);
    int rowHeight();
    void setRowHeight(int height);
    int processedCount();
    void setProcessedCount(int value);
    int fieldRow(RptFieldObject* field);
    int fieldCol(RptFieldObject* field);

    void buildMatrix();
    QList<RptFieldObject*> fieldList;
    RptFieldObject *parentField;

    void addElement(RptTabElement element);
    void resortMatrix();

private:
    unsigned m_colCount;
    unsigned m_rowCount;
    unsigned m_rowHeight;
    unsigned m_processedCount;

    bool colTotalExists;
    bool rowTotalExists;

	QString	stTotal;

    void addField(RptFieldObject *field);

};

using SPtrCrossTab = QSharedPointer<RptCrossTabObject>;

Q_DECLARE_METATYPE(RptCrossTabObject)

QDebug operator<<(QDebug dbg, const RptCrossTabObject &obj);
QDebug operator<<(QDebug dbg, const RptCrossTabObject *obj);

#endif // RPTCROSSTABOBJECT_H
