#include "manager.h"
#include "keydownloader.h"
#include "keyuploader.h"
#include "imageuploader.h"
#include "imagedownloader.h"
#include "filelister.h"
#include "servermodel.h"

#include <QUrl>
#include <QFile>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QStandardPaths>

Manager::Manager(ServerModel *serverModel, QObject *parent)
    : QObject(parent)
    , m_ip("localhost")
    , m_port("8085")
    , m_address()
{
    setAddress();
    this->manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(handle(QNetworkReply*)));


    kd = new KeyDownloader(manager);
    ku = new KeyUploader(manager);
    id = new ImageDownloader(manager);
    iu = new ImageUploader(manager);
    fl = new FileLister(manager);
    sm = serverModel;

    m_downloadFolder = "file://" + QStandardPaths::locate(QStandardPaths::DownloadLocation, "",
                                                          QStandardPaths::LocateDirectory);
    m_downloadFolder[m_downloadFolder.size() - 1] = ' ';
    m_downloadFolder = m_downloadFolder.trimmed();

    m_picturesFolder = "file://" + QStandardPaths::locate(QStandardPaths::PicturesLocation, "",
                                                          QStandardPaths::LocateDirectory);
    m_picturesFolder[m_picturesFolder.size() - 1] = ' ';
    m_picturesFolder = m_picturesFolder.trimmed();
}

Manager::~Manager()
{
    delete kd;
    delete ku;
    delete id;
    delete iu;
    delete fl;
}

void Manager::handle(QNetworkReply *reply) {
    QObject* object = reply->request().originatingObject();
    if (object == ku ) {
        ku->replyFinished(reply);
    }
    else if (object == kd ) {
        kd->replyFinished(reply);
    }
    else if (object == id ) {
        id->replyFinished(reply);
    }
    else if (object == iu ) {
        iu->replyFinished(reply);
    }
    else if (object == fl ) {
        fl->replyFinished(reply);
    }
}

void Manager::setAddress()
{
    this-> m_address = "https://" + m_ip + ":" + m_port + "/api/";
}

void Manager::uploadKey() {
    qInfo() << "Uploading key...";
    ku->doUpload(m_address);
}


void Manager::uploadImage(QString fileURL) {
    QString name = QUrl(fileURL).toLocalFile();
    qInfo() << "Uploading image" << name << "...";
    iu->doUpload(m_address, name);
}

void Manager::downloadKey() {
    qInfo() << "Downloading key...";
    kd->doDownload(m_address);
}

void Manager::downloadImage(QString fileName) {
    id->doDownload(m_address);
    id->decrypt();
    qInfo() << "Downloading image" << fileName << "...";

}

void Manager::updateServerModel()
{
    qInfo() << "Updating server model...";
    fl->listFiles(m_address);

    sm->clear();
    QFile file("filelist.txt");
    if (file.open(QFile::ReadOnly)) {
        QRegularExpression re("\"([^\"]+)\"");
        QRegularExpressionMatchIterator i = re.globalMatch(file.readAll());
        if (i.hasNext()) {
            i.next();
            while (i.hasNext()) {
                QRegularExpressionMatch match = i.next();
                sm->addFile(match.captured(1));
            }
        }
        file.close();
    }
}

void Manager::setIP(QString ip) {
    qInfo() << "Setting ip to:" << ip + "...";
    this-> m_ip = ip;
    setAddress();
}

void Manager::setPort(QString port)
{
    qInfo() << "Setting port to:" << port + "...";
    this->m_port = port;
    setAddress();
}

QString Manager::downloadFolder() const
{
    return m_downloadFolder;
}

QString Manager::picturesFolder() const
{
    return m_picturesFolder;
}

QString Manager::address() const
{
    return m_address.mid(8, m_address.size() - 13);
}

//void Manager::setDownloadFolder(QString downloadFolder)
//{
//    m_downloadFolder = downloadFolder;
//}

//void Manager::setPicturesFolder(QString picturesFolder)
//{
//    m_picturesFolder = picturesFolder;
//}
