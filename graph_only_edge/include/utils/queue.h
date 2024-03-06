#ifndef GRAPH_QUEUE_H
#define GRAPH_QUEUE_H

#define InitQueue(queue, capacity) \
    VertexId elements[capacity];    \
    queue.front = queue.rear = 0;   \
    queue.elements = elements

#define Enqueue(queue, element) \
    queue.elements[queue.rear++] = element

#define Dequeue(queue) queue.elements[queue.front++]

typedef int VertexId;

typedef struct {
    int front;
    int rear;
    VertexId *elements;
}Queue;

#endif //GRAPH_QUEUE_H
