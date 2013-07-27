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
#include "kccore.h"
#include "interfaces/kcpanel.h"
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

#define HELP_URL "http://github.com/louvainlinux/KapCompta/wiki"
#define TRANSITION_DURATION 175

class KCMainWindowPrivate
{
public:
    QList<KCPanel*> panels;
    QHash<QAction*, KCPanel*> actions;
    KCPanel* currentPanel;
    KCPanel* pendingPanel;
    // transition between panels
    QPropertyAnimation a_slideIn;
    QPropertyAnimation a_slideOut;
    QParallelAnimationGroup animation;

    KCMainWindowPrivate() : currentPanel(NULL), pendingPanel(NULL) {
        a_slideIn.setPropertyName("pos");
        a_slideIn.setDuration(TRANSITION_DURATION);
        a_slideIn.setEndValue(QPoint(0,0));
        a_slideOut.setPropertyName("pos");
        a_slideOut.setDuration(TRANSITION_DURATION);
        a_slideOut.setStartValue(QPoint(0,0));
        animation.addAnimation(&a_slideIn);
        animation.addAnimation(&a_slideOut);
    }
};

KCMainWindow::KCMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::KCMainWindow),
    d(new KCMainWindowPrivate)
{
    ui->setupUi(this);
    d->panels = QList<KCPanel*>(KCCore::instance()->panels());
    for (QList<KCPanel*>::iterator it = d->panels.begin(); it != d->panels.end(); ++it)
        loadPanel(*it);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(ui->mainToolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(toolbarTriggered(QAction*)));
    connect(&d->animation, SIGNAL(finished()), this, SLOT(transitionCompleted()));
}

KCMainWindow::~KCMainWindow()
{
    delete ui;
}

void KCMainWindow::loadPanel(KCPanel *p)
{
    QString iconPath;
    if (p->iconName().isEmpty()) iconPath = QString(":/icon/no.jpeg");
    else iconPath = p->iconName();
    QAction* action = ui->mainToolBar->addAction(QIcon(iconPath), p->panelName());
    d->actions.insert(action, p);
    p->panel()->adjustSize();
    p->panel()->setParent(ui->centralWidget);
    p->panel()->move(-p->panel()->width()-10,0);
}

void KCMainWindow::on_actionSettings_triggered()
{

}

void KCMainWindow::on_actionOnline_Documentation_triggered()
{
    QDesktopServices::openUrl(QUrl(QString(HELP_URL)));
}

void KCMainWindow::on_actionAbout_KapCompta_triggered()
{

}

void KCMainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}


void KCMainWindow::toolbarTriggered(QAction *a)
{
    KCPanel* panel = d->actions.value(a);
    d->pendingPanel = panel;
    panel->panel()->setVisible(true);
    if (!d->currentPanel) {
        qDebug() << "no current";
        d->pendingPanel->panel()->move(0,0);
        d->currentPanel = d->pendingPanel;
        d->currentPanel->selected();
        return;
    }
    d->currentPanel->unselected();
    d->a_slideIn.setTargetObject(panel->panel());
    d->a_slideIn.setStartValue(QPoint(-panel->panel()->width()-10,0));
    d->a_slideOut.setTargetObject(d->currentPanel->panel());
    d->a_slideOut.setEndValue(QPoint(this->width()+10,0));
    d->animation.start();
}

void KCMainWindow::transitionCompleted()
{
    d->currentPanel->panel()->setVisible(false);
    d->currentPanel = d->pendingPanel;
    d->currentPanel->selected();
}
