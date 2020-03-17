#include "vector.h"
#include <stdio.h>

DEF_CTOR(vector)
{
	_this->size = 0;
	_this->capacity = 0;
	_this->sizeOfElement = sizeof(int);
	
	_this->activeBufferIndex = 0;
	_this->data[0] = NULL;
	_this->data[1] = NULL;
}
END_CTOR

DEF_DTOR(vector)
{
	for (int i = 0; i < 2; i++)
	{
		if (_this->data[i] != NULL)
			DELETE_OBJ(_this->data[i]);
	}
/*
	if (_this->capacity > 0)
	{		
		DELETE_OBJ(_this->data);	
	}*/	
	
}
END_DTOR

MEM_FUN_IMPL(vector, push_back, int val)
{
	if (_this->size >= _this->capacity)
	{
		int currBuffIdx = _this->activeBufferIndex;
		int newBuffIdx = 1 - _this->activeBufferIndex;

		int newCapacity;
		if (_this->capacity == 0)
			newCapacity = 1;
		else
			newCapacity = (_this->capacity) * 2;
		
		NEW_OF_SIZE(_this->data[newBuffIdx], int, newCapacity);
		
		for (int i = 0; i < _this->size; i++)
			_this->data[newBuffIdx][i] = _this->data[currBuffIdx][i];
		if (_this->data[currBuffIdx] != NULL)
		    DELETE_OBJ(_this->data[currBuffIdx]);

		_this->capacity = newCapacity;
		_this->activeBufferIndex = newBuffIdx;
	}
	_this->data[_this->activeBufferIndex][_this->size] = val;
	_this->size++;
}
END_FUN;

MEM_FUN_IMPL(vector, at, int idx, int * ret_val)
{
	if (idx >= (_this->size))
	{
		THROW_MSG("OUT_OF_RANGE");
	}
		
	*ret_val = _this->data[_this->activeBufferIndex][idx];
}
END_FUN;

MEM_FUN_IMPL(vector, begin, vectorIterator retVecIt)
{
	CREATE(vectorIterator, retVecIter), _this, 0);
	retVecIt = retVecIter;
}
END_FUN;

MEM_FUN_IMPL(vector, end, vectorIterator retVecIt)
{
	CREATE(vectorIterator, retVecIter), _this, _this->size);
	retVecIt = retVecIter;

}
END_FUN;

MEM_FUN_IMPL(vector, print)
{
	for(int i = 0; i < _this->size; i ++)
		printf("%d ", _this->data[_this->activeBufferIndex][i]);
}
END_FUN;


INIT_CLASS(vector)
BIND(vector, push_back);
BIND(vector, at);
BIND(vector, begin);
BIND(vector, end);
BIND(vector, print);
END_INIT_CLASS

