#include "sslconfig.h"

SslConfig::SslConfig(bool turnedOn)
{
    this->turnedOn = turnedOn;
    config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::TlsV1_0);

    if (!turnedOn) {
       config.setPeerVerifyMode(QSslSocket::VerifyNone);
    }
    else {
        //certificate for SSL handshake
        certificateFile = new QFile(":/ssl/mycert.crt");
        certificateFile->open(QIODevice::ReadOnly);
        QSslCertificate certificate(certificateFile);
        keyFile = new QFile(":/ssl/mycert.key");
        keyFile->open(QIODevice::ReadOnly);
        QSslKey key(keyFile, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey);


        //protocol TLS V1_2
        config = QSslConfiguration::defaultConfiguration();
        config.setProtocol(QSsl::TlsV1_2);


        certificates.append(certificate);
        config.setCaCertificates(certificates);
        config.setPeerVerifyMode(QSslSocket::QueryPeer);
        config.setPrivateKey(key);

    }
}

QSslConfiguration SslConfig::getConfig() {
    return config;
}

SslConfig::~SslConfig() {
    if (turnedOn) {
        delete certificateFile;
        delete keyFile;
    }
}
