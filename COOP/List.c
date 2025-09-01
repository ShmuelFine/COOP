#include "List.h"
#include "COOP.h"
#include <stdlib.h>
#include <string.h>

DEF_CTOR(GenericList, MEM_SIZE_T dataTypeSize)
{
	_this->size = 0;
	_this->capacity = 0;
	_this->elementSize = dataTypeSize;

	_this->data = NULL;
}
END_CTOR


