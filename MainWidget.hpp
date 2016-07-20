#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#define MAX_LENGTH  100

#include <QDir>
#include <QFileInfo>
#include <QGridLayout>
#include <QLineEdit>
#include <QLinkedList>
#include <QListWidget>
#include <QPair>
#include <QProgressBar>
#include <QPushButton>
#include <QSettings>
#include <QString>
#include <QWidget>

class MainWidget : public QWidget
{
    enum    fsRole {
        FULLPATH    = Qt::UserRole,
        VISIBILITY  = FULLPATH + 1,
    };

    Q_OBJECT
    private:
        QGridLayout *m_layout;
        QLineEdit   *m_pathLine;
        QLinkedList<QListWidgetItem*> m_okItems;
        QListWidget *m_fsWidget;
        QProgressBar    *m_loader;
        QPushButton *m_hideOkButton;
        QPushButton *m_processButton;
        QPushButton *m_searchButton;
        QPushButton *m_showOkButton;
        QPushButton *m_startButton;
        QSettings   &m_set;

    public:
        MainWidget(QSettings &settings, QWidget *parent = 0);

    public slots:
        void        setPath(void);
        void        startCrawling(void);
        void        crawl(const QFileInfo &info, quint16 depth = 0, bool last = false);
        void        hideOk(void);
        void        showOk(void);
        void        process(void);
};

#endif // MAINWINDOW_HPP
