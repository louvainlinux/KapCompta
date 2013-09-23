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

#include "ticketpanel.h"
#include "ui_ticket.h"
#include "ui_addticket.h"
#include <QtWidgets/QWidget>
#include <kcaccountfile.h>
#include <kcglobals.h>
#include <kccore.h>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include <QSqlRecord>

class TicketPanelPrivate {
public:
    QWidget widget;
    QDialog dialog;
    QSqlTableModel *personModel;
    QSqlTableModel *eventsModel;
    QSqlRelationalTableModel *model;

    TicketPanelPrivate()
    {}
};

TicketPanel::TicketPanel(KCAccountFile *account, QWidget *parent) :
    KCPanel(account, parent),
    ui(new Ui::Ticket),
    addT(new Ui::AddTicket),
    d(new TicketPanelPrivate)
{
    ui->setupUi(&d->widget);
    addT->setupUi(&d->dialog);
    addT->person->setEditable(true);
    addT->event->setEditable(true);
    // prepare the dialog
    d->dialog.hide();
    d->dialog.setModal(true);
    d->dialog.setWindowTitle(tr("Add a ticket"));
    connect(ui->add, SIGNAL(clicked()), this, SLOT(showDialog()));
    connect(addT->cancel, SIGNAL(clicked()), &d->dialog, SLOT(hide()));
    connect(addT->ok, SIGNAL(clicked()), this, SLOT(addTicket()));
    connect(addT->person, SIGNAL(currentIndexChanged(int)), this, SLOT(checkAddTicket()));
    connect(addT->event, SIGNAL(currentIndexChanged(int)), this, SLOT(checkAddTicket()));
    connect(ui->remove, SIGNAL(clicked()), SLOT(removeTicket()));
}

TicketPanel::~TicketPanel()
{
    delete ui;
    delete addT;
    delete d;
}

const QString TicketPanel::panelName()
{
    return tr("Ticket management");
}

QWidget* TicketPanel::panel()
{
    return &d->widget;
}

const QString TicketPanel::iconName()
{
    return QString(":/icon/ticket");
}

void TicketPanel::allPanelsCreated()
{
    d->personModel = (QSqlTableModel*)KCPanel::account->model(MODEL_PERSON);
    addT->person->setModel(d->personModel);
    addT->person->setModelColumn(1);
    d->eventsModel = (QSqlTableModel*)KCPanel::account->model(MODEL_EVENTS);
    addT->event->setModel(d->eventsModel);
    addT->event->setModelColumn(1);
}

void TicketPanel::selected()
{
    d->personModel->select();
    d->eventsModel->select();
}

void TicketPanel::unselected()
{
    if (!d->personModel->submitAll())
        KCCore::instance()->warning(
                    tr("Failed to submit changes to the people,\nreason: %1")
                    .arg(d->personModel->lastError().text()));
    if (!d->eventsModel->submitAll())
        KCCore::instance()->warning(
                    tr("Failed to submit changes to the events,\nreason: %1")
                    .arg(d->personModel->lastError().text()));
}

void TicketPanel::removeTicket()
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
    d->model->select();
}

void TicketPanel::showDialog()
{
    // disable the ok button of the dialog for later uses
    addT->ok->setDisabled(true);
    addT->amount->setValue(0);
    addT->event->setCurrentIndex(-1);
    addT->person->setCurrentIndex(-1);
    addT->isIncome->setEnabled(false);
    addT->misc->clear();
    addT->date->setSelectedDate(QDate::currentDate());
    d->dialog.show();
}

void TicketPanel::checkAddTicket()
{
    if (addT->person->currentIndex() != -1 && addT->event->currentIndex() != -1)
        addT->ok->setDisabled(false);
    else addT->ok->setDisabled(true);
}

void TicketPanel::addTicket()
{
    // Generate a new row record
    QSqlRecord ins = d->model->record();
    // fill in our new data
    ins.setValue(ins.indexOf("amount"), addT->amount->value());
    ins.setValue(ins.indexOf("misc"), addT->misc->toPlainText());
    ins.setValue(ins.indexOf("date"), addT->date->selectedDate());
    ins.setValue(ins.indexOf("isExpense"), !addT->isIncome->isEnabled());
    ins.setValue(ins.indexOf("person_id"),
                 d->personModel->record(addT->person->currentIndex()).value("id"));
    ins.setValue(ins.indexOf("event_id"),
                 d->eventsModel->record(addT->person->currentIndex()).value("id"));
    // insert it at the bottom of the table
    d->model->insertRecord(-1, ins);
    if (!d->model->submit()) {
        KCCore::instance()->warning(tr("Failed to insert a new ticket !\nreason: %1")
                                    .arg(d->model->lastError().text()));
    }
    d->model->select();
    d->dialog.hide();
}
