#ifndef FILEDELETER_H
#define FILEDELETER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QEventLoop>

class FileDeleter : public QObject
{
    Q_OBJECT
public:
    explicit FileDeleter(QNetworkAccessManager *aManager = 0, QObject *parent = 0);
    void deleteFiles(QString address);

public slots:
    void replyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
};

#endif // FILEDELETER_H
