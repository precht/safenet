/*
* FTPPort
* Creates and parses FTP port information
*/

#ifndef FTPPORT_H
#define FTPPORT_H

#include <QObject>
#include <QStringList>
#include <QDebug>

class FtpPort : public QObject
{
    Q_OBJECT
public:
    explicit FtpPort(QObject *parent = 0);
    FtpPort(QString aIPAddress, uint aPort, QObject *parent);
signals:

public slots:

public:

    QString getIPAddress();

    void setIPAddress(QString nIPAddress);

    quint16 getPort();

    void setPort(quint16 nPort);

    QString ToString();

    void FromString(QString Data);

    void clear();

private:
    QString IPAddress;     // IP Address
    quint16 port;          // port number

};

#endif // FTPPORT_H
