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

#ifndef KCPROPERTIESPANEL_H
#define KCPROPERTIESPANEL_H

#include <QWidget>
#include <kcpanel.h>
#include <QList>

QT_BEGIN_NAMESPACE
class QPushButton;
QT_END_NAMESPACE

class KCFileManager;
class KCAccountSetting;

class KCPropertiesPanel : public QWidget, public KCPanel
{
    /*
     * KCPanel to handle account properties
     */
    Q_OBJECT
    Q_INTERFACES(KCPanel)

public:
    long ID() const {return 2;}
    explicit KCPropertiesPanel(KCFileManager *fm, QWidget *parent = 0);
    // KCPanel interface
    QWidget* panel();
    const QString title() const;
    const QString iconPath() const;
    void buildGUI(const QString &connection);
    void setSettingList(QList<KCAccountSetting*> l);

public slots:
    void unselectPanel();

private slots:
    // Called by the save button
    void saveSettings();
    // Called by the cancel button
    void reloadSettings();
    // Called by the KCAccountSetting instances
    void settingsAltered();

private:
    KCFileManager *fm;
    QList<KCAccountSetting*> settingsList;
    QPushButton *saveBtn;
    QPushButton *reloadBtn;
    bool autosave;

    void reload();
};

#endif // KCPROPERTIESPANEL_H
