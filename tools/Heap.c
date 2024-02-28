#include "Heap.h"
#include <stdlib.h>
#include <stdio.h>

static void PercolateDown(HeapElement *array, int father, int len){
    int child, theTop;
    theTop = array[father];
    for( ; (child = father << 1) < len; father = child){
        if(child + 1 != len && array[child + 1] < array[child])
            child++;
        if(array[child] < theTop)
            array[father] = array[child];
        else
            break;
    }
    array[father] = theTop;
}

Heap CreateHeap(int capacity){
    Heap heap = malloc(sizeof(struct Heap));
    heap->capacity = capacity;
    heap->size = 0;
    heap->elements = malloc((capacity + 1) * sizeof(HeapElement));
    heap->elements[0] = MINUS_INFINITY;
    return heap;
}

void HeapInsert(Heap heap, HeapElement element){
    int i, child;
    if(heap->capacity == heap->size){
        fputs("HeapInsert:Full\n", stderr);
        return;
    }
    for(i = ++heap->size; heap->elements[child = i >> 1] > element; i = child)
        heap->elements[i] = heap->elements[child];
    heap->elements[i] = element;
}

HeapElement DeleteMin(Heap heap){
    HeapElement theMin;
    if(heap->size == 0){
        fputs("DeleteMin:Empty\n", stderr);
        return MINUS_INFINITY;
    }
    theMin = heap->elements[1];
    heap->elements[1] = heap->elements[heap->size];
    PercolateDown(heap->elements, 1, heap->size--);
    return theMin;
}

void DeleteHeap(Heap heap){
    free(heap->elements);
    free(heap);
}

void BuildHeap(HeapElement *array, int len){
    for(int i = len >> 1; i; i--){
        PercolateDown(array, i, len);
    }
}