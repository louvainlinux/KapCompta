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

#include "kccomboboxdelegate.h"
#include <QComboBox>
#include <QEvent>
#include <QAbstractItemModel>
#include <QDebug>

class KCComboBoxDelegatePrivate {
public:
    QAbstractItemModel *model;
    int displayColumn;
    int indexColumn;

    KCComboBoxDelegatePrivate(QAbstractItemModel* m, int d, int i)
        : model(m), displayColumn(d), indexColumn(i)
    {}
};

KCComboBoxDelegate::KCComboBoxDelegate(QAbstractItemModel* model, int displayColumn,
                                       int indexColumn, QObject *parent)
    : QStyledItemDelegate(parent),
      d(new KCComboBoxDelegatePrivate(model, displayColumn, indexColumn))
{}

KCComboBoxDelegate::~KCComboBoxDelegate()
{
    delete d;
}

QWidget *KCComboBoxDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    QComboBox *editor = new QComboBox(parent);
    editor->setModel(d->model);
    editor->setEditable(false);
    editor->setModelColumn(d->displayColumn);
    editor->installEventFilter(const_cast<KCComboBoxDelegate *>(this));
    return editor;
}

void KCComboBoxDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();
    QComboBox *box = static_cast<QComboBox*>(editor);
    box->setCurrentIndex(d->model->match(d->model->index(0, d->indexColumn),
                                         Qt::DisplayRole, value, 1, Qt::MatchExactly).at(0).row());
}

void KCComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QComboBox *box = static_cast<QComboBox*>(editor);
    int value = box->currentIndex();

    model->setData(index, d->model->data(
                       d->model->index(value, d->indexColumn)), Qt::EditRole);
}

void KCComboBoxDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex& index) const
{
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}

bool KCComboBoxDelegate::eventFilter(QObject *object, QEvent *event)
{
    QComboBox * comboBox = dynamic_cast<QComboBox*>(object);
    if (comboBox) {
        if (event->type() == QEvent::MouseButtonRelease) {
            comboBox->showPopup();
            return true;
        }
    } else
        return QStyledItemDelegate::eventFilter(object, event);
    return false;
}

QString KCComboBoxDelegate::displayText(const QVariant & value, const QLocale & locale) const
{
    Q_UNUSED(locale)
    int v = value.toInt();
    return d->model->data(d->model->index(
                              d->model->match(d->model->index(0, d->indexColumn),
                                          Qt::DisplayRole, v, 1, Qt::MatchExactly).at(0).row(),
                              d->displayColumn),
                          Qt::EditRole).toString();
}
