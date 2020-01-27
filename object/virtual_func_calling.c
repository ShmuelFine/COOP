#include "virtual_func_calling.h"

function* call_obj_function(function* func)
{
	function* funcPtr = func;
	if (func->next)
		return call_obj_function(func->next);
	else
		return funcPtr;
}

