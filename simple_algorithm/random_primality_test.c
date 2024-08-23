#include "random_primality_test.h"
#include "random.h"

unsigned long primalityTestBody(unsigned base, unsigned exponent, unsigned n){
    if(exponent == 0)
        return 1;

    unsigned long x, y;

    // 递归计算 base ^ exponent % n
    x = primalityTestBody(base, exponent / 2, n);
    if(x == 0)
        return 0;

    y = x * x % n;

    // 中间检测
    if(y == 1 && x != 1 && x != n - 1)
        return 0;

    if(exponent % 2 != 0)
        y = base * y % n;

    return y;
}

int primalityTest(unsigned n, unsigned testTimes){
    if(n < 2)
        return 0;

    if(n < 4)
        return 1;

    for(unsigned time = 0; time < testTimes; time++) {
        if (primalityTestBody(randuint(2, n - 1), n - 1, n) != 1)
            return 0;
    }

    return 1;
}
