#ifndef CMTCIPHER_H
#define CMTCIPHER_H

#include <key.h>

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
    void encrypt(QImage &image);
    void decrypt(QImage &image);

private:
    // function used for sorting chaotic matrixes
    static bool cmp(const Pair &a, const Pair &b);
    // funcition to generate chaotic matrices and translation matrixes based of key and size
    void slmm2d(const QSize &size);
    // chaotic magic transform and its reverse
    void cmt(QImage &image);
    void rCmt(QImage &image);
    // pixel substitution and its reverse
    void substitution(QImage &image);
    void rSubstitution(QImage &image);

    const double PI = 3.14159265358979323846;
    const int B = 3;
    Key K;
    // two chaotic matrixes
    std::vector<double> S1;
    std::vector<double> S2;
    // two matrixes defining translation, ordered S1 and S2
    std::vector<Pair> T1;
    std::vector<Pair> T2;


};

#endif // CMTCIPHER_H
