#ifndef CRAWLER_HPP
#define CRAWLER_HPP

#include <QFileInfo>
#include <QLabel>
#include <QObject>

class Crawler : public QObject
{
    Q_OBJECT
    private:
        const quint8    m_maxLength;
        QLabel          &m_label;

    public:
        Crawler(QLabel &label, const quint8 &maxLength = 255, QObject *parent = 0);

        void    crawl(const QFileInfo &info, bool signal = false);

    signals:
        void    currentlyOn(const QString &name);
        void    fileFound(const QFileInfo &file);
        void    finished(void);
};

#endif // CRAWLER_HPP
