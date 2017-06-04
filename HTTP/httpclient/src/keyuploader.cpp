#include "keyuploader.h"
#include "sslconfig.h"

KeyUploader::KeyUploader(QNetworkAccessManager *aManager, QObject *parent) : manager(aManager), QObject(parent)
{

}

void KeyUploader::doUpload(QString address) {

  /*  connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));*/


    QString completeAddress = address.append(QString("upload/key"));
    QUrl url(completeAddress);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json {
        {"x", K.x},
        {"y", K.y},
        {"a", K.a},
        {"h", K.H},
        {"g1", K.G1},
        {"g2", K.G2},
    };


    request.setUrl(url);


    SslConfig aConfig(true);
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


    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
}


void KeyUploader::replyFinished (QNetworkReply *reply)
{
    qDebug() << "Key upload";

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
        qDebug() << "Connection encrypted: " << reply->attribute(QNetworkRequest::ConnectionEncryptedAttribute).toString();


    reply->deleteLater();

    }
}
