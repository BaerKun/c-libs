#ifndef GRAPH_QUEUE_H
#define GRAPH_QUEUE_H

#define InitQueue(queue, capacity) \
    Vertex elements[capacity];    \
    (queue).front = (queue).rear = 0;   \
    (queue).elements = elements

#define Enqueue(queue, element) \
    (queue).elements[(queue).rear++] = element

#define Dequeue(queue) (queue).elements[(queue).front++]

#include "graph.h"

typedef struct {
    int front;
    int rear;
    Vertex *elements;
}Queue, *QueuePtr;

#endif //GRAPH_QUEUE_H
