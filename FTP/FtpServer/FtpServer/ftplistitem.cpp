#include "ftplistitem.h"

FtpListItem::FtpListItem(QObject *parent) :
    QObject(parent)
{
}

QString FtpListItem::Convert(QFileInfo &FileInfo)
{
    QString line;

    //Get the entry type
    GetType(FileInfo, line);

    //Get the permissions
    GetPermissions(FileInfo, line);

    //Get the number of links
    GetLinks(FileInfo, line);

    //Get the owner
    GetOwner(FileInfo, line);

    //Get the group
    GetGroup(FileInfo, line);

    //Get the size
    GetSize(FileInfo, line);

    //Get the modified date
    GetDate(FileInfo, line);

    //Get the name
    GetName(FileInfo, line);

    return line;
}

void FtpListItem::GetType(QFileInfo &FileInfo, QString &Line)
{
	//"-" means a file, "d" means a directory and "l" means a link
    if(FileInfo.isSymLink())
    {
        Line.append("l");
    }
    else if (FileInfo.isDir()) {
        Line.append("d");
    }else
    {
        Line.append("-");
    }
}

void FtpListItem::GetPermissions(QFileInfo &FileInfo, QString &Line)
{
    //file permissions for owner, group, other users - r/w/x/-

    (FileInfo.permissions() && QFile::ReadOwner) ? Line.append("r") : Line.append("-");

    (FileInfo.permissions() && QFile::WriteOwner) ? Line.append("w") : Line.append("-");

    (FileInfo.permissions() && QFile::ExeOwner) ? Line.append("x") : Line.append("-");

    (FileInfo.permissions() && QFile::ReadGroup) ? Line.append("r") : Line.append("-");

    (FileInfo.permissions() && QFile::WriteGroup) ? Line.append("w") : Line.append("-");

    (FileInfo.permissions() && QFile::ExeGroup) ? Line.append("x") : Line.append("-");

    (FileInfo.permissions() && QFile::ReadOther) ? Line.append("r") : Line.append("-");

    (FileInfo.permissions() && QFile::WriteOther) ? Line.append("w") : Line.append("-");

    (FileInfo.permissions() && QFile::ExeOther) ? Line.append("x") : Line.append("-");

    Line.append(" ");
}

void FtpListItem::GetLinks(QFileInfo &FileInfo, QString &Line)
{
    //padded by 3 and right justified
    Line.append("  1 ");
}

void FtpListItem::GetOwner(QFileInfo &FileInfo, QString &Line)
{
    //padded by 10 and left justified
    QString Owner = FileInfo.owner();

    if(Owner == "")
    {
        Owner = "0";
    }

    Line.append(Owner.leftJustified(10,' ',true));
    Line.append(" ");
}

void FtpListItem::GetGroup(QFileInfo &FileInfo, QString &Line)
{
    //padded by 10 and left justified
    QString Group = FileInfo.group();

    if(Group == "")
    {
        Group = "0";
    }

    Line.append(Group.leftJustified(10,' ',true));
    Line.append(" ");
}

void FtpListItem::GetSize(QFileInfo &FileInfo, QString &Line)
{
    //padded by 10 and right justified
    QString size = QString::number(FileInfo.size());
    Line.append(size.rightJustified(10,' ',true));
    Line.append(" ");
}

void FtpListItem::GetDate(QFileInfo &FileInfo, QString &Line)
{
    //padded by 12 and left justified
    QString ModDate = FileInfo.lastModified().toString("MMM dd hh:mm");
    Line.append(ModDate.rightJustified(12,' ',true));
    Line.append(" ");
}

void FtpListItem::GetName(QFileInfo &FileInfo, QString &Line)
{
    Line.append(FileInfo.fileName());
    Line.append("\r\n");
}
