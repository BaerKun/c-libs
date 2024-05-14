#ifndef STACK_H
#define STACK_H

typedef int StackElement;
typedef struct Stack Stack, *StackPtr;

struct Stack{
    StackElement *elements;
    int capacity;
    int top;
};

StackPtr newStack(int capacity);

void stackPush(StackPtr stack, StackElement element);

StackElement stackPop(StackPtr stack);

void deleteStack(StackPtr stack);

#endif //STACK_H
