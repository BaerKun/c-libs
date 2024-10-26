#include "auto_diff.h"
#include <math.h>
#include <stdlib.h>


float calculate(const MathFunction func, const float var[]) {
    func->grad = 0.f;
    if (func->type == CONSTANT)
        return func->value;

    if (func->type >= VARIABLE)
        return func->value = var[func->type];

    ComputationNodePtr operand = func->operand;
    switch (func->operation) {
        case ADD:
            func->value = 0.f;
            do {
                func->value += calculate(operand, var);
                operand = operand->next;
            } while (operand != NULL);
            break;
        case INV:
            func->value = -calculate(operand, var);
            break;
        case MUL:
            func->value = 1.f;
            do {
                func->value *= calculate(operand, var);
                operand = operand->next;
            } while (operand != NULL);
            break;
        case REC:
            func->value = 1.f / calculate(operand, var);
            break;
        case POW:
            func->value = powf(calculate(operand->next, var), calculate(operand, var));
            break;
        case EXP:
            func->value = expf(calculate(operand, var));
            break;
        case LOG:
            func->value = logf(calculate(operand, var));
            break;
        case SIN:
            func->value = sinf(calculate(operand, var));
            break;
        case COS:
            func->value = cosf(calculate(operand, var));
            break;
        case TAN:
            func->value = tanf(calculate(operand, var));
            break;
        default:
            break;
    }
    return func->value;
}

static ComputationNodePtr newNode() {
    const ComputationNodePtr node = malloc(sizeof(struct ComputationGraph));
    node->grad = 0;
    node->operand = node->next = NULL;
    return node;
}

ComputationNodePtr createConstant(const float value) {
    const ComputationNodePtr node = newNode();

    node->type = CONSTANT;
    node->operation = UNKNOWN;
    node->value = value;
    return node;
}

ComputationNodePtr createVariable(const int index) {
    const ComputationNodePtr node = newNode();

    node->type = index;
    node->operation = UNKNOWN;
    return node;
}

ComputationNodePtr createIntermediate(const OperationType operation) {
    const ComputationNodePtr node = newNode();

    node->type = INTERMEDIATE;
    node->operation = operation;
    return node;
}

void linkOperand(const ComputationNodePtr node, const ComputationNodePtr operand) {
    operand->next = node->operand;
    node->operand = operand;
}

static void backward(const MathFunction func) {
    if (func->type != INTERMEDIATE)
        return;

    ComputationNodePtr operand = func->operand;
    switch (func->operation) {
        case ADD:
            do {
                operand->grad += func->grad;
                backward(operand);
                operand = operand->next;
            } while (operand != NULL);
            break;
        case INV:
            operand->grad -= func->grad;
            backward(operand);
            break;
        case MUL:
            do {
                operand->grad += func->grad * func->value / operand->value;
                backward(operand);
                operand = operand->next;
            } while (operand != NULL);
            break;
        case REC:
            operand->grad -= func->grad / (operand->value * operand->value);
            backward(operand);
            break;
        case POW:
            const ComputationNodePtr base = operand->next;
            base->grad += func->grad * func->value / base->value;
            backward(base);

            operand->grad += func->grad * func->value * logf(base->value);
            backward(operand);
            break;
        case EXP:
            operand->grad += func->grad * func->value;
            break;
        case LOG:
            operand->grad += func->grad / operand->value;
            break;
        case SIN:
            operand->grad += func->grad * cosf(operand->value);
            break;
        case COS:
            operand->grad += func->grad * -sinf(operand->value);
            break;
        case TAN:
            const float cosVal = cosf(operand->value);
            operand->grad += func->grad / (cosVal * cosVal);
            break;
        default:
            break;
    }
}

float autoDiff(const MathFunction func, const float var[]) {
    const float result = calculate(func, var);

    func->grad = 1.f;
    backward(func);

    return result;
}

void destroyFunction(const MathFunction func) {
    if (func->type == INTERMEDIATE) {
        ComputationNodePtr operand = func->operand;
        do {
            const ComputationNodePtr next = operand->next;
            destroyFunction(operand);
            operand = next;
        } while (operand != NULL);
    }

    free(func);
}
