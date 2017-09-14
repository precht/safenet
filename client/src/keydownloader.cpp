#include "keydownloader.h"
#include "sslconfig.h"

KeyDownloader::KeyDownloader(QNetworkAccessManager *aManager, QObject *parent)
    : QObject(parent)
    , manager(aManager)
{
}

void KeyDownloader::doDownload(QString address){

    desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

  /*  connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));*/



    QString completeAddress = address.append(QString("download/key"));
    QUrl url(completeAddress);
    request.setUrl(url);


    //ssl configuration
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



    QNetworkReply *reply = manager->get(request);


    //to see presumable Ssl errors
   /* connect(reply, SIGNAL(sslErrors(const QList<QSslError> &)),
                 this, SLOT(sslError(const QList<QSslError> &)) );*/

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

}


void KeyDownloader::replyFinished (QNetworkReply *reply)
{
    qDebug() << "-->>" << "Key download";

    if(reply->error())
    {
        qDebug() << "-->>" << "ERROR!";
        qDebug() << "-->>" << reply->errorString();
    }
    else
    {
        qDebug() << "-->>" << reply->header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug() << "-->>" << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();
        qDebug() << "-->>" << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
        qDebug() << "-->>" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << "-->>" << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        qDebug() << "-->>" << "Connection encrypted: " << reply->attribute(QNetworkRequest::ConnectionEncryptedAttribute).toString();

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
