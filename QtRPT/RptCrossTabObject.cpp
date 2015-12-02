/*
Name: QtRpt
Version: 1.5.5
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

#include "RptCrossTabObject.h"

RptCrossTabObject::RptCrossTabObject() {
    colHeaderVisible = false;
    rowHeaderVisible = false;
    colTotalVisible = false;
    rowTotalVisible = false;
    colTotalExists = false;
    rowTotalExists = false;
    name = "RptCrossTabObject_DEMO";
    rect.setX(0);
    rect.setY(0);
    rect.setWidth(500);
    rect.setHeight(500);
	stTotal = QObject::tr("Total");

    qRegisterMetaType<RptCrossTabObject>( "RptCrossTabObject" );
}

//including Total if it visible
int RptCrossTabObject::colCount() const {
    return m_colHeader.size();
}

//including Total if it visible
int RptCrossTabObject::rowCount() const {
    return m_rowHeader.size();
}

int RptCrossTabObject::allColCount() const {
    int finalColCount = colCount();

    if (isRowHeaderVisible())
        finalColCount += 1;

    return finalColCount;
}

int RptCrossTabObject::allRowCount() const {
    int finalRowCount = rowCount();

    if (isColHeaderVisible())
        finalRowCount += 1;

    return finalRowCount;
}

//pure col count (without header and total)
int RptCrossTabObject::colDataCount() const {
    if (isColTotalVisible())
        return colCount()-1;
    else
        return colCount();
}

//pure row count (without header and total)
int RptCrossTabObject::rowDataCount() const {
    if (isRowTotalVisible())
        return rowCount()-1;
    else
        return rowCount();
}

void RptCrossTabObject::addCol(QString colName) {
    m_colHeader << colName;
}

void RptCrossTabObject::addRow(QString rowName) {
    m_rowHeader << rowName;
}

//
// Set vibility to Column Total, set colTotalExists
void RptCrossTabObject::setColTotalVisible(bool blValue) {
	colTotalVisible = blValue;
	setColTotalExists(blValue);
}

//
// Set vibility to Row Total, set rowTotalExists
void RptCrossTabObject::setRowTotalVisible(bool blValue) {
	rowTotalVisible = blValue;
	setRowTotalExists(blValue);
}

//
// Set existence of Row Total, when setted cannot be removed
void RptCrossTabObject::setRowTotalExists(bool blValue) {
	if (rowTotalExists)	// Row total already exists, cannot be removed
		return;
	if (!blValue)		// If false we return, we don't create the row, we cannot erase rows
		return;

	addRow(stTotal);
	rowTotalExists = blValue;
}

//
// Set existence of Column Total, when setted cannot be removed
void RptCrossTabObject::setColTotalExists(bool blValue) {
	if (colTotalExists)	// Col total already exists, cannot be removed
		return;
	if (!blValue)		// If false we return, we don't create the col, we cannot erase cols
		return;

	addCol(stTotal);
	colTotalExists = blValue;
}

//
// Get the name of column header
QString RptCrossTabObject::getColName(int col) const {
    return m_colHeader[col];
}

//
// Get the name of row header
QString RptCrossTabObject::getRowName(int row) const {
    return m_rowHeader[row];
}

//
// Get the index of a particular Column name, return 0 if not found, prevents cores
int RptCrossTabObject::getColIndex(QString stCol) const {
	int		siRet = m_colHeader.indexOf(stCol);
	if (siRet > 0)
		return siRet;
	else
		return 0;
}

//
// Get the index of a particular Row name, return 0 if not found, prevents cores
int RptCrossTabObject::getRowIndex(QString stRow) const {
	int		siRet = m_rowHeader.indexOf(stRow);
	if (siRet > 0)
		return siRet;
	else
		return 0;
}

void RptCrossTabObject::initMatrix() {
	double	dbWk1 = 0;  //Init value of cells
    valuesArray.resize(m_rowHeader.size());  //Set row count

    QMutableVectorIterator<VectorRptTabElement> iRows(valuesArray);
    while (iRows.hasNext())
        (iRows.next()).resize(m_colHeader.size());

	for (int row = 0; row < rowCount(); row++)
		for (int col = 0; col < colCount(); col++)
			valuesArray[row][col].value = dbWk1;
}

QVariant RptCrossTabObject::getMatrixValue(int col,int row) const {
    return valuesArray[row][col].value;
}

void RptCrossTabObject::setMatrixValue(QString stCol, QString stRow, QVariant vaValue) {
	double	dbWk1;

	// Put the value in the correct case, depending on name of row/col
	//
    valuesArray[getRowIndex(stRow)][getColIndex(stCol)].value = vaValue;

	// Add row total if exists
	if (rowTotalExists) {
    	dbWk1 = valuesArray[getRowIndex(stTotal)][getColIndex(stCol)].value.toDouble();
		dbWk1 += vaValue.toDouble();
    	valuesArray[getRowIndex(stTotal)][getColIndex(stCol)].value = dbWk1;
		// Add total/total
    	dbWk1 = valuesArray[getRowIndex(stTotal)][getColIndex(stTotal)].value.toDouble();
		dbWk1 += vaValue.toDouble();
    	valuesArray[getRowIndex(stTotal)][getColIndex(stTotal)].value = dbWk1;
	}

	// Add col total if exists
	if (colTotalExists) {
    	dbWk1 = valuesArray[getRowIndex(stRow)][getColIndex(stTotal)].value.toDouble();
		dbWk1 += vaValue.toDouble();
    	valuesArray[getRowIndex(stRow)][getColIndex(stTotal)].value = dbWk1;
		// Add total/total only if rowTotalExists is false
		if (!rowTotalExists) {
    		dbWk1 = valuesArray[getRowIndex(stTotal)][getColIndex(stTotal)].value.toDouble();
			dbWk1 += vaValue.toDouble();
    		valuesArray[getRowIndex(stTotal)][getColIndex(stTotal)].value = dbWk1;
		}
	}
}

void RptCrossTabObject::setMatrixElement(int col,int row, RptTabElement &element) {
    valuesArray[row][col] = element;
}

void RptCrossTabObject::makeFeelMatrix() {
	qDebug() << "makeFeelMatrix";
    float fieldWidth = rect.width();
    float fieldheight = rect.height();
    if (colCount() == 0) return;
    if (rowCount() == 0) return;

    fieldWidth = rect.width()/allColCount();
    fieldheight = rect.height()/allRowCount();
    //fieldWidth = rect.width()/colCount();
    //fieldheight = rect.height()/rowCount();

    qDebug() << "AllCol:" << allColCount() << "Col:" << colCount();
    qDebug() << "AllRow:" << allRowCount() << "Row:" << rowCount();

    for (int row=0; row < rowCount(); row++) {
        if (isRowHeaderVisible()) {
            //Make a rowHeader
            RptFieldObject *h1 = new RptFieldObject();
            h1->name = QString("rh%1").arg(row);
            h1->fieldType = Text;
            h1->rect.setTop(rect.top() + fieldheight + fieldheight*row);
            h1->rect.setLeft(rect.left());
            h1->rect.setHeight(fieldheight);
            h1->rect.setWidth(fieldWidth);
            h1->value = m_rowHeader[row];
            h1->font.setBold(true);
            h1->setDefaultBackgroundColor(Qt::lightGray); //Set default background color
            h1->aligment = Qt::AlignCenter;
            addField(h1);  //Append field
        }
        // if (isRowTotalVisible()) {
            //Make a rowTotal
            // RptFieldObject *h1 = new RptFieldObject();
            // h1->name = QString("rh%1").arg(row);
            // h1->fieldType = Text;
            // h1->rect.setTop(rect.top() + fieldheight + fieldheight*row);
            // h1->rect.setLeft(rect.left() + fieldWidth*(colCount()+1));
            // h1->rect.setHeight(fieldheight);
            // h1->rect.setWidth(fieldWidth);
            // h1->value = QString("TOTAL %1").arg(row);
            // h1->font.setBold(true);
            // h1->setDefaultBackgroundColor(Qt::lightGray); //Set default background color
            // h1->aligment = Qt::AlignCenter;
            // addField(h1);  //Append field
        // }

        for (int col=0; col < colCount(); col++) {
            if (row == 0 && isColHeaderVisible()) {
                //Make a colHeader
                RptFieldObject *h1 = new RptFieldObject();
                h1->name = QString("ch%1").arg(col);
                h1->fieldType = Text;
                h1->rect.setTop(rect.top());
                h1->rect.setLeft(rect.left() + fieldWidth + fieldWidth*col-1);
                h1->rect.setHeight(fieldheight);
                h1->rect.setWidth(fieldWidth);
                h1->value = m_colHeader[col];
                h1->font.setBold(true);
                h1->setDefaultBackgroundColor(Qt::lightGray); //Set default background color
                h1->aligment = Qt::AlignCenter;
                addField(h1);  //Append field
            }
            // if (row == rowCount()-1 && isColHeaderVisible()) {
                // //Make a colTotal
                // RptFieldObject *h1 = new RptFieldObject();
                // h1->name = QString("rh%1").arg(row);
                // h1->fieldType = Text;
                // h1->rect.setTop(rect.top() + fieldheight + fieldheight*rowCount());
                // h1->rect.setLeft(rect.left() + fieldWidth + fieldWidth*col-1);
                // h1->rect.setHeight(fieldheight);
                // h1->rect.setWidth(fieldWidth);
                // h1->value = QString("TOTAL %1").arg(col);
                // h1->font.setBold(true);
                // h1->setDefaultBackgroundColor(Qt::lightGray); //Set default background color
                // h1->aligment = Qt::AlignCenter;
                // addField(h1);  //Append field
            // }

            //Fill values
            RptFieldObject *h1 = new RptFieldObject();
            h1->name = QString("f%1%2").arg(col).arg(row);
            h1->fieldType = Text;
            h1->rect.setTop(rect.top() + fieldheight + fieldheight*row);
            h1->rect.setLeft(rect.left() + fieldWidth + fieldWidth*col -1);
            h1->rect.setHeight(fieldheight);
            h1->rect.setWidth(fieldWidth);
            h1->value =  getMatrixValue(col,row).toString();
            h1->font.setBold(true);
            h1->setDefaultBackgroundColor(Qt::lightGray); //Set default background color
            h1->aligment = Qt::AlignCenter;
            addField(h1);  //Append field
        }
    }

    // if (isRowTotalVisible()) {
        // RptFieldObject *h1 = new RptFieldObject();
        // h1->name = "cTotal";
        // h1->fieldType = Text;
        // h1->rect.setTop(rect.top());
        // h1->rect.setLeft(rect.left() + fieldWidth*(colCount()+1));
        // h1->rect.setHeight(fieldheight);
        // h1->rect.setWidth(fieldWidth);
        // h1->value = QObject::tr("C-Total");
        // h1->font.setBold(true);
        // h1->setDefaultBackgroundColor(Qt::lightGray); //Set default background color
        // h1->aligment = Qt::AlignCenter;
        // addField(h1);  //Append field
    // }
    // if (isColTotalVisible()) {
        // RptFieldObject *h1 = new RptFieldObject();
        // h1->name = "rTotal";
        // h1->fieldType = Text;
        // h1->rect.setTop(rect.top() + fieldheight + fieldheight*rowCount());
        // h1->rect.setLeft(rect.left());
        // h1->rect.setHeight(fieldheight);
        // h1->rect.setWidth(fieldWidth);
        // h1->value = QObject::tr("R-Total");
        // h1->font.setBold(true);
        // h1->setDefaultBackgroundColor(Qt::lightGray); //Set default background color
        // h1->aligment = Qt::AlignCenter;
        // addField(h1);  //Append field
    // }
}

/*!
 \fn void RptCrossTabObject::addField(RptFieldObject *field)
    Add \a field to current RptCrossTabObject object.

    \sa RptFieldObject
*/
void RptCrossTabObject::addField(RptFieldObject *field) {
    //field->parentBand = this;
    fieldList.append(field);
}

/*!
  Destroys the object, deleting all its child objects.
 */
RptCrossTabObject::~RptCrossTabObject() {
    for (int i=0; i<fieldList.size(); i++)
        if (fieldList.at(i) != 0)
            delete fieldList.at(i);
    fieldList.clear();
}

QDebug operator<<(QDebug dbg, const RptCrossTabObject &obj) {
    dbg << obj.name << "\n";

    if (obj.isColHeaderVisible()) {
        dbg << "\t";
        for(int col=0; col<obj.colCount(); col++) {
            dbg << "|" << obj.getColName(col) << "\t";
        }
        dbg << "\n";
    }

    for(int row=0; row<obj.rowCount(); row++) {
        if (obj.isRowHeaderVisible()) {
            dbg << obj.getRowName(row);
            dbg << "\t";
        }

        for(int col=0; col<obj.colCount(); col++) {
            dbg << "|" << obj.getMatrixValue(col,row).toString() << "\t";
        }
        dbg << "\n";
    }
    return dbg;
}

QDebug operator<<(QDebug dbg, const RptCrossTabObject *obj) {
    return dbg << *obj;
}

//Bellow functions for working with a grid

/*!
 \fn void RptCrossTabObject::addElement(RptTabElement element)
    Add \a element as a RptFieldObject to the matrix.
    This function is only required when RptCrossTabObject is used when
    carry out report export to Excel.
*/
void RptCrossTabObject::addElement(RptTabElement element) {
    int correlation = 50;
    int tmpCol = 0, tmpRow = 0;

    //---
    bool fnd = false;
    for (int col=0; col < colVector.size(); col++) {
        if (element.left <= colVector.at(col)+correlation &&
            element.left >= colVector.at(col)-correlation ) {
            fnd = true;
            element.corrLeft = colVector.at(col);
            tmpCol = col;
            break;
        }
    }
    if (!fnd) {
        colVector.append(element.left);
        tmpCol = appendColumn(QString("%1").arg(element.left));
    }
    //---
    fnd = false;
    for (int row=0; row < rowVector.size(); row++) {
        if (element.top <= rowVector.at(row)+correlation &&
            element.top >= rowVector.at(row)-correlation ) {
            fnd = true;
            //qDebug()<< element.top << rowVector.at(row)+correlation << rowVector.at(row)-correlation;
            element.corrTop = rowVector.at(row);
            tmpRow = row;
            break;
        }
    }
    if (!fnd) {
        rowVector.append(element.top);
        tmpRow = appendRow(QString("%1").arg(element.top));
    }
    initMatrix();
    setMatrixElement(tmpCol,tmpRow,element);
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
void RptCrossTabObject::resortMatrix() {
    //resort rows
    int n = m_rowHeader.size();
    for(int i=0; i<n; ++i)
        for(int j=i+1; j<n; ++j)
            if(QString(m_rowHeader[j]).toInt() < QString(m_rowHeader[i]).toInt()) {
                qSwap(m_rowHeader[i], m_rowHeader[j]);
                qSwap(valuesArray[i], valuesArray[j]);
            }

    //resort columns
    n = m_colHeader.size();
    for(int i=0; i<n; ++i)
        for(int j=i+1; j<n; ++j)
            if(QString(m_colHeader[j]).toInt() < QString(m_colHeader[i]).toInt()) {
                qSwap(m_colHeader[i], m_colHeader[j]);
                for(int row=0; row<m_rowHeader.size(); row++) {
                    qSwap(valuesArray[row][i], valuesArray[row][j]);
                }
            }
}

int RptCrossTabObject::appendRow(QString rowName) {
    m_rowHeader << rowName;
    valuesArray.resize(m_rowHeader.size());  //Set row count
    return m_rowHeader.size()-1;
}

int RptCrossTabObject::appendColumn(QString colName) {
    m_colHeader << colName;

    QMutableVectorIterator<VectorRptTabElement> iRows(valuesArray);
    while (iRows.hasNext())
        (iRows.next()).resize(m_colHeader.size());
    return m_colHeader.size()-1;
}

