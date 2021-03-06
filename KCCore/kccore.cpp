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
#include <QMessageBox>
#include "kcdatabase.h"
#include "kcaccountfile.h"
#include "kcglobals.h"

class KCCorePrivate {
public:
    /*
     * Stores the metadata about plugins
     **/
    QHash<QString, QVariant> p_versions;
    QHash<QString, QVariantList> p_dependencies;
    QHash<QString, QString> p_locations;
    /*
     * The plugins themselves
     **/
    QList<KCPlugin*> p_core;

private:
    void extractPluginInfo(const QString &p)
    {
        QPluginLoader loader(p);
        QString id = loader.metaData().value("MetaData").toObject().value("identifier").toString();
        p_dependencies.insert(id, loader.metaData().value("MetaData").toObject()
                              .value("dependencies").toArray().toVariantList());
        p_versions.insert(id, loader.metaData().value("MetaData").toObject()
                          .value("version").toVariant());
        p_locations.insert(id, p);

    }

    bool loadable(const QString &p)
    {
        QVariantList dep = p_dependencies.value(p_locations.key(p));
        for (QVariantList::iterator it = dep.begin(); it != dep.end(); ++it) {
            QVariantMap map = (*it).toMap();
            QString id = map.value("identifier").toString();
            QVariant version = map.value("version");
            if (!p_versions.contains(id)) {
                qDebug() << Q_FUNC_INFO << "Missing dependency" << id << "for" << p;
                return false;
            }
            if (p_versions.value(id) != version) {
                qDebug() << Q_FUNC_INFO << "Uncompatible version, found"
                         << p_versions.value(id).toString() << "instead of"
                         << version.toString() << "required by" << p;
                return false;
            }
            if (!loadable(p_locations.value(id))) {
                qDebug() << Q_FUNC_INFO << "Cannot load dependency" << id << "for" << p;
                return false;
            }
        }
        return true;
    }

    void loadPlugin(const QString &p)
    {
        if (loadable(p)) {
            QPluginLoader loader(p);
            QString id = loader.metaData().value("MetaData").toObject().value("identifier").toString();
            QObject *plugin = loader.instance();
            if (!loader.isLoaded()) qDebug() << Q_FUNC_INFO << loader.errorString();
            else {
                KCPlugin* kcplugin = qobject_cast<KCPlugin *>(plugin);
                if (kcplugin) { // is it a real KCPlugin ?
                    kcplugin->id = p_locations.key(p) + "/" + p_versions.value(id).toString();
                    p_core.append(kcplugin);
                }
            }
        }
    }

public:
    KCCorePrivate()
    {
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
        qDebug() << Q_FUNC_INFO << "Loaded" << p_core.size() << "plugins";
    }

    ~KCCorePrivate()
    {
        qDeleteAll(p_core);
    }
};

KCCore* KCCore::s_instance = NULL;

KCCore::KCCore() : QObject(),
    d(new KCCorePrivate)
{
    QCoreApplication::setOrganizationName("Louvain-li-Nux");
    QCoreApplication::setOrganizationDomain("louvainlinux.be");
    QCoreApplication::setApplicationName("KapCompta");
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

const QList<KCPanel*> KCCore::panels(KCAccountFile *account)
{
    QList<KCPanel*> l;
    for (QList<KCPlugin*>::iterator it = d->p_core.begin(); it != d->p_core.end(); ++it)
        l << ((KCPlugin*)*it)->panels(account);
    return l;
}

void KCCore::setStatus(const QString &s, int timeout)
{
    emit statusUpdate(s, timeout);
}

// Change this function to change the way errors are reported to the user
void KCCore::warning(const QString &message)
{
    QMessageBox::critical(0, tr("Something went wrong ..."), message,
                          QMessageBox::Ok, QMessageBox::Ok);
}

void KCCore::createAccount(const QString &location,
                           const QString &name,
                           const QString &description)
{
    KCAccountFile f(location);
    // add a default database
    KCDatabase::create(&f);
    // set the default properties
    f.setProperty(PROPERTY_ACCOUNT_NAME, name);
    f.setProperty(PROPERTY_ACCOUNT_DESCR, description);
    // initialize the file for all plugins
    for (QList<KCPlugin*>::iterator it = d->p_core.begin(); it != d->p_core.end(); ++it)
        ((KCPlugin*)*it)->init(&f);
    for (QList<KCPlugin*>::iterator it = d->p_core.begin(); it != d->p_core.end(); ++it) {
        ((KCPlugin*)*it)->initDone(&f);
        f.setProperty(INIT_PREFIX + ((KCPlugin*)*it)->id, true);
    }
    // save our base file
    f.save();
}

bool KCCore::openAccount(KCAccountFile *f)
{
    if (!f->read()) return false;
    // ensures that the file has been initialized by all plugins
    for (QList<KCPlugin*>::iterator it = d->p_core.begin(); it != d->p_core.end(); ++it)
        if (!f->getProperty(INIT_PREFIX + ((KCPlugin*)*it)->id).toBool())
            ((KCPlugin*)*it)->init(f);
    for (QList<KCPlugin*>::iterator it = d->p_core.begin(); it != d->p_core.end(); ++it)
        if (!f->getProperty(INIT_PREFIX + ((KCPlugin*)*it)->id).toBool()){
            ((KCPlugin*)*it)->initDone(f);
            f->setProperty(INIT_PREFIX + ((KCPlugin*)*it)->id, true);
        }
    return true;
}
