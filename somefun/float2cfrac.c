#include <stdio.h>
#include <math.h>

void float2fraction_shift(const float x, int *numerator, int *rightShift, const int maxShift) {
    const int *px = (int *) &x;
    int _numerator = *px & 0x007FFFFF | 0x00800000;
    int _rightShift = 23 - (((*px & 0x7F800000) >> 23) - 127);

    if (*px & 0x80000000) {
        printf("input is negative!");
    }

    while (_rightShift > maxShift || (_numerator & 1) == 0) {
        _numerator >>= 1;
        _rightShift--;
    }

    if (_rightShift < 0) {
        printf("rightShift is negative!");
    }

    *numerator = _numerator;
    *rightShift = _rightShift;
}

int greatestCommonDivisor(int a, int b) {
    int t;
    while (b != 0) {
        t = a % b;
        a = b;
        b = t;
    }
    return a;
}

void float2fraction_precise0(float x, int *numerator, int *denominator, const int maxDeno) {
    const int * const px = (int *) &x,
        exponent = ((*px & 0x7F800000) >> 23) - 127;
    int _nume = *px & 0x007FFFFF | 0x00800000,
        _deno,
        isNagative = 0;

    if (exponent >= 23) {
        if (exponent >= 31) {
            printf("input is too large!");
            *denominator = 0;
            return;
        }
        *numerator = x;
        *denominator = 1;
        return;
    }
    if (exponent < -8) {
        if (exponent < -31) {
            printf("input is too small!");
            *numerator = 0;
            return;
        }
        _nume >>= -exponent - 8;
        _deno = 0x7fffffff;
    } else {
        _deno = 1 << 23 - exponent;
    }

    if (x < 0.f) {
        isNagative = 1;
        x = -x;
    }

    int gcd,
        outNume = _nume,
        outDeno = _deno;
    float diff1, diff2,
        minDiff = INFINITY;
    do {
        diff1 = fabs(x - (float) _nume / _deno);
        while (1) {
            diff2 = fabs(x - (float) (_nume - 1) / _deno);
            if (diff1 < diff2)
                break;
            diff1 = diff2;
            --_nume;
        }

        if (diff1 > minDiff)
            continue;

        gcd = greatestCommonDivisor(_deno, _nume);
        if (_deno / gcd > maxDeno)
            continue;

        minDiff = diff1;
        outNume = _nume / gcd;
        outDeno = _deno / gcd;
    } while (--_deno > 0);

    *numerator = isNagative ? -outNume : outNume;
    *denominator = outDeno;
}

void float2fraction_precise(float x, int *numerator, int *denominator, const int maxDeno) {
    const int * const px = (int *) &x,
        exponent = ((*px & 0x7F800000) >> 23) - 127;
    int deno,
        isNagative = 0;

    if (exponent >= 23) {
        if (exponent >= 31) {
            printf("input is too large!");
            *denominator = 0;
            return;
        }
        *numerator = x;
        *denominator = 1;
        return;
    }
    if (exponent < -8) {
        if (exponent < -31) {
            printf("input is too small!");
            *numerator = 0;
            return;
        }
        deno = 0x7fffffff;
    } else {
        deno = 1 << 23 - exponent;
    }

    if (x < 0.f) {
        isNagative = 1;
        x = -x;
    }

    int n, d, gcd,
        outNume = 0,
        outDeno = 1;
    float diff1, diff2,
        minDiff = INFINITY;

    do {
        diff1 = fabs(x - (float) n / d);
        while (1) {
            diff2 = fabs(x - (float) (n + 1) / d);
            if (diff1 < diff2)
                break;
            diff1 = diff2;
            ++n;
        }

        if (diff1 >= minDiff)
            continue;

        gcd = greatestCommonDivisor(d, n);
        if (d / gcd > maxDeno)
            continue;

        minDiff = diff1;
        outNume = n / gcd;
        outDeno = d / gcd;
    } while (d++ < deno);

    *numerator = isNagative ? -outNume : outNume;
    *denominator = outDeno;
}

int main() {
    int n = 40;
    int p = 1;
    do {
        if (n <= 4) {
            p *= n;
            break;
        }
        p *= 3;
        n -= 3;
    } while (1);
    printf("%d", p);
    return 0;
}
// 2721 1001