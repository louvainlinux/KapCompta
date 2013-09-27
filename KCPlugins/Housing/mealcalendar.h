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

#ifndef MEALCALENDAR_H
#define MEALCALENDAR_H

#include <QCalendarWidget>
#include <QHash>

class MealCalendar : public QCalendarWidget
{
    /*
     * Customise the basic calendar widget to color cells containing meals
     **/
    Q_OBJECT
public:
    explicit MealCalendar(QWidget *parent = 0);
    /*
     * Gives an hashmap where keys are day number in the month (thus 1-31)
     * and values are the number of highlights on that day
     **/
    void setCurrentMonthHighlights(const QHash<int,int>& highlights);
    /**
     * @brief setHighlightsForDay set the number of highlights for the given day
     * @param day the day of the month (1-31)
     * @param highlights the number of highlights for that day
     */
    void setHighlightsForDay(int day, int highlights);
signals:

public slots:

protected:
    void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const;

private:
    QHash<int,int> highlights;
};
#endif // MEALCALENDAR_H
