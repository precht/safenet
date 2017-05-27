QT += core
QT += gui
#QT += widgets

TARGET = algorithms
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ./include

SOURCES += src/main.cpp \
    src/cipher.cpp \
    src/key.cpp

HEADERS += \
    include/cipher.h \
    include/key.h


QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -pedantic -O3
