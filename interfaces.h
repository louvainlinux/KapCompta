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
#include <QList>

QT_BEGIN_NAMESPACE
class QWidget;
class QString;
class QVariant;
class QPrinter;
QT_END_NAMESPACE

/*
 * Interface to make callbacks: cannot use the signal/slot system on QPlugins
 * ~ Java Listeners
 **/
class KCObserver {
public:
    ~KCObserver() {}

    /*
     * The callback function,
     * in the future: should handle args and/or allow to have an array of functions
     **/
    virtual void callback() = 0;
};

class KCPanel {
public:
    virtual ~KCPanel() {}

    /*
     * Build the GUI of the panel, knowing the database connection name
     **/
    virtual void buildGUI(const QString& connection) = 0;
    /*
     * Performs required operation on the database named connection
     * to allow its use as datasource by the panel
     **/
    virtual void initDB(const QString& connection) = 0;
    /*
     * Returns the widget to be displayed on the side panel
     * Pointer will be owned by the caller!
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
    /*
     * Called when the panel is selected
     **/
    virtual void selectPanel() {}
    /*
     * Called when the panel is about to be unselected
     **/
    virtual void unselectPanel() {}
    /*
     * Implement this if your panel needs to know when to save its data
     */
    virtual void saveAll() {}
};

struct KCAccountProperties {
    int balance;
    QString name;
    QString iban;

    KCAccountProperties() {}

    KCAccountProperties(int b, QString n, QString i)
    {
        balance = b;
        name = n;
        iban = i;
    }

};

class KCSummaryView {
public:
    virtual ~KCSummaryView() {}

    /*
     * Returns the widget representing the summary view
     * Pointer will be owned by the caller!
     **/
    virtual QWidget* summaryView() = 0;
    /*
     * Sets the database connection name
     **/
    virtual void setConnectionName(const QString& c) = 0;
    /*
     * Returns the view name
     **/
    virtual const QString& summaryName() = 0;
    /*
     * Returns the widget containing the view display options
     * Pointer will be owned by the caller!
     **/
    virtual QWidget* displayOptions() = 0;
    /*
     * Returns true if the option panel should be displayed under the summary view,
     * usefull for i.e. width views
     **/
    virtual bool optionsUnder() = 0;
    /*
     * Sets the account initial balance
     **/
    virtual void setAccountProperties(KCAccountProperties properties) = 0;
    /*
     * print to the specified QPrinter
     **/
    virtual void printSummary(QPrinter *printer) = 0;
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
     * Pointer will be owned by the caller!
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
    /*
     * Returns the settings key
     **/
    virtual const QStringList& keys() = 0;
    /*
     * Register o as an observer to be notified when a setting changes
     **/
    virtual void addObserver(KCObserver *o) = 0;
};

class KCFileFormat {

public:
    virtual ~KCFileFormat() {}

    /*
     * Returns the format name, following the QFileDialog standards:
     * Full name (*.extension)
     **/
    virtual const QString& formatName() = 0;
    /*
     * Write the database data to fileName in the desired format, could start modal panels to
     * provide additional parameters (i.e. like png compression rate)
     **/
    virtual void exportToFile(const QString& fileName, const QString& connectionName,
                              KCSummaryView *view) = 0;
};

/*
 * To enable plugins to provide several features with several instances of interface.
 **/
class KCPlugin {
public:
    /*
     * Returns the list of KCPanels in the plugin
     **/
    virtual QList<KCPanel *> panels() { return QList<KCPanel*>(); }
    /*
     * Returns the list of KCSummaryViews in the plugin
     **/
    virtual QList<KCSummaryView *> summaryView() { return QList<KCSummaryView*>(); }
    /*
     * Returns the list of KCAccountSettings in the plugin
     **/
    virtual QList<KCAccountSetting *> accountSettings() { return QList<KCAccountSetting*>(); }
    /*
     * Returns the list of KCFileFormats in the plugin
     **/
    virtual QList<KCFileFormat *> fileFormats() { return QList<KCFileFormat*>(); }
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(KCObserver, "org.kapcompta.kcobserver/1.0")
Q_DECLARE_INTERFACE(KCPanel, "org.kapcompta.kcpanel/1.0")
Q_DECLARE_INTERFACE(KCSummaryView, "org.kapcompta.kcsummaryview/1.0")
Q_DECLARE_INTERFACE(KCAccountSetting, "org.kapcompta.kcaccountsetting/1.0")
Q_DECLARE_INTERFACE(KCFileFormat, "org.kapcompta.kcfileformat/1.0")
Q_DECLARE_INTERFACE(KCPlugin, "org.kapcompta.kcplugin/1.0")
QT_END_NAMESPACE

#endif // INTERFACES_H
