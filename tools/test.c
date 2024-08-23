#include "random.h"
#include <stdio.h>


int main(){
    unsigned i;
    for(i = 0; i < 100; ++i){
        printf("%lf ", randfloat(-100, 100));
    }

    return 0;
}