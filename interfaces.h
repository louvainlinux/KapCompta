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

#include <QtPlugin>
#include <QHash>

QT_BEGIN_NAMESPACE
class QWidget;
class QString;
class QVariant;
QT_END_NAMESPACE

class KCPanel {
public:
    virtual ~KCPanel() {}

    /*
     * Build the GUI of the panel, knowing the database connection name
     **/
    virtual void buildGUI(const QString& connection) = 0;
    /*
     * Returns the widget to be displayed on the side panel
     **/
    virtual QWidget* panel() = 0;
    /*
     * Returns the panel name, displayed under the button
     **/
    virtual const QString& title() = 0;
    /*
     * Returns the path to the icon displayed on the button selecting the panel
     **/
    virtual const QString& iconPath() = 0;
public slots:
    /*
     * Called when the panel is selected
     **/
    virtual void selectPanel() {}
};

class KCSummaryView {
public:
    virtual ~KCSummaryView() {}

    /*
     * Returns the widget representing the summary view
     **/
    virtual QWidget* summaryView() = 0;
    /*
     * Returns the view name
     **/
    virtual const QString& summaryName() = 0;
    /*
     * Returns the widget containing the view display options
     **/
    virtual QWidget* displayOptions() = 0;
};

class KCAccountSetting {
public:
    virtual ~KCAccountSetting() {}

    /*
     * Returns the widget displaying the settings
     **/
    virtual QWidget* settingPanel() = 0;
    /*
     * Sets whether the settings are editable
     **/
    virtual void setEditable(bool editable) = 0;
    /*
     * Returns a hash map of key/value representing the settings
     **/
    virtual QHash<QString, QVariant>* settings() = 0;
    /*
     * Sets the setting value for the specified key
     **/
    virtual void setSetting(const QString& key, const QVariant& value) = 0;
    /*
     * Returns the setting category name
     **/
    virtual const QString& category() = 0;
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(KCPanel, "org.kapcompta.kcpanel/1.0")
Q_DECLARE_INTERFACE(KCSummaryView, "org.kapcompta.kcsummaryview/1.0")
Q_DECLARE_INTERFACE(KCAccountSetting, "org.kapcompta.kcaccountsetting/1.0")
QT_END_NAMESPACE

#endif // INTERFACES_H
