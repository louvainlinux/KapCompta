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

#include <QObject>

class KCAccountFilePrivate;
/**
 * @brief The KCAccountFile class provides an abstraction for all operation available
 *        on an account file.
 */
class KCAccountFile : public QObject
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
    bool save() const;
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

private:
    KCAccountFilePrivate *d;
};

#endif // KCACCOUNTFILE_H
