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
#include <QTableView>
#include <QtSql/QSqlRelationalTableModel>
#include <QtSql/QSqlRelationalDelegate>
#include <QtSql/QSqlQueryModel>
#include <QListView>
#include <QHBoxLayout>
#include <QtSql/QSqlRelation>
#include <QHeaderView>

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
    popupContentLayout->setSizeConstraint(QLayout::SetFixedSize);
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
    popupLayout->setSizeConstraint(QLayout::SetFixedSize);
    hLayout->setSizeConstraint(QLayout::SetFixedSize);
    popup->setLayout(popupLayout);
    // Setup layout
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(calendar);
    this->setLayout(layout);
}

void Meal::addMeal()
{
    MealEditor *editor = new MealEditor(day, connection);
    popupContentLayout->addWidget(editor);
    connect(editor, SIGNAL(removed(MealEditor*)), this, SLOT(mealRemoved(MealEditor*)));
    popup->adjustSize();
    refreshCalendar(QDate::currentDate().year(), QDate::currentDate().month());
}

void Meal::mealRemoved(MealEditor *editor)
{
    popupContentLayout->removeWidget(editor);
    refreshCalendar(QDate::currentDate().year(), QDate::currentDate().month());
    editor->deleteLater();
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
        MealEditor *editor = new MealEditor(day, connection, record.value("id").toInt());
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
               "mealid INTEGER REFERENCES meals(id) "
               "ON UPDATE CASCADE ON DELETE CASCADE, "
               "personid INTEGER REFERENCES person(id) "
               "ON UPDATE CASCADE ON DELETE CASCADE, "
               "cross INTEGER)");
    query.exec("CREATE TABLE meals_ticket (id INTEGER PRIMARY KEY, "
               "mealid INTEGER REFERENCES meals(id) "
               "ON UPDATE CASCADE ON DELETE CASCADE, "
               "ticketid INTEGER REFERENCES tickets(id) "
               "ON UPDATE CASCADE ON DELETE CASCADE)");
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
    meal_id = query.lastInsertId().toInt();
}

void MealEditor::buildGUI()
{
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    QGroupBox *box = new QGroupBox(this);
    QVBoxLayout *layout = new QVBoxLayout();
    QPushButton *remove = new QPushButton("-", this);
    QHBoxLayout *header = new QHBoxLayout();
    connect(remove, SIGNAL(clicked()), this, SLOT(remove()));
    subscriptions = new QLabel(this);
    subscriptions->setStyleSheet("border: 1px solid grey");
    individualPrice = new QLabel(this);
    individualPrice->setStyleSheet("border: 1px solid grey");
    totalPrice = new QLabel(this);
    totalPrice->setStyleSheet("border: 1px solid grey");
    header->addWidget(remove);
    header->addWidget(subscriptions);
    header->addWidget(individualPrice);
    header->addWidget(totalPrice);
    header->addStretch(1);
    // Setup subscription table
    QTableView *subsTable = new QTableView(this);
    QSqlRelationalTableModel *model = new QSqlRelationalTableModel(
                this, QSqlDatabase::database(connection));
    model->setTable("meals_subscription");
    model->setFilter("mealid = '" + QString::number(meal_id) + "'");
    int personField = model->fieldIndex("personid");
    int crossField = model->fieldIndex("cross");
    int mealidField = model->fieldIndex("mealid");
    int idField = model->fieldIndex("id");
    model->setRelation(personField, QSqlRelation("person", "id", "name"));
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();
    model->setHeaderData(personField, Qt::Horizontal, tr("Person"));
    model->setHeaderData(crossField, Qt::Horizontal, tr("Number of meals"));
    subsTable->setModel(model);
    subsTable->setItemDelegate(new QSqlRelationalDelegate(subsTable));
    // hide mealid/id columns (but do not remove them or we cannot edit the table anymore)
    subsTable->setColumnHidden(mealidField, true);
    subsTable->setColumnHidden(idField, true);
    subsTable->horizontalHeader()->setResizeMode(crossField, QHeaderView::ResizeToContents);
    subsTable->horizontalHeader()->setResizeMode(personField, QHeaderView::Stretch);
    // Setup tickets management
    int exp_id = retrieveExpenseID(); // this is ugly!
    availQuery = "SELECT date, amount, id FROM tickets WHERE expenseid = '"
            + QString::number(exp_id) + "' AND id NOT IN "
            "(SELECT ticketid FROM meals_ticket)";
    usedQuery = "SELECT date, amount, id FROM tickets WHERE "
            "id IN (SELECT ticketid FROM meals_ticket WHERE mealid = '"
            + QString::number(meal_id) + "')";
    availableTickets = new QListView(this);
    availableTickets->setMaximumWidth(200);
    availableTickets->setMinimumWidth(100);
    availableTickets->setSelectionMode(QAbstractItemView::MultiSelection);
    usedTickets = new QListView(this);
    usedTickets->setMaximumWidth(200);
    usedTickets->setMinimumWidth(100);
    usedTickets->setSelectionMode(QAbstractItemView::MultiSelection);
    availModel = new TicketQueryModel();
    availModel->setQuery(availQuery, QSqlDatabase::database(connection));
    usedModel = new TicketQueryModel();
    usedModel->setQuery(usedQuery, QSqlDatabase::database(connection));
    availableTickets->setModel(availModel);
    usedTickets->setModel(usedModel);
    // Button to make the ticket switch around
    QPushButton *add = new QPushButton(">>>", this);
    QPushButton *free = new QPushButton("<<<", this);
    connect(add, SIGNAL(clicked()), this, SLOT(addTicket()));
    connect(free, SIGNAL(clicked()), this, SLOT(freeTicket()));
    // Setup layout
    QVBoxLayout *btnLayout = new QVBoxLayout();
    btnLayout->addStretch(1);
    btnLayout->addWidget(add);
    btnLayout->addWidget(free);
    btnLayout->addStretch(1);
    QVBoxLayout *availL = new QVBoxLayout();
    QVBoxLayout *usedL = new QVBoxLayout();
    availL->addWidget(new QLabel(tr("Available tickets"), this));
    availL->addWidget(availableTickets, 1);
    usedL->addWidget(new QLabel(tr("Selected tickets"), this));
    usedL->addWidget(usedTickets, 1);
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(subsTable, 1);
    hLayout->addLayout(availL, 1);
    hLayout->addLayout(btnLayout);
    hLayout->addLayout(usedL, 1);
    layout->addLayout(header);
    layout->addLayout(hLayout);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    box->setLayout(layout);
    this->setLayout(new QVBoxLayout());
    this->layout()->addWidget(box);
    updateHeader();
    this->adjustSize();
}
void MealEditor::addTicket()
{
    foreach (QModelIndex idx, availableTickets->selectionModel()->selectedIndexes()) {
        // Get selected row
        int row = idx.row();
        // Find id based on selected row
        const QModelIndex index_id = availModel->index(row, 2);
        QString id = availModel->data(index_id, Qt::DisplayRole).toString();
        // Add it to the assigned tickets
        QSqlQuery query(QSqlDatabase::database(connection));
        query.exec("INSERT INTO meals_ticket (mealid, ticketid) "
                   "VALUES('" + QString::number(meal_id) + "', '"
                   + id + "')");
    }
    usedModel->setQuery(usedQuery, QSqlDatabase::database(connection));
    availModel->setQuery(availQuery, QSqlDatabase::database(connection));
    updateHeader();
}

void MealEditor::freeTicket()
{
    foreach (QModelIndex idx, usedTickets->selectionModel()->selectedIndexes()) {
        // Get selected row
        int row = idx.row();
        // Find ticketid based on selected row
        const QModelIndex index_id = usedModel->index(row, 2);
        QString id = usedModel->data(index_id, Qt::DisplayRole).toString();
        // Remove it from the list
        QSqlQuery query(QSqlDatabase::database(connection));
        query.exec("DELETE FROM meals_ticket WHERE ticketid = '"
                   + id + "'");
    }
    usedModel->setQuery(usedQuery, QSqlDatabase::database(connection));
    availModel->setQuery(availQuery, QSqlDatabase::database(connection));
    updateHeader();
}

int MealEditor::retrieveExpenseID()
{
    QSqlQuery query(QSqlDatabase::database(connection));
    query.exec("SELECT id FROM expenses where name = '" + tr("Meals") + "'");
    query.first();
    return query.record().value(0).toInt();
}

void MealEditor::remove()
{
    QSqlQuery query(QSqlDatabase::database(connection));
    query.exec("DELETE FROM meals WHERE id = '" + QString::number(meal_id) + "'");
    emit removed(this);
    parentWidget()->adjustSize();
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
               + QString::number(meal_id) + "')");
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

TicketQueryModel::TicketQueryModel(QObject *parent)
    : QSqlQueryModel(parent)
{
}

QVariant TicketQueryModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);
    if (value.isValid() && role == Qt::DisplayRole && index.column() == 0) {
        QVariant value2 = QSqlQueryModel::data(this->index(index.row(), index.column() + 1), role);
        return value.toString() + " (" + value2.toString() + " " + tr("eur.") +")";
    }
    // Other roles/columns value will follow the classical behaviour
    return value;
}
