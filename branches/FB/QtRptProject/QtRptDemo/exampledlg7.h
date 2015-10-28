/*
Name: QtRpt
Version: 1.5.4
Programmer: Aleksey Osipov
E-mail: aliks-os@ukr.net
2012-2015

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

#ifndef EXAMPLEDLG7_H
#define EXAMPLEDLG7_H

#include <QDialog>
#include "qtrpt.h"

namespace Ui {
class ExampleDlg7;
}

class ExampleDlg7 : public QDialog
{
    Q_OBJECT

public:
    explicit ExampleDlg7(QWidget *parent = 0);
    ~ExampleDlg7();

private:
    Ui::ExampleDlg7 *ui;
    QtRPT *report;

private slots:
    void print();
    void setValue(const int recNo, const QString paramName, QVariant &paramValue, const int reportPage);
    void setValueDiagram(Chart &chart);
};

#endif // EXAMPLEDLG7_H
