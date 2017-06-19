#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "cryptoimage.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<CryptoImage>("CryptoImage", 1, 0, "CryptoImage");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
