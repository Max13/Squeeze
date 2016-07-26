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

    QStringList args(qApp->arguments());
    int         i = args.indexOf(QStringLiteral("-l"), 1);
    quint8      maxLength = 250;
    if (i != -1 && args.length() >= i) {
        maxLength = static_cast<quint8>(args.at(i + 1).toUShort());
    }

    QSettings       s(QSettings::SystemScope, a.organizationName(), a.applicationName());
    MainWidget      w(s, maxLength);

    w.show();

    return a.exec();
}
