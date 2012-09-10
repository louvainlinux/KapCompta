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

#include "kcspendingpanel.h"
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

KCSpendingPanel::KCSpendingPanel(QWidget *parent) :
    QWidget(parent)
{
}

QWidget* KCSpendingPanel::panel()
{
    return this;
}

const QString& KCSpendingPanel::title()
{
    static QString s = QString(tr("Expense items"));
    return s;
}

const QString& KCSpendingPanel::iconPath()
{
    static QString s = QString(":/icons/event");
    return s;
}

void KCSpendingPanel::buildGUI(const QString &connection)
{
    connectionName = QString(connection);

    model = new QSqlTableModel(this,QSqlDatabase::database(connectionName));
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    model->setTable("expenses");
    model->select();

    QLineEdit *name = new QLineEdit(this);
    QCompleter *nameCompleter = new QCompleter(this);
    nameCompleter->setModel(model);
    nameCompleter->setCompletionColumn(model->fieldIndex("name"));
    nameCompleter->setCompletionMode(QCompleter::InlineCompletion);
    name->setCompleter(nameCompleter);
    QLineEdit *date = new QLineEdit(this);
    QCompleter *dateCompleter = new QCompleter(this);
    dateCompleter->setModel(model);
    dateCompleter->setCompletionColumn(model->fieldIndex("date"));
    dateCompleter->setCompletionMode(QCompleter::InlineCompletion);
    date->setCompleter(dateCompleter);
    QTextEdit *description = new QTextEdit(this);
    description->setAcceptRichText(false);
    QPushButton *add = new QPushButton(QString("+"),this);
    QPushButton *remove = new QPushButton(QString("-"), this);

    listView = new QListView();
    listView->setAlternatingRowColors(true);
    listView->setSelectionMode(QAbstractItemView::SingleSelection);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listView->setModel(model);
    listView->setModelColumn(model->fieldIndex("name"));

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(model);
    mapper->addMapping(name, model->fieldIndex("name"));
    mapper->addMapping(date, model->fieldIndex("date"));
    mapper->addMapping(description, model->fieldIndex("description"));
    mapper->setCurrentIndex(-1);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();
    QFormLayout *layout = new QFormLayout();
    layout->addRow(tr("Name:"), name);
    layout->addRow(tr("Date:"), date);
    layout->addRow(tr("Description:"), description);
    buttonLayout->addWidget(add);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(remove);
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(listView);
    leftLayout->addLayout(buttonLayout);
    hLayout->addLayout(leftLayout);
    hLayout->addLayout(layout,1);
    this->setLayout(hLayout);

    connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            listView, SLOT(dataChanged(QModelIndex,QModelIndex)));
    connect(listView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,SLOT(setCurrentModelIndex()));
    connect(add, SIGNAL(clicked()), this, SLOT(addEntry()));
    connect(remove, SIGNAL(clicked()), this, SLOT(removeEntry()));
}

void KCSpendingPanel::addEntry()
{
    QSqlRecord record = model->record();
    record.setValue(model->fieldIndex("name"),QVariant(tr("New Expense Name")));
    record.setValue(model->fieldIndex("date"),QVariant("i.e. 27th of September 2012"));
    record.setValue(model->fieldIndex("description"),QVariant(tr("Description of the expenses items")));
    model->insertRecord(1,record);
}

void KCSpendingPanel::removeEntry()
{
    model->removeRow(listView->currentIndex().row());
    model->submitAll();
}

void KCSpendingPanel::setCurrentModelIndex()
{
    mapper->setCurrentModelIndex(listView->currentIndex());
}
