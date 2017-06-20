#include "manager.h"

#include <QDebug>
#include <QStandardPaths>
#include <QUrl>

Manager::Manager(QObject *parent) : QObject(parent)
{
    this->manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(handle(QNetworkReply*)));


    kd = new KeyDownloader(manager);
    ku = new KeyUploader(manager);
    id = new ImageDownloader(manager);
    iu = new ImageUploader(manager);


    address = QString("https://localhost:8085/api/");

    downloadFolder_ = "file://" + QStandardPaths::locate(QStandardPaths::DownloadLocation, "", QStandardPaths::LocateDirectory);
    downloadFolder_[downloadFolder_.size() - 1] = ' ';
    downloadFolder_ = downloadFolder_.trimmed();

    picturesFolder_ = "file://" + QStandardPaths::locate(QStandardPaths::PicturesLocation, "", QStandardPaths::LocateDirectory);
    picturesFolder_[picturesFolder_.size() - 1] = ' ';
    picturesFolder_ = picturesFolder_.trimmed();
}

void Manager::handle(QNetworkReply *reply) {
    QObject* object = reply->request().originatingObject();
    if (object == ku ) {
        ku->replyFinished(reply);
    }
    else if (object == kd ) {
        kd->replyFinished(reply);
    }

    else if (object == id ) {
        id->replyFinished(reply);
    }
    else if (object == iu ) {
        iu->replyFinished(reply);
    }
}

void Manager::uploadKey() {
    ku->doUpload(address);
    qInfo() << "Uploading key...";
}


void Manager::uploadImage(QString fileURL) {
    QString name = QUrl(fileURL).toLocalFile();
    iu->doUpload(address, name);
    qInfo() << "Uploading image" << name << "...";

}

void Manager::downloadKey() {
    kd->doDownload(address);
    qInfo() << "Downloading key...";

}

void Manager::downloadImage(QString fileName) {
    id->doDownload(address);
    id->decrypt();
    qInfo() << "Downloading image" << fileName << "...";

}

void Manager::setAddress(QString IPaddress) {
    this-> address = "https://" + IPaddress + ":8085/api/";
    qInfo() << "Setting ip to:" << IPaddress + "...";
}

QString Manager::downloadFolder() const
{
    return downloadFolder_;
}

QString Manager::picturesFolder() const
{
    return picturesFolder_;
}

void Manager::setDownloadFolder(QString downloadFolder)
{
    downloadFolder_ = downloadFolder;
}

void Manager::setPicturesFolder(QString picturesFolder)
{
    picturesFolder_ = picturesFolder;
}
