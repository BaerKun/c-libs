#define TEST_SORT
#include "sort.h"

#include "timer.h"
#include "random.h"

int main(){
    int n = 999999;
    int array[n];

    uint64_t seed = time(0) ^ (uint64_t)main;
    pcg32_srandom(seed, seed << 1 | 1);

    for(int i = 0; i < n; i++)
        array[i] = i;

    shuffleArray(array, n);

    TEST_TIME(sort(array, n));

    for(int i = 0; i < n; i++)
        if(array[i] != i)
            printf("%d ", array[i]);

    return 0;
}