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

static void insertSort(SORT_ELEMENT_TYPE *array, int size) {
    int i, j;
    SORT_ELEMENT_TYPE tmp;
    for (i = 1; i < size; i++) {
        tmp = array[i];
        for (j = i; j && SORT_LESS_THAN(tmp, array[j - 1]); j--)
            array[j] = array[j - 1];
        array[j] = tmp;
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

static void quickSortHelper(SORT_ELEMENT_TYPE *array, int left, int right) {
    if (left + 10 < right) {
        int i = quickBody(array, left, right);
        quickSortHelper(array, left, i - 1);
        quickSortHelper(array, i + 1, right);
    } else
        insertSort(array + left, right - left + 1);
}

static void sort(SORT_ELEMENT_TYPE *array, int size){
    quickSortHelper(array, 0, size - 1);
}




#endif //SORT_H
