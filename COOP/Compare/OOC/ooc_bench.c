#define _POSIX_C_SOURCE 200809L
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Queue_ooc.h"   

static inline uint64_t now_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ull + (uint64_t)ts.tv_nsec;
}

int main(int argc, char** argv) {
    const int N = (argc > 1) ? atoi(argv[1]) : 1000000;

    Queue_int q;
    Queue_int_ctor(&q);

    uint64_t t0 = now_ns();
    for (int i = 0; i < N; ++i) {
        Queue_int_enqueue(&q, i);
    }
    uint64_t t1 = now_ns();

    long long sum = 0;
    {
        List L = queue_get_list((Queue)q.impl);
        for (ListIterator it = list_first(L); it; it = list_next(L, it)) {
            int* pi = (int*)list_get_item(it);
            if (pi) sum += *pi;
        }
    }
    uint64_t t2 = now_ns();

    for (int i = 0; i < N; ++i) {
        int out = 0;
        Queue_int_dequeue(&q, &out);
        (void)out;
    }
    uint64_t t3 = now_ns();

    Queue_int_dtor(&q);

    printf("OOC Queue<int>: N=%d, enqueue=%.3f ms, iterate=%.3f ms, dequeue=%.3f ms, sum=%lld\n",
        N,
        (double)(t1 - t0) / 1e6,
        (double)(t2 - t1) / 1e6,
        (double)(t3 - t2) / 1e6,
        sum);
    return 0;
}
