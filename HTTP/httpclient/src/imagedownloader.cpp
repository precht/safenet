#include "imagedownloader.h"
#include "sslconfig.h"
#include <QCoreApplication>

ImageDownloader::ImageDownloader(QNetworkAccessManager *aManager, QObject *parent) : manager(aManager), QObject(parent)
{

}

void ImageDownloader::doDownload(){

    qDebug() << "imagedownloaded do download";

    desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

  /*  connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));*/



    QUrl url("https://localhost:8085/api/download/image");

    request.setUrl(url);

    //turned off ssl verification
    SslConfig aConfig(false);
    QSslConfiguration config = aConfig.getConfig();
    request.setSslConfiguration(config);

    request.setOriginatingObject(this);

    manager->get(request);

}


void ImageDownloader::replyFinished(QNetworkReply* reply)
{
    qDebug() << "get";
    if(reply->error())
    {
        qDebug() << "ERROR!";
        qDebug() << reply->errorString();
    }
    else
    {

        qDebug() << reply->header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug() << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();
        qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
        qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

        QString fileToWrite = desktopPath + "/downloadedimage.png";
        QFile *file = new QFile(fileToWrite);

        if(file->open(QFile::ReadWrite | QFile::Truncate))
        {
            file->write(reply->readAll());
            file->flush();
            file->close();
        }
        delete file;
    }

    reply->deleteLater();
    reply->abort();

}
