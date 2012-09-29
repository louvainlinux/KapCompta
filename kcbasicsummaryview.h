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

#include "interfaces.h"
#include <QModelIndex>
#include <QObject>

QT_BEGIN_NAMESPACE
class QPainter;
class QListView;
class QPushButton;
class QSqlTableModel;
class QSqlRecord;
class QTextEdit;
class QWidget;
QT_END_NAMESPACE

class KCBasicSummaryView : public QObject, public KCSummaryView
{
    Q_OBJECT
    Q_INTERFACES(KCSummaryView)

public:
    explicit KCBasicSummaryView(QObject *parent = 0);
    QWidget* summaryView();
    const QString& summaryName();
    QWidget* displayOptions();
    void setConnectionName(const QString& c);
    void setInitialBalance(int i);
    bool optionsUnder();
    void printSummary(QPrinter *printer);

signals:

public slots:
    void backToGeneralView();
    void selectPage(QModelIndex idx);
    void refreshView();

protected:

private:
    void makeExpensePage(QSqlRecord *r);
    void makeGeneralPage();

    QWidget *optionsPanel;
    QPushButton *generalBtn;
    QListView *expenseList;
    QSqlTableModel *expenseModel;
    QTextEdit *view;

    QString connection;
    int balance;
    int minHeight;
    int minWidth;
    int selectedView;
};

#endif // KCBASICSUMMARYVIEW_H
