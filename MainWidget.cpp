#include <QColor>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QMimeData>
#include <QString>
#include <QStringList>

#include "MainWidget.hpp"

MainWidget::MainWidget(QSettings &settings, QWidget *parent) : QWidget(parent), m_set(settings)
{
    this->m_fsWidget = new QListWidget(this);
    this->m_hideOkButton = new QPushButton(QStringLiteral("Cacher OK"), this);
    this->m_layout = new QGridLayout(this);
    this->m_loader = new QProgressBar(this);
    this->m_pathLine = new QLineEdit(this->m_set.value(QStringLiteral("history/dir")).toString(), this);
    this->m_processButton = new QPushButton(QStringLiteral("Confirmer"), this);
    this->m_searchButton = new QPushButton(QStringLiteral("Sélectionner"), this);
    this->m_showOkButton = new QPushButton(QStringLiteral("Montrer OK"), this);
    this->m_startButton = new QPushButton(QStringLiteral("Démarrer"), this);

    this->setMinimumSize(640, 480);

    this->m_layout->addWidget(this->m_pathLine, 0, 0, 1, 2);
    this->m_layout->addWidget(this->m_searchButton, 0, 2);
    this->m_layout->addWidget(this->m_startButton, 1, 0, 1, -1);
    this->m_layout->addWidget(this->m_loader, 1, 0, 1, -1);
    this->m_layout->addWidget(this->m_fsWidget, 2, 0, 1, -1);
    this->m_layout->addWidget(this->m_hideOkButton, 3, 0, 1, 1);
    this->m_layout->addWidget(this->m_showOkButton, 3, 1, 1, 1);
    this->m_layout->addWidget(this->m_processButton, 3, 2, 1, 1);

    this->m_loader->setRange(0, 0);
    this->m_loader->setHidden(true);

    this->m_pathLine->setMinimumWidth(250);
    this->m_pathLine->setPlaceholderText(QStringLiteral("Chemin"));
    this->m_pathLine->setReadOnly(true);

    this->m_hideOkButton->setDisabled(true);
    this->m_processButton->setDisabled(true);
    this->m_showOkButton->setDisabled(true);
    this->m_startButton->setDisabled(this->m_pathLine->text().isEmpty());

    this->connect(this->m_hideOkButton, &QPushButton::clicked, this, &MainWidget::hideOk);
    this->connect(this->m_searchButton, &QPushButton::clicked, this, &MainWidget::setPath);
    this->connect(this->m_showOkButton, &QPushButton::clicked, this, &MainWidget::showOk);
    this->connect(this->m_startButton, &QPushButton::clicked, this, &MainWidget::startCrawling);
}

// SLOTS

void    MainWidget::setPath(void)
{
    const QString searchPath = QFileDialog::getExistingDirectory(
        this,
        QStringLiteral("Selectionnez le dossier de recherche"),
        QDir::rootPath()
    );

    if (!searchPath.isEmpty() && !QDir(searchPath).exists()) {
        QMessageBox::warning(
            this,
            QStringLiteral(),
            QStringLiteral("Ce dossier n'existe pas")
        );
    } else {
        this->m_set.setValue(QStringLiteral("history/dir"), searchPath);
    }
    this->m_pathLine->setText(searchPath);
    this->m_startButton->setDisabled(searchPath.isEmpty());
}

void    MainWidget::startCrawling(void)
{
    this->m_fsWidget->clear();
    this->m_okItems.clear();
    this->m_startButton->setHidden(true);
    this->m_loader->setHidden(false);

    this->crawl(QFileInfo(this->m_pathLine->text()));

    this->m_loader->setHidden(true);
    this->m_startButton->setHidden(false);

    this->m_hideOkButton->setEnabled(true);
    this->m_showOkButton->setEnabled(true);
    this->m_processButton->setEnabled(true);
}

void    MainWidget::crawl(const QFileInfo &info, quint16 depth, bool last)
{
    bool            isOk(info.fileName().length() <= MAX_LENGTH);
    QFileInfoList   entriesList;
    QListWidgetItem *rootItem = new QListWidgetItem(this->m_fsWidget);
    QString         text;

    if (depth == 0) {
        text = info.canonicalFilePath();
        rootItem->setFlags(Qt::NoItemFlags);
    } else if (depth == 1) {
        text.append(last ? "└" : "├").append("── ").append(info.fileName());
    } else if (depth > 1) {
        text.append("│").append(QString(depth * 3 + 1, ' ')).append(last ? "└" : "├").append("── ").append(info.fileName());
    }

    rootItem->setData(MainWidget::FULLPATH, info.canonicalFilePath());
    rootItem->setData(MainWidget::VISIBILITY, isOk);
    rootItem->setText(text);
    if (depth != 0) {
        if (isOk) {
            rootItem->setHidden(isOk);
            this->m_okItems.append(rootItem);
        } else {
            rootItem->setTextColor(Qt::red);
        }
    }

    if (info.isDir()) {
        entriesList = QDir(info.canonicalFilePath()).entryInfoList(
            QDir::AllEntries | QDir::NoDotAndDotDot,
            QDir::Name
        );

        for (QFileInfoList::ConstIterator i(entriesList.cbegin()); i != entriesList.cend(); ++i) {
            this->crawl(*i, depth + 1, *i == entriesList.last());
        }
    }
}

void    MainWidget::hideOk(void)
{
    for (QLinkedList<QListWidgetItem*>::Iterator i(this->m_okItems.begin()); i != this->m_okItems.end(); ++i) {
        (*i)->setHidden(true);
    }
}

void    MainWidget::showOk(void)
{
    for (QLinkedList<QListWidgetItem*>::Iterator i(this->m_okItems.begin()); i != this->m_okItems.end(); ++i) {
        (*i)->setHidden(false);
    }
}

void    MainWidget::process(void)
{

}
