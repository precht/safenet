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


class Manager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString picturesFolder READ picturesFolder WRITE setPicturesFolder NOTIFY picturesFolderChanged)
    Q_PROPERTY(QString downloadFolder READ downloadFolder WRITE setDownloadFolder NOTIFY downloadFolderChanged)

public:
    explicit Manager(QObject *parent = 0);
    Q_INVOKABLE void uploadKey();
    Q_INVOKABLE void uploadImage(QString fileURL);
    Q_INVOKABLE void downloadKey();
    Q_INVOKABLE void downloadImage(QString fileName);
    void setAddress(QString IPaddress);
    QString downloadFolder() const;
    QString picturesFolder() const;

signals:

    void downloadFolderChanged(QString downloadFolder);
    void picturesFolderChanged(QString picturesFolder);

public slots:
    void handle(QNetworkReply*);
    void setDownloadFolder(QString downloadFolder);
    void setPicturesFolder(QString picturesFolder);

private:
    QNetworkAccessManager *manager;
    KeyDownloader *kd;
    KeyUploader *ku;
    ImageDownloader *id;
    ImageUploader *iu;
    QString address;
    QString downloadFolder_;
    QString picturesFolder_;
    QString m_downloadFolder;
    QString m_picturesFolder;
};

#endif // MANAGER_H
