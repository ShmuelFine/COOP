// bench_list_coop.c — COOP List microbench to mirror GLib GQueue benchmark phases.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../COOP.h"
#include "../List.h"
#include "../Iterator.h"

FUN_IMPL(main, int argc, char** argv)
{
    init_global_memory(0, HEAP_BASED_MEMORY);
    const char* phase = (argc > 1) ? argv[1] : "all";
    int  N = (argc > 2) ? (int)strtoull(argv[2], NULL, 10) : (int)1000000;

    CREATE(List_int, lst) CALL;

 

    IF(strcmp(phase, "pop_back") == 0 ||
        strcmp(phase, "pop_front") == 0)
    {
        FOR(int i = 0; i < N; ++i)
        {
            MFUN(&lst, push_back), (int)i CALL;
        }END_LOOP;

        IF(strcmp(phase, "pop_back") == 0)
        {
            FOR(int i = 0; i < N; ++i)
            {
                int out = 0;
                MFUN(&lst, pop_back), & out CALL;
                (void)out;
            }END_LOOP;
        }

        ELSE_IF(strcmp(phase, "pop_front") == 0)
        {
            FOR(int i = 0; i < N; ++i)
            {
                int out = 0;
                MFUN(&lst, pop_front), & out CALL;
                (void)out;
            }
            END_LOOP;
        }END_IF
    }

    // ---- push_back ----
    ELSE_IF(strcmp(phase, "push_back") == 0)
    {
        FOR(int i = 0; i < N; ++i)
        {
            MFUN(&lst, push_back), (const int)i CALL;
        }
        END_LOOP;
    }
    // ---- push_front ----
    ELSE_IF(strcmp(phase, "push_front") == 0)
    {
        FOR(int i = 0; i < N; ++i)
        {
            MFUN(&lst, push_front), (const int)i CALL;
        }
        END_LOOP;
    }    
    // ---- size ----
    ELSE_IF(strcmp(phase, "size") == 0)
    {
        MEM_SIZE_T size = 0;
        FOR(int i = 0; i < N; ++i)
        {
            MFUN(&lst, size), &size CALL;
        }
        END_LOOP;
    }
    // ---- empty (API size==0) ----
    ELSE_IF(strcmp(phase, "empty") == 0)
    {
        bool is_empty = 0;
        FOR(int i = 0; i < N; ++i)
        {
            MFUN(&lst, empty), &is_empty CALL;
        }
        END_LOOP;
    }
    ELSE_IF(strcmp(phase, "clear") == 0) {
        FOR(int i = 0; i < N; ++i)
        {
            MFUN(&lst, push_back), (const int)i CALL;
        }
        END_LOOP;
        MFUN(&lst, clear) CALL;
    }
    ELSE_IF(strcmp(phase, "front") == 0)
    {

        int pFront = 0;
        FOR(int i = 0; i < N; ++i)
        {
            MFUN(&lst, front), & pFront CALL;
        }
        END_LOOP;

    }
    ELSE
    {
        FOR(int i = 0; i < N / 2; ++i)
        {
            MEM_SIZE_T size = 0;
            bool is_empty = 0;
            MFUN(&lst, push_back), (int)i CALL;
            MFUN(&lst, push_front), (int)i CALL;
            MFUN(&lst, size),& size CALL;
            MFUN(&lst, empty),& is_empty CALL;
        }
        END_LOOP;

        FOR(int i = 0; i < N / 2; ++i)
        {
            int out = 0;
            MFUN(&lst, pop_back),& out CALL;
            MFUN(&lst, pop_front),& out CALL;
        }END_LOOP;

        FOR(int i = 0; i < N; ++i)
        {
            MFUN(&lst, push_back), (int)i CALL;
        }END_LOOP;
        MFUN(&lst, clear) CALL;

    }END_IF
}END_FUN

