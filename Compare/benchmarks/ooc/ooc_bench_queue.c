// bench_queue_ooc.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "Queue_ooc.h"


static void phase_enqueue(Queue_int* q, size_t N) {
    for (size_t i = 0; i < N; ++i) {
         (void)Queue_int_enqueue(q, (int)i);
    }
}

static void phase_dequeue(Queue_int* q, size_t N) {
    for (size_t i = 0; i < N; ++i) {
        int out = 0;
        (void)Queue_int_dequeue(q, &out);    
    }
}

static void phase_front(Queue_int* q, size_t N) {
    (void)Queue_int_enqueue(q, 123);
    int* pf = NULL;
    for (size_t i = 0; i < N; ++i) {
        Queue_int_front(q, &pf);
       
    }
}

static void phase_front_cref( Queue_int* q, size_t N) {
   
    (void)Queue_int_enqueue(q, 123);
    const int* pcref = NULL;
    for (size_t i = 0; i < N; ++i) {
        Queue_int_front_cref(q, &pcref);
     }
}

static void phase_size(Queue_int* q, size_t N) {
    for (size_t i = 0; i < N; ++i) {
        (void)Queue_int_size(q);
    }
}

static void phase_empty(Queue_int* q, size_t N) {
    for (size_t i = 0; i < N; ++i) {
        (void)Queue_int_empty(q);
    }
}


static void phase_all(Queue_int* q, size_t N) {
    
    const int* pc = NULL;
    int* p = NULL;
    for (size_t i = 0; i < N; ++i) {
        (void)Queue_int_enqueue(q, (int)i);
        Queue_int_front(q, &p);
        Queue_int_front_cref(q, &pc);
        (void)Queue_int_size(q);
        (void)Queue_int_empty(q);
       
    }
    //dequeue all
    for (size_t i = 0; i < N; ++i) {
        int tmp;
        (void)Queue_int_dequeue(q, &tmp);
    }
}

int main(int argc, char** argv)
{
    const char* phase = (argc > 1) ? argv[1] : "all";
    size_t N = (argc > 2) ? (size_t)strtoull(argv[2], NULL, 10) : (size_t)1000000;

    Queue_int q;
    Queue_int_ctor(&q);

    if (strcmp(phase, "enqueue") == 0) phase_enqueue(&q, N);
    else if (strcmp(phase, "dequeue") == 0) 
    {
        phase_enqueue(&q, N);
        phase_dequeue(&q, N);
    }
    else if (strcmp(phase, "front") == 0) phase_front(&q, N);
    else if (strcmp(phase, "front_cref") == 0) phase_front_cref(&q, N);
    else if (strcmp(phase, "size") == 0) phase_size(&q, N);
    else if (strcmp(phase, "empty") == 0) phase_empty(&q, N);
    else                                  phase_all(&q, N);

    Queue_int_dtor(&q);
    return 0;
}
