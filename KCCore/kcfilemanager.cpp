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
#include <QMessageBox>
#include <QApplication>

#define KC_CURRENT_FILE_VERSION ( (qint32)0xAAAAAAA5 )

KCFileManager::KCFileManager(const QString& filePath, QObject *parent) :
    QObject(parent),
    fileName(QString(filePath))
{
    db = new QTemporaryFile(this);
    if (!db->open()) {
        QMessageBox::critical(0, tr("Cannot create temporary file !"),
            tr("Unable to open the temporary database file."
               ), QMessageBox::Cancel);
        QApplication::exit();
    }
    // Need to close as SQLite will open the database file itself later on,
    // and ask for exclusive access on it
    db->close();
    properties = new QTemporaryFile(this);
    if (!properties->open()) {
        QMessageBox::critical(0, tr("Cannot create temporary file !"),
            tr("Unable to open the temporary account properties file."
               ), QMessageBox::Cancel);
        QApplication::exit();
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
    // save will register the empty account properties file
    return save();
}

void KCFileManager::setValue(const QString& key, const QVariant& value)
{
    QSettings settings(properties->fileName(),
                        QSettings::IniFormat);
    settings.setValue(key,value);
    // Let's ensure that the new settings are written to disk right away
    // in case we're about to save the whole account file
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
    // Commit changes only at the end to refrain IO usage
    settings.sync();
}

const QVariant KCFileManager::value(const QString& key) const
{
    QSettings settings(properties->fileName(),
                        QSettings::IniFormat);
    return settings.value(key);
}

const QVariant KCFileManager::valueForCategory
    (const QString& key, const QString& category) const
{
    return value(category+"/"+key);
}

const QString KCFileManager::dbPath() const
{
    return QString(db->fileName());
}

bool KCFileManager::open()
{
    qint32 version;
    QByteArray dbContent;
    QByteArray propertiesContent;

    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QDataStream stream(&file);
    stream >> version;
    //magic number check ...
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
        QMessageBox::critical(0, tr("Unsupported file!"),
            tr("Unable to open the specified account file.\nVersion mismatch."
               ), QMessageBox::Cancel);
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
    // save file version tracking
    stream << KC_CURRENT_FILE_VERSION;
    db->open();
    properties->open();
    stream << qCompress(properties->readAll()) << qCompress(db->readAll());
    db->close();
    properties->close();
    file.close();
    return true;
}

const QString KCFileManager::shortenedPath(int maxLength) const
{
    return shortenedPath(maxLength, fileName);
}

const QString KCFileManager::shortenedPath(int maxLength, const QString& str)
{
    QString projectPath = QString(str);
    if (projectPath.length() > maxLength) {
        int adjustedMaxLength = maxLength + 5; // # char to add [...]
        int diff = projectPath.length() - adjustedMaxLength;
        projectPath.remove(diff/2, diff); // we remove the middle of the string
        projectPath.insert(diff/2,"[...]"); // and replace it by [...]
    }
    return projectPath;
}
