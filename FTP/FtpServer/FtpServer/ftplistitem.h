/*
* FTPListItem
* Converts a QFileInfo into a Unix style LIST item
* http://publib.boulder.ibm.com/infocenter/iseries/v5r4/index.jsp?topic=%2Frzaiq%2Frzaiqrzaiqfdrtu.htm
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

#ifndef FTPLISTITEM_H
#define FTPLISTITEM_H

#include <QObject>
#include <QString>
#include <QFileInfo>
#include <QDateTime>

class FtpListItem : public QObject
{
    Q_OBJECT
public:
    explicit FtpListItem(QObject *parent = 0);


    /*
    * Converts the QFileInfo into a Unix style string
    */
    QString Convert(QFileInfo &FileInfo);

private:


    void GetType(QFileInfo &FileInfo, QString &Line);

    void GetPermissions(QFileInfo &FileInfo, QString &Line);

    void GetLinks(QFileInfo &FileInfo, QString &Line);

    void GetOwner(QFileInfo &FileInfo, QString &Line);

    void GetGroup(QFileInfo &FileInfo, QString &Line);

    void GetSize(QFileInfo &FileInfo, QString &Line);

    void GetDate(QFileInfo &FileInfo, QString &Line);

    void GetName(QFileInfo &FileInfo, QString &Line);
};

#endif // FTPLISTITEM_H
