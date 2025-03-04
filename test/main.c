#include "math_func.h"
#include <stdio.h>

int main() {
    const ComputationNodePtr x = variable(0);
    MathFunction *func = mfAdd(mfMul(x, mfExp(x)), mfPow(x, constant(2)));

    const float lr = 0.1f;
    x->value = 2;
    for (int i = 0; i < 100; i++) {
        autoDiff(func);
        const float grad = x->grad;
        x->value -= lr * grad;
        printf("x = %f, f(x) = %f\n", x->value, calculate(func));
    }
    return 0;
}