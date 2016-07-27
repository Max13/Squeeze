#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QMimeData>
#include <QProcess>
#include <QString>
#include <QtConcurrent>

#include "Crawler.hpp"
#include "MainWidget.hpp"

MainWidget::MainWidget(QSettings &settings, const quint8 &maxLength, QWidget *parent)
    : QWidget(parent), m_maxLength(maxLength), m_set(settings)
{
    this->m_currentEntry = new QLabel(this);
    this->m_fsWidget = new QListWidget(this);
    this->m_layout = new QGridLayout(this);
    this->m_loader = new QProgressBar(this);
    this->m_pathLine = new QLineEdit(this->m_set.value(QStringLiteral("history/dir")).toString(), this);
    this->m_processButton = new QPushButton(QStringLiteral("Confirmer"), this);
    this->m_searchButton = new QPushButton(QStringLiteral("Sélectionner"), this);
    this->m_startButton = new QPushButton(QStringLiteral("Démarrer"), this);

    this->setMinimumSize(640, 480);

    this->m_currentEntry->setHidden(true);

    this->m_fsWidget->setAlternatingRowColors(true);
    this->m_fsWidget->setTextElideMode(Qt::ElideMiddle);
    this->m_fsWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->m_layout->addWidget(this->m_pathLine, 0, 0, 1, 2);
    this->m_layout->addWidget(this->m_searchButton, 0, 2);
    this->m_layout->addWidget(this->m_startButton, 1, 0, 1, -1);
    this->m_layout->addWidget(this->m_loader, 1, 0, 1, -1);
    this->m_layout->addWidget(this->m_currentEntry, 2, 0, 1, -1);
    this->m_layout->addWidget(this->m_fsWidget, 3, 0, 1, -1);
    this->m_layout->addWidget(this->m_processButton, 4, 0, 1, -1);

    this->m_loader->setRange(0, 0);
    this->m_loader->setHidden(true);

    this->m_pathLine->setMinimumWidth(250);
    this->m_pathLine->setPlaceholderText(QStringLiteral("Chemin"));
    this->m_pathLine->setReadOnly(true);

    this->m_processButton->setDisabled(true);
    this->m_startButton->setDisabled(this->m_pathLine->text().isEmpty());

    this->connect(this->m_fsWidget, &QListWidget::doubleClicked, this, &MainWidget::openPath);
    this->connect(this->m_searchButton, &QPushButton::clicked, this, &MainWidget::setPath);
    this->connect(this->m_startButton, &QPushButton::clicked, this, &MainWidget::startCrawling);
    this->connect(this->m_processButton, &QPushButton::clicked, this, &MainWidget::process);

    qRegisterMetaType<QFileInfo>("QFileInfo");
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

void    MainWidget::startLoading(void)
{
    this->m_startButton->setHidden(true);
    this->m_loader->setHidden(false);
    this->m_currentEntry->setHidden(false);
    this->setDisabled(true);
}

void    MainWidget::endLoading(void)
{
    this->m_currentEntry->setHidden(true);
    this->m_loader->setHidden(true);
    this->m_startButton->setHidden(false);

    this->m_processButton->setEnabled(true);
    this->setEnabled(true);
}

void    MainWidget::startCrawling(void)
{
    this->m_fsWidget->clear();
    this->m_items.clear();

    this->startLoading();

    Crawler *crawler = new Crawler(*this->m_currentEntry, this->m_maxLength, this);
    this->connect(crawler, &Crawler::fileFound, this, &MainWidget::addFound);
    this->connect(crawler, &Crawler::finished, this, &MainWidget::endLoading);
    this->connect(crawler, &Crawler::finished, crawler, &Crawler::deleteLater);
    QtConcurrent::run(crawler, &Crawler::crawl, QFileInfo(this->m_pathLine->text()), true);
}

void    MainWidget::addFound(const QFileInfo &info)
{
    QListWidgetItem *rootItem = new QListWidgetItem(info.fileName(), this->m_fsWidget);

    rootItem->setToolTip(info.canonicalFilePath());

    this->m_items.append(rootItem);
}

void    MainWidget::openPath(const QModelIndex &index)
{
    const QString   file(static_cast<QListWidgetItem*>(index.internalPointer())->toolTip());

#ifdef  Q_OS_OSX
    QProcess::startDetached(QStringLiteral("open"), QStringList() << QStringLiteral("-Rn") << file);
# elif  defined(Q_OS_WIN32)
    QProcess::startDetached(QString("explorer.exe /n,/start,\"%1\"").arg(QDir::toNativeSeparators(file)));
# else
#       error "OS Not supported"
#endif

}

void    MainWidget::process(void)
{
    QMessageBox::StandardButton ok;

    ok = QMessageBox::warning(
             this,
             tr("Êtes-vous sûr"),
             tr("%1 fichiers/dossiers trop longs ont été trouvés.\n"
                "Squeeze va simplement tronquer les noms pour arriver à\n"
                "%2 caractères maximum.\n\n"
                "Vous devez également fermer les applications qui pourraient\n"
                "utiliser ces fichiers.").arg(this->m_items.count()).arg(this->m_maxLength),
             QMessageBox::Cancel | QMessageBox::Ok,
             QMessageBox::Cancel
    );

    if (ok != QMessageBox::Ok) {
        return;
    }
}
