#ifndef __MY__NDARRAY__H__
#define __MY__NDARRAY__H__
#endif

#include "COOP.h"
#include "Vector.h"

////////////////////////////////////////////////

DEF_CLASS(GenericTensor);
Vector_int shape;
MEM_SIZE_T num_elements;
MEM_SIZE_T size_bytes;
GenericVector data;
MEM_SIZE_T _element_size;
END_DEF(GenericTensor);

FUNCTIONS(GenericTensor, MEM_SIZE_T ndim, MEM_SIZE_T* shape, MEM_SIZE_T elementSize);
MEM_FUN_DECL(GenericTensor, _get_location, MEM_SIZE_T* coords, MEM_SIZE_T* ret_val);

MEM_FUN_DECL(GenericTensor, __generic_at, MEM_SIZE_T* pos, char** val_ptr);

MEM_FUN_DECL(GenericTensor, at_int, MEM_SIZE_T* pos, int** val_ptr);
MEM_FUN_DECL(GenericTensor, at_char, MEM_SIZE_T* pos, char** val_ptr);
MEM_FUN_DECL(GenericTensor, at_float, MEM_SIZE_T* pos, float** val_ptr);

MEM_FUN_DECL(GenericTensor, get_int, MEM_SIZE_T* pos, int * value);
MEM_FUN_DECL(GenericTensor, get_char, MEM_SIZE_T* pos, char * value);
MEM_FUN_DECL(GenericTensor, get_float, MEM_SIZE_T* pos, float * value);

MEM_FUN_DECL(GenericTensor, set_int, MEM_SIZE_T* pos, int value);
MEM_FUN_DECL(GenericTensor, set_char, MEM_SIZE_T* pos, char value);
MEM_FUN_DECL(GenericTensor, set_float, MEM_SIZE_T* pos, float value);

MEM_FUN_DECL(GenericTensor, reshape, MEM_SIZE_T ndim, MEM_SIZE_T* shape);

MEM_FUN_DECL(GenericTensor, zero_all);

END_FUNCTIONS(GenericTensor);

////////////////////////////////////////////////

#define DECLARE_SPECIFIC_TENSOR_TYPE(type)					\
DEF_DERIVED_CLASS(Tensor_ ##type, GenericTensor);			\
END_DEF_DERIVED(Tensor_ ##type);							\
															\
DERIVED_FUNCTIONS(Tensor_ ##type, GenericTensor, MEM_SIZE_T ndim, MEM_SIZE_T* shape);			\
MEM_FUN_DECL(Tensor_ ##type, at, MEM_SIZE_T* pos, type ** val_ptr);	\
MEM_FUN_DECL(Tensor_ ##type, get, MEM_SIZE_T* pos, type * value);	\
MEM_FUN_DECL(Tensor_ ##type, set, MEM_SIZE_T* pos, type value);	\
MEM_FUN_DECL(Tensor_ ##type, reshape, MEM_SIZE_T ndim, MEM_SIZE_T* shape);	\
MEM_FUN_DECL(Tensor_ ##type, zero_all);\
END_DERIVED_FUNCTIONS(Tensor_ ##type);

////////////////////////////////////////////////

DECLARE_SPECIFIC_TENSOR_TYPE(int);
DECLARE_SPECIFIC_TENSOR_TYPE(char);
DECLARE_SPECIFIC_TENSOR_TYPE(float);
DECLARE_SPECIFIC_TENSOR_TYPE(object);
