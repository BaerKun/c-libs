#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdio.h>

#ifndef QUEUE_ELEMENT_TYPE
#define QUEUE_ELEMENT_TYPE int
#endif

typedef struct Queue Queue, *QueuePtr;

struct Queue{
    int capacity;
    int front;
    int rear;
    int size;
    QUEUE_ELEMENT_TYPE *elements;
};

static QueuePtr newQueue(int capacity){
    QueuePtr queue = malloc(sizeof(struct Queue));
    *queue = (Queue){capacity, 0, 0, 0, malloc(capacity * sizeof(QUEUE_ELEMENT_TYPE))};
    return queue;
}

static void enqueue(QueuePtr queue, QUEUE_ELEMENT_TYPE element){
//    if(queue->size == queue->capacity) {
//        fputs("Enqueue:Full\n", stderr);
//        return;
//    }

    queue->elements[queue->rear] = element;
    if(++queue->rear == queue->capacity)
        queue->rear = 0;
    queue->size++;
}

static QUEUE_ELEMENT_TYPE dequeue(QueuePtr queue){
//    if(queue->size == 0) {
//        fputs("Dequeue:Empty\n", stderr);
//        return -1;
//    }

    QUEUE_ELEMENT_TYPE front = queue->elements[queue->front];
    if(++queue->front == queue->capacity)
        queue->front = 0;
    queue->size--;

    return front;
}

static void queue_makeEmpty(QueuePtr queue){
    queue->front = queue->rear = queue->size = 0;
}

static void queue_destroy(QueuePtr queue){
    free(queue->elements);
    free(queue);
}
#endif //QUEUE_H
