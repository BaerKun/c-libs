#include "Queue.h"
#include <stdlib.h>
#include <stdio.h>

Queue CreateQueue(int capacity){
    Queue queue = malloc(sizeof(struct Queue));
    queue->elements = malloc(capacity * sizeof(QueueElementType));
    queue->capacity = capacity;
    queue->front = queue->rear = queue->size = 0;
    return queue;
}
int GetSize(Queue queue){
    return queue->size;
}
void Enqueue(Queue queue, QueueElementType element){
    if(queue->size == queue->capacity) {
        fputs("Enqueue:Full\n", stderr);
        return;
    }
    queue->elements[queue->rear] = element;
    if(++queue->rear == queue->capacity)
        queue->rear = 0;
    queue->size++;
}
QueueElementType Dequeue(Queue queue){
    QueueElementType frontElement;
    if(queue->size == 0) {
        fputs("Dequeue:Empty\n", stderr);
        return -1;
    }
    frontElement = queue->elements[queue->front];
    if(++queue->front == queue->capacity)
        queue->front = 0;
    queue->size--;
    return frontElement;
}
void MakeEmpty(Queue queue){
    queue->front = queue->rear = queue->size = 0;
}
void DeleteQueue(Queue queue){
    free(queue->elements);
    free(queue);
}