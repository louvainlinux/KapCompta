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

#include "kcaccountfile.h"

#include <QTemporaryDir>
#include <QHash>
#include <QList>
#include <QSet>
#include <QDebug>

#define ACCOUNT_MAGIC 0xa1a4a5a8
#define VERSION_1 0xdeadbeef

class KCAccountFilePrivate {
public:
    QString lasterror;
    QString filename;
    QTemporaryDir tempDir;
    QHash<QString, QVariant> properties;
    QSet<QString> components;

    KCAccountFilePrivate(const QString& f)
        : filename(QString(f))
    {}
    ~KCAccountFilePrivate()
    {}
};

KCAccountFile::KCAccountFile(const QString &filename, QObject *parent)
    : QObject(parent),
      d(new KCAccountFilePrivate(filename))
{
}

KCAccountFile::~KCAccountFile()
{
    delete d;
}

bool KCAccountFile::read() const
{
    d->lasterror = QString();
    QFile file(d->filename);
    if (!file.open(QIODevice::ReadOnly)) {
        d->lasterror = tr("Failed to open the account file !");
        qDebug() << Q_FUNC_INFO << d->lasterror;
        return false;
    }
    QDataStream in(&file);
    quint32 magic;
    qint32 version;
    in >> magic;
    if (magic != (quint32)ACCOUNT_MAGIC) {
        d->lasterror = tr("Wrong magic number");
        qDebug() << Q_FUNC_INFO << d->lasterror;
        return false;
    }
    in >> version;
    switch (version) {
    case VERSION_1:
        in.setVersion(QDataStream::Qt_5_1);
        d->properties.clear();
        in >> d->properties;
        quint32 componentCount;
        in >> componentCount;
        d->components.clear();
        for (unsigned int i = 0; i < componentCount; ++i) {
            QString s;
            QByteArray content;
            in >> s >> content;
            d->components.insert(s);
            QFile f(componentPath(s));
            if (!f.open(QIODevice::WriteOnly)) {
                d->lasterror = tr("Failed to open the component %1").arg(f.fileName());
                qDebug() << Q_FUNC_INFO << d->lasterror;
            }
            f.write(content);
            f.close();
            ++i;
        }
        file.close();
        return d->lasterror.isEmpty();
    default:
        d->lasterror = tr("Unkown version of the account file !");
        file.close();
        return false;
    }
}

bool KCAccountFile::save()
{
    emit busy();
    d->lasterror = QString();
    QFile file(d->filename);
    if (!file.open(QIODevice::WriteOnly)) {
        d->lasterror = tr("Failed to open the account file !");
        qDebug() << Q_FUNC_INFO << d->lasterror;
        emit available();
        return false;
    }
    QDataStream out(&file);
    // Specify our filetype
    out << (quint32)ACCOUNT_MAGIC;
    // Specify our version code and the corresponding encoding scheme
    out << (qint32)VERSION_1;
    out.setVersion(QDataStream::Qt_5_1);
    // Save the account properties
    QMap<QString, QVariant> prop;
    QHash<QString, QVariant>::const_iterator propertiesIterator = d->properties.constBegin();
    while (propertiesIterator != d->properties.constEnd()) {
        prop[propertiesIterator.key()] = propertiesIterator.value();
        ++propertiesIterator;
    }
    out << prop;
    // Save the name/data pair for each component.
    out << (quint32)d->components.size();
    QSet<QString>::const_iterator i = d->components.constBegin();
    while (i != d->components.constEnd()) {
        out << *i;
        QFile f(componentPath(*i));
        if (!f.open(QIODevice::ReadOnly)) {
            d->lasterror = tr("Failed to open the component %1").arg(f.fileName());
            qDebug() << Q_FUNC_INFO << d->lasterror;
        }
        out << f.readAll();
        f.close();
        ++i;
    }
    file.close();
    emit available();
    return d->lasterror.isEmpty();
}

void KCAccountFile::addComponent(const QString &componentName)
{
    d->components.insert(componentName);
    // Attempt to pre-create the file.
    QFile file(componentPath(componentName));
    if (!file.exists())
        if (file.open(QIODevice::WriteOnly))
            file.close();
}

const QString KCAccountFile::componentPath(const QString &componentName) const
{
    if (!d->components.contains(componentName)) return QString();
    QString tDir = d->tempDir.path();
    return QString("%1/%2").arg(tDir.endsWith('/') ? tDir.remove(tDir.length()-1, 1) : tDir,
                                *(d->components.find(componentName)));
}

void KCAccountFile::setProperty(const QString& key, const QVariant& value)
{
    d->properties[key] = value;
}

const QVariant KCAccountFile::getProperty(const QString& key) const
{
    return d->properties.value(key);
}

const QString KCAccountFile::lastError() const
{
    return d->lasterror;
}

const QString KCAccountFile::fileName() const
{
    return d->filename;
}
