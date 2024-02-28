#ifndef QUEUE_H
#define QUEUE_H

typedef struct Queue *Queue;
typedef int QueueElementType;

struct Queue{
    int capacity;
    int front;
    int rear;
    int size;
    QueueElementType *elements;
};

Queue CreateQueue(int capacity);
void Enqueue(Queue queue, QueueElementType element);
QueueElementType Dequeue(Queue queue);
int GetSize(Queue queue);
void MakeEmpty(Queue queue);
void DeleteQueue(Queue queue);

#endif //QUEUE_H
