#include <math.h>
#include <random.h>
#include <stdio.h>

static inline double sqaure(const double x) { return x * x; }

double MonteCarloPi(const unsigned numberOfTest) {
    unsigned n = 0;
    unsigned numPointInCircle = 0;

    pcg32_srandom(MonteCarloPi, 1 | (uint64_t)MonteCarloPi << 1);
    do {
        const double x = randdouble(0, 1);
        const double y = randdouble(0, 1);
        if(sqaure(x) + sqaure(y) <= 1.0)
            numPointInCircle++;
    }while (++n != numberOfTest);

    return 4.0 * numPointInCircle / n;
}

double GaussLegendrePi() {
    double a = 1.0, b = 0.7071067811865475, t = 0.25, p = 1.0;

    for(int i = 0; i < 3; ++i) {
        const double tmpa = a;
        a = (a + b) / 2.0;
        b = sqrt(tmpa * b);
        t -= p * sqaure(tmpa - a);
        p *= 2.0;
    }

    return sqaure(a + b) / 4.0 / t;
}
