#include "httpwindow.h"
#include "manager.h"
#include <QtWidgets>

HttpWindow::HttpWindow(Manager *manager, QWidget *parent) : manager(manager), QDialog(parent)
      , downloadButton(new QPushButton(tr("Download")))
      , uploadButton(new QPushButton(tr("Upload")))
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("HttpClient"));
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    QDialogButtonBox *buttonBox = new QDialogButtonBox;
    buttonBox->addButton(downloadButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(uploadButton, QDialogButtonBox::ActionRole);

    connect(downloadButton, &QAbstractButton::clicked, this, &HttpWindow::doDownload);
    connect(uploadButton, &QAbstractButton::clicked, this, &HttpWindow::doUpload);

    mainLayout->addWidget(buttonBox);

}


void HttpWindow::doDownload(){
    manager->downloadKey();

    //file to be downloaded at server working directory
    manager->downloadImage();
}

void HttpWindow::doUpload(){

    manager->uploadKey();

    //file to be uploaded located in home directory
    QString fileNameForUpload = "lena.png";
    manager->uploadImage(fileNameForUpload);

}
