#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <stdio.h>

typedef struct {
    clock_t start;
    clock_t stop;
} Timer;

static Timer __static_timer__;

static inline void timerStart(Timer *timer) {
    timer->start = clock();
}

static inline void timerStop(Timer *timer) {
    timer->stop = clock();
}

static inline clock_t timerGetms(const Timer *timer) {
    return (timer->stop - timer->start) / (CLOCKS_PER_SEC / 1000);
}


#define TEST_TIME(code) \
    __static_timer__.start = clock(); \
    code; \
    __static_timer__.stop = clock(); \
    printf("Time: %ld ms\n", (__static_timer__.stop - __static_timer__.start) / (CLOCKS_PER_SEC / 1000))

#endif //TIMER_H
