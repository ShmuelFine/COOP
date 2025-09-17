#include "List.h"
#include "COOP.h"
#include <string.h>
#include <stdio.h>

/* ======================== CTOR / DTOR ======================== */

DEF_CTOR(GenericList, MEM_SIZE_T dataTypeSize, List_ElementType enumTag)
{
	_this->size = 0;
	_this->elementSize = dataTypeSize;
	_this->head = NULL;
	_this->tail = NULL;
	_this->elem_type = (enumTag);

	CREATE(ListIter, begin),ITER_BIDIRECTIONAL, _this CALL;
	CREATE(ListIter, end), ITER_BIDIRECTIONAL, _this CALL;

	_this->begin_iter = begin;
	_this->end_iter = end;

	LIST_UPDATE_ITERS_TAILEND(_this);

}
END_CTOR

DEF_DTOR(GenericList)
{
	ListNode* cur = _this->head;

	WHILE(cur != NULL)
	{
		ListNode* next = cur->next;
		IF(_this->elem_type == LIST_ELEM_OBJ_SPTR)
		{
			objSPtr* sp = (objSPtr*)(cur->payload);
			DESTROY(sp);
		}
		END_IF;
		FREE(cur);
		cur = next;
	}
	END_LOOP;

	_this->head = NULL;
	_this->tail = NULL;
	_this->size = 0;
	LIST_UPDATE_ITERS_TAILEND(_this);

}
END_DTOR

/* ======================== Helpers ======================== */

MEM_FUN_IMPL(GenericList, __make_node, const char* src_bytes, MEM_SIZE_T buff_size, ListNode** out_node)
{
	THROW_MSG_UNLESS(out_node, "out_node must not be NULL");
	IF(src_bytes != NULL) {
		THROW_MSG_UNLESS(buff_size == _this->elementSize, "Invalid Data Size");
	}
	END_IF;

	MEM_SIZE_T bytes = (MEM_SIZE_T)(sizeof(ListNode) + _this->elementSize);

	ListNode* nd = NULL;
	ALLOC_ARRAY(nd, char, bytes);

	nd->prev = NULL;
	nd->next = NULL;

	IF(src_bytes != NULL) {
		memcpy(nd->payload, src_bytes, (size_t)_this->elementSize);
	}
	ELSE{
		memset(nd->payload, 0, (size_t)_this->elementSize);
	}
	END_IF;

	*out_node = nd;
}
END_FUN

/* ======================== print functions ======================== */


MEM_FUN_IMPL(GenericList, __print_value, const void* p)
{
	IF(_this->elem_type == LIST_ELEM_INT) {
		printf("%d ", *(const int*)p);
	}

	ELSE_IF(_this->elem_type == LIST_ELEM_FLOAT) {
		printf("%f ", *(const float*)p);
	}

	ELSE_IF(_this->elem_type == LIST_ELEM_CHAR) {
		printf("%c ", *(const char*)p);
	}

	ELSE_IF(_this->elem_type == LIST_ELEM_OBJ_SPTR)
	{
		MFUN((objSPtr*)p, print) CALL;
	}

	ELSE 
	{
		const unsigned char* bytes = (const unsigned char*)p;
		FOR(MEM_SIZE_T i = 0; i < _this->elementSize; i++) {
			printf("%02X ", bytes[i]);
		}
		END_LOOP;
			printf(" ");
	}END_IF;
}
END_FUN

MEM_FUN_IMPL(GenericList, print)
{
	IF(_this->head == NULL) {
		RETURN;
	}END_IF;

	printf("\n");
	
	ITER_FOR(void*, value, _this) {
		MFUN(_this, __print_value), (const void*)&value CALL;
	}END_ITER_FOR;

	printf("\n");
}
END_FUN



/* ======================== size / empty / clear ======================== */



MEM_FUN_IMPL(GenericList, size, MEM_SIZE_T* out_size)
{
	THROW_MSG_UNLESS(out_size, "out_size must not be NULL");
	*out_size = _this->size;
}
END_FUN

MEM_FUN_IMPL(GenericList, empty, bool* out_is_empty)
{
	THROW_MSG_UNLESS(out_is_empty, "out_is_empty must not be NULL");
	*out_is_empty = (_this->size == 0);
}
END_FUN

MEM_FUN_IMPL(GenericList, clear)
{
	ListNode* cur = _this->head;

	WHILE(cur != NULL) {
		ListNode* next = cur->next;
		IF(_this->elem_type == LIST_ELEM_OBJ_SPTR)
		{
			objSPtr* sp = (objSPtr*)(cur->payload);
			DESTROY(sp);
		}
		END_IF;
		FREE(cur);
		cur = next;
	}
	END_LOOP;

	_this->head = NULL;
	_this->tail = NULL;
	_this->size = 0;
	LIST_UPDATE_ITERS_TAILEND(_this);

}
END_FUN

/* ======================== list functions  ======================== */


/* ---- push_back ---- */
MEM_FUN_IMPL(GenericList, __push_back_generic, char* buff, MEM_SIZE_T buff_size)
{
	THROW_MSG_UNLESS(buff_size == _this->elementSize, "Invalid Data Size");

	ListNode* nd = NULL;
	MFUN(_this, __make_node), (const char*)buff, buff_size, & nd CALL;

	IF(_this->tail == NULL) {
		_this->head = nd;
		_this->tail = nd;
	}
	ELSE{
		nd->prev = _this->tail;
		_this->tail->next = nd;
		_this->tail = nd;
	}
	END_IF;

	_this->size += 1;
	LIST_UPDATE_ITERS_TAILEND(_this);

}
END_FUN

#define IMPL_PUSH_BACK_OF_TYPE(type)												\
MEM_FUN_IMPL(GenericList, push_back_ ##type, type val) {							\
    MFUN(_this, __push_back_generic), (char*)&(val), (MEM_SIZE_T)sizeof(type) CALL; \
} END_FUN;
IMPL_PUSH_BACK_OF_TYPE(int)

/* ---- push_front ---- */
MEM_FUN_IMPL(GenericList, __push_front_generic, char* buff, MEM_SIZE_T buff_size)
{
	THROW_MSG_UNLESS(buff_size == _this->elementSize, "Invalid Data Size");

	ListNode* nd = NULL;
	MFUN(_this, __make_node), (const char*)buff, buff_size, & nd CALL;

	IF(_this->head == NULL) {
		_this->head = nd;
		_this->tail = nd;
	}
	ELSE{
		nd->next = _this->head;
		_this->head->prev = nd;
		_this->head = nd;
	}
	END_IF;

	_this->size += 1;
	LIST_UPDATE_ITERS_TAILEND(_this);

}
END_FUN

#define IMPL_PUSH_FRONT_OF_TYPE(type) \
MEM_FUN_IMPL(GenericList, push_front_ ##type, type val) { \
    MFUN(_this, __push_front_generic), (char*)&(val), (MEM_SIZE_T)sizeof(type) CALL; \
} END_FUN;
IMPL_PUSH_FRONT_OF_TYPE(int)

/* ---- back ---- */
MEM_FUN_IMPL(GenericList, __back_generic, char* buff, MEM_SIZE_T buff_size)
{
	THROW_MSG_UNLESS(_this->tail, "back() on empty list");
	THROW_MSG_UNLESS(buff_size == _this->elementSize, "Invalid Data Size");
	ASSERT_NOT_NULL(buff);
	memcpy(buff, _this->tail->payload, (size_t)_this->elementSize);
}
END_FUN

#define IMPL_BACK_OF_TYPE(type) \
MEM_FUN_IMPL(GenericList, back_ ##type, type* out_val) { \
    MFUN(_this, __back_generic), (char*)(out_val), (MEM_SIZE_T)sizeof(type) CALL; \
} END_FUN;
IMPL_BACK_OF_TYPE(int)

/* ---- front ---- */
MEM_FUN_IMPL(GenericList, __front_generic, char* buff, MEM_SIZE_T buff_size)
{
	THROW_MSG_UNLESS(_this->head, "front() on empty list");
	THROW_MSG_UNLESS(buff_size == _this->elementSize, "Invalid Data Size");
	ASSERT_NOT_NULL(buff);
	memcpy(buff, _this->head->payload, (size_t)_this->elementSize);
}
END_FUN

#define IMPL_FRONT_OF_TYPE(type) \
MEM_FUN_IMPL(GenericList, front_ ##type, type* out_val) { \
    MFUN(_this, __front_generic), (char*)(out_val), (MEM_SIZE_T)sizeof(type) CALL; \
} END_FUN;
IMPL_FRONT_OF_TYPE(int)

/* ---- pop_back ---- */
MEM_FUN_IMPL(GenericList, __pop_back_generic, char* buff, MEM_SIZE_T buff_size)
{
	THROW_MSG_UNLESS(_this->tail, "pop_back() on empty list");
	THROW_MSG_UNLESS(buff_size == _this->elementSize, "Invalid Data Size");
	ASSERT_NOT_NULL(buff);

	ListNode* nd = _this->tail;

	IF(_this->elem_type == LIST_ELEM_OBJ_SPTR)
	{
		const objSPtr* src = (const objSPtr*)nd->payload;
		objSPtr* dst = (objSPtr*)buff;
		MFUN(dst, copyFrom), src CALL;

		DESTROY((objSPtr*)src);
	}
	ELSE
	{
		memcpy(buff, nd->payload, (size_t)_this->elementSize);
	}
	END_IF;

	_this->tail = nd->prev;
	IF(_this->tail) {
		_this->tail->next = NULL;
	}
	ELSE{
		_this->head = NULL;
	}
	END_IF;

	FREE(nd);
	_this->size -= 1;

	LIST_UPDATE_ITERS_TAILEND(_this);

}
END_FUN

#define IMPL_POP_BACK_OF_TYPE(type) \
MEM_FUN_IMPL(GenericList, pop_back_ ##type, type* out_val) { \
    MFUN(_this, __pop_back_generic), (char*)(out_val), (MEM_SIZE_T)sizeof(type) CALL; \
} END_FUN;
IMPL_POP_BACK_OF_TYPE(int)

/* ---- pop_front ---- */
MEM_FUN_IMPL(GenericList, __pop_front_generic, char* buff, MEM_SIZE_T buff_size)
{
	THROW_MSG_UNLESS(_this->head, "pop_front() on empty list");
	THROW_MSG_UNLESS(buff_size == _this->elementSize, "Invalid Data Size");
	ASSERT_NOT_NULL(buff);

	ListNode* nd = _this->head;
	IF(_this->elem_type == LIST_ELEM_OBJ_SPTR)
	{
		const objSPtr* src = (const objSPtr*)nd->payload;
		objSPtr* dst = (objSPtr*)buff;
		MFUN(dst, copyFrom), src CALL;
		DESTROY((objSPtr*)src);
	}
	ELSE
	{
		memcpy(buff, nd->payload, (size_t)_this->elementSize);
	}
	END_IF;

	_this->head = nd->next;
	IF(_this->head) {
		_this->head->prev = NULL;
	}
	ELSE{
		_this->tail = NULL;
	}
	END_IF;

	FREE(nd);
	_this->size -= 1;

	LIST_UPDATE_ITERS_TAILEND(_this);

}
END_FUN

#define IMPL_POP_FRONT_OF_TYPE(type) \
MEM_FUN_IMPL(GenericList, pop_front_ ##type, type* out_val) { \
    MFUN(_this, __pop_front_generic), (char*)(out_val), (MEM_SIZE_T)sizeof(type) CALL; \
} END_FUN;
IMPL_POP_FRONT_OF_TYPE(int)

/* ======================== BIND – GenericList ======================== */

INIT_CLASS(GenericList);
BIND(GenericList, size);
BIND(GenericList, empty);
BIND(GenericList, clear);
BIND(GenericList, __make_node);

BIND(GenericList, __print_value);
BIND(GenericList, print);


BIND(GenericList, __push_back_generic);
BIND(GenericList, __push_front_generic);
BIND(GenericList, __pop_back_generic);
BIND(GenericList, __pop_front_generic);
BIND(GenericList, __front_generic);
BIND(GenericList, __back_generic);

/* typed-on-base  */
BIND(GenericList, push_back_int);
BIND(GenericList, push_front_int);
BIND(GenericList, pop_back_int);
BIND(GenericList, pop_front_int);
BIND(GenericList, front_int);
BIND(GenericList, back_int);

END_INIT_CLASS(GenericList)

/* =========================================================
 *                 ListIter (derived from Iterator)
 * ========================================================= */

DEF_DERIVED_CTOR(ListIter, Iterator, IteratorCategory category, void* container_ptr) SUPER, category, container_ptr ME
{
	_this->node = NULL;
}
END_DERIVED_CTOR

DEF_DERIVED_DTOR(ListIter, Iterator)
{
	_this->node = NULL;
}
END_DERIVED_DTOR

FUN_OVERRIDE_IMPL(ListIter, Iterator, equals, Iterator* other, bool* out_equal)
{
	THROW_MSG_UNLESS(out_equal, "out_equal must not be NULL");
	*out_equal = false;

	IF(other) {
		bool base_equals = false;
		FUN_BASE(_this, equals), other, & base_equals CALL;
		*out_equal = base_equals && (((ListIter*)other)->node == _this->node);
	} END_IF;
}
END_FUN


FUN_OVERRIDE_IMPL(ListIter, Iterator, next)
{
	IF(_this->node) {
		_this->node = _this->node->next;
	}
	END_IF;
}
END_FUN

FUN_OVERRIDE_IMPL(ListIter, Iterator, prev)
{
	IF(_this->node) {
		IF(_this->node->prev) {
			_this->node = _this->node->prev;
		}
		ELSE{
			THROW_MSG("Iterator::prev called on begin()");
		}
		END_IF;
	}
	ELSE{
	   GenericList * owner = (GenericList*)(_this)->_base.container_ptr;
	   _this->node = owner ? owner->tail : NULL;
	}
	END_IF;
}
END_FUN

FUN_OVERRIDE_IMPL(ListIter, Iterator, get_ref, void** out_ptr)
{
	THROW_MSG_UNLESS(out_ptr, "out_ptr must not be NULL");
	THROW_MSG_UNLESS(_this->node, "dereference end() iterator");
	*out_ptr = (void*)_this->node->payload;
}
END_FUN

FUN_OVERRIDE_IMPL(ListIter, Iterator, get_cref, const void** out_ptr)
{
	THROW_MSG_UNLESS(out_ptr, "out_ptr must not be NULL");
	THROW_MSG_UNLESS(_this->node, "dereference end() iterator");
	*out_ptr = (const void*)_this->node->payload;
}
END_FUN

FUN_OVERRIDE_IMPL(ListIter, Iterator, distance, Iterator* other_iter, ptrdiff_t* out_distance)
{
	THROW_MSG_UNLESS(out_distance, "out_distance must not be NULL");
	THROW_MSG_UNLESS(other_iter, "other_iter must not be NULL");

	GenericList* this_list = (GenericList*)_this->_base.container_ptr;
	GenericList* other_list = (GenericList*)((Iterator*)other_iter)->container_ptr;
	THROW_MSG_UNLESS(this_list && (this_list == other_list),
		"iterators must belong to the same list");

	ListIter* target_iter = (ListIter*)other_iter;

	IF(_this->node == target_iter->node) {
		*out_distance = 0;
	} END_IF;

	{
		ptrdiff_t  steps_forward = 0;
		ListNode* current_node = _this->node;
		WHILE(current_node) {
			IF(current_node == target_iter->node) {
				*out_distance = steps_forward;
			} END_IF;
			current_node = current_node->next;
			steps_forward += 1;
		} END_LOOP;
	}

	{
		ptrdiff_t  steps_backward = 0;
		ListNode* current_node = _this->node;
		WHILE(current_node) {
			IF(current_node == target_iter->node) {
				*out_distance = steps_backward;
			} END_IF;
			current_node = current_node->prev;
			steps_backward -= 1;
		} END_LOOP;
	}
}
END_FUN


FUN_OVERRIDE_IMPL(ListIter, Iterator, advance, ptrdiff_t n)
{
	FUN_BASE(_this, advance), n CALL;
}
END_FUN

FUN_OVERRIDE_IMPL(ListIter, Iterator, reset_begin)
{
	GenericList* owner = (GenericList*)(_this)->_base.container_ptr;
	THROW_MSG_UNLESS(owner, "Iterator not bound");
	_this->node = owner->head;
}
END_FUN

INIT_DERIVED_CLASS(ListIter, Iterator);
BIND_OVERIDE(ListIter, Iterator, equals);
BIND_OVERIDE(ListIter, Iterator, next);
BIND_OVERIDE(ListIter, Iterator, prev);
BIND_OVERIDE(ListIter, Iterator, get_ref);
BIND_OVERIDE(ListIter, Iterator, get_cref);
BIND_OVERIDE(ListIter, Iterator, distance);
BIND_OVERIDE(ListIter, Iterator, advance);
BIND_OVERIDE(ListIter, Iterator, reset_begin);
END_INIT_CLASS(ListIter)

////////////////////////////////////////////////

#define IMPL_SPECIFIC_LIST_TYPE_xTORs(type, enumTag)                                          \
DEF_DERIVED_CTOR(List_##type, GenericList) SUPER, sizeof(type), enumTag ME { } END_DERIVED_CTOR \
DEF_DERIVED_DTOR(List_##type, GenericList) { } END_DERIVED_DTOR

#define IMPL_SPECIFIC_LIST_TYPE_FUNCTIONS(type)                                               \
MEM_FUN_IMPL(List_##type, push_back,  type val)        { FUN_BASE(_this, push_back_##type),  val CALL; } END_FUN; \
MEM_FUN_IMPL(List_##type, push_front, type val)        { FUN_BASE(_this, push_front_##type), val CALL; } END_FUN; \
MEM_FUN_IMPL(List_##type, pop_back,   type* out)       { FUN_BASE(_this, pop_back_##type),   out CALL; } END_FUN; \
MEM_FUN_IMPL(List_##type, pop_front,  type* out)       { FUN_BASE(_this, pop_front_##type),  out CALL; } END_FUN; \
MEM_FUN_IMPL(List_##type, front,      type* out)       { FUN_BASE(_this, front_##type),      out CALL; } END_FUN; \
MEM_FUN_IMPL(List_##type, back,       type* out)       { FUN_BASE(_this, back_##type),       out CALL; } END_FUN; \
MEM_FUN_IMPL(List_##type, size,       MEM_SIZE_T* out) { FUN_BASE(_this, size),              out CALL; } END_FUN; \
MEM_FUN_IMPL(List_##type, empty,      bool* out)       { FUN_BASE(_this, empty),             out CALL; } END_FUN; \
MEM_FUN_IMPL(List_##type, clear)                        { FUN_BASE(_this, clear) CALL; } END_FUN;               \
MEM_FUN_IMPL(List_##type, print)                        { FUN_BASE(_this, print) CALL; } END_FUN;              \
INIT_DERIVED_CLASS(List_##type, GenericList);                                               \
BIND(List_##type, push_back);                                                               \
BIND(List_##type, push_front);                                                              \
BIND(List_##type, pop_back);                                                                \
BIND(List_##type, pop_front);                                                               \
BIND(List_##type, front);                                                                   \
BIND(List_##type, back);                                                                    \
BIND(List_##type, size);                                                                    \
BIND(List_##type, empty);                                                                   \
BIND(List_##type, clear);                                                                   \
BIND(List_##type, print);                                                                   \
END_INIT_CLASS(List_##type);

IMPL_SPECIFIC_LIST_TYPE_xTORs(int, LIST_ELEM_INT);
IMPL_SPECIFIC_LIST_TYPE_FUNCTIONS(int);
