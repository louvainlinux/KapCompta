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

#ifndef KCSUMMARYVIEW_H
#define KCSUMMARYVIEW_H

#include "KCCore_global.h"
#include <QString>

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

struct KCAccountProperties;

class KCCORESHARED_EXPORT KCSummaryView {
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
    virtual const QString summaryName() const = 0;
    /*
     * Returns the widget containing the view display options
     * Pointer will be owned by the caller!
     **/
    virtual QWidget* displayOptions() { return NULL; }
    /*
     * Returns true if the option panel should be displayed under the summary view,
     * usefull for i.e. width views
     **/
    virtual const bool optionsUnder() const { return true; }
    /*
     * Sets the account initial balance
     **/
    virtual void setAccountProperties(const KCAccountProperties& properties) = 0;
    /*
     * print to the specified QPrinter
     **/
    virtual void printSummary(QPrinter *printer) = 0;
};

struct KCCORESHARED_EXPORT KCAccountProperties {
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

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(KCSummaryView, "org.kapcompta.kcsummaryview/1.0")
QT_END_NAMESPACE

#endif // KCSUMMARYVIEW_H
