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
#include <QDialog>
#include <QSqlTableModel>
#include <kcdatabase.h>
#include <QSqlRecord>
#include <kccore.h>
#include <QSqlError>
#include <kcaccountfile.h>
#include <kcglobals.h>

class PeoplePanelPrivate {
public:
    QWidget widget;
    QDialog dialog;
    QSqlTableModel *model;
    KCDatabase* db;

    PeoplePanelPrivate()
    {}

    void setupModel(KCAccountFile *f)
    {
        db = new KCDatabase(f);
        model = new QSqlTableModel(f, db->db());
        f->registerModel(model, MODEL_PERSON);
    }
};

PeoplePanel::PeoplePanel(KCAccountFile *account, QWidget *parent) :
    KCPanel(account, parent),
    ui(new Ui::People),
    addP(new Ui::AddPerson),
    d(new PeoplePanelPrivate)
{
    ui->setupUi(&d->widget);
    addP->setupUi(&d->dialog);
    d->setupModel(KCPanel::account);
    d->model->setTable("person");
    // We will submit changes when the user exit the application or navigates to another panel
    d->model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    d->model->select();
    // Declare better names for our columns
    d->model->setHeaderData(d->model->record().indexOf("name"),
                         Qt::Horizontal,
                         tr("Name"));
    d->model->setHeaderData(d->model->record().indexOf("misc"),
                         Qt::Horizontal,
                         tr("Description"));
    // bind the model to the table view
    ui->tableView->setModel(d->model);
    // We don't want to display the column id
    ui->tableView->hideColumn(d->model->record().indexOf("id"));
    // And we authorize the user to edit the edit as he wishes
    ui->tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->tableView->setSortingEnabled(true);
    // We want the columns to take all available space, but want to favor the name column
    QHeaderView *header = ui->tableView->horizontalHeader();
    header->setStretchLastSection(true);
    header->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    header->setSectionsMovable(true);
    // prepare the dialog
    d->dialog.hide();
    d->dialog.setModal(true);
    d->dialog.setWindowTitle(tr("Add a person"));
    // By default the ok button in the dialog is disabled, and we'll listen for changes in the
    // dialog to enable it on the fly
    addP->ok->setDisabled(true);
    connect(ui->addPerson, SIGNAL(clicked()), &d->dialog, SLOT(show()));
    connect(addP->cancel, SIGNAL(clicked()), &d->dialog, SLOT(hide()));
    connect(addP->ok, SIGNAL(clicked()), this, SLOT(addPerson()));
    connect(addP->name, SIGNAL(textChanged(QString)), this, SLOT(checkAddPerson()));
    connect(ui->removePerson, SIGNAL(clicked()), SLOT(removePeople()));
}

PeoplePanel::~PeoplePanel()
{
    delete ui;
    delete addP;
    delete d;
}

const QString PeoplePanel::panelName()
{
    return tr("People management");
}

QWidget* PeoplePanel::panel()
{
    return &d->widget;
}

const QString PeoplePanel::iconName()
{
    return QString(":/icon/people");
}

void PeoplePanel::checkAddPerson()
{
    if (addP->name->text().length() > 0) addP->ok->setDisabled(false);
    else addP->ok->setDisabled(true);
}

void PeoplePanel::addPerson()
{
    // Generate a new row record
    QSqlRecord ins = d->model->record();
    // fill in our new data
    ins.setValue(ins.indexOf("name"), addP->name->text());
    ins.setValue(ins.indexOf("misc"), addP->misc->toPlainText());
    // insert it at the bottom of the table
    d->model->insertRecord(-1, ins);
    if (!d->model->submit()) {
        KCCore::instance()->warning(tr("Failed to insert a new person !\nreason: %1")
                                    .arg(d->model->lastError().text()));
    }
    d->dialog.hide();
    // disable the ok button of the dialog for later uses
    addP->ok->setDisabled(true);
    addP->name->clear();
    addP->misc->clear();
}

void PeoplePanel::selected()
{
    d->model->select();
}

void PeoplePanel::unselected()
{
    if (!d->model->submitAll())
        KCCore::instance()->warning(
                    tr("Failed to submit changes to the people,\nreason: %1")
                    .arg(d->model->lastError().text()));
}

void PeoplePanel::removePeople()
{
    // Freeze view update when the data change
    ui->tableView->setUpdatesEnabled(false);
    QModelIndexList indexes = ui->tableView->selectionModel()->selectedIndexes();
    qSort(indexes.begin(), indexes.end());
    // remove all rows one by one
    for(int i = indexes.count() - 1; i > -1; --i) {
        d->model->removeRow(indexes.at(i).row());
    }
    // update the database/view
    ui->tableView->setUpdatesEnabled(true);
    d->model->submitAll();
}
