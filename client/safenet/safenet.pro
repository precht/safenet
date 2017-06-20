TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += src/main.cpp \
    src/cipher.cpp \
    src/key.cpp \
    src/cryptoimage.cpp \
    src/manager.cpp \
    src/imagedownloader.cpp \
    src/imageuploader.cpp \
    src/keydownloader.cpp \
    src/keyuploader.cpp \
    src/sslconfig.cpp \
    
HEADERS += \
    include/cipher.h \
    include/key.h \
    include/cryptoimage.h \
    include/imagedownloader.h \
    include/imageuploader.h \
    include/keydownloader.h \
    include/keyuploader.h \
    include/manager.h \
    include/sslconfig.h \

RESOURCES += \
    resource/qml.qrc \
    resource/certificates.qrc


INCLUDEPATH += include/

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -pedantic -O3

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
