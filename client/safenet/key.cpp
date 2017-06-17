#include <key.h>
#include <random>

Key::Key()
{
//    std::random_device rd;
//    std::minstd_rand gen( rd());
//    const double max = (quint64)(-1) >> 12;

//    x = ((((quint64)(gen() >> 12)) << 32) | gen()) / max;
//    y = ((((quint64)(gen() >> 12)) << 32) | gen()) / max;
//    a = ((((quint64)(gen() >> 12)) << 32) | gen()) / max;
//    H = ((((quint64)(gen() >> 12)) << 32) | gen()) / max;
//    G1 = gen() >> 8;
//    G2 = gen() >> 8;

    x = 0.9380;
    y = 0.7006;
    a = 0.4902;
    H = 0.1318;
    G1 = 1144200;
    G2 = 12173572;
}
