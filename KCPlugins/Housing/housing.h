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

#ifndef HOUSING_H
#define HOUSING_H

#include <kcplugin.h>

class Housing : public QObject, public KCPlugin
{
    /*
     * Plugin to manage meals and drinks for a group of people
     */
    Q_OBJECT
    Q_INTERFACES(KCPlugin)

public:
    // KCPlugin interface
    const QList<KCPanel *> panels();
    const QList<KCSummaryView *> summaryView();
    const QList<KCAccountSetting *> accountSettings() { return QList<KCAccountSetting*>();}
    const QList<KCFileFormat *> fileFormats() { return QList<KCFileFormat*>();}
private:
};

#endif // HOUSING_H
