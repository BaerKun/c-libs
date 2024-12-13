#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <stdio.h>

#ifdef __unix
typedef struct {
    struct timespec start;
    struct timespec stop;
} Timer;

static Timer __static_timer__;

static inline void timerStart(Timer *timer) {
    clock_gettime(CLOCK_MONOTONIC, &timer->start);
}

static inline void timerStop(Timer *timer) {
    clock_gettime(CLOCK_MONOTONIC, &timer->stop);
}

static inline unsigned long long timerGetns(const Timer *timer) {
    return (timer->stop.tv_sec - timer->start.tv_sec) * 1000000000llu + (timer->stop.tv_nsec - timer->start.tv_nsec);
}
#endif

#define TEST_TIME(code) \
    timerStart(&__static_timer__); \
    code; \
    timerStop(&__static_timer__); \
    printf("time: %llu ns\n", timerGetns(&__static_timer__))

#endif //TIMER_H
