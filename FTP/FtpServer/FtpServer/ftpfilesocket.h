/*
* FTPFileSocket
* This is the data socket used by the client
* Follows rfc 959, 3659, 755
*
* http://www.w3.org/Protocols/rfc959/
* http://en.wikipedia.org/wiki/File_Transfer_Protocol
*
* Created in 2012
* Bryan Cairns - bcairns@voidrealms.com
* http://www.voidrealms.com
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#ifndef FTPFILESOCKET_H
#define FTPFILESOCKET_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include "ftpport.h"
#include "ftplistitem.h"
#include "ftpmlsitem.h"


class FtpFileSocket : public QTcpServer
{
    Q_OBJECT
public:
    explicit FtpFileSocket(QObject *parent = 0);

    bool isConnected();



signals:

    void onConnected();

    void onListening();

    void onError(QString Message);

    void onFinished();

    void onDisconnected();


public slots:

    // connect to the client on the IP and port specified
    void connectTo(FtpPort &PortInfo);


    // client port info and an IP Address to bind to
    void listenFor(FtpPort &portInfo, QString IPAddress);


    // client uploads file to server
    void upload(QString Filename, qint64 Position);


    // client downloads file from the server
    void Download(QString Filename, qint64 Position);


    // sends a list of file names with extended info
    void LIST(QString Path);


    // sends a listing of files to the client
    void NLST(QString Path);


    // sends a listing of files to the client in standard format
    void MLSD(QString Path,QString Requested);


    // gets the request from the client
    void socketReadyRead();


    // slot for the TCP socket connected() signal
    void socketConnected();


    // slot for the TCP socket disconnected() signal
    void socketDisconnected();


    // sends the response to the client socket (the QByteArray)
    void sendResponse(QByteArray data);


   // sends the response to the client socket (the QString)
    void sendResponse(QString data);


   // sends the response to the client socket
    void sendResponse(const char* data);


    void closeSocket();



protected:

    void incomingConnection(int socketDescriptor);


private:
    QTcpSocket *socket;
    int sessionID;
    bool isUploading;            // if the client is uploading
    QString filename;          // name of the file the client is working with

    // Connects the signals and slots
    void connectEvents();
};

#endif // FTPFILESOCKET_H
