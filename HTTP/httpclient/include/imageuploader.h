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


class ImageUploader : public QObject
{
    Q_OBJECT
public:
    explicit ImageUploader(QNetworkAccessManager *aManager = 0, QObject *parent = 0);
    void doUpload(QString fileName);
signals:

public slots:
    void replyFinished (QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
};

#endif // UPLOADER_H
