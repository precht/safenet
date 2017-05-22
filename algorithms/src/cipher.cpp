#include <cipher.h>

#include <QImage>
#include <QSize>
#include <QDebug>

#include <cmath>

void CmtIeaCipher::encrypt(QImage &image)
{
    slmm2d(image.size());
    cmt(image);
    substitution(image);
}

void CmtIeaCipher::decrypt(QImage &image)
{
    slmm2d(image.size());
    rSubstitution(image);
    rCmt(image);
}

void CmtIeaCipher::slmm2d(const QSize &size)
{
    const int height = size.height();
    const int width = size.width();
    const int total = size.height() * size.width();

    S1.resize(total);
    S2.resize(total);
    T1.resize(total);
    T2.resize(total);
    S1.clear();
    S2.clear();
    T1.clear();
    T2.clear();

    // generate chaotic sequences
    double xPrev = std::fmod(K.x + K.G1 * K.H, 1.0);
    double yPrev = std::fmod(K.y + K.G1 * K.H, 1.0);
    double A = 0.9 + (std::fmod(K.a + K.G1 * K.H, 0.1));
    double xNext;
    double yNext;
    int counter = 0;
    for (int i = 0; i < total; ++i) {
        xNext = A * (std::sin(PI * yPrev) + B) * xPrev * (1 - xPrev);
        yNext = A * (std::sin(PI * xNext) + B) * yPrev * (1 - yPrev);
        S1.push_back(xNext + yNext);
        T1.push_back(Pair(counter++, xNext + yNext));
        xPrev = xNext;
        yPrev = yNext;
        counter %= height;
    }

    xPrev = std::fmod(K.x + K.G2 * K.H, 1.0);
    yPrev = std::fmod(K.y + K.G2 * K.H, 1.0);
    A = 0.9 + (std::fmod(K.a + K.G2 * K.H, 0.1));
    counter = 0;
    for (int i = 0; i < total; ++i) {
        xNext = A * (std::sin(PI * yPrev) + B) * xPrev * (1 - xPrev);
        yNext = A * (std::sin(PI * xNext) + B) * yPrev * (1 - yPrev);
        S2.push_back(xNext + yNext);
        T2.push_back(Pair(counter++, xNext + yNext));
        xPrev = xNext;
        yPrev = yNext;
        counter %= height;
    }

    // sort by columns
    auto it1 = T1.begin();
    auto it2 = T2.begin();
    for (int i = 0; i < width; ++i, it1 += height, it2 += height) {
        std::sort(it1, it1 + height, cmp);
        std::sort(it2, it2 + height, cmp);
    }
}

void CmtIeaCipher::cmt(QImage &image)
{
    const int height = image.height();
    const int width = image.width();

    std::vector<QRgb> cache;
    cache.resize(width);

    // transform using matrix T1
    for (int i = 0; i < height; ++i) {
        cache.clear();
        // cache values
        {
            int j = 0;
            int x = (i + 1) % width;
            for (; j < width; ++j, x = (x + 1) % width) {
                const int y = T1[height * x + i].n;
                cache.push_back(image.pixel(x, y));
            }
        }
        // put values in new positions
        for (int x = 0; x < width; ++x) {
            const int y = T1[height * x + i].n;
            image.setPixel(x, y, cache[x]);
        }
    }

    // transform using matrix T2
    for (int i = 0; i < height; ++i) {
        cache.clear();
        // cache values
        {
            int j = 0;
            int x = (i + 1) % width;
            for (; j < width; ++j, x = (x + 1) % width) {
                const int y = T2[height * x + i].n;
                cache.push_back(image.pixel(x, y));
            }
        }
        // put values in new positions
        for (int x = 0; x < width; ++x) {
            const int y = T2[height * x + i].n;
            image.setPixel(x, y, cache[x]);
        }
    }
}

void CmtIeaCipher::rCmt(QImage &image)
{
    const int height = image.height();
    const int width = image.width();

    std::vector<QRgb> cache;
    cache.resize(width);

    // transform using matrix T2
    for (int i = 0; i < height; ++i) {
        cache.clear();
        // cache values
        for (int x = 0; x < width; ++x) {
            const int y = T2[height * x + i].n;
            cache.push_back(image.pixel(x, y));
        }
        // put values in new positions
        {
            int j = 0;
            int x = (i + 1) % width;
            for (; j < width; ++j, x = (x + 1) % width) {
                const int y = T2[height * x + i].n;
                image.setPixel(x, y, cache[j]);
            }
        }
    }

    // transform using matrix T1
    for (int i = 0; i < height; ++i) {
        cache.clear();
        // cache values
        for (int x = 0; x < width; ++x) {
            const int y = T1[height * x + i].n;
            cache.push_back(image.pixel(x, y));
        }
        // put values in new positions
        {
            int j = 0;
            int x = (i + 1) % width;
            for (; j < width; ++j, x = (x + 1) % width) {
                const int y = T1[height * x + i].n;
                image.setPixel(x, y, cache[j]);
            }
        }
    }
}

void CmtIeaCipher::substitution(QImage &image)
{
    const int width = image.width();
    const int height = image.height();
    const quint64 coeficient = 0x100000000;

    // 1-st row substitution
    for (int y = 0; y < height; ++y) {
        // the first pixel in row
        {
            const QRgb shift = image.pixel(width - 1, y) + (QRgb)std::floor(S1[y] * coeficient);
            const QRgb value = image.pixel(0, y) + shift;
            image.setPixel(0, y, value);
        }
        // rest pixels in row
        for (int x = 1; x < width; ++x) {
            const QRgb shift = image.pixel(x - 1, y) + (QRgb)std::floor(S1[height * x + y] * coeficient);
            const QRgb value = image.pixel(x, y) + shift;
            image.setPixel(x, y, value);
        }
    }

    // 1-st column substitution
    for (int x = 0; x < width; ++x) {
        // the first pixel in column
        {
            const QRgb shift = image.pixel(x, height - 1) + (QRgb)std::floor(S1[height * x] * coeficient);
            const QRgb value = image.pixel(x, 0) + shift;
            image.setPixel(x, 0, value);
        }
        // rest pixels in column
        for (int y = 1; y < height; ++y) {
            const QRgb shift = image.pixel(x, y - 1) + (QRgb)std::floor(S1[height * x + y] * coeficient);
            const QRgb value = image.pixel(x, y) + shift;
            image.setPixel(x, y, value);
        }
    }

    // 2-nd row substitution
    for (int y = 0; y < height; ++y) {
        // the first pixel in row
        {
            const QRgb shift = image.pixel(width - 1, y) + (QRgb)std::floor(S2[y] * coeficient);
            const QRgb value = image.pixel(0, y) + shift;
            image.setPixel(0, y, value);
        }
        // rest pixels in row
        for (int x = 1; x < width; ++x) {
            const QRgb shift = image.pixel(x - 1, y) + (QRgb)std::floor(S2[height * x + y] * coeficient);
            const QRgb value = image.pixel(x, y) + shift;
            image.setPixel(x, y, value);
        }
    }

    // 2-nd column substitution
    for (int x = 0; x < width; ++x) {
        // the first pixel in column
        {
            const QRgb shift = image.pixel(x, height - 1) + (QRgb)std::floor(S2[height * x] * coeficient);
            const QRgb value = image.pixel(x, 0) + shift;
            image.setPixel(x, 0, value);
        }
        // rest pixels in column
        for (int y = 1; y < height; ++y) {
            const QRgb shift = image.pixel(x, y - 1) + (QRgb)std::floor(S2[height * x + y] * coeficient);
            const QRgb value = image.pixel(x, y) + shift;
            image.setPixel(x, y, value);
        }
    }

}

void CmtIeaCipher::rSubstitution(QImage &image)
{
    const int width = image.width();
    const int height = image.height();
    const quint64 coeficient = 0x100000000;

    // 2-nd column substitution
    for (int x = 0; x < width; ++x) {
        // all pixels in row except the first one
        for (int y = height - 1; y > 0; --y) {
            const QRgb shift = image.pixel(x, y - 1) + (QRgb)std::floor(S2[height * x + y] * coeficient);
            const QRgb value = image.pixel(x, y) - shift;
            image.setPixel(x, y, value);
        }
        // the first pixel in row
        {
            const QRgb shift = image.pixel(x, height - 1) + (QRgb)std::floor(S2[height * x] * coeficient);
            const QRgb value = image.pixel(x, 0) - shift;
            image.setPixel(x, 0, value);
        }
    }

    // 2-nd row substitution
    for (int y = 0; y < height; ++y) {
        // all pixels in row except the first one
        for (int x = width - 1; x > 0; --x) {
            const QRgb shift = image.pixel(x - 1, y) + (QRgb)std::floor(S2[height * x + y] * coeficient);
            const QRgb value = image.pixel(x, y) - shift;
            image.setPixel(x, y, value);
        }
        // the first pixel in row
        {
            const QRgb shift = image.pixel(width - 1, y) + (QRgb)std::floor(S2[y] * coeficient);
            const QRgb value = image.pixel(0, y) - shift;
            image.setPixel(0, y, value);
        }
    }

    // 1-st column substitution
    for (int x = 0; x < width; ++x) {
        // all pixels in row except the first one
        for (int y = height - 1; y > 0; --y) {
            const QRgb shift = image.pixel(x, y - 1) + (QRgb)std::floor(S1[height * x + y] * coeficient);
            const QRgb value = image.pixel(x, y) - shift;
            image.setPixel(x, y, value);
        }
        // the first pixel in row
        {
            const QRgb shift = image.pixel(x, height - 1) + (QRgb)std::floor(S1[height * x] * coeficient);
            const QRgb value = image.pixel(x, 0) - shift;
            image.setPixel(x, 0, value);
        }
    }

    // 1-st row substitution
    for (int y = 0; y < height; ++y) {
        // all pixels in row except the first one
        for (int x = width - 1; x > 0; --x) {
            const QRgb shift = image.pixel(x - 1, y) + (QRgb)std::floor(S1[height * x + y] * coeficient);
            const QRgb value = image.pixel(x, y) - shift;
            image.setPixel(x, y, value);
        }
        // the first pixel in row
        {
            const QRgb shift = image.pixel(width - 1, y) + (QRgb)std::floor(S1[y] * coeficient);
            const QRgb value = image.pixel(0, y) - shift;
            image.setPixel(0, y, value);
        }
    }
}

bool CmtIeaCipher::cmp(const Pair &a, const Pair &b)
{
    if (a.v == b.v) {
        return a.n < b.n;
    }
    return a.v < b.v;
}

Pair::Pair(int n, double v)
    : n(n)
    , v(v)
{
}
