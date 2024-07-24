#ifndef HEAP_H
#define HEAP_H
#define MINUS_INFINITY 0x80000000

# ifndef  HEAP_ELEMENT_TYPE;
# define HEAP_ELEMENT_TYPE int
# endif

#ifndef HEAP_LESS_THAN
#define HEAP_LESS_THAN(a, b) (a < b)
#endif


#include <stdlib.h>
#include <stdio.h>

typedef struct Heap Heap, *HeapPtr;

struct Heap{
    int capacity;
    int size;
    HEAP_ELEMENT_TYPE  *elements;
};

static void PercolateDown(HEAP_ELEMENT_TYPE *array, int father, int len){
    int child, theTop;
    theTop = array[father];

    for( ; (child = father << 1) < len; father = child){
        if(child + 1 != len && HEAP_LESS_THAN(array[child + 1], array[child]))
            child++;
        if(HEAP_LESS_THAN(array[child], theTop))
            array[father] = array[child];
        else
            break;
    }
    array[father] = theTop;
}

static HeapPtr CreateHeap(int capacity){
    HeapPtr heap = malloc(sizeof(struct Heap));
    heap->capacity = capacity;
    heap->size = 0;
    heap->elements = malloc((capacity + 1) * sizeof(HEAP_ELEMENT_TYPE));
    heap->elements[0] = MINUS_INFINITY;
    return heap;
}

static void HeapInsert(HeapPtr heap, HEAP_ELEMENT_TYPE element){
    int i, child;
    if(heap->capacity == heap->size){
        fputs("HeapInsert:Full\n", stderr);
        return;
    }
    for(i = ++heap->size; HEAP_LESS_THAN(element, heap->elements[child = i >> 1]); i = child)
        heap->elements[i] = heap->elements[child];
    heap->elements[i] = element;
}

static HEAP_ELEMENT_TYPE DeleteMin(HeapPtr heap){
    if(heap->size == 0){
        fputs("DeleteMin:Empty\n", stderr);
        return MINUS_INFINITY;
    }

    HEAP_ELEMENT_TYPE theMin = heap->elements[1];
    heap->elements[1] = heap->elements[heap->size];
    PercolateDown(heap->elements, 1, heap->size--);
    return theMin;
}

static void DeleteHeap(HeapPtr heap){
    free(heap->elements);
    free(heap);
}

static void BuildHeap(HEAP_ELEMENT_TYPE *array, int len){
    for(int i = len >> 1; i; i--){
        PercolateDown(array, i, len);
    }
}

#endif //HEAP_H
