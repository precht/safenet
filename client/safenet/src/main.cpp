#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QScopedPointer>
#include <QDebug>
#include <QStandardPaths>

#include "cryptoimage.h"
#include "manager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qInfo() << "\n\n\n";
    qInfo() << "App path:" << QCoreApplication::applicationDirPath();
    qInfo() << "Pictures path:" << QStandardPaths::locate(QStandardPaths::PicturesLocation, "", QStandardPaths::LocateDirectory);
    qInfo() << "Download path:" << QStandardPaths::locate(QStandardPaths::DownloadLocation, "", QStandardPaths::LocateDirectory);
    qInfo() << "\n\n\n";

    // register CrytpoIamge as QML component
    qmlRegisterType<CryptoImage>("CryptoImage", 1, 0, "CryptoImage");

    QScopedPointer<Manager> manager(new Manager());

    QQmlApplicationEngine engine;

    // add Manager as global property
    engine.rootContext()->setContextProperty("manager", manager.data());

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
