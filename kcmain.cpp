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
#include <QGroupBox>

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
    bool open = fileManager->open();
    if (open) {
        QString accountName = QString(fileManager->value("General/accountName").toString());
        this->setWindowTitle("KapCompta - " + accountName + " - " + *projectPath);
        buildGUI();
        delete welcomeScreen;
    }
}

void KCMain::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;
    qobject_cast<KCPanel*>(sidePanel->currentWidget())->unselectPanel();
    sidePanel->setCurrentIndex(wList->row(current));
    qobject_cast<KCPanel*>(sidePanel->currentWidget())->selectPanel();
    box->setTitle(qobject_cast<KCPanel*>(sidePanel->currentWidget())->title());
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
    panels << new KCSummaryPanel(fileManager,this) << new KCPropertiesPanel(fileManager, this)
           << new KCPeoplePanel(this) << new KCSpendingPanel(this)
           << new KCTicketPanel(this);
    QList<KCPanel*>::const_iterator iterator;
    bool initDB = fileManager->value("dbFileHasBeenInitialized").toBool();
    for(iterator = panels.begin(); iterator != panels.end(); ++iterator) {
        if (!initDB) {
            (*iterator)->initDB(fileManager->dbPath());
        }
        (*iterator)->buildGUI(fileManager->dbPath());
        sidePanel->addWidget((*iterator)->panel());
        QListWidgetItem *button = new QListWidgetItem(wList);
        button->setIcon(QIcon((*iterator)->iconPath()));
        button->setText((*iterator)->title());
        button->setTextAlignment(Qt::AlignHCenter);
        button->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    }
    if (!initDB) {
        fileManager->setValue("dbFileHasBeenInitialized",QVariant(true));
    }
    wList->setCurrentRow(0);
    connect(wList,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
    QGroupBox *leftBox = new QGroupBox(this);
    QVBoxLayout *l = new QVBoxLayout();
    l->addWidget(wList);
    leftBox->setLayout(l);
    layout->addWidget(leftBox);
    wList->setMaximumWidth(200);
    wList->adjustSize();
    const QRect screen = QApplication::desktop()->screenGeometry();
    box = new QGroupBox(qobject_cast<KCPanel*>(sidePanel->currentWidget())->title(),this);
    QVBoxLayout *vBox = new QVBoxLayout();
    vBox->addWidget(sidePanel);
    box->setLayout(vBox);
    layout->addWidget(box,1);
    this->setLayout(layout);
    this->adjustSize();
    this->move(screen.center() - this->rect().center());
    this->show();
    //this->showMaximized();
}
