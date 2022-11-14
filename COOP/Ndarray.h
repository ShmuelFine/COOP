#ifndef __MY__NDARRAY__H__
#define __MY__NDARRAY__H__
#endif

#include "coop.h"
#include <stdbool.h>

DEF_CLASS(Ndarray);
int* shape;
int ndim;
int size;
float* data;
//float min;
//float max;
END_DEF(Ndarray);

FUNCTIONS(Ndarray, int ndim, int* shape);
MEM_FUN_DECL(Ndarray, get_location, int* coords, int* ret_val);
MEM_FUN_DECL(Ndarray, set, int* pos, float value);
MEM_FUN_DECL(Ndarray, at, int* pos, float* ret_val);
MEM_FUN_DECL(Ndarray, reshape, int ndim, int* shape);
MEM_FUN_DECL(Ndarray, fill, float val);
MEM_FUN_DECL(Ndarray, sum, float* ret_value);
MEM_FUN_DECL(Ndarray, contains, float val, bool* ret_val);
MEM_FUN_DECL(Ndarray, min, float * ret_val);
MEM_FUN_DECL(Ndarray, max, float * ret_val);
END_FUNCTIONS(Ndarray);

//min, max, _
// min, max, sum, fill, contains
//avarage, sort, product, clip, round, arrange, index, count, append,...

//throw, memory, memset