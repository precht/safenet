QT += core
QT += network
QT += gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += console

INCLUDEPATH += ./include

HEADERS += \
    include/imagedownloader.h \
    include/imageuploader.h \
    include/keydownloader.h \
    include/keyuploader.h \
    include/manager.h \
    include/sslconfig.h \
    include/httpwindow.h \
    include/cipher.h \
    include/key.h

SOURCES += \
    src/imagedownloader.cpp \
    src/imageuploader.cpp \
    src/keydownloader.cpp \
    src/keyuploader.cpp \
    src/main.cpp \
    src/manager.cpp \
    src/sslconfig.cpp \
    src/httpwindow.cpp \
    src/cipher.cpp \
    src/key.cpp

RESOURCES += \
    certificates.qrc
