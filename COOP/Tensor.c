#include "Tensor.h"


DEF_CTOR(GenericTensor, MEM_SIZE_T ndim, MEM_SIZE_T* shape, MEM_SIZE_T elementSize);
{
	THROW_MSG_UNLESS(ndim > 0, "dimension must be > 0");
	_this->_element_size = elementSize;

	INITIALIZE_INSTANCE(Vector_int, _this->shape) CALL;
	MFUN(&_this->shape, resize), ndim CALL;
	_this->num_elements = 1;
	for (MEM_SIZE_T i = 0; i < ndim; i++) {
		THROW_MSG_UNLESS(shape[i] <= 0, "shape dims must be > 0");
		MFUN(&_this->shape, push_back), shape[i] CALL;
		_this->num_elements *= shape[i];
	}
	_this->size_bytes = _this->num_elements * elementSize;

	INITIALIZE_INSTANCE(GenericVector, _this->data), elementSize CALL;
	MFUN(&_this->data, resize), _this->size_bytes CALL;
	MFUN(&_this->data, zero_all) CALL;
}
END_CTOR

DEF_DTOR(GenericTensor)
{
	DESTROY(&(_this->shape));
	DESTROY(&(_this->data));
}
END_DTOR;

MEM_FUN_IMPL(GenericTensor, _get_location, MEM_SIZE_T* coords, MEM_SIZE_T* ret_val)
{
	MEM_SIZE_T innerOffset = 0;
	MEM_SIZE_T dimsProduct = 1;
	int ith_dim = 0;
	MEM_SIZE_T num_dims = 0;
	MFUN(&(_this->shape), size), & num_dims CALL;

	for (MEM_SIZE_T dim_idx = 0; dim_idx < num_dims; dim_idx++)
	{
		innerOffset += coords[dim_idx] * dimsProduct;

		MFUN(&_this->shape, get), dim_idx, &ith_dim CALL;
		THROW_MSG_UNLESS(coords[dim_idx] <= ith_dim, "index out of range");
		dimsProduct *= ith_dim;
	}

	(*ret_val) = innerOffset;
}
END_FUN


MEM_FUN_IMPL(GenericTensor, __generic_at, MEM_SIZE_T* pos, char** val_ptr)
{
	MEM_SIZE_T index = 0;
	MFUN(_this, _get_location), pos, & index CALL;

	char* val_ptr = NULL;
	MFUN(&_this->data, __at_generic), index, _this->_element_size, & val_ptr CALL;
}
END_FUN

#define IMPL_AT_OF_TYPE(type)\
MEM_FUN_IMPL(GenericTensor, at_ ##type, MEM_SIZE_T* pos, type ** val_ptr)\
{																	 \
	MFUN(_this, __generic_at), pos, (char**)val_ptr CALL;					 \
}																	 \
END_FUN

IMPL_AT_OF_TYPE(int);
IMPL_AT_OF_TYPE(char);
IMPL_AT_OF_TYPE(float);

#define IMPL_GET_OF_TYPE(type)									\
MEM_FUN_IMPL(GenericTensor, get_ ##type, MEM_SIZE_T* pos, type * value)	\
{																\
	type * val_ptr = NULL;										\
	MFUN(_this, at_ ##type), pos, &val_ptr CALL;				\
	ASSERT_NOT_NULL(val_ptr);									\
																\
	*value = *val_ptr;											\
}																\
END_FUN

IMPL_GET_OF_TYPE(int);
IMPL_GET_OF_TYPE(char);
IMPL_GET_OF_TYPE(float);

#define IMPL_SET_OF_TYPE(type)									\
MEM_FUN_IMPL(GenericTensor, set_ ##type, MEM_SIZE_T* pos, type value)	\
{																\
	type * val_ptr = NULL;										\
	MFUN(_this, at_ ##type), pos, &val_ptr CALL;				\
	ASSERT_NOT_NULL(val_ptr);									\
																\
	*(val_ptr) = value;									\
}																\
END_FUN

IMPL_SET_OF_TYPE(int);
IMPL_SET_OF_TYPE(char);
IMPL_SET_OF_TYPE(float);

MEM_FUN_IMPL(GenericTensor, reshape, MEM_SIZE_T ndim, MEM_SIZE_T* shape)
{
	MEM_SIZE_T new_size = 1;
	for (size_t i = 0; i < ndim; i++)
	{
		new_size *= shape[i];
		THROW_MSG_UNLESS(shape[i] <= 0, "shape dims must be > 0");
	}
	THROW_MSG_UNLESS(new_size != _this->num_elements, "invalid shape");
	
	MFUN(&_this->shape, resize), ndim CALL;
	
	for (MEM_SIZE_T i = 0; i < ndim; i++) {
		MFUN(&_this->shape, set), i, shape[i] CALL;
	}
}
END_FUN

MEM_FUN_IMPL(GenericTensor, zero_all)
{
	MFUN(&_this->data, zero_all) CALL;
}
END_FUN


INIT_CLASS(GenericTensor)
BIND(GenericTensor, _get_location);

BIND(GenericTensor, __generic_at);

BIND(GenericTensor, at_int);
BIND(GenericTensor, at_char);
BIND(GenericTensor, at_float);

BIND(GenericTensor, get_int);
BIND(GenericTensor, get_char);
BIND(GenericTensor, get_float);

BIND(GenericTensor, set_int);
BIND(GenericTensor, set_char);
BIND(GenericTensor, set_float);

BIND(GenericTensor, reshape);

BIND(GenericTensor, zero_all);
END_INIT_CLASS(GenericTensor)


////////////////////////////////////////////////

#define IMPL_SPECIFIC_TENSOR_TYPE(type)																				\
DEF_DERIVED_CTOR(Tensor_ ##type, GenericTensor, MEM_SIZE_T ndim, MEM_SIZE_T* shape) SUPER, ndim, shape, sizeof(type) ME {} END_DERIVED_CTOR	\
DEF_DERIVED_DTOR(Tensor_ ##type, GenericTensor) {} END_DERIVED_DTOR													\
																													\
MEM_FUN_IMPL(Tensor_ ##type, at, MEM_SIZE_T* pos, type ** val_ptr)			{ FUN_BASE(_this, at_ ##type) , pos, val_ptr CALL; } END_FUN;	\
MEM_FUN_IMPL(Tensor_ ##type, get, MEM_SIZE_T* pos, type * val)				{ FUN_BASE(_this, get_ ##type), pos, val CALL; } END_FUN;	\
MEM_FUN_IMPL(Tensor_ ##type, set, MEM_SIZE_T* pos, type val)				{ FUN_BASE(_this, set_ ##type), pos, val CALL; } END_FUN;	\
MEM_FUN_IMPL(Tensor_ ##type, reshape, MEM_SIZE_T ndim, MEM_SIZE_T* shape)	{ FUN_BASE(_this, reshape), ndim, shape CALL; }END_FUN;\
MEM_FUN_IMPL(Tensor_ ##type, zero_all)										{ FUN_BASE(_this, zero_all) CALL; }END_FUN;\
																													\
INIT_DERIVED_CLASS(Tensor_ ##type, GenericTensor);\
BIND(Tensor_ ##type, at);						\
BIND(Tensor_ ##type, get);						\
BIND(Tensor_ ##type, set);						\
BIND(Tensor_ ##type, reshape);					\
BIND(Tensor_ ##type, zero_all);					\
END_INIT_CLASS(Tensor_ ##type)					

////////////////////////////////////////////////

IMPL_SPECIFIC_TENSOR_TYPE(int);
IMPL_SPECIFIC_TENSOR_TYPE(char);
IMPL_SPECIFIC_TENSOR_TYPE(float);

