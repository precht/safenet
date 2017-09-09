#include "manager.h"
#include "keydownloader.h"
#include "keyuploader.h"
#include "imageuploader.h"
#include "imagedownloader.h"
#include "filelister.h"
#include "servermodel.h"
#include "imageprovider.h"

#include <QUrl>
#include <QFile>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QStandardPaths>
#include <QGuiApplication>

Manager::Manager(ServerModel *serverModel, ImageProvider *imageProvider, QObject *parent)
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
    imp = imageProvider;
    sm = serverModel;

#ifdef Q_OS_WIN
    QString prefix = "file:///";
#else
    QString prefix = "file://";
#endif

    m_downloadFolder = QStandardPaths::locate(QStandardPaths::DownloadLocation, "", QStandardPaths::LocateDirectory);
    m_picturesFolder = prefix + QStandardPaths::locate(QStandardPaths::PicturesLocation, "",
                                                          QStandardPaths::LocateDirectory);
    m_picturesFolder[m_picturesFolder.size() - 1] = ' ';
    m_picturesFolder = m_picturesFolder.trimmed();

    m_appFolder = QGuiApplication::applicationDirPath();
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
        imp->setImage(id->replyFinished(reply));
        qDebug() << "emiting imageChanged!!!";
        emit imageChanged();
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


void Manager::uploadImage(QString fileURL)
{
    QString name = QUrl(fileURL).toLocalFile();
    iu->doUpload(m_address, name, m_downloadFolder);
    qInfo() << "Uploaded image" << name << "...";
}

void Manager::downloadKey() {
    kd->doDownload(m_address);
    qInfo() << "Downloaded key...";
}

void Manager::downloadImage(QString fileName) {
    id->doDownload(m_address, fileName, m_downloadFolder);
//    id->decrypt(); // already called by doDownload
    qInfo() << "Downloaded image" << fileName << "...";
}

void Manager::updateServerModel()
{
    // clear old content
    QFile tmp("filelist.txt");
    tmp.remove();

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
    qInfo() << "Updated server model...";
}

void Manager::setIP(QString ip) {
    this-> m_ip = ip;
    setAddress();
    qInfo() << "Set ip to:" << ip + "...";
}

void Manager::setPort(QString port)
{
    this->m_port = port;
    setAddress();
    qInfo() << "Set port to:" << port + "...";
}

void Manager::save(QString name)
{
    qInfo() << "Saving to : " << m_downloadFolder + name;
    imp->saveImage(m_downloadFolder + name);
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

QString Manager::ip() const
{
    return m_ip;
}

QString Manager::port() const
{
    return m_port;
}

ImageProvider* Manager::getImageProvider()
{
    return imp;
}

QString Manager::appFolder() const
{
    return m_appFolder;
}

//void Manager::setDownloadFolder(QString downloadFolder)
//{
//    m_downloadFolder = downloadFolder;
//}

//void Manager::setPicturesFolder(QString picturesFolder)
//{
//    m_picturesFolder = picturesFolder;
//}
