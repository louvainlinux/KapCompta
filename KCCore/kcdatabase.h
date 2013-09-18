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

#ifndef KCDATABASE_H
#define KCDATABASE_H

#include "kccore_global.h"

#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>

class KCAccountFile;
class KCDatabasePrivate;
class KCCORESHARED_EXPORT KCDatabase : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief defaultName default database name
     */
    static const QString defaultName;
    /**
     * @brief KCDatabase Initialize and open a database connection
     * @param file the account file containing this database
     * @param databaseName the name of the database to use
     * @param parent the parent QObject
     */
    explicit KCDatabase(KCAccountFile* file,
                        const QString& databaseName = defaultName,
                        QObject *parent = 0);
    ~KCDatabase();
    /**
     * @brief create add a new database to the given account file
     * @param file the account which will have the new database
     * @param databaseName the name of the new database
     */
    static void create(KCAccountFile* file,
                       const QString& databaseName = defaultName);
    /**
     * @brief getQuery instantiates a new sqlquery object linked to this database.
     * @param queryText the query to execute if not empty.
     * @return the query executed if the queryText was not empty, otherwise a clean query object
     */
    QSqlQuery query(const QString& queryText = QString());
    /**
     * @brief db returns a database connection
     * @return a database connection
     */
    QSqlDatabase db();

signals:

public slots:
    /**
     * @brief open Opens the connection to the database
     */
    void open();
    /**
     * @brief close closes the connection to the database
     */
    void close();
private:
    KCDatabasePrivate *d;
};

#endif // KCDATABASE_H
