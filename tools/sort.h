#ifndef SORT_H
#define SORT_H

#ifndef SORT_ELEMENT_TYPE
#define SORT_ELEMENT_TYPE int
#endif

#ifndef SORT_LESS_THAN
#define SORT_LESS_THAN(x, y) ((x) < (y))
#endif

static inline void swap(SORT_ELEMENT_TYPE *x, SORT_ELEMENT_TYPE *y) {
    SORT_ELEMENT_TYPE z = *x;
    *x = *y;
    *y = z;
}

static void insertionSort(SORT_ELEMENT_TYPE *array, int size) {
    int i, j;
    SORT_ELEMENT_TYPE tmp;
    for (i = 1; i < size; i++) {
        tmp = array[i];
        for (j = i; j && SORT_LESS_THAN(tmp, array[j - 1]); j--)
            array[j] = array[j - 1];
        array[j] = tmp;
    }
}

void heapSort(SORT_ELEMENT_TYPE *array, int size);

void shellSort(SORT_ELEMENT_TYPE *array, int size);

void mergeSort(SORT_ELEMENT_TYPE *array, int size);

void quickSort(SORT_ELEMENT_TYPE *array, int size);

void bucketSort(SORT_ELEMENT_TYPE *array, int size);

#endif //SORT_H
