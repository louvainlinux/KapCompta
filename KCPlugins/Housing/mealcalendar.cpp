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

#include "mealcalendar.h"

#include <QPainter>
#include <QRect>

MealCalendar::MealCalendar(QWidget *parent) :
    QCalendarWidget(parent)
{
}

void MealCalendar::setCurrentMonthHighlights(const QHash<int,int>& highlights)
{
    this->highlights = QHash<int,int>(highlights);
    update();
}

void MealCalendar::setHighlightsForDay(int day, int highlights)
{
    this->highlights[day] = highlights;
    updateCell(QDate(QCalendarWidget::yearShown(), QCalendarWidget::monthShown(), day));
}

void MealCalendar::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
    QCalendarWidget::paintCell(painter, rect, date);
    if (highlights.value(date.day()) > 0
            && date.month() == QCalendarWidget::selectedDate().month()) {
        painter->save();
        // Set background color to green because there are meal(s) that day
        QFontMetrics fm = painter->fontMetrics();
        int w = fm.width(QString::number(highlights.value(date.day())));
        int h = fm.height();
        int max = qMax(w, h) + 3;
        QRect r = QRect(rect.x(), rect.y(), max, max);
        painter->setBrush(QBrush(Qt::darkCyan, Qt::SolidPattern));
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(r);
        painter->setBrush(Qt::NoBrush);
        painter->setPen(Qt::lightGray);
        painter->drawRect(QRect(rect.x(), rect.y(), rect.width()-1, rect.height()-1));
        painter->restore();
        painter->drawText(QRect(r.x(), r.y() + max/2 - h/2, max, max),
                          QString::number(highlights.value(date.day())),
                          QTextOption(Qt::AlignCenter));
    }
}
