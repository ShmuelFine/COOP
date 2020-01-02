#include "LinkedLists.h"

function* callFunction(function* func)
{
	function* funcPtr = func;
	if (func->next)
		return callFunction(func->next);
	return funcPtr;
}

