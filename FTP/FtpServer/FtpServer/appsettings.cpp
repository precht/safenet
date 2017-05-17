#include "appsettings.h"


AppSettings::AppSettings(QObject *parent) :
    QSettings(ApplicationPath() + "/Settings.ini",QSettings::IniFormat,parent)
{
}

QString AppSettings::ApplicationPath()
{
    return QDir::currentPath();
}

QDir AppSettings::ApplicationDir()
{
    QDir path(QDir::currentPath());
    return path;
}

QString AppSettings::DefaultGroup()
{
    return "FTPPlugin";
}

QString AppSettings::HashPassword(QString Password)
{
    QByteArray hash = QCryptographicHash::hash(Password.toLatin1(),QCryptographicHash::Sha1);
    return QString(hash.toBase64());
}
