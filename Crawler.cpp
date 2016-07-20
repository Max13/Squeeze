#include <QDir>
#include <QString>

#include "Crawler.hpp"
#include "MainWidget.hpp"

Crawler::Crawler(QListWidget *&fsWidget, QLinkedList<QListWidgetItem*> &okItems, QObject *parent)
    : QObject(parent), m_okItems(okItems), m_fsWidget(fsWidget)
{}

void    Crawler::crawl(const QFileInfo &info, quint16 depth, bool last)
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
        text.append("│").append(QString((depth - 1) * 3, ' ')).append(last ? "└" : "├").append("── ").append(info.fileName());
    }

    rootItem->setData(MainWidget::FULLPATH, info.canonicalFilePath());
    rootItem->setData(MainWidget::VISIBILITY, isOk);
    rootItem->setFont(rootItem->font());
    rootItem->setText(text);
    if (depth != 0) {
        if (isOk) {
            rootItem->setHidden(true);
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

    if (depth == 0) {
        emit this->crawlFinished();
    }
}
