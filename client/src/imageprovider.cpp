#include <imageprovider.h>
#include <QString>
#include <QDebug>

ImageProvider::ImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
{
}

QImage ImageProvider::requestImage(const QString&, QSize *size, const QSize &requestedSize)
{
    QImage result;
    if (size)
        *size = QSize(image_.width(), image_.height());

    if (image_.isNull()) {
        qDebug() << "ImageProvider: stored image is empty";
    } else {
        if (requestedSize.width() > 0 || requestedSize.height() > 0) {
            int w = requestedSize.width() > 0 ? requestedSize.width() : image_.width();
            int h = requestedSize.height() > 0 ? requestedSize.height() : image_.height();
            result = image_.scaled(w, h);
        } else {
            result = image_;
        }
    }
    return result;
}

void ImageProvider::setImage(QImage image)
{
    image_ = image;
}

void ImageProvider::saveImage(QString path)
{
    image_.save(path);
}
