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

#ifndef KCCORE_H
#define KCCORE_H

#include "kccore_global.h"
#include <QObject>
#include <QList>

class KCCorePrivate;
class KCPanel;
class KCAccountFile;

class KCCORESHARED_EXPORT KCCore : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief instance gives the global instance of the shared lib
     * @return the shared object
     */
    static KCCore* instance();
    /**
     * @brief panels concatenate all available panels and export them to the caller.
     *              those can come from plugins, ... Multiple calls will generate new instances
     *              which must be managed by the caller.
     * @brief account the account file linked to these panel instances
     * @return  the list of panel.
     */
    const QList<KCPanel *> panels(KCAccountFile *account);
    /**
     * @brief createAccount initialize a new KCAccountFile at the given location
     * @param location the place where the account should be saved
     * @param name the account name
     * @param description the account description
     */
    void createAccount(const QString &location, const QString& name, const QString &description);
    /**
     * @brief openAccount Attempts to open the given account and initialize it if needed. i.e.
     *              if new plugins have been activated
     * @param account the account to open
     * @return true on success, false otherwise
     */
    bool openAccount(KCAccountFile *account);

signals:
    /**
     * @brief statusUpdate emitted when the shared wants to notify all observers that a new status
     *                      message is available.
     * @param timeout the duration in ms after which the message is no longer valid.
     */
    void statusUpdate(const QString&, int timeout = 0);

public slots:
    /**
     * @brief setStatus post a status update message; triggers statusUpdate()
     * @param timeout @see statusUpdate()
     */
    void setStatus(const QString&, int timeout = 0);
    /**
     * @brief warning displays a warning message to the user
     * @param message the information to display
     */
    void warning(const QString& message);

private:
    KCCore();
    ~KCCore();

    static KCCore *s_instance;
    KCCorePrivate *d;
};

#endif // KCCORE_H
