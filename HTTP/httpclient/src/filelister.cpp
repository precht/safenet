#include "filelister.h"

FileLister::FileLister(QNetworkAccessManager *aManager, QObject *parent) : manager(aManager), QObject(parent)
{

}


void FileLister::listFiles(QString address){
    QString completeAddress = address.append(QString("listfiles"));
    QUrl url(completeAddress);
    request.setUrl(url);

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

void FileLister::replyFinished(QNetworkReply* reply)
{

    qDebug() << "Listing files from server...";

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

        QString fileToWrite = "filelist.txt";
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
