#ifndef HEAP_H
#define HEAP_H

# ifndef HEAP_ELEMENT_TYPE
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
    int needFreeElements;
    HEAP_ELEMENT_TYPE  *prev; // 指向堆顶的前一个
};

static void heap_percolateDown(HEAP_ELEMENT_TYPE *prev, int father, int end){
    int child;
    HEAP_ELEMENT_TYPE theTop;
    theTop = prev[father];

    for( ; (child = father << 1) <= end; father = child){
        if(child + 1 <= end && HEAP_LESS_THAN(prev[child + 1], prev[child]))
            child++;
        if(HEAP_LESS_THAN(prev[child], theTop))
            prev[father] = prev[child];
        else
            break;
    }
    prev[father] = theTop;
}

static HeapPtr newHeap(int capacity){
    HeapPtr heap = malloc(sizeof(struct Heap));
    heap->capacity = capacity;
    heap->size = 0;
    heap->needFreeElements = 1;
    heap->prev = malloc(capacity * sizeof(HEAP_ELEMENT_TYPE));
    heap->prev--;

    return heap;
}

static void heap_insert(HeapPtr heap, HEAP_ELEMENT_TYPE element){
    int i, child;
    if(heap->capacity == heap->size){
        fputs("HeapInsert:Full\n", stderr);
        return;
    }

    for(i = ++heap->size; i && HEAP_LESS_THAN(element, heap->prev[child = i >> 1]); i = child)
        heap->prev[i] = heap->prev[child];

    heap->prev[i] = element;
}

static HEAP_ELEMENT_TYPE heap_deleteMin(HeapPtr heap){
    if(heap->size == 0){
        fputs("DeleteMin:Empty\n", stderr);
        exit(1);
    }

    HEAP_ELEMENT_TYPE theMin = heap->prev[1];
    heap->prev[1] = heap->prev[heap->size];
    heap_percolateDown(heap->prev, 1, --heap->size);

    return theMin;
}

static void heap_destroy(HeapPtr heap){
    if(heap->needFreeElements)
        free(heap->prev + 1);
    free(heap);
}

static HeapPtr buildHeap(HEAP_ELEMENT_TYPE *prev, int end){
    for(int i = end >> 1; i; i--)
        heap_percolateDown(prev, i, end);

    HeapPtr heap = malloc(sizeof(Heap));
    heap->capacity = heap->size = end;
    heap->prev = prev;
    heap->needFreeElements = 0;

    return heap;
}

#endif //HEAP_H
