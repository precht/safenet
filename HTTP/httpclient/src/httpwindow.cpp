#include "httpwindow.h"
#include "manager.h"
#include <QtWidgets>


static const char defaultIP[] = "localhost";

HttpWindow::HttpWindow(Manager *manager, QWidget *parent) : manager(manager), QDialog(parent)
      , downloadButton(new QPushButton(tr("Download")))
      , uploadButton(new QPushButton(tr("Upload")))
      , deleteButton(new QPushButton(tr("Delete")))
      , listButton(new QPushButton(tr("List")))
      , IPLineEdit(new QLineEdit(defaultIP))


{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("HttpClient"));

    connect(IPLineEdit, &QLineEdit::textChanged,
                this, &HttpWindow::setAddress);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("IP:"), IPLineEdit);


    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));

    QPushButton *quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);
    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);

    QDialogButtonBox *buttonBox = new QDialogButtonBox;
    buttonBox->addButton(downloadButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(uploadButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(deleteButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(listButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    connect(downloadButton, &QAbstractButton::clicked, this, &HttpWindow::doDownload);
    connect(uploadButton, &QAbstractButton::clicked, this, &HttpWindow::doUpload);
    connect(deleteButton, &QAbstractButton::clicked, this, &HttpWindow::doDelete);
    connect(listButton, &QAbstractButton::clicked, this, &HttpWindow::doList);


    mainLayout->addWidget(buttonBox);

}


void HttpWindow::doDownload(){

    manager->downloadKey();

    //file to be downloaded at server working directory
    manager->downloadImage();
}

void HttpWindow::doUpload(){

    manager->uploadKey();

    //file to be uploaded located in working directory
    QString fileNameForUpload = "lena.png";
    manager->uploadImage(fileNameForUpload);

}

void HttpWindow::doDelete(){
    manager->deleteFilesFromServer();
}

void HttpWindow::doList(){
    manager->listFiles();
}

void HttpWindow::setAddress() {
    const QString IPaddress = IPLineEdit->text().trimmed();
    manager->setAddress(IPaddress);
}
