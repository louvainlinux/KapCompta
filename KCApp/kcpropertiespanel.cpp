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

#include "kcpropertiespanel.h"
#include "kcfilemanager.h"
#include "kcbasicaccountproperties.h"
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QList>
#include <QVariant>
#include "kcsettings.h"
#include <kcaccountsetting.h>

KCPropertiesPanel::KCPropertiesPanel(KCFileManager *fm, QWidget *parent) :
    QWidget(parent)
{
    this->fm = fm;
    // Check whether the user wants autosaves or not
    autosave = false || KCSettings::valueOf("autoSaveProjectProperties").toBool();
}

void KCPropertiesPanel::setSettingList(QList<KCAccountSetting*> l)
{
    settingsList = QList<KCAccountSetting*>(l);
    settingsList.prepend(new KCBasicAccountProperties(this));
}

QWidget* KCPropertiesPanel::panel()
{
    return this;
}

const QString KCPropertiesPanel::title() const
{
    return tr("Account properties");
}

const QString KCPropertiesPanel::iconPath() const
{
    return QString(":/icons/option");
}

void KCPropertiesPanel::buildGUI(const QString &connection)
{
    Q_UNUSED(connection);

    QVBoxLayout *layout = new QVBoxLayout();
    // Add a Box for any group of settings we registered
    QList<KCAccountSetting*>::iterator settings;
    for(settings = settingsList.begin(); settings != settingsList.end(); ++settings) {
        (*settings)->setEditable(true);
        QGroupBox *box = new QGroupBox((*settings)->category(),this);
        QWidget *widget = (*settings)->settingPanel();
        QVBoxLayout *vLayout = new QVBoxLayout();
        vLayout->addWidget(widget);
        box->setLayout(vLayout);
        layout->addWidget(box);
        // And keep track of when edits have been made
        connect(*settings, SIGNAL(settingChanged()), this, SLOT(settingsAltered()));
    }
    // Control buttons
    saveBtn = new QPushButton(tr("Save account properties"));
    reloadBtn = new QPushButton(tr("Revert to saved"));
    connect(saveBtn, SIGNAL(clicked()), this, SLOT(saveSettings()));
    connect(reloadBtn,SIGNAL(clicked()),this,SLOT(reloadSettings()));
    // Final layout
    QHBoxLayout *btns = new QHBoxLayout();
    btns->addWidget(reloadBtn);
    btns->addWidget(saveBtn);
    btns->addStretch(1);
    layout->addLayout(btns);
    layout->addStretch(1);
    this->setLayout(layout);
    // Load intial values
    this->reload();
}

void KCPropertiesPanel::saveSettings()
{
    // Retrieve all settings and update the configuration file accordingly
    foreach(KCAccountSetting* as, settingsList) {
        fm->setValues(as->settings(),as->category());
    }
    // Settings are written, cannot go-back anymore this disable the buttons
    reloadBtn->setEnabled(false);
    saveBtn->setEnabled(false);
}

void KCPropertiesPanel::reload()
{
    QList<KCAccountSetting*>::const_iterator settings;
    // Look up in the configuration file and load its settings
    for(settings = settingsList.begin(); settings != settingsList.end(); ++settings) {
        QString category = QString((*settings)->category());
        QStringList keys = (*settings)->keys();
        // Apply all settings present in the configurtion file for the category
        QStringList::const_iterator it = keys.constBegin();
        while (it != keys.constEnd()) {
            (*settings)->setSetting(*it, fm->value(category + "/" +*it));
            ++it;
        }
    }
    // Settings are up-to-date
    reloadBtn->setEnabled(false);
    saveBtn->setEnabled(false);
}

void KCPropertiesPanel::reloadSettings()
{
    reload();
}

void KCPropertiesPanel::settingsAltered()
{
    saveBtn->setEnabled(true);
    reloadBtn->setEnabled(true);
    if (autosave) this->saveSettings();
}

void KCPropertiesPanel::unselectPanel()
{
    if (autosave) this->saveSettings();
}
