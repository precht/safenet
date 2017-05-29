#include "manager.h"

Manager::Manager(QObject *parent) : QObject(parent)
{
    this->manager = new QNetworkAccessManager(this);

   connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(handle(QNetworkReply*)));


    kd = new KeyDownloader(manager);
    ku = new KeyUploader(manager);
    id = new ImageDownloader(manager);
    iu = new ImageUploader(manager);


}

void Manager::handle(QNetworkReply *reply) {
    QObject* object = reply->request().originatingObject();
    if (object == ku ) {
        qDebug() << "ku";
        ku->replyFinished(reply);
    }
    else if (object == kd ) {
        qDebug() << "kd";
        kd->replyFinished(reply);
    }

    else if (object == id ) {
        qDebug() << "id";
        id->replyFinished(reply);
    }
    else if (object == iu ) {
        qDebug() << "iu";
        iu->replyFinished(reply);
    }
}


void Manager::uploadKey() {
    ku->doUpload();
}


void Manager::uploadImage(QString fileName) {
    iu->doUpload(fileName);
}

void Manager::downloadKey() {
    kd->doDownload();
}

void Manager::downloadImage() {
    id->doDownload();
}
