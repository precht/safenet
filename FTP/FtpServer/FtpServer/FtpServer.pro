QT += core
QT += network
QT -= gui

CONFIG += c++11

TARGET = ftpserver
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app


HEADERS += \
    appsettings.h \
    ftpdatasocket.h \
    ftpfilesocket.h \
    ftplistitem.h \
    ftpmlsitem.h \
    ftpplugin.h \
    ftpport.h \
    ftpserver.h \
    PluginInterface.h

SOURCES += \
    appsettings.cpp \
    ftpdatasocket.cpp \
    ftpfilesocket.cpp \
    ftplistitem.cpp \
    ftpmlsitem.cpp \
    ftpplugin.cpp \
    ftpport.cpp \
    ftpserver.cpp \
    main.cpp
