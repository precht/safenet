#include "imageuploader.h"
#include "sslconfig.h"

ImageUploader::ImageUploader(QNetworkAccessManager *aManager, QObject *parent) : manager(aManager), QObject(parent)
{

}


void ImageUploader::doUpload() {


    qDebug() << "upload";
    /*connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));*/


    //file to be uploaded
    QString homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString absoluteFilePath(homePath + "/lena.png");


    QUrl url("https://localhost:8085/api/upload/image");
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QString("multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW")));


    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    multiPart->setBoundary("----WebKitFormBoundary7MA4YWxkTrZu0gW");


    //name of file seen by server
    QString destinationFileName("lena3.png");
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\"" + destinationFileName + "\""));
    QFile *file = new QFile(absoluteFilePath);
    file->open(QIODevice::ReadOnly);
    imagePart.setBodyDevice(file);

    multiPart->append(imagePart);

    //turned off ssl verification
    SslConfig aConfig(false);
    QSslConfiguration config = aConfig.getConfig();
    request.setSslConfiguration(config);

    request.setOriginatingObject(this);

    QNetworkReply *reply = manager->post(request, multiPart);
    multiPart->setParent(reply);
}



void ImageUploader::replyFinished (QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR!";
        qDebug() << reply->errorString();
    }

    else {
        qDebug() << reply->header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug() << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();
        qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
        qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    }

    reply->deleteLater();
}