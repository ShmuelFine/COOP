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
void* container_ptr;
END_DEF(Iterator);


FUNCTIONS(Iterator, IteratorCategory category, void* container_ptr);

MEM_FUN_DECL(Iterator, equals, Iterator* other, bool* out_equal);
MEM_FUN_DECL(Iterator, next);
MEM_FUN_DECL(Iterator, prev);
MEM_FUN_DECL(Iterator, get_ref, void** out_ptr);
MEM_FUN_DECL(Iterator, get_cref, const void** out_ptr);
MEM_FUN_DECL(Iterator, distance, Iterator* other, ptrdiff_t* out_dist);
MEM_FUN_DECL(Iterator, advance, ptrdiff_t n);

END_FUNCTIONS(Iterator);


#define ITER_EQUALS(IT_A, IT_B, OUT_BOOL)         MFUN((IT_A), equals), (IT_B), (OUT_BOOL) CALL
#define ITER_NEXT(IT)                             MFUN((IT), next) CALL
#define ITER_PREV(IT)                             MFUN((IT), prev) CALL
#define ITER_GET_REF(IT, OUT_VOIDPTR)             MFUN((IT), get_ref), (void**)(OUT_VOIDPTR) CALL
#define ITER_GET_CREF(IT, OUT_CVOIDPTR)           MFUN((IT), get_cref), (const void**)(OUT_CVOIDPTR) CALL
#define ITER_DISTANCE(IT_A, IT_B, OUT_DIST)       MFUN((IT_A), distance), (IT_B), (OUT_DIST) CALL
#define ITER_ADVANCE(IT, N)                       MFUN((IT), advance), (N) CALL
#define ITER_CATEGORY(IT)   ((IT)->_category)

#define ITER_CONTINUE do { goto __ITER_CONTINUE_LABEL__;} while(0)

#define ITER_FOR(ElemType, varName, objPtr)          \
{                                                    \
       bool __eq = 0;                                \
       Iterator *_it=NULL;                           \
       Iterator *_end=NULL;                          \
       MFUN(objPtr,begin),&_it CALL;                 \
       MFUN(objPtr, end), &_end CALL;                \
       FOR (;!(__eq)&&!(IS_BREAKING);) {             \
         ITER_EQUALS(_it,_end,&__eq);                \
         if (__eq||IS_BREAKING){break;}              \
         const ElemType *_p_##varName = NULL;        \
         ITER_GET_CREF(_it,&_p_##varName);           \
         ElemType varName = *_p_##varName;           \

#define END_ITER_FOR                 \
            __ITER_CONTINUE_LABEL__: \
            ITER_NEXT(_it);          \
        END_LOOP                     \
        DESTROY(_it);                \
	    DESTROY(_end);               \
      }                              \
}



#endif 