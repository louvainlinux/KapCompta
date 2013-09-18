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

#ifndef KCACCOUNTFILE_H
#define KCACCOUNTFILE_H

#include "kccore_global.h"
#include <QObject>
#include <QVariant>

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
QT_END_NAMESPACE

class KCAccountFilePrivate;
/**
 * @brief The KCAccountFile class provides an abstraction for all operation available
 *        on an account file.
 */
class KCCORESHARED_EXPORT KCAccountFile : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief KCAccountFile initializes an account file at the given path.
     * @param filename the name of the account file to create.
     */
    KCAccountFile(const QString& filename, QObject *parent = 0);
    ~KCAccountFile();
    /**
     * @brief saves the current state of the account to its filename.
     * @return true on success, false otherwise.
     */
    bool save();
    /**
     * @brief reads the account, potentially resetting any unsaved change.
     * @return true on success, false otherwise.
     */
    bool read() const;
    /**
     * @brief addComponent adds a component with the given name to the account file.
     * @param componentName the name of the component to be added.
     */
    void addComponent(const QString& componentName);
    /**
     * @brief componentPath provide a path which can be used to retrieve (or set) the content of
     *          the given account file component.
     * @param componentName the name of the component to look for
     * @return the absolute path to that component if it exists, or an empty string.
     */
    const QString componentPath(const QString& componentName) const;
    /**
     * @brief setProperty will associate the given value to the given key for this account file
     * @param key the property name
     * @param value its associated value
     */
    void setProperty(const QString& key, const QVariant& value);
    /**
     * @brief getProperty will return the property value corresponding to the given key
     * @param key the property to look for
     * @return its associated value or an empty QVariant if not present
     */
    const QVariant getProperty(const QString& key) const;
    /**
     * @brief lastError returns the last error message that happened
     * @return the message or an empty string if no error happened
     */
    const QString lastError() const;
    /**
     * @brief fileName returns the filename of this account
     * @return the corresponding file for this acount
     */
    const QString fileName() const;
    /**
     * @brief registerModel registers the given model to the account with the specified key.
     * @param model the object modelling some data about this account
     * @param key the associated key
     */
    void registerModel(QAbstractItemModel* model, const QString& key);
    /**
     * @brief model returns the model associated to the acount for the given key
     * @param key the key to look for
     * @return the model, or NULL if not found
     */
    QAbstractItemModel* model(const QString& key) const;
    /**
     * @brief openedAccount gives the opened accounts
     * @return the list of path to every opened account
     */
    static const QStringList openedAccount();

signals:
    /**
     * @brief aboutToSave is emitted when the account file needs to be accessed in an exclusive way,
     * thus that all opened file descriptors to its components should be closed.
     * i.e. save operation
     */
    void busy();
    /**
     * @brief saved is emitted when the account file is no longer used in an exclusive mode.
     * (counterpart of busy() )
     */
    void available();

private:
    KCAccountFilePrivate *d;
};

#endif // KCACCOUNTFILE_H
