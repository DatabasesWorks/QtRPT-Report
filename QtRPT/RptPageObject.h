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

#include <QObject>
#include <qtrptnamespace.h>
#include <RptBandObject.h>
#include <RptFieldObject.h>
#include <RptSql.h>

using namespace QtRptName;

class QtRPT;
class RptBandObject;
class RptFieldObject;

class RptPageObject : public QObject
{
    Q_OBJECT

    friend class QtRPT;

public:
    RptPageObject(QtRPT *qtrpt = nullptr);
    ~RptPageObject();
    int pageNo;
    int orientation;
    int ph;
    int pw;
    int ml;
    int mr;
    int mt;
    int mb;
    bool border;
    int borderWidth;
    QColor borderColor;
    QString borderStyle;
    bool watermark;
    float watermarkOpacity;
    QPixmap watermarkPixmap;
    RptSql *rtpSql;
    RptSqlConnection sqlConnection;

    void addBand(RptBandObject *band);
    RptBandObject *getBand(BandType type, int No);
    int bandsCountByType(BandType type);
    RptFieldObject *findFieldObjectByName(QString name);
    void initCrossTabProcessedRows();
    int crossTabParts();

    Q_INVOKABLE void setVisible(bool value);
    bool isVisible();

    quint16 totalPages();
    void setTotalPages(quint16 value);
    QList<RptFieldObject*> crossTabs();
    QList<RptBandObject*> bandList;
    int recordCount;
    RptPageObject *clone();

private:
	QtRPT *m_qtrpt;
    bool m_visible;
    quint16 m_totalPages;
    void setProperty(QtRPT *qtrpt, QDomElement docElem);

};

//Q_DECLARE_METATYPE(RptPageObject)
QDebug operator<<(QDebug dbg, const RptPageObject &obj);
