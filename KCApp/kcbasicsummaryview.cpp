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
#include <QListView>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>
#include <QComboBox>
#include <QStringList>
#include <QFormLayout>
#include <QCheckBox>
#include <kccore.h>

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
    individual = new QCheckBox(tr("Show individual balance"), optionsPanel);
    individual->setChecked(false);
    optionsPanel->layout()->addWidget(generalBtn);
    optionsPanel->layout()->addWidget(expenseList);
    optionsPanel->layout()->addWidget(individual);
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
    connect(individual, SIGNAL(clicked()), this, SLOT(refreshView()));
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

void KCBasicSummaryView::setAccountProperties(const KCAccountProperties& properties)
{
    this->properties = properties;
    expenseModel->select();
    backToGeneralView();
}

const QString KCBasicSummaryView::summaryName() const
{
    return tr("Default Summary view");
}

QWidget* KCBasicSummaryView::displayOptions()
{
    return optionsPanel;
}

bool KCBasicSummaryView::optionsUnder() const
{
    return false;
}

void KCBasicSummaryView::setConnectionName(const QString& c)
{
    connection = QString(c);
    expenseModel = new QSqlTableModel(optionsPanel, QSqlDatabase::database(connection));
    expenseModel->setTable("expenses");
    expenseModel->setFilter("hidden = 0");
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

void KCBasicSummaryView::makeExpensePage(const QSqlRecord& r)
{
    if (individual->checkState() == Qt::Checked) {
        makeIndividualPage(false, r);
        return;
    }
    QString name = r.value("name").toString();
    QString description = r.value("description").toString();
    description.replace("\n","<br />");
    view->insertHtml("<center><h1><b>" + name + "</b></h1></center>"
                     "<p><i>" + description + "</i></p>"
                     + KCCore::hr());

    QString str = "<table width='100%'><tr><td><b><h3>" + tr("Amount") + "</h3></b></td>"
            "<td><b><h3>" + tr("Date") + "</h3></b></td>"
            "<td><b><h3>" + tr("Notes") + "</h3></b></td></tr>";
    int id = r.value("id").toInt();
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
    view->append(KCCore::hr());
    view->append("<p><b><h3>" + tr("Total:") + " "
                 + QString::number(KCDataBaseHelper::sumExpenses(connection, id))
                 + " " + tr("eur."));
}

void KCBasicSummaryView::makeGeneralPage()
{
    if (individual->checkState() == Qt::Checked) {
        makeIndividualPage(true, QSqlRecord());
        return;
    }
    view->insertHtml("<h1><b><center>" + properties.name + "</center></b></h1>");
    view->append(KCCore::hr());

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
            + KCCore::hr();
    str += "<p>" + tr("Initial Account Balance: ") + QString::number(properties.balance) + " " + tr("eur.")
            + "<br /><b>" + tr("Current Account Balance: ")
            + QString::number(properties.balance + KCDataBaseHelper::sumAllExpenses(connection)) + " " + tr("eur.") +
            "</b></p>";
    view->append(str);
}

void KCBasicSummaryView::makeIndividualPage(bool global, const QSqlRecord& r)
{
    QString str = "<h2><b><center>Individual balance [";
    if (global) str += properties.name;
    else str += r.value("name").toString();
    str +=  "]</center></b></h2>" + KCCore::hr();
    str +=  "<table width ='100%'><tr><td><center><b><h2>"
            + tr("Person name") + "</h2></b></center></td><td><b><center><h2>"
            + tr("Balance") + "</h2></center></b></td></tr></table>"
            + KCCore::hr();
    QSqlQuery people(QSqlDatabase::database(connection));
    people.exec("SELECT id, name FROM person ORDER BY name ASC");
    str += "<table width ='100%'>";
    while (people.next()) {
        QSqlRecord entry = people.record();
        str += "<tr><td><h3><center>" + entry.value("name").toString() + "</h3></center></td><td><center><h3><b>";
        if (global) str += QString::number(
                    KCDataBaseHelper::overalBalance(connection, entry.value("id").toInt())
                    );
        else str += QString::number(
                    KCDataBaseHelper::overalBalanceForExpense(connection,
                                                    entry.value("id").toInt(),
                                                    r.value("id").toInt())
                    );
        str += " " + tr("eur.") + "</b></h3></center></td></tr>";
    }
    str += "</table>";
    view->setHtml(str);
}

void KCBasicSummaryView::refreshView()
{
    view->clear();
    // Switch to enable support for more type of view
    switch(selectedView) {
    case -1:
        makeGeneralPage();
        break;
    default:
        QSqlRecord r = expenseModel->record(selectedView);
        makeExpensePage(r);
        break;
    }
}
