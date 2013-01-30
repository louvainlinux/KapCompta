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
#include <QSqlDatabase>
#include <QApplication>
#include <QSqlRecord>

void KCDataBaseHelper::initDB(const QString &path)
{
    // We currently do not need to perform any further initialisation
    // steps as we're letting each KCPanel perform another step
    // to initialize the DB to its needs.
    createConnection(path);
}

QVariant KCDataBaseHelper::firstEntryOf(const QString& connection, const QString& q)
{
    QSqlQuery query(QSqlDatabase::database(connection));
    if (query.exec(q)) {
        query.first();
        return query.record().value(0);
    } else { // if for some reasons the query failed
        return QVariant();
    }
}

double KCDataBaseHelper::sumAllExpenses(const QString& connection)
{
    return firstEntryOf(connection, "SELECT SUM(amount) FROM tickets").toDouble();
}

double KCDataBaseHelper::sumExpenses(const QString& connection, const int expense_id)
{

    return firstEntryOf(connection,
                        "SELECT SUM(amount) FROM tickets "
                        "WHERE expenseid = " + QString::number(expense_id)).toDouble();
}

double KCDataBaseHelper::overalBalance(const QString &connection, const int person_id)
{
    return firstEntryOf(connection,
                        "SELECT SUM(amount) FROM tickets "
                        "WHERE personid = " + QString::number(person_id)).toDouble();
}

double KCDataBaseHelper::overalBalanceForExpense(const QString& connection,
                                                        const int person_id, const int expense_id)
{
    return firstEntryOf(connection,
                        "SELECT SUM(amount) FROM tickets "
                        "WHERE personid = " + QString::number(person_id) +
                        " AND expenseid = " + QString::number(expense_id)).toDouble();
}

double KCDataBaseHelper::sumNegativeExpenses(const QString& connection, const int expense_id)
{
    return firstEntryOf(connection,
                        "SELECT SUM(amount) FROM tickets "
                        "WHERE (CAST(amount AS REAL) < 0) AND expenseid = "
                        + QString::number(expense_id)).toDouble();
    // Cast used to circonvent the fact that SQLITE doesn't recognise that the string
    // "-3" represents a real
}

double KCDataBaseHelper::sumPositiveExpenses(const QString& connection, const int expense_id)
{
    return firstEntryOf(connection,
                        "SELECT SUM(amount) FROM tickets "
                        "WHERE (CAST(amount AS REAL) > 0) AND expenseid = "
                        + QString::number(expense_id)).toDouble();
}

void KCDataBaseHelper::createConnection(const QString &path)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",path);
    db.setDatabaseName(path);
    if (!db.open()) {
        QMessageBox::critical(0, QObject::tr("Cannot open database"),
                              QObject::tr("Unable to establish a database connection."
                                 ), QMessageBox::Cancel);
        QApplication::exit();
    }
}

void KCDataBaseHelper::close(const QString& path)
{
    QSqlDatabase::database(path).commit();
    QSqlDatabase::database(path).close();
}
