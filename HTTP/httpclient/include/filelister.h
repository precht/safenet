#ifndef FILELISTER_H
#define FILELISTER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QEventLoop>
#include <QFile>

class FileLister : public QObject
{
    Q_OBJECT
public:
    explicit FileLister(QNetworkAccessManager *aManager = 0, QObject *parent = 0);
    void listFiles(QString address);

public slots:
    void replyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
};

#endif // FILELISTER_H
