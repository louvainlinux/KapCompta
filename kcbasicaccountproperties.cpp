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
#include <QDoubleValidator>

#define NAMEKEY    ( QString("accountName")    )
#define IBANKEY    ( QString("accountIBAN")    )
#define BALANCEKEY ( QString("accountBalance") )

KCBasicAccountProperties::KCBasicAccountProperties(QObject *parent) :
    QObject(parent),
    panel(new QWidget()),
    iban(new QLineEdit(QString("BEXX XXXX XXXX XXXX"), panel)),
    name(new QLineEdit(tr("Unnamed account"), panel)),
    balance(new QLineEdit(QString("0"), panel))
{
    QDoubleValidator *balanceValidator = new QDoubleValidator(this);
    balance->setValidator(balanceValidator);

    connect(iban,SIGNAL(editingFinished()),this,SLOT(altered()));
    connect(name,SIGNAL(editingFinished()),this,SLOT(altered()));
    connect(balance,SIGNAL(editingFinished()),this,SLOT(altered()));

    QFormLayout *layout = new QFormLayout();
    layout->addRow(tr("Account name:"), name);
    layout->addRow(tr("Account bank number (i.e. IBAN):"), iban);
    layout->addRow(tr("Initial Balance:"), balance);
    panel->setLayout(layout);

    observer = NULL;
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
    hash->insert(NAMEKEY, QVariant(name->text()));
    hash->insert(IBANKEY, QVariant(iban->text()));
    hash->insert(BALANCEKEY, QVariant(balance->text()));
    return hash;
}

void KCBasicAccountProperties::setSetting(const QString& key, const QVariant& value)
{
    if (!key.compare(NAMEKEY)) {
        name->setText(value.toString());
    } else if (!key.compare(IBANKEY)) {
        iban->setText(value.toString());
    } else if (!key.compare(BALANCEKEY)) {
        balance->setText(value.toString());
    }
}

const QString& KCBasicAccountProperties::category()
{
    static QString a = QString("General");
    return a;
}

const QStringList &KCBasicAccountProperties::keys(){
    static QStringList l = QStringList() << NAMEKEY << IBANKEY << BALANCEKEY;
    return l;
}

void KCBasicAccountProperties::addObserver(KCObserver *o)
{
    observer = o;
}

void KCBasicAccountProperties::altered()
{
    if (observer != NULL) observer->callback();
}
