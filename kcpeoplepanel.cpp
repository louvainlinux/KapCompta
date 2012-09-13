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
#include <QTextEdit>
#include <QListView>
#include <QPushButton>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QSqlRecord>
#include <QCompleter>
#include <QSqlQuery>

KCPeoplePanel::KCPeoplePanel(QWidget *parent) :
    QWidget(parent)
{
}

void KCPeoplePanel::setCurrentModelIndex()
{
    mapper->setCurrentModelIndex(listView->currentIndex());
}

QWidget* KCPeoplePanel::panel()
{
    return this;
}

const QString& KCPeoplePanel::title()
{
    static QString s = QString(tr("Person management"));
    return s;
}

const QString& KCPeoplePanel::iconPath()
{
    static QString s = QString(":/icons/people");
    return s;
}

void KCPeoplePanel::addEntry()
{
    QSqlRecord record = model->record();
    record.setValue(model->fieldIndex("name"),QVariant(tr("New Name")));
    record.setValue(model->fieldIndex("iban"),QVariant("BEXX XXXX XXXX XXXX"));
    record.setValue(model->fieldIndex("misc"),QVariant(tr("Misc.")));
    model->insertRecord(-1,record);
    hideRows();
}

void KCPeoplePanel::removeEntry()
{
    model->removeRow(listView->currentIndex().row());
    model->submitAll();
    hideRows();
}

void KCPeoplePanel::buildGUI(const QString &connection)
{
    connectionName = QString(connection);

    model = new QSqlTableModel(this,QSqlDatabase::database(connectionName));
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->setTable("person");
    model->select();

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
    QTextEdit *misc = new QTextEdit(this);
    misc->setAcceptRichText(false);
    QPushButton *add = new QPushButton(QString("+"),this);
    QPushButton *remove = new QPushButton(QString("-"), this);

    listView = new QListView();
    listView->setAlternatingRowColors(true);
    listView->setSelectionMode(QAbstractItemView::SingleSelection);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listView->setModel(model);
    listView->setModelColumn(model->fieldIndex("name"));
    hideRows();

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(model);
    mapper->addMapping(name, model->fieldIndex("name"));
    mapper->addMapping(iban, model->fieldIndex("iban"));
    mapper->addMapping(misc, model->fieldIndex("misc"));
    mapper->setCurrentIndex(-1);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();
    QFormLayout *layout = new QFormLayout();
    layout->addRow(tr("Name:"), name);
    layout->addRow(tr("Bank account:"), iban);
    layout->addRow(tr("Misc.:"), misc);
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
    connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(hideRows()));
    connect(listView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,SLOT(setCurrentModelIndex()));
    connect(add, SIGNAL(clicked()), this, SLOT(addEntry()));
    connect(remove, SIGNAL(clicked()), this, SLOT(removeEntry()));

    this->setTabOrder(add,remove);
    this->setTabOrder(remove,listView);
    this->setTabOrder(listView,name);
    this->setTabOrder(name,iban);
    this->setTabOrder(iban,misc);
}

void KCPeoplePanel::hideRows()
{
    listView->setRowHidden(0,true);
}

void KCPeoplePanel::initDB(const QString& connection)
{
    QSqlQuery query(QSqlDatabase::database(connection));
    query.exec("CREATE TABLE person (id INTEGER PRIMARY KEY, "
               "name TEXT, iban VARCHAR(27), misc TEXT)");
    query.exec("INSERT INTO person(id, name) VALUES(1,'"+tr("Nobody")+"')");
}
