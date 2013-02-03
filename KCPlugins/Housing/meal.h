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

#ifndef MEAL_H
#define MEAL_H

#include <QWidget>
#include <QDate>
#include <kcpanel.h>

class MealCalendar;
class MealEditor;

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QLabel;
QT_END_NAMESPACE

class Meal : public QWidget, public KCPanel
{
    /*
     * Provide a KCPanel to encode meals
     **/
    Q_OBJECT
    Q_INTERFACES(KCPanel)

public:
    long ID() const {return 6;}
    explicit Meal(QWidget *parent = 0);
    ~Meal();
    // KCPanel interface
    void buildGUI(const QString& connection);
    void initDB(const QString& connection);
    QWidget* panel();
    const QString title() const;
    const QString iconPath() const;

signals:

public slots:
    void refreshCalendar(int month, int year);
    void selectPanel();
    void unselectPanel();
    void editDay(const QDate& day);
    void addMeal();
    void mealRemoved(MealEditor *editor);

private:
    void cleanPopup();

    QString connection;
    MealCalendar *calendar;
    QWidget *popup;
    QVBoxLayout *popupContentLayout;
    QLabel *popupDay;
    QDate day;
};

class MealEditor : public QWidget
{
    Q_OBJECT

public:
    explicit MealEditor(const QDate& day, const QString& connection, QWidget *parent = 0);
    MealEditor(const QDate& day, const QString& connection, int meal_id, QWidget *parent = 0);

signals:
    void removed();

private:
    void addRecord();
    void buildGUI();

    QDate day;
    QString connection;
    int meal_id;
};

#endif // MEAL_H
