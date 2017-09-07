#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QImage>
class QString;

class ImageProvider : public QQuickImageProvider
{
public:
    ImageProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    void setImage(QImage image);
    void saveImage(QString path);

private:
    QImage image_;
};

#endif // IMAGEPROVIDER_H
