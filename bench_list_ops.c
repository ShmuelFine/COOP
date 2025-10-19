// bench_list_coop.c — COOP List microbench to mirror GLib GQueue benchmark phases.
//
// Usage:
//   ./bench_list_coop <phase> [N]
// Phases: push_back | push_front | pop_back | pop_front | size | empty | iterate | all
//
// Notes:
// - Keeps pre-fill outside of measured phase for pop_* and iterate (same as GLib version).
// - Uses COOP macros exactly as in the project (CREATE/MFUN/ITER_FOR).
// - Uses volatile sinks to prevent dead-code elimination on accumulations/results.

#define _POSIX_C_SOURCE 199309L
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "COOP.h"
#include "List.h"

static volatile long long sink_ll; // prevents DCE for integer accumulations
static volatile int        sink_i; // prevents DCE for boolean accumulations

static uint64_t now_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ull + (uint64_t)ts.tv_nsec;
}

static int parse_n(char** argv, int idx, int def) {
    return (argv[idx] ? atoi(argv[idx]) : def);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <phase> [N]\n", argv[0]);
        fprintf(stderr, "phases: push_back | push_front | pop_back | pop_front | size | empty | iterate | all\n");
        return 2;
    }

    const char* phase = argv[1];
    const int   N = (argc > 2) ? atoi(argv[2]) : 1000000;

    // List under test (COOP typed wrapper).
    CREATE(List_int, lst) CALL;

    // Pre-fill for phases that consume or traverse.
    if (!strcmp(phase, "pop_back") ||
        !strcmp(phase, "pop_front") ||
        !strcmp(phase, "iterate")) {
        FOR(int i = 0; i < N; ++i) {
            MFUN(&lst, push_back), i CALL;
        } END_LOOP;
    }

    // Optional wall-time stamps (not printed per-phase; left here if you want to log).
    (void)now_ns;

    // ---- PHASE: push_back ----------------------------------------------------
    if (!strcmp(phase, "push_back")) {
        FOR(int i = 0; i < N; ++i) {
            MFUN(&lst, push_back), i CALL;
        } END_LOOP;
    }
    // ---- PHASE: push_front ---------------------------------------------------
    else if (!strcmp(phase, "push_front")) {
        FOR(int i = 0; i < N; ++i) {
            MFUN(&lst, push_front), i CALL;
        } END_LOOP;
    }
    // ---- PHASE: pop_back -----------------------------------------------------
    else if (!strcmp(phase, "pop_back")) {
        FOR(int i = 0; i < N; ++i) {
            int out = 0;
            MFUN(&lst, pop_back), & out CALL;
            (void)out;
        } END_LOOP;
    }
    // ---- PHASE: pop_front ----------------------------------------------------
    else if (!strcmp(phase, "pop_front")) {
        FOR(int i = 0; i < N; ++i) {
            int out = 0;
            MFUN(&lst, pop_front), & out CALL;
            (void)out;
        } END_LOOP;
    }
    // ---- PHASE: size ---------------------------------------------------------
    else if (!strcmp(phase, "size")) {
        long long acc = 0;
        FOR(int i = 0; i < N; ++i) {
            MEM_SIZE_T s = 0;
            MFUN(&lst, size), & s CALL;
            acc += (long long)s;
        } END_LOOP;
        sink_ll = acc;
    }
    // ---- PHASE: empty --------------------------------------------------------
    // If there is no explicit "empty" method in COOP List, this phase reduces to (size==0).
    else if (!strcmp(phase, "empty")) {
        int acc = 0;
        FOR(int i = 0; i < N; ++i) {
            MEM_SIZE_T s = 0;
            MFUN(&lst, size), & s CALL;
            acc ^= (s == 0); // boolean parity over all checks
        } END_LOOP;
        sink_i = acc;
    }
    // ---- PHASE: iterate ------------------------------------------------------
    else if (!strcmp(phase, "iterate")) {
        long long sum = 0;
        ITER_FOR(void*, ptr, &lst) {
            sum += *(int*)ptr;
        } END_ITER_FOR;
        sink_ll = sum;
    }
    // ---- PHASE: all ----------------------------------------------------------
    else {
        // push N
        FOR(int i = 0; i < N; ++i) {
            MFUN(&lst, push_back), i CALL;
        } END_LOOP;

        // iterate
        long long sum = 0;
        ITER_FOR(void*, ptr, &lst) {
            sum += *(int*)ptr;
        } END_ITER_FOR;

        // pop N (from head, matching GLib "all" which dequeues from front)
        FOR(int i = 0; i < N; ++i) {
            int out = 0;
            MFUN(&lst, pop_front), & out CALL;
            (void)out;
        } END_LOOP;

        sink_ll = sum;
    }

    // Cleanup; mirrors g_queue_free() in the GLib version.
    DESTROY(&lst);

    return 0;
}
