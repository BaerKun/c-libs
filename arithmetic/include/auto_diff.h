#ifndef AUTO_DIFF_H
#define AUTO_DIFF_H

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
    NumberType type;
    OperationType operation;
    float value, grad;
    ComputationLink *link;
    ComputationLink **lastNext;
};

ComputationNode *constant(float value);

ComputationNode *variable(int index);

ComputationNode *intermediate(OperationType operation);

void linkOperand(ComputationNodePtr node, ComputationNodePtr operand);

float calculate(MathFunction *func);

float autoDiff(MathFunction *func);

MathFunction *derivative(const MathFunction *func);

void printFunc(const MathFunction *func);

static ComputationNodePtr mfAdd(const ComputationNodePtr a, const ComputationNodePtr b) {
    const ComputationNodePtr node = intermediate(ADD);
    linkOperand(node, a);
    linkOperand(node, b);
    return node;
}

static ComputationNodePtr mfInv(const ComputationNodePtr a) {
    const ComputationNodePtr node = intermediate(INV);
    linkOperand(node, a);
    return node;
}

static ComputationNodePtr mfSub(const ComputationNodePtr a, const ComputationNodePtr b) {
    const ComputationNodePtr node = mfAdd(a, mfInv(b));
    return node;
}

static ComputationNodePtr mfMul(const ComputationNodePtr a, const ComputationNodePtr b) {
    const ComputationNodePtr node = intermediate(MUL);
    linkOperand(node, a);
    linkOperand(node, b);
    return node;
}

static ComputationNodePtr mfMul3(const ComputationNodePtr a, const ComputationNodePtr b, const ComputationNodePtr c) {
    const ComputationNodePtr node = intermediate(MUL);
    linkOperand(node, a);
    linkOperand(node, b);
    linkOperand(node, c);
    return node;
}

static ComputationNodePtr mfDiv(const ComputationNodePtr a, const ComputationNodePtr b) {
    const ComputationNodePtr rec = intermediate(REC);
    linkOperand(rec, b);
    return mfMul(a, rec);
}

static ComputationNodePtr mfPow(const ComputationNodePtr a, const ComputationNodePtr b) {
    const ComputationNodePtr node = intermediate(POW);
    linkOperand(node, a);
    linkOperand(node, b);
    return node;
}

static ComputationNodePtr mfExp(const ComputationNodePtr a) {
    const ComputationNodePtr node = intermediate(EXP);
    linkOperand(node, a);
    return node;
}

static ComputationNodePtr mfLog(const ComputationNodePtr a) {
    const ComputationNodePtr node = intermediate(LOG);
    linkOperand(node, a);
    return node;
}

static ComputationNodePtr mfSin(const ComputationNodePtr a) {
    const ComputationNodePtr node = intermediate(SIN);
    linkOperand(node, a);
    return node;
}

static ComputationNodePtr mfCos(const ComputationNodePtr a) {
    const ComputationNodePtr node = intermediate(COS);
    linkOperand(node, a);
    return node;
}

static ComputationNodePtr mfTan(const ComputationNodePtr a) {
    const ComputationNodePtr node = intermediate(TAN);
    linkOperand(node, a);
    return node;
}

#endif //AUTO_DIFF_H
