#include "vectorIterator.h"


DEF_CTOR(vectorIterator, vector * vec, int startIdx)
{
	_this->vec = vec;
	_this->currIdx = startIdx;
}
END_CTOR

DEF_DTOR(vectorIterator)
{
	

}
END_DTOR

MEM_FUN_IMPL(vectorIterator, increment)
{
	_this->currIdx++;
}
END_FUN;

MEM_FUN_IMPL(vectorIterator, getContentsOf, int * retVal)
{
	FUN(_this->vec, at), _this->currIdx, retVal CALL
}
END_FUN;


INIT_CLASS(vectorIterator)
BIND(vectorIterator, increment);
BIND(vectorIterator, getContentsOf, int retVal);
END_INIT_CLASS

