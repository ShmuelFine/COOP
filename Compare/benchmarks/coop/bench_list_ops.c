// bench_list_coop.c — COOP List microbench to mirror GLib GQueue benchmark phases.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "COOP.h"
#include "List.h"


// push_back
FUN_IMPL(push_back, List_int* lst, int N)
{
    FOR(int i = 0; i < N; ++i)
    {
        MFUN(lst, push_back), (int)i CALL;
    }
    END_LOOP;
}END_FUN

// push_front
FUN_IMPL(push_front, List_int* lst, int N)
{
    FOR(int i = 0; i < N; ++i)
    {
        MFUN(lst, push_front), (int)i CALL;
    }
    END_LOOP;
}END_FUN

// pop_back
FUN_IMPL(pop_back, List_int* lst, int N)
{
    int out = 0;
    FOR(int i = 0; i < N; ++i)
    {
        MFUN(lst, pop_back), & out CALL;
        (void)out; // avoid warnings

    }
    END_LOOP;
}END_FUN

// pop_front 
FUN_IMPL(pop_front, List_int* lst, int N)
{
    int out = 0;
    FOR(int i = 0; i < N; ++i)
    {
        MFUN(lst, pop_front), & out CALL;
        (void)out; // avoid warnings
    }
    END_LOOP;
}END_FUN


// front ??
FUN_IMPL(front, List_int* lst, int N)
{
    MFUN(lst, push_back), 123 CALL;

    int out_val = 0;
    FOR(int i = 0; i < N; ++i)
    {
        MFUN(lst, front), & out_val CALL;
    }
    END_LOOP;
}END_FUN

// back??
FUN_IMPL(back, List_int* lst, int N)
{

    int out_val = 0;
    FOR(int i = 0; i < N; ++i)
    {
        MFUN(lst, back), & out_val CALL;
    }
    END_LOOP;
}END_FUN


// size
FUN_IMPL(size, List_int* lst, int N)
{
    int size = 0;
    FOR(int i = 0; i < N; ++i)
    {
        MFUN(lst, size), & size CALL;
    }
    END_LOOP;
}END_FUN

// empty
FUN_IMPL(empty, List_int* lst, int N)
{
    bool is_empty = false;
    FOR(int i = 0; i < N; ++i)
    {
        MFUN(lst, empty), & is_empty CALL;
    }
    END_LOOP;
}END_FUN

// clear 
FUN_IMPL(clear, List_int* lst, int N)
{
    MFUN(lst, clear) CALL;
}END_FUN

// all
FUN_IMPL(all, List_int* lst, int N)
{
    bool is_empty = false;
    int size = 0;
    int out_val = 0;

    FOR(int i = 0; i < N / 2; ++i)
    {
        MFUN(lst, push_back), (int)i CALL;
        MFUN(lst, push_front), (int)i CALL;
        MFUN(lst, front), & out_val CALL;
        MFUN(lst, back), & out_val CALL;
        MFUN(lst, size), & size CALL;
        MFUN(lst, empty), & is_empty CALL;
    }
    END_LOOP;

    int out = 0;
    FOR(int i = 0; i < N / 2; ++i)
    {
        MFUN(lst, pop_back), & out CALL;
        MFUN(lst, pop_front), & out CALL;
    }END_LOOP;


    FOR(int i = 0; i < N; ++i)
    {
        MFUN(lst, push_back), (int)i CALL;
    }END_LOOP;
    MFUN(lst, clear) CALL;
}END_FUN


// --------------------------- main ---------------------------

FUN_IMPL(main, int argc, char** argv)
{
    init_global_memory(0, HEAP_BASED_MEMORY);

    const char* phase = (argc > 1) ? argv[1] : "all";
    size_t N_size_t = (argc > 2) ? (size_t)strtoull(argv[2], NULL, 10) : (size_t)1000000;
    int N = (int)N_size_t;

    CREATE(List_int, lst) CALL;

    IF(strcmp(phase, "push_back") == 0)
        push_back(&lst, N);
    ELSE_IF(strcmp(phase, "push_front") == 0)
        push_front(&lst, N);
    ELSE_IF(strcmp(phase, "pop_back") == 0)
        pop_back(&lst, N);
    ELSE_IF(strcmp(phase, "pop_front") == 0)
        pop_front(&lst, N);
    ELSE_IF(strcmp(phase, "front") == 0)
        front(&lst, N);
    ELSE_IF(strcmp(phase, "back") == 0)
        back(&lst, N);
    ELSE_IF(strcmp(phase, "size") == 0)
        size(&lst, N);
    ELSE_IF(strcmp(phase, "empty") == 0)
        empty(&lst, N);
    ELSE_IF(strcmp(phase, "clear") == 0)
        clear(&lst, N);
    ELSE
        all(&lst, N);
    END_IF;

}END_FUN