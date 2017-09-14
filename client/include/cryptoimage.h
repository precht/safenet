#ifndef CRYPTOIMAGE_H
#define CRYPTOIMAGE_H

#include <QQuickPaintedItem>
#include <QPainter>


class CryptoImage : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource)
    Q_PROPERTY(QImage image READ image WRITE setImage)

public:
    CryptoImage(QQuickItem *parent = Q_NULLPTR);
    Q_INVOKABLE void encrypt();
    Q_INVOKABLE void decrypt();
    QString source() const;
    QImage image() const;

public slots:
    void setSource(QString source);
    void setImage(QImage image);

protected:
    virtual void paint(QPainter *painter) Q_DECL_OVERRIDE;

private:
    void loadImage();

    QString source_;
    QImage image_;
};

#endif // CRYPTOIMAGE_H
