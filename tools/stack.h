#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdio.h>

#ifndef STACK_ELEMENT_TYPE
#define STACK_ELEMENT_TYPE int
#endif

typedef struct Stack Stack, *StackPtr;

struct Stack{
    STACK_ELEMENT_TYPE *elements;
    int capacity;
    int top;
};

static StackPtr newStack(int capacity){
    StackPtr stack = malloc(sizeof(Stack));

    *stack = (Stack){malloc(sizeof(STACK_ELEMENT_TYPE) * capacity), capacity, 0};

    return stack;
}

static void stack_push(StackPtr stack, STACK_ELEMENT_TYPE element){
    if(stack->top == stack->capacity)
        fputs("stack_push: Stack is full!", stderr);
    else
        stack->elements[stack->top++] = element;
}

static STACK_ELEMENT_TYPE stack_pop(StackPtr stack){
    if(stack->top == 0){
        fputs("stack_pop: Stack is empty!", stderr);
        exit(1);
    }
    return stack->elements[--stack->top];
}

static void stack_destroy(StackPtr stack){
    free(stack->elements);
    free(stack);
}

#endif //STACK_H
