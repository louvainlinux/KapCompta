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

#include "mealpanel.h"
#include "ui_meal.h"
#include "ui_mealsubscription.h"
#include <QtWidgets/QWidget>
#include <QDialog>

class MealPanelPrivate {
public:
    QWidget* widget;
    QDialog* dialog;

    MealPanelPrivate() : widget(new QWidget), dialog(new QDialog) {}
};

MealPanel::MealPanel(QWidget *parent) :
    KCPanel(parent),
    ui(new Ui::Meal),
    dialog(new Ui::MealSubscription),
    d(new MealPanelPrivate)
{
    ui->setupUi(d->widget);
    dialog->setupUi(d->dialog);
    d->dialog->hide();
    d->dialog->setModal(true);
    d->dialog->setWindowTitle(tr("Register a subscription to a meal"));
    connect(ui->add, SIGNAL(clicked()), d->dialog, SLOT(show()));
    connect(dialog->cancel, SIGNAL(clicked()), d->dialog, SLOT(hide()));
}

MealPanel::~MealPanel()
{
    delete ui;
}

const QString MealPanel::panelName()
{
    return "Meal management";
}

QWidget* MealPanel::panel()
{
    return d->widget;
}

const QString MealPanel::iconName()
{
    return QString(":/icon/meal");
}
