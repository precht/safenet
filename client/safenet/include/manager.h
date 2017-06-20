#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

class KeyDownloader;
class KeyUploader;
class ImageDownloader;
class ImageUploader;
class FileLister;
class ServerModel;

class Manager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString picturesFolder READ picturesFolder)
    Q_PROPERTY(QString downloadFolder READ downloadFolder)
    Q_PROPERTY(QString address READ address)

public:
    explicit Manager(ServerModel *serverModel, QObject *parent = 0);
    ~Manager();

    Q_INVOKABLE void uploadKey();
    Q_INVOKABLE void uploadImage(QString fileURL);
    Q_INVOKABLE void downloadKey();
    Q_INVOKABLE void downloadImage(QString fileName);
    Q_INVOKABLE void updateServerModel();
    Q_INVOKABLE void setIP(QString ip);
    Q_INVOKABLE void setPort(QString port);

    QString downloadFolder() const;
    QString picturesFolder() const;

    QString address() const;

public slots:
    void handle(QNetworkReply*);
//    void setDownloadFolder(QString downloadFolder);
//    void setPicturesFolder(QString picturesFolder);

private:
    void setAddress();

    QNetworkAccessManager *manager;
    KeyDownloader *kd;
    KeyUploader *ku;
    ImageDownloader *id;
    ImageUploader *iu;
    FileLister *fl;
    ServerModel *sm;
    QString m_ip;
    QString m_port;
    QString m_address;
    QString m_downloadFolder;
    QString m_picturesFolder;
};

#endif // MANAGER_H
