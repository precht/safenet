#ifndef SSLCONFIG_H
#define SSLCONFIG_H

#include <QSslKey>
#include <QSslConfiguration>
#include <QFile>


class SslConfig
{
 public:
    SslConfig(bool turnedOn);
    QSslConfiguration getConfig();
    ~SslConfig();
private:
   bool turnedOn;
   QSslConfiguration config;
   QFile *certificateFile;
   QFile *keyFile;
   QList<QSslCertificate> certificates;
};

#endif // SSLCONFIG_H
