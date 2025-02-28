#include "auto_diff.h"
#include <stdio.h>

int main() {
    const ComputationNodePtr x = variable(0);
    MathFunction *func = mfPow(mfSub(constant(1), mfTan(x)), constant(2));

    x->value = 3.f / 4;
    MathFunction *deriv = derivative(func);
    printFunc(deriv);
    printf("\n%.2f\n", calculate(deriv));
}