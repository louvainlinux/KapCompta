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

#ifndef PEOPLEPANEL_H
#define PEOPLEPANEL_H

#include "interfaces/kcpanel.h"

namespace Ui {
class People;
class AddPerson;
}

class PeoplePanelPrivate;

class PeoplePanel : public KCPanel
{
    Q_OBJECT
public:
    explicit PeoplePanel(KCAccountFile *account, QWidget *parent = 0);
    ~PeoplePanel();
    const QString panelName();
    QWidget* panel();
    const QString iconName();

signals:

public slots:

private:
    Ui::People *ui;
    Ui::AddPerson *addP;
    PeoplePanelPrivate *d;
};

#endif // PEOPLEPANEL_H
