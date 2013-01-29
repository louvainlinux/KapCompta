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
#include "KCCore_global.h"

QT_BEGIN_NAMESPACE
class QVariant;
class QTemporaryFile;
QT_END_NAMESPACE

class KCCORESHARED_EXPORT KCFileManager : public QObject
{
    /*
     * Provide access to read/write the content of an account savefile,
     * as well as convenience methods around it.
     */
    Q_OBJECT

public:
    /*
     * Initialize an instance by creating its temporary files.
     * filePath is the path where the account save file is located.
     */
    explicit KCFileManager(const QString& filePath, QObject *parent = 0);
    /*
     * Clean the temporary files, update the save file, and close the
     * database conection.
     */
    ~KCFileManager();
    /*
     * Create a new account file at filePath.
     */
    bool createNewAccount();
    /*
     * Set the <key, value> property for the account file
     */
    void setValue(const QString& key, const QVariant& value);
    /*
     * Set the <key, value> properties for the account file for the given
     * category of properties.
     */
    void setValues(const QHash<QString, QVariant>& values, const QString &category);
    /*
     * Retrieves property value for the given key of the account file
     */
    const QVariant value(const QString& key) const;
    /*
     * Retrieves property value for the given key of the account file in
     * the given category
     */
    const QVariant valueForCategory(const QString& key,
                                          const QString& category) const;
    /*
     * Return the path to the temporary database file
     */
    const QString dbPath() const;
    /*
     * Open the account save file
     */
    bool open();
    /*
     * Propagate the changes in the temporary files to the account save file
     */
    bool save();
    /*
     * Return the filePath made of up to maxLength chars.
     */
    const QString shortenedPath(int maxLength) const;
    /*
     * Return the str made of up to maxLength chars.
     * Should be moved to KCCore ?
     */
    static const QString shortenedPath(int maxLength, const QString &str);

private:
    QTemporaryFile *db;
    QTemporaryFile *properties;
    QString fileName;
};

#endif // KCFILEMANAGER_H
