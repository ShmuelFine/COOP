#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../COOP.h"
#include "../Queue.h"



FUN_IMPL(main, int argc, char** argv)
{
    init_global_memory(0, HEAP_BASED_MEMORY);

    const char* phase = (argc > 1) ? argv[1] : "all";
    int  N = (argc > 2) ? (int)strtoull(argv[2], NULL, 10) : (int)1000000;

    // Create queue of ints
    CREATE(Queue_int, q) CALL;

    // --------------------------- enqueue ---------------------------
    IF(strcmp(phase, "enqueue") == 0)
    {
        FOR(int i = 0; i < N; ++i)
        {
            MFUN(&q, enqueue), (int)i CALL;
        }
        END_LOOP;
    }
    // --------------------------- dequeue ---------------------------
    ELSE_IF(strcmp(phase, "dequeue") == 0)
    {
        // prefill
        FOR(int i = 0; i < N; ++i)
        {
            MFUN(&q, enqueue), (int)i CALL;
        }
        END_LOOP;

        // then dequeue
        int out = 0;
        FOR(int i = 0; i < N; ++i)
        {
            MFUN(&q, dequeue), & out CALL;
            (void)out; // avoid warnings
        }
        END_LOOP;
    }
    // --------------------------- front -----------------------------
    ELSE_IF(strcmp(phase, "front") == 0)
    {
        int* pFront = NULL;
        FOR(int i = 0; i < N; ++i)
        {
            MFUN(&q, front),&pFront  CALL;
        }
        END_LOOP;     
    }
    // --------------------------- front_cref ------------------------
    ELSE_IF(strcmp(phase, "front_cref") == 0)
    {
        const int* pcFront = NULL;
        FOR(int i = 0; i < N; ++i)
        {
            MFUN(&q, front_cref), & pcFront CALL;
        }
        END_LOOP
    }
    // --------------------------- size ------------------------------
    ELSE_IF(strcmp(phase, "size") == 0)
    {
        int size = 0;
		FOR(int i = 0;i < N; ++i)
		{
			MFUN(&q, size), &size CALL;
		}
		END_LOOP;
    }
    // --------------------------- empty -----------------------------
    ELSE_IF(strcmp(phase, "empty") == 0)
    {
        bool is_empty = false;

        FOR(int i = 0;i < N; ++i)
        {
            MFUN(&q, empty), &is_empty CALL;
        }
        END_LOOP;
    }
        // --------------------------- clear -----------------------------
    ELSE_IF(strcmp(phase, "clear") == 0)
    {
        // prefill
        FOR(int i = 0; i < N; ++i)
        {
            MFUN(&q, enqueue), (int)i CALL;
        }
        END_LOOP;

        MFUN(&q, clear) CALL;
	}
        // --------------------------- all (default) ---------------------
    ELSE
    {
        bool is_empty = false;
        int size = 0;
        const int* pcFront = NULL;
        int* pFront = NULL;
		//check all enqueue, front, size, empty
        FOR(int i = 0; i < N; ++i)
        {
            MFUN(&q, enqueue), (int)i CALL;
            MFUN(&q, front), & pFront CALL;
            MFUN(&q, front_cref), & pcFront CALL;
            MFUN(&q, size), &size CALL;
            MFUN(&q, empty), & is_empty CALL;
        }
        END_LOOP;


        // Dequeue all
        int out = 0;
        FOR(int i = 0; i < N; ++i)
        {
            MFUN(&q, dequeue),& out CALL;
        }
        END_LOOP;

        // Clear 
        MFUN(&q, clear) CALL;
    }END_IF;

}
END_FUN
