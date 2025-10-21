// bench_queue_ooc.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "Queue_ooc.h"

// sinks to prevent DCE
static volatile long long sink_ll;
static volatile int       sink_i;
static volatile uintptr_t sink_p;

/* sum via iteration over the underlying list (no prints) */
static long long queue_sum(const Queue_int* q) {
    long long acc = 0;
    List L = queue_get_list((Queue)q->impl);
    for (ListIterator it = list_first(L); it; it = list_next(L, it)) {
        int* pi = (int*)list_get_item(it);
        if (pi) acc += *pi;
    }
    return acc;
}

static void phase_enqueue(Queue_int* q, size_t N) {
    for (size_t i = 0; i < N; ++i) {
        sink_i ^= Queue_int_enqueue(q, (int)i);
    }
}

static void phase_dequeue(Queue_int* q, size_t N) {
    // prepare
    for (size_t i = 0; i < N; ++i) (void)Queue_int_enqueue(q, (int)i);
    // dequeue
    for (size_t i = 0; i < N; ++i) {
        int out = 0;
        int ok = Queue_int_dequeue(q, &out);
        sink_i ^= ok ^ out;
    }
}

static void phase_front(Queue_int* q, size_t N) {
    // prepare
    for (size_t i = 0; i < N; ++i) (void)Queue_int_enqueue(q, (int)i);
    // probe many times
    for (size_t i = 0; i < N; ++i) {
        int* pf = NULL;
        Queue_int_front(q, &pf);
        if (pf) sink_i ^= *pf;
        // advance occasionally to exercise front on different heads
        if ((i & 15) == 0) {
            int tmp;
            (void)Queue_int_dequeue(q, &tmp);
        }
    }
}

static void phase_front_cref(const Queue_int* qc, size_t N) {
    // Note: need non-const to fill; then read via cref
    Queue_int* q = (Queue_int*)qc;
    for (size_t i = 0; i < N; ++i) (void)Queue_int_enqueue(q, (int)i);

    for (size_t i = 0; i < N; ++i) {
        const int* pcref = NULL;
        Queue_int_front_cref(qc, &pcref);
        if (pcref) sink_i ^= *pcref;
        if ((i & 31) == 0) { int tmp; (void)Queue_int_dequeue(q, &tmp); }
    }
}

static void phase_size(Queue_int* q, size_t N) {
    for (size_t i = 0; i < N; ++i) (void)Queue_int_enqueue(q, (int)i);
    sink_ll = (long long)Queue_int_size(q);
}

static void phase_empty(Queue_int* q, size_t N) {
    // empty at start
    sink_i ^= (Queue_int_empty(q) ? 1 : 0);
    // fill
    for (size_t i = 0; i < N; ++i) (void)Queue_int_enqueue(q, (int)i);
    sink_i ^= (Queue_int_empty(q) ? 1 : 0);
}

static void phase_clear(Queue_int* q, size_t N) {
    for (size_t i = 0; i < N; ++i) (void)Queue_int_enqueue(q, (int)i);
    Queue_int_clear(q);
    sink_ll = (long long)Queue_int_size(q); // should be 0
}

static void phase_iter_sum(Queue_int* q, size_t N) {
    for (size_t i = 0; i < N; ++i) (void)Queue_int_enqueue(q, (int)i);
    sink_ll = queue_sum(q);
}

static void phase_begin_end(Queue_int* q, size_t N) {
    // This phase demonstrates the internal iterators via queue_get_list
    for (size_t i = 0; i < N; ++i) (void)Queue_int_enqueue(q, (int)i);
    List L = queue_get_list((Queue)q->impl);
    for (ListIterator it = list_first(L); it; it = list_next(L, it)) {
        sink_p ^= (uintptr_t)list_get_item(it);
    }
}

static void phase_all(Queue_int* q, size_t N) {
    // 1) enqueue
    for (size_t i = 0; i < N; ++i) (void)Queue_int_enqueue(q, (int)i);
    // 2) iterate sum
    sink_ll = queue_sum(q);
    // 3) front checks
    const int* pc = NULL; Queue_int_front_cref(q, &pc); if (pc) sink_i ^= *pc;
    int* p = NULL; Queue_int_front(q, &p); if (p) sink_i ^= *p;
    // 4) size & empty
    sink_ll ^= (long long)Queue_int_size(q);
    sink_i ^= (Queue_int_empty(q) ? 1 : 0);
    // 5) dequeue all
    for (size_t i = 0; i < N; ++i) { int tmp; (void)Queue_int_dequeue(q, &tmp); sink_i ^= tmp; }
    // 6) clear (no-op now)
    Queue_int_clear(q);
}

int main(int argc, char** argv)
{
    const char* phase = (argc > 1) ? argv[1] : "all";
    size_t      N = (argc > 2) ? (size_t)strtoull(argv[2], NULL, 10) : (size_t)100000;

    Queue_int q;
    Queue_int_ctor(&q);

    if (strcmp(phase, "enqueue") == 0) phase_enqueue(&q, N);
    else if (strcmp(phase, "dequeue") == 0) phase_dequeue(&q, N);
    else if (strcmp(phase, "front") == 0) phase_front(&q, N);
    else if (strcmp(phase, "front_cref") == 0) phase_front_cref(&q, N);
    else if (strcmp(phase, "size") == 0) phase_size(&q, N);
    else if (strcmp(phase, "empty") == 0) phase_empty(&q, N);
    else if (strcmp(phase, "clear") == 0) phase_clear(&q, N);
    else if (strcmp(phase, "iter_sum") == 0) phase_iter_sum(&q, N);
    else if (strcmp(phase, "begin_end") == 0) phase_begin_end(&q, N);
    else                                      phase_all(&q, N);

    Queue_int_dtor(&q);
    return 0;
}
