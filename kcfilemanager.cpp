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

#include "kcfilemanager.h"
#include <QVariant>
#include <QTemporaryFile>
#include <QSettings>
#include <QFileSystemWatcher>
#include <QDataStream>
#include "kcdatabasehelper.h"

#define KC_CURRENT_FILE_VERSION ( (qint32)0xAAAAAAA0 )

KCFileManager::KCFileManager(const QString& filePath, QObject *parent) :
    QObject(parent),
    fileName(QString(filePath))
{
    db = new QTemporaryFile(this);
    if (!db->open()) {
        qDebug("can't create temp db file!");
    }
    db->close();
    properties = new QTemporaryFile(this);
    if (!properties->open()) {
        qDebug("can't create temp properties file!");
    }
    properties->close();
}

KCFileManager::~KCFileManager()
{
    KCDataBaseHelper::close(db->fileName());
    save();
    db->close();
    properties->close();
}

bool KCFileManager::createNewAccount()
{
    KCDataBaseHelper::initDB(db->fileName());
    setValue("dbFileHasBeenInitialized",QVariant(false));
    return save();
}

void KCFileManager::setValue(const QString& key, const QVariant& value)
{
    QSettings settings(properties->fileName(),
                        QSettings::IniFormat);
    settings.setValue(key,value);
    settings.sync();
}

void KCFileManager::setValues(const QHash<QString, QVariant>& values, const QString& category)
{
    QSettings settings(properties->fileName(),
                        QSettings::IniFormat);
    settings.beginGroup(category);
    QHash<QString, QVariant>::const_iterator iterator = values.constBegin();
    while (iterator != values.constEnd()) {
        settings.setValue(iterator.key(),iterator.value());
        iterator++;
    }
    settings.endGroup();
    settings.sync();
}

QVariant KCFileManager::value(const QString& key)
{
    QSettings settings(properties->fileName(),
                        QSettings::IniFormat);
    return settings.value(key);
}

QString KCFileManager::dbPath()
{
    return QString(db->fileName());
}

bool KCFileManager::open()
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QDataStream stream(&file);
    qint32 version;
    QByteArray dbContent;
    QByteArray propertiesContent;
    stream >> version;
    if (version == KC_CURRENT_FILE_VERSION) {
        stream.setVersion(QDataStream::Qt_4_8);
        stream >> propertiesContent  >> dbContent;
        db->open();
        properties->open();
        db->write(qUncompress(dbContent));
        properties->write(qUncompress(propertiesContent));
        db->close();
        properties->close();
        file.close();
        KCDataBaseHelper::createConnection(db->fileName());
        return true;
    } else {
        qDebug("unsupported file version");
        file.close();
        return false;
    }
}

bool KCFileManager::save()
{
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    QDataStream stream(&file);
    stream.setVersion(QDataStream::Qt_4_8);
    stream << KC_CURRENT_FILE_VERSION;
    db->open();
    properties->open();
    stream << qCompress(properties->readAll()) << qCompress(db->readAll());
    db->close();
    properties->close();
    file.close();
    return true;
}

