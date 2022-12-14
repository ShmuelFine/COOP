#include "ObjectLifecycleManagement.h"
#include <stdlib.h>


void _scope_obj_list_add(object* scope_list, object* obj)
{
	obj->_next = scope_list->_next;
	scope_list->_next = obj;
}

void FreeMostInnerScope(object* _scope_obj_list_ptr)
{
	for (object* it = _scope_obj_list_ptr->_next; it != NULL; it = it->_next)
	{
		DESTROY(it);
	}
}


const char* LAST_EXCEPTION_ERROR_MSG;