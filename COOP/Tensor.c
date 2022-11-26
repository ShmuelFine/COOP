#include "Tensor.h"

DEF_CTOR(Tensor, MEM_SIZE_T ndim, MEM_SIZE_T* shape, MEM_SIZE_T elementSize);
{
	THROW_MSG_UNLESS(ndim > 0, "dimension must be > 0");

	INITIALIZE_INSTANCE(GenericVector, _this->shape), sizeof(MEM_SIZE_T) CALL;
	FUN(&_this->shape, resize), ndim CALL;
	_this->num_elements = 1;
	for (MEM_SIZE_T i = 0; i < ndim; i++) {
		THROW_MSG_UNLESS(shape[i] <= 0, "shape dims must be > 0");
		FUN(&_this->shape, push_back), shape[i] CALL;
		_this->num_elements *= shape[i];
	}
	_this->size_bytes = _this->num_elements * elementSize;

	INITIALIZE_INSTANCE(GenericVector, _this->data), elementSize CALL;
	FUN(&_this->data, resize), _this->size_bytes CALL;
	FUN(&_this->data, zero_all) CALL;
}
END_CTOR

DEF_DTOR(Tensor)
{

	if (_this->shape)
		DELETE(_this->shape);
	if (_this->data)
		DELETE(_this->data);
}
END_DTOR

MEM_FUN_IMPL(Tensor, _get_location, int* coords, int* ret_val)
{
	int index = 0;
	int dim = 1;
	for (int i = _this->ndim - 1; i >= 0; i--)
	{
		if (coords[i] >= _this->shape[i])
		{
			THROW_MSG("index out of range")
		}
		index += coords[i] * dim;
		dim *= _this->shape[i];
	}

	//if (index >= _this->size)
	//{
	//	THROW_MSG("index out of range")
	//}
	(*ret_val) = index;

}
END_FUN

MEM_FUN_IMPL(Tensor, set, int* pos, float val)
{
	int index = 0;
	FUN(_this, _get_location), pos, & index CALL;

	_this->data[index] = val;
}
END_FUN

MEM_FUN_IMPL(Tensor, at, int* pos, float* ret_val) {
	int index = 0;
	FUN(_this, _get_location), pos, & index CALL;

	(*ret_val) = _this->data[index];
}
END_FUN

MEM_FUN_IMPL(Tensor, reshape, int ndim, int* shape)
{
	int new_size = 1;
	for (size_t i = 0; i < ndim; i++)
	{
		new_size *= shape[i];
	}
	if (new_size != _this->size) {
		THROW_MSG("invalid shape")
	}
	_this->ndim = ndim;
	memcpy(_this->shape, shape, ndim);
}
END_FUN

MEM_FUN_IMPL(Tensor, fill, float val)
{
	for (int i = 0; i < _this->size; i++)
	{
		_this->data[i] = val;
	}
	//memset(_this->data, val, _this->size * sizeof(float));

}
END_FUN

MEM_FUN_IMPL(Tensor, sum, float* ret_value)
{
	float sum = 0;
	for (size_t i = 0; i < _this->ndim; i++)
	{
		sum += _this->data[i];
	}
	*ret_value = sum;
}
END_FUN

MEM_FUN_IMPL(Tensor, contains, float val, bool* ret_val)
{
	int i = 0;
	for (i = 0; i < _this->size && _this->data[i] != val; i++);
	*ret_val = i != _this->size;
}
END_FUN

MEM_FUN_IMPL(Tensor, min, float* ret_val)
{
	float min = _this->data[0];
	for (int i = 1; i < _this->size; i++) {
		if (_this->data[i] < min)
		{
			min = _this->data[i];
		}
	}
	*ret_val = min;
}
END_FUN

MEM_FUN_IMPL(Tensor, max, float* ret_val)
{
	float max = _this->data[0];
	for (int i = 1; i < _this->size; i++) {
		if (_this->data[i] > max)
		{
			max = _this->data[i];
		}
	}
	*ret_val = max;
}
END_FUN


INIT_CLASS(Tensor)
BIND(Tensor, _get_location);
BIND(Tensor, set);
BIND(Tensor, at);
BIND(Tensor, reshape);
BIND(Tensor, fill);
BIND(Tensor, sum);
BIND(Tensor, min);
BIND(Tensor, max);
BIND(Tensor, contains);
END_INIT_CLASS(Tensor)