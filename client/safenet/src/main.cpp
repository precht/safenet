#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QScopedPointer>
#include <QDebug>
#include <QStandardPaths>

#include "cryptoimage.h"
#include "manager.h"
#include "servermodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // register CrytpoIamge as QML component
    qmlRegisterType<CryptoImage>("CryptoImage", 1, 0, "CryptoImage");

    ServerModel serverModel;
    Manager manager(&serverModel);
    manager.updateServerModel();
    QQmlApplicationEngine engine;

    // add Manager and ServerModel as global properties
    engine.rootContext()->setContextProperty("manager", &manager);
    engine.rootContext()->setContextProperty("serverModel", &serverModel);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
