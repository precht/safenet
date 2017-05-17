#include "ftpfilesocket.h"

FtpFileSocket::FtpFileSocket(QObject *parent)
{
    isUploading = false;
    filename = "";
}

void FtpFileSocket::connectEvents()
{
    try
    {
        //connect the signals
        connect(socket,SIGNAL(readyRead()),this,SLOT(socketReadyRead()),Qt::DirectConnection);
        connect(socket,SIGNAL(disconnected()),this,SLOT(socketDisconnected()),Qt::DirectConnection);
        connect(socket,SIGNAL(connected()),this,SLOT(socketConnected()),Qt::DirectConnection);
    }
    catch(QString err)
    {
        emit onError(err);
    }
    catch(...)
    {
        emit onError("Unknown error in ConnectEvents()");
    }
}

bool FtpFileSocket::isConnected()
{
    try
    {
        if(!socket) qDebug() << "Socket is NULL!";

        if(socket->state() == QTcpSocket::ConnectedState)
        {
            qDebug() << "isConnected = true";
            return true;
        }
        else
        {
            qDebug() << "isConnected = false";
            return false;
        }
    }
    catch(QString err)
    {
        emit onError(err);
        return false;
    }
    catch(...)
    {
        emit onError("Unknown error in isConnected()");
        return false;
    }
}

void FtpFileSocket::connectTo(FtpPort &portInfo)
{
    try
    {
        socket = new QTcpSocket(this);

        //connect the signals
        connectEvents();

        qDebug() << "-- File Socket Connecting to --" << portInfo.getIPAddress() << ":" << portInfo.getPort();

        //Connect to the remote computer
        socket->connectToHost(portInfo.getIPAddress(),portInfo.getPort());
        socket->waitForConnected();

        qDebug() << "-- File Socket State --" << socket->state();
        qDebug() << "-- File Socket Error String --" << socket->errorString();
    }
    catch(QString err)
    {
        emit onError(err);
    }
    catch(...)
    {
        emit onError("Unknown error in ConnectTo()");
    }
}

void FtpFileSocket::listenFor(FtpPort &portInfo, QString IPAddress)
{
    try
    {
        //Pick a random port and start listening
        if(listen(QHostAddress::Any))
        {
            //We found an open port, update the port info
            portInfo.setIPAddress(IPAddress);
            portInfo.setPort(serverPort());

            //Let connected objects know we are listening
            emit onListening();
        }
        else
        {
            emit onError("File socket is not listening!");
        }
    }
    catch(QString err)
    {
        emit onError(err);
    }
    catch(...)
    {
        emit onError("Unknown error in ListenFor()");
    }
}

void FtpFileSocket::upload(QString filename, qint64 position)
{
    try
    {
        //Set the transfer variables
        isUploading = true;
        this->filename = filename;

        //Truncate the file if needed
        if(position > 0)
        {
            QFile file(filename);
            if(!file.resize(position))
            {
                emit onError("File could not be truncated!");
                return;
            }
        }

        //Wait for data to come in - see SocketReadyRead()
    }
    catch(QString err)
    {
        emit onError(err);
    }
    catch(...)
    {
        emit onError("Unknown error in Upload()");
    }
}

void FtpFileSocket::Download(QString Filename, qint64 Position)
{
    try
    {
        //Set the transfer variables
        isUploading = false;
        this->filename = Filename;

        //Open the file
        QFile file(Filename);

        if(!file.open(QFile::ReadOnly))
        {
            emit onError("Could not open file!");
            return;
        }

        qDebug() << "*** FileSocket *** " << Filename;

        //Move to the requested position
        if(Position > 0)
        {
            file.seek(Position);
        }

        //Send the contents of the file
        while (!file.atEnd()) {
            QByteArray buffer = file.read(1024 * 8);
            sendResponse(buffer);
        }

        //Close the file
        file.close();

        //Tell connected objects we are done
        emit onFinished();

        //Close the socket once we are done
        closeSocket();
    }
    catch(QString err)
    {
        emit onError(err);
    }
    catch(...)
    {
        emit onError("Unknown error in Download()");
    }
}

void FtpFileSocket::incomingConnection(int socketDescriptor)
{
    try
    {
        //Clients will connect to the server in passive mode
        sessionID = socketDescriptor;
        socket = new QTcpSocket(this);

        //connect the signals
        connectEvents();

        if(!socket->setSocketDescriptor(sessionID))
        {
            qDebug() << "-- File Socket --" << sessionID << " Error binding socket: " << socket->errorString();
            emit onError("Error binding socket");
        }
        else
        {
            qDebug() << "-- File Socket --" << sessionID << " session Connected";
            //Let connected objects know we are connected
            emit onConnected();
        }

        //Stop listing for connections
        this->close();
    }
    catch(QString err)
    {
        emit onError(err);
    }
    catch(...)
    {
        emit onError("Unknown error in incomingConnection()");
    }
}

void FtpFileSocket::socketReadyRead()
{
    try
    {
        //Put the data into the file if the client is uploading
        if(isUploading == true && filename != "")
        {
            //for the sake of simplicity we store all files in append mode
            QFile file(filename);

            if(!file.open(QFile::Append))
            {
                emit onError("Could not open file!");
                return;
            }
            //Read the data in from the socket
            QByteArray buffer = socket->readAll();

            //Write the data to the file
            file.write(buffer);

            //Close the file incase the connection drops
            file.close();
        }
    }
    catch(QString err)
    {
        emit onError(err);
    }
    catch(...)
    {
        emit onError("Unknown error in SocketReadyRead()");
    }
}

void FtpFileSocket::socketConnected()
{
    try
    {
        //Let connected objects know we are connected
        emit onConnected();
    }
    catch(QString err)
    {
        emit onError(err);
    }
    catch(...)
    {
        emit onError("Unknown error in SocketConnected()");
    }
}


void FtpFileSocket::socketDisconnected()
{
    try
    {
        qDebug() << "******************** File Socket Disconnected";
        //Let connected objects know we disconnected
        emit onDisconnected();

        //Cleanup
        socket->deleteLater();
        this->deleteLater();
    }
    catch(QString err)
    {
        emit onError(err);
    }
    catch(...)
    {
        emit onError("Unknown error in Socket"
                     ""
                     "");
    }
}

void FtpFileSocket::sendResponse(QByteArray data)
{
    try
    {
        //qDebug() << "FILE: " << data;
        //Write the data to the socket
        socket->write(data);
    }
    catch(QString err)
    {
        emit onError(err);
    }
    catch(...)
    {
        emit onError("Unknown error in SendResponse()");
    }
}

void FtpFileSocket::sendResponse(QString data)
{
    try
    {
       sendResponse(data.toLatin1());
    }
    catch(QString err)
    {
        emit onError(err);
    }
    catch(...)
    {
        emit onError("Unknown error in SendResponse()");
    }
}

void FtpFileSocket::sendResponse(const char* data)
{
    try
    {
        sendResponse(QString::fromLatin1(data));
    }
    catch(QString err)
    {
        emit onError(err);
    }
    catch(...)
    {
        emit onError("Unknown error in ConnectEvents()");
    }
}

void FtpFileSocket::closeSocket()
{
    //mSocket->waitForBytesWritten();
    socket->close();
}

void FtpFileSocket::LIST(QString Path)
{
    try
    {
        //if we are not connected then we have a major problem
        if(!isConnected())
        {
            qDebug() << "!!!!!! FILE SOCKET NOT CONNECTED !!!!!!!!!!";
        }


        //Set the transfer variables
        isUploading = false;
        filename = "";

        qDebug() << "SENDING LIST";

        //permissions[space]number?[space]owner[space]group[space]filesize[space]date[space]filename

        QFileInfo fi(Path);

        if(!fi.exists())
        {
            //Not found
            emit onError("Directory not found");
        }
        else
        {
            //Found
            if(fi.isDir())
            {
                QDir dir(Path);

                //list a directory
                qDebug() << "LIST SEND DIRECTORY";
                foreach(QFileInfo item, dir.entryInfoList())
                {
                    FtpListItem ftpList;
                    sendResponse(ftpList.Convert(item));

                    //mSocket->waitForBytesWritten();
                }
                emit onFinished();
            }
            else
            {
                //list a single file
                qDebug() << "LIST SEND FILE";
                FtpListItem ftpList;
                sendResponse(ftpList.Convert(fi));
                emit onFinished();
            }
        }

        //close the socket once we are done
        closeSocket();

    }
    catch(QString err)
    {
        emit onError(err);
    }
    catch(...)
    {
        emit onError("Unknown error in LIST()");
    }
}

void FtpFileSocket::NLST(QString Path)
{

    try
    {
        //if we are not connected then we have a major problem
        if(!isConnected())
        {
            qDebug() << "!!!!!! FILE SOCKET NOT CONNECTED !!!!!!!!!!";
        }

        //Set the transfer variables
        isUploading = false;
        filename = "";

        QDir dir(Path);

        if(!dir.exists())
        {
            emit onError("Directory not found");
        }
        else
        {
            qDebug() << "NLST SEND";
            foreach(QString item, dir.entryList())
            {
                sendResponse(item + "\r\n");
                //mSocket->waitForBytesWritten();
            }
            emit onFinished();

        }

        //close the socket once we are done
        closeSocket();
    }
    catch(QString err)
    {
        emit onError(err);
    }
    catch(...)
    {
        emit onError("Unknown error in NLST()");
    }
}

void FtpFileSocket::MLSD(QString Path,QString Requested)
{
    try
    {
        //if we are not connected then we have a major problem
        if(!isConnected())
        {
            qDebug() << "!!!!!! FILE SOCKET NOT CONNECTED !!!!!!!!!!";
        }

        //Set the transfer variables
        isUploading = false;
        filename = "";


        QFileInfo fi(Path);
        QDir dir(Path);

        if(!fi.exists())
        {
            emit onError("Directory not found");
        }
        else
        {
            FtpMLSItem mls;

            qDebug() << "MLSD SEND";
            sendResponse(mls.RequestedPath(fi,Requested));

            foreach(QFileInfo item, dir.entryInfoList())
            {

                sendResponse(mls.Convert(item));
            }
            emit onFinished();

        }

        //close the socket once we are done
        closeSocket();

    }
    catch(QString err)
    {
        emit onError(err);
    }
    catch(...)
    {
        emit onError("Unknown error in MLSD()");
    }
}



