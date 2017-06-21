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
#include <QEventLoop>
#include <QImage>
#include <QTimer>

class ImageDownloader : public QObject
{
    Q_OBJECT
public:
    explicit ImageDownloader(QNetworkAccessManager *aManager = 0, QObject *parent = 0);
    void doDownload(QString address, QString fileName, QString destPath);
    void decrypt();
signals:


public slots:
    void replyFinished(QNetworkReply *reply);


private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QString destPath;
};

#endif // DOWNLOADER_H
