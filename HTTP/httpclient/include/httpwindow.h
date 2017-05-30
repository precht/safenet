#ifndef HTTPWINDOW_H
#define HTTPWINDOW_H

#include <QDialog>
#include "manager.h"

class HttpWindow : public QDialog
{
    Q_OBJECT
public:
    explicit HttpWindow(Manager *manager, QWidget *parent = 0);

signals:

public slots:

private slots:
    void doDownload();
    void doUpload();

private:
    QPushButton *downloadButton;
    QPushButton *uploadButton;
    Manager *manager;
};

#endif // HTTPWINDOW_H
