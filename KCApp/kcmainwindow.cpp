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

#include "kcmainwindow.h"
#include "ui_kcmainwindow.h"
#include "startupdialog.h"
#include <kcglobals.h>
#include <kccore.h>
#include <interfaces/kcpanel.h>
#include <kcaccountfile.h>
#include <QList>
#include <QDesktopServices>
#include <QUrl>
#include <QCloseEvent>
#include <QHash>
#include <QAction>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QPoint>
#include <QtDebug>
#include <QSettings>
#include <QActionGroup>

static const QString WINDOW_FRAME_KEY = QString("default/kcmainwindow_geometry");
static const QString CURRENT_PANEL_KEY = QString("default/kcmainwindow_panel_index");
static const QUrl HELP_URL = QUrl("http://github.com/louvainlinux/KapCompta/wiki");

#define TRANSITION_DURATION 175
#define STATUS_DURATION 5000

class KCMainWindowPrivate
{
public:
    KCAccountFile *account;
    QList<KCPanel*> panels;
    QHash<QAction*, KCPanel*> actions;
    KCPanel* currentPanel;
    KCPanel* pendingPanel;
    // transition between panels
    QPropertyAnimation *a_slideIn;
    QPropertyAnimation *a_slideOut;
    QParallelAnimationGroup animation;

    KCMainWindowPrivate(KCAccountFile *accountPath) :
        account(accountPath),
        currentPanel(NULL),
        pendingPanel(NULL),
        a_slideIn(new QPropertyAnimation),
        a_slideOut(new QPropertyAnimation)
    {
        a_slideIn->setPropertyName("pos");
        a_slideIn->setDuration(TRANSITION_DURATION);
        a_slideIn->setEasingCurve(QEasingCurve::InOutCubic);
        a_slideOut->setPropertyName("pos");
        a_slideOut->setDuration(TRANSITION_DURATION);
        a_slideOut->setEasingCurve(QEasingCurve::InOutCubic);
        animation.addAnimation(a_slideIn);
        animation.addAnimation(a_slideOut);
    }

    ~KCMainWindowPrivate()
    {
        delete account;
        qDeleteAll(panels);
        qDeleteAll(actions.keys());
    }
};

KCMainWindow::KCMainWindow(KCAccountFile* account, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::KCMainWindow),
    d(new KCMainWindowPrivate(account))
{
    // ensures that this window is delete when closed
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    d->panels = QList<KCPanel*>(KCCore::instance()->panels(account));
    // Prepare the group which will render all navigation actions exclusive from each other
    QActionGroup *aGroup = new QActionGroup(this);
    // load all panels
    for (QList<KCPanel*>::iterator it = d->panels.begin(); it != d->panels.end(); ++it)
        loadPanel(*it, aGroup);
    for (QList<KCPanel*>::iterator it = d->panels.begin(); it != d->panels.end(); ++it)
        ((KCPanel*)*it)->allPanelsCreated();
    // load the dta from the last session if available regarding the window size
    QSettings s;
    QRect r = s.value(WINDOW_FRAME_KEY).toRect();
    if (r != QRect(0,0,0,0)) this->setGeometry(r);
    // And the last selected panel
    int oldPanelIndex = s.value(CURRENT_PANEL_KEY, 0).toInt();
    if (oldPanelIndex < 0 || oldPanelIndex >= d->panels.size())
        oldPanelIndex = 0;
    d->currentPanel = d->panels.at(oldPanelIndex);
    d->currentPanel->panel()->setVisible(true);
    d->currentPanel->selected();
    d->actions.key(d->currentPanel)->setChecked(true);
    this->setWindowTitle(d->account->getProperty(PROPERTY_ACCOUNT_NAME).toString()
                         + " - " + d->currentPanel->panelName());
    connect(ui->mainToolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(toolbarTriggered(QAction*)));
    connect(&d->animation, SIGNAL(finished()), this, SLOT(transitionCompleted()));
    connect(KCCore::instance(), SIGNAL(statusUpdate(QString,int)), ui->statusBar, SLOT(showMessage(QString,int)));
    ui->statusBar->showMessage(tr("Successfuly opened [%1]").arg(account->fileName()), STATUS_DURATION);
}

KCMainWindow::~KCMainWindow()
{
    delete ui;
    delete d;
}

void KCMainWindow::loadPanel(KCPanel *p, QActionGroup *aGroup)
{
    QString iconPath;
    // assign the proper icon to the panel
    if (p->iconName().isEmpty()) iconPath = QString(":/icon/no.jpeg");
    else iconPath = p->iconName();
    // add a navigation entry for it
    QAction* action = new QAction(p->panelName(), aGroup);
    action->setIcon(QIcon(iconPath));
    action->setCheckable(true);
    ui->mainToolBar->addAction(action);
    d->actions.insert(action, p);
    ui->menuPanels->addAction(action);
    // pack and add the panel to the main window
    p->panel()->adjustSize();
    ui->centralWidget->layout()->addWidget(p->panel());
    p->panel()->setVisible(false);
}

void KCMainWindow::on_actionSettings_triggered()
{

}

void KCMainWindow::on_actionOnline_Documentation_triggered()
{
    QDesktopServices::openUrl(HELP_URL);
}

void KCMainWindow::on_actionAbout_KapCompta_triggered()
{

}

void KCMainWindow::closeEvent(QCloseEvent *event)
{
    QSettings s;
    // Save the settings regarding the window size and the current panel
    s.setValue(WINDOW_FRAME_KEY, this->geometry());
    s.setValue(CURRENT_PANEL_KEY, d->panels.indexOf(d->currentPanel));
    s.sync();
    // notify the current panel that we're leaving it
    d->currentPanel->unselected();
    // TODO: options for autosave ?
    d->account->save();
    // allow the window to close
    event->accept();
}


void KCMainWindow::toolbarTriggered(QAction *a)
{
    KCPanel* panel = d->actions.value(a);
    if (panel == d->currentPanel) return;
    // disable the layout manager to let us organize the widget ourselves
    ui->centralWidget->layout()->setEnabled(false);
    panel->panel()->resize(d->currentPanel->panel()->size());
    d->pendingPanel = panel;
    panel->panel()->setVisible(true);
    d->currentPanel->unselected();
    // animate the transition between the two panels
    d->a_slideIn->setTargetObject(panel->panel());
    d->a_slideIn->setStartValue(QPoint(-panel->panel()->width(),d->currentPanel->panel()->pos().y()));
    d->a_slideIn->setEndValue(d->currentPanel->panel()->pos());
    d->a_slideOut->setTargetObject(d->currentPanel->panel());
    d->a_slideOut->setEndValue(QPoint(this->width(),d->currentPanel->panel()->pos().y()));
    d->animation.start();
}

void KCMainWindow::transitionCompleted()
{
    d->currentPanel->panel()->setVisible(false);
    d->currentPanel = d->pendingPanel;
    d->currentPanel->selected();
    ui->centralWidget->layout()->setEnabled(true);
    const QString pName = d->currentPanel->panelName();
    this->setWindowTitle(d->account->getProperty(PROPERTY_ACCOUNT_NAME).toString()
                         + " - " + pName);
    ui->statusBar->showMessage(tr("Switched to %1").arg(pName), STATUS_DURATION);
}

void KCMainWindow::on_actionAbout_Qt_triggered()
{
    QApplication::aboutQt();
}

void KCMainWindow::on_actionOpen_Create_account_triggered()
{
    StartupDialog* s = new StartupDialog();
    s->show();
}

void KCMainWindow::on_actionSave_triggered()
{
    d->account->save();
    ui->statusBar->showMessage(tr("Changes to [%1] have been saved").arg(d->account->fileName()),
                               STATUS_DURATION);
}

void KCMainWindow::on_actionClose_triggered()
{
    this->close();
}
