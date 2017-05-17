#include "ftpdatasocket.h"

FtpDataSocket::FtpDataSocket(int SocketDescriptor, QObject *parent) :
    QObject(parent)
{
    try
    {
        loadResponses();
        loadSettings();

        sessionID = SocketDescriptor;

        socket = new QTcpSocket(this);
        if(!socket->setSocketDescriptor(sessionID))
        {
            qDebug() << sessionID << " Error binding socket";
            return;
        }

        //connect the signals
        connect(socket,SIGNAL(readyRead()),this,SLOT(socketReadyRead()),Qt::DirectConnection);
        connect(socket,SIGNAL(disconnected()),this,SLOT(socketDisconnected()),Qt::DirectConnection);

        qDebug() << sessionID << " session Connected";

        //Send the banner
        sendResponse(getResponse(220));

    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in Constructor()");
    }
}

void FtpDataSocket::loadResponses()
{
    try
    {
        responseCodes.insert(110,"Restart marker reply.");
        responseCodes.insert(120,"Service ready in nnn minutes.");
        responseCodes.insert(125,"Data connection already open; transfer starting.");
        responseCodes.insert(150,"File status okay; about to open data connection.");
        responseCodes.insert(200,"Command okay.");
        responseCodes.insert(202,"Command not implemented, superfluous at this site.");
        responseCodes.insert(211,"System status, nothing to report.");
        responseCodes.insert(212,"Directory status.");
        responseCodes.insert(213,"End of status.");
        responseCodes.insert(214,"Help command successful.");
        responseCodes.insert(215,"NAME system type.");
        responseCodes.insert(220,"Service ready for new user.");
        responseCodes.insert(221,"Service closing control connection.");
        responseCodes.insert(225,"Data connection open; no transfer in progress.");
        responseCodes.insert(226,"Closing data connection.");
        responseCodes.insert(227,"Entering Passive Mode (h1,h2,h3,h4,p1,p2).");
        responseCodes.insert(230,"User logged in, proceed.");
        responseCodes.insert(250,"Requested file action okay, completed.");
        responseCodes.insert(257,"Path was created.");
        responseCodes.insert(331,"Password required.");
        responseCodes.insert(332,"Need account for login.");
        responseCodes.insert(350,"Requested file action pending further information.");
        responseCodes.insert(421,"Service not available, closing control connection.");
        responseCodes.insert(425,"Can't open data connection.");
        responseCodes.insert(426,"Connection closed; transfer aborted.");
        responseCodes.insert(450,"Requested file action not taken.");
        responseCodes.insert(451,"Requested action aborted: local error in processing.");
        responseCodes.insert(452,"Requested action not taken.");
        responseCodes.insert(500,"Syntax error, command unrecognized.");
        responseCodes.insert(501,"Syntax error in parameters or arguments.");
        responseCodes.insert(502,"Command not implemented.");
        responseCodes.insert(503,"Bad sequence of commands.");
        responseCodes.insert(504,"Command not implemented for that parameter.");
        responseCodes.insert(530,"Not logged in.");
        responseCodes.insert(532,"Need account for storing files.");
        responseCodes.insert(550,"Requested action not taken.");
        responseCodes.insert(551,"Requested action aborted: page type unknown.");
        responseCodes.insert(552,"Requested file action aborted.");
        responseCodes.insert(553,"Requested action not taken.");
        responseCodes.insert(550,"The filename, directory name, or volume label syntax is incorrect.");
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in LoadResponses()");
    }
}

void FtpDataSocket::createFileSocket()
{
    try
    {
        //Create the file socket
        fileSocket = new FtpFileSocket(this);

        //Connect the events
        connect(fileSocket,SIGNAL(onConnected()),this,SLOT(onFileConnected()));
        connect(fileSocket,SIGNAL(onDisconnected()),this,SLOT(onFileDisconnected()));
        connect(fileSocket,SIGNAL(onError(QString)),this,SLOT(onFileError(QString)));
        connect(fileSocket,SIGNAL(onFinished()),this,SLOT(onFileFinished()));
        connect(fileSocket,SIGNAL(onListening()),this,SLOT(onFileListening()));
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in CreateFileSocket()");
    }
}

void FtpDataSocket::socketReadyRead()
{
    try
    {
        //Append to the command buffer
        commandBuffer.append(socket->readAll());

        //Check to see if it is the end of line
        if(commandBuffer.endsWith('\n'))
        {
            processCommand(commandBuffer);
            commandBuffer.clear();
        }

    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in SocketReadyRead()");
    }


}

void FtpDataSocket::socketDisconnected()
{
    try
    {
        qDebug() << sessionID << "************************ DISCONNECTED **************************";

        //Cleanup
        socket->deleteLater();
        this->deleteLater();
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in SocketDisconnected()");
    }
}

void FtpDataSocket::sendResponse(const char* data)
{

    try
    {
        sendResponse(QString::fromLatin1(data));
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in SendResponse()");
    }
}

void FtpDataSocket::sendResponse(QString data)
{

    try
    {
        sendResponse(data.toLatin1());
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in SendResponse()");
    }
}

void FtpDataSocket::sendResponse(int ResponseCode)
{

    try
    {
        sendResponse(getResponse(ResponseCode));
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in SendResponse()");
    }
}

void FtpDataSocket::sendResponse(int ResponseCode, QString ResponseText)
{
    try
    {
        QString ret = QString::number(ResponseCode) + " " + ResponseText + "\r\n";
        sendResponse(ret);
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in SendResponse()");
    }

}

void FtpDataSocket::sendResponse(QByteArray data)
{
    try
    {
        qDebug() << "Server: " << data;

        //Send the data to the client
        socket->write(data);
        //mSocket->waitForBytesWritten();
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in SendResponse()");
    }

}

void FtpDataSocket::closeSocket()
{

    try
    {
        // mSocket->waitForBytesWritten();
         socket->close();
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in CloseSocket()");
    }
}

int FtpDataSocket::ID()
{
    return sessionID;
}

QString FtpDataSocket::getResponse(int ResponseCode)
{
    QString ret = "";

    try
    {
        if(responseCodes.contains(ResponseCode))
        {
            ret = QString::number(ResponseCode) + " " + responseCodes.value(ResponseCode);
        }
        else
        {
             ret = QString::number(550) + " " + responseCodes.value(550);
        }


    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in GetResponse()");
    }

    return ret + "\r\n";
}

void FtpDataSocket::processCommand(QString ClientCommand)
{
    try
    {
        //Some clients like to put multiple commands on a single packet command1<crlf>command2<crlf>
        //Bad form yes but we have to deal with it regardless

        QStringList Commands = ClientCommand.split("\n");

        foreach(QString Command, Commands)
        {
            QString ClientCommand = Command.trimmed();
            if(ClientCommand != "")
            {
                //Execute the command
                executeCommand(ClientCommand);
            }
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in ProcessCommand()");
    }
}

void FtpDataSocket::executeCommand(QString ClientCommand)
{
    try
    {
        QString Command = "";
        QString Arg = "";

        if(ClientCommand.contains(" "))
        {
            //Contains arquments
            int pos = ClientCommand.indexOf(" ");
            Command = ClientCommand.mid(0,pos).trimmed();
            Arg = ClientCommand.mid(pos + 1).trimmed();
        }
        else
        {
            //No arquments
            Command = ClientCommand.trimmed();
        }

        qDebug() << "Client: " << Command << " " << Arg;

        QMetaObject metaObject = this->staticMetaObject;
        QMetaEnum metaEnum = metaObject.enumerator(metaObject.indexOfEnumerator("RequestCommands"));

        switch(metaEnum.keysToValue(Command.toUpper().toLatin1()))
        {

            // RFC 959  --------------------------------------------------
            case HELP: // Lists all available commands
            DoHelp();
            break;

            case QUIT: // Log out
            DoQuit();
            break;

            case USER: // Username
            DoUser(Arg);
            break;

            case PASS: // Password
            DoPass(Arg);
            break;

            case ACCT: // Account
            DoACCT(Arg);
            break;

            case CWD: // Change working directory
            DoCWD(Arg);
            break;

            case CDUP: // Change to parent directory
            DoCDUP();
            break;

            case SMNT: // Structure Mount
            DoSMNT(Arg);
            break;

            case REIN: // Reinitialize
            DoREIN();
            break;

            case PORT: // PORT h1,h2,h3,h4,p1,p2
            DoPORT(Arg);
            break;

            case PASV: // Passive mode
            DoPASV(Arg);
            break;

            case TYPE: // Representation Type
            DoTYPE(Arg);
            break;

            case STRU: // File Structure
            DoSTRU(Arg);
            break;

            case MODE: // Transfer Mode
            DoMODE(Arg);
            break;

            case RETR: // Retrieve a file
            DoRETR(Arg);
            break;

            case STOR: // Store a file
            DoSTOR(Arg);
            break;

            case STOU: // Store a Unique file
            DoSTOU(Arg);
            break;

            case APPE: // Append a file
            DoAPPE(Arg);
            break;

            case ALLO: // Allocate space
            DoALLO(Arg);
            break;

            case RNFR: // Rename from
            DoRNFR(Arg);
            break;

            case RNTO: // Rename to
            DoRNTO(Arg);
            break;

            case ABOR: // Abort
            DoABOR(Arg);
            break;

            case DELE: // Delete
            DoDELE(Arg);
            break;

            case RMD: // Remove Directory
            DoRMD(Arg);
            break;

            case MKD: // Make Directory
            DoMKD(Arg);
            break;

            case PWD: // Print Working Directory
            DoPWD();
            break;

            case LIST: // List a directory
            DoLIST(Arg);
            break;

            case NLST: // Name List
            DoNLST(Arg);
            break;

            case SITE: // Site Parameters
            DoSITE(Arg);
            break;

            case SYST: // System
            DoSYST();
            break;

            case STAT: // Status
            DoSTAT(Arg);
            break;

            case NOOP: // No Operation
            sendResponse(200);
            break;

            //RFC 3659 --------------------------------------------------
            case SIZE: // Returns the size of a file
            DoSize(Arg);
            break;

            case REST: // Restart failed transfer
            DoREST(Arg);
            break;

            case MLST: // Get information about an entity
            DoMLST(Arg);
            break;

            case MLSD: // Get information about an directory entity
            DoMLSD(Arg);
            break;

            case FEAT: // Client wants a detailed list of features
            DoFEAT();
            break;

            //RFC 755  --------------------------------------------------
            case XCUP: // Changes to the parent
            DoCDUP();
            break;

            case XMKD: // Creates a directory
            DoMKD(Arg);
            break;

            case XPWD: // Prints the current directory
            DoPWD();
            break;

            case XRMD: // Removes a directory
            DoRMD(Arg);
            break;


            default://Command not recognized
            sendResponse(500);
            break;
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in ExecuteCommand()");
    }
}

void FtpDataSocket::DoHelp()
{
    try
    {
        QString ret;
        ret.append("214 The following commands are available:");

        ret.append(" ABOR\r\n");
        ret.append(" ALLO\r\n");
        ret.append(" APPE\r\n");
        ret.append(" CDUP\r\n");
        ret.append(" CWD\r\n");
        ret.append(" DELE\r\n");
        ret.append(" FEAT\r\n");
        ret.append(" HELP\r\n");
        ret.append(" LIST\r\n");
        ret.append(" MLSD\r\n");
        ret.append(" MLST\r\n");
        ret.append(" MKD\r\n");
        ret.append(" MODE\r\n");
        ret.append(" NLST\r\n");
        ret.append(" NOOP\r\n");
        ret.append(" PASS\r\n");
        ret.append(" PASV\r\n");
        ret.append(" PWD\r\n");
        ret.append(" QUIT\r\n");
        ret.append(" REIN\r\n");
        ret.append(" REST\r\n");
        ret.append(" RETR\r\n");
        ret.append(" RMD\r\n");
        ret.append(" RNFR\r\n");
        ret.append(" RNTO\r\n");
        ret.append(" SIZE\r\n");
        ret.append(" SMNT\r\n");
        ret.append(" STAT\r\n");
        ret.append(" STOR\r\n");
        ret.append(" STOU\r\n");
        ret.append(" STRU\r\n");
        ret.append(" SYST\r\n");
        ret.append(" TYPE\r\n");
        ret.append(" USER\r\n");
        ret.append(" XCDUP\r\n");
        ret.append(" XPWD\r\n");
        ret.append(" XRMD\r\n");

        sendResponse(ret);
        sendResponse(214);
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoHelp()");
    }
}

void FtpDataSocket::DoFEAT()
{
    try
    {

        QString ret;
        ret.append("211-Extension supported\r\n");

        ret.append("ABOR\r\n");
        ret.append("ALLO\r\n");
        ret.append("APPE\r\n");
        ret.append("CDUP\r\n");
        ret.append("CWD\r\n");
        ret.append("DELE\r\n");
        ret.append("FEAT\r\n");
        ret.append("HELP\r\n");
        ret.append("LIST\r\n");
        ret.append("MLSD Type*;Size*;Create;Modify*;Win32.ea*;\r\n");
        ret.append("MLST Type*;Size*;Create;Modify*;Win32.ea*;\r\n");
        ret.append("MKD\r\n");
        ret.append("MODE\r\n");
        ret.append("NLST\r\n");
        ret.append("NOOP\r\n");
        ret.append("PASS\r\n");
        ret.append("PASV\r\n");
        ret.append("PWD\r\n");
        ret.append("QUIT\r\n");
        ret.append("REIN\r\n");
        ret.append("REST\r\n");
        ret.append("RETR\r\n");
        ret.append("RMD\r\n");
        ret.append("RNFR\r\n");
        ret.append("RNTO\r\n");
        ret.append("SIZE\r\n");
        ret.append("SMNT\r\n");
        ret.append("STAT\r\n");
        ret.append("STOR\r\n");
        ret.append("STOU\r\n");
        ret.append("STRU\r\n");
        ret.append("SYST\r\n");
        ret.append("TYPE\r\n");
        ret.append("USER\r\n");
        ret.append("XCDUP\r\n");
        ret.append("XPWD\r\n");
        ret.append("XRMD\r\n");

        sendResponse(ret);
        sendResponse(211);
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoFEAT()");
    }
}

void FtpDataSocket::DoQuit()
{

    try
    {
        sendResponse(221);
        //mSocket->waitForBytesWritten();
        closeSocket();
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoQuit()");
    }
}

bool FtpDataSocket::checkAuthenticated()
{
    if(!authenticated)
    {
        sendResponse(530);
    }

    return authenticated;
}

QString FtpDataSocket::relativePath()
{
    QString RelPath;

    try
    {
        if(RelPath.toUpper() == rootPath.toUpper())
        {
            RelPath = "/";
        }
        else
        {
            RelPath = currentPath.mid(rootPath.length());
            if(RelPath.length() == 0)
            {
                RelPath = "/";
            }
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in RelativePath()");
    }

    return RelPath;
}

QString FtpDataSocket::clientPath(QString Requested)
{
    //Some clients ask for a full path others only give a name

    QString temp = "";
    QString ret = "";
    try
    {

        //make sure the root path is configured
        if(rootPath == "") return "";

        //Make sure the client has a current path
        if(currentPath == "") currentPath = rootPath;

        //If we get a blank request, set it ot the current path
        if(Requested == "") Requested = currentPath;

        //Find the local path
        if(Requested.startsWith("/"))
        {
            //They are requesting with a full path
            if(Requested == "/")
            {
                //they want the root path
                temp = rootPath;
            }
            else
            {
                //they want a sub path
                temp = rootPath + Requested;
            }
        }
        else
        {
            //they are only requesting with a name
            if(!currentPath.endsWith("/"))
            {
                currentPath.append("/");
            }
            temp = currentPath + Requested;
        }



        ret = QDir::cleanPath(temp);

        qDebug() << "PATH temp = " << temp;
        qDebug() << "PATH ret = " << ret;

        //TO DO - make sure the path is allowed - ie, within the root
        QDir rootpath(temp);
        QDir retpath(ret);

        //make sure it is within root
        if(!retpath.absolutePath().contains(rootpath.absolutePath()))
        {
            ret = "";
        }



    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in ClientPath()");
    }

    //return a cleaned path
    return ret;
}

bool FtpDataSocket::connectFileSocket()
{
    qDebug() << "*** ConnectFileSocket";

    bool ret = false;
    try
    {
        if(!passive)
        {
            //Tell the client we are going to open the file socket
            sendResponse(150);

            //Open a file socket and connect to the port specified
            fileSocket->connectTo(portInfo);
        }
        else
        {
            //make sure we are listening as they should have send a PASV command
            sendResponse(125);
        }

        if(fileSocket)
        {
            qDebug() << "*** FileSocket not null";
            qDebug() << fileSocket->isConnected();
            qDebug() << "FileSocket " << fileSocket;


            if(fileSocket->isConnected())
            {
                qDebug() << "** FileSocket is connected **";
                ret = true;
            }
            else
            {
                qDebug() << "-- FileSocket not connected --";
                qDebug() << "-- Passive -- " << passive;
                qDebug() << "-- Connected -- " << fileSocket->isConnected();
                qDebug() << "-- Listening -- " << fileSocket->isListening();
                sendResponse(503, "FILE SOCKET NOT CONNECTED!");
                ret = false;
            }
        }
        else
        {
            qDebug() << "No File Socket";
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in ConnectFileSocket()");
    }

    return ret;
}

void FtpDataSocket::logonUser()
{
    try
    {
        //Set the Authenticated flag
        authenticated = true;

        //Send the Logged in message
        sendResponse(230);
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in LogonUser()");
    }
}

void FtpDataSocket::loadSettings()
{
    try
    {
        //Load the general settings
        AppSettings Settings;
        Settings.beginGroup(Settings.DefaultGroup());
        authenticated = false;
        anonymous = Settings.value("anonymous", false).toBool();
        username = Settings.value("username","").toString();
        password = Settings.value("password","").toString();
        account = Settings.value("account","").toString();
        rootPath = Settings.value("rootpath","").toString().replace("\\","/");;
        currentPath = rootPath;
        listRoots = Settings.value("listroots",false).toBool();
        passive = false;
        allowSMNT = false;
        filePosition = 0;
        IPAddress = Settings.value("ipaddress","127.0.0.1").toString();

        if(!rootPath.endsWith("/"))
        {
            rootPath.append("/");
        }

        Settings.endGroup();
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in LoadSettings()");
    }
}


void FtpDataSocket::onFileConnected()
{
    qDebug() << sessionID << " onFileConnected";
}

void FtpDataSocket::onFileListening()
{
    qDebug() << sessionID << " OnFileListening";
    qDebug() << "Listening on: " + portInfo.getIPAddress() + ":" + QString::number(portInfo.getPort());

    //tell the client we are listening
    sendResponse(227,"Entering Passive Mode (" + portInfo.ToString() + ").");
}

void FtpDataSocket::onFileError(QString Message)
{
     qDebug() << sessionID << " OnFileError";

     //Close the file socket
     fileSocket->closeSocket();
     filePosition = 0;

    //Send the error message back to the client
    sendResponse(550,Message);
}

void FtpDataSocket::onFileFinished()
{
    qDebug() << sessionID << " OnFileFinished";

    //Remove the reference to the file
    filename = "";
    filePosition = 0;

    //Inform the client we are finished
    sendResponse(250);
}

void FtpDataSocket::onFileDisconnected()
{
    qDebug() << sessionID << " OnFileDisconnected";

    //Inform the client we are closing the file socket
    sendResponse(226);

    filePosition = 0;
    fileSocket = NULL;
}

void FtpDataSocket::DoUser(QString Arg)
{
    try
    {
        if(anonymous)
        {
            sendResponse(331);
            return;
        }

        //If there is no matching username tell them to enter one
        if(username == "")
        {
            sendResponse(530, "Username has not been configured.");
            return;
        }

        if(username.toUpper() != Arg.toUpper())
        {
            sendResponse(530, "Username not valid.");
            return;
        }

        username = Arg;

        //Ask for a password
        sendResponse(331);

    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoUser()");
    }
}

void FtpDataSocket::DoPass(QString Arg)
{
    try
    {
        if(anonymous)
        {
            logonUser();
            return;
        }

        AppSettings Settings;
        QString Hash = Settings.HashPassword(Arg);

        if(Hash == password && username != "")
        {
            logonUser();
        }
        else
        {
            sendResponse(530);
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoPass()");
    }
}

void FtpDataSocket::DoACCT(QString Arg)
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        account = Arg;

        //Send the OK message
        sendResponse(200);
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoACCT()");
    }
}

void FtpDataSocket::DoCWD(QString Arg)
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        QString Requested = clientPath(Arg);

        //Make sure we have a valid path
        if(Requested == "")
        {
            sendResponse(550, "Requested path not valid.");
            return;
        }

        //make sure the path exists
        QDir path(Requested);
        if(!path.exists())
        {
            sendResponse(550, "Requested path not found.");
        }
        else
        {
             currentPath = Requested;
             sendResponse(250, "CWD command successful.");
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoCWD()");
    }

}

void FtpDataSocket::DoPWD()
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        sendResponse(257, "\"" + relativePath() +"\" is the current path.");
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoPWD()");
    }

}

void FtpDataSocket::DoCDUP()
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        if(relativePath() != "/")
        {
            QDir path(currentPath);
            if(path.cdUp())
            {
                currentPath = path.absolutePath();
                sendResponse(250, "CWD command successful.");
            }
            else
            {
                //could not change to parent
                sendResponse(550, "Could not change to parent");
            }
        }
        else
        {
            //could not change to parent
            sendResponse(550, "Could not change to parent");
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoCDUP()");
    }

}

void FtpDataSocket::DoSMNT(QString Arg)
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        //Warning...this is a HUGE security issue
        if(allowSMNT)
        {
            QDir path(Arg);
            if(path.exists())
            {
                rootPath = path.absolutePath();
                sendResponse(250, "SMNT command successful.");
            }
            else
            {
                //could not change to parent
                sendResponse(550, "Could not change root directory");
            }
        }
        else
        {
            //could not change to parent
            sendResponse(550, "SMNT not allowed.");
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoSMNT()");
    }

}

void FtpDataSocket::DoREIN()
{    try
     {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        //Load the general settings
        AppSettings Settings;
        Settings.beginGroup(Settings.DefaultGroup());
        account = Settings.value("account","").toString();
        rootPath = Settings.value("rootpath","").toString();
        currentPath = rootPath;
        listRoots = Settings.value("listroots",false).toBool();
        passive = false;
        allowSMNT = false;
        filename = "";
        filePosition = 0;
        Settings.endGroup();

        sendResponse(250);
     }
     catch(QString err)
     {
         sendResponse(550,err);
     }
     catch(...)
     {
         sendResponse(550,"Unknown error in DoREIN()");
     }
}

void FtpDataSocket::DoPORT(QString Arg)
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        //Cleear the old port information
        portInfo.clear();
        portInfo.FromString(Arg);

        //There is a glaring issue with the PORT command, the client will send their interal IP
        //This is obviously of no use to us if they are behind a NAT based router
        //Do not trust their PORT info, instead use the IP address od the connected socket
        portInfo.setIPAddress(socket->peerAddress().toString());

        //Client is not in passive mode
        passive = false;

        //Create the file socket
        createFileSocket();

        //tell the client we are ready
        sendResponse(200);
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoPORT()");
    }
}


void FtpDataSocket::DoPASV(QString Arg)
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        //Clear the old port info
        portInfo.clear();

        //Client is not in passive mode
        passive = true;

        //Create the file socket and start listening
        createFileSocket();

        //Find an open port in the range and start listening
        fileSocket->listenFor(portInfo,IPAddress);
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoPASV()");
    }
}

void FtpDataSocket::DoNLST(QString Arg)
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        QString Requested;
        if(Arg == "")
        {
            //They want the current path
            Requested = currentPath;
        }
        else
        {
            Requested = clientPath(Arg);
            if(Requested == "")
            {
                sendResponse(550, "Requested path not valid.");
                return;
            }
        }

        if(connectFileSocket())
        {
            qDebug() << "** FileSocket NLST **";
            fileSocket->NLST(Requested);
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoNLST()");
    }
}

void FtpDataSocket::DoLIST(QString Arg)
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;


        QString Requested;
        if(Arg == "")
        {
            //They want the current path
            Requested = currentPath;

        }
        else
        {
            Requested = clientPath(Arg);
            if(Requested == "")
            {
                sendResponse(550, "Requested path not valid.");
                return;
            }
        }

        //Make sure we have a valid path
        if(Requested == "")
        {
            sendResponse(550, "Requested path not valid.");
            return;
        }

        if(connectFileSocket())
        {
            qDebug() << "** FileSocket LIST **";
            fileSocket->LIST(Requested);
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoLIST()");
    }
}

void FtpDataSocket::DoTYPE(QString Arg)
{
    try
    {
        //We don't do any byte transformations so just send the OK message
        sendResponse(200);
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoTYPE()");
    }
}

void FtpDataSocket::DoMKD(QString Arg)
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;


        QString Requested = clientPath(Arg);

        //Make sure we have a valid path
        if(Requested == "")
        {
            sendResponse(550, "Requested path not valid.");
            return;
        }

        QDir path(Requested);
        if(!path.mkdir(Requested))
        {
            sendResponse(530);
        }
        else
        {
            sendResponse(257);
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoMKD()");
    }
}

void FtpDataSocket::DoRNFR(QString Arg)
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        QString Requested = clientPath(Arg);
        filename = "";

        //Make sure we have a valid path
        if(Requested == "")
        {
            sendResponse(550, "Requested path not valid.");
            return;
        }

        QFileInfo path(Requested);
        if(!path.exists())
        {
            sendResponse(550, "Could not find '" + Arg + "'");
        }
        else
        {
            filename = Requested;
            sendResponse(350);
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoRNFR()");
    }
}

void FtpDataSocket::DoRNTO(QString Arg)
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        //Make sure the filename was configured
        if(filename == "")
        {
            sendResponse(503);
            return;
        }

        QString Requested = clientPath(Arg);
        QDir dir(filename);

        if(dir.rename(filename,Requested))
        {
            filename = "";
            sendResponse(250);
        }
        else
        {
            filename = "";
            sendResponse(553);
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoRNTO()");
    }
}

void FtpDataSocket::DoSTRU(QString Arg)
{
    try
    {
        //Little used command, we don't play with file names just send ok
        sendResponse(200);
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoSTRU()");
    }
}

void FtpDataSocket::DoMODE(QString Arg)
{
    try
    {
        //Little used command, we don't play with file modes just send ok
        sendResponse(200);
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoMODE()");
    }
}

void FtpDataSocket::DoRETR(QString Arg)
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        QString Requested = clientPath(Arg);

        //make sure the file exists
        QFile file(Requested);

        if(!file.exists())
        {
            sendResponse(450);
            return;
        }

        if(connectFileSocket())
        {
            qDebug() << "** FileSocket RETR **";
            fileSocket->Download(Requested, filePosition);
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoRETR()");
    }
}

void FtpDataSocket::DoSTOR(QString Arg)
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        QString Requested = clientPath(Arg);
        QFile file(Requested);

        //if the file exists, overwrite it
        if(file.exists())
        {
            if(!file.remove())
            {
                sendResponse(551, "File could not be overwritten");
                return;
            }
        }

        if(connectFileSocket())
        {
            qDebug() << "** FileSocket STOR **";
            fileSocket->upload(Requested,filePosition);
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoSTOR()");
    }
}

void FtpDataSocket::DoSize(QString Arg)
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        QString Requested = clientPath(Arg);

        //make sure the file does not exists
        QFile file(Requested);

        if(!file.exists())
        {
            sendResponse(550, "File not found");
            return;
        }

        sendResponse(213, QString::number(file.size()));
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoSize()");
    }

}

void FtpDataSocket::DoSTOU(QString Arg)
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        QString Requested = clientPath(Arg);
        QFile file(Requested);

        //if the file exists, send an error
        if(file.exists())
        {
            sendResponse(551, "File already exists!");
            return;
        }

        if(connectFileSocket())
        {
            qDebug() << "** FileSocket STOU **";
            fileSocket->upload(Requested,filePosition);
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoSTOU()");
    }
}

void FtpDataSocket::DoAPPE(QString Arg)
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        QString Requested = clientPath(Arg);

        if(connectFileSocket())
        {
            qDebug() << "** FileSocket STOR **";
            fileSocket->upload(Requested, filePosition);
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoAPPE()");
    }
}

void FtpDataSocket::DoALLO(QString Arg)
{
    try
    {
        //We dont allocate space as we can not be certain of the OS file system
        sendResponse(200);
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoALLO()");
    }
}

void FtpDataSocket::DoREST(QString Arg)
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        //make sure we have a filename
        if(filename == "")
        {
            sendResponse(550);
            return;
        }

        //Set the file position
        bool ok;
        filePosition = Arg.toInt(&ok);

        if(ok)
        {
            sendResponse(350);
        }
        else
        {
            filePosition = 0;
            sendResponse(500);
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoREST()");
    }
}

void FtpDataSocket::DoABOR(QString Arg)
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        if(fileSocket)
        {
            if(fileSocket->isConnected() || fileSocket->isListening())
            {
                fileSocket->closeSocket();
                fileSocket = NULL;

                sendResponse(226);
            }
            else
            {
                sendResponse(226);
            }
        }
        else
        {
            sendResponse(226);
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoABOR()");
    }
}

void FtpDataSocket::DoDELE(QString Arg)
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        QString Requested = clientPath(Arg);

        QFile file(Requested);
        if(file.exists())
        {

            if(file.remove())
            {
                sendResponse(250);
            }
            else
            {
                sendResponse(550,"Could not delete file!");
            }
        }
        else
        {
            sendResponse(550,"File not found!");
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoDELE()");
    }
}

void FtpDataSocket::DoRMD(QString Arg)
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        QString Requested = clientPath(Arg);

        QDir dir(Requested);
        if(dir.exists())
        {

            if(dir.rmdir(Requested))
            {
                sendResponse(250);
            }
            else
            {
                sendResponse(550,"Could not delete directory!");
            }
        }
        else
        {
            sendResponse(550,"Directory not found!");
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoRMD()");
    }
}

void FtpDataSocket::DoSITE(QString Arg)
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;


        //We don't impliment the site command
        //We could add a signal and let the connected objects deal with it
        sendResponse(202);
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoSITE()");
    }
}

void FtpDataSocket::DoSYST()
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        //This is typically not allowed due to security reasons

#ifdef Q_OS_WIN
        sendResponse(215,"Windows");
        return;
#endif

#ifdef Q_OS_MAC
        SendResponse(215,"Mac");
        return;
#endif

#ifdef Q_OS_UNIX
        SendResponse(215,"Unix");
        return;
#endif

#ifdef Q_OS_LINUX
        SendResponse(215,"Linux");
        return;
#endif

        sendResponse(215,"Unknown");
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoSYST()");
    }
}

void FtpDataSocket::DoSTAT(QString Arg)
{

    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        //This command works in three ways
        //1 - no arg and no transfers
        //2 - a arg and no transfers
        //3 - no arg and a transfer in progress

        //No File transfer and a argument
        if(Arg != "")
        {
            sendResponse("213-status of " + Arg + "\r\n");

            //send the directory listing
            QString Requested = clientPath(Arg);

            QDir dir(Requested);
            if(dir.exists())
            {
                foreach(QFileInfo item, dir.entryInfoList())
                {
                    FtpListItem ftpList;
                    sendResponse(ftpList.Convert(item));
                }
            }
            else
            {
                sendResponse("Not found!");
            }

            sendResponse(213);
            return;
        }


        //We are in a transfer
        if(fileSocket)
        {
            sendResponse(211, "Transfering data.");
            return;
        }

        //No File transfer and no argument
        if(!fileSocket && Arg == "")
        {
            sendResponse(211);
            return;
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoSTAT()");
    }
}

void FtpDataSocket::DoMLST(QString Arg)
{
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        //get the requested file
        QString Requested = clientPath(Arg);

        QFileInfo fi(Requested);
        if(fi.exists())
        {
            sendResponse("250-Listing " + Arg);
            FtpMLSItem item;
            sendResponse(item.Convert(fi));
            sendResponse(250,"End.");
        }
        else
        {
            sendResponse(550);
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoMLST()");
    }
}

void FtpDataSocket::DoMLSD(QString Arg)
{
    qDebug() << "*** DoMLSD";
    try
    {
        //Make sure they are authenticated
        if(!checkAuthenticated()) return;

        QString Requested;
        if(Arg == "")
        {
            //They want the current path
            Requested = currentPath;
        }
        else
        {
            Requested = clientPath(Arg);
            if(Requested == "")
            {
                sendResponse(550, "Requested path not valid.");
                return;
            }
        }

        qDebug() << "*** PATH = " << Requested;


        //Make sure we have a valid path
        if(Requested == "")
        {
            sendResponse(550, "Requested path not valid.");
            return;
        }


        qDebug() << "*** DoMLSD attempting";
        if(connectFileSocket())
        {
            qDebug() << "** FileSocket LIST **";
            fileSocket->MLSD(Requested, Arg);
        }
    }
    catch(QString err)
    {
        sendResponse(550,err);
    }
    catch(...)
    {
        sendResponse(550,"Unknown error in DoMLSD()");
    }
}








