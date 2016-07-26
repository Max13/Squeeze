#include <QApplication>
#include <QSettings>

#include "MainWidget.hpp"

int main(int argc, char *argv[])
{
    QApplication    a(argc, argv);

    a.setApplicationName(QStringLiteral("Squeeze"));
    a.setOrganizationDomain(QStringLiteral("rihan.fr"));
    a.setOrganizationName(QStringLiteral("rihan.fr"));
    a.setWindowIcon(QIcon(QStringLiteral(":/icon/svg")));

    QSettings       s(QSettings::SystemScope, a.organizationName(), a.applicationName());
    MainWidget      w(s);

    w.show();

    return a.exec();
}
