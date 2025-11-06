#ifndef __COOP__ITERATOR__H_
#define __COOP__ITERATOR__H_

#include "COOP.h"
#include <stddef.h>

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
void* container_ptr; /* Pointer to the container (Vector, Queue, List) this iterator belongs to */
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
#define ITER_GET_CREF(IT, OUT_CVOIDPTR)           MFUN((IT), get_cref), (const void**)(OUT_CVOIDPTR) CALL


#define ITER_FOR(ElemType, varName, objPtr)          \
{                                                    \
       bool __eq = 0;                                \
       Iterator *_it=NULL;                           \
       Iterator *_end=NULL;                          \
       MFUN((objPtr),begin),&_it CALL;               \
       MFUN((objPtr), end), &_end CALL;              \
       FOR (;!(__eq);) {                             \
         ITER_EQUALS(_it,_end,&__eq);                \
         IF (__eq){BREAK;} END_IF                    \
         const ElemType *_p_##varName = NULL;        \
         ITER_GET_CREF(_it,&_p_##varName);           \
         ElemType varName = *_p_##varName;           \

#define END_ITER_FOR                 \
        ITER_NEXT(_it);              \
        END_LOOP                     \
        IF(IS_CONTINUING) {ITER_NEXT(_it);} END_IF  \
      }                              \
        DESTROY(_it);                \
	    DESTROY(_end);               \
}


#endif 