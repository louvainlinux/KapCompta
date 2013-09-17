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

#ifndef KCPLUGIN_H
#define KCPLUGIN_H

#include <QList>
#include <QtPlugin>

class KCPanel;
class KCAccountFile;

/**
 * @brief The KCPlugin class is the base interface for the plugins.
 */
class KCPlugin {
public:
    virtual ~KCPlugin() {}
    /**
     * @brief panels returns all panels provided by the plugins. Successive calls to this
     *              method should result in multiple instances of the same kind of panel,
     *              which will later allow one to have mutliple account opened at once.
     * @brief account the account file linked to these panel instances
     * @return all panels provided by the plugin (or an empty list)
     */
    virtual const QList<KCPanel*> panels(KCAccountFile *account) = 0;
};

#define KCPLUGIN_IID "org.kapcompta.kcplugin"
Q_DECLARE_INTERFACE(KCPlugin, KCPLUGIN_IID)
#endif // KCPLUGIN_H
