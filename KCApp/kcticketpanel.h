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

#ifndef KCTICKETPANEL_H
#define KCTICKETPANEL_H

#include <QWidget>
#include <kcpanel.h>

QT_BEGIN_NAMESPACE
class QListView;
class QSqlRelationalTableModel;
class QSqlTableModel;
class QDataWidgetMapper;
QT_END_NAMESPACE

class KCTicketPanel : public QWidget, public KCPanel
{
    Q_OBJECT
    Q_INTERFACES(KCPanel)

public:
    long ID() const {return 5;}
    explicit KCTicketPanel(QWidget *parent = 0);
    ~KCTicketPanel();
    // KCPanel interface
    QWidget* panel();
    const QString title() const;
    const QString iconPath() const;
    void buildGUI(const QString &connection);
    void initDB(const QString& connection);

public slots:
    void selectPanel();
    void unselectPanel();
    void saveAll();

signals:

private slots:
    // The user pressed the + button
    void addEntry();
    // The user pressed the - button
    void removeEntry();
    // Propagate the index changes to the QDWM from the listView
    void setCurrentModelIndex();
    // The user changed the filtering through the dropdown button
    void filterChanged(QString s);

private:
    QListView *listView;
    QSqlRelationalTableModel *model;
    QSqlTableModel *personRel;
    QSqlTableModel *expenseRel;
    QDataWidgetMapper *mapper;
    QString connectionName;
    int personid;
    int expenseid;
};

#endif // KCTICKETPANEL_H
