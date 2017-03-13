/*
Name: QtRpt
Version: 2.0.1
Web-site: http://www.qtrpt.tk
Programmer: Aleksey Osipov
E-mail: aliks-os@ukr.net
Web-site: http://www.aliks-os.tk

Copyright 2012-2017 Aleksey Osipov

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

#include "RptCrossTabObject.h"

RptCrossTabObject::RptCrossTabObject()
{
    name = "RptCrossTabObject_DEMO";
    rect.setX(0);
    rect.setY(0);
    rect.setWidth(500);
    rect.setHeight(500);
    m_rowHeight = 20;
    m_rowCount = 3;
    m_colCount = 3;
    m_processedCount = 0;
    m_totalByRowVisible = false;
    m_totalByColumnVisible = false;
    m_subTotalVisible = false;

    qRegisterMetaType<RptCrossTabObject>("RptCrossTabObject");
}

/*!
 \fn int RptCrossTabObject::rowHeight()
    Gets height of the row.

    \sa setRowHeight
*/
int RptCrossTabObject::rowHeight()
{
    return m_rowHeight;
}

/*!
 \fn void RptCrossTabObject::setRowHeight(int height)
    Sets \a height of the row.

    \sa rect
*/
void RptCrossTabObject::setRowHeight(int height)
{
    m_rowHeight = height;
}

int RptCrossTabObject::colCount() const
{
    return m_colCount;
}

void RptCrossTabObject::setColCount(int value)
{
    m_colCount = m_totalByRowVisible ? value+1 : value;
}

int RptCrossTabObject::rowCount() const
{
    return m_rowCount;
}

void RptCrossTabObject::setRowCount(int value)
{
    m_rowCount = value;

    if (m_subTotalVisible)
        m_rowCount = m_rowCount + qCeil(value/visibleRowCount());
    if (m_totalByColumnVisible)
        m_rowCount = m_rowCount+1;
}

bool RptCrossTabObject::isTotalByRowVisible()
{
    return m_totalByRowVisible;
}

void RptCrossTabObject::setTotalByRowVisible(bool value)
{
    m_totalByRowVisible = value;
}

bool RptCrossTabObject::isTotalByColumnVisible()
{
    return m_totalByColumnVisible;
}

void RptCrossTabObject::setTotalByColumnVisible(bool value)
{
    m_totalByColumnVisible = value;
}

bool RptCrossTabObject::isSubTotalVisible()
{
    return m_subTotalVisible;
}

void RptCrossTabObject::setSubTotalVisible(bool value)
{
    m_subTotalVisible = value;
}

int RptCrossTabObject::processedCount()
{
    return m_processedCount;
}

void RptCrossTabObject::setProcessedCount(int value)
{
    m_processedCount = value;
}

void RptCrossTabObject::buildMatrix()
{
    qDeleteAll(fieldList);
    fieldList.clear();
    m_processedCount = 0;

    float fieldWidth = rect.width();
    float fieldheight = rect.height();
    if (m_colCount == 0)
        return;
    if (m_rowCount == 0)
        return;

    fieldWidth  = rect.width()/colCount();
    fieldheight = rowHeight();

    for (unsigned row = 0; row < m_rowCount; row++) {
        for (unsigned col = 0; col < m_colCount; col++) {
            auto h1 = new RptFieldObject();
            h1->parentCrossTab = this;
            h1->name = QString("f%1%2").arg(col).arg(row);
            h1->fieldType = Text;
            h1->rect.setTop(rect.top() + fieldheight * row);
            h1->rect.setLeft(rect.left() + fieldWidth * col);
            h1->rect.setHeight(fieldheight);
            h1->rect.setWidth(fieldWidth);
            h1->aligment = Qt::AlignCenter;
            addField(h1);
        }
    }
}

/*!
 \fn int RptCrossTabObject::fieldRow(RptFieldObject* field)
    Define the row of the \a field RptCrossTabObject object.

    \sa fieldCol
*/
int RptCrossTabObject::fieldRow(RptFieldObject *field)
{
    int index = fieldList.indexOf(field);
    if (index != -1)
        index = unsigned(index / m_colCount);

    return index;
}

/*!
 \fn int RptCrossTabObject::fieldCol(RptFieldObject* field)
    Define the column of the \a field RptCrossTabObject object.

    \sa fieldRow
*/
int RptCrossTabObject::fieldCol(RptFieldObject *field)
{
    int index = fieldList.indexOf(field);
    if (index != -1) {
        unsigned row = unsigned(index / m_colCount);
        unsigned column = index - row * m_colCount;
        index = column;
    }
    return index;
}

/*!
 \fn RptCrossTabObject::visibleRowCount()
    return the visible count of rows on one page.

    \sa rowCount();
*/
int RptCrossTabObject::visibleRowCount()
{
    return (int)(parentField->rect.height() / rowHeight());
}

/*!
 \fn void RptCrossTabObject::addField(RptFieldObject *field)
    Add \a field to current RptCrossTabObject object.

    \sa RptFieldObject
*/
void RptCrossTabObject::addField(RptFieldObject *field)
{
    fieldList.append(field);
}

/*!
  Destroys the object, deleting all its child objects.
 */
RptCrossTabObject::~RptCrossTabObject()
{
    qDeleteAll(fieldList);
    fieldList.clear();
}

QDebug operator<<(QDebug dbg, const RptCrossTabObject &obj)
{
    dbg << obj.name << "\n";
    return dbg;
}

QDebug operator<<(QDebug dbg, const RptCrossTabObject *obj)
{
    return dbg << *obj;
}

//Bellow functions for working with a grid
/*!
 \fn void RptCrossTabObject::addElement(RptTabElement element)
    Add \a element as a RptFieldObject to the matrix.
    This function is only required when RptCrossTabObject is used when
    carry out report export to Excel.
*/
void RptCrossTabObject::addElement(RptTabElement element)
{
    m_elements.append(element);
}

/*!
 \fn void RptCrossTabObject::resortMatrix()
    This function resort Matrix which stores values of RptTabElement.
    This function is only required when RptCrossTabObject is used when
    carry out report export to Excel. In this case matrix hold on values
    of all fields. m_colHeader and m_rowHeader lists hold a numbers of positions
    of the each field, to right export to Excel grid, these numbers should be resorted
    and transfer to row(col) number of Excel.

    \sa RptTabElement element
*/
#ifdef QXLSX_LIBRARY
void RptCrossTabObject::buildXlsx(QXlsx::Document *xlsx)
{
    std::sort(m_elements.begin(), m_elements.end(), [](RptTabElement e1, RptTabElement e2) {return e1.top < e2.top; });

    int row = 0;
    int prevTop = INT_MIN;
    for (auto &element : m_elements) {
        if ((int)(element.top - 40) > prevTop) {
            row += 1;
            prevTop = element.top;
        }
        element.row = row;
    }

    std::sort(m_elements.begin(), m_elements.end(), [](RptTabElement e1, RptTabElement e2) {return e1.left < e2.left; });

    int col = 0;
    int prevLeft = INT_MIN;
    for (auto &element : m_elements) {
        if ((int)(element.left - 200) > prevLeft) {
            col += 1;
            prevLeft = element.left;
        }
        element.col = col;
    }

    for (const auto &element : m_elements)
        xlsx->write(element.row, element.col, element.value);
}
#endif
