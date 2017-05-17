/*
* FTPMLSItem
* Converts a QFileInfo into a MLS item
* http://www.rhinosoft.com/newsletter/NewsL2005-07-06.asp?prod=rs
* Type=file;Size=4;Modify=20050502221000;sample.txt
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

#ifndef FTPMLSITEM_H
#define FTPMLSITEM_H

#include <QObject>
#include <QString>
#include <QFileInfo>
#include <QDateTime>

class FtpMLSItem : public QObject
{
    Q_OBJECT
public:
    explicit FtpMLSItem(QObject *parent = 0);

    /*!
    * Converts the QFileInfo into a Unix style string
    * @param FileInfo The QFileInfo object
    */
    QString Convert(QFileInfo &FileInfo);

    /*!
    * Converts the QFileInfo into a Unix style string
    * @param FileInfo The QFileInfo object
    * @param Reqeuested The requested path
    */
    QString RequestedPath(QFileInfo &FileInfo, QString Reqeuested);

signals:

public slots:

};

#endif // FTPMLSITEM_H
