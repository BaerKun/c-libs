#ifndef MATH_FUNC_H
#define MATH_FUNC_H

typedef enum {
    CONSTANT = -2, INTERMEDIATE = -1, VARIABLE = 0
} NumberType;

typedef enum {
    UNKNOWN, ADD, INV, MUL, REC, POW, EXP, LOG, SIN, COS, TAN
} OperationType;

typedef struct ComputationNode_ ComputationNode, MathFunction, *ComputationNodePtr;
typedef struct ComputationLink_ ComputationLink;

struct ComputationLink_ {
    ComputationNode *node;
    ComputationLink *next;
};

struct ComputationNode_ {
    float value, grad;
    NumberType type;
    OperationType operation;
    ComputationLink *link;
    ComputationLink **lastNext;
    int shared;
};

ComputationNode *constant(float value);

ComputationNode *variable(int index);

ComputationNode *intermediate(OperationType operation);

void deleteNode(ComputationNodePtr node);

void linkOperand(ComputationNodePtr node, ComputationNodePtr operand);

float calculate(MathFunction *func);

float autoDiff(MathFunction *func);

MathFunction *derivative(const MathFunction *func);

void printFunc(const MathFunction *func);

ComputationNodePtr mfAdd(ComputationNodePtr a, ComputationNodePtr b);

ComputationNodePtr mfInv(ComputationNodePtr a);

ComputationNodePtr mfSub(ComputationNodePtr a, ComputationNodePtr b);

ComputationNodePtr mfMul(ComputationNodePtr a, ComputationNodePtr b);

ComputationNodePtr mfMul3(ComputationNodePtr a, ComputationNodePtr b, ComputationNodePtr c);

ComputationNodePtr mfDiv(ComputationNodePtr a, ComputationNodePtr b);

ComputationNodePtr mfPow(ComputationNodePtr a, ComputationNodePtr b);

ComputationNodePtr mfExp(ComputationNodePtr a);

ComputationNodePtr mfLog(ComputationNodePtr a);

ComputationNodePtr mfSin(ComputationNodePtr a);

ComputationNodePtr mfCos(ComputationNodePtr a);

ComputationNodePtr mfTan(ComputationNodePtr a);

#endif //MATH_FUNC_H
