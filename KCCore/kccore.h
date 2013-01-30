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

#ifndef KCCORE_H
#define KCCORE_H

#include <QDir>
#include "KCCore_global.h"

QT_BEGIN_NAMESPACE
class QApplication;
class QWidget;
QT_END_NAMESPACE

class KCCORESHARED_EXPORT KCCore
{
    /*
     * Provides general-purpose utility functions
     */
public:
    /*
     * Returns the QDir containing the plugins for the given application
     */
    static QDir pluginsDir(QApplication* app);
    /*
     * Center the given widget on the screen
     */
    static void center(QWidget* w);
    /*
     * Return a QString containing html code to display an horizontal line
     */
    static const QString hr();
};

#endif // KCCORE_H
