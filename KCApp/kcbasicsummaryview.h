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

#include <kcsummaryview.h>
#include <QModelIndex>
#include <QObject>
#include <QSqlRecord>

QT_BEGIN_NAMESPACE
class QPainter;
class QListView;
class QPushButton;
class QSqlTableModel;
class QTextEdit;
class QWidget;
class QComboBox;
class QCheckBox;
QT_END_NAMESPACE

class KCBasicSummaryView : public QObject, public KCSummaryView
{
    /*
     * Provide a basic summary view
     */
    Q_OBJECT
    Q_INTERFACES(KCSummaryView)

public:
    explicit KCBasicSummaryView(QObject *parent = 0);
    // KCSummaryView interface
    QWidget* summaryView();
    const QString summaryName() const;
    QWidget* displayOptions();
    void setConnectionName(const QString& c);
    void setAccountProperties(const KCAccountProperties &properties);
    bool optionsUnder() const;
    void printSummary(QPrinter *printer);

signals:

public slots:
    // The user wants a complete summary of the account
    void backToGeneralView();
    // The user wants the details of an item of expense
    void selectPage(QModelIndex idx);
    // Ups  update the view to reflect the selection changes
    void refreshView();
    // The user wants to re-order the item of expense list
    void orderChanged(QString s);

protected:

private:
    // Display the content of r as the view
    void makeExpensePage(const QSqlRecord& r);
    // Display the general summary view
    void makeGeneralPage();
    // Display the individual balance page, will ignore r if global is
    // set to true
    void makeIndividualPage(bool global, const QSqlRecord& r);

    QWidget *optionsPanel;
    QPushButton *generalBtn;
    QListView *expenseList;
    QSqlTableModel *expenseModel;
    QTextEdit *view;
    QComboBox *orderBy;
    QString order;
    QCheckBox *individual;

    QString connection;
    KCAccountProperties properties;
    int minHeight;
    int minWidth;
    int selectedView;
};

#endif // KCBASICSUMMARYVIEW_H
