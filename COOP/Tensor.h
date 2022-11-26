#ifndef __MY__NDARRAY__H__
#define __MY__NDARRAY__H__
#endif

#include "coop.h"
#include "Vector.h"
#include <stdbool.h>

DEF_CLASS(Tensor);
Vector_int shape;
MEM_SIZE_T num_elements;
MEM_SIZE_T size_bytes;
GenericVector data;
//MEM_SIZE_T _elementSize;
END_DEF(Tensor);

FUNCTIONS(Tensor, MEM_SIZE_T ndim, MEM_SIZE_T* shape, MEM_SIZE_T elementSize);
MEM_FUN_DECL(Tensor, _get_location, int* coords, int* ret_val);
MEM_FUN_DECL(Tensor, set, int* pos, float value);
MEM_FUN_DECL(Tensor, at, int* pos, float* ret_val);
MEM_FUN_DECL(Tensor, reshape, int ndim, int* shape);
MEM_FUN_DECL(Tensor, fill, float val);
MEM_FUN_DECL(Tensor, sum, float* ret_value);
MEM_FUN_DECL(Tensor, contains, float val, bool* ret_val);
MEM_FUN_DECL(Tensor, min, float * ret_val);
MEM_FUN_DECL(Tensor, max, float * ret_val);
END_FUNCTIONS(Tensor);