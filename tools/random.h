#ifndef RANDOM_H
#define RANDOM_H

#include <stdlib.h>
#include <time.h>

static int randInt(int start, int end){
    srand(time(0));
    return rand() % (end - start + 1) + start;
}

static void shuffleArray(int *array, int len){
    int i, b, c;
    for(i = 1; i < len; i++){
        b = randInt(0, i);
        c = array[i];
        array[i] = array[b];
        array[b] = c;
    }
}

#endif //RANDOM_H
