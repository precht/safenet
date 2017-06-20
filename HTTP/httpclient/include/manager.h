#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include "keydownloader.h"
#include "keyuploader.h"
#include "imageuploader.h"
#include "imagedownloader.h"
#include "filedeleter.h"
#include "filelister.h"


class Manager : public QObject
{
    Q_OBJECT
public:
    explicit Manager(QObject *parent = 0);
    void uploadKey();
    void uploadImage(QString fileName);
    void downloadKey();
    void downloadImage();
    void deleteFilesFromServer();
    void listFiles();
    void setAddress(QString IPaddress);

signals:

public slots:
    void handle(QNetworkReply*);

private:
    QNetworkAccessManager *manager;
    KeyDownloader *kd;
    KeyUploader *ku;
    ImageDownloader *id;
    ImageUploader *iu;
    FileDeleter *fd;
    FileLister *fl;
    QString address;
};

#endif // MANAGER_H
