#ifndef __COOP__ITERATOR__H_
#define __COOP__ITERATOR__H_

#include "COOP.h"
typedef enum IteratorCategory {
    ITER_INPUT = 1,
    ITER_OUTPUT,
    ITER_FORWARD,
    ITER_BIDIRECTIONAL,
    ITER_RANDOM_ACCESS
} IteratorCategory;

/* ===== Base Class: Iterator ===== */
DEF_CLASS(Iterator);
IteratorCategory _category;
END_DEF(Iterator);


FUNCTIONS(Iterator, IteratorCategory category);

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
#define ITER_CATEGORY(IT)   ((IT)->_category)

#define ITER_CONTINUE do { __iter_skip_next = true; MFUN(_it, next) CALL; goto __ITER_CONTINUE_LABEL__; } while(0)


#define ITER_FOR(ElemType, varName, objPtr)                              \
    bool __eq = 0;                      \
    bool __iter_skip_next = false;                                       \
    for (Iterator* _it = (Iterator*)&((objPtr)->_base.begin_iter),          \
                  *_end = (Iterator*)&((objPtr)->_base.end_iter);           \
         !(__eq)&&!(IS_BREAKING); )                                                              \
        for (;;) {                                                          \
            MFUN(_it, equals), (object*)_end, &__eq CALL;                   \
            if (__eq) break;                                                \
            const ElemType *_p_##varName = NULL;                            \
            MFUN(_it, get_cref), &_p_##varName CALL;                        \
            ElemType varName = *_p_##varName;                               \
            SCOPE_START;

#define END_ITER_FOR \
        __ITER_CONTINUE_LABEL__: \
        _scope_obj_list_call_dtors(&_scope_obj_list);                \
        if (!IS_BREAKING && !IS_IN_RETURNING() && !IS_IN_THROWING() && !__iter_skip_next) \
            MFUN(_it, next) CALL;                                    \
        __iter_skip_next = false;                                    \
    }                                                                \
    if (IS_BREAKING) { break; IS_BREAKING=false; }                 \
    else if (IS_IN_RETURNING() || IS_IN_THROWING()) break;           \
}



#endif
