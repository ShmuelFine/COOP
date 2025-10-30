#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "COOP.h"
#include "BinaryTree.h"

/* -------- COOP functions (not static C) -------- */

FUN_IMPL(phase_insert, BTree_int* bt, MEM_SIZE_T N)
{
    FOR(MEM_SIZE_T i = 0; i < N; ++i) {
        MFUN(bt, insert), (int)i CALL;
    } END_LOOP;
}
END_FUN

FUN_IMPL(phase_remove, BTree_int* bt, MEM_SIZE_T N)
{
    /* prepare */
    FOR(MEM_SIZE_T i = 0; i < N; ++i) {
        MFUN(bt, insert), (int)i CALL;
    } END_LOOP;

    /* remove */
    FOR(MEM_SIZE_T i = 0; i < N; ++i) {
        bool removed = false;
        MFUN(bt, remove), (int)i, & removed CALL;
    } END_LOOP;
}
END_FUN

FUN_IMPL(phase_size, BTree_int* bt, MEM_SIZE_T N)
{
    MEM_SIZE_T s = 0;
    FOR(MEM_SIZE_T i = 0; i < N; ++i) {
        MFUN(bt, get_size), & s CALL;
    } END_LOOP;
}
END_FUN

FUN_IMPL(phase_empty, BTree_int* bt, MEM_SIZE_T N)
{
    bool is_empty = false;
    FOR(MEM_SIZE_T i = 0; i < N; ++i) {
        MFUN(bt, is_empty), & is_empty CALL;
    } END_LOOP;
}
END_FUN

FUN_IMPL(phase_all, BTree_int* bt, MEM_SIZE_T N)
{
    MEM_SIZE_T s = 0;
    bool is_empty = false;

    FOR(MEM_SIZE_T i = 0; i < N; ++i) {
        MFUN(bt, insert), (int)i CALL;
        MFUN(bt, get_size), & s CALL;
        MFUN(bt, is_empty), & is_empty CALL;
    } END_LOOP;

    FOR(MEM_SIZE_T i = 0; i < N; ++i) {
        bool removed = false;
        MFUN(bt, remove), (int)i, & removed CALL;
    } END_LOOP;
}
END_FUN

/* ------------------ main ------------------ */
FUN_IMPL(main, int argc, char** argv)
{
    FUN(init_global_memory) 0, HEAP_BASED_MEMORY CALL;

    const char* phase = (argc > 1) ? argv[1] : "all";
    MEM_SIZE_T  N = (argc > 2) ? (MEM_SIZE_T)strtoull(argv[2], NULL, 10) : (MEM_SIZE_T)10000;

    CREATE(BTree_int, bt) CALL;

    if (strcmp(phase, "insert") == 0)
    {
        FUN(phase_insert)& bt, N CALL;
    }
    else if (strcmp(phase, "remove") == 0)
    {
        FUN(phase_remove)& bt, N CALL;
    }
    else if (strcmp(phase, "size") == 0)
    {
        FUN(phase_size)& bt, N CALL;
    }
    else if (strcmp(phase, "empty") == 0)
    {
        FUN(phase_empty)& bt, N CALL;
    }
    else
    {
        FUN(phase_all)& bt, N CALL;
    }

    DESTROY(&bt);
    RETURN(0);
}
END_FUN
