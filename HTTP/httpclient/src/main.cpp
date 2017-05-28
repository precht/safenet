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
    manager.downloadImage();
    //manager.uploadImage();


    return a.exec();

}
