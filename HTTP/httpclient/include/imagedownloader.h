#ifndef IMAGEDOWNLOADER_H
#define IMAGEDOWNLOADER_H


#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDateTime>
#include <QFile>
#include <QDebug>
#include <QStandardPaths>

class ImageDownloader : public QObject
{
    Q_OBJECT
public:
    explicit ImageDownloader(QNetworkAccessManager *aManager = 0, QObject *parent = 0);
    void doDownload();

signals:

public slots:
    void replyFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QString desktopPath;
};

#endif // DOWNLOADER_H
