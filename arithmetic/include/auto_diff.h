#ifndef AUTO_DIFF_H
#define AUTO_DIFF_H

typedef enum {
    CONSTANT = -2, INTERMEDIATE = -1, VARIABLE = 0
} NumberType;

typedef enum {
    UNKNOWN, ADD, INV, MUL, REC, POW, EXP, LOG, SIN, COS, TAN
} OperationType;

typedef struct ComputationGraph *ComputationNodePtr, *MathFunction;

struct ComputationGraph {
    NumberType type;
    OperationType operation;
    float value, grad;
    ComputationNodePtr operand, next;
};

float calculate(MathFunction func, const float var[]);

ComputationNodePtr createConstant(float value);

ComputationNodePtr createVariable(int index);

ComputationNodePtr createIntermediate(OperationType operation);

void linkOperand(ComputationNodePtr node, ComputationNodePtr operand);

float autoDiff(MathFunction func, const float var[]);

void destroyFunction(MathFunction func);

#endif //AUTO_DIFF_H
