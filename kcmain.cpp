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
#include <QDir>
#include <QPluginLoader>
#include <QCloseEvent>

KCMain::KCMain(QWidget *parent)
    : QWidget(parent)
{
    QDir pluginsDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    pluginsDir.cd("plugins");

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin) {
            KCPlugin* p = qobject_cast<KCPlugin *>(plugin);
            if (p) plugins << p;
        }
    }
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

    if (fileManager->open()) {
        int maxLength = 30;
        if (projectPath->length() > maxLength) {
            int i = maxLength/2;
            projectPath->remove(i, projectPath->length() - maxLength);
            projectPath->insert(i,"[...]");
        }
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

void KCMain::extractPluginList()
{
    QList<KCPanel*> pluginPanels;
    QList<KCSummaryView*> pluginSummaryViews;
    QList<KCAccountSetting*> pluginAccountSettings;
    QList<KCFileFormat*> pluginFileFormats;

    QList<KCPlugin*>::const_iterator i;
    for (i = plugins.begin(); i != plugins.end(); ++i) {
        pluginPanels.append((*i)->panels());
        pluginSummaryViews.append((*i)->summaryView());
        pluginAccountSettings.append((*i)->accountSettings());
        pluginFileFormats.append((*i)->fileFormats());
    }

    KCPropertiesPanel *properties = new KCPropertiesPanel(fileManager, this);
    properties->setSettingList(pluginAccountSettings);
    KCSummaryPanel *summary = new KCSummaryPanel(fileManager,this);
    summary->setSummaryViews(pluginSummaryViews);
    summary->setFileFormats(pluginFileFormats);
    KCPeoplePanel *people = new KCPeoplePanel(this);
    KCSpendingPanel *expenses = new KCSpendingPanel(this);
    KCTicketPanel *tickets = new KCTicketPanel(this);
    panels << properties << summary << people << expenses << tickets;
    panels.append(pluginPanels);
}

void KCMain::buildGUI()
{
    extractPluginList();
    QHBoxLayout *layout = new QHBoxLayout();

    wList = new QListWidget();
    wList->setViewMode(QListView::IconMode);
    wList->setIconSize(QSize(48, 48));
    wList->setMovement(QListView::Static);
    wList->setSpacing(12);

    sidePanel = new QStackedWidget();

    QList<KCPanel*>::const_iterator iterator;
    bool initDB = fileManager->value("dbFileHasBeenInitialized").toBool();
    if (!initDB) {
        for(iterator = panels.begin(); iterator != panels.end(); ++iterator) {
            (*iterator)->initDB(fileManager->dbPath());
        }
        fileManager->setValue("dbFileHasBeenInitialized",QVariant(true));
    }
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

    QGroupBox *leftBox = new QGroupBox(this);
    QVBoxLayout *l = new QVBoxLayout();
    l->addWidget(wList);
    leftBox->setLayout(l);
    layout->addWidget(leftBox);
    wList->setMaximumWidth(200);
    wList->setMinimumWidth(175);
    wList->adjustSize();

    box = new QGroupBox(qobject_cast<KCPanel*>(sidePanel->currentWidget())->title(),this);
    QVBoxLayout *vBox = new QVBoxLayout();
    vBox->addWidget(sidePanel);
    box->setLayout(vBox);
    layout->addWidget(box,1);
    this->setLayout(layout);

    this->adjustSize();
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move(screen.center() - this->rect().center());
    this->show();
}

void KCMain::closeEvent(QCloseEvent *event)
{
    this->saveAllPanels();
    event->accept();

}

void KCMain::saveAllPanels()
{
    qDebug("save");
    QList<KCPanel*>::const_iterator i;
    for (i = panels.begin(); i != panels.end(); ++i) {
        (*i)->saveAll();
    }
}
