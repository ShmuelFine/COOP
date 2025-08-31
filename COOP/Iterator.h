#ifndef __COOP__ITERATOR__H_
#define __COOP__ITERATOR__H_

#include "COOP.h"


typedef enum IteratorCategory_e {
    ITER_INPUT = 1,     //input
    ITER_OUTPUT,        //output
    ITER_FORWARD,       //read/write
    ITER_BIDIRECTIONAL, //both forward and backward
    ITER_RANDOM_ACCESS  //full pointer
} IteratorCategory;

/* ===== Base Class: Iterator ===== */
DEF_CLASS(Iterator);
IteratorCategory category;  
END_DEF(Iterator);


FUNCTIONS(Iterator);

MEM_FUN_DECL(Iterator, equals, object* other, bool* out_equal);
MEM_FUN_DECL(Iterator, next);
MEM_FUN_DECL(Iterator, prev);
MEM_FUN_DECL(Iterator, get_ref, void** out_ptr);
MEM_FUN_DECL(Iterator, get_cref, const void** out_ptr);
MEM_FUN_DECL(Iterator, distance, object* other, ptrdiff_t* out_dist);
MEM_FUN_DECL(Iterator, advance, ptrdiff_t n);

END_FUNCTIONS(Iterator);


#define ITER_EQUALS(IT_A, IT_B, OUT_BOOL)         MFUN((IT_A), equals), (object*)(IT_B), (OUT_BOOL) CALL
#define ITER_NEXT(IT)                             MFUN((IT), next) CALL
#define ITER_PREV(IT)                             MFUN((IT), prev) CALL
#define ITER_GET_REF(IT, OUT_VOIDPTR)             MFUN((IT), get_ref), (void**)(OUT_VOIDPTR) CALL
#define ITER_GET_CREF(IT, OUT_CVOIDPTR)           MFUN((IT), get_cref), (const void**)(OUT_CVOIDPTR) CALL
#define ITER_DISTANCE(IT_A, IT_B, OUT_DIST)       MFUN((IT_A), distance), (object*)(IT_B), (OUT_DIST) CALL
#define ITER_ADVANCE(IT, N)                       MFUN((IT), advance), (N) CALL


#define ITER_CONTINUE  do { goto __ITER_CONTINUE__; } while (0)
#define ITER_BREAK     do { goto __ITER_BREAK__; } while (0)

#define ITER_FOR(T, VAR, BEGIN_IT, END_IT)                           \
    do {                                                             \
        Iterator* __it_cur__ = (BEGIN_IT);                           \
        Iterator* __it_end__ = (END_IT);                             \
        for (;;) {                                                   \
            bool __eq__ = false;                                     \
            ITER_EQUALS(__it_cur__, __it_end__, &__eq__);            \
            if (__eq__) break;                                       \
            T* VAR = NULL;                                           \
            ITER_GET_REF(__it_cur__, (void**)&(VAR));                \
            do /* body starts */

#define END_ITER_FOR(BEGIN_IT)                                       \
            while (0);                                               \
            __ITER_CONTINUE__:                                       \
            __ITER_SILENCE_MSVC_UNUSED_LABELS__();                   \
            ITER_NEXT((BEGIN_IT));                                   \
            continue;                                                \
            __ITER_BREAK__:                                          \
            __ITER_SILENCE_MSVC_UNUSED_LABELS__();                   \
            break;                                                   \
        }                                                            \
    } while (0)


#endif
