#include <math.h>
#include <random.h>
#include <stdio.h>

static inline double sqaure(double x) { return x * x; }

double MonteCarloPi(uint32_t numberOfTest) {
    double x, y;
    uint32_t n = 0;
    uint32_t numInCircle = 0;

    pcg32_srandom(MonteCarloPi, 1 | (uint64_t)MonteCarloPi << 1);
    do {
        x = randdouble(0, 1);
        y = randdouble(0, 1);
        if(sqaure(x) + sqaure(y) <= 1.0)
            numInCircle++;
    }while (++n != numberOfTest);

    return 4.0 * numInCircle / n;
}

double GaussLegendrePi() {
    double a = 1.0, b = 0.7071067811865475, t = 0.25, p = 1.0;
    double tmpa;

    for(int i = 0; i < 3; ++i) {
        tmpa = a;
        a = (a + b) / 2.0;
        b = sqrt(tmpa * b);
        t -= p * sqaure(tmpa - a);
        p *= 2.0;
    }

    return sqaure(a + b) / 4.0 / t;
}

int main() {
    printf("%.20lf", GaussLegendrePi());
    return 0;
}

