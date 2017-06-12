#include <cipher.h>
#include <key.h>

#include <QCoreApplication>
#include <QImage>
#include <QDebug>
#include <QThread>
#include <QImageReader>
#include <QElapsedTimer>

#include <cstring>

// this define makes qt-creator wait for outputs from all threads
//#define INSIDE_QT_CREATOR

const QString HELP_MSG = "   Options:\n"
                         "   --enc\n"
                         "   --dec\n"
                         "   --in <file>\n"
                         "   --out <file>\n"
                         "   --cycles [1..2](=2)\n"
                         "   --help";

int finish(int val) {
#ifdef INSIDE_QT_CREATOR
    QThread::msleep(500);
#endif
    return val;
}

int main(int argc, char *argv[])
{
    bool encOption = false;
    bool decOption = false;
    int cyclesOption = 2;
    QString inPath;
    QString outPath;

    int p = 0;
    while (++p < argc) {
        if (std::strcmp(argv[p], "--help") == 0) {
            qInfo().noquote() << HELP_MSG;
            return finish(-1);
        }
        if (std::strcmp(argv[p], "--cycles") == 0) {
            int val = QString(argv[++p]).toInt();
            if (val == 1 || val == 2) {
                cyclesOption = val;
            } else {
                qInfo() << "WARNING: Wrong number of cycles, setting default (=2)";
            }
            continue;
        }
        if (std::strcmp(argv[p], "--enc") == 0) {
            encOption = true;
            continue;
        }
        if (std::strcmp(argv[p], "--dec") == 0) {
            decOption = true;
            continue;
        }
        if (std::strcmp(argv[p], "--in") == 0) {
            inPath = argv[++p];
            continue;
        }
        if (std::strcmp(argv[p], "--out") == 0) {
            outPath = argv[++p];
            continue;
        }
        qCritical().noquote() << "Unknown option:" << "'" + QString(argv[p]) + "'.";
        return finish(-1);
    }
    if (encOption & decOption) {
        qCritical() << "Cannot encrypt and decrypt at the same time.";
        return finish(-1);
    }
    if (!(encOption | decOption)) {
        qInfo() << "WARNING: Neither '--enc' nor '--dec' was specified.";
    }
    if (inPath.isEmpty()) {
        qCritical().noquote() << "No input image.";
        return finish(-1);
    }
//    if (outPath.isEmpty()) {
//        qCritical().noquote() << "No output image.";
//        return finish(-1);
//    }

    QImage image(inPath);
    if (image.isNull()) {
        qCritical().noquote() << "Failed to load image:" << "'" + inPath + "'.";
        return finish(-1);
    }

    CmtIeaCipher cipher(cyclesOption);

    QElapsedTimer timer;
    timer.start();

    if (encOption) {
        cipher.encrypt(image);
    } else if(decOption) {
        cipher.decrypt(image);
    }

    qreal sec = timer.elapsed();
    int min = sec / 60000;
    sec -= min * 60000;
    sec /= 1000;
    qInfo().noquote() << "cipher\t" + QString::number(min) + "m" + QString::number(sec) + "s";

    if (!outPath.isEmpty() && !image.save(outPath)) {
        qCritical().noquote() << "Failed to save image:" << "'" + outPath + "'.";
    }

    return finish(0);
}
