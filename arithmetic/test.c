#include <integer.h>
#include  <stdio.h>
#include "arithmetic.h"
#include "arith_utils.h"
#include "timer.h"
#include "auto_diff.h"

int main() {
    int x = 3*4*7*3*17*89;
    int n, f[100];
    TEST_TIME(
    primeFactorization(x, f, &n););
    int mul = 1;
    for(int i = 0; i < n; i++) {
        mul *= f[i];
        printf("%d ", f[i]);
    }
    printf("\n%d\n", mul);
    // printf("%d\n", sqrti(121));
}
// 2721 1001