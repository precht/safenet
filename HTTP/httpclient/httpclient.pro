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
    include/httpwindow.h

SOURCES += \
    src/imagedownloader.cpp \
    src/imageuploader.cpp \
    src/keydownloader.cpp \
    src/keyuploader.cpp \
    src/manager.cpp \
    src/sslconfig.cpp \
    src/httpwindow.cpp \
    src/main.cpp

RESOURCES += \
    certificates.qrc
