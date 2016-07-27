#include <QDebug>
#include <QDir>

#include "Crawler.hpp"

Crawler::Crawler(QLabel &label, const quint8 &maxLength, QObject *parent)
    : QObject(parent), m_maxLength(maxLength), m_label(label)
{}

void    Crawler::crawl(const QFileInfo &info, bool signal)
{
    this->m_label.setText(info.canonicalFilePath());

    if (info.fileName().length() > this->m_maxLength) {
        emit this->fileFound(info);
    }

    if (info.isDir() && !info.isSymLink()) {
        QFileInfoList   entriesList = QDir(info.canonicalFilePath()).entryInfoList(
            QDir::AllEntries | QDir::NoDotAndDotDot,
            QDir::Name
        );

        foreach (const QFileInfo &file, entriesList) {
            this->crawl(file);
        }
    }

    if (signal) {
        emit this->finished();
    }
}
