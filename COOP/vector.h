#ifndef __COOP__VECTOR__H_
#define __COOP__VECTOR__H_

#include "COOP.h"
#include "SharedObjPtr.h"
#include "Iterator.h"
#include <stdbool.h>
#include <stddef.h> 


DEF_DERIVED_CLASS(VectorIter, Iterator);
MEM_SIZE_T index;
END_DEF_DERIVED(VectorIter);

DERIVED_FUNCTIONS(VectorIter, Iterator, void* container_ptr);
FUN_OVERRIDE(Iterator, equals, Iterator* other, bool* out_equal);
FUN_OVERRIDE(Iterator, next);
FUN_OVERRIDE(Iterator, prev);
FUN_OVERRIDE(Iterator, get_ref, void** out_ptr);
FUN_OVERRIDE(Iterator, get_cref, const void** out_ptr);
FUN_OVERRIDE(Iterator, distance, Iterator* other, ptrdiff_t* out_dist);
FUN_OVERRIDE(Iterator, advance, ptrdiff_t n);
END_DERIVED_FUNCTIONS(VectorIter);

//////////////////////////////////////////////////////


DEF_CLASS(GenericVector);
MEM_SIZE_T size;
MEM_SIZE_T capacity;
MEM_SIZE_T elementSize;
char* data;
END_DEF(GenericVector);

FUNCTIONS(GenericVector, MEM_SIZE_T dataTypeSize);

MEM_FUN_DECL(GenericVector, dataPtr, char** out_ptr);

MEM_FUN_DECL(GenericVector, __at_generic, MEM_SIZE_T i, MEM_SIZE_T data_size, char** val_ptr);
MEM_FUN_DECL(GenericVector, at_int, MEM_SIZE_T i, int** val_ptr);
MEM_FUN_DECL(GenericVector, at_char, MEM_SIZE_T i, char** val_ptr);
MEM_FUN_DECL(GenericVector, at_float, MEM_SIZE_T i, float** val_ptr);
MEM_FUN_DECL(GenericVector, at_objSPtr, MEM_SIZE_T i, objSPtr** val_ptr);

MEM_FUN_DECL(GenericVector, get_int, MEM_SIZE_T i, int* val);
MEM_FUN_DECL(GenericVector, get_char, MEM_SIZE_T i, char* val);
MEM_FUN_DECL(GenericVector, get_float, MEM_SIZE_T i, float* val);
MEM_FUN_DECL(GenericVector, get_objSPtr, MEM_SIZE_T i, objSPtr* val_ptr);

MEM_FUN_DECL(GenericVector, set_int, MEM_SIZE_T i, int val);
MEM_FUN_DECL(GenericVector, set_char, MEM_SIZE_T i, char val);
MEM_FUN_DECL(GenericVector, set_float, MEM_SIZE_T i, float val);
MEM_FUN_DECL(GenericVector, set_objSPtr, MEM_SIZE_T i, objSPtr val);

MEM_FUN_DECL(GenericVector, resize, MEM_SIZE_T new_capacity);
MEM_FUN_DECL(GenericVector, size, MEM_SIZE_T* out_size);

MEM_FUN_DECL(GenericVector, __push_back_generic, char* buff, MEM_SIZE_T buff_size);

MEM_FUN_DECL(GenericVector, push_back_int, int val);
MEM_FUN_DECL(GenericVector, push_back_char, char val);
MEM_FUN_DECL(GenericVector, push_back_float, float val);
MEM_FUN_DECL(GenericVector, push_back_objSPtr, objSPtr val);

MEM_FUN_DECL(GenericVector, __pop_back_generic, char* buff, MEM_SIZE_T buff_size);
MEM_FUN_DECL(GenericVector, pop_back_int, int* val);
MEM_FUN_DECL(GenericVector, pop_back_char, char* val);
MEM_FUN_DECL(GenericVector, pop_back_float, float* val);
MEM_FUN_DECL(GenericVector, pop_back_objSPtr, objSPtr* val);

MEM_FUN_DECL(GenericVector, zero_all);
MEM_FUN_DECL(GenericVector, begin, Iterator** out_it);
MEM_FUN_DECL(GenericVector, end, Iterator** out_it);

END_FUNCTIONS(GenericVector);

//////////////////////////////////////////////////////



#define DECLARE_SPECIFIC_VECTOR_TYPE(type) \
DEF_DERIVED_CLASS(Vector_ ##type, GenericVector); \
END_DEF_DERIVED(Vector_ ##type); \
DERIVED_FUNCTIONS(Vector_ ##type, GenericVector); \
MEM_FUN_DECL(Vector_ ##type, dataPtr, type ** out_ptr); \
MEM_FUN_DECL(Vector_ ##type, push_back, type val); \
MEM_FUN_DECL(Vector_ ##type, pop_back, type * val); \
MEM_FUN_DECL(Vector_ ##type, at, MEM_SIZE_T i, type** val_ptr); \
MEM_FUN_DECL(Vector_ ##type, get, MEM_SIZE_T i, type* val); \
MEM_FUN_DECL(Vector_ ##type, set, MEM_SIZE_T i, type val); \
MEM_FUN_DECL(Vector_ ##type, resize, MEM_SIZE_T new_capacity); \
MEM_FUN_DECL(Vector_ ##type, size, MEM_SIZE_T * out_size); \
MEM_FUN_DECL(Vector_ ##type, zero_all); \
MEM_FUN_DECL(Vector_ ##type, print); \
MEM_FUN_DECL(Vector_ ##type, begin, VectorIter* out_it); \
MEM_FUN_DECL(Vector_ ##type, end, VectorIter* out_it); \
END_DERIVED_FUNCTIONS(Vector_ ##type);

DECLARE_SPECIFIC_VECTOR_TYPE(int);
DECLARE_SPECIFIC_VECTOR_TYPE(char);
DECLARE_SPECIFIC_VECTOR_TYPE(float);
DECLARE_SPECIFIC_VECTOR_TYPE(objSPtr);

#endif
