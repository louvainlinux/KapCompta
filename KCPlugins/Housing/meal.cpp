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
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>

Meal::Meal(QWidget *parent) :
    QWidget(parent)
{
}

Meal::~Meal()
{
    delete popup;
}

void Meal::buildGUI(const QString& connection)
{
    this->connection = QString(connection);
    // Setup calendar
    calendar = new MealCalendar(this);
    calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    calendar->setGridVisible(true);
    calendar->setFirstDayOfWeek(Qt::Monday);
    refreshCalendar(QDate::currentDate().year(), QDate::currentDate().month());
    connect(calendar, SIGNAL(currentPageChanged(int,int)), this, SLOT(refreshCalendar(int,int)));
    connect(calendar, SIGNAL(clicked(QDate)), this, SLOT(editDay(QDate)));
    // Setup popup editor widget
    popup = new QWidget();
    popup->setWindowFlags(Qt::Popup);
    popup->setVisible(false);
    popup->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    // Setup popup contents
    QVBoxLayout *popupLayout = new QVBoxLayout();
    popupContentLayout = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();
    QPushButton *plus = new QPushButton("+");
    connect(plus, SIGNAL(clicked()), this, SLOT(addMeal()));
    popupDay = new QLabel(this);
    popupDay->setAlignment(Qt::AlignCenter);
    // Setup popup layouts
    hLayout->addWidget(plus);
    hLayout->addWidget(popupDay);
    hLayout->addStretch(1);
    popupLayout->addLayout(hLayout);
    popupLayout->addLayout(popupContentLayout, 1);
    popup->setLayout(popupLayout);
    // Setup layout
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(calendar);
    this->setLayout(layout);
}

void Meal::addMeal()
{
    MealEditor *editor = new MealEditor(day, connection, this);
    popupContentLayout->addWidget(editor);
    connect(editor, SIGNAL(removed(MealEditor*)), this, SLOT(mealRemoved(MealEditor*)));
    popup->adjustSize();
    refreshCalendar(QDate::currentDate().year(), QDate::currentDate().month());
}

void Meal::mealRemoved(MealEditor *editor)
{
    popupContentLayout->removeWidget(editor);
    popup->adjustSize();
    refreshCalendar(QDate::currentDate().year(), QDate::currentDate().month());
    delete editor;
}

void Meal::editDay(const QDate &day)
{
    cleanPopup();
    this->day = QDate(day);
    popupDay->setText(tr("Edit meals on the <b>") + day.toString("dd/MM/yy") + "</b>");
    QSqlQuery query(QSqlDatabase::database(connection));
    query.exec(QString("SELECT id FROM meals WHERE date = '") + day.toString("dd/MM/yyyy") + "'");
    while (query.next()) {
        QSqlRecord record = query.record();
        MealEditor *editor = new MealEditor(day, connection, record.value("id").toInt(), this);
        popupContentLayout->addWidget(editor);
        connect(editor, SIGNAL(removed(MealEditor*)), this, SLOT(mealRemoved(MealEditor*)));
    }
    popup->adjustSize();
    popup->show();
}

void Meal::refreshCalendar(int year, int month)
{
    QHash<int,int> months;
    QSqlQuery query(QSqlDatabase::database(connection));
    for (int day = 1; day < 32; ++day) {
        QString queryText = QString("SELECT COUNT(*) FROM meals WHERE date = '") + KCCore::twoDigit(day)
                + "/" + KCCore::twoDigit(month) + "/" + KCCore::twoDigit(year) + "'";
        query.exec(queryText);
        query.first();
        QSqlRecord record = query.record();
        months.insert(day, record.value(0).toInt());
    }
    calendar->setCurrentMonthHighlights(months);
}

void Meal::selectPanel()
{
    refreshCalendar(QDate::currentDate().year(), QDate::currentDate().month());
}

void Meal::unselectPanel()
{
    cleanPopup();
}

void Meal::cleanPopup()
{
    popup->hide();
    // Empty it
    QLayoutItem* item;
    while ((item = popupContentLayout->takeAt(0)) != NULL)
    {
        delete item->widget()->layout();
        delete item->widget();
        delete item;
    }
}

void Meal::initDB(const QString& connection)
{
    QSqlQuery query(QSqlDatabase::database(connection));
    query.exec("CREATE TABLE meals (id INTEGER PRIMARY KEY, "
               "date TEXT)");
    query.exec("INSERT INTO expenses(name, hidden) VALUES('"+tr("Meals")+"', 1)");
    query.exec("CREATE TABLE meals_subscription (id INTEGER PRIMARY KEY, "
               "mealid INTEGER REFERENCES meals(id)"
               "ON UPDATE CASCADE ON DELETE DELETE)"
               "personid INTEGER REFERENCES person(id) "
               "ON UPDATE CASCADE ON DELETE DELETE, "
               "cross INTEGER)");
    query.exec("CREATE TABLE meals_ticket (id INTEGER PRIMARY KEY, "
               "mealid INTEGER REFERENCES meals(id)"
               "ON UPDATE CASCADE ON DELETE DELETE)"
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

MealEditor::MealEditor(const QDate& day, const QString& connection, QWidget *parent)
    : QWidget(parent)
{
    this->day = QDate(day);
    this->connection = QString(connection);
    addRecord();
    buildGUI();
}

MealEditor::MealEditor(const QDate& day, const QString& connection, int meal_id, QWidget *parent)
    : QWidget(parent)
{
    this->day = QDate(day);
    this->meal_id = meal_id;
    this->connection = QString(connection);
    buildGUI();
}

void MealEditor::addRecord()
{
    QSqlQuery query(QSqlDatabase::database(connection));
    QString queryText = "INSERT INTO meals(date) VALUES('" + day.toString("dd/MM/yyyy")+ "')";
    query.exec(queryText);
}

void MealEditor::buildGUI()
{
    QGroupBox *box = new QGroupBox(this);
    QVBoxLayout *layout = new QVBoxLayout();
    QPushButton *remove = new QPushButton("-", this);
    QHBoxLayout *header = new QHBoxLayout();
    connect(remove, SIGNAL(clicked()), this, SLOT(remove()));
    subscriptions = new QLabel(this);
    individualPrice = new QLabel(this);
    totalPrice = new QLabel(this);
    header->addWidget(remove);
    header->addWidget(subscriptions);
    header->addWidget(individualPrice);
    header->addWidget(totalPrice);

    layout->addLayout(header);
    box->setLayout(layout);
    this->setLayout(new QVBoxLayout());
    this->layout()->addWidget(box);
    updateHeader();
    this->adjustSize();
}

void MealEditor::remove()
{
    QSqlQuery query(QSqlDatabase::database(connection));
    query.exec("DELETE FROM meals WHERE id = '" + QString::number(meal_id) + "'");
    emit removed(this);
}

void MealEditor::updateHeader()
{
    QSqlQuery query(QSqlDatabase::database(connection));
    query.exec("SELECT COUNT(*) FROM meals_subscription WHERE mealid = '"
               + QString::number(meal_id) + "'");
    query.first();
    int subsCount = query.record().value(0).toInt();
    query.exec("SELECT SUM(amount) FROM tickets WHERE id IN "
               "(SELECT ticketid FROM meals_ticket WHERE mealid = '"
               + QString::number(meal_id) + "'");
    query.first();
    double sum = query.record().value(0).toInt();
    double costPP = (subsCount == 0 ? 0 : sum/subsCount);

    totalPrice->setText(tr("Total meal price: ") + QString::number(sum) + " " + tr("eur."));
    individualPrice->setText(tr("Price per person: ") + QString::number(costPP) + " " + tr("eur."));
    subscriptions->setText(QString::number(subsCount)
                          + (subsCount > 1 ? tr(" people have subscribed to the meal")
                                           : tr(" person has subscribed to the meal")
                                             ));
    this->adjustSize();
}
