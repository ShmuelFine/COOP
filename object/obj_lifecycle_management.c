#include "obj_lifecycle_management.h"
#include <stdlib.h>


void _scope_obj_list_add(object* scope_list, object* obj)
{
	obj->_next = scope_list->_next;
	scope_list->_next = obj;
}

void _scope_obj_list_free(object* scope_list)
{
	if (scope_list->_next)
		_scope_obj_list_free(scope_list->_next);

	if (scope_list->vTable)
		scope_list->vTable->_dtor(scope_list);
}

void FreeMostInnerScope(object* _scope_obj_list_ptr)
{
	object* it = _scope_obj_list_ptr;

	while (it->_next) {
		if (it->_next->vTable != NULL)
		{
			it->_next->vTable->_dtor(it->_next);
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


jmp_buf SCOPE_FALLBACK_ADDR[10] = { 0 };
int _CurrScope_Idx = 0;

