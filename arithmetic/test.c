#include  <stdio.h>
#include "arithmetic.h"
#include "arith_utils.h"
#include "timer.h"
#include "auto_diff.h"

int main() {
    const MathFunction func = createIntermediate(POW);
    linkOperand(func, createConstant(2));
    const ComputationNodePtr y = createIntermediate(SUB);
    linkOperand(func, y);
    linkOperand(y, createVariable(0));
    linkOperand(y, createConstant(4.5));
    float var[] = { 5 };
    float result = calculate(func, var);
    printf("%.2f\n", result);
}
// 2721 1001