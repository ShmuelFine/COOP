#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "COOP.h"
#include "Queue.h"

// --------------------------- phases ---------------------------
FUN_IMPL(phase_enqueue, Queue_int* q, int N)
{
	FOR(int i = 0; i < N; ++i)
	{
		MFUN(q, enqueue), (int)i CALL;
	}
	END_LOOP;
}END_FUN

FUN_IMPL(phase_dequeue, Queue_int* q, int N)
{
    int out = 0;
    FOR(int i = 0; i < N; ++i)
    {
        MFUN(q, dequeue), & out CALL;
        (void)out; // avoid warnings
    }
    END_LOOP;
}END_FUN

FUN_IMPL(phase_front, Queue_int* q, int N)
{
    MFUN(q, enqueue), 123 CALL;
    int* pFront = NULL;
    FOR(int i = 0; i < N; ++i)
    {
        MFUN(q, front), & pFront  CALL;
    }
    END_LOOP;
}END_FUN

FUN_IMPL(phase_front_cref, Queue_int* q, int N)
{
    MFUN(q, enqueue), 123 CALL;
    const int* pcFront = NULL;
    FOR(int i = 0; i < N; ++i)
    {
       MFUN(q, front_cref), & pcFront CALL;
    }
    END_LOOP
}END_FUN

FUN_IMPL(phase_size, Queue_int* q, int N)
{
    int size = 0;
    FOR(int i = 0;i < N; ++i)
    {
        MFUN(q, size), & size CALL;
    }
    END_LOOP;
}END_FUN

FUN_IMPL(phase_empty, Queue_int* q, int N)
{
    bool is_empty = false;

    FOR(int i = 0;i < N; ++i)
    {
        MFUN(q, empty), & is_empty CALL;
    }
    END_LOOP;
}END_FUN


FUN_IMPL(phase_all, Queue_int* q, int N)
{
    bool is_empty = false;
    int size = 0;
    const int* pcFront = NULL;
    int* pFront = NULL;
    //check all enqueue, front, size, empty
    FOR(int i = 0; i < N; ++i)
    {
        MFUN(q, enqueue), (int)i CALL;
        MFUN(q, front), & pFront CALL;
        MFUN(q, front_cref), & pcFront CALL;
        MFUN(q, size), & size CALL;
        MFUN(q, empty), & is_empty CALL;
    }
    END_LOOP;

    // Dequeue all
    int out = 0;
    FOR(int i = 0; i < N; ++i)
    {
        MFUN(q, dequeue), & out CALL;
    }
    END_LOOP;
    
}END_FUN

// --------------------------- main ---------------------------

FUN_IMPL(main, int argc, char** argv)
{
    init_global_memory(0, HEAP_BASED_MEMORY);

    const char* phase = (argc > 1) ? argv[1] : "all";
    size_t N = (argc > 2) ? (size_t)strtoull(argv[2], NULL, 10) : (size_t)1000000;

    CREATE(Queue_int, q) CALL;

    IF(strcmp(phase, "enqueue") == 0)
        phase_enqueue(&q, N);
    ELSE_IF(strcmp(phase, "dequeue") == 0) {
        phase_enqueue(&q, N);
        phase_dequeue(&q, N);
    }
    ELSE_IF(strcmp(phase, "front") == 0)
        phase_front(&q, N);
    ELSE_IF(strcmp(phase, "front_cref") == 0)
        phase_front_cref(&q, N);
    ELSE_IF(strcmp(phase, "size") == 0)
        phase_size(&q, N);
    ELSE_IF(strcmp(phase, "empty") == 0)
        phase_empty(&q, N);
    ELSE
        phase_all(&q, N);
    END_IF;

}
END_FUN
