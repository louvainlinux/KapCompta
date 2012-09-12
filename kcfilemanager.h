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

#ifndef KCFILEMANAGER_H
#define KCFILEMANAGER_H

#include <QObject>
#include <QHash>

QT_BEGIN_NAMESPACE
class QVariant;
class QTemporaryFile;
QT_END_NAMESPACE

class KCFileManager : public QObject
{
    Q_OBJECT
public:
    explicit KCFileManager(const QString& filePath, QObject *parent = 0);
    ~KCFileManager();
    bool createNewAccount();
    void setValue(const QString& key, const QVariant& value);
    void setValues(const QHash<QString, QVariant>& values, const QString &category);
    QVariant value(const QString& key);
    QString dbPath();
    bool open();
    bool save();

signals:
    
public slots:

private:
    QTemporaryFile *db;
    QTemporaryFile *properties;
    QString fileName;
};

#endif // KCFILEMANAGER_H
