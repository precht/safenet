#include "imageuploader.h"
#include "sslconfig.h"
#include "cipher.h"

#include <QDir>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>
#include <QBuffer>

ImageUploader::ImageUploader(QNetworkAccessManager *aManager, QObject *parent)
    : QObject(parent)
    , manager(aManager)
{
}


void ImageUploader::doUpload(QString address, QString filePath, QString destPath) {

    this->destPath = destPath;
    /*connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));*/

    // encrypt and prepare image to send
    QImage image(filePath);
    if (image.isNull()) {
        qDebug() << "Failed to load the image:" << filePath;
        return;
    }

    CmtIeaCipher cipher;
    QByteArray arr; // raw image
    QBuffer saveBuff(&arr);
    QBuffer readBuff(&arr);

    cipher.encrypt(image);
    saveBuff.open(QIODevice::WriteOnly);
    image.save(&saveBuff, "PNG");
    saveBuff.close();
    readBuff.open(QIODevice::ReadOnly);

    // send image

    QString completeAddress = address.append(QString("upload/image"));
    QUrl url(completeAddress);
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QString("multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW")));
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    multiPart->setBoundary("----WebKitFormBoundary7MA4YWxkTrZu0gW");

    //name of file seen by server
    qInfo() << "this is what i have: " << filePath;
    QRegularExpression re("\/([^\/]+)\\.\\w+$");
    QRegularExpressionMatch m = re.match(filePath);
    QString fileName = m.hasMatch() ? m.captured(1) + ".png" : "unknown_file_name.png";

    QString destinationFileName(fileName);
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\"" + destinationFileName + "\""));
    imagePart.setBodyDevice(&readBuff);
    multiPart->append(imagePart);

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

    QNetworkReply *reply = manager->post(request, multiPart);
    multiPart->setParent(reply);

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    delete multiPart;
}



void ImageUploader::replyFinished (QNetworkReply *reply)
{
    qDebug() << "-->>" << "Image upload";
    if(reply->error())
    {
        qDebug() << "-->>" << "ERROR!";
        qDebug() << "-->>" << reply->errorString();
    }

    else {
        qDebug() << "-->>" << reply->header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug() << "-->>" << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();
        qDebug() << "-->>" << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
        qDebug() << "-->>" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << "-->>" << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        qDebug() << "-->>" << "Connection encrypted: " << reply->attribute(QNetworkRequest::ConnectionEncryptedAttribute).toString();
    }

    reply->deleteLater();
}

void ImageUploader::encrypt(QImage original){
    CmtIeaCipher cipher;
    QImage other(original);
    cipher.encrypt(other);
    other.save(destPath + "encrypted.png");
}
