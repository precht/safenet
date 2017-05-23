#include <cipher.h>

#include <QImage>
#include <QSize>
#include <QDebug>

#include <cmath>
#include <iostream>
#include <thread>

CmtIeaCipher::~CmtIeaCipher()
{
    delete[] S1;
    delete[] S2;
    delete[] T1;
    delete[] T2;
}

void CmtIeaCipher::encrypt(QImage &image)
{
    slmm2d(image.size());
    cmt(image);
    substitution(image);
}

void CmtIeaCipher::decrypt(QImage &image)
{
    slmm2d(image.size());
    reverseSubstitution(image);
    reverseCmt(image);
}

void CmtIeaCipher::slmm2d(const QSize &size)
{
    const int width = size.width();
    const int height = size.height();
    const int total = width * height;

    delete[] S1;
    delete[] S2;
    delete[] T1;
    delete[] T2;
    S1 = new double[total];
    S2 = new double[total];
    T1 = new Pair[total];
    T2 = new Pair[total];

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
        S1[i] = xNext + yNext;
        T1[i] = Pair(counter++, xNext + yNext);
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
        S2[i] = xNext + yNext;
        T2[i] = Pair(counter++, xNext + yNext);
        xPrev = xNext;
        yPrev = yNext;
        counter %= height;
    }

    int threadsNumber = std::thread::hardware_concurrency();
    if (threadsNumber <= 0 ) threadsNumber = 1;
    std::vector<std::thread> threadsPool;

    // create many threads and sort columns on them
    for (int i = 1; i < threadsNumber; ++i) {
        threadsPool.push_back(std::move(std::thread(threadSlmm2dSort, i, threadsNumber, width, height, T1, T2)));
    }
    threadSlmm2dSort(0, threadsNumber, width, height, T1, T2);

    // wait for all threads to end
    for (auto it = threadsPool.begin(); it != threadsPool.end(); ++it) {
        it->join();
    }
}

void CmtIeaCipher::threadSlmm2dSort(const int threadIndex, const int threadsNumber, const int width, const int height,
                                    Pair *T1, Pair *T2)
{
    for (int i = threadIndex; i < width; i += threadsNumber) {
        const int p = i * height;
        std::sort(T1 + p, T1 + p + height, cmp);
        std::sort(T2 + p, T2 + p + height, cmp);
    }
}

void CmtIeaCipher::cmt(QImage &image)
{
    const int width = image.width();
    const int height = image.height();

    int threadsNumber = std::thread::hardware_concurrency();
    if (threadsNumber <= 0 ) threadsNumber = 1;
    std::vector<std::thread> threadsPool;

    // create many threads and transform using matrix T1
    for (int i = 1; i < threadsNumber; ++i) {
        threadsPool.push_back(std::move(std::thread(threadCmt, i, threadsNumber,
                                                    width, height, (unsigned *)image.bits(), T1)));
    }
    threadCmt(0, threadsNumber, width, height, (unsigned *)image.bits(), T1);

    // wait for all threads to end
    for (auto it = threadsPool.begin(); it != threadsPool.end(); ++it) {
        it->join();
    }
    threadsPool.clear();

    //do the same for matrix T2
    for (int i = 1; i < threadsNumber; ++i) {
        threadsPool.push_back(std::move(std::thread(threadCmt, i, threadsNumber,
                                                    width, height, (unsigned *)image.bits(), T2)));
    }
    threadCmt(0, threadsNumber, width, height, (unsigned *)image.bits(), T2);

    // wait for all threads to end
    for (auto it = threadsPool.begin(); it != threadsPool.end(); ++it) {
        it->join();
    }
}

void CmtIeaCipher::threadCmt(const int threadIndex, const int threadsNumber, const int width, const int height,
                               QRgb *image, Pair *T)
{
    std::vector<QRgb> cache;
    cache.resize(width);

    for (int i = threadIndex; i < height; i += threadsNumber) {
        cache.clear();
        // cache values
        {
            int j = 0;
            int x = (i + 1) % width;
            for (; j < width; ++j, x = (x + 1) % width) {
                const int y = T[height * x + i].position;
                cache.push_back(image[y * width + x]);
            }
        }
        // put values in new positions
        for (int x = 0; x < width; ++x) {
            const int y = T[height * x + i].position;
            image[y * width + x] =  cache[x];
        }
    }
}

void CmtIeaCipher::reverseCmt(QImage &image)
{
    const int width = image.width();
    const int height = image.height();

    int threadsNumber = std::thread::hardware_concurrency();
    if (threadsNumber <= 0 ) threadsNumber = 1;
    std::vector<std::thread> threadsPool;

    // create many threads and reverse transform using matrix T2
    for (int i = 1; i < threadsNumber; ++i) {
        threadsPool.push_back(std::move(std::thread(threadRcmt, i, threadsNumber,
                                                    width, height, (unsigned *)image.bits(), T2)));
    }
    threadRcmt(0, threadsNumber, width, height, (unsigned *)image.bits(), T2);

    // wait for all threads to end
    for (auto it = threadsPool.begin(); it != threadsPool.end(); ++it) {
        it->join();
    }
    threadsPool.clear();

    //do the same for matrix T1
    for (int i = 1; i < threadsNumber; ++i) {
        threadsPool.push_back(std::move(std::thread(threadRcmt, i, threadsNumber,
                                                    width, height, (unsigned *)image.bits(), T1)));
    }
    threadRcmt(0, threadsNumber, width, height, (unsigned *)image.bits(), T1);

    // wait for all threads to end
    for (auto it = threadsPool.begin(); it != threadsPool.end(); ++it) {
        it->join();
    }
}

void CmtIeaCipher::threadRcmt(const int threadIndex, const int threadsNumber, const int width, const int height,
                              QRgb *image, Pair *T)
{
    std::vector<QRgb> cache;
    cache.resize(width);

    for (int i = threadIndex; i < height; i += threadsNumber) {
        cache.clear();
        // cache values
        for (int x = 0; x < width; ++x) {
            const int y = T[height * x + i].position;
                cache.push_back(image[y * width + x]);
        }
        // put values in new positions
        {
            int j = 0;
            int x = (i + 1) % width;
            for (; j < width; ++j, x = (x + 1) % width) {
                const int y = T[height * x + i].position;
                image[y * width + x] = cache[j];
            }
        }
    }
}

void CmtIeaCipher::substitution(QImage &image)
{
    const int width = image.width();
    const int height = image.height();

    int threadsNumber = std::thread::hardware_concurrency();
    if (threadsNumber <= 0 ) threadsNumber = 1;
    std::vector<std::thread> threadsPool;

    // create many threads and substitute pixels using S1 matrix

    // substitute rows
    for (int i = 1; i < threadsNumber; ++i) {
        threadsPool.push_back(std::move(std::thread(threadRowSubstitution, i, threadsNumber,
                                                    width, height, C, (QRgb *)image.bits(), S1)));
    }
    threadRowSubstitution(0, threadsNumber, width, height, C, (QRgb *)image.bits(), S1);
    for (auto it = threadsPool.begin(); it != threadsPool.end(); ++it) {
        it->join();
    }
    threadsPool.clear();

    //substitute columns
    for (int i = 1; i < threadsNumber; ++i) {
        threadsPool.push_back(std::move(std::thread(threadColumnSubstitution, i, threadsNumber,
                                                    width, height, C, (QRgb *)image.bits(), S1)));
    }
    threadColumnSubstitution(0, threadsNumber, width, height, C, (QRgb *)image.bits(), S1);
    for (auto it = threadsPool.begin(); it != threadsPool.end(); ++it) {
        it->join();
    }
    threadsPool.clear();

    // create many threads and substitute pixels using S2 matrix

    // substitute rows
    for (int i = 1; i < threadsNumber; ++i) {
        threadsPool.push_back(std::move(std::thread(threadRowSubstitution, i, threadsNumber,
                                                    width, height, C, (QRgb *)image.bits(), S2)));
    }
    threadRowSubstitution(0, threadsNumber, width, height, C, (QRgb *)image.bits(), S2);
    for (auto it = threadsPool.begin(); it != threadsPool.end(); ++it) {
        it->join();
    }
    threadsPool.clear();

    //substitute columns
    for (int i = 1; i < threadsNumber; ++i) {
        threadsPool.push_back(std::move(std::thread(threadColumnSubstitution, i, threadsNumber,
                                                    width, height, C, (QRgb *)image.bits(), S2)));
    }
    threadColumnSubstitution(0, threadsNumber, width, height, C, (QRgb *)image.bits(), S2);
    for (auto it = threadsPool.begin(); it != threadsPool.end(); ++it) {
        it->join();
    }
}

void CmtIeaCipher::threadRowSubstitution(const int threadIndex, const int threadsNumber, const int width,
                                         const int height, const quint64 C, unsigned *image, double *S)
{
    for (int y = threadIndex; y < height; y += threadsNumber) {
        // the first pixel in row
        image[y * width] = image[y * width] + image[(y + 1) * width - 1] + (QRgb)std::floor(S[y] * C);
        // rest pixels in row
        for (int x = 1; x < width; ++x) {
            image[y * width + x] = image[y * width + x] + image[y * width + x - 1]
                    + (QRgb)std::floor(S[height * x + y] * C);
        }
    }
}

void CmtIeaCipher::threadColumnSubstitution(const int threadIndex, const int threadsNumber, const int width,
                                            const int height, const quint64 C, unsigned *image, double *S)
{
    for (int x = threadIndex; x < width; x += threadsNumber) {
        // the first pixel in column
        image[x] = image[x] + image[(height - 1) * width + x] + (QRgb)std::floor(S[height * x] * C);
        // rest pixels in column
        for (int y = 1; y < height; ++y) {
            image[y * width + x] = image[y * width + x] + image[(y - 1) * width + x]
                    + (QRgb)std::floor(S[height * x + y] * C);
        }
    }
}

void CmtIeaCipher::reverseSubstitution(QImage &image)
{
    const int width = image.width();
    const int height = image.height();

    int threadsNumber = std::thread::hardware_concurrency();
    if (threadsNumber <= 0 ) threadsNumber = 1;
    std::vector<std::thread> threadsPool;

    // create many threads and reverse substitute pixels using S1 matrix

    // substitute columns
    for (int i = 1; i < threadsNumber; ++i) {
        threadsPool.push_back(std::move(std::thread(threadReverseColumnSubstitution, i, threadsNumber,
                                                    width, height, C, (QRgb *)image.bits(), S2)));
    }
    threadReverseColumnSubstitution(0, threadsNumber, width, height, C, (QRgb *)image.bits(), S2);
    for (auto it = threadsPool.begin(); it != threadsPool.end(); ++it) {
        it->join();
    }
    threadsPool.clear();

    //substitute rows
    for (int i = 1; i < threadsNumber; ++i) {
        threadsPool.push_back(std::move(std::thread(threadReverseRowSubstitution, i, threadsNumber,
                                                    width, height, C, (QRgb *)image.bits(), S2)));
    }
    threadReverseRowSubstitution(0, threadsNumber, width, height, C, (QRgb *)image.bits(), S2);
    for (auto it = threadsPool.begin(); it != threadsPool.end(); ++it) {
        it->join();
    }
    threadsPool.clear();

    // create many threads and reverse substitute pixels using S1 matrix

    // substitute columns
    for (int i = 1; i < threadsNumber; ++i) {
        threadsPool.push_back(std::move(std::thread(threadReverseColumnSubstitution, i, threadsNumber,
                                                    width, height, C, (QRgb *)image.bits(), S1)));
    }
    threadReverseColumnSubstitution(0, threadsNumber, width, height, C, (QRgb *)image.bits(), S1);
    for (auto it = threadsPool.begin(); it != threadsPool.end(); ++it) {
        it->join();
    }
    threadsPool.clear();

    //substitute rows
    for (int i = 1; i < threadsNumber; ++i) {
        threadsPool.push_back(std::move(std::thread(threadReverseRowSubstitution, i, threadsNumber,
                                                    width, height, C, (QRgb *)image.bits(), S1)));
    }
    threadReverseRowSubstitution(0, threadsNumber, width, height, C, (QRgb *)image.bits(), S1);
    for (auto it = threadsPool.begin(); it != threadsPool.end(); ++it) {
        it->join();
    }
}

void CmtIeaCipher::threadReverseRowSubstitution(const int threadIndex, const int threadsNumber, const int width,
                                                const int height, const quint64 C, unsigned *image, double *S)
{
    for (int y = threadIndex; y < height; y += threadsNumber) {
        // all pixels in row except the first one
        for (int x = width - 1; x > 0; --x) {
            image[y * width + x] = image[y * width + x] - image[y * width + x - 1]
                    - (QRgb)std::floor(S[height * x + y] * C);
        }
        // the first pixel in row
        image[y * width] = image[y * width] - image[(y + 1) * width - 1] - (QRgb)std::floor(S[y] * C);
    }
}

void CmtIeaCipher::threadReverseColumnSubstitution(const int threadIndex, const int threadsNumber, const int width,
                                                   const int height, const quint64 C, unsigned *image, double *S)
{
    for (int x = threadIndex; x < width; x += threadsNumber) {
        // all pixels in column except the first one
        for (int y = height - 1; y > 0; --y) {
            image[y * width + x] = image[y * width + x] - image[(y - 1) * width + x]
                    - (QRgb)std::floor(S[height * x + y] * C);
        }
        // the first pixel in column
        image[x] = image[x] - image[(height - 1) * width + x] - (QRgb)std::floor(S[height * x] * C);
    }
}

bool CmtIeaCipher::cmp(const Pair &a, const Pair &b)
{
    if (a.value == b.value) {
        return a.position < b.position;
    }
    return a.value < b.value;
}

Pair::Pair(int n, double v)
    : position(n)
    , value(v)
{
}
