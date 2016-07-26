#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QDir>
#include <QFileInfo>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QLinkedList>
#include <QListWidget>
#include <QMutex>
#include <QPair>
#include <QProgressBar>
#include <QPushButton>
#include <QSettings>
#include <QString>
#include <QWidget>

typedef QLinkedList<QListWidgetItem*>   LinkedItems;

class MainWidget : public QWidget
{
    Q_OBJECT
    private:
        const quint8            &m_maxLength;
        LinkedItems             m_items;
        QGridLayout             *m_layout;
        QLabel                  *m_currentEntry;
        QLineEdit               *m_pathLine;
        QListWidget             *m_fsWidget;
        QProgressBar            *m_loader;
        QPushButton             *m_processButton;
        QPushButton             *m_searchButton;
        QPushButton             *m_startButton;
        QSettings               &m_set;

    public:
        enum    fsRole {
            FULLPATH    = Qt::UserRole,
        };

        MainWidget(QSettings &settings, const quint8 &maxLength = 250, QWidget *parent = 0);

    signals:

    public slots:
        void    setPath(void);
        void    startLoading(void);
        void    endLoading(void);
        void    startCrawling(void);
        void    crawl(const QFileInfo &info, quint16 depth = 0);
        void    openPath(const QModelIndex &index);
        void    process(void);
};

#endif // MAINWINDOW_HPP
