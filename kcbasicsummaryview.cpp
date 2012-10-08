/*
 * Copyright (c) 2012, Olivier Tilmans
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

#include "kcbasicsummaryview.h"
#include <QPrinter>
#include <QPainter>
#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include "kcdatabasehelper.h"
#include <QRect>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QListView>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>
#include <QComboBox>
#include <QStringList>
#include <QFormLayout>

#define _MARGIN 10

KCBasicSummaryView::KCBasicSummaryView(QObject *parent) :
    QObject(parent)
{
    optionsPanel = new QWidget();
    optionsPanel->setLayout(new QVBoxLayout());
    minHeight = 450;
    minWidth = 175;
    expenseList = new QListView(optionsPanel);
    generalBtn = new QPushButton(tr("General summary view"), optionsPanel);
    orderBy = new QComboBox(optionsPanel);
    orderBy->addItems(QStringList() << tr("Date") << tr("Amount"));
    order = "date";
    QFormLayout *orderByL = new QFormLayout();
    orderByL->addRow(tr("Order By:"), orderBy);
    orderBy->setEnabled(false);
    optionsPanel->layout()->addWidget(generalBtn);
    optionsPanel->layout()->addWidget(expenseList);
    qobject_cast<QVBoxLayout*>(optionsPanel->layout())->addLayout(orderByL);
    generalBtn->setEnabled(false);
    expenseList->setAlternatingRowColors(true);
    expenseList->setEditTriggers(QListView::NoEditTriggers);
    selectedView = -1;
    view = new QTextEdit();
    view->setReadOnly(true);
    view->setMinimumSize(500,350);

    connect(generalBtn, SIGNAL(clicked()), this, SLOT(backToGeneralView()));
    connect(orderBy, SIGNAL(currentIndexChanged(QString)), this, SLOT(orderChanged(QString)));
}

void KCBasicSummaryView::orderChanged(QString s)
{
    order = s.toLower();
    this->refreshView();
}

QWidget* KCBasicSummaryView::summaryView()
{
    return view;
}

void KCBasicSummaryView::backToGeneralView()
{
    selectedView = -1;
    refreshView();
    expenseList->reset();
    generalBtn->setEnabled(false);
    orderBy->setEnabled(false);
}

void KCBasicSummaryView::selectPage(QModelIndex idx)
{
    selectedView = idx.row();
    refreshView();
    generalBtn->setEnabled(true);
    orderBy->setEnabled(true);
}

void KCBasicSummaryView::setAccountProperties(KCAccountProperties properties)
{
    this->properties = properties;
    expenseModel->select();
    backToGeneralView();
}

const QString& KCBasicSummaryView::summaryName()
{
    static QString s = tr("Default Summary view");
    return s;
}

QWidget* KCBasicSummaryView::displayOptions()
{
    return optionsPanel;
}

bool KCBasicSummaryView::optionsUnder()
{
    return false;
}

void KCBasicSummaryView::setConnectionName(const QString& c)
{
    connection = QString(c);
    expenseModel = new QSqlTableModel(optionsPanel, QSqlDatabase::database(connection));
    expenseModel->setTable("expenses");
    expenseModel->select();
    expenseList->setModel(expenseModel);
    expenseList->setModelColumn(expenseModel->fieldIndex("name"));
    connect(expenseList->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(selectPage(QModelIndex)));
    refreshView();
}

void KCBasicSummaryView::printSummary(QPrinter *printer)
{
    view->print(printer);
}

void KCBasicSummaryView::makeExpensePage(QSqlRecord *r)
{
    QString name = r->value("name").toString();
    QString description = r->value("description").toString();
    description.replace("\n","<br />");
    view->insertHtml("<center><h1><b>" + name + "</b></h1></center>"
                     "<p><i>" + description + "</i></p>"
                     "<table width='100%'><tr><td><hr /></td></tr></table>");
    // <hr /> tag bugs !

    QString str = "<table width='100%'><tr><td><b><h3>" + tr("Amount") + "</h3></b></td>"
            "<td><b><h3>" + tr("Date") + "</h3></b></td>"
            "<td><b><h3>" + tr("Notes") + "</h3></b></td></tr>";
    int id = r->value("id").toInt();
    QSqlQuery query(QSqlDatabase::database(connection));
    query.exec("SELECT amount, notes, date FROM tickets WHERE expenseid = " + QString::number(id)
               + " ORDER BY " + order +" ASC");
    while (query.next()) {
        QSqlRecord entry = query.record();
        QString notes = entry.value("notes").toString();
        notes.replace("\n","<br />");
        QString date = entry.value("date").toString();
        QString amount = entry.value("amount").toString();

        str += "<tr>"
                "<td><b>" + amount + " " + tr("eur.") + "</b></td>"
                "<td>" + date + "</td>"
                "<td><i>" + notes + "</i></td>"
                "</tr>";
    }
    str += "</table>";
    view->append(str);
    view->append("<table width=\"100%\"><tr><td><hr /></td></tr></table>");
    view->append("<p><b><h3>" + tr("Total:") + " "
                 + QString::number(KCDataBaseHelper::sumExpenses(connection, id))
                 + " " + tr("eur."));
}

void KCBasicSummaryView::makeGeneralPage()
{
    view->insertHtml("<h1><b><center>" + properties.name + "</center></b></h1>");
    view->append("<table width='100%'><tr><td><hr /></td></tr></table>");

    QString str = "<table width ='100%'><tr><td><b><h4>"
            + tr("Item of expenses name") + "</h4></b></td>"
            "<td><b><h4>" + tr("Income") + "</h4></b></td>"
            "<td><b><h4>" + tr("Expense") + "</h4></b></td>"
            "<td><b><h4>" + tr("Balance") + "</h4></b></td></tr>";

    QSqlQuery query(QSqlDatabase::database(connection));
    query.exec("SELECT name, id FROM expenses ORDER BY name ASC");
    while (query.next()) {
        QSqlRecord entry = query.record();
        int id = entry.value("id").toInt();
        QString name = entry.value("name").toString();

        str += "<tr>"
                "<td>" + name + "</td>"
                "<td><i>" + QString::number(KCDataBaseHelper::sumPositiveExpenses(connection, id))
                + " " + tr("eur.") + "</i></td>"
                "<td><i>" + QString::number(KCDataBaseHelper::sumNegativeExpenses(connection, id))
                + " " + tr("eur.") + "</i></td>"
                "<td><b>" + QString::number(KCDataBaseHelper::sumExpenses(connection, id))
                + " " + tr("eur.") + "</b></td>"
                "</tr>";
    }

    str += "</table>"
            "<table width=\"100%\"><tr><td><hr /></td></tr></table>"
            "<p>" + tr("Initial Account Balance: ") + QString::number(properties.balance) + " " + tr("eur.")
            + "<br /><b>" + tr("Current Account Balance: ")
            + QString::number(properties.balance + KCDataBaseHelper::sumAllExpenses(connection)) + " " + tr("eur.") +
            "</b></p>";
    view->append(str);
}

void KCBasicSummaryView::refreshView()
{
    view->clear();
    if (selectedView == -1) {
        makeGeneralPage();
    } else {
        QSqlRecord r = expenseModel->record(selectedView);
        makeExpensePage(&r);
    }
}
