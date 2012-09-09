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

#include "kcmain.h"
#include "kcwelcomescreen.h"
#include "kcsettings.h"
#include <QVariant>
#include "interfaces.h"
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QListWidget>
#include <QStackedWidget>
#include "kcsummarypanel.h"
#include "kcticketpanel.h"
#include "kcpeoplepanel.h"
#include "kcspendingpanel.h"
#include "kcpropertiespanel.h"
#include <QListWidgetItem>
#include <QApplication>
#include "kcfilemanager.h"
#include "kcdatabasehelper.h"


KCMain::KCMain(QWidget *parent)
    : QWidget(parent)
{    
}

KCMain::~KCMain()
{
    
}

void KCMain::start()
{
    welcomeScreen = new KCWelcomeScreen();
    connect(welcomeScreen, SIGNAL(knowProject(QString)), this, SLOT(openProject(QString)));
    welcomeScreen->show();
}

void KCMain::openProject(QString p)
{
    projectPath = new QString(p);
    KCSettings::setProperty(QString("lastOpenedFile"), QVariant(*projectPath));
    fileManager = new KCFileManager(*projectPath,this);
    fileManager->open();
    QString accountName = QString(fileManager->value("general/accountName").toString());
    this->setWindowTitle("KapCompta - " + accountName);
    buildGUI();
    delete welcomeScreen;
}

void KCMain::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    sidePanel->setCurrentIndex(wList->row(current));
}

void KCMain::buildGUI()
{
    QHBoxLayout *layout = new QHBoxLayout();
    wList = new QListWidget();
    wList->setViewMode(QListView::IconMode);
    wList->setIconSize(QSize(48, 48));
    wList->setMovement(QListView::Static);
    wList->setSpacing(12);
    sidePanel = new QStackedWidget();
    QList<KCPanel*> panels;
    panels << new KCSummaryPanel(this) << new KCPropertiesPanel(this)
           << new KCPeoplePanel(this) << new KCSpendingPanel(this)
           << new KCTicketPanel(this);
    QList<KCPanel*>::const_iterator iterator;
    for(iterator = panels.begin(); iterator != panels.end(); ++iterator) {

        (*iterator)->buildGUI(fileManager->dbPath());
        sidePanel->addWidget((*iterator)->panel());
        QListWidgetItem *button = new QListWidgetItem(wList);
        button->setIcon(QIcon((*iterator)->iconPath()));
        button->setText((*iterator)->title());
        button->setTextAlignment(Qt::AlignHCenter);
        button->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    }
    wList->setCurrentRow(0);
    connect(wList,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
    layout->addWidget(wList);
    wList->setMaximumWidth(175);
    layout->addWidget(sidePanel,1);
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->setLayout(layout);
    this->move(screen.center() - this->rect().center());
    this->showMaximized();
}
