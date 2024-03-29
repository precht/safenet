#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDateTime>
#include <QFile>
#include <QDebug>
#include <QStandardPaths>
#include <QSslKey>
#include <QEventLoop>

class KeyDownloader : public QObject
{
    Q_OBJECT
public:
    explicit KeyDownloader(QNetworkAccessManager *aManager = 0, QObject *parent = 0);
    void doDownload(QString address);

signals:

public slots:
    void replyFinished(QNetworkReply *reply);
    //void sslError( const QList<QSslError> &errors);

private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QString desktopPath;
};

#endif // DOWNLOADER_H
