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

#include "kcspinnerdelegate.h"

#include <QDoubleSpinBox>

class KCSpinnerDelegatePrivate {
public:
    QString prefix;
    QString suffix;

    KCSpinnerDelegatePrivate(const QString& pre, const QString& su)
        : prefix(pre), suffix(su)
    {}
};

KCSpinnerDelegate::KCSpinnerDelegate(const QString& prefix,
                                     const QString& suffix,
                                     QObject *parent)
    : QStyledItemDelegate(parent), d(new KCSpinnerDelegatePrivate(prefix, suffix))
{}

KCSpinnerDelegate::~KCSpinnerDelegate()
{
    delete d;
}

QWidget *KCSpinnerDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setFrame(false);
    editor->setMaximum(std::numeric_limits<double>::max());
    editor->setMinimum(0);
    editor->setPrefix(d->prefix);
    editor->setSuffix(d->suffix);
    return editor;
}

void KCSpinnerDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    double value = index.model()->data(index, Qt::EditRole).toDouble();
    QDoubleSpinBox *box = static_cast<QDoubleSpinBox*>(editor);
    box->setValue(value);
}

void KCSpinnerDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QDoubleSpinBox *box = static_cast<QDoubleSpinBox*>(editor);
    double value = box->value();
    model->setData(index, value, Qt::EditRole);
}

void KCSpinnerDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex& index) const
{
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}

QString KCSpinnerDelegate::displayText(const QVariant & value, const QLocale & locale) const
{
    Q_UNUSED(locale)
    return tr("%1 %2").arg(QString::number(value.toDouble()), tr("€"));
}

