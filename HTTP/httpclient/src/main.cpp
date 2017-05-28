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

    Manager manager;
    //manager.downloadKey();
    //manager.uploadKey();

    //file to be downloaded at server working directory
    //manager.downloadImage();


    //file to be uploaded located in home directory
    QString fileNameForUpload = "lena.png";
    manager.uploadImage(fileNameForUpload);


    return a.exec();

}
