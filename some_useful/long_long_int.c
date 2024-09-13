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
    uint8_t *aptr = a + size, *bptr = b + size;

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
    const uint8_t *inarr1 = input1, *inarr2 = input2;

    for (int j, i = 0; i != size; ++i) {
        for (j = size - i - 1; j >= 0; --j) {
            temp[i + j] += (uint32_t) inarr1[i] * inarr2[j];
        }
    }

    uint8_t *outptr = output, *end = outptr + size;
    uint32_t *this = temp, *next;
    do {
        *outptr = *this;
        next = this + 1;
        *next += *this >> 8;
        this = next;
    } while (++outptr != end);

    free(temp);
}

void lldMul32(uint8_t *output, uint8_t *input1, uint32_t input2, uint32_t size) {
    uint8_t *outptr = output;
    uint8_t *inptr = input1, *end = input1 + size;
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
        *quot_ptr = divd_tmp / divisor;
        divd_tmp = divd_tmp % divisor;
    }

    if (remainder != NULL)
        *remainder = divd_tmp;
}

void lldDiv(uint8_t *quotient, uint8_t *dividend, uint8_t *divisor, uint8_t *remainder, uint32_t size) {
    uint8_t *divisor_high = lldHigh(divisor, size);
    uint32_t dividend_size = lldHigh(dividend, size) - dividend + 1;
    uint32_t divisor_size = divisor_high - divisor + 1;

    if (divisor_size == 1) {
        if(remainder != NULL)
            lldSet(remainder, 0, size);
        lldSet(quotient + divisor_size, 0, size - divisor_size);
        lldDiv8(quotient, dividend, *divisor, remainder, dividend_size);
        return;
    }

    lldSet(quotient, 0, size);
    if (dividend_size < divisor_size)
        return;

    uint16_t dividend_16 = 0;
    uint16_t divisor_16 = *divisor_high << 8 | *(divisor_high - 1);
    uint32_t divisor_size_plus = divisor_size + 1;

    uint8_t *remainder_ = remainder == NULL ? malloc(size) : remainder;
    uint8_t *remainder_high = remainder_ + dividend_size - 1;
    uint8_t *remainder_low;

    uint8_t divisor_complement[divisor_size_plus];
    uint8_t subtracted_buffer[divisor_size_plus];

    lldMov(remainder_, dividend, size);
    lldComplement(divisor_complement, divisor, divisor_size_plus);

    for (uint8_t *ptr = quotient + dividend_size - divisor_size; ptr >= quotient; --ptr, --remainder_high) {
        dividend_16 = dividend_16 << 8 | *remainder_high;
        if (dividend_16 < *divisor_high)
            continue;

        *ptr = ((uint32_t)dividend_16 << 8 | *(remainder_high - 1)) / divisor_16;
        if (*ptr == 0)
            continue;

        remainder_low = remainder_high - divisor_size + 1;
        lldMul32(subtracted_buffer, divisor, *ptr, divisor_size_plus);

        if (lldComp(subtracted_buffer, remainder_low, divisor_size_plus) == 1) {
            lldAdd(subtracted_buffer, subtracted_buffer, divisor_complement, divisor_size_plus);
            --*ptr;
        }

        lldComplement(subtracted_buffer, subtracted_buffer, divisor_size_plus);
        lldAdd(remainder_low, remainder_low, subtracted_buffer, divisor_size_plus);
        dividend_16 = *remainder_high;
    }

    if (remainder == NULL)
        free(remainder_);
}

void lldPrint(uint8_t *a, uint32_t size) {
    printf("0x");
    uint8_t *ptr = lldHigh(a, size);
    do {
        printf("%02x", *ptr);
    } while (ptr-- != a);
}

int main() {
    uint8_t a[8] = {0x19, 0xef, 0xab, 0x0f, 0x40, 0x04, 0x10, 0x01};
    uint8_t b[8] = {0xab, 0x1, 0, 0, 0, 0, 0, 0};
    uint8_t c[8];
    uint8_t d[8];
    lldPrint(a, 8);
    printf("\n");
    lldPrint(b, 8);
    printf("\n");
    lldDiv(c, a, b, d, 8);
    lldPrint(c, 8);
    printf("\n");
    lldPrint(d, 8);
    printf("\n");
    lldMul(c, c, b, 8);
    lldAdd(c, c, d, 8);
    lldPrint(c, 8);
    return 0;
}
