
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "COOP.h"
#include "Queue.h"

static uint64_t now_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ull + (uint64_t)ts.tv_nsec;
}

FUN_IMPL(main, int argc, char** argv) {

    init_global_memory(10, HEAP_BASED_MEMORY);
    const int N = (argc > 1) ? atoi(argv[1]) : 1000000;

    CREATE(Queue_int, q) CALL;

    uint64_t t0 = now_ns();
    FOR(int i = 0; i < N; ++i) {
        MFUN(&q, enqueue), i CALL;
    } END_LOOP;
    uint64_t t1 = now_ns();

    long long sum = 0;
    ITER_FOR(int, ptr, &q) {
        sum += ptr;
    } END_ITER_FOR;
    uint64_t t2 = now_ns();

    FOR(int i = 0; i < N; ++i) {
        int out = 0;
        MFUN(&q, dequeue), & out CALL;
        (void)out;
    } END_LOOP;
    uint64_t t3 = now_ns();

    printf("Queue<int>: N=%d, enqueue=%.3f ms, iterate=%.3f ms, dequeue=%.3f ms, sum=%lld\n",
        N, (t1 - t0) / 1e6, (t2 - t1) / 1e6, (t3 - t2) / 1e6, sum);
}END_FUN;
