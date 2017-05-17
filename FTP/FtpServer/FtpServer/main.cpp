#include <QtCore/QCoreApplication>
#include "ftpplugin.h"
#include "appsettings.h"
#include <QString>
#include <QDir>
#include <QDebug>
#include <QStandardPaths>
#include "ftplistitem.h"
#include "QFileInfo"
#include "ftpmlsitem.h"
#include "ftpport.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Safenet");
    QCoreApplication::setApplicationName("ImageSender");

    AppSettings Settings;
    Settings.beginGroup(Settings.DefaultGroup());

    Settings.setValue("anonymous",true);
    Settings.setValue("username","admin");
    Settings.setValue("password",Settings.HashPassword("password"));
    Settings.setValue("ipaddress","127.0.0.1");
    Settings.setValue("port","21");
    QString path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    Settings.setValue("rootpath", path);
    Settings.endGroup();

    FtpPlugin cPlugin;
    cPlugin.Start();

    return a.exec();
}
