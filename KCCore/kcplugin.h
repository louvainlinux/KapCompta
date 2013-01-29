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

#ifndef INTERFACES_H
#define INTERFACES_H

#include <KCCore_global.h>
#include <QtPlugin>
#include <QList>

class KCPanel;
class KCSummaryView;
class KCAccountSetting;
class KCFileFormat;

/*
 * Plugin interface; factory pattern.
 **/
class KCCORESHARED_EXPORT KCPlugin {
public:
    virtual ~KCPlugin() {}
    /*
     * Returns the list of KCPanels in the plugin
     **/
    virtual const QList<KCPanel *> panels() = 0;
    /*
     * Returns the list of KCSummaryViews in the plugin
     **/
    virtual const QList<KCSummaryView *> summaryView() = 0;
    /*
     * Returns the list of KCAccountSettings in the plugin
     **/
    virtual const QList<KCAccountSetting *> accountSettings() = 0;
    /*
     * Returns the list of KCFileFormats in the plugin
     **/
    virtual const QList<KCFileFormat *> fileFormats() = 0;
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(KCPlugin, "org.kapcompta.kcplugin/1.0")
QT_END_NAMESPACE

#endif // INTERFACES_H
