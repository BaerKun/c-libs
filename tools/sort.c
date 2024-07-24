#include "sort.h"
#include <stdlib.h>

void shellSort(SORT_ELEMENT_TYPE *array, int len) {
    int i, j, delta;
    SORT_ELEMENT_TYPE tmp;
    for (delta = 1; delta < len; delta = delta << 1 | 1);
    for (delta >>= 1; delta; delta >>= 1) {
        for (i = delta; i < len; i++) {
            tmp = array[i];
            for (j = i; j >= delta && SORT_LESS_THAN(tmp, array[j - delta]); j -= delta)
                array[j] = array[j - delta];
            array[j] = tmp;
        }
    }
}

static void percDown(SORT_ELEMENT_TYPE *array, int father, int len) {
    const SORT_ELEMENT_TYPE theTop = array[father];
    for (int child; (child = father << 1) < len; father = child) {
        if (child + 1 != len && SORT_LESS_THAN(array[child + 1], array[child]))
            child++;
        if (SORT_LESS_THAN(theTop, array[child]))
            array[father] = array[child];
        else
            break;
    }
    array[father] = theTop;
}

void heapSort(SORT_ELEMENT_TYPE *array, int len) {
    int i;
    array--;
    len++;
    for (i = len >> 1; i; i--)
        percDown(array, i, len);
    for (i = len - 1; i; i--) {
        swap(array + i, array + 1);
        percDown(array, 1, i);
    }
}

static void merge(SORT_ELEMENT_TYPE *array, SORT_ELEMENT_TYPE *tmpArray, int Lpos, int Rpos, int Rend) {
    int Lend, tmpPos, Lstart;
    Lend = Rpos - 1;
    tmpPos = Lstart = Lpos;
    while (1) {
        if (SORT_LESS_THAN(array[Lpos], array[Rpos])) {
            tmpArray[tmpPos++] = array[Lpos++];
            if (Lpos > Lend)
                break;
        } else {
            tmpArray[tmpPos++] = array[Rpos++];
            if (Rpos > Rend)
                break;
        }
    }
    while (Lpos <= Lend)
        tmpArray[tmpPos++] = array[Lpos++];
    while (Rpos <= Rend)
        tmpArray[tmpPos++] = array[Rpos++];
    for (Lpos = Lstart; Lpos <= Rend; Lpos++)
        array[Lpos] = tmpArray[Lpos];
}

static void _mergeSort(SORT_ELEMENT_TYPE *array, int *tmpArray, int left, int right) {
    if (left < right) {
        int center = (left + right) >> 1;
        _mergeSort(array, tmpArray, left, center);
        _mergeSort(array, tmpArray, ++center, right);
        merge(array, tmpArray, left, center, right);
    }
}

void mergeSort(SORT_ELEMENT_TYPE *array, int len) {
    SORT_ELEMENT_TYPE *tmpArray = malloc(len * sizeof(SORT_ELEMENT_TYPE));
    if (tmpArray) {
        _mergeSort(array, tmpArray, 0, len - 1);
        free(tmpArray);
    }
}

static SORT_ELEMENT_TYPE median(SORT_ELEMENT_TYPE *array, int left, int right) {
    const int center = (left + right) >> 1;

    if (SORT_LESS_THAN(array[center], array[left]))
        swap(array + left, array + center);
    if (SORT_LESS_THAN(array[right], array[left]))
        swap(array + left, array + right);
    if (SORT_LESS_THAN(array[right], array[center]))
        swap(array + center, array + right);

    swap(array + center, array + right - 1);

    return array[right - 1];
}

static int quickBody(SORT_ELEMENT_TYPE *array, int left, int right) {
    const SORT_ELEMENT_TYPE pivot = median(array, left, right);
    int i = left, j = right - 1;

    while (1) {
        while (SORT_LESS_THAN(array[++i], pivot));
        while (SORT_LESS_THAN(pivot, array[--j]));
        if (i < j)
            swap(array + i, array + j);
        else
            break;
    }
    swap(array + i, array + right - 1);

    return i;
}

static void _quickSort(SORT_ELEMENT_TYPE *array, int left, int right) {
    if (left + 10 < right) {
        int i = quickBody(array, left, right);
        _quickSort(array, left, i - 1);
        _quickSort(array, i + 1, right);
    } else
        insertionSort(array + left, right - left + 1);
}

void quickSort(SORT_ELEMENT_TYPE *array, int len) {
    _quickSort(array, 0, len - 1);
}

void _quickSelect(SORT_ELEMENT_TYPE *array, int left, int right, int number) {
    if (left + 10 < right) {
        int i = quickBody(array, left, right);
        if(number < i)
            _quickSelect(array, left, i - 1, number);
        else if(number > i)
            _quickSelect(array, i + 1, right, number);
    } else
        insertionSort(array + left, right - left + 1);
}

void quickSelect(SORT_ELEMENT_TYPE *array, int len, int number) {
    _quickSelect(array, 0, len - 1, number - 1);
}

static void _bucketSort(SORT_ELEMENT_TYPE *iArray, SORT_ELEMENT_TYPE *oArray, int len, int *buckets, int time) {
    int i, shift;
    SORT_ELEMENT_TYPE tmp1, tmp2;
    shift = time << 2;
    for (i = 0; i < 16; i++)
        buckets[i] = 0;
    for (i = 0; i < len; i++)
        buckets[iArray[i] >> shift & 15]++;

    tmp1 = buckets[0];
    buckets[0] = 0;
    for (i = 1; i < 16; i++) {
        tmp2 = buckets[i];
        buckets[i] = tmp1 + buckets[i - 1];
        tmp1 = tmp2;
    }
    for (i = 0; i < len; i++) {
        oArray[buckets[iArray[i] >> shift & 15]++] = iArray[i];
    }
}

void bucketSort(SORT_ELEMENT_TYPE *array, int len) {
    int i, time, buckets[16];
    SORT_ELEMENT_TYPE *tmpArray = malloc(len * sizeof(SORT_ELEMENT_TYPE));
    time = 0;
    do {
        _bucketSort(array, tmpArray, len, buckets, time);
        if (buckets[1] == len) {
            for (i = 0; i < len; i++)
                array[i] = tmpArray[i];
            break;
        }
        time++;
        _bucketSort(tmpArray, array, len, buckets, time);
        time++;
    } while (buckets[1] != len);
    free(tmpArray);
}
