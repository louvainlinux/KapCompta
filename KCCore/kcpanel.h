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

#ifndef KCPANEL_H
#define KCPANEL_H

#include "KCCore_global.h"
#include <QString>
#include <QObject>

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

class KCCORESHARED_EXPORT KCPanel {

public:
    virtual ~KCPanel() {}

    /*
     * Build the GUI of the panel, knowing the database connection name
     **/
    virtual void buildGUI(const QString& connection) = 0;
    /*
     * Performs required operation on the database named connection
     * to allow its use as datasource by the panel
     **/
    virtual void initDB(const QString& connection) { Q_UNUSED(connection)}
    /*
     * Returns the widget to be displayed on the side panel
     * Pointer will be owned by the caller!
     **/
    virtual QWidget* panel() = 0;
    /*
     * Returns the panel name, displayed under the button
     **/
    virtual const QString title() const = 0;
    /*
     * Returns the path to the icon displayed on the button selecting the panel
     **/
    virtual const QString iconPath() const = 0;

public slots:
    /*
     * Called when the panel is selected
     **/
    virtual void selectPanel() {}
    /*
     * Called when the panel is about to be unselected
     **/
    virtual void unselectPanel() {}
    /*
     * Called when the panel needs to save its data
     */
    virtual void saveAll() {}
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(KCPanel, "org.kapcompta.kcpanel/1.0")
QT_END_NAMESPACE

#endif // KCPANEL_H
