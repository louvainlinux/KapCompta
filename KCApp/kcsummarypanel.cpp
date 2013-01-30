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

#include "kcsummarypanel.h"
#include "kcfilemanager.h"
#include "kcbasicsummaryview.h"
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QStringListModel>
#include <QPrinter>
#include <QPrintDialog>
#include <QFileDialog>
#include "kcsettings.h"
#include "kcpdfformat.h"
#include <kcfileformat.h>

KCSummaryPanel::KCSummaryPanel(KCFileManager *fm, QWidget *parent) :
    QWidget(parent)
{
    this->fm = fm;
}

void KCSummaryPanel::setSummaryViews(QList<KCSummaryView*> l)
{
    views = QList<KCSummaryView*>(l);
    views.prepend(new KCBasicSummaryView(this));
}

void KCSummaryPanel::setFileFormats(QList<KCFileFormat*> l)
{
    formats = QList<KCFileFormat*>(l);
    formats.prepend(new KCPdfFormat(this));
}

void KCSummaryPanel::selectPanel()
{
    updateProperties();
    views.at(selectView->currentIndex())->setAccountProperties(properties);
}

void KCSummaryPanel::updateProperties()
{
    int b = fm->value("General/accountBalance").toInt();
    QString n = fm->value("General/accountName").toString();
    QString i = fm->value("General/accountIban").toString();
    properties = KCAccountProperties(b,n,i);
}

void KCSummaryPanel::initDB(const QString& connection)
{
    Q_UNUSED(connection)
}

QWidget* KCSummaryPanel::panel()
{
    return this;
}

const QString KCSummaryPanel::title() const
{
    return tr("Account summary");
}

const QString KCSummaryPanel::iconPath() const
{
    return QString(":/icons/summary");
}

void KCSummaryPanel::buildGUI(const QString &connection)
{
    connectionName = QString(connection);
    updateProperties();
    stackView = new QStackedWidget(this);
    QStringList names;

    QList<KCSummaryView*>::iterator i;
    for (i = views.begin(); i != views.end(); ++i) {
        (*i)->setConnectionName(connection);
        (*i)->setAccountProperties(properties);

        QWidget *page = new QWidget(stackView);

        names << (*i)->summaryName();
        QGroupBox *options = new QGroupBox(tr("Options"),page);
        QVBoxLayout *optLayout = new QVBoxLayout();
        optLayout->addWidget((*i)->displayOptions());
        options->setLayout(optLayout);

        if ((*i)->optionsUnder()) {
            QHBoxLayout *optionsL = new QHBoxLayout();
            optionsL->addStretch(1);
            optionsL->addWidget(options);
            optionsL->addStretch(1);

            QHBoxLayout *summaryL = new QHBoxLayout();
            summaryL->addWidget((*i)->summaryView());

            QVBoxLayout *vLayout = new QVBoxLayout();
            vLayout->addLayout(summaryL,1);
            vLayout->addLayout(optionsL);

            page->setLayout(vLayout);
        } else {
            QHBoxLayout *hLayout = new QHBoxLayout();
            hLayout->addWidget(options);
            hLayout->addWidget((*i)->summaryView(),1);
            page->setLayout(hLayout);
        }
        stackView->addWidget(page);
    }

    selectView = new QComboBox(this);
    QStringListModel *selectModel = new QStringListModel(this);
    QHBoxLayout *selectL = new QHBoxLayout();
    QFormLayout *selectF = new QFormLayout();
    selectF->addRow(tr("View type:"), selectView);
    selectL->addLayout(selectF);
    selectL->addStretch(1);
    selectModel->setStringList(names);
    selectView->setModel(selectModel);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *exportBtn = new QPushButton(tr("Export"),this);
    QPushButton *printBtn = new QPushButton(tr("Print"),this);
    btnLayout->addWidget(exportBtn);
    btnLayout->addWidget(printBtn);

    QVBoxLayout *vBox = new QVBoxLayout();
    vBox->addLayout(selectL);
    vBox->addWidget(stackView);
    vBox->addLayout(btnLayout);
    this->setLayout(vBox);

    stackView->setCurrentIndex(0);

    connect(selectView, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshStackView(int)));
    connect(exportBtn, SIGNAL(clicked()), this, SLOT(exportView()));
    connect(printBtn, SIGNAL(clicked()), this, SLOT(printSummaryView()));
}

void KCSummaryPanel::refreshStackView(int idx)
{
    stackView->setCurrentIndex(idx);
    // To transmit the last version of the account properties to
    // the newly selected view
    selectPanel();
}

void KCSummaryPanel::printSummaryView()
{
    QPrinter printer;

    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print Account Summary"));
    if (dialog->exec() == QDialog::Accepted)
    {
        views.at(selectView->currentIndex())->printSummary(&printer);
    }
    this->activateWindow();
}

void KCSummaryPanel::exportView()
{
    QString filter(formats.first()->formatName());

    QList<KCFileFormat*>::const_iterator iterator = formats.constBegin() + 1;
    while (iterator != formats.constEnd()) {
        filter += ";;" + (*iterator)->formatName();
        ++iterator;
    }
    QString selected;
    QString result = QFileDialog::getSaveFileName
            (this,
             tr("Export Account"),
             KCSettings::valueOf("lastExportedFile").toString(),
             filter,
             &selected);
    if (result != NULL) {
        QList<KCFileFormat*>::const_iterator iterator = formats.constBegin();
        while (iterator != formats.constEnd() && selected.compare((*iterator)->formatName())) {
            ++iterator;
        }
        if (iterator == formats.constEnd()) {
            return;
        }
        (*iterator)->exportToFile(result,
                                  connectionName,
                                  views.at(selectView->currentIndex()));
        KCSettings::setProperty("lastExportedFile",QVariant(result));
    }
    this->activateWindow();
}
