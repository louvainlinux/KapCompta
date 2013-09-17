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

class KCAccountFile;
class KCDatabasePrivate;
class KCCORESHARED_EXPORT KCDatabase : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief KCDatabase Initialize a database provider
     * @param file the account file containing this database
     * @param databaseName the name of the database to use
     * @param create whether the database has to be created or already exists
     * @param parent the parent QObject
     */
    explicit KCDatabase(KCAccountFile* file,
                        const QString& databaseName = QString("DEFAULT_DATABASE"),
                        bool create = false,
                        QObject *parent = 0);
signals:

public slots:

private:
    KCDatabasePrivate *d;
};

#endif // KCDATABASE_H
