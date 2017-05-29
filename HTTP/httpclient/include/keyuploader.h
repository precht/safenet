#ifndef KEYUPLOADER_H
#define KEYUPLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QFile>
#include <QEventLoop>

class KeyUploader : public QObject
{
    Q_OBJECT

public:
    explicit KeyUploader(QNetworkAccessManager *aManager = 0, QObject *parent = 0);
    void doUpload();

signals:

public slots:
    void replyFinished (QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
};

#endif // KEYUPLOADER_H
