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

#ifndef KCACCOUNTPROPERTY_H
#define KCACCOUNTPROPERTY_H

#include <kcaccountsetting.h>
#include <QObject>

QT_BEGIN_NAMESPACE
class QWidget;
class QLineEdit;
QT_END_NAMESPACE

class KCBasicAccountProperties : public KCAccountSetting
{
    /*
     * Provide basic properties for an account
     */
    Q_OBJECT

public:
    explicit KCBasicAccountProperties(QObject *parent = 0);
    ~KCBasicAccountProperties();
    // KCAccountSetting interface
    QWidget* settingPanel();
    void setEditable(bool editable);
    const QHash<QString, QVariant> settings() const;
    void setSetting(const QString& key, const QVariant& value);
    const QString category() const;
    const QStringList keys() const;

signals:
    void settingChanged();

private:
    QWidget *panel;
    QLineEdit *iban;
    QLineEdit *name;
    QLineEdit *balance;
};

#endif // KCACCOUNTPROPERTY_H
