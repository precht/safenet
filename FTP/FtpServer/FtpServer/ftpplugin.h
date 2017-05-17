/*
* FTPPlugin
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

#ifndef FTPPLUGIN_H
#define FTPPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include <QPointer>
#include <QSettings>
#include <QByteArray>
#include <QCryptographicHash>
#include "PluginInterface.h"
#include "ftpserver.h"
#include "appsettings.h"

class FtpPlugin : public QObject, PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
    Q_PLUGIN_METADATA(IID "Safenet")

public:
    explicit FtpPlugin(QObject *parent = 0);

    /*!
    * Returns the name of the plugin
    */
    QString Name() const;

    /*!
    * Returns the description of the plugin
    */
    QString Description() const;

    /*!
    * Returns the status of the plugin
    */
    QString Status() const;

    /*!
    * Tells the plugin that it is being installed
    * The plugin should perform any installtion code
    */
    QString Install() const;

    /*!
    * Tells the plugin that it is being uninstalled
    * The plugin should perform any uninstalltion code
    */
    QString Uninstall() const;

    /*!
    * Called when the plugin is started
    */
    QString Start() const;

    /*!
    * Called when the plugin is stopped
    */
    QString Stop() const;

    /*!
    * Returns a QStringList of available commands
    */
    QStringList Commands() const;

    /*!
    * Askes the plugin to execute a command and returns the results
    * @param command The command to execute
    * @param args A list of arguments for the command
    */
    QString Exec(QString command, QStringList args) const;

private:
    FtpServer *Server;
public slots:

};

#endif // FTPPLUGIN_H
