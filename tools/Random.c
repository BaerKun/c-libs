#include "Random.h"
#include <stdlib.h>
#include <time.h>

int RandInt(int start, int end){
    srand(time(0));
    return rand() % (end - start + 1) + start;
//    if in Unix, better
//    return random() % (end - start + 1) + start;
}
void ShuffleArray(int *array, int len){
    int i, b, c;
    for(i = 1; i < len; i++){
        b = RandInt(0, i);
        c = array[i];
        array[i] = array[b];
        array[b] = c;
    }
}