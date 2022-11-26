#include "vector.h"
#include <stdio.h>


DEF_CTOR(GenericVector, MEM_SIZE_T dataTypeSize)
{
	_this->size = 0;
	_this->capacity = 0;
	_this->elementSize = dataTypeSize;

	INITIALIZE_INSTANCE(Shared_ptr, _this->data) CALL;
}
END_CTOR

DEF_DTOR(GenericVector)
{
	FUN(&(_this->data), Release) CALL;
}
END_DTOR

MEM_FUN_IMPL(GenericVector, __at_generic, MEM_SIZE_T i, MEM_SIZE_T data_size, char** val_ptr);
{
	THROW_MSG_UNLESS(data_size == _this->elementSize, "Invalid Data Size");

	if (i + 1 > _this->capacity) // same as (i > size - 1), yet remember that capacity can be zero and this is an unsigned type.
	{
		THROW_MSG("Index out of range");
	}
	*val_ptr = ((char*)_this->data.px) + _this->elementSize * i;
}
END_FUN;

#define IMPL_AT_OF_TYPE(type)\
MEM_FUN_IMPL(GenericVector, at_ ##type, MEM_SIZE_T i, type* val) {\
	char * result;\
	FUN(_this, __at_generic), i, sizeof(type), & result CALL;\
	*val = *((type*)result);\
}\
END_FUN;

IMPL_AT_OF_TYPE(int);
IMPL_AT_OF_TYPE(char);
IMPL_AT_OF_TYPE(float);


MEM_FUN_IMPL(GenericVector, resize, MEM_SIZE_T new_capacity);
{
	CREATE(Shared_ptr, new_ptr) CALL;
	void* new_buff = NULL;
	NEW_ARRAY(new_buff, char, _this->elementSize* new_capacity);
	ASSERT_NOT_NULL(new_buff);
	FUN(&new_ptr, Reset), new_buff CALL;

	if (_this->size > 0) {
		memcpy(new_buff, _this->data.px, _this->elementSize * _this->capacity);
	}

	FUN(&_this->data, CopyFrom), & new_ptr CALL;

	_this->capacity = new_capacity;
}

END_FUN;
MEM_FUN_IMPL(GenericVector, __push_back_generic, char* buff, MEM_SIZE_T buff_size)
{
	THROW_MSG_UNLESS(buff_size == _this->elementSize, "Invalid Data Size");
	if (_this->size >= _this->capacity)
	{
		MEM_SIZE_T new_capacity = _this->capacity == 0 ? 1 : _this->capacity * 2;

		FUN(_this, resize), new_capacity CALL;
	}
	
	char* placeToAssign = NULL;
	FUN(_this, __at_generic), _this->size, buff_size, &placeToAssign CALL;
	ASSERT_NOT_NULL(placeToAssign);
	memcpy(placeToAssign, buff, buff_size);
	_this->size++;
}
END_FUN;

MEM_FUN_IMPL(GenericVector, zero_all);
{
	memset(_this->data.px, 0, _this->capacity);
}
END_FUN;




#define IMPL_PUSH_OF_TYPE(type)\
MEM_FUN_IMPL(GenericVector, push_back_ ##type, type val) {\
	FUN(_this, __push_back_generic), (char*)& val, sizeof(type) CALL;\
}END_FUN;

IMPL_PUSH_OF_TYPE(int);
IMPL_PUSH_OF_TYPE(char);
IMPL_PUSH_OF_TYPE(float);


MEM_FUN_IMPL(GenericVector, __pop_back_generic, char* buff, MEM_SIZE_T buff_size)
{
	THROW_MSG_UNLESS(buff_size == _this->elementSize, "Invalid Data Size");

	char* val_ptr = NULL;
	FUN(_this, __at_generic), _this->size - 1, buff_size, &val_ptr CALL;
	ASSERT_NOT_NULL(val_ptr);
	memcpy(buff, val_ptr, buff_size);
	_this->size--;
}
END_FUN

#define IMPL_POP_OF_TYPE(type)\
MEM_FUN_IMPL(GenericVector, pop_back_ ##type, type * val) {\
	FUN(_this, __pop_back_generic), (char*)val, sizeof(type) CALL;\
}END_FUN;

IMPL_POP_OF_TYPE(int);
IMPL_POP_OF_TYPE(char);
IMPL_POP_OF_TYPE(float);

INIT_CLASS(GenericVector)
BIND(GenericVector, __at_generic);
BIND(GenericVector, at_int);
BIND(GenericVector, at_char);
BIND(GenericVector, at_float);

BIND(GenericVector, resize);

BIND(GenericVector, __push_back_generic);
BIND(GenericVector, push_back_int);
BIND(GenericVector, push_back_char);
BIND(GenericVector, push_back_float);

BIND(GenericVector, __pop_back_generic);
BIND(GenericVector, pop_back_int);
BIND(GenericVector, pop_back_char);
BIND(GenericVector, pop_back_float);
END_INIT_CLASS(GenericVector)

////////////////////////////////////////////////

#define IMPL_SPECIFIC_VECTOR_TYPE(type)																				\
DEF_DERIVED_CTOR(Vector_ ##type, GenericVector) SUPER, sizeof(type) ME {} END_DERIVED_CTOR							\
DEF_DERIVED_DTOR(Vector_ ##type, GenericVector) {} END_DERIVED_DTOR													\
																													\
MEM_FUN_IMPL(Vector_ ##type, push_back, type val) { FUN_BASE(_this, push_back_ ##type), val CALL; } END_FUN;		\
MEM_FUN_IMPL(Vector_ ##type, pop_back, type * val)	{ FUN_BASE(_this, pop_back_ ##type), val CALL; } END_FUN;		\
MEM_FUN_IMPL(Vector_ ##type, at, MEM_SIZE_T i, type * val) { FUN_BASE(_this, at_ ##type), i, val CALL; } END_FUN;	\
MEM_FUN_IMPL(Vector_ ##type, resize, MEM_SIZE_T new_capacity) {FUN_BASE(_this, resize), new_capacity CALL; }END_FUN;\
MEM_FUN_IMPL(Vector_ ##type, print) {																				\
	printf("\n");																									\
	char* format = " %d"; char first_type_name_letter = * #type;													\
	if (first_type_name_letter == 'c') /*its a char type*/ format = "%c ";											\
	else if (first_type_name_letter == 'f') /*its a float type*/ format = "%f ";									\
	type val;																										\
	for (MEM_SIZE_T i = 0; i < _this->_base.size; i++) 																\
	{ 																												\
		FUN(_this, at), i, & val CALL;																				\
		printf(format, val); 																						\
	} 																												\
	printf("\n");																									\
} END_FUN;																											\
																													\
INIT_DERIVED_CLASS(Vector_ ##type, GenericVector);																	\
BIND(Vector_ ##type, push_back);																					\
BIND(Vector_ ##type, pop_back);																						\
BIND(Vector_ ##type, at);																							\
BIND(Vector_ ##type, print);																							\
END_INIT_CLASS(Vector_ ##type)																						

////////////////////////////////////////////////

IMPL_SPECIFIC_VECTOR_TYPE(int);
IMPL_SPECIFIC_VECTOR_TYPE(char);
IMPL_SPECIFIC_VECTOR_TYPE(float);

//
//DEF_DERIVED_CTOR(Vector_float, GenericVector) SUPER, sizeof(float) ME {} END_DERIVED_CTOR							
//DEF_DERIVED_DTOR(Vector_float, GenericVector) {} END_DERIVED_DTOR													
//																													
//MEM_FUN_IMPL(Vector_float, push_back, float val) { FUN_BASE(_this, push_back_float), val CALL; } END_FUN;		
//MEM_FUN_IMPL(Vector_float, pop_back, float * val)	{ FUN_BASE(_this, pop_back_float), val CALL; } END_FUN;		
//MEM_FUN_IMPL(Vector_float, at, MEM_SIZE_T i, float * val) { FUN_BASE(_this, at_float), i, val CALL; } END_FUN;	
//MEM_FUN_IMPL(Vector_float, resize, MEM_SIZE_T new_capacity) { FUN_BASE(_this, resize), new_capacity CALL; }END_FUN;
//MEM_FUN_IMPL(Vector_float, print) {
//	char* format = " %d";
//	if ('f' == 'c') /*its a char type*/ format = "%c ";
//	if ('f' == 'f') /*its a char type*/ format = "%f ";
//	float val;
//	for (int i = 0; i < _this->_base.size; i++) 
//	{ 
//		FUN(_this, at), i, & val CALL;
//		printf(format, val); 
//	} 
//} END_FUN;
//
//INIT_DERIVED_CLASS(Vector_float, GenericVector);																	
//BIND(Vector_float, push_back);																					
//BIND(Vector_float, pop_back);																						
//BIND(Vector_float, at);																							
//END_INIT_CLASS(Vector_float)																						
