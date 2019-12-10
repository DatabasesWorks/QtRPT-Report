/*
Name: QtRpt
Version: 2.0.3
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

#include "ScriptEditor.h"
#include "ui_ScriptEditor.h"

ScriptEditor::ScriptEditor(QSharedPointer<QDomDocument> xmlDoc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScriptEditor)
{
    ui->setupUi(this);
    m_xmlDoc = xmlDoc;
}

void ScriptEditor::showScript()
{
    QDomElement documentElement = m_xmlDoc->documentElement();
    QDomNodeList elements = documentElement.elementsByTagName("Script");

    if (elements.size() > 0) {
        QDomNode cdataNode = elements.at(0).childNodes().at(0);
        if (cdataNode.isCDATASection()) {
            QString script = cdataNode.toCDATASection().data();
            ui->editor->setPlainText(script);
        }
    }
}

QDomElement ScriptEditor::saveParamToXML(QSharedPointer<QDomDocument> xmlDoc, QDomElement element)
{
    QDomElement elem;
//    if (ui->rbSql->isChecked()) {
        elem = xmlDoc->createElement("Script");
        element.appendChild(elem);
//        elem.setAttribute("name","DB1");
//        elem.setAttribute("type","SQL");
//        elem.setAttribute("dbType",ui->cmbType->currentText());
//        elem.setAttribute("dbName",ui->edtDBName->text());
//        elem.setAttribute("dbHost",ui->edtHost->text());
//        elem.setAttribute("dbUser",ui->edtUserName->text());
//        elem.setAttribute("dbPassword",ui->edtPassword->text());
//        elem.setAttribute("dbCoding",ui->edtConnectionCoding->text());
//        elem.setAttribute("charsetCoding",ui->edtCharsetCoding->text());
//        elem.setAttribute("dbConnectionName",ui->edtConName->text());
//        elem.setAttribute("dbPort",ui->edtPort->text());
//        QDomText t = xmlDoc->createTextNode(ui->sqlEditor->toPlainText());
//        elem.appendChild(t);
//        currentScene->save(xmlDoc, elem);
//    }
//    if (ui->rbXml->isChecked()) {

//    }

//        QDomElement script = document.createElement("script");
//        script.setAttribute("type", "text/javascript");
//        body.appendChild(script);

        QString js = "if(x < 0){\n/*do something*/\n}";
        QDomCDATASection data = xmlDoc->createCDATASection(js);
        elem.appendChild(data);
    return elem;
}

ScriptEditor::~ScriptEditor()
{
    delete ui;
}
