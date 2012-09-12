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

#include "kcsummarypanel.h"
#include "kcfilemanager.h"
#include "kcbasicsummaryview.h"
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QStringListModel>

KCSummaryPanel::KCSummaryPanel(KCFileManager *fm, QWidget *parent) :
    QWidget(parent)
{
    this->fm = fm;
    views << new KCBasicSummaryView(this);
}

void KCSummaryPanel::selectPanel()
{
    int b = fm->value("General/accountBalance").toInt();
    if (b != balance) {
        QList<KCSummaryView*>::iterator i;
        for (i = views.begin(); i != views.end(); ++i) {
            (*i)->setInitialBalance(b);
        }
        balance = b;
    }
}

void KCSummaryPanel::initDB(const QString& connection)
{
    Q_UNUSED(connection)
}

QWidget* KCSummaryPanel::panel()
{
    return this;
}

const QString& KCSummaryPanel::title()
{
    static QString s = QString(tr("Account summary"));
    return s;
}

const QString& KCSummaryPanel::iconPath()
{
    static QString s = QString(":/icons/summary");
    return s;
}

void KCSummaryPanel::buildGUI(const QString &connection)
{
    balance = fm->value("General/accountBalance").toInt();
    QStackedWidget *stack = new QStackedWidget(this);
    QStringList names;

    QList<KCSummaryView*>::iterator i;
    for (i = views.begin(); i != views.end(); ++i) {
        (*i)->setInitialBalance(balance);
        (*i)->setConnectionName(connection);

        QWidget *page = new QWidget(stack);

        names << (*i)->summaryName();
        QGroupBox *options = new QGroupBox(tr("Options"),page);
        QVBoxLayout *optLayout = new QVBoxLayout();
        optLayout->addWidget((*i)->displayOptions());
        options->setLayout(optLayout);

        QScrollArea *scroll = new QScrollArea(stack);
        scroll->setWidget((*i)->summaryView());
        scroll->setWidgetResizable(false);

        if ((*i)->optionsUnder()) {
            QHBoxLayout *optionsL = new QHBoxLayout();
            optionsL->addStretch(1);
            optionsL->addWidget(options);
            optionsL->addStretch(1);

            QHBoxLayout *summaryL = new QHBoxLayout();
            summaryL->addWidget(scroll);

            QVBoxLayout *vLayout = new QVBoxLayout();
            vLayout->addLayout(summaryL,1);
            vLayout->addLayout(optionsL);

            page->setLayout(vLayout);
        } else {
            QHBoxLayout *hLayout = new QHBoxLayout();
            hLayout->addWidget(options);
            hLayout->addWidget(scroll,1);
            page->setLayout(hLayout);
        }
        stack->addWidget(page);
    }

    QComboBox *selectView = new QComboBox(this);
    QStringListModel *selectModel = new QStringListModel(this);
    QHBoxLayout *selectL = new QHBoxLayout();
    QFormLayout *selectF = new QFormLayout();
    selectF->addRow(tr("View type:"), selectView);
    selectL->addLayout(selectF);
    selectModel->setStringList(names);
    selectView->setModel(selectModel);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *exportBtn = new QPushButton(tr("Export"),this);
    QPushButton *printBtn = new QPushButton(tr("Print"),this);
    btnLayout->addWidget(exportBtn);
    //btnLayout->addStretch(1);
    btnLayout->addWidget(printBtn);

    QVBoxLayout *vBox = new QVBoxLayout();
    vBox->addLayout(selectL);
    vBox->addWidget(stack);
    vBox->addLayout(btnLayout);
    this->setLayout(vBox);

    stack->setCurrentIndex(0);

    connect(selectView, SIGNAL(currentIndexChanged(int)), stack, SLOT(setCurrentIndex(int)));
    connect(exportBtn, SIGNAL(clicked()), this, SLOT(exportView()));
    connect(printBtn, SIGNAL(clicked()), this, SLOT(printSummaryView()));
}

void KCSummaryPanel::printSummaryView()
{

}

void KCSummaryPanel::exportView()
{

}
