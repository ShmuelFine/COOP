#include "Ndarray.h"

DEF_CTOR(Ndarray, int ndim, int* shape)
{
	_this->ndim = ndim;
	if (ndim < 0) {
		THROW_MSG("dimension can't be negative")
	}
	if (ndim == 0) {
		_this->size = 0;
		_this->data = NULL;
		NEW_OF_SIZE(_this->shape, int, 1);
		_this->shape[0] = 0;
	}
	else {
		_this->size = 1;

		NEW_OF_SIZE(_this->shape, int, ndim);

		for (size_t i = 0; i < ndim; i++)
		{
			if (shape[i] <= 0) {
				THROW_MSG("invalid shape")
			}
			_this->shape[i] = shape[i];
			_this->size *= shape[i];
		}

		NEW_OF_SIZE(_this->data, float, _this->size);
		memset(_this->data, 0, _this->size * sizeof(float));

		//_this->min = _this->max = 0;
	}
}
END_CTOR

DEF_DTOR(Ndarray)
{
	if (_this->shape != NULL)
		DELETE_OBJ(_this->shape);
	if (_this->data != NULL)
		DELETE_OBJ(_this->data);
}
END_DTOR

MEM_FUN_IMPL(Ndarray, get_location, int* coords, int* ret_val)
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

MEM_FUN_IMPL(Ndarray, set, int* pos, float val)
{
	int index = 0;
	FUN(_this, get_location), pos, & index CALL;

	_this->data[index] = val;

	//if (val > _this->max) {
	//	_this->max = val;
	//}
	//if (val < _this->min) {
	//	_this->min = val;
	//}
}
END_FUN

MEM_FUN_IMPL(Ndarray, at, int* pos, float* ret_val) {
	int index = 0;
	FUN(_this, get_location), pos, & index CALL;

	*ret_val = _this->data[index];
}
END_FUN

MEM_FUN_IMPL(Ndarray, reshape, int ndim, int* shape)
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

MEM_FUN_IMPL(Ndarray, fill, float val)
{
	for (int i = 0; i < _this->size; i++)
	{
		_this->data[i] = val;
	}
	//memset(_this->data, val, _this->size * sizeof(float));
	//_this->min = _this->max = val;

}
END_FUN

MEM_FUN_IMPL(Ndarray, sum, float* ret_value)
{
	float sum = 0;
	for (size_t i = 0; i < _this->ndim; i++)
	{
		sum += _this->data[i];
	}
	*ret_value = sum;
}
END_FUN

MEM_FUN_IMPL(Ndarray, contains, float val, bool* ret_val)
{
	int i = 0;
	for (i = 0; i < _this->size && _this->data[i] != val; i++);
	*ret_val = i != _this->size;
}
END_FUN

MEM_FUN_IMPL(Ndarray, min, float* ret_val)
{

}
END_FUN

MEM_FUN_IMPL(Ndarray, max, float* ret_val)
{

}
END_FUN


INIT_CLASS(Ndarray)
BIND(Ndarray, get_location);
BIND(Ndarray, set);
BIND(Ndarray, at);
BIND(Ndarray, reshape);
BIND(Ndarray, fill);
BIND(Ndarray, sum);
BIND(Ndarray, min);
BIND(Ndarray, max);
BIND(Ndarray, contains);
END_INIT_CLASS