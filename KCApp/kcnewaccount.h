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

#ifndef KCNEWACCOUNT_H
#define KCNEWACCOUNT_H

#include <QWidget>

class KCAccountSetting;

class KCNewAccount : public QWidget
{
    /*
     * Provide a wizard to create a new account file
     */
    Q_OBJECT

public:
    explicit KCNewAccount(QWidget *parent = 0);
signals:
    /*
     * Emitted when the new account has been created, give the path to it
     */
    void accountSet(QString path);
    /*
     * Emitted if the user cancels the account creation
     */
    void cancel();

private slots:
    /*
     * Notifies the instance that it must ask the user where to save the account
     */
    void setSaveLocation();
private:
    /*
     * Create an account at the given path
     */
    void createAt(const QString&path);

    KCAccountSetting *accountProperties;
};

#endif // KCNEWACCOUNT_H
