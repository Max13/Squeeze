#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QDir>
#include <QFileInfo>
#include <QGridLayout>
#include <QLineEdit>
#include <QLinkedList>
#include <QListWidget>
#include <QPair>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

typedef struct  s_DirStruct {
        QString                     name;
        QLinkedList<s_DirStruct*>   entries;
} DirStruct;

class MainWidget : public QWidget
{
    Q_OBJECT
    private:
        QGridLayout *m_searchLayout;
        QLineEdit   *m_pathLine;
        QListWidget *m_fsWidget;
        QPushButton *m_searchButton;
        QPushButton *m_startButton;
        QVBoxLayout *m_layout;

    public:
        MainWidget(QWidget *parent = 0);
        void        dump(const DirStruct *current, int depth = -1, bool last = false);

    public slots:
        void        setPath(void);
        void        startCrawling(void);
        DirStruct   *crawl(const QFileInfo &info, bool isRoot = false);
};

#endif // MAINWINDOW_HPP
