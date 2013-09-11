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

#ifndef KCPANEL_H
#define KCPANEL_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

class KCPanel : public QObject
{
    Q_OBJECT
public:
    KCPanel(QObject *parent = 0) : QObject(parent) {}
    virtual ~KCPanel() {}

    /**
     * Mandatory methods
     **/
public:
    /**
     * @brief panelName returns the displayed name of the panel.
     * @return the panel name
     */
    virtual const QString panelName() = 0;
    /**
     * @brief panel return the panel to display
     * @return the widget provided by this panel
     */
    virtual QWidget* panel() = 0;

    /**
     * Optional
     **/
public:
    /**
     * @brief iconName returns the path to the panel's icon (can be a ressource path).
     * @return an image path
     */
    virtual const QString iconName() { return QString(); }
public slots:
    /**
     * @brief selected notifies the panel that it appeared on the screen.
     */
    virtual void selected() {}
    /**
     * @brief unselected notifies the panel that itappeared on the screen.
     */
    virtual void unselected() {}
};

#endif // KCPANEL_H
