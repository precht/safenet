#include "ftpplugin.h"
//Q_EXPORT_PLUGIN2( FTPPlugin, FTPPlugin )

FtpPlugin::FtpPlugin(QObject *parent) : QObject(parent)
{
    Server = new FtpServer();
}

QString FtpPlugin::Name() const
{
    return "FTP Server Plugin";
}

QString FtpPlugin::Description() const
{
    return "Allows remote file transfers.";
}

QString FtpPlugin::Status() const
{
    QString ret = "Unknown";

    if(Server->isListening())
    {
        ret = "Started";
    }
    else
    {
        ret = "Stopped";
    }

    return ret;
}

QString FtpPlugin::Install() const
{
    return "Complete";
}

QString FtpPlugin::Uninstall() const
{
    return "Complete";
}

QString FtpPlugin::Start() const
{
    AppSettings Settings;

    Settings.beginGroup(Settings.DefaultGroup());
    QVariant vPort = Settings.value("port",21);
    Settings.endGroup();

    Server->startServer(vPort.toUInt());

    return Status();
}

QString FtpPlugin::Stop() const
{
    Server->stopServer();

    return Status();
}

QStringList FtpPlugin::Commands() const
{
    QStringList List;
    List.append("setusername (username) - Sets the username for the ftp server.");
    List.append("setpassword (password) - Sets the password for the ftp server.");
    List.append("setport (port) - Sets the port to run the telnet server on.");

    return List;
}

QString FtpPlugin::Exec(QString command, QStringList args) const
{

    return "not implimented";
}

