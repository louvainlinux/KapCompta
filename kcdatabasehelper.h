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

#include <QObject>

QT_BEGIN_NAMESPACE
class QStringList;
class QFile;
QT_END_NAMESPACE

class KCDataBaseHelper : public QObject
{
    Q_OBJECT
public:
    explicit KCDataBaseHelper(QObject *parent = 0);
    static void initDB(const QString &path);
    static void createConnection(const QString &path);
    static void close(const QString &path);
    static double sumAllExpenses(const QString& connection);
    static double sumExpenses(const QString& connection, const int expense_id);
    static double sumNegativeExpenses(const QString& connection, const int expense_id);
    static double sumPositiveExpenses(const QString& connection, const int expense_id);

signals:

public slots:

};

#endif // KCDATABASEHELPER_H
