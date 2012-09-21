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

#include "kcbasicsummaryview.h"
#include <QPrinter>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QCheckBox>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QSqlTableModel>
#include "kcdatabasehelper.h"
#include <QRect>
#include <QSqlQuery>
#include <QTabWidget>

KCBasicSummaryView::KCBasicSummaryView(QWidget *parent) :
    QWidget(parent)
{
    optionsPanel = new QWidget(this);
    optionsPanel->setLayout(new QVBoxLayout());
    pagesTab = new QTabWidget(this);
    mainLayout = new QWidget(this);
    mainLayout->setLayout(new QVBoxLayout());
    pagesTab->addTab(mainLayout,tr("General"));
    this->setLayout(new QVBoxLayout());
    this->layout()->addWidget(pagesTab);
}

QWidget* KCBasicSummaryView::summaryView()
{
    return this;
}

void KCBasicSummaryView::setInitialBalance(int i)
{
    balance = i;
    refresh();
}

const QString& KCBasicSummaryView::summaryName()
{
    static QString s = tr("Default Summary view");
    return s;
}

QWidget* KCBasicSummaryView::displayOptions()
{
    return optionsPanel;
}

bool KCBasicSummaryView::optionsUnder()
{
    return false;
}

void KCBasicSummaryView::setConnectionName(const QString& c)
{
    connection = QString(c);
}

void KCBasicSummaryView::printSummary(QPrinter *printer)
{
    QPainter painter;
    painter.begin(printer);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    renderPage(mainLayout, &painter, printer);
    if (pages.count()) {
        printer->newPage();
    }

    QList<QWidget*>::iterator i = pages.begin();
    while (i != pages.end())
    {
        QWidget *currentPrintView = qobject_cast<QWidget*>(*i);
        renderPage(currentPrintView, &painter, printer);
        if (currentPrintView != *(pages.end())) {
            printer->newPage();
        }
        ++i;
    }

    painter.end();
}

void KCBasicSummaryView::renderPage(QWidget *page, QPainter* painter, QPrinter *printer)
{
    QRect pageRect = printer->pageRect();
    pageRect.moveTo(0,0);
    // BUGFIX: by default Qt adds twice the margin to pageRect resulting
    // in undrawed areas on the borders ...
    // see: http://stackoverflow.com/questions/915775/qt-printing-pagerect-and-paperrect-issues

    double xscale = printer->pageRect().width()/double(page->width());
    double yscale = printer->pageRect().height()/double(page->height());
    double scale = qMin(xscale, yscale);
    painter->translate(printer->paperRect().x() + printer->pageRect().width()/2,
                       printer->paperRect().y() + printer->pageRect().height()/2);
    painter->scale(scale, scale);
    painter->translate(- page->width()/2, - page->height()/2);

    page->render(painter);
}

void KCBasicSummaryView::clear(QLayout* layout, bool deleteWidgets)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (deleteWidgets)
        {
            if (QWidget* widget = item->widget())
                delete widget;
        }
        else if (QLayout* childLayout = item->layout())
            clear(childLayout, deleteWidgets);
        delete item;
    }
}

void KCBasicSummaryView::refresh()
{
    this->clear(optionsPanel->layout(),true);
    this->clear(mainLayout->layout(),true);
    //pagesTab->clear();
    pages.clear();
    // memory leak here !

    QSqlQuery query(QSqlDatabase::database(connection));
    if (query.exec("SELECT id, name FROM expenses ORDER BY name ASC")) {
        QFormLayout *fl = new QFormLayout();
        fl->addRow(tr("<b>Income</b>"), new QLabel(tr("<b>Expense</b>"),this));
        qobject_cast<QVBoxLayout*>(mainLayout->layout())->addLayout(fl);
        while (query.next()) {
            QWidget *p = makeExpensePage(query.value(0).toInt(), query.value(1).toString());
            pages.append(p);
            pagesTab->addTab(p,query.value(1).toString());
            mainLayout->layout()->addWidget(new QLabel("<b><center>"+query.value(1).toString()+" : "+
                                                       QString::number(
                                                           KCDataBaseHelper::sumExpenses(connection, query.value(0).toInt()))
                                                       +"</center></b>",this));
            QFormLayout *f = new QFormLayout();
            f->addRow(QString::number(
                          KCDataBaseHelper::sumPositiveExpenses(connection, query.value(0).toInt())),
                      new QLabel(QString::number(
                                     KCDataBaseHelper::sumNegativeExpenses(connection, query.value(0).toInt())), this));
            qobject_cast<QVBoxLayout*>(mainLayout->layout())->addLayout(f);
        }

        mainLayout->layout()->addWidget(new QLabel(tr("Initial balance: ") + QString::number(balance)));
        mainLayout->layout()->addWidget(new QLabel(tr("Current balance: ")
                                                   + QString::number(
                                                       KCDataBaseHelper::sumAllExpenses(connection))));
        qobject_cast<QVBoxLayout*>(mainLayout->layout())->addStretch(1);
        pagesTab->adjustSize();
    } else {
        QLabel *nothing = new QLabel(tr("Nothing to display!"), this);
        mainLayout->layout()->addWidget(nothing);
    }
}

QWidget* KCBasicSummaryView::makeExpensePage(int expense_id, const QString& name)
{
    QWidget *w = new QWidget(this);
    QSqlQuery query(QSqlDatabase::database(connection));

    return w;
}
