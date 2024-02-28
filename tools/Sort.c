#include <stdlib.h>

static void Swap(int *x, int *y){
    int z = *x;
    *x = *y;
    *y = z;
}
void InsertionSort(int *array, int len){
    int i, j, tmp;
    for(i = 1; i < len; i++){
        tmp = array[i];
        for(j = i; j && array[j - 1] > tmp; j--)
            array[j] = array[j - 1];
        array[j] = tmp;
    }
}
void ShellSort(int *array, int len){
    int i, j, delta, tmp;
    for(delta = 1; delta < len; delta = delta << 1 | 1);
    for(delta >>= 1; delta; delta >>= 1){
        for(i = delta; i < len; i++){
            tmp = array[i];
            for(j = i; j >= delta && array[j - delta] > tmp; j -= delta)
                array[j] = array[j - delta];
            array[j] = tmp;
        }
    }
}
static void PercDown(int *array, int father, int len){
    int child, theTop;
    theTop = array[father];
    for(; (child = father << 1) < len; father = child) {
        if(child + 1 != len && array[child + 1] > array[child])
            child++;
        if (theTop < array[child])
            array[father] = array[child];
        else
            break;
    }
    array[father] = theTop;
}
void HeapSort(int *array, int len){
    int i;
    array--;
    len++;
    for(i = len >> 1; i; i--)
        PercDown(array, i, len);
    for(i = len - 1; i; i--){
        Swap(array + i, array + 1);
        PercDown(array, 1, i);
    }
}
static void Merge(int *array, int *tmpArray, int Lpos, int Rpos, int Rend){
    int Lend, tmpPos, Lstart;
    Lend = Rpos - 1;
    tmpPos = Lstart = Lpos;
    while(1){
        if(array[Lpos] < array[Rpos]){
            tmpArray[tmpPos++] = array[Lpos++];
            if(Lpos > Lend)
                break;
        }else{
            tmpArray[tmpPos++] = array[Rpos++];
            if(Rpos > Rend)
                break;
        }
    }
    while(Lpos <= Lend)
        tmpArray[tmpPos++] = array[Lpos++];
    while(Rpos <= Rend)
        tmpArray[tmpPos++] = array[Rpos++];
    for(Lpos = Lstart; Lpos <= Rend; Lpos++)
        array[Lpos] = tmpArray[Lpos];
}
static void Msort(int *array, int *tmpArray, int left, int right){
    int center;
    if(left < right){
        center = (left + right) >> 1;
        Msort(array, tmpArray, left, center);
        Msort(array, tmpArray, ++center, right);
        Merge(array, tmpArray, left, center, right);
    }
}
void MergeSort(int *array, int len){
    int *tmpArray = malloc(len << 2);
    if(tmpArray){
        Msort(array, tmpArray, 0, len - 1);
        free(tmpArray);
    }
}
static int Median(int *array, int left, int right){
    int center;
    center = (left + right) >> 1;
    if(array[left] > array[center])
        Swap(array +left, array+center);
    if(array[left] > array[right])
        Swap(array+left, array+right);
    if(array[center] > array[right])
        Swap(array+center, array+right);
    Swap(array+center, array+right - 1);
    return array[right - 1];
}
static void Qsort(int *array, int left, int right){
    int i, j, pivot;
    if(left + 10 < right) {
        pivot = Median(array, left, right);
        i = left;
        j = right - 1;
        while (1) {
            while (array[++i] < pivot);
            while (array[--j] > pivot);
            if (i < j)
                Swap(array+i, array+j);
            else
                break;
        }
        Swap(array+i, array+right - 1);
        Qsort(array, left, i - 1);
        Qsort(array, i + 1, right);
    }else
        InsertionSort(array + left, right - left + 1);
}
void QuickSort(int *array, int len){
    Qsort(array, 0, len - 1);
}
static void Bsort(int *iArray, int *oArray, int len, int *digits, int time){
    int i, shift, tmp1, tmp2;
    shift = time << 2;
    for(i = 0; i < 16; i++)
        digits[i] = 0;
    for(i = 0; i < len; i++)
        digits[iArray[i] >> shift & 15]++;
    tmp1 = digits[0];
    digits[0] = 0;
    for(i = 1; i < 16; i++){
        tmp2 = digits[i];
        digits[i] = tmp1 + digits[i - 1];
        tmp1 = tmp2;
    }
    for(i = 0; i < len; i++){
        oArray[digits[iArray[i] >> shift & 15]++] = iArray[i];
    }
}
void BucketSort(int *array, int len){
    int i, time, digits[16];
    int *tmpArray = malloc(len << 2);
    time = 0;
    do{
        Bsort(array, tmpArray, len, digits, time);
        if(digits[1] == len) {
            for (i = 0; i < len; i++)
                array[i] = tmpArray[i];
            break;
        }
        time++;
        Bsort(tmpArray, array, len, digits, time);
        time++;
    }while(digits[1] != len);
    free(tmpArray);
}
