#ifndef HEAP_H
#define HEAP_H

# ifndef HEAP_ELEMENT_TYPE
# define HEAP_ELEMENT_TYPE int
# endif

#ifndef HEAP_LESS_THAN
#define HEAP_LESS_THAN(a, b) ((a) < (b))
#endif

#include <stdlib.h>
#include <stdio.h>

typedef struct Heap Heap, *HeapPtr;

struct Heap{
    int capacity;
    int size;
    HEAP_ELEMENT_TYPE  *prev; // 指向堆顶的前一个
};

static void heap_percolateDown(HEAP_ELEMENT_TYPE *prev, int father, int end){
    int child;
    HEAP_ELEMENT_TYPE theTop = prev[father];

    for( ; (child = father << 1) <= end; father = child){
        if(child != end && HEAP_LESS_THAN(prev[child + 1], prev[child]))
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
    heap->prev = malloc(capacity * sizeof(HEAP_ELEMENT_TYPE));
    heap->prev--;

    return heap;
}

static void heap_insert(HeapPtr heap, HEAP_ELEMENT_TYPE element){
//    if(heap->capacity == heap->size){
//        fputs("HeapInsert:Full\n", stderr);
//        return;
//    }

    int i, child;
    for(i = ++heap->size; i && HEAP_LESS_THAN(element, heap->prev[child = i >> 1]); i = child)
        heap->prev[i] = heap->prev[child];

    heap->prev[i] = element;
}

static HEAP_ELEMENT_TYPE heap_deleteMin(HeapPtr heap){
//    if(heap->size == 0){
//        fputs("DeleteMin:Empty\n", stderr);
//        exit(1);
//    }

    HEAP_ELEMENT_TYPE theMin = heap->prev[1];
    heap->prev[1] = heap->prev[heap->size];
    heap_percolateDown(heap->prev, 1, --heap->size);

    return theMin;
}

static void heap_destroy(HeapPtr heap){
    free(heap->prev + 1);
    free(heap);
}

static void buildHeap(HeapPtr heap, int end){
    for(int i = end >> 1; i; i--)
        heap_percolateDown(heap->prev, i, end);
    heap->size = end;
}

#endif //HEAP_H
