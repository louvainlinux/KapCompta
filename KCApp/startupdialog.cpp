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

#include "startupdialog.h"
#include "kcmainwindow.h"
#include "ui_startupdialog.h"

#include <QSettings>

#define RECENT_ACCOUNT_KEY QString("default/recently_opened")

StartupDialog::StartupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartupDialog)
{
    ui->setupUi(this);
    populateRecentAccounts();
    this->setWindowTitle(tr("Welcome in KapCompta"));
    connect(ui->accountName, SIGNAL(textChanged(QString)), this, SLOT(checkCreationInfo()));
    connect(ui->create, SIGNAL(clicked()), this, SLOT(createAccount()));
    connect(ui->browse, SIGNAL(clicked()), this, SLOT(browseAccount()));
    connect(ui->recent, SIGNAL(activated(int)), this, SLOT(recentAccount(int)));
}

StartupDialog::~StartupDialog()
{
    delete ui;
}

void StartupDialog::createAccount()
{
    this->openAccount(NULL);
}

void StartupDialog::browseAccount()
{
    this->openAccount(NULL);
}

void StartupDialog::recentAccount(int index)
{
    if (!index) return;
    this->openAccount(NULL);
}

void StartupDialog::checkCreationInfo()
{
    if (ui->accountName->text().length()) ui->create->setEnabled(true);
    else ui->create->setEnabled(false);
}

void StartupDialog::populateRecentAccounts()
{
    QSettings s;
    QStringList recents = s.value(RECENT_ACCOUNT_KEY).toStringList();
    recents.prepend(tr("..."));
    ui->recent->insertItems(0, recents);
}

void StartupDialog::openAccount(const QString &path)
{
    KCMainWindow *mainWindow = new KCMainWindow(path);
    mainWindow->adjustSize();
    mainWindow->show();
    this->hide();
    this->deleteLater();
}
