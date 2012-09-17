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

#include "kcdatabasehelper.h"
#include <QStringList>
#include <QFile>
#include <QSqlQuery>
#include <QMessageBox>
#include <QVariant>
#include <QSqlDatabase>
#include <QApplication>
#include <QSqlRecord>

KCDataBaseHelper::KCDataBaseHelper(QObject *parent) :
    QObject(parent)
{
}

void KCDataBaseHelper::initDB(const QString &path)
{
    createConnection(path);
}
#include <QDebug>
double KCDataBaseHelper::sumAllExpenses(const QString& connection)
{
    qDebug("all expenses");
    QSqlQuery query(QSqlDatabase::database(connection));
    if (query.exec("SELECT SUM(amount) FROM tickets")) {
        query.first();
        return query.record().value(0).toDouble();
    } else {
        return -1;
    }
}


double KCDataBaseHelper::sumExpenses(const QString& connection, const int expense_id)
{
    qDebug("sum expenses");
    QSqlQuery query(QSqlDatabase::database(connection));
    if (query.exec("SELECT SUM(amount) FROM tickets WHERE id = " + expense_id)) {
        query.first();
        return query.record().value(0).toDouble();
    } else {
        return -1;
    }
}

double KCDataBaseHelper::sumNegativeExpenses(const QString& connection, const int expense_id)
{
    qDebug("sum neg expenses");
    QSqlQuery query(QSqlDatabase::database(connection));
    if (query.exec("SELECT SUM(amount) FROM tickets WHERE amount < 0 AND id = " + expense_id)) {
        query.first();
        return query.record().value(0).toDouble();
    } else {
        return -1;
    }
}

double KCDataBaseHelper::sumPositiveExpenses(const QString& connection, const int expense_id)
{
    qDebug("sum pos expenses");
    QSqlQuery query(QSqlDatabase::database(connection));
    if (query.exec("SELECT SUM(amount) FROM expenses WHERE amount > 0 AND id = " + expense_id)) {
        query.first();
        return query.record().value(0).toDouble();
    } else {
        return -1;
    }
}

void KCDataBaseHelper::createConnection(const QString &path)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",path);
    db.setDatabaseName(path);
    if (!db.open()) {
        QMessageBox::critical(0, tr("Cannot open database"),
                              tr("Unable to establish a database connection."
                                 ), QMessageBox::Cancel);
        QApplication::exit();
    }
}

void KCDataBaseHelper::close(const QString& path)
{
    QSqlDatabase::database(path).commit();
    QSqlDatabase::database(path).close();
}
