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

#ifndef KCMAINWINDOW_H
#define KCMAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QCloseEvent;
class QAction;
QT_END_NAMESPACE

class KCPanel;
class KCMainWindowPrivate;

namespace Ui {
class KCMainWindow;
}

class KCMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit KCMainWindow(const QString& account, QWidget *parent = 0);
    ~KCMainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::KCMainWindow *ui;
    KCMainWindowPrivate *d;

private slots:
    void loadPanel(KCPanel*);
    void on_actionSettings_triggered();
    void on_actionOnline_Documentation_triggered();
    void on_actionAbout_KapCompta_triggered();
    void toolbarTriggered(QAction*);
    void transitionCompleted();
    void on_actionAbout_Qt_triggered();
};

#endif // KCMAINWINDOW_H
