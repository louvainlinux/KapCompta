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

KCDataBaseHelper::KCDataBaseHelper(QObject *parent) :
    QObject(parent)
{
}

void KCDataBaseHelper::initDB(const QString &path)
{
    createConnection(path);
    QSqlQuery query(QSqlDatabase::database(path));
    query.exec("CREATE TABLE person (id INTEGER PRIMARY KEY, "
               "name TEXT, iban VARCHAR(27), misc TEXT)");
    query.exec("INSERT INTO person(name, iban, misc) VALUES('test', 'compte','nada')");
}

void KCDataBaseHelper::createConnection(const QString &path)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",path);
    db.setDatabaseName(path);
    if (!db.open()) {
        QMessageBox::critical(0, tr("Cannot open database"),
            tr("Unable to establish a database connection."
               ), QMessageBox::Cancel);
    }
}

void KCDataBaseHelper::close(const QString& path)
{
    QSqlDatabase::database(path).commit();
    QSqlDatabase::database(path).close();
    QSqlDatabase::removeDatabase(path);
}
