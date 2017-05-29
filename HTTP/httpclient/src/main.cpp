#include <QCoreApplication>
#include <QImage>
#include <QThread>
#include "keydownloader.h"
#include "keyuploader.h"
#include "imageuploader.h"
#include "imagedownloader.h"
#include "manager.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString argument = QString(argv[1]), send = QString("send"), get = QString("get");

    Manager manager;

    if (argc < 2) {
        qDebug() << "Wrong number of arguments!";
        return -1;
    }

    if (argument == send) {

        qDebug() << "send";
        manager.uploadKey();

        //file to be uploaded located in home directory
        QString fileNameForUpload = "lena.png";
        manager.uploadImage(fileNameForUpload);

    }

    else if (argument == get) {
        manager.downloadKey();

        //file to be downloaded at server working directory
        manager.downloadImage();
    }

    else {
        qDebug() << "Something wrong with arguments!";
        return -1;
    }

    return a.exec();

}
