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
#include <QtWidgets/QWidget>
#include <kcaccountfile.h>
#include <kcglobals.h>
#include <kccore.h>
#include <QSqlError>
#include <QSqlTableModel>

class TicketPanelPrivate {
public:
    QWidget widget;
    QSqlTableModel *personModel;
    QSqlTableModel *eventsModel;

    TicketPanelPrivate()
    {}
};

TicketPanel::TicketPanel(KCAccountFile *account, QWidget *parent) :
    KCPanel(account, parent),
    ui(new Ui::People),
    d(new TicketPanelPrivate)
{
    ui->setupUi(&d->widget);
    ui->person->setEditable(true);
    ui->event->setEditable(true);
}

TicketPanel::~TicketPanel()
{
    delete ui;
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
    ui->person->setModel(d->personModel);
    ui->person->setModelColumn(1);
    d->eventsModel = (QSqlTableModel*)KCPanel::account->model(MODEL_EVENTS);
    ui->event->setModel(d->eventsModel);
    ui->event->setModelColumn(1);
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
