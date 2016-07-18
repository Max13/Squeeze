#include <QColor>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QString>
#include <QStringList>

#include "MainWidget.hpp"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    this->m_fsWidget = new QListWidget(this);
    this->m_layout = new QVBoxLayout(this);
    this->m_pathLine = new QLineEdit(this);
    this->m_searchLayout = new QGridLayout;
    this->m_searchButton = new QPushButton(QStringLiteral("Sélectionner"), this);
    this->m_startButton = new QPushButton(QStringLiteral("Démarrer"), this);

    this->setMinimumSize(640, 480);

    this->m_layout->addLayout(this->m_searchLayout);
    this->m_layout->addWidget(this->m_fsWidget);

    this->m_pathLine->setMinimumWidth(250);
    this->m_pathLine->setPlaceholderText(QStringLiteral("Chemin"));
    this->m_pathLine->setReadOnly(true);

    this->m_searchLayout->addWidget(this->m_pathLine, 0, 0);
    this->m_searchLayout->addWidget(this->m_searchButton, 0, 1);
    this->m_searchLayout->addWidget(this->m_startButton, 1, 0, 1, 2);

    this->connect(this->m_searchButton, &QPushButton::clicked, this, &MainWidget::setPath);
    this->connect(this->m_startButton, &QPushButton::clicked, this, &MainWidget::startCrawling);
}

void MainWidget::dump(const DirStruct *current, int depth, bool last)
{
    QString output;
    if (depth == -1) {
        if (!current->entries.isEmpty()) {
            this->dump(current->entries.first(), depth + 1);
        }
        return;
    }

    if (depth == 0) {

    } else if (depth == 1) {
        output.append(last ? "└" : "├").append("── ");
    } else if (depth > 1) {
        output.append("│").append(QString((depth - 1) * 3, ' ')).append(last ? "└" : "├").append("── ");
    }
    output.append(current->name);

    qDebug().noquote() << output;
    this->m_fsWidget->addItem(output);
    if (output.length() > 250) {
        this->m_fsWidget->item(this->m_fsWidget->count() - 1)->setTextColor(QColor(Qt::red));
    }

    if (!current->entries.isEmpty()) {
        foreach (const DirStruct *item, current->entries) {
            this->dump(item, depth + 1, item == current->entries.last());
        }
    }

    qDebug().resetFormat();
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
    }

    this->m_pathLine->setText(searchPath);
}

void    MainWidget::startCrawling(void)
{
    QDir        dir(this->m_pathLine->text());
    DirStruct   *entries = new DirStruct;

    entries->name = QDir::rootPath();
    entries->entries.append(this->crawl(QFileInfo(this->m_pathLine->text()), true));

    this->dump(entries);
    this->m_fsWidget->item(0)->setFlags(Qt::NoItemFlags);
}

DirStruct   *MainWidget::crawl(const QFileInfo &info, bool isRoot)
{
    DirStruct       *entry = new DirStruct;
    QFileInfoList   entriesList;

    entry->name = isRoot ? info.canonicalFilePath() : info.fileName();

    if (info.isDir()) {
        entriesList = QDir(info.canonicalFilePath()).entryInfoList(
            QDir::AllEntries | QDir::NoDotAndDotDot,
            QDir::Name
        );

        foreach (const QFileInfo &entryInfo, entriesList) {
            if (entryInfo.isDir()) {
                entry->entries.append(this->crawl(entryInfo));
            } else {
                DirStruct   *file = new DirStruct;
                file->name = entryInfo.fileName();
                entry->entries.append(file);
            }
        }
    }

    return entry;
}