#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "COOP.h"
#include "List.h"

static uint64_t now_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ull + (uint64_t)ts.tv_nsec;
}

int main(int argc, char** argv) {
    const int N = (argc > 1) ? atoi(argv[1]) : 1000000;

    CREATE(List_int, lst) CALL;

    uint64_t t0 = now_ns();
    FOR(int i = 0; i < N; ++i) {
        MFUN(&lst, push_back), i CALL;
    } END_LOOP;
    uint64_t t1 = now_ns();

    long long sum = 0;
    ITER_FOR(void*, ptr, &lst) {
        sum += *(int*)ptr;
    } END_ITER_FOR;
    uint64_t t2 = now_ns();

    FOR(int i = 0; i < N; ++i) {
        int out = 0;
        MFUN(&lst, pop_front), & out CALL;
        (void)out;
    } END_LOOP;
    uint64_t t3 = now_ns();

    printf("N=%d, push=%.3f ms, iterate=%.3f ms, pop=%.3f ms, sum=%lld\n",
        N,
        (t1 - t0) / 1e6, (t2 - t1) / 1e6, (t3 - t2) / 1e6, sum);
    return 0;
}
