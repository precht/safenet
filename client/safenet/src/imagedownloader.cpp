#include "imagedownloader.h"
#include "sslconfig.h"
#include "cipher.h"
#include <QCoreApplication>

ImageDownloader::ImageDownloader(QNetworkAccessManager *aManager, QObject *parent)
    : QObject(parent)
    , manager(aManager)
{
}

void ImageDownloader::doDownload(QString address){

  /*  connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));*/


    QString completeAddress = address.append(QString("download/image"));
    QUrl url(completeAddress);
    request.setUrl(url);


    //turned off ssl verification
    SslConfig aConfig(false);
    QSslConfiguration config = aConfig.getConfig();
    request.setSslConfiguration(config);

    request.setOriginatingObject(this);


    //login data for authorization
    QString username("test");
    QString password("test");
    QString concatenated = username + ":" + password;
    QByteArray data = concatenated.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;
    request.setRawHeader("Authorization", headerData.toLocal8Bit());


    QNetworkReply *reply = manager->get(request);

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

}


void ImageDownloader::replyFinished(QNetworkReply* reply)
{

    qDebug() << "Image download";

    if(reply->error())
    {
        qDebug() << "ERROR!";
        qDebug() << reply->errorString();
        qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    }
    else
    {

        qDebug() << reply->header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug() << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();
        qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
        qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        qDebug() << "Connection encrypted: " << reply->attribute(QNetworkRequest::ConnectionEncryptedAttribute).toString();

        QString fileToWrite = "downloadedimage.png";

        QFile *file = new QFile(fileToWrite);

        if(file->open(QFile::ReadWrite | QFile::Truncate))
        {
            file->write(reply->readAll());
            file->flush();
            file->close();
        }

        delete file;

    }

    decrypt();

    reply->deleteLater();
    reply->abort();


}


void ImageDownloader::decrypt(){
    CmtIeaCipher cipher;
    QString file("downloadedimage.png");
    QImage toDecrypt(file);
    cipher.decrypt(toDecrypt);
    toDecrypt.save("result.png");
}