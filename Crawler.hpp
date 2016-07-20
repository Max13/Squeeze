#ifndef CRAWLER_HPP
#define CRAWLER_HPP

#define MAX_LENGTH  100

#include <QFileInfo>
#include <QLinkedList>
#include <QListWidget>
#include <QObject>

class Crawler : public QObject
{
    Q_OBJECT
    private:
        QLinkedList<QListWidgetItem*> &m_okItems;
        QListWidget                   *&m_fsWidget;

    public:
        Crawler(QListWidget *&fsWidget, QLinkedList<QListWidgetItem*> &okItems, QObject *parent = 0);

    signals:
        void    crawlFinished(void);

    public slots:
        void    crawl(const QFileInfo &info, quint16 depth = 0, bool last = false);
};

#endif // CRAWLER_HPP
