#include "manager.h"

Manager::Manager(QObject *parent) : QObject(parent)
{
    this->manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(handle(QNetworkReply*)));


    kd = new KeyDownloader(manager);
    ku = new KeyUploader(manager);
    id = new ImageDownloader(manager);
    iu = new ImageUploader(manager);\


    address = QString("https://localhost:8085/api/");

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
}


void Manager::uploadImage(QString fileName) {
    iu->doUpload(address, fileName);
}

void Manager::downloadKey() {
    kd->doDownload(address);
}

void Manager::downloadImage() {
    id->doDownload(address);
    id->decrypt();
}

void Manager::setAddress(QString IPaddress) {
    this-> address = "https://" + IPaddress + ":8085/api/";
}
