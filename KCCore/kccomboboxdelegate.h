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

#ifndef KCCOMBOBOXDELEGATE_H
#define KCCOMBOBOXDELEGATE_H

#include "kccore_global.h"
#include <QStyledItemDelegate>

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
QT_END_NAMESPACE

class KCComboBoxDelegatePrivate;
class KCCORESHARED_EXPORT KCComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    KCComboBoxDelegate(QAbstractItemModel* model, int displayColumn,
                       int indexColumn, QObject *parent = 0);
    ~KCComboBoxDelegate();
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool eventFilter(QObject *object, QEvent *event);
    QString displayText(const QVariant & value, const QLocale & locale) const;
private:
    KCComboBoxDelegatePrivate *d;
};

#endif // KCCOMBOBOXDELEGATE_H
