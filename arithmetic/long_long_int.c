#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

void lldMov(uint8_t *a, const uint8_t *b, uint32_t size) {
    memcpy(a, b, size);
}

void lldSet(uint8_t *a, uint8_t val, uint32_t size) {
    memset(a, val, size);
}

// a > b -> 1; a == b -> 0; a < b -> -1
int8_t lldComp(uint8_t *a, uint8_t *b, uint32_t size) {
    const uint8_t *aptr = a + size, *bptr = b + size;

    while (aptr != a) {
        --aptr;
        --bptr;
        if (*aptr > *bptr)
            return 1;
        if (*aptr < *bptr)
            return -1;
    }
    return 0;
}

void lldAdd(uint8_t *output, const uint8_t *input1, const uint8_t *input2, uint32_t size) {
    uint16_t buffer, carry = 0;
    uint8_t *outptr = output;
    const uint8_t *inptr1 = input1, *inptr2 = input2, *end = output + size;

    do {
        buffer = carry + *inptr1 + *inptr2;
        carry = buffer >> 8;
        *outptr = buffer;
        ++inptr1;
        ++inptr2;
    } while (++outptr != end);
}

void lldAdd32(uint8_t *output, const uint8_t *input1, uint32_t input2, uint32_t size) {
    uint64_t buffer = input2;
    uint8_t *outptr = output;
    const uint8_t *inptr = input1,
        *end = output + size;

    while (outptr != end && buffer != 0) {
        buffer += *inptr;
        *outptr = buffer;
        buffer >>= 8;
        ++inptr;
        ++outptr;
    }
}

void lldComplement(uint8_t *out, const uint8_t *in, uint32_t size) {
    const uint8_t *inptr = in, *end = in + size;
    uint8_t *outptr = out;

    do {
        if (inptr == end)
            return;
        if (*inptr != 0)
            break;
        *outptr = 0;
        ++outptr;
        ++inptr;
    } while (1);

    *outptr = ~*inptr + 1;
    while (++inptr != end) {
        ++outptr;
        *outptr = ~*inptr;
    }
}

void lldSub(uint8_t *output, const uint8_t *minuend, const uint8_t *subtracted, uint32_t size) {
    uint8_t *complement = malloc(size);
    lldComplement(complement, subtracted, size);
    lldAdd(output, minuend, complement, size);
    free(complement);
}

void lldMul(uint8_t *output, const uint8_t *input1, const uint8_t *input2, uint32_t size) {
    uint32_t *temp = calloc(size, 4);

    for (int j, i = 0; i != size; ++i) {
        for (j = size - i - 1; j >= 0; --j) {
            temp[i + j] += (uint32_t) input1[i] * input2[j];
        }
    }

    uint8_t *outptr = output, *end = output + size;
    uint32_t *this = temp, *next;
    do {
        *outptr = *this;
        next = this + 1;
        *next += *this >> 8;
        this = next;
    } while (++outptr != end);

    free(temp);
}

void lldMul32(uint8_t *output, const uint8_t *input1, const uint32_t input2, uint32_t size) {
    uint8_t *outptr = output;
    const uint8_t *inptr = input1, *end = input1 + size;
    uint64_t tmp = 0;

    while (inptr != end) {
        tmp = (tmp >> 8) + (uint64_t) *inptr * input2;
        *outptr = tmp;
        ++inptr;
        ++outptr;
    }
}

uint8_t *lldHigh(uint8_t *a, uint32_t size) {
    uint8_t *ptr = a + size;

    while (--ptr != a && *ptr == 0);

    return ptr;
}

void lldDiv8(uint8_t *quotient, uint8_t *dividend, uint8_t divisor, uint8_t *remainder, uint32_t size) {
    if (divisor == 0) {
        fprintf(stderr, "Division by zero!\n");
        return;
    }

    uint8_t *quot_ptr = quotient + size;
    uint8_t *divd_ptr = dividend + size;
    uint16_t divd_tmp = 0;

    while (quot_ptr != quotient) {
        --quot_ptr;
        --divd_ptr;
        divd_tmp = divd_tmp << 8 | *divd_ptr;
        if(quotient != NULL)
            *quot_ptr = divd_tmp / divisor;
        divd_tmp = divd_tmp % divisor;
    }

    if (remainder != NULL)
        *remainder = divd_tmp;
}

void lldDiv(uint8_t *quotient, uint8_t *dividend, uint8_t *divisor, uint8_t *remainder, uint32_t size) {
    const uint8_t *divisor_high = lldHigh(divisor, size);
    const uint32_t dividend_size = lldHigh(dividend, size) - dividend + 1;
    const uint32_t divisor_size = divisor_high - divisor + 1;

    if (divisor_size == 1) {
        if(remainder != NULL)
            lldSet(remainder, 0, size);
        if(quotient != NULL)
            lldSet(quotient + dividend_size, 0, size - dividend_size);
        lldDiv8(quotient, dividend, *divisor, remainder, dividend_size);
        return;
    }

    if(quotient != NULL)
        lldSet(quotient, 0, size);
    if (dividend_size < divisor_size)
        return;

    uint8_t quotient_tmp;
    uint16_t dividend_16 = 0;
    const uint16_t divisor_16 = *divisor_high << 8 | *(divisor_high - 1);
    const uint32_t divisor_size_plus = divisor_size + 1;

    uint8_t *remainder_ = remainder == NULL ? malloc(size) : remainder;
    uint8_t *remainder_high = remainder_ + dividend_size - 1;
    uint8_t *remainder_low;

    uint8_t *divisor_complement = malloc(divisor_size_plus);
    uint8_t *subtracted_buffer = malloc(divisor_size_plus);

    lldMov(remainder_, dividend, size);
    lldComplement(divisor_complement, divisor, divisor_size_plus);

    for (uint8_t *ptr = quotient + dividend_size - divisor_size; ptr >= quotient; --ptr, --remainder_high) {
        dividend_16 = dividend_16 << 8 | *remainder_high;
        if (dividend_16 < *divisor_high)
            continue;

        quotient_tmp = (dividend_16 << 8 | *(remainder_high - 1)) / divisor_16;
        if (quotient_tmp == 0)
            continue;

        remainder_low = remainder_high - divisor_size + 1;
        lldMul32(subtracted_buffer, divisor, quotient_tmp, divisor_size_plus);

        if (lldComp(subtracted_buffer, remainder_low, divisor_size_plus) == 1) {
            lldAdd(subtracted_buffer, subtracted_buffer, divisor_complement, divisor_size_plus);
            --quotient_tmp;
        }

        if(quotient != NULL)
            *ptr = quotient_tmp;

        lldComplement(subtracted_buffer, subtracted_buffer, divisor_size_plus);
        lldAdd(remainder_low, remainder_low, subtracted_buffer, divisor_size_plus);
        dividend_16 = *remainder_high;
    }

    free(divisor_complement);
    free(subtracted_buffer);
    if (remainder == NULL)
        free(remainder_);
}

void lldPrint16(uint8_t *a, uint32_t size) {
    const uint8_t *ptr = lldHigh(a, size);
    do {
        printf("%02x", *ptr);
    } while (ptr-- != a);
}

void lldPrint10(uint8_t *a, uint32_t size) {
    uint8_t *buffer = malloc(size);
    uint8_t *stack_buffer = malloc((int)(size * 8 * 0.301029995664) + 1);
    uint8_t *top = stack_buffer;
    lldMov(buffer, a, size);

    const uint8_t *high_ptr = lldHigh(buffer, size);
    do {
        lldDiv8(buffer, buffer, 10, top, size);
        ++top;
        if(*high_ptr == 0) {
            if(high_ptr == buffer)
                break;
            --high_ptr;
        }
    } while (1);
    free(buffer);

    do {
        --top;
        printf("%d", *top);
    }while (top != stack_buffer);
    free(stack_buffer);
}

void lldInput10(uint8_t *a, uint8_t size) {
    char c;
    uint8_t d;
    lldSet(a, 0, size);
    while (1){
        c = getchar();
        if(c < '0' || c > '9')
            break;
        lldMul32(a, a, 10, size);
        lldAdd32(a, a, c - '0', size);
    }
}

int main() {
    uint8_t a[16], b[16], c[16], d[16];
    lldInput10(a, 16);
    lldInput10(b, 16);
    lldDiv(c, a, b, d, 16);
    lldPrint10(a, 16);
    printf(" = ");
    lldPrint10(b, 16);
    printf(" * ");
    lldPrint10(c, 16);
    printf(" + ");
    lldPrint10(d, 16);
    return 0;
}
