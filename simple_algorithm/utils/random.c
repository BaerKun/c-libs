#include "random.h"
#include <stdlib.h>
#include <time.h>

long randInt(long start, long end){
    static unsigned int b = 0;
    srand(time(0) + b++);
//    return rand() % (end - start + 1) + start;

//    if in Unix, better
    return random() % (end - start + 1) + start;
}

void shuffleArray(int *array, int len){
    int i, b, c;
    for(i = 1; i < len; i++){
        b = (int)randInt(0, i);
        c = array[i];
        array[i] = array[b];
        array[b] = c;
    }
}