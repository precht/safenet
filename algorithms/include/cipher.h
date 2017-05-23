#ifndef CMTCIPHER_H
#define CMTCIPHER_H

#include <key.h>
#include <QColor>
#include <vector>

class QImage;
class QSize;

struct Pair {
    int position;
    double value;

    Pair() = default;
    Pair(int position, double value);
};

class CmtIeaCipher
{
public:
    CmtIeaCipher() = default;
    ~CmtIeaCipher();
    void encrypt(QImage &image);
    void decrypt(QImage &image);

private:
    // function used for sorting chaotic matrixes
    static bool cmp(const Pair &a, const Pair &b);

    // funcition to generate chaotic matrices and translation matrixes based of key and size
    void slmm2d(const QSize &size);
    static void threadSlmm2dSort(const int threadIndex, const int width, const int height, const int threadsNumber,
                                 Pair *T1, Pair *T2);

    // chaotic magic transform and its reverse
    void cmt(QImage &image);
    void reverseCmt(QImage &image);
    static void threadCmt(const int threadIndex, const int threadsNumber, const int width, const int height,
                          QRgb *image, Pair *T);
    static void threadRcmt(const int threadIndex, const int threadsNumber, const int width, const int height,
                           QRgb *image, Pair *T);

    // pixel substitution and its reverse
    void substitution(QImage &image);
    void reverseSubstitution(QImage &image);
    static void threadRowSubstitution(const int threadIndex, const int threadsNumber, const int width,
                                      const int height, const quint64 C, unsigned *image, double *S);
    static void threadColumnSubstitution(const int threadIndex, const int threadsNumber, const int width,
                                         const int height, const quint64 C, unsigned *image, double *S);
    static void threadReverseRowSubstitution(const int threadIndex, const int threadsNumber, const int width,
                                             const int height, const quint64 C, unsigned *image, double *S);
    static void threadReverseColumnSubstitution(const int threadIndex, const int threadsNumber, const int width,
                                                const int height, const quint64 C, unsigned *image, double *S);

    const quint64 C = 0x100000000; // substitution coeficient
    const double PI = 3.14159265358979323846;
    const int B = 3;
    Key K;
    // two chaotic matrixes
    double *S1 = nullptr;
    double *S2 = nullptr;
    // two matrixes defining translation, ordered S1 and S2
    Pair *T1 = nullptr;
    Pair *T2 = nullptr;

};

#endif // CMTCIPHER_H
