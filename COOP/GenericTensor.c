#include "GenericTensor.h"


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
	MEM_SIZE_T index = 0;
	MEM_SIZE_T dim = 1;
	int ith_dim = 0;
	MEM_SIZE_T num_dims = _this->shape._base.size;
	for (MEM_SIZE_T k = 0; k < num_dims; k++)
	{
		MEM_SIZE_T i = (num_dims - 1) - k; // we don't do usual "reverse iteration" since MEM_SIZE_T is unsigned.

		MFUN(&_this->shape, get), i, &ith_dim CALL;
		THROW_MSG_UNLESS(coords[i] <= ith_dim, "index out of range");
		index += coords[i] * dim;
		dim *= ith_dim;
	}

	(*ret_val) = index;
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
	}
	THROW_MSG_UNLESS(new_size != _this->num_elements, "invalid shape");
	
	MFUN(&_this->shape, resize), ndim CALL;
	
	for (MEM_SIZE_T i = 0; i < ndim; i++) {
		THROW_MSG_UNLESS(shape[i] <= 0, "shape dims must be > 0");
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


/*
MEM_FUN_IMPL(GenericTensor, sum, float* ret_value)
{
	float sum = 0;
	for (size_t i = 0; i < _this->ndim; i++)
	{
		sum += _this->data[i];
	}
	*ret_value = sum;
}
END_FUN

MEM_FUN_IMPL(GenericTensor, contains, float val, bool* ret_val)
{
	MEM_SIZE_T i = 0;
	for (i = 0; i < _this->size && _this->data[i] != val; i++);
	*ret_val = i != _this->size;
}
END_FUN

MEM_FUN_IMPL(GenericTensor, min, float* ret_val)
{
	float min = _this->data[0];
	for (MEM_SIZE_T i = 1; i < _this->size; i++) {
		if (_this->data[i] < min)
		{
			min = _this->data[i];
		}
	}
	*ret_val = min;
}
END_FUN

MEM_FUN_IMPL(GenericTensor, max, float* ret_val)
{
	float max = _this->data[0];
	for (MEM_SIZE_T i = 1; i < _this->size; i++) {
		if (_this->data[i] > max)
		{
			max = _this->data[i];
		}
	}
	*ret_val = max;
}
END_FUN
*/