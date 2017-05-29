#include "keydownloader.h"
#include "sslconfig.h"

KeyDownloader::KeyDownloader(QNetworkAccessManager *aManager, QObject *parent) : manager(aManager), QObject(parent)
{

}

void KeyDownloader::doDownload(){

    desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

  /*  connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));*/


    QUrl url("https://localhost:8085/api/download/key");
    request.setUrl(url);

    SslConfig aConfig(true);
    QSslConfiguration config = aConfig.getConfig();
    request.setSslConfiguration(config);

    request.setOriginatingObject(this);
    QNetworkReply *reply = manager->get(request);
   /* connect( aReply, SIGNAL(sslErrors(const QList<QSslError> &)),
                 this, SLOT(sslError(const QList<QSslError> &)) );*/

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

}


void KeyDownloader::replyFinished (QNetworkReply *reply)
{
    qDebug() << "Key downloader";

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

        QString fileToWrite = desktopPath + "/downloadedkey.txt";
        QFile *file = new QFile(fileToWrite);
        //if(file->open(QFile::Append))
        if(file->open(QFile::ReadWrite | QFile::Truncate))
        {
            file->write(reply->readAll());
            file->flush();
            file->close();
        }
        delete file;
      }


    reply->deleteLater();
}

/*void KeyDownloader::sslError( const QList<QSslError> &errors )
{
    foreach( const QSslError &error, errors ) {
        qDebug() << "SSL Error: " << error.errorString();
    }
}*/
