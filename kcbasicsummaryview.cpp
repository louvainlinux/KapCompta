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
#include <QPainter>
#include <QPrinter>

KCBasicSummaryView::KCBasicSummaryView(QWidget *parent) :
    QWidget(parent)
{
    connection = QString("");
}

QWidget* KCBasicSummaryView::summaryView()
{
    return this;
}

void KCBasicSummaryView::setInitialBalance(int i)
{
    initialBalance = i;
    this->update();
}

const QString& KCBasicSummaryView::summaryName()
{
    static QString s = tr("Default Summary view");
    return s;
}

QWidget* KCBasicSummaryView::displayOptions(){
    return new QWidget();
}

bool KCBasicSummaryView::optionsUnder()
{
    return false;
}

void KCBasicSummaryView::setConnectionName(const QString& c)
{
    connection = QString(c);
    this->update();
}

void KCBasicSummaryView::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    this->paint(&painter, event);
    painter.end();
}

void KCBasicSummaryView::paint(QPainter *painter, QPaintEvent *event)
{
    Q_UNUSED(event);

    QString test;
    if (connection.compare(QString(""))) {
        test = "pas de connection";
    } else {
        test = connection;
    }

    painter->drawText(0,0,test);
}

void KCBasicSummaryView::printSummary(QPrinter *printer)
{

}
