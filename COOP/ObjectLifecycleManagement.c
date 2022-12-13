#include "ObjectLifecycleManagement.h"
#include <stdlib.h>


void _scope_obj_list_add(object* scope_list, object* obj)
{
	obj->_next = scope_list->_next;
	scope_list->_next = obj;
}

void FreeMostInnerScope(object* _scope_obj_list_ptr)
{
	object* it = _scope_obj_list_ptr;

	while (it->_next) {
		if (it->_next->vTable != NULL)
		{
			DESTROY(it->_next);
			it->_next = it->_next->_next;
		}
		else
		{
			it->_next = it->_next->_next;
			break;
		}
	}
	if (it->_next && it->_next->vTable == NULL)
		_scope_obj_list_ptr->_next = NULL;
}


const char* LAST_EXCEPTION_ERROR_MSG;