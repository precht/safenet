/*
* FTPServer
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

#ifndef FTPSERVER_H
#define FTPSERVER_H


#include <QObject>
#include <QTcpServer>
#include "ftpdatasocket.h"

class FtpServer : public QTcpServer
{
    Q_OBJECT
public:

    explicit FtpServer(QObject *parent = 0);

    //Starts the TCP Server - port to start the server on
    void startServer(qint16 port);

    void stopServer();

signals:
    /*!
    * Signal indicates the TCP Server has stopped
    */
    void OnStarted();

    /*!
    * Signal indicates the TCP Server has stopped
    */
    void OnStopped();

public slots:


protected:

    /*!
    * Processes incomming TCP connections
    * @param socketDescriptor The descriptor of the conection
    */
    virtual void incomingConnection(qintptr handle);


private:


};

#endif // FTPSERVER_H
