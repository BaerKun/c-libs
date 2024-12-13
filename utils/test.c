#define TEST_SORT
#define SORT_ELEMENT_TYPE double
#include "sort.h"

#include "timer.h"
#include "random.h"

void test_sort() {
    const int n = 10000;
    SORT_ELEMENT_TYPE array[n];

    uint64_t seed = time(0) ^ (uint64_t)test_sort;
    pcg32_srandom(seed, seed << 1 | 1);

    for(int i = 0; i < n; i++)
        array[i] = randdouble(0, n);

    TEST_TIME(sort(array, n));

    for(int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            printf("%.2lf ", array[i * 100 + j]);
        }
        putchar('\n');
    }
}

int main(){
    test_sort();
}