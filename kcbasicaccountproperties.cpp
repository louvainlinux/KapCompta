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

#include "kcbasicaccountproperties.h"
#include <QWidget>
#include <QHash>
#include <QVariant>
#include <QFormLayout>
#include <QLineEdit>

#define NAMEKEY "accountName"
#define IBANKEY "accountIBAN"
#define BALANCEKEY "accountBalance"

KCBasicAccountProperties::KCBasicAccountProperties(QObject *parent) :
    QObject(parent),
    panel(new QWidget()),
    iban(new QLineEdit(QString("000-0000000-00"), panel)),
    name(new QLineEdit(tr("Unnamed account"), panel)),
    balance(new QLineEdit(QString("0"), panel))
{
    QFormLayout *layout = new QFormLayout();
    layout->addRow(tr("Account name:"), name);
    layout->addRow(tr("Account bank number (i.e. IBAN):"), iban);
    layout->addRow(tr("Balance:"), balance);
    panel->setLayout(layout);
}

KCBasicAccountProperties::~KCBasicAccountProperties()
{
    delete panel;
}

QWidget* KCBasicAccountProperties::settingPanel()
{
    return panel;
}

void KCBasicAccountProperties::setEditable(bool editable)
{
    name->setEnabled(editable);
    balance->setEnabled(editable);
    iban->setEnabled(editable);
}

QHash<QString, QVariant>* KCBasicAccountProperties::settings()
{
    QHash<QString, QVariant> *hash = new QHash<QString, QVariant>();
    hash->insert(QString(NAMEKEY), QVariant(name->text()));
    hash->insert(QString(IBANKEY), QVariant(iban->text()));
    hash->insert(QString(BALANCEKEY), QVariant(balance->text()));
    return hash;
}

void KCBasicAccountProperties::setSetting(const QString& key, const QVariant& value)
{
    if (!key.compare(QString(NAMEKEY))) {
        name->setText(value.toString());
    } else if (!key.compare(QString(IBANKEY))) {
        iban->setText(value.toString());
    } else if (!key.compare(QString(BALANCEKEY))) {
        balance->setText(value.toString());
    }
}

const QString& KCBasicAccountProperties::category()
{
    static QString a = QString("General");
    return a;
}
