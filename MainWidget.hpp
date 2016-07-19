#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#define MAX_LENGTH  250

#include <QDir>
#include <QFileInfo>
#include <QGridLayout>
#include <QLineEdit>
#include <QLinkedList>
#include <QListWidget>
#include <QPair>
#include <QPushButton>
#include <QSettings>
#include <QString>
#include <QWidget>

typedef struct  s_DirStruct {
        QString                     name;
        QLinkedList<s_DirStruct*>   entries;
} DirStruct;

class MainWidget : public QWidget
{
    Q_OBJECT
    private:
        QGridLayout *m_layout;
        QLineEdit   *m_pathLine;
        QListWidget *m_fsWidget;
        QPushButton *m_hideOkButton;
        QPushButton *m_searchButton;
        QPushButton *m_showOkButton;
        QPushButton *m_startButton;
        QSettings   &m_set;

    public:
        MainWidget(QSettings &settings, QWidget *parent = 0);
        void        dump(const DirStruct *current, int depth = -1, bool last = false);

    public slots:
        void        setPath(void);
        void        startCrawling(void);
        DirStruct   *crawl(const QFileInfo &info, bool isRoot = false);
        void        hideOk(void);
        void        showOk(void);
};

#endif // MAINWINDOW_HPP
