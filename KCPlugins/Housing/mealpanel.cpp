/*
 * Copyright (c) 2012-2013, Olivier Tilmans
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

#include "mealpanel.h"
#include "ui_meal.h"
#include "ui_mealsubscription.h"
#include <QtWidgets/QWidget>
#include <QDialog>
#include <QSqlError>
#include <QSqlTableModel>
#include <kcglobals.h>
#include <kccore.h>
#include <kcaccountfile.h>
#include <kcdatabase.h>
#include <QDebug>
#include <QSqlRecord>
#include <kccomboboxdelegate.h>

#define MEAL_KEY "meal_event_id"

class MealPanelPrivate {
public:
    QWidget widget;
    QDialog dialog;
    QSqlTableModel *personModel;
    QSqlTableModel *model;
    KCDatabase* db;

    MealPanelPrivate()
    {}
};

MealPanel::MealPanel(KCAccountFile *account, QWidget *parent) :
    KCPanel(account, parent),
    ui(new Ui::Meal),
    dialog(new Ui::MealSubscription),
    d(new MealPanelPrivate)
{
    ui->setupUi(&d->widget);
    dialog->setupUi(&d->dialog);
    dialog->comboBox->setEditable(true);
    d->dialog.hide();
    d->dialog.setModal(true);
    d->dialog.setWindowTitle(tr("Register a subscription to a meal"));
    // Setup database model
    d->db = new KCDatabase(KCPanel::account);
    d->model = new QSqlTableModel(this, d->db->db());
    d->model->setTable("meals");
    d->model->setEditStrategy(QSqlTableModel::OnFieldChange);
    d->model->select();
    QSqlRecord r = d->model->record();
    // bind the model to the table view
    ui->tableView->setModel(d->model);
    // We don't want to display the column id
    ui->tableView->hideColumn(r.indexOf("id"));
    ui->tableView->hideColumn(r.indexOf("date"));
    // And we authorize the user to edit as he wishes
    ui->tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->tableView->setSortingEnabled(true);
    // setup column stretching
    QHeaderView *header = ui->tableView->horizontalHeader();
    header->setStretchLastSection(true);
    header->setSectionResizeMode(r.indexOf("subscription_count"), QHeaderView::ResizeToContents);
    header->setSectionResizeMode(r.indexOf("person_id"), QHeaderView::Stretch);
    header->setSectionsMovable(true);
    // Pretty column names
    d->model->setHeaderData(r.indexOf("subscription_count"), Qt::Horizontal, tr("Subscriptions"));
    d->model->setHeaderData(r.indexOf("person_id"), Qt::Horizontal, tr("Person"));
    connect(ui->add, SIGNAL(clicked()), &d->dialog, SLOT(show()));
    connect(dialog->cancel, SIGNAL(clicked()), &d->dialog, SLOT(hide()));
    connect(dialog->ok, SIGNAL(clicked()), this, SLOT(addSubcription()));
    connect(ui->remove, SIGNAL(clicked()), this, SLOT(removeSubscriptions()));
    connect(ui->calendarWidget, SIGNAL(selectionChanged()), this, SLOT(updateFilter()));
    connect(ui->calendarWidget, SIGNAL(currentPageChanged(int,int)), this, SLOT(pageChanged(int,int)));
    connect(d->model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(updateHighlightsOfTheDay()));
    ui->calendarWidget->setSelectedDate(QDate::currentDate());
    pageChanged(QDate::currentDate().year(), QDate::currentDate().month());
}

MealPanel::~MealPanel()
{
    delete ui;
    delete dialog;
    delete d;
}

const QString MealPanel::panelName()
{
    return tr("Meal management");
}

QWidget* MealPanel::panel()
{
    return &d->widget;
}

const QString MealPanel::iconName()
{
    return QString(":/icon/meal");
}

void MealPanel::allPanelsCreated()
{
    d->personModel = (QSqlTableModel*)KCPanel::account->model(MODEL_PERSON);
    QSqlRecord p = d->personModel->record();
    dialog->comboBox->setModel(d->personModel);
    dialog->comboBox->setModelColumn(p.indexOf("name"));
    ui->tableView->setItemDelegateForColumn(d->model->record().indexOf("person_id"),
                                            new KCComboBoxDelegate(d->personModel,
                                                                   p.indexOf("name"),
                                                                   p.indexOf("id"),
                                                                   ui->tableView));
    // Attempts to retrieve the meal event id
    int oldId = KCPanel::account->getProperty(MEAL_KEY).toInt();
    // Query the db to find it
    KCDatabase db(KCPanel::account);
    QSqlQuery query = db.query("SELECT name FROM events WHERE id = " + QString::number(oldId));
    // If we found it (and there was one set previously)
    if (query.first() && oldId) {
        query.clear();
        // Ensures that it has the proper values to be identifiable
        query.exec("UPDATE events SET name='" + tr("Meals") + "', misc='"
                   + tr("Autogenerated event for the meal management module.")
                   + "', date = NULL"
                   + " WHERE id = " + QString::number(oldId));
    } else {
        // Otherwise create it
        query = db.query("INSERT INTO events(name, misc) VALUES('"
                         + tr("Meals") + "', '"
                         + tr("Autogenerated event for the meal management module.")
                         + "')");
        // And saves its id
        KCPanel::account->setProperty(MEAL_KEY, query.lastInsertId());
    }
    if (!query.isActive())
        qDebug() << Q_FUNC_INFO << query.lastError();
}

void MealPanel::selected()
{
    d->personModel->select();
}

void MealPanel::unselected()
{
    if (!d->personModel->submitAll())
        KCCore::instance()->warning(
                    tr("Failed to submit changes to the people,\nreason: %1")
                    .arg(d->personModel->lastError().text()));
}

void MealPanel::updateHighlightsOfTheDay()
{
    setHighlights(ui->calendarWidget->selectedDate());
}

void MealPanel::setHighlights(const QDate &date)
{
    QSqlQuery q = d->db->query("SELECT SUM(subscription_count) FROM meals "
                               "WHERE date = '" + QVariant(date).toString() + "'");
    if (!q.first())
        KCCore::instance()->warning("Failed to compute the sum of the meal subscriptions for"
                                    " the day,\nreason: " + q.lastError().text());
    else {
        ui->calendarWidget->setHighlightsForDay(date.day(), q.record().value(0).toInt());
    }
}

void MealPanel::pageChanged(int year, int month)
{
    QDate d(year, month, 1);
    for (int i = 1; i < d.daysInMonth() + 1; ++i)
        setHighlights(QDate(year, month, i));
}

void MealPanel::updateFilter()
{
    d->model->setFilter("date = '" + QVariant(ui->calendarWidget->selectedDate()).toString() + "'");
}

void MealPanel::removeSubscriptions()
{
    // Freeze view update when the data change
    ui->tableView->setUpdatesEnabled(false);
    QModelIndexList indexes = ui->tableView->selectionModel()->selectedIndexes();
    qSort(indexes.begin(), indexes.end());
    // remove all rows one by one
    for(int i = indexes.count() - 1; i > -1; --i) {
        d->model->removeRow(indexes.at(i).row());
    }
    // update the database/view
    ui->tableView->setUpdatesEnabled(true);
    d->model->submitAll();
    d->model->select();
    updateHighlightsOfTheDay();
}

void MealPanel::addSubcription()
{
    d->personModel->submit();
    int subs = dialog->spinBox->value();
    qDebug() << subs;
    QString pid = d->personModel->record(dialog->comboBox->currentIndex()).value("id").toString();
    QDate date = ui->calendarWidget->selectedDate();
    QSqlQuery q = d->db->query("SELECT subscription_count FROM meals WHERE date = '"
                               + QVariant(date).toString() + "' AND person_id = " + pid);
    if (!q.first()) {
        QSqlRecord ins = d->model->record();
        // fill in our new data
        ins.setValue(ins.indexOf("subscription_count"), subs);
        ins.setValue(ins.indexOf("date"), date);
        ins.setValue(ins.indexOf("person_id"), pid);
        // insert it at the bottom of the table
        if (!d->model->insertRecord(-1, ins) || !d->model->submit()) {
            KCCore::instance()->warning(tr("Failed to insert a new meal subscription !\nreason: %1")
                                        .arg(d->model->lastError().text()));
        }
    } else {
        int oldSubs = q.record().value("subscription_count").toInt();
        q.clear();
        if (!q.exec("UPDATE meals SET subscription_count = "
                    + QString::number(subs + oldSubs)
                    + " WHERE date = '"
                    + QVariant(date).toString() + "' AND person_id = " + pid))
            KCCore::instance()->warning(tr("Failed to update a meal subscription !\nreason: %1")
                                        .arg(d->model->lastError().text()));
    }
    d->model->select();
    d->dialog.hide();
    updateHighlightsOfTheDay();
}
