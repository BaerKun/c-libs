#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

#ifndef QUEUE_ELEMENT_TYPE
#define QUEUE_ELEMENT_TYPE int
#endif

typedef struct Queue_ Queue, *QueuePtr;

struct Queue_ {
    int capacity;
    int front;
    int rear;
    int size;
    QUEUE_ELEMENT_TYPE *elements;
};

static QueuePtr newQueue(const int capacity){
    QueuePtr queue = malloc(sizeof(Queue));
    *queue = (Queue){capacity, 0, 0, 0, malloc(capacity * sizeof(QUEUE_ELEMENT_TYPE))};
    return queue;
}

static void enqueue(const QueuePtr queue, const QUEUE_ELEMENT_TYPE element){
    queue->elements[queue->rear] = element;
    if(++queue->rear == queue->capacity)
        queue->rear = 0;
    queue->size++;
}

static QUEUE_ELEMENT_TYPE dequeue(const QueuePtr queue){
    const QUEUE_ELEMENT_TYPE front = queue->elements[queue->front];
    if(++queue->front == queue->capacity)
        queue->front = 0;
    queue->size--;

    return front;
}

static void queue_makeEmpty(const QueuePtr queue){
    queue->front = queue->rear = queue->size = 0;
}

static void queue_destroy(const QueuePtr queue){
    free(queue->elements);
    free(queue);
}
#endif //QUEUE_H
