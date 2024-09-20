#include "timer.h"
#include "random.h"
#include "sort.h"

int main(){
    int a[10000];
    for(int i = 0; i < 10000; i++){
        a[i] = randint(0, 10000);
    }

    TEST_TIME(sort(a, 10000));

    for(int i = 0; i < 10000; i++){
        printf("%d ", a[i]);
    }

    return 0;
}