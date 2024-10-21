#include "auto_diff.h"
#include <math.h>
#include <stdlib.h>

float calculate(const MathFunction func, const float var[]) {
    if (func->type == CONSTANT)
        return func->value;

    if (func->type >= VARIABLE)
        return func->value = var[func->type];

    ComputationNodePtr operand = func->operand;
    switch (func->operation) {
        case ADD:
            func->value = 0;
            do {
                func->value += calculate(operand, var);
                operand = operand->next;
            } while (operand != NULL);
            break;
        case SUB:
            func->value = calculate(operand->next, var) - calculate(operand, var);
            break;
        case MUL:
            func->value = 1;
            do {
                func->value *= calculate(operand, var);
                operand = operand->next;
            } while (operand != NULL);
            break;
        case DIV:
            func->value = calculate(operand->next, var) / calculate(operand, var);
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
