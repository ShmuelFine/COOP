#include "vector.h"

DEF_CTOR(vector)
{
	_this->size = 0;
	_this->capacity = 0;
	_this->sizeOfElement = sizeof(int);
}
END_CTOR

DEF_DTOR(vector)
{
	if (_this->capacity > 0)
	{		
		DELETE_OBJ(_this->data);	
	}	
	
}
END_DTOR

MEM_FUN_IMPL(vector, push_back, int val)
{
	if (_this->size >= _this->capacity)
	{
		int newCapacity;
		if (_this->capacity == 0)
			newCapacity = 1;
		else
			newCapacity = (_this->capacity) * 2;
		NEW_OF_SIZE(_this->newData, int, newCapacity);
		for (int i = 0; i < _this->size; i++)
			_this->newData[i] = _this->data[i];
		if (_this->capacity > 0)
		    DELETE_OBJ(_this->data);
		NEW_OF_SIZE(_this->data, int, newCapacity);
		for (int i = 0; i < _this->size; i++)
			_this->data[i] = _this->newData[i];
		DELETE_OBJ(_this->newData);
		(_this->capacity) = newCapacity;
	}
	_this->data[_this->size] = val;
	(_this->size)++;
}
END_FUN;

MEM_FUN_IMPL(vector, at, int idx, int * ret_val)
{
	if(idx >= _this->size)
		THROW_MSG("OUT_OF_RANGE");
	*ret_val = _this->data[idx];
}
END_FUN;

MEM_FUN_IMPL(vector, front, int* ret_val)
{
	
}
END_FUN;

INIT_CLASS(vector)
BIND(vector, push_back);
BIND(vector, at);
BIND(vector, front);
END_INIT_CLASS

