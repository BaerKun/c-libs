#include "auto_diff.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

static float square(const float x) {
    return x * x;
}

static ComputationNode *newNode() {
    const ComputationNodePtr node = malloc(sizeof(ComputationNode));
    node->link = NULL;
    node->lastNext = &node->link;
    return node;
}

ComputationNode *constant(const float value) {
    const ComputationNodePtr node = newNode();

    node->type = CONSTANT;
    node->operation = UNKNOWN;
    node->value = value;
    return node;
}

ComputationNode *variable(const int index) {
    const ComputationNodePtr node = newNode();

    node->type = index;
    node->operation = UNKNOWN;
    return node;
}

ComputationNode *intermediate(const OperationType operation) {
    const ComputationNodePtr node = newNode();

    node->type = INTERMEDIATE;
    node->operation = operation;
    return node;
}

void linkOperand(const ComputationNodePtr node, const ComputationNodePtr operand) {
    ComputationLink *link = malloc(sizeof(ComputationLink));
    link->node = operand;
    link->next = NULL;
    *node->lastNext = link;
    node->lastNext = &link->next;
}

float calculate(MathFunction *const func) {
    func->grad = 0;
    if (func->type != INTERMEDIATE)
        return func->value;

    const ComputationLink *link = func->link;
    const ComputationNodePtr operand = link->node;
    switch (func->operation) {
        case ADD:
            func->value = 0.f;
            do {
                func->value += calculate(link->node);
                link = link->next;
            } while (link != NULL);
            break;
        case INV:
            func->value = -calculate(operand);
            break;
        case MUL:
            func->value = 1.f;
            do {
                func->value *= calculate(link->node);
                link = link->next;
            } while (link != NULL);
            break;
        case REC:
            func->value = 1.f / calculate(operand);
            break;
        case POW:
            func->value = powf(calculate(operand), calculate(link->next->node));
            break;
        case EXP:
            func->value = expf(calculate(operand));
            break;
        case LOG:
            func->value = logf(calculate(operand));
            break;
        case SIN:
            func->value = sinf(calculate(operand));
            break;
        case COS:
            func->value = cosf(calculate(operand));
            break;
        case TAN:
            func->value = tanf(calculate(operand));
            break;
        default:
            break;
    }
    return func->value;
}

static void backward(const MathFunction *const func, float grad) {
    if (func->type != INTERMEDIATE)
        return;

    const ComputationLink *link = func->link;
    ComputationNode *operand = link->node;
    float _grad;
    switch (func->operation) {
        case ADD:
            do {
                operand = link->node;
                operand->grad += grad;
                backward(operand, grad);
                link = link->next;
            } while (link != NULL);
            break;
        case INV:
            grad = -grad;
            operand->grad += grad;
            backward(operand, grad);
            break;
        case MUL:
            grad *= func->value;
            do {
                operand = link->node;
                _grad = grad / operand->value;
                operand->grad += _grad;
                backward(operand, _grad);
                link = link->next;
            } while (link != NULL);
            break;
        case REC:
            grad = -grad / square(operand->value);
            operand->grad += grad;
            backward(operand, grad);
            break;
        case POW:
            ComputationNode *const exponent = link->next->node;
            grad *= func->value;

            _grad = grad * exponent->value / operand->value;
            operand->grad += _grad;
            backward(operand, _grad);

            _grad = grad * logf(operand->value);
            exponent->grad += _grad;
            backward(exponent, _grad);
            break;
        case EXP:
            grad *= func->value;
            operand->grad += grad;
            backward(operand, grad);
            break;
        case LOG:
            grad /= operand->value;
            operand->grad += grad;
            backward(operand, grad);
            break;
        case SIN:
            grad *= cosf(operand->value);
            operand->grad += grad;
            backward(operand, grad);
            break;
        case COS:
            grad *= -sinf(operand->value);
            operand->grad += grad;
            backward(operand, grad);
            break;
        case TAN:
            grad *= 1.f + square(func->value);
            operand->grad += grad;
            backward(operand, grad);
            break;
        default:
            break;
    }
}

float autoDiff(MathFunction *func) {
    const float result = calculate(func);

    backward(func, func->grad = 1.f);

    return result;
}

void printFunc(const MathFunction *func) {
    if (func->type == CONSTANT) {
        printf("%.2f", func->value);
        return;
    }
    if (func->type == VARIABLE) {
        printf("x%d", func->type);
        return;
    }
    const ComputationLink *link = func->link;
    ComputationNodePtr operand = link->node;
    switch (func->operation) {
        case ADD:
            putchar('(');
            while (1) {
                printFunc(operand);
                if (link->next == NULL)
                    break;
                printf(" + ");
                link = link->next;
                operand = link->node;
            }
            putchar(')');
            break;
        case MUL:
            while (1) {
                printFunc(operand);
                if (link->next == NULL)
                    break;
                printf(" * ");
                link = link->next;
                operand = link->node;
            }
            break;
        case INV:
            putchar('-');
            printFunc(operand);
            break;
        case REC:
            putchar('(');
            printFunc(operand);
            printf(")^(-1)");
            break;
        case EXP:
            printf("exp(");
            printFunc(operand);
            putchar(')');
            break;
        case LOG:
            printf("log(");
            printFunc(operand);
            putchar(')');
            break;
        case POW:
            putchar('(');
            printFunc(operand);
            printf(")^(");
            printFunc(link->next->node);
            putchar(')');
            break;
        case SIN:
            printf("sin(");
            printFunc(operand);
            putchar(')');
            break;
        case COS:
            printf("cos(");
            printFunc(operand);
            putchar(')');
            break;
        case TAN:
            printf("tan(");
            printFunc(operand);
            putchar(')');
            break;
        default:
            break;
    }
}

MathFunction *derivative(const MathFunction *func) {
    if (func->type == CONSTANT)
        return constant(0);
    if (func->type == VARIABLE)
        return constant(1);

    const ComputationLink *link = func->link;
    const ComputationNodePtr operand = link->node;
    ComputationNodePtr diff;
    switch (func->operation) {
        case ADD:
            diff = intermediate(ADD);
            do {
                linkOperand(diff, derivative(link->node));
                link = link->next;
            } while (link != NULL);
            break;
        case INV:
            diff = mfInv(derivative(operand));
            break;
        case MUL:
            diff = intermediate(MUL);
            do {
                /* ... */
                link = link->next;
            } while (link != NULL);
            break;
        case REC:
            diff = mfInv(mfDiv(derivative(operand), mfPow(operand, constant(2))));
            break;
        case POW:
            const ComputationNodePtr exponent = link->next->node;
            diff = mfAdd(mfMul3(derivative(operand), exponent, mfPow(operand, mfSub(exponent, constant(1)))),
                mfMul3(derivative(exponent), mfLog(operand), mfPow(operand, exponent)));
            break;
        case EXP:
            diff = mfMul(derivative(operand), mfExp(operand));
            break;
        case LOG:
            diff = mfDiv(derivative(operand), operand);
            break;
        case SIN:
            diff = mfMul(derivative(operand), mfCos(operand));
            break;
        case COS:
            diff = mfInv(mfMul(derivative(operand), mfSin(operand)));
            break;
        case TAN:
            diff = mfMul(derivative(operand), mfPow(mfCos(operand), constant(-2)));
            break;
        default:
            diff = NULL;
            break;
    }
    return diff;
}
