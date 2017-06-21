#ifndef UPLOADER_H
#define UPLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDateTime>
#include <QFile>
#include <QDebug>
#include <QHttpMultiPart>
#include <QFileInfo>
#include <QStandardPaths>
#include <QEventLoop>
#include <QImage>


class ImageUploader : public QObject
{
    Q_OBJECT
public:
    explicit ImageUploader(QNetworkAccessManager *aManager = 0, QObject *parent = 0);
    void doUpload(QString address, QString fileName, QString destPath);
    void encrypt(QImage original);
signals:

public slots:
    void replyFinished (QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QString destPath;
};

#endif // UPLOADER_H
