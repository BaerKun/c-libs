#include "random_primality_test.h"
#include "utils/random.h"

long primalityTestBody(long base, long exponent, long n){
    if(exponent == 0)
        return 1;

    long long x, y;

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

int primalityTest(long n, unsigned testTimes){
    if(n < 2)
        return 0;

    if(n < 4)
        return 1;

    for(int time = 0; time < testTimes; time++) {
        if (primalityTestBody(randInt(2, n - 2), n - 1, n) != 1)
            return 0;
    }

    return 1;
}
