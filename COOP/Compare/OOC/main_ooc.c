// =============================
// File: main_ooc.c
// =============================
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include "Queue_ooc.h"

int main(void) {
    printf(">>> start\n");

    Queue_int q;
    Queue_int_ctor(&q);
    printf("ctor ok\n");

    for (int i = 0; i < 5; ++i) {
        Queue_int_enqueue(&q, i + 10);
    }
    printf("enqueued 5\n");

    int* pFront = NULL; const int* pcFront = NULL;
    Queue_int_front(&q, &pFront);
    printf("front ptr=%p val=%d\n", (void*)pFront, pFront ? *pFront : -999);

    Queue_int_front_cref(&q, &pcFront);
    printf("front_cref ptr=%p val=%d\n", (void*)pcFront, pcFront ? *pcFront : -999);

    int tmp = -1;
    Queue_int_dequeue(&q, &tmp); printf("deq1=%d\n", tmp);
    Queue_int_dequeue(&q, &tmp); printf("deq2=%d\n", tmp);

    size_t n = Queue_int_size(&q); printf("size=%zu (expect 3)\n", n);
    bool e = Queue_int_empty(&q);  printf("empty=%d (expect 0)\n", (int)e);

    Queue_int_print(&q);

    Queue_int_clear(&q); printf("cleared. empty=%d\n", (int)Queue_int_empty(&q));
    Queue_int_dtor(&q);  printf("dtor ok\n");

    printf(">>> end\n");
    return 0;
}
