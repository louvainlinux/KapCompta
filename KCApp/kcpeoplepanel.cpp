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

#include "kcpeoplepanel.h"
#include <QHBoxLayout>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QListView>
#include <QPushButton>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QSqlRecord>
#include <QCompleter>
#include <QSqlQuery>
#include <QModelIndex>

KCPeoplePanel::KCPeoplePanel(QWidget *parent) :
    QWidget(parent)
{
}

void KCPeoplePanel::setCurrentModelIndex()
{
    mapper->setCurrentModelIndex(listView->currentIndex());
}

KCPeoplePanel::~KCPeoplePanel() {
}

QWidget* KCPeoplePanel::panel()
{
    return this;
}

const QString KCPeoplePanel::title() const
{
    return tr("Person management");
}

const QString KCPeoplePanel::iconPath() const
{
    return QString(":/icons/people");
}

void KCPeoplePanel::addEntry()
{
    QSqlRecord record = model->record();
    record.setValue(model->fieldIndex("name"),QVariant(tr("New Name")));
    record.setValue(model->fieldIndex("iban"),QVariant("BEXX XXXX XXXX XXXX"));
    record.setValue(model->fieldIndex("misc"),QVariant(tr("Misc.")));
    record.setValue(model->fieldIndex("hidden"),QVariant(0));
    // Insert new entry at the end
    model->insertRecord(-1,record);
}

void KCPeoplePanel::removeEntry()
{
    model->removeRow(listView->currentIndex().row());
    // Propagate changes
    model->submitAll();
}

void KCPeoplePanel::unselectPanel()
{
    // As our model/sqlmodel are asynchronous, save changes before losing selection
    saveAll();
}

void KCPeoplePanel::saveAll()
{
    QModelIndex idx = listView->currentIndex();
    model->submitAll();
    // Calling submit on model causes a selection reset
    listView->setCurrentIndex(idx);
    mapper->setCurrentModelIndex(idx);
}

void KCPeoplePanel::buildGUI(const QString &connection)
{
    connectionName = QString(connection);
    // Setup SQL model
    model = new QSqlTableModel(this,QSqlDatabase::database(connectionName));
    // QT-BUG xxx: cannot use both sqlmodel & DWM in autosubmit
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("person");
    model->setFilter("hidden = 0");
    model->setSort(model->fieldIndex("name"), Qt::AscendingOrder);
    model->select();
    // Setup form/controls
    QLineEdit *name = new QLineEdit(this);
    QCompleter *nameCompleter = new QCompleter(this);
    nameCompleter->setModel(model);
    nameCompleter->setCompletionColumn(model->fieldIndex("name"));
    nameCompleter->setCompletionMode(QCompleter::InlineCompletion);
    name->setCompleter(nameCompleter);
    QLineEdit *iban = new QLineEdit(this);
    QCompleter *ibanCompleter = new QCompleter(this);
    ibanCompleter->setModel(model);
    ibanCompleter->setCompletionColumn(model->fieldIndex("iban"));
    ibanCompleter->setCompletionMode(QCompleter::InlineCompletion);
    iban->setCompleter(ibanCompleter);
    QPlainTextEdit *misc = new QPlainTextEdit(this);
    QPushButton *add = new QPushButton(QString("+"),this);
    QPushButton *remove = new QPushButton(QString("-"), this);

    listView = new QListView();
    listView->setAlternatingRowColors(true);
    listView->setSelectionMode(QAbstractItemView::SingleSelection);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listView->setModel(model);
    listView->setModelColumn(model->fieldIndex("name"));
    // Setup DataWidgetMapper
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(model);
    mapper->addMapping(name, model->fieldIndex("name"));
    mapper->addMapping(iban, model->fieldIndex("iban"));
    mapper->addMapping(misc, model->fieldIndex("misc"));
    // We start with no selection
    mapper->setCurrentIndex(-1);
    // Setup layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();
    QFormLayout *layout = new QFormLayout();
    layout->addRow(tr("Name:"), name);
    layout->addRow(tr("Bank account:"), iban);
    layout->addRow(tr("Misc.:"), misc);
    layout->setSizeConstraint(QLayout::SetNoConstraint);
    layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    buttonLayout->addWidget(add);
    buttonLayout->addWidget(remove);
    QVBoxLayout *leftLayout = new QVBoxLayout();
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
    // Setup tabbing
    this->setTabOrder(add,remove);
    this->setTabOrder(remove,listView);
    this->setTabOrder(listView,name);
    this->setTabOrder(name,iban);
    this->setTabOrder(iban,misc);
}

void KCPeoplePanel::initDB(const QString& connection)
{
    QSqlQuery query(QSqlDatabase::database(connection));
    query.exec("CREATE TABLE person (id INTEGER PRIMARY KEY, "
               "name TEXT, iban VARCHAR(27), misc TEXT,"
               "hidden INTEGER DEFAULT 0)");
    query.exec("INSERT INTO person(name, hidden) VALUES('"+tr("Nobody")+"', 1)");
}