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
#include <kcsettings.h>
#include <QVariant>
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
#include <QPluginLoader>
#include <QCloseEvent>
#include <kccore.h>
#include <kcplugin.h>
#include <kcsummaryview.h>
#include <kcfileformat.h>
#include <kcpanel.h>
#include <kcaccountsetting.h>

KCMain::KCMain(QWidget *parent)
    : QWidget(parent)
{
    QDir pluginsDir = KCCore::pluginsDir(qApp);
    // Load all plugins in the directory and add them to the list 'plugins'
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader loader(pluginsDir.filePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin) { // did we manage to load our plugin
            KCPlugin* p = qobject_cast<KCPlugin *>(plugin);
            if (p) { // is it a real KCPlugin ?
                plugins << p;
            }
        }
    }
}

KCMain::~KCMain()
{
}

void KCMain::start()
{
    welcomeScreen = new KCWelcomeScreen();
    // This class will stay idle in the background until a project is opened
    connect(welcomeScreen, SIGNAL(knowProject(QString)), this, SLOT(openProject(QString)));
    welcomeScreen->show();
}

void KCMain::openProject(QString p)
{
    projectPath = new QString(p);
    // update the lastOpenedFile field of the global settings
    KCSettings::setProperty(QString("lastOpenedFile"), QVariant(*projectPath));

    fileManager = new KCFileManager(*projectPath,this);
    if (fileManager->open()) { // Open the request project file
        QString accountName = QString(fileManager->value("General/accountName").toString());
        this->setWindowTitle("KapCompta - " + accountName + " - " + fileManager->shortenedPath(30));
        buildGUI();
        // We should not need that anymore
        delete welcomeScreen;
    } else { // We failed to open the file for some reason, bring back the welcome screen
        welcomeScreen->show();
    }
}

void KCMain::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;
    // Notify the current widget that its about to lose focus
    qobject_cast<KCPanel*>(sidePanel->currentWidget())->unselectPanel();
    // Change the displayed KCPanel
    sidePanel->setCurrentIndex(wList->row(current));
    // Notify the widget that it now has focus
    qobject_cast<KCPanel*>(sidePanel->currentWidget())->selectPanel();
    box->setTitle(qobject_cast<KCPanel*>(sidePanel->currentWidget())->title());
}

void KCMain::extractPluginList()
{
    // Current type of plugin contents we support
    QList<KCPanel*> pluginPanels;
    QList<KCSummaryView*> pluginSummaryViews;
    QList<KCAccountSetting*> pluginAccountSettings;
    QList<KCFileFormat*> pluginFileFormats;
    // Explore each loaded plugin
    QList<KCPlugin*>::const_iterator i;
    for (i = plugins.begin(); i != plugins.end(); ++i) {
        pluginPanels.append((*i)->panels());
        pluginSummaryViews.append((*i)->summaryView());
        pluginAccountSettings.append((*i)->accountSettings());
        pluginFileFormats.append((*i)->fileFormats());
    }
    KCPropertiesPanel *properties = new KCPropertiesPanel(fileManager, this);
    KCSummaryPanel *summary = new KCSummaryPanel(fileManager,this);
    KCPeoplePanel *people = new KCPeoplePanel(this);
    KCSpendingPanel *expenses = new KCSpendingPanel(this);
    KCTicketPanel *tickets = new KCTicketPanel(this);
    // Give the plugins to the views requiring them
    properties->setSettingList(pluginAccountSettings);
    summary->setSummaryViews(pluginSummaryViews);
    summary->setFileFormats(pluginFileFormats);
    // Add all KCPanel as our content
    panels << properties << summary << people << expenses << tickets;
    panels.append(pluginPanels);
}

void KCMain::buildGUI()
{
    extractPluginList();
    QHBoxLayout *layout = new QHBoxLayout();
    // Left side window list acting as menu
    wList = new QListWidget();
    wList->setViewMode(QListView::IconMode);
    wList->setIconSize(QSize(48, 48));
    wList->setMovement(QListView::Static);
    wList->setSpacing(12);
    // Check that each loaded KCPanel has had its chance to initialize the database
    foreach(KCPanel* p, panels) {
        bool initDB = fileManager->value(QString::number(p->ID()) + "dbFileHasBeenInitialized").toBool();
        if (!initDB) {
            p->initDB(fileManager->dbPath());
            fileManager->setValue(QString::number(p->ID()) + "dbFileHasBeenInitialized",QVariant(true));
        }
    }
    // The widget holding all our KCPanel
    sidePanel = new QStackedWidget();
    // Add all KCPanel to it
    foreach(KCPanel* p, panels) {
        p->buildGUI(fileManager->dbPath());
        sidePanel->addWidget(p->panel());
        // Add their name/picture to the menu
        QListWidgetItem *button = new QListWidgetItem(wList);
        button->setIcon(QIcon(p->iconPath()));
        button->setText(p->title());
        button->setTextAlignment(Qt::AlignHCenter);
        button->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    }
    // We start with the top widget
    wList->setCurrentRow(0);
    // We need to update the menu and the stack of KCPanel to stay sync'ed
    connect(wList,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
    // Setup left side layout
    QGroupBox *leftBox = new QGroupBox(this);
    QVBoxLayout *l = new QVBoxLayout();
    l->addWidget(wList);
    leftBox->setLayout(l);
    layout->addWidget(leftBox);
    wList->setMaximumWidth(200);
    wList->setMinimumWidth(175);
    wList->adjustSize();
    // Setup right side layout
    box = new QGroupBox(qobject_cast<KCPanel*>(sidePanel->currentWidget())->title(),this);
    QVBoxLayout *vBox = new QVBoxLayout();
    vBox->addWidget(sidePanel);
    box->setLayout(vBox);
    layout->addWidget(box,1);
    // Setup layout, center and display
    this->setLayout(layout);
    this->adjustSize();
    KCCore::center(this);
    this->show();
}

void KCMain::closeEvent(QCloseEvent *event)
{
    // Let's not lose data if for some reason the user choosed to close the window
    this->saveAllPanels();
    // And allow the closing of the window
    event->accept();
}

void KCMain::saveAllPanels()
{
    foreach (KCPanel *p, panels) {
        p->saveAll();
    }
}
