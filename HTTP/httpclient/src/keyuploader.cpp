#include "keyuploader.h"
#include "sslconfig.h"

KeyUploader::KeyUploader(QNetworkAccessManager *aManager, QObject *parent) : manager(aManager), QObject(parent)
{

}

void KeyUploader::doUpload() {

  /*  connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));*/


    QUrl url("https://localhost:8085/api/upload/key");


    QUrlQuery query;

    //in place of "akey" we need to add our key to be sent
    query.addQueryItem("key","akey");


    url.setQuery(query);
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    SslConfig aConfig(true);
    QSslConfiguration config = aConfig.getConfig();
    request.setSslConfiguration(config);
    request.setOriginatingObject(this);

    QNetworkReply *reply = manager->post(request, query.toString(QUrl::FullyEncoded).toUtf8());

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
}


void KeyUploader::replyFinished (QNetworkReply *reply)
{

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


    reply->deleteLater();

    }
}
