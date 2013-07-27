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

#include "kccore.h"
#include "interfaces/kcplugin.h"
#include <QtDebug>
#include <QHash>
#include <QDir>
#include <QPluginLoader>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QVariant>
#include <QCoreApplication>

class KCCorePrivate {
public:
    /*
     * Stores the metadata about plugins
     **/
    QHash<QString, QVariant> p_versions;
    QHash<QString, QVariantList> p_dependancies;
    QHash<QString, QString> p_locations;
    /*
     * The plugins themselves
     **/
    QList<KCPlugin*> p_core;
};

KCCore* KCCore::s_instance = NULL;

KCCore::KCCore() : QObject(),
    d(new KCCorePrivate) {
    QDir path = QDir(qApp->applicationDirPath());

#if defined(Q_OS_MAC)
    path.cdUp();
#endif

    path.cd("plugins");
    QFileInfoList files = path.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    for (QFileInfoList::iterator it = files.begin(); it != files.end(); ++it)
        extractPluginInfo((*it).absoluteFilePath());
    for (QFileInfoList::iterator it = files.begin(); it != files.end(); ++it)
        loadPlugin((*it).absoluteFilePath());
}

KCCore::~KCCore()
{
    delete d;
}

KCCore* KCCore::instance()
{
    if (!s_instance) s_instance = new KCCore();
    return s_instance;
}

void KCCore::extractPluginInfo(const QString &p)
{
    QPluginLoader loader(p);
    QString id = loader.metaData().value("MetaData").toObject().value("id").toString();
    d->p_dependancies.insert(id, loader.metaData().value("MetaData").toObject()
                             .value("dependancies").toArray().toVariantList());
    d->p_versions.insert(id, loader.metaData().value("MetaData").toObject()
                         .value("version").toVariant());
    d->p_locations.insert(id, p);
}

void KCCore::loadPlugin(const QString &p)
{
    if (loadable(p)) {
        QPluginLoader loader(p);
        QObject *plugin = loader.instance();
        if (plugin) { // did we manage to load our plugin
            KCPlugin* p = qobject_cast<KCPlugin *>(plugin);
            if (p) { // is it a real KCPlugin ?
                d->p_core.append(p);
            }
        }
    }
}

bool KCCore::loadable(const QString &p)
{
    QVariantList dep = d->p_dependancies.value(p);
    for (QVariantList::iterator it = dep.begin(); it != dep.end(); ++it) {
        QVariantMap map = (*it).toMap();
        QString id = map.value("identifier").toString();
        QVariant version = map.value("version");
        if (!d->p_versions.contains(id)) {
            qDebug() << Q_FUNC_INFO << "Missing dependancy " << id << " for " << p;
            return false;
        }
        if (d->p_versions.value(id) != version) {
            qDebug() << Q_FUNC_INFO << "Uncompatible version, found "
                     << d->p_versions.value(id).toString() << " instead of "
                     << version.toString() << " required by " << p;
            return false;
        }
        if (!loadable(d->p_locations.value(id))) {
            qDebug() << Q_FUNC_INFO << "Cannot load dependancy " << id << " for " << p;
            return false;
        }
    }
    return true;
}


const QList<KCPanel*> KCCore::panels()
{
    QList<KCPanel*> l;
    for (QList<KCPlugin*>::iterator it = d->p_core.begin(); it != d->p_core.end(); ++it)
        l << ((KCPlugin*)*it)->panels();
    return l;
}
