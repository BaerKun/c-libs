#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>

struct pcg_state_setseq_64 {
    uint64_t state;
    uint64_t inc;
};

typedef struct pcg_state_setseq_64 pcg32_random_t;

#define PCG32_INITIALIZER   { 0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL }

static pcg32_random_t pcg32_global = PCG32_INITIALIZER;

static uint32_t pcg32_random(void) {
    uint64_t oldstate = pcg32_global.state;
    pcg32_global.state = oldstate * 6364136223846793005ULL + pcg32_global.inc;
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
};

static void pcg32_srandom(uint64_t initstate, uint64_t initseq) {
    pcg32_global.state = 0U;
    pcg32_global.inc = (initseq << 1u) | 1u;
    pcg32_random();
    pcg32_global.state += initstate;
    pcg32_random();
};

// return a <= x < b
static inline uint32_t randuint(uint32_t a, uint32_t b) {
    return a + pcg32_random() % (b - a);
}

static inline int32_t randint(int32_t a, int32_t b) {
    return (int) randuint(a, b);
}

static inline float randfloat(float a, float b) {
    return a + (b - a) * (float) pcg32_random() / 4294967296.f;
}

static inline double randdouble(double a, double b) {
    return a + (b - a) * pcg32_random() / 4294967296.0;
}

static void shuffleArray(int *array, int len) {
    unsigned i, j;
    int c;
    for (i = 2; i <= len; i++) {
        j = randuint(0, i);
        c = array[i];
        array[i] = array[j];
        array[j] = c;
    }
}

#endif //RANDOM_H
