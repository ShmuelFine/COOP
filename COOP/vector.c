#include "Vector.h"
#include "MathUtils.h"
#include <stdio.h>


/* =========================================================
 *                 VectorIter (derived from Iterator)
 * ========================================================= */

DEF_DERIVED_CTOR(VectorIter, Iterator)SUPER, (ITER_RANDOM_ACCESS)ME
{
	_this->index = 0;
	_this->vec = NULL;
}
END_DERIVED_CTOR

DEF_DERIVED_DTOR(VectorIter, Iterator)
{
	
}
END_DERIVED_DTOR

FUN_OVERRIDE_IMPL(VectorIter, Iterator, equals, object* other, bool* out_equal)
{

	*out_equal = 0;
	IF(other) {
		VectorIter* o = (VectorIter*)other;
		*out_equal = (o->vec == _this->vec) && (o->index == _this->index);

	}END_IF
}
END_FUN;

/* ++it */
FUN_OVERRIDE_IMPL(VectorIter, Iterator, next)
{
	THROW_MSG_UNLESS(_this->vec != NULL, "Iterator not bound");
	MEM_SIZE_T n = 0;
	MFUN((GenericVector*)_this->vec, size), & n CALL;
	THROW_MSG_UNLESS(_this->index <= n, "Advance past end");
	_this->index++;
	
}
END_FUN;

/* --it */
FUN_OVERRIDE_IMPL(VectorIter, Iterator, prev)
{
	THROW_MSG_UNLESS(_this->vec != NULL, "Iterator not bound");
	THROW_MSG_UNLESS(_this->index > 0, "Advance before begin");
	_this->index--;
}
END_FUN;

FUN_OVERRIDE_IMPL(VectorIter, Iterator, get_ref, void** out_ptr)
{
	THROW_MSG_UNLESS(_this->vec != NULL, "Iterator not bound");
	char* p = NULL;
	GenericVector* vec = (GenericVector*)_this->vec;
	MFUN(vec, __at_generic), _this->index,vec->elementSize, & p CALL;
	*out_ptr = (void*)p;
}
END_FUN;

/* read-only pointer */
FUN_OVERRIDE_IMPL(VectorIter, Iterator, get_cref, const void** out_ptr)
{
	THROW_MSG_UNLESS(_this->vec != NULL, "Iterator not bound");
	char* p = NULL;
	GenericVector* vec = (GenericVector*)_this->vec;
	MFUN(vec, __at_generic), _this->index, vec->elementSize, & p CALL;
	*out_ptr = (const void*)p;
}
END_FUN;

FUN_OVERRIDE_IMPL(VectorIter, Iterator, distance, object* other, ptrdiff_t* out_dist)
{
	THROW_MSG_UNLESS(other != NULL, "Null other iterator");
	VectorIter* o = (VectorIter*)other;
	THROW_MSG_UNLESS(o->vec == _this->vec, "Iterators of different vectors");
	*out_dist = (ptrdiff_t)o->index - (ptrdiff_t)_this->index;
}
END_FUN;


FUN_OVERRIDE_IMPL(VectorIter, Iterator, advance, ptrdiff_t n)
{
	THROW_MSG_UNLESS(_this->vec != NULL, "Iterator not bound");

	MEM_SIZE_T sz = 0;
	MFUN((GenericVector*)_this->vec, size), & sz CALL;

	ptrdiff_t target = (ptrdiff_t)_this->index + n;
	THROW_MSG_UNLESS(target >= 0 && target <= (ptrdiff_t)sz, "Iterator advance out of range");

	_this->index = (MEM_SIZE_T)target;
}
END_FUN;
FUN_OVERRIDE_IMPL(VectorIter, Iterator, reset_begin)
{
	THROW_MSG_UNLESS(_this->vec != NULL, "Iterator not bound");
	_this->index = 0;
}END_FUN;

INIT_DERIVED_CLASS(VectorIter, Iterator);
BIND_OVERIDE(VectorIter, Iterator, equals);
BIND_OVERIDE(VectorIter, Iterator, next);
BIND_OVERIDE(VectorIter, Iterator, prev);
BIND_OVERIDE(VectorIter, Iterator, get_ref);
BIND_OVERIDE(VectorIter, Iterator, get_cref);
BIND_OVERIDE(VectorIter, Iterator, distance);
BIND_OVERIDE(VectorIter, Iterator, advance);
BIND_OVERIDE(VectorIter, Iterator, reset_begin);
END_INIT_CLASS(VectorIter)



////////////////////////////////////////////////

DEF_CTOR(GenericVector, MEM_SIZE_T dataTypeSize)
{
	_this->size = 0;
	_this->capacity = 0;
	_this->elementSize = dataTypeSize;
	_this->data = NULL;

	CREATE(VectorIter, begin)CALL;
	CREATE(VectorIter, end) CALL;
	_this->begin_iter = begin;
	_this->end_iter = end;

	_this->begin_iter.vec = _this;
	_this->begin_iter.index = 0;

	_this->end_iter.vec = _this;
	_this->end_iter.index = _this->size;	
	
}
END_CTOR

DEF_DTOR(GenericVector)
{
	FREE(_this->data);

}
END_DTOR

MEM_FUN_IMPL(GenericVector, dataPtr, char** out_ptr)
{
	*out_ptr = _this->data;
}
END_FUN;

MEM_FUN_IMPL(GenericVector, __at_generic, MEM_SIZE_T i, MEM_SIZE_T data_size, char** val_ptr)
{
	THROW_MSG_UNLESS(data_size == _this->elementSize, "Invalid Data Size");
	IF (i+1  > _this->capacity) // same as (i > size - 1), yet remember that capacity can be zero and this is an unsigned type.
	{
		THROW_MSG("Index out of range");
	}END_IF
	*val_ptr = _this->data + _this->elementSize * i;
}
END_FUN;



#define IMPL_AT_OF_TYPE(type)\
MEM_FUN_IMPL(GenericVector, at_ ##type, MEM_SIZE_T i, type ** val_ptr) {\
	MFUN(_this, __at_generic), i, sizeof(type), ((char**)val_ptr) CALL;\
}\
END_FUN;

IMPL_AT_OF_TYPE(int);
IMPL_AT_OF_TYPE(char);
IMPL_AT_OF_TYPE(float);
IMPL_AT_OF_TYPE(objSPtr);


#define IMPL_SET_OF_POD(type) \
MEM_FUN_IMPL(GenericVector, set_##type, MEM_SIZE_T i, type val) { \
    type* slot = NULL; \
    MFUN(_this, __at_generic), i, sizeof(type), (char**)&slot CALL; \
    ASSERT_NOT_NULL(slot); \
    *slot = val; \
} END_FUN;

IMPL_SET_OF_POD(int)
IMPL_SET_OF_POD(char)
IMPL_SET_OF_POD(float)

MEM_FUN_IMPL(GenericVector, set_objSPtr, MEM_SIZE_T i, objSPtr val)
{
	objSPtr* slot = NULL;
	MFUN(_this, __at_generic), i, sizeof(objSPtr), (char**)&slot CALL;
	ASSERT_NOT_NULL(slot);
	/* release old pointer in the slot */
	DESTROY(slot);
	/* store new pointer (assuming plain assign is OK in your objSPtr semantics) */
	*slot = val;
}
END_FUN;


#define IMPL_GET_OF_TYPE(type)\
MEM_FUN_IMPL(GenericVector, get_ ##type, MEM_SIZE_T i, type * val) {\
	type * val_ptr = NULL;\
	MFUN(_this, __at_generic), i, sizeof(type), ((char**)&val_ptr) CALL;\
	ASSERT_NOT_NULL(val_ptr)\
	*val = *val_ptr;\
}\
END_FUN;

IMPL_GET_OF_TYPE(int);
IMPL_GET_OF_TYPE(char);
IMPL_GET_OF_TYPE(float);
IMPL_GET_OF_TYPE(objSPtr);

MEM_FUN_IMPL(GenericVector, resize, MEM_SIZE_T new_capacity)
{
	char* new_data = NULL;
	ALLOC_ARRAY(new_data, char, _this->elementSize * new_capacity);

	IF (_this->size > 0) {
		memcpy(new_data, _this->data, _this->elementSize * MIN(new_capacity, _this->capacity));
	}END_IF
	FREE(_this->data);
	_this->data = new_data;
	_this->capacity = new_capacity;
	_this->size = MIN(_this->size, _this->capacity);
	_this->end_iter.index = _this->size;
}
END_FUN;

MEM_FUN_IMPL(GenericVector, size, MEM_SIZE_T* out_size)
{
	*out_size = _this->size;
}
END_FUN;

MEM_FUN_IMPL(GenericVector, __push_back_generic, char* buff, MEM_SIZE_T buff_size)
{
	THROW_MSG_UNLESS(buff_size == _this->elementSize, "Invalid Data Size");
	IF (_this->size >= _this->capacity)
	{
		MEM_SIZE_T new_capacity = _this->capacity == 0 ? 1 : _this->capacity * 2;

		MFUN(_this, resize), new_capacity CALL;
	}END_IF

	char* placeToAssign = NULL;
	MFUN(_this, __at_generic), _this->size, buff_size, & placeToAssign CALL;
	ASSERT_NOT_NULL(placeToAssign);
	memcpy(placeToAssign, buff, buff_size);

	_this->size++;
	_this->end_iter.index = _this->size;
}
END_FUN;

MEM_FUN_IMPL(GenericVector, zero_all)
{
	if (_this->size > 0) {
		if (_this->elementSize == sizeof(objSPtr)) {
			for (MEM_SIZE_T i = 0; i < _this->size; ++i) {
				objSPtr* slot = (objSPtr*)(_this->data + i * _this->elementSize);
				DESTROY(slot);
			}
		}
		memset(_this->data, 0, _this->elementSize * _this->size);
	}
}
END_FUN;

MEM_FUN_IMPL(GenericVector, __pop_back_generic, char* buff, MEM_SIZE_T buff_size)
{
	THROW_MSG_UNLESS(buff_size == _this->elementSize, "Invalid Data Size");
	THROW_MSG_UNLESS(_this->size > 0, "Pop from empty vector");

	MEM_SIZE_T last = _this->size - 1;
	char* src = _this->data + (last * _this->elementSize);
	ASSERT_NOT_NULL(src);

	memcpy(buff, src, buff_size);
	memset(src, 0, buff_size); /* sanitize */
	_this->size = last;
	_this->end_iter.index = _this->size;
}
END_FUN;

#define IMPL_PUSH_OF_TYPE(type)\
MEM_FUN_IMPL(GenericVector, push_back_ ##type, type val) {\
	MFUN(_this, __push_back_generic), (char*)& val, sizeof(type) CALL;\
}END_FUN;

IMPL_PUSH_OF_TYPE(int);
IMPL_PUSH_OF_TYPE(char);
IMPL_PUSH_OF_TYPE(float);
IMPL_PUSH_OF_TYPE(objSPtr);



#define IMPL_POP_OF_TYPE(type)\
MEM_FUN_IMPL(GenericVector, pop_back_ ##type, type * val) {\
	MFUN(_this, __pop_back_generic), (char*)val, sizeof(type) CALL;\
}END_FUN;


IMPL_POP_OF_TYPE(int);
IMPL_POP_OF_TYPE(char);
IMPL_POP_OF_TYPE(float);
IMPL_POP_OF_TYPE(objSPtr);


INIT_CLASS(GenericVector)
BIND(GenericVector, dataPtr);

BIND(GenericVector, __at_generic);
BIND(GenericVector, at_int);
BIND(GenericVector, at_char);
BIND(GenericVector, at_float);
BIND(GenericVector, at_objSPtr);

BIND(GenericVector, get_int);
BIND(GenericVector, get_char);
BIND(GenericVector, get_float);
BIND(GenericVector, get_objSPtr);

BIND(GenericVector, set_int);
BIND(GenericVector, set_char);
BIND(GenericVector, set_float);
BIND(GenericVector, set_objSPtr);

BIND(GenericVector, resize);
BIND(GenericVector, size);

BIND(GenericVector, __push_back_generic);
BIND(GenericVector, push_back_int);
BIND(GenericVector, push_back_char);
BIND(GenericVector, push_back_float);
BIND(GenericVector, push_back_objSPtr);

BIND(GenericVector, __pop_back_generic);
BIND(GenericVector, pop_back_int);
BIND(GenericVector, pop_back_char);
BIND(GenericVector, pop_back_float);
BIND(GenericVector, pop_back_objSPtr);

BIND(GenericVector, zero_all);


END_INIT_CLASS(GenericVector)



#define IMPL_SPECIFIC_VECTOR_TYPE_xTORs(type)																		\
DEF_DERIVED_CTOR(Vector_ ##type, GenericVector) SUPER, sizeof(type) ME {} END_DERIVED_CTOR							\
DEF_DERIVED_DTOR(Vector_ ##type, GenericVector) {} END_DERIVED_DTOR													

#define IMPL_SPECIFIC_VECTOR_TYPE_FUNCITONS(type)																	\
MEM_FUN_IMPL(Vector_ ##type, dataPtr, type ** out_ptr) { FUN_BASE(_this, dataPtr), (char**) out_ptr CALL; } END_FUN;\
MEM_FUN_IMPL(Vector_ ##type, push_back, type val) { FUN_BASE(_this, push_back_ ##type), val CALL; } END_FUN;		\
MEM_FUN_IMPL(Vector_ ##type, pop_back, type * val)	{ FUN_BASE(_this, pop_back_ ##type), val CALL; } END_FUN;		\
MEM_FUN_IMPL(Vector_ ##type, at, MEM_SIZE_T i, type ** val_ptr) { FUN_BASE(_this, at_ ##type), i, val_ptr CALL; } END_FUN;	\
MEM_FUN_IMPL(Vector_ ##type, get, MEM_SIZE_T i, type * val) { FUN_BASE(_this, get_ ##type), i, val CALL; } END_FUN;	\
MEM_FUN_IMPL(Vector_ ##type, set, MEM_SIZE_T i, type val) { FUN_BASE(_this, set_ ##type), i, val CALL; } END_FUN;	\
MEM_FUN_IMPL(Vector_ ##type, resize, MEM_SIZE_T new_capacity) {FUN_BASE(_this, resize), new_capacity CALL; }END_FUN;\
MEM_FUN_IMPL(Vector_ ##type, size, MEM_SIZE_T * out_size) {FUN_BASE(_this, size), out_size CALL; }END_FUN;			\
MEM_FUN_IMPL(Vector_ ##type, zero_all) {FUN_BASE(_this, zero_all) CALL; }END_FUN;									\
MEM_FUN_IMPL(Vector_ ##type, print) {																				\
	printf("\n");																									\
	char* format = " %d"; char first_type_name_letter = * #type;													\
	IF (first_type_name_letter == 'c') /*its a char type*/ format = "%c ";											\
	ELSE_IF (first_type_name_letter == 'f') /*its a float type*/ format = "%f ";									\
	ELSE_IF (first_type_name_letter == 'o') /*its a float type*/ format = "%p ";									\
	END_IF                                                                                                          \
    type val;																										\
	/*FOR (MEM_SIZE_T i = 0; i < _this->_base.size; i++) 																\
	{ 																												\
		MFUN(_this, get), i, & val CALL;																			\
		printf(format, val); 																						\
	}END_LOOP; 																										*/\
	ITER_FOR(type, val, _this) {                                                                                     \
		printf(format, val);                                                                                         \
	} END_ITER_FOR                                                                                                  \
	printf("\n");																									\
} END_FUN																											\
																													\
INIT_DERIVED_CLASS(Vector_ ##type, GenericVector);																	\
BIND(Vector_ ##type, dataPtr);																						\
BIND(Vector_ ##type, push_back);																					\
BIND(Vector_ ##type, pop_back);																						\
BIND(Vector_ ##type, at);																							\
BIND(Vector_ ##type, get);																							\
BIND(Vector_ ##type, set);																							\
BIND(Vector_ ##type, resize);																						\
BIND(Vector_ ##type, size);																							\
BIND(Vector_ ##type, zero_all);																						\
BIND(Vector_ ##type, print);																							\
END_INIT_CLASS(Vector_ ##type)																						

////////////////////////////////////////////////

IMPL_SPECIFIC_VECTOR_TYPE_xTORs(int);
IMPL_SPECIFIC_VECTOR_TYPE_FUNCITONS(int);

IMPL_SPECIFIC_VECTOR_TYPE_xTORs(char);
IMPL_SPECIFIC_VECTOR_TYPE_FUNCITONS(char);

IMPL_SPECIFIC_VECTOR_TYPE_xTORs(float);
IMPL_SPECIFIC_VECTOR_TYPE_FUNCITONS(float);

// obj vector is different:
DEF_DERIVED_CTOR(Vector_objSPtr, GenericVector) SUPER, sizeof(objSPtr) ME{} END_DERIVED_CTOR
DEF_DERIVED_DTOR(Vector_objSPtr, GenericVector) {

	MEM_SIZE_T numElements = 0;
	MFUN(_this, size), & numElements CALL;
	FOR(int i = 0; i < numElements; i++) {
		objSPtr curr;
		MFUN(_this, get), i, & curr CALL;
		DESTROY(&curr);
	}END_LOOP;
} END_DERIVED_DTOR
IMPL_SPECIFIC_VECTOR_TYPE_FUNCITONS(objSPtr);

