#include "cryptoimage.h"
#include <QDebug>
#include "cipher.h"

CryptoImage::CryptoImage(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
}

void CryptoImage::encrypt()
{
    if (image_.isNull())
        return;
    CmtIeaCipher cipher;
    cipher.encrypt(image_);
    update();
}

void CryptoImage::decrypt()
{
    if (image_.isNull())
        return;

    CmtIeaCipher cipher;
    cipher.decrypt(image_);
    update();
}

void CryptoImage::paint(QPainter *painter)
{
    QImage si = image_.scaled(width(), height(), Qt::KeepAspectRatio);
    painter->drawImage((width() - si.width()) / 2, (height() - si.height()) / 2, si);
}


QString CryptoImage::source() const
{
    return source_;
}

QImage CryptoImage::image() const
{
    return image_;
}

void CryptoImage::setSource(QString source)
{
    if (source_ == source)
        return;

    source_ = source;
    loadImage();
    update();
}

void CryptoImage::setImage(QImage image)
{
    image_ = image;
}

void CryptoImage::loadImage()
{
    image_.load(QUrl(source_).toLocalFile());
    if (image_.isNull()) {
        qWarning() << "WARNING: Failed to load image:" << source_;
    }
}
