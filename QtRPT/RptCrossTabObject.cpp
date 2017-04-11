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
    totalBackgroundColor = Qt::white;
    headerBackgroundColor = Qt::white;
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
        m_rowCount = m_rowCount + qCeil((double)value/visibleRowCount());
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

    for (quint32 row = 0; row < m_rowCount; row++) {
        for (quint32 col = 0; col < m_colCount; col++) {
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

bool RptCrossTabObject::isTotalField(RptFieldObject *field)
{
    quint32 col = fieldCol(field);
    quint32 row = fieldRow(field, true);
    quint32 page = (int)row/visibleRowCount();
    quint32 visibleRow = row - page * visibleRowCount();

    bool totalByRow = false;
    if (m_totalByRowVisible && col+1 == m_colCount)
        totalByRow = true;

    bool totalByCol = false;
    if (m_totalByColumnVisible && row+1 == m_rowCount)
        totalByCol = true;

    bool sub_totalByCol = false;
    if (m_subTotalVisible) {
        if (visibleRow+1 == (quint32)visibleRowCount())
            sub_totalByCol = true;

        if (m_totalByColumnVisible) {
            if (row+2 == m_rowCount)
                sub_totalByCol = true;
        } else {
            if (row+1 == m_rowCount)
                sub_totalByCol = true;
        }
    }

    return totalByRow || sub_totalByCol || totalByCol;
}

void RptCrossTabObject::total(RptFieldObject *field)
{
    quint32 col = fieldCol(field);
    quint32 row = fieldRow(field, true);
    quint32 page = (int)row/visibleRowCount();
    bool grandTotal = row+1 == m_rowCount ? true : false;

    double total = 0;

    for (const auto &m_field : fieldList) {
        quint32 m_col = fieldCol(m_field);
        quint32 m_row = fieldRow(m_field, true);

        if (col+1 == m_colCount) {
            // total by row
            if (/*!isTotalField(m_field) &&*/ m_field != field && m_row == row)
                total += m_field->value.toDouble();
        } else {
            // total by column
            quint32 m_page = (int)m_row/visibleRowCount();

            if (!isTotalField(m_field) && m_col == col) {
                if (!grandTotal) {
                    if (m_page == page)
                        total += m_field->value.toDouble();
                } else {
                    total += m_field->value.toDouble();
                }
            }
        }
    }

    field->value = QString::number(total);
}

/*!
 \fn int RptCrossTabObject::fieldRow(RptFieldObject* field, bool realNr = false)
    Return the row number of the \a field RptCrossTabObject object.
    If \a realNr is a true, returns field row incliding all Total and Sub Total
    rows. By default the realNr is false, so we dont take in account the
    Total rows.

    \sa fieldCol
*/
int RptCrossTabObject::fieldRow(RptFieldObject *field, bool realNr)
{
    int index = fieldList.indexOf(field);
    if (index != -1)
        index = quint32(index / m_colCount);

    if (realNr) {
        // the row number, including all Totals
        return index;
    } else {
        // the row number, not including Total
        quint32 row = index;
        quint32 page = (int)row/visibleRowCount();
        return row - page;
    }
}

/*!
 \fn int RptCrossTabObject::fieldCol(RptFieldObject* field)
    Retun the column number of the \a field RptCrossTabObject object.

    \sa fieldRow
*/
int RptCrossTabObject::fieldCol(RptFieldObject *field)
{
    int index = fieldList.indexOf(field);
    if (index != -1) {
        quint32 row = quint32(index / m_colCount);
        quint32 column = index - row * m_colCount;
        index = column;
    }
    return index;
}

/*!
 \fn int RptCrossTabObject::visibleRowCount()
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
    const RptCrossTabObject *tmp = &obj;
    if (tmp != nullptr)
        dbg << obj.name << "\n";
    else
        dbg << nullptr;
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
