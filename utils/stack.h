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

static inline StackPtr newStack(int capacity){
    StackPtr stack = (StackPtr)malloc(sizeof(Stack));

    stack->capacity = capacity;
    stack->top = 0;
    stack->elements = (STACK_ELEMENT_TYPE *)malloc(sizeof(STACK_ELEMENT_TYPE) * capacity);

    return stack;
}

static inline void stack_push(StackPtr stack, STACK_ELEMENT_TYPE element){
//    if(stack->top == stack->capacity) {
//        fputs("stack_push: Stack is full!", stderr);
//        return;
//    }

    stack->elements[stack->top++] = element;
}

static inline STACK_ELEMENT_TYPE stack_pop(StackPtr stack){
//    if(stack->top == 0){
//        fputs("stack_pop: Stack is empty!", stderr);
//        exit(1);
//    }

    return stack->elements[--stack->top];
}

static inline STACK_ELEMENT_TYPE stack_peek(StackPtr stack){
    return stack->elements[stack->top - 1];
}

static inline void stack_destroy(StackPtr stack){
    free(stack->elements);
    free(stack);
}

#endif //STACK_H
