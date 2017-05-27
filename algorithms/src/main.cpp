#include <cipher.h>
#include <key.h>

#include <QCoreApplication>
#include <QImage>
#include <QDebug>
#include <QThread>
#include <QImageReader>
#include <QElapsedTimer>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        qCritical() << "Pass file name as application argument.";
        QThread::msleep(500);
        return -1;
    }

//    QImage original(5, 3, QImage::Format_ARGB32);
//    for (int i = 0; i < 5; i++) {
//        for (int j = 0; j < 3; j++) {
//            original.setPixel(i, j, 5 * j + i);
//        }
//    }

    QString fileName = QString::fromLocal8Bit(argv[1]);
    QImage original(fileName);
    if (original.isNull()) {
        qCritical() << "Failed to load image: " << fileName;
        QThread::msleep(500);
        return -1;
    }

    QElapsedTimer timer;
    timer.start();

    CmtIeaCipher cipher;
    QImage other(original);
    cipher.encrypt(other);
    other.save("encrypted1.jpg");
    cipher.decrypt(other);

    qInfo() << "(orginal == decrypted) is:" << (original == other);
    qInfo() << "time:" << timer.elapsed() << "ms";

    // qInfo runs on defferent thread, let's wait to make sure that msg gets to cerr before ending
    QThread::msleep(500);
    return 0;
}
