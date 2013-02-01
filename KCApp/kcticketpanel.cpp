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

#include "kcticketpanel.h"
#include <QHBoxLayout>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QListView>
#include <QPushButton>
#include <QSqlRelationalTableModel>
#include <QSqlTableModel>
#include <QSqlRelation>
#include <QSqlRelationalDelegate>
#include <QDataWidgetMapper>
#include <QSqlRecord>
#include <QCompleter>
#include <QComboBox>
#include <QStringListModel>
#include <QDoubleValidator>
#include <QSqlQuery>
#include <QDate>
#include <QModelIndex>
#include <kcdateedit.h>

KCTicketPanel::KCTicketPanel(QWidget *parent) :
    QWidget(parent)
{
}

KCTicketPanel::~KCTicketPanel() {
}

QWidget* KCTicketPanel::panel()
{
    return this;
}

const QString KCTicketPanel::title() const
{
    return tr("Receipts management");
}

const QString KCTicketPanel::iconPath() const
{
    return QString(":/icons/ticket");
}

void KCTicketPanel::unselectPanel()
{
    saveAll();
}

void KCTicketPanel::saveAll()
{
    QModelIndex idx = listView->currentIndex();
    model->submitAll();
    listView->setCurrentIndex(idx);
    mapper->setCurrentModelIndex(idx);
}

void KCTicketPanel::buildGUI(const QString &connection)
{
    connectionName = QString(connection);

    model = new QSqlRelationalTableModel(this,QSqlDatabase::database(connectionName));
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("tickets");
    personid = model->fieldIndex("personid");
    model->setRelation(personid,
                       QSqlRelation("person","id","name"));
    expenseid = model->fieldIndex("expenseid");
    model->setRelation(expenseid,
                       QSqlRelation("expenses","id","name"));
    model->select();
    personRel = model->relationModel(personid);
    expenseRel = model->relationModel(expenseid);

    QLineEdit *amount = new QLineEdit(this);
    amount->setValidator(new QDoubleValidator(this));
    QPlainTextEdit *notes = new QPlainTextEdit(this);
    QPushButton *add = new QPushButton(QString("+"),this);
    QPushButton *remove = new QPushButton(QString("-"), this);

    KCDateEdit *date = new KCDateEdit(QDate::currentDate(), this);
    date->setCalendarPopup(true);
    date->setDisplayFormat(QString("dd/MM/yyyy"));

    QComboBox *filter = new QComboBox(this);
    QStringListModel *filterModel = new QStringListModel(this);
    filterModel->setStringList(QStringList() << tr("Date") << tr("Amount"));
    filter->setModel(filterModel);
    model->sort(model->fieldIndex("date"),Qt::AscendingOrder);

    QComboBox *person = new QComboBox(this);
    person->setModel(personRel);
    person->setModelColumn(personRel->fieldIndex("name"));
    person->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    QComboBox *expense = new QComboBox(this);
    expense->setModel(expenseRel);
    expense->setModelColumn(expenseRel->fieldIndex("name"));
    expense->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    filter->setEditable(false);
    person->setEditable(false);
    expense->setEditable(false);

    listView = new QListView();
    listView->setAlternatingRowColors(true);
    listView->setSelectionMode(QAbstractItemView::SingleSelection);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listView->setModel(model);
    listView->setModelColumn(model->fieldIndex("date"));
    listView->setItemDelegate(new QSqlRelationalDelegate(this));

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(model);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(amount, model->fieldIndex("amount"));
    mapper->addMapping(date, model->fieldIndex("date"));
    mapper->addMapping(notes, model->fieldIndex("notes"));
    mapper->addMapping(person, personid);
    mapper->addMapping(expense,expenseid);
    mapper->setCurrentIndex(-1);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();
    QFormLayout *layout = new QFormLayout();
    layout->addRow(tr("Date:"), date);
    layout->addRow(tr("Amount:"), amount);
    layout->addRow(tr("Person:"), person);
    layout->addRow(tr("Expense Item:"), expense);
    layout->addRow(tr("Notes:"), notes);
    layout->setSizeConstraint(QLayout::SetNoConstraint);
    layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    buttonLayout->addWidget(add);
    buttonLayout->addWidget(remove);
    QVBoxLayout *leftLayout = new QVBoxLayout();
    QFormLayout *choserLayout = new QFormLayout();
    choserLayout->addRow(tr("List by:"),filter);
    choserLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    leftLayout->addLayout(choserLayout);
    leftLayout->addLayout(buttonLayout);
    leftLayout->addWidget(listView);
    hLayout->addLayout(leftLayout);
    hLayout->addLayout(layout,1);
    this->setLayout(hLayout);

    connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            listView, SLOT(dataChanged(QModelIndex,QModelIndex)));
    connect(listView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,SLOT(setCurrentModelIndex()));
    connect(add, SIGNAL(clicked()), this, SLOT(addEntry()));
    connect(remove, SIGNAL(clicked()), this, SLOT(removeEntry()));
    connect(filter, SIGNAL(currentIndexChanged(QString)), this,SLOT(filterChanged(QString)));

    this->setTabOrder(filter,add);
    this->setTabOrder(add,remove);
    this->setTabOrder(remove,listView);
    this->setTabOrder(listView,amount);
    this->setTabOrder(amount,date);
    this->setTabOrder(date,person);
    this->setTabOrder(person,expense);
    this->setTabOrder(expense,notes);
}

void KCTicketPanel::filterChanged(QString s)
{
    saveAll();
    int idx = 0;
    if (!s.compare(tr("Amount"))) {
        idx = model->fieldIndex("amount");
    } else if (!s.compare(tr("Date"))) {
        idx = model->fieldIndex("date");
    }
    listView->setModelColumn(idx);
    model->sort(idx,Qt::AscendingOrder);
}

void KCTicketPanel::addEntry()
{
    QSqlRecord record = model->record();
    record.setValue(model->fieldIndex("date"),
                    QVariant(QDate::currentDate().toString("dd/MM/yyyy")));
    record.setValue(model->fieldIndex("amount"),QVariant(tr("the amount")));
    record.setValue(model->fieldIndex("notes"),QVariant(tr("Misc.")));
    record.setValue(personid,QVariant(1));
    record.setValue(expenseid,QVariant(1));
    model->insertRecord(-1,record);
}

void KCTicketPanel::removeEntry()
{
    model->removeRow(listView->currentIndex().row());
    model->submitAll();
}

void KCTicketPanel::setCurrentModelIndex()
{
    mapper->setCurrentModelIndex(listView->currentIndex());
}

void KCTicketPanel::selectPanel()
{
    model->select();
    personRel->select();
    expenseRel->select();
}

void KCTicketPanel::initDB(const QString& connection)
{
    QSqlQuery query(QSqlDatabase::database(connection));
    query.exec("CREATE TABLE tickets (id INTEGER PRIMARY KEY, "
               "date TEXT, amount REAL, notes TEXT, "
               "personid INTEGER DEFAULT 1 REFERENCES person(id) "
               "ON UPDATE CASCADE ON DELETE SET NULL, "
               "expenseid INTEGER DEFAULT 1 REFERENCES expenses(id) "
               "ON UPDATE CASCADE ON DELETE SET NULL"
               ")");
}
