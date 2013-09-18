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

#include "kcdatabase.h"
#include "kcaccountfile.h"

#include <QSqlDatabase>
#include <QDebug>

class KCDatabasePrivate {
public:
    KCAccountFile *file;
    QString name;
    QString id;

    KCDatabasePrivate(KCAccountFile *f, QString n) :
        file(f),
        name(n),
        id(name + ":" + file->fileName())
    {
    }

    ~KCDatabasePrivate()
    {
        close();
    }

    void open()
    {
        instances[id]++;
        if (instances[id] == 1) {
            QSqlDatabase db = QSqlDatabase::database(name);
            if (db.isValid()) close();
            db = QSqlDatabase::addDatabase("QSQLITE", file->componentPath(name));
            db.setDatabaseName(name);
            if (!db.open())
                qDebug() << Q_FUNC_INFO << "Cannot open database";
        }
    }

    void close()
    {
        instances[id]--;
        QSqlDatabase db = QSqlDatabase::database(name);
        if (db.isValid()) {
            db.commit();
            if (instances[id] < 1) {
                instances.remove(id);
                db.close();
            }
        }
    }

private:
    static QHash<QString, int> instances;
};

QHash<QString, int> KCDatabasePrivate::instances = QHash<QString, int>();

KCDatabase::KCDatabase(KCAccountFile* file,
                       const QString &databaseName,
                       QObject *parent) :
    QObject(parent),
    d(new KCDatabasePrivate(file, databaseName))
{
    connect(file, SIGNAL(busy()), this, SLOT(close()));
    connect(file, SIGNAL(available()), this, SLOT(open()));
    d->open();
}

void KCDatabase::create(KCAccountFile *file, const QString &databaseName)
{
    file->addComponent(databaseName);
}

QSqlQuery KCDatabase::query(const QString& queryText)
{
    qDebug() << Q_FUNC_INFO << queryText;
    return QSqlQuery(queryText, QSqlDatabase::database(d->name));
}

void KCDatabase::open()
{
    d->open();
}

void KCDatabase::close()
{
    d->close();
}

const QString KCDatabase::defaultName = QString("DEFAULT_DATABASE");
