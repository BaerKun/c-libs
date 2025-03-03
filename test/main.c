#include "math_func.h"
#include <stdio.h>

int main() {
    ComputationNodePtr x = variable(0);
    MathFunction *func = mfAdd(mfMul(x, mfExp(x)), mfPow(x, x));
    MathFunction *deriv = derivative(func);

    x->value = 1;
    float dy = calculate(deriv);
    float _dy = autoDiff(func);
    printFunc(func);
    printf("\n");
    printFunc(deriv);
    printf("\ndy = %f, _dy = %f\n", dy, x->grad);
    deleteNode(func);
    deleteNode(deriv);
    return 0;
}