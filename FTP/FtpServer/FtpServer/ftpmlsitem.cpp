#include "ftpmlsitem.h"

FtpMLSItem::FtpMLSItem(QObject *parent) :
    QObject(parent)
{
}

QString FtpMLSItem::Convert(QFileInfo &FileInfo)
{
    //Type=file;Size=4;Modify=20050502221000;sample.txt
    QString ret;

    //Get the type
    ret.append("Type=");
    if(FileInfo.isDir())
    {
        ret.append("dir");
    }
    else
    {
        ret.append("file");
    }
    ret.append(";");

    if(FileInfo.isFile())
    {
        //get the size
        ret.append("Size=");
        ret.append(QString::number(FileInfo.size()));
        ret.append(";");
    }

    //get the date last modifed
    ret.append("Modify=");
    ret.append(QString::number(FileInfo.lastModified().toMSecsSinceEpoch()));
    ret.append(";");

    //get the name
    ret.append(" ");
    ret.append(FileInfo.fileName());



    ret.append("\r\n");
    return ret;
}

QString FtpMLSItem::RequestedPath(QFileInfo &FileInfo, QString Reqeuested)
{
    //Type=cdir;Size=4;Modify=20050502221000;sample.txt
    QString ret;

    //Get the type
    ret.append("Type=cdir;");

    //get the date last modifed
    ret.append("Modify=");
    ret.append(QString::number(FileInfo.lastModified().toMSecsSinceEpoch()));
    ret.append(";");

    //get the name
    ret.append(" ");
    ret.append(Reqeuested);


    ret.append("\r\n");
    return ret;
}
