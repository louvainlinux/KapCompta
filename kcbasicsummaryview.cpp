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
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QCheckBox>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QSqlTableModel>
#include "kcdatabasehelper.h"
#include <QColor>
#include <QRect>
#include <QSqlQuery>

KCBasicSummaryView::KCBasicSummaryView(QWidget *parent) :
    QWidget(parent)
{
    this->setLayout(new QVBoxLayout());
    optionsPanel = new QWidget(this);
    optionsPanel->setLayout(new QVBoxLayout());
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
    QRect pageRect = printer->pageRect();
    pageRect.moveTo(0,0);
    // BUGFIX: by default Qt adds twice the margin to pageRect resulting
    // in undrawed areas on the borders ...
    // see: http://stackoverflow.com/questions/915775/qt-printing-pagerect-and-paperrect-issues

    QList<QWidget*>::iterator i = pages.begin();
    while (i != pages.end())
    {
        QWidget *currentPrintView = qobject_cast<QWidget*>(*i);

        double xscale = printer->pageRect().width()/double(currentPrintView->width());
        double yscale = printer->pageRect().height()/double(currentPrintView->height());
        double scale = qMin(xscale, yscale);
        painter.translate(printer->paperRect().x() + printer->pageRect().width()/2,
                          printer->paperRect().y() + printer->pageRect().height()/2);
        painter.scale(scale, scale);
        painter.translate(- currentPrintView->width()/2, - currentPrintView->height()/2);

        currentPrintView->render(&painter);
        if (currentPrintView != *(pages.end())) {
            printer->newPage();
        }
        ++i;
    }

    painter.end();
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
    qDebug("refresh");
    this->clear(optionsPanel->layout(),true);
    this->clear(this->layout(),true);
    delete this->layout();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);

    qDebug(connection.toAscii());
    QSqlQuery query(QSqlDatabase::database(connection));
    if (query.exec("SELECT id, name FROM expenses ORDER BY name ASC")) {
        while (query.next()) {


        }

        mainLayout->addWidget(new QLabel(tr("Initial balance: ") + QString::number(balance)));
        mainLayout->addWidget(new QLabel(tr("Current balance: ")
                                         + QString::number(
                                             KCDataBaseHelper::sumAllExpenses(connection))));
        mainLayout->addStretch(1);

    } else {
        QLabel *nothing = new QLabel(tr("Nothing to display!"), this);
        mainLayout->addWidget(nothing);
    }
}
