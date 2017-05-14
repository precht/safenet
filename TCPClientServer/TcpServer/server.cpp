#include <QtWidgets>
#include <QtNetwork>
#include <QImage>

#include <stdlib.h>

#include "server.h"

Server::Server(QWidget *parent)
    : QDialog(parent)
    , statusLabel(new QLabel)
    , tcpServer(Q_NULLPTR)
    , networkSession(0)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    statusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &Server::sessionOpened);

        statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    } else {
        sessionOpened();
    }

        QPushButton *quitButton = new QPushButton(tr("Quit"));
        quitButton->setAutoDefault(false);
        connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);
        connect(tcpServer, &QTcpServer::newConnection, this, &Server::sendFile);

        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addStretch(1);
        buttonLayout->addWidget(quitButton);
        buttonLayout->addStretch(1);

        QVBoxLayout *mainLayout = Q_NULLPTR;
        if (QGuiApplication::styleHints()->showIsFullScreen() || QGuiApplication::styleHints()->showIsMaximized()) {
            QVBoxLayout *outerVerticalLayout = new QVBoxLayout(this);
            outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
            QHBoxLayout *outerHorizontalLayout = new QHBoxLayout;
            outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
            QGroupBox *groupBox = new QGroupBox(QGuiApplication::applicationDisplayName());
            mainLayout = new QVBoxLayout(groupBox);
            outerHorizontalLayout->addWidget(groupBox);
            outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
            outerVerticalLayout->addLayout(outerHorizontalLayout);
            outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
        } else {
            mainLayout = new QVBoxLayout(this);
        }

        mainLayout->addWidget(statusLabel);
        mainLayout->addLayout(buttonLayout);

        setWindowTitle(QGuiApplication::applicationDisplayName());
}

void Server::sessionOpened()
{
    // Save the used configuration
    if (networkSession) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id;
        if (config.type() == QNetworkConfiguration::UserChoice)
            id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
        else
            id = config.identifier();

        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }

    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen()) {
        QMessageBox::critical(this, tr("TCP Server"),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        close();
        return;
    }
    QString ipAddress;
    // provides a listing of the host's IP addresses and network interfaces.
    // This convenience function returns all IP addresses found on the host machine.
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    statusLabel->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                            "Run the TCP Client example now.")
                         .arg(ipAddress).arg(tcpServer->serverPort()));
}

//Returns the server's port if the server is listening for connections; otherwise returns 0.

void Server::sendFile()
{

    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected,
            clientConnection, &QObject::deleteLater);

    QStringList nameFilter("*.jpg");
    QString path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QDir directory(path);
    QStringList jpgFilesList = directory.entryList(nameFilter);
    QString result = path.append(QString("/")).append(jpgFilesList.at(0));

    qDebug() << result;
    QFile inputFile(result);
    inputFile.open(QIODevice::ReadOnly);

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint64)0;
    QByteArray proper_content = inputFile.readAll();
    block.append(proper_content);

    inputFile.close();
    out.device()->seek(0);
    out << (quint64)(block.size() - sizeof(quint64));

    qint64 x = 0;
    while(x < block.size())
    {
        qint64 y = clientConnection->write(block);
        x = x + y;
        qDebug() << x;
    }

    clientConnection->disconnectFromHost();
}
