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
#include "mealcalendar.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QVariant>
#include <QVBoxLayout>
#include <QHash>
#include <kccore.h>

Meal::Meal(QWidget *parent) :
    QWidget(parent)
{
}

void Meal::buildGUI(const QString& connection)
{
    this->connection = QString(connection);
    // Setup calendar
    calendar = new MealCalendar(this);
    calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    calendar->setGridVisible(true);
    refreshCalendar(QDate::currentDate().day(), QDate::currentDate().year());
    connect(calendar, SIGNAL(currentPageChanged(int,int)), this, SLOT(refreshCalendar(int,int)));
    connect(calendar, SIGNAL(clicked(QDate)), this, SLOT(editDay(QDate)));
    // Setup layout
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(calendar);
    this->setLayout(layout);
}

void Meal::editDay(const QDate &day)
{

}

void Meal::refreshCalendar(int year, int month)
{
    QHash<int,int> months;
    QSqlQuery query(QSqlDatabase::database(connection));
    for (int day = 1; day < 32; ++day) {
        query.exec(QString("SELECT COUNT(*) FROM meals WHERE date = ") + KCCore::twoDigit(day)
                   + "/" + KCCore::twoDigit(month) + "/" + KCCore::twoDigit(year));
        query.first();
        QSqlRecord record = query.record();
        months.insert(day, record.value(0).toInt());
    }
    calendar->setCurrentMonthHighlights(months);
}

void Meal::selectPanel()
{

}

void Meal::unselectPanel()
{

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
