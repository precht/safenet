QT += core
QT += network
QT += gui

CONFIG += c++11
CONFIG += console

INCLUDEPATH += ./include

HEADERS += \
    include/imagedownloader.h \
    include/imageuploader.h \
    include/keydownloader.h \
    include/keyuploader.h \
    include/manager.h \
    include/sslconfig.h

SOURCES += \
    src/imagedownloader.cpp \
    src/imageuploader.cpp \
    src/keydownloader.cpp \
    src/keyuploader.cpp \
    src/main.cpp \
    src/manager.cpp \
    src/sslconfig.cpp

RESOURCES += \
    certificates.qrc
