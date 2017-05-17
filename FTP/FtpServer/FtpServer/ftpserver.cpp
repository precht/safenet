#include "ftpserver.h"

FtpServer::FtpServer(QObject *parent)
{
}

//16-bit TCP port address
void FtpServer::startServer(qint16 port)
{ 
    //The dual stack any-address. A socket bound with this address will listen on both IPv4 and IPv6 interfaces.

    #if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
        if(!this->listen(QHostAddress::AnyIPv4, port))
    #else
        if(!this->listen(QHostAddress::Any, port))
    #endif
    {
        qDebug() << "Could not start ftp server";
    }
    else
    {
        qDebug() << "FTP Server Listening  on port " << port;

        //notify connected objects
        emit OnStarted();
    }
}

void FtpServer::stopServer()
{
    this->close();
    qDebug() << "Server stopped";

    //notify connected objects
    emit OnStopped();
}

//is called by QTcpServer when a new connection is available.
void FtpServer::incomingConnection(qintptr handle)
{
    qDebug() << " Connecting..." << handle;

    //Accept the incomming client
    FtpDataSocket *cClient = new FtpDataSocket(handle,this);
    connect(this,SIGNAL(OnStopped()),cClient,SLOT(CloseSocket()));
}
