#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

// size of data need be divisible by 8

typedef struct {
    uint32_t size;
    uint8_t data[0];
} LongLongInt;

LongLongInt *lldNew(const uint32_t size) {
    LongLongInt *a = malloc(sizeof(LongLongInt) + size);
    a->size = size;
    return a;
}

void lldMov(LongLongInt *a, const LongLongInt *b) {
    memcpy(a->data, b->data, a->size);
}

void lldAdd(LongLongInt *output, const LongLongInt *input1, const LongLongInt *input2) {
    uint64_t buffer, carry = 0;
    uint32_t *outptr = output->data;
    const uint32_t *inptr1 = input1->data, *inptr2 = input2->data, *end = (uint32_t *) (output->data + output->size);

    do {
        buffer = (uint64_t) *inptr1 + (uint64_t) *inptr2 + carry;
        carry = buffer >> 32;
        *outptr = buffer;
        ++inptr1;
        ++inptr2;
    } while (++outptr != end);
}

void lldComplement(LongLongInt *out, const LongLongInt *in) {
    const uint64_t *inptr = in->data, *end = (uint64_t *) (in->data + in->size);
    uint64_t *outptr = out->data;

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

void lldSub(LongLongInt *output, const LongLongInt *minuend, const LongLongInt *subtracted) {
    LongLongInt *complement = malloc(sizeof(LongLongInt) + subtracted->size);
    lldComplement(complement, subtracted);
    lldAdd(output, minuend, complement);
    free(complement);
}

void lldMul(LongLongInt *output, const LongLongInt *input1, const LongLongInt *input2) {
    uint64_t *temp = calloc(input1->size, 8);
    const uint16_t *inarr1 = input1->data, *inarr2 = input2->data, size2 = input1->size / 2;

    for (int j, i = 0; i != size2; ++i) {
        for (j = size2 - i - 1; j >= 0; --j) {
            temp[i + j] += (uint32_t) inarr1[i] * inarr2[j];
        }
    }

    uint16_t *outptr = output->data, *end = outptr + size2;
    uint64_t *this = temp, *next;
    do {
        *outptr = *this;
        next = this + 1;
        *next += *this >> 16;
        this = next;
    } while (++outptr != end);

    free(temp);
}

void lldPrint(const LongLongInt *a) {
    printf("0x");
    for (int i = a->size - 1; i >= 0; --i) {
        if (a->data[i] != 0)
            printf("%x", a->data[i]);
    }
}

int main() {
    LongLongInt *a = lldNew(32), *b = lldNew(32);
    a->data[0] = 0xf3;
    a->data[1] = 0xff;
    b->data[0] = 0x2f;
    b->data[1] = 0x8f;
    lldPrint(a);
    printf("\n");
    lldPrint(b);
    printf("\n");
    lldMul(a, a, b);
    lldPrint(a);
    printf("\n");
    lldSub(a, a, b);
    lldPrint(a);
    free(a);
    free(b);
    return 0;
}
