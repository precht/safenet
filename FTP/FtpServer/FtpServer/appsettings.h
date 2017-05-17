/*
* AppSettings
* Saves and loads the application settings to a common location
*
* Created in 2012
* Bryan Cairns - bcairns@voidrealms.com
* http://www.voidrealms.com
*
* All rights reserved.
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
*/

#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QSettings>
#include <QString>
#include <QDir>
#include <QByteArray>
#include <QCryptographicHash>

class AppSettings : public QSettings
{
    Q_OBJECT
public:
    explicit AppSettings(QObject *parent = 0);

    /*!
    * Gets the application path
    */
    QString ApplicationPath();

    /*!
    * Gets the application QDir
    */
    QDir ApplicationDir();

    /*!
    * Gets the group name
    */
    QString DefaultGroup();

    /*!
    * Returns a hashed password
    * @param Password The password to hash
    */
    QString HashPassword(QString Password);

signals:

public slots:

};


#endif // APPSETTINGS_H
