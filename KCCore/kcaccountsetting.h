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

#ifndef KCACCOUNTSETTING_H
#define KCACCOUNTSETTING_H

#include "KCCore_global.h"
#include <QString>
#include <QVariant>
#include <QHash>
#include <QObject>

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

class KCCORESHARED_EXPORT KCAccountSetting : QObject {
    Q_OBJECT

public:
    virtual ~KCAccountSetting() {}

    /*
     * Returns the widget displaying the settings
     **/
    virtual QWidget* settingPanel() = 0;
    /*
     * Sets whether the settings are editable
     **/
    virtual void setEditable(const bool editable) = 0;
    /*
     * Returns a hash map of key/value representing the settings
     **/
    virtual const QHash<QString, QVariant> settings() const = 0;
    /*
     * Sets the setting value for the specified key
     **/
    virtual void setSetting(const QString& key, const QVariant& value) = 0;
    /*
     * Returns the setting category name
     **/
    virtual const QString category() const = 0;
    /*
     * Returns the settings key
     **/
    virtual const QStringList keys() const = 0;

signals:
    /*
     * Emitted when a setting has been changed
     */
    void settingChanged();
};

#endif // KCACCOUNTSETTING_H
