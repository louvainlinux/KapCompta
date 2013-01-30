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

#include "kcwelcomescreen.h"
#include "kcnewaccount.h"
#include "kcsettings.h"
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>
#include <QDesktopWidget>
#include <QApplication>
#include <QFileDialog>
#include <QVariant>
#include <QGroupBox>
#include <QDir>
#include <kcfilemanager.h>
#include <kccore.h>

KCWelcomeScreen::KCWelcomeScreen(QWidget *parent) :
    QWidget(parent),
    w(NULL)
{
    // Setup controls
    QPushButton *create = new QPushButton(tr("Create an account"), this);
    QPushButton *open = new QPushButton(tr("Open an account"), this);
    // Setup welcome text
    QLabel *welcomeText = new QLabel(tr("Welcome in KapCompta!"), this);
    QFont font;
    font.setPixelSize(16);
    welcomeText->setFont(font);
    // Setup the last opened shortcut
    QLabel *lastOpenedText = new QLabel(tr("Last account opened:"), this);
    lastOpenButton = new QPushButton(this);
    lastOpenButton->setFlat(true);
    QString lastOpen = KCSettings::valueOf(QString("lastOpenedFile")).toString();
    if (lastOpen == NULL) {
        lastOpen = tr("No recent account has been opened");
        lastOpenButton->setEnabled(false);
    } else {
        lastOpen = KCFileManager::shortenedPath(40, lastOpen);
        lastOpenButton->setEnabled(true);
    }
    lastOpenButton->setText(lastOpen);
    // Connect signals/slots
    connect(create, SIGNAL(clicked()), this, SLOT(newAccount()));
    connect(open, SIGNAL(clicked()), this, SLOT(openAccount()));
    connect(lastOpenButton, SIGNAL(clicked()), this, SLOT(openLast()));
    // Setup layout
    QHBoxLayout *layout = new QHBoxLayout();
    QVBoxLayout *left = new QVBoxLayout();
    QVBoxLayout *right = new QVBoxLayout();
    left->addStretch(1);
    left->addWidget(create);
    left->addWidget(open);
    left->addStretch(1);
    right->addStretch(1);
    right->addWidget(welcomeText);
    right->addWidget(lastOpenedText);
    QGroupBox *box = new QGroupBox(this);
    box->setLayout(new QVBoxLayout());
    box->layout()->addWidget(lastOpenButton);
    right->addWidget(box);
    right->addStretch(1);
    layout->addLayout(left);
    layout->addLayout(right);
    layout->addStretch(1);
    // Apply layout and center the window
    this->setLayout(layout);
    this->adjustSize();
    KCCore::center(this);
    // Setup tabbing
    this->setTabOrder(create,open);
    this->setTabOrder(open,lastOpenButton);
}

KCWelcomeScreen::~KCWelcomeScreen()
{
    delete w;
}

void KCWelcomeScreen::openAccount()
{
    QString result = QFileDialog::getOpenFileName(this,
                        tr("Select the account file"),
                        QDir::home().absolutePath(),QString(tr("Account File (*.account)")));
    if (result != NULL) {
        this->hide();
        emit knowProject(result); // The user has selected a project
    }
}

void KCWelcomeScreen::newAccount()
{
    w = new KCNewAccount();
    // The newly created account will be opened right away
    connect(w, SIGNAL(accountSet(QString)), this, SIGNAL(knowProject(QString)));
    // If the user cancels the creation, brings back the welcome screen
    connect(w, SIGNAL(cancel()), this, SLOT(show()));
    // Center and displays the wizard
    KCCore::center(w);
    w->show();
    this->hide();
}

void KCWelcomeScreen::openLast()
{
    // ! Cannot retrieve the value from the button label as it
    // might have been shortened.
    this->hide();
    emit knowProject(KCSettings::valueOf(QString("lastOpenedFile")).toString());
}