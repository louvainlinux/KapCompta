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

#ifndef KCWELCOMESCREEN_H
#define KCWELCOMESCREEN_H

#include <QWidget>

class KCNewAccount;

QT_BEGIN_NAMESPACE
class QPushButton;
QT_END_NAMESPACE

class KCWelcomeScreen : public QWidget
{
    /*
     * Provide a welcome screen allowing to load/create accounts
     */
    Q_OBJECT

public:
    explicit KCWelcomeScreen(QWidget *parent = 0);
    ~KCWelcomeScreen();

signals:
    /*
     * Emitted with the project path when it is known
     */
    void knowProject(QString);
    /*
     * Emitted when a new account file has been created
     */
    void accountCreated(QString);

private slots:
    /*
     * Notifies the instance that it must start the new account process
     */
    void newAccount();
    /*
     * Notifies the instance that it must open an account
     */
    void openAccount();
    /*
     * Notifies the instance that it must open the last opened account
     */
    void openLast();

private:
    KCNewAccount *w;
    QPushButton *lastOpenButton;
};

#endif // KCWELCOMESCREEN_H
