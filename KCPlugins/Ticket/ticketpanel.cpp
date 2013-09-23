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
#include <QSqlTableModel>
#include <QSqlRecord>
#include <kcdatabase.h>
#include <QSqlRelation>
#include <QSqlRelationalDelegate>
#include <kcdatedelegate.h>
#include <kcspinnerdelegate.h>
#include <QDebug>
#include <kcbooleandelegate.h>
#include <kccomboboxdelegate.h>

class TicketPanelPrivate {
public:
    QWidget widget;
    QDialog dialog;
    QSqlTableModel *personModel;
    QSqlTableModel *eventsModel;
    QSqlTableModel *model;
    KCDatabase *db;
    QSqlRecord record;

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
    // Configure our main data model
    d->db = new KCDatabase(KCPanel::account);
    d->model = new QSqlTableModel(this, d->db->db());
    KCPanel::account->registerModel(d->model, MODEL_TICKET);
    d->model->setTable("ticket");
    d->model->setEditStrategy(QSqlTableModel::OnFieldChange);
    d->model->select();
    QSqlRecord r = d->model->record();
    d->record = r;
    // Setup view delegates
    ui->tableView->setItemDelegateForColumn(r.indexOf("date"), new KCDateDelegate(ui->tableView));
    ui->tableView->setItemDelegateForColumn(r.indexOf("amount"),
                                            new KCSpinnerDelegate("", tr("€"), ui->tableView));
    ui->tableView->setItemDelegateForColumn(r.indexOf("isExpense"),
                                            new KCBooleanDelegate(tr("Expense"), tr("Income"), ui->tableView));
    // bind the model to the table view
    ui->tableView->setModel(d->model);
    // We don't want to display the column id
    ui->tableView->hideColumn(r.indexOf("id"));
    // And we authorize the user to edit the edit as he wishes
    ui->tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->tableView->setSortingEnabled(true);
    // setup column stretching
    QHeaderView *header = ui->tableView->horizontalHeader();
    header->setStretchLastSection(true);
    header->setSectionResizeMode(r.indexOf("amount"), QHeaderView::ResizeToContents);
    header->setSectionResizeMode(r.indexOf("isExpense"), QHeaderView::ResizeToContents);
    header->setSectionResizeMode(r.indexOf("person_id"), QHeaderView::ResizeToContents);
    header->setSectionResizeMode(r.indexOf("event_id"), QHeaderView::ResizeToContents);
    header->setSectionResizeMode(r.indexOf("date"), QHeaderView::Fixed);
    header->setDefaultSectionSize(100);
    header->setSectionsMovable(true);
    // Pretty column names
    d->model->setHeaderData(r.indexOf("amount"), Qt::Horizontal, tr("Amount"));
    d->model->setHeaderData(r.indexOf("misc"), Qt::Horizontal, tr("Description"));
    d->model->setHeaderData(r.indexOf("isExpense"), Qt::Horizontal, tr("Expense"));
    d->model->setHeaderData(r.indexOf("person_id"), Qt::Horizontal, tr("Person"));
    d->model->setHeaderData(r.indexOf("event_id"), Qt::Horizontal, tr("Event"));
    d->model->setHeaderData(r.indexOf("date"), Qt::Horizontal, tr("Date"));
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
    QSqlRecord p = d->personModel->record();
    addT->person->setModel(d->personModel);
    addT->person->setModelColumn(p.indexOf("name"));
    d->eventsModel = (QSqlTableModel*)KCPanel::account->model(MODEL_EVENTS);
    QSqlRecord e = d->eventsModel->record();
    addT->event->setModel(d->eventsModel);
    addT->event->setModelColumn(e.indexOf("name"));
    ui->tableView->setItemDelegateForColumn(d->record.indexOf("person_id"),
                                            new KCComboBoxDelegate(d->personModel,
                                                                   p.indexOf("name"),
                                                                   p.indexOf("id"),
                                                                   ui->tableView));
    ui->tableView->setItemDelegateForColumn(d->record.indexOf("event_id"),
                                            new KCComboBoxDelegate(d->eventsModel,
                                                                   e.indexOf("name"),
                                                                   e.indexOf("id"),
                                                                   ui->tableView));
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
    addT->isIncome->setChecked(false);
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
    d->personModel->submit();
    d->eventsModel->submit();
    // Generate a new row record
    QSqlRecord ins = QSqlRecord(d->record);
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
    if (!d->model->insertRecord(-1, ins) || !d->model->submit()) {
        KCCore::instance()->warning(tr("Failed to insert a new ticket !\nreason: %1")
                                    .arg(d->model->lastError().text()));
    }
    d->model->select();
    d->dialog.hide();
}
