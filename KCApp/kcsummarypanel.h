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

#ifndef KCSUMMARYPANEL_H
#define KCSUMMARYPANEL_H

#include <QWidget>
#include <kcpanel.h>
#include <QList>
#include <kcsummaryview.h>


QT_BEGIN_NAMESPACE
class QComboBox;
class QStackedWidget;
QT_END_NAMESPACE

class KCFileManager;
class KCFileFormat;

class KCSummaryPanel : public QWidget, public KCPanel
{
    /*
     * KCPanel to display summaryviews of the account
     */
    Q_OBJECT
    Q_INTERFACES(KCPanel)

public:
    long ID() const {return 4;}
    explicit KCSummaryPanel(KCFileManager *fm, QWidget *parent = 0);
    // KCPanel interface
    QWidget* panel();
    const QString title() const;
    const QString iconPath() const;
    void buildGUI(const QString &connection);
    void initDB(const QString& connection);
    void selectPanel();
    // Plugin support
    void setSummaryViews(QList<KCSummaryView*> l);
    void setFileFormats(QList<KCFileFormat*> l);

private slots:
    // Called by the print button
    void printSummaryView();
    // Called by the export button
    void exportView();
    // The user has selected another summary view type
    void refreshStackView(int);

private:
    // Update the accountProperties
    void updateProperties();

    KCFileManager *fm;
    QList<KCSummaryView*> views;
    QList<KCFileFormat*> formats;
    QComboBox *selectView;
    QString connectionName;
    QStackedWidget *stackView;

    KCAccountProperties properties;
};

#endif // KCSUMMARYPANEL_H
