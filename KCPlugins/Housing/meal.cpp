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

#include "meal.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QListView>
#include <QTableView>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>

Meal::Meal(QWidget *parent) :
    QWidget(parent)
{
}

void Meal::buildGUI(const QString& connection)
{
    // -- Left side of the panel
    QListView *list = new QListView(this);

    QPushButton *add = new QPushButton("+",this);
    QPushButton *remove = new QPushButton("-",this);
    QHBoxLayout *add_remove = new QHBoxLayout();
    add_remove->addWidget(add);
    add_remove->addWidget(remove);

    QComboBox *filter = new QComboBox(this);
    QFormLayout *filter_layout = new QFormLayout();
    filter_layout->addRow(tr("List by:"), filter);
    filter_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    filter_layout->setSizeConstraint(QLayout::SetNoConstraint);

    QVBoxLayout *left = new QVBoxLayout();
    left->addLayout(filter_layout);
    left->addLayout(add_remove);
    left->addWidget(list);

    // -- Right side of the panel
    QFormLayout *date_layout = new QFormLayout();
    QLineEdit *date = new QLineEdit(this);
    date_layout->addRow(tr("Date:"),date);
    date_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    date_layout->setSizeConstraint(QLayout::SetNoConstraint);

    QTableView *people = new QTableView(this);

    QHBoxLayout *ticket_text_layout = new QHBoxLayout();
    ticket_text_layout->addWidget(new QLabel(tr("Available tickets"), this));
    ticket_text_layout->addStretch();
    ticket_text_layout->addWidget(new QLabel(tr("Tickets selected for the meal")));

    QTableView *available_tickets = new QTableView(this);
    available_tickets->setAlternatingRowColors(true);

    QPushButton *select_ticket = new QPushButton(">>>",this);
    QPushButton *deselect_ticket = new QPushButton("<<<",this);
    QVBoxLayout *selection_layout = new QVBoxLayout();
    selection_layout->addStretch();
    selection_layout->addWidget(select_ticket);
    selection_layout->addWidget(deselect_ticket);
    selection_layout->addStretch();

    QTableView *selected_tickets = new QTableView(this);
    selected_tickets->setAlternatingRowColors(true);

    QHBoxLayout *table_layout = new QHBoxLayout();
    table_layout->addWidget(available_tickets);
    table_layout->addLayout(selection_layout);
    table_layout->addWidget(selected_tickets);

    QVBoxLayout *box_layout = new QVBoxLayout();
    box_layout->addLayout(ticket_text_layout);
    box_layout->addLayout(table_layout);

    QGroupBox *tickets_box = new QGroupBox(this);
    tickets_box->setLayout(box_layout);
    tickets_box->setTitle(tr("Tickets"));

    QVBoxLayout *right = new QVBoxLayout();
    right->addLayout(date_layout);
    right->addWidget(people);
    right->addWidget(tickets_box);
    // Panel layout
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addLayout(left);
    layout->addLayout(right,1);
    this->setLayout(layout);
}

void Meal::initDB(const QString& connection)
{
    QSqlQuery query(QSqlDatabase::database(connection));
    query.exec("CREATE TABLE meals (id INTEGER PRIMARY KEY, "
               "date TEXT)");
    query.exec("INSERT INTO expenses(name, hidden) VALUES('"+tr("Meals")+"', 1)");
    query.exec("CREATE TABLE meals_subscription (id INTEGER PRIMARY KEY, "
               "personid INTEGER REFERENCES person(id) "
               "ON UPDATE CASCADE ON DELETE SET NULL, "
               "cross INTEGER)");
    query.exec("CREATE TABLE meals_ticket (id INTEGER PRIMARY KEY, "
               "ticketid INTEGER REFERENCES tickets(id) "
               "ON UPDATE CASCADE ON DELETE DELETE)");
}

QWidget* Meal::panel()
{
    return this;
}

const QString Meal::title() const
{
    return tr("Meal management");
}

const QString Meal::iconPath() const
{
    return QString(":/meals");
}
