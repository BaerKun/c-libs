#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

StackPtr newStack(int capacity){
    StackPtr stack = malloc(sizeof(Stack));

    stack->elements = malloc(sizeof(StackElement) * capacity);
    stack->capacity = capacity;
    stack->top = 0;

    return stack;
}

void stackPush(StackPtr stack, StackElement element){
    if(stack->top == stack->capacity)
        fputs("stackPush: Stack is full!", stderr);
    else
        stack->elements[stack->top++] = element;
}

StackElement stackPop(StackPtr stack){
    if(stack->top == 0){
        fputs("stackPop: Stack is empty!", stderr);
        return 0;
    }
    return stack->elements[--stack->top];
}

void deleteStack(StackPtr stack){
    free(stack->elements);
    free(stack);
}