#include <QApplication>
#include "client.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGuiApplication::setApplicationDisplayName(Client::tr("TCP Client"));
    Client client;
    client.show();
    return app.exec();
}
