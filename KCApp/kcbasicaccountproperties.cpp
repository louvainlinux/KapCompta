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

static QString NAMEKEY    = ( QString("accountName")    );
static QString IBANKEY    = ( QString("accountIBAN")    );
static QString BALANCEKEY = ( QString("accountBalance") );

KCBasicAccountProperties::KCBasicAccountProperties(QObject *parent) :
    KCAccountSetting(parent),
    panel(new QWidget()),
    iban(new QLineEdit(QString("BEXX XXXX XXXX XXXX"), panel)),
    name(new QLineEdit(tr("Unnamed account"), panel)),
    balance(new QLineEdit(QString("0"), panel))
{
    // Validate user input data
    QDoubleValidator *balanceValidator = new QDoubleValidator(this);
    balance->setValidator(balanceValidator);
    // Connect signals/slots
    connect(iban,SIGNAL(editingFinished()),this,SLOT(altered()));
    connect(name,SIGNAL(editingFinished()),this,SLOT(altered()));
    connect(balance,SIGNAL(editingFinished()),this,SLOT(altered()));
    // Setup layout
    QFormLayout *layout = new QFormLayout();
    layout->addRow(tr("Account name:"), name);
    layout->addRow(tr("Account bank number (i.e. IBAN):"), iban);
    layout->addRow(tr("Initial Balance:"), balance);
    layout->setSizeConstraint(QLayout::SetNoConstraint);
    layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    panel->setLayout(layout);
    // Setup tabbing
    panel->setTabOrder(name,iban);
    panel->setTabOrder(iban,balance);
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

const QHash<QString, QVariant> KCBasicAccountProperties::settings() const
{
    QHash<QString, QVariant> hash;
    hash.insert(NAMEKEY, QVariant(name->text()));
    hash.insert(IBANKEY, QVariant(iban->text()));
    hash.insert(BALANCEKEY, QVariant(balance->text()));
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

const QString KCBasicAccountProperties::category() const
{
    return QString("General");
}

const QStringList KCBasicAccountProperties::keys() const
{
    return QStringList() << NAMEKEY << IBANKEY << BALANCEKEY;
}
