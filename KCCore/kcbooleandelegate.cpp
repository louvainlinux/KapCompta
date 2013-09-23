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

#include "kcbooleandelegate.h"
#include <QComboBox>
#include <QEvent>

class KCBooleanDelegatePrivate {
public:
    QString off;
    QString on;

    KCBooleanDelegatePrivate(const QString& onText, const QString& offText)
        : off(offText), on(onText)
    {}
};

KCBooleanDelegate::KCBooleanDelegate(const QString& on, const QString& off, QObject *parent)
    : QStyledItemDelegate(parent), d(new KCBooleanDelegatePrivate(on, off))
{}

KCBooleanDelegate::~KCBooleanDelegate()
{
    delete d;
}

QWidget *KCBooleanDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    QComboBox *editor = new QComboBox(parent);
    editor->addItem(d->off);
    editor->addItem(d->on);
    editor->installEventFilter(const_cast<KCBooleanDelegate *>(this));
    return editor;
}

void KCBooleanDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();
    QComboBox *box = static_cast<QComboBox*>(editor);
    box->setCurrentIndex(value);
}

void KCBooleanDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QComboBox *box = static_cast<QComboBox*>(editor);
    int value = box->currentIndex();
    model->setData(index, value, Qt::EditRole);
}

void KCBooleanDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex& index) const
{
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}

QString KCBooleanDelegate::displayText(const QVariant & value, const QLocale & locale) const
{
    Q_UNUSED(locale)
    return value.toBool() ? d->on : d->off;
}

bool KCBooleanDelegate::eventFilter(QObject *object, QEvent *event)
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
