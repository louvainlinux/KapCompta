/*
 * Copyright (c) 2012-2013, Olivier Tilmans
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **/

#include "peoplepanel.h"
#include "ui_people.h"
#include "ui_addperson.h"
#include <QtWidgets/QWidget>
#include "QDialog"

class PeoplePanelPrivate {
public:
    QWidget widget;
    QDialog dialog;

    PeoplePanelPrivate()
    {}
};

PeoplePanel::PeoplePanel(QWidget *parent) :
    KCPanel(parent),
    ui(new Ui::People),
    addP(new Ui::AddPerson),
    d(new PeoplePanelPrivate)
{
    ui->setupUi(&d->widget);
    addP->setupUi(&d->dialog);
    d->dialog.hide();
    d->dialog.setModal(true);
    d->dialog.setWindowTitle(tr("Add a person"));
    connect(ui->addPerson, SIGNAL(clicked()), &d->dialog, SLOT(show()));
    connect(addP->cancel, SIGNAL(clicked()), &d->dialog, SLOT(hide()));
}

PeoplePanel::~PeoplePanel()
{
    delete ui;
    delete addP;
    delete d;
}

const QString PeoplePanel::panelName()
{
    return "People management";
}

QWidget* PeoplePanel::panel()
{
    return &d->widget;
}

const QString PeoplePanel::iconName()
{
    return QString(":/icon/people");
}
