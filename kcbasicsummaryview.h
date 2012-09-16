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

#ifndef KCBASICSUMMARYVIEW_H
#define KCBASICSUMMARYVIEW_H

#include <QWidget>
#include "interfaces.h"
#include <QList>

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class KCBasicSummaryView : public QWidget, public KCSummaryView
{
    Q_OBJECT
    Q_INTERFACES(KCSummaryView)

public:
    explicit KCBasicSummaryView(QWidget *parent = 0);
    QWidget* summaryView();
    const QString& summaryName();
    QWidget* displayOptions();
    void setConnectionName(const QString& c);
    void setInitialBalance(int i);
    bool optionsUnder();
    void printSummary(QPrinter *printer);
signals:
    
public slots:
    
protected:

private:
    QList<QWidget*> pages;
    QWidget *optionsPanel;
    QString connection;
    QLabel *balanceField;

};

#endif // KCBASICSUMMARYVIEW_H
