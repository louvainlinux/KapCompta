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

#ifndef KCDATABASEHELPER_H
#define KCDATABASEHELPER_H

#include <QVariant>
#include <QObject>
#include "KCCore_global.h"

QT_BEGIN_NAMESPACE
class QStringList;
class QFile;
QT_END_NAMESPACE

class KCCORESHARED_EXPORT KCDataBaseHelper : QObject
{
    /*
     * Provides general purpose functions on the data stored in the
     * SQLite DB.
     */
public:
    /*
     * Initialize a database file at the given path and open a connection to it.
     */
    static void initDB(const QString &path);
    /*
     * Open a connection to the given database file
     */
    static void createConnection(const QString &path);
    /*
     * Close (gracefully) the connection for the given database file
     */
    static void close(const QString &path);
    /*
     * Return the sum of all expenses for the given database connection
     */
    static double sumAllExpenses(const QString& connection);
    /*
     * Return the sum of all expenses matching expense_id (the given item
     * of expense) for the given database connection
     */
    static double sumExpenses(const QString& connection, const int expense_id);
    /*
     * Return the sum of all negative expenses for the given database connection
     */
    static double sumNegativeExpenses(const QString& connection, const int expense_id);
    /*
     * Return the sum of all positive expenses for the given database connection
     */
    static double sumPositiveExpenses(const QString& connection, const int expense_id);
    /*
     * Return the overal Balance of person X in the account
     */
    static double overalBalance(const QString &connection, const int person_id);
    /*
     * Return the balance of person X in the account for the specified item of expense
     */
    static double overalBalanceForExpense(const QString &connection, const int person_id, const int expense_id);
private:
    /*
     * Return the first record resulting of querying the database opened
     * in the given connection, with the query q
     */
    static QVariant firstEntryOf(const QString& connection, const QString& q);
};

#endif // KCDATABASEHELPER_H
