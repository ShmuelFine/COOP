#include "BinaryTree.h"
#include "DynamicMemoryManagement.h"
#include "SharedObjPtr.h"
#include "InMemoryCache.h"
#include <string.h>


/* ====== BTNode implementation ====== */

DEF_CTOR(BTNode, MEM_SIZE_T elementSize, const void* src_bytes, BTNode* parent)
{
	_this->left = NULL;
	_this->right = NULL;
	_this->parent = NULL;
	_this->value = NULL;

	IF(elementSize > 0)
	{
		ALLOC_ARRAY(_this->value, char, elementSize);
		IF(src_bytes != NULL)
		{
			memcpy(_this->value, src_bytes, elementSize);
		}
		ELSE
		{
			memset(_this->value, 0, elementSize);
		}
		END_IF;
	}
	END_IF;

	_this->parent = parent;
}
END_CTOR

DEF_DTOR(BTNode)
{
	FREE(_this->value);
	_this->value = NULL;
	_this->left = NULL;
	_this->right = NULL;
	_this->parent = NULL;
}
END_DTOR

INIT_CLASS(BTNode)
END_INIT_CLASS(BTNode)

/* ====== GenericBinaryTree implementation ====== */

DEF_CTOR(GenericBinaryTree, MEM_SIZE_T elementSize, BT_ElementType BT_type)
{
	_this->root = NULL;
	_this->size = 0;
	_this->elementSize = elementSize;
	_this->BT_type = BT_type;
}
END_CTOR

DEF_DTOR(GenericBinaryTree)
{

	IF(_this->root == NULL)
	{
		_this->size = 0;
		RETURN;
	}
	END_IF;

	MEM_SIZE_T cap = (_this->size ? _this->size : 8);
	BTNode** stack1 = NULL;
	ALLOC_ARRAY(stack1, BTNode*, cap);
	BTNode** stack2 = NULL;
	ALLOC_ARRAY(stack2, BTNode*, cap);
	MEM_SIZE_T top1 = 0, top2 = 0;

	stack1[top1++] = _this->root;

	/* Step 1: Fill stack2 in reverse order */
	WHILE(top1 > 0)
	{
		BTNode* curr = stack1[--top1];
		stack2[top2++] = curr;

		IF(curr->left)
		{
			stack1[top1++] = curr->left;
		}
		END_IF;
		IF(curr->right)
		{
			stack1[top1++] = curr->right;
		}
		END_IF;
	}
	END_LOOP;

	/* Step 2: Pop from stack2 — this is where the actual release is done */
	WHILE(top2 > 0)
	{
		BTNode* curr = stack2[--top2];
		DESTROY(curr);
		FREE(curr);
	}
	END_LOOP;

	FREE(stack1);
	FREE(stack2);

	_this->root = NULL;
	_this->size = 0;
}
END_DTOR

MEM_FUN_IMPL(GenericBinaryTree, is_empty, bool* out)
{
	*out = (_this->size == 0);
}
END_FUN

MEM_FUN_IMPL(GenericBinaryTree, get_size, MEM_SIZE_T* out)
{
	*out = _this->size;
}
END_FUN

/* Move by levels when it finds a right or left that are NULL, inserts there */
MEM_FUN_IMPL(GenericBinaryTree, __insert_generic, const void* src)
{
	IF(_this->root == NULL)
	{
		BTNode* n = NULL;
		ALLOC(n, BTNode);
		INITIALIZE_INSTANCE(BTNode, (*n)), _this->elementSize, (const void*)src, (BTNode*)NULL CALL;
		_this->root = n;
		_this->size = 1;
		RETURN;
	}
	END_IF;

	MEM_SIZE_T cap = _this->size + 2;
	BTNode** q = NULL;
	ALLOC_ARRAY(q, BTNode*, cap);
	MEM_SIZE_T head = 0, tail = 0;
	q[tail++] = _this->root;

	WHILE(head < tail)
	{
		BTNode* curr = q[head++];

		IF(curr->left == NULL)
		{
			BTNode* n = NULL;
			ALLOC(n, BTNode);
			INITIALIZE_INSTANCE(BTNode, (*n)), _this->elementSize, (const void*)src, curr CALL;
			curr->left = n;
			_this->size++;
			FREE(q);
			RETURN;
		}
		ELSE
		{
			q[tail++] = curr->left;
		}
		END_IF;

		IF(curr->right == NULL)
		{
			BTNode* n = NULL;
			ALLOC(n, BTNode);
			INITIALIZE_INSTANCE(BTNode, (*n)), _this->elementSize, (const void*)src, curr CALL;
			curr->right = n;
			_this->size++;
			FREE(q);
			RETURN;
		}
		ELSE
		{
			q[tail++] = curr->right;
		}
		END_IF;
	}
	END_LOOP;

	FREE(q);
}
END_FUN

#define IMPL_INSERT_OF_TYPE(type)\
MEM_FUN_IMPL(GenericBinaryTree, insert_ ##type, type val) {\
	MFUN(_this, __insert_generic), (const void*)&val CALL;\
}\
END_FUN;

IMPL_INSERT_OF_TYPE(int);
/* add more types */

MEM_FUN_IMPL(GenericBinaryTree, __remove_generic, const void* key, bool* out_removed)
{
	*out_removed = false;
	IF(_this->size == 0)
	{
		RETURN;
	}
	END_IF;

	MEM_SIZE_T cap = _this->size + 2;
	BTNode** q = NULL;
	ALLOC_ARRAY(q, BTNode*, cap);
	MEM_SIZE_T head = 0, tail = 0;
	q[tail++] = _this->root;

	BTNode* target = NULL;
	BTNode* last = NULL;

	WHILE(head < tail)
	{
		BTNode* curr = q[head++];

		IF(target == NULL && memcmp(curr->value, key, _this->elementSize) == 0)
		{
			target = curr;
		}
		END_IF;

		last = curr;
		IF(curr->left != NULL)
		{
			q[tail++] = curr->left;
		}
		END_IF;

		IF(curr->right != NULL)
		{
			q[tail++] = curr->right;
		}
		END_IF;
	}
	END_LOOP;

	/* not found */
	IF(target == NULL)
	{
		FREE(q);
		RETURN;
	}
	END_IF;

	/* Single node tree case */
	IF(last == target && target->parent == NULL && target->left == NULL && target->right == NULL)
	{
		DESTROY(target);
		FREE(target);
		_this->root = NULL;
		_this->size = 0;
		*out_removed = true;
		FREE(q);
		RETURN;
	}
	END_IF;

	/* Copy the value of last to the target and detach last from its parent */
	memcpy(target->value, last->value, _this->elementSize);

	BTNode* parent = last->parent;
	IF(parent != NULL)
	{
		IF(parent->left == last)
		{
			parent->left = NULL;
		}
		ELSE
		{
			parent->right = NULL;
		}
		END_IF;
	}
	ELSE
	{
		_this->root = NULL;
	}
	END_IF;

	DESTROY(last);
	FREE(last);
	_this->size--;
	*out_removed = true;
	FREE(q);
}
END_FUN

#define IMPL_REMOVE_OF_TYPE(type)\
MEM_FUN_IMPL(GenericBinaryTree, remove_ ##type, type key, bool* out_removed) {\
	MFUN(_this, __remove_generic), (const void*)&key, out_removed CALL;\
}END_FUN;

IMPL_REMOVE_OF_TYPE(int);
/* add more types */

MEM_FUN_IMPL(GenericBinaryTree, __print_value, const void* p)
{
	IF(_this->BT_type == INT)
	{
		printf("%d ", *(const int*)p);
	}
	ELSE_IF(_this->BT_type == FLOAT)
	{
		printf("%f ", *(const float*)p);
	}
	ELSE_IF(_this->BT_type == CHAR)
	{
		printf("%c ", *(const char*)p);
	}
	ELSE_IF(_this->BT_type == OBJ_SPTR)
	{
		MFUN((objSPtr*)p, print) CALL;
		printf(" ");
	}
	ELSE /* RAW_BYTES */
	{
		const unsigned char* bytes = (const unsigned char*)p;
		FOR(MEM_SIZE_T i = 0; i < _this->elementSize; i++)
		{
			printf("%02X ", bytes[i]);
		}
		END_LOOP;
		printf(" ");
	}
	END_IF;
}
END_FUN

MEM_FUN_IMPL(GenericBinaryTree, traverse_pre)
{
	IF(_this->root == NULL)
	{
		printf("(empty)");
		RETURN;
	}
	END_IF;

	MEM_SIZE_T cap = (_this->size ? _this->size : 8);
	BTNode** stack = NULL;
	ALLOC_ARRAY(stack, BTNode*, cap);
	MEM_SIZE_T top = 0;

	stack[top++] = _this->root;

	WHILE(top > 0)
	{
		BTNode* curr = stack[--top];

		IF(curr && curr->value)
		{
			MFUN(_this, __print_value), (const void*)curr->value CALL;
		}
		END_IF;

		/* push right then left so left pops first */
		IF(curr && curr->right)
		{
			stack[top++] = curr->right;
		}
		END_IF;

		IF(curr && curr->left)
		{
			stack[top++] = curr->left;
		}
		END_IF;
	}
	END_LOOP;

	FREE(stack);
}
END_FUN

MEM_FUN_IMPL(GenericBinaryTree, traverse_in)
{
	IF(_this->root == NULL)
	{
		printf("(empty)");
		RETURN;
	}
	END_IF;

	BTInOrderIterator* it = NULL;
	BTInOrderIterator* end = NULL;

	MFUN(_this, begin_inorder), & it  CALL;
	MFUN(_this, end_inorder), & end CALL;

	/* while (it != end): */
	FOR(;;)
	{
		bool eq = false;
		MFUN(it, equals), (object*)end, & eq CALL;
		IF(eq)
		{
			BREAK;
		}
		END_IF;

		const void* p = NULL;
		MFUN(it, get_cref), & p CALL;
		IF(p != NULL)
		{
			MFUN(_this, __print_value), p CALL;
		}
		END_IF;

		MFUN(it, next) CALL;                         /* NEXT */
	}
	END_LOOP;

	MFUN(_this, it_destroy), it  CALL;
	MFUN(_this, it_destroy), end CALL;
}
END_FUN


MEM_FUN_IMPL(GenericBinaryTree, traverse_post)
{
	IF(_this->root == NULL)
	{
		printf("(empty)");
		RETURN;
	}
	END_IF;

	MEM_SIZE_T cap = (_this->size ? _this->size : 8);
	BTNode** stack1 = NULL;
	ALLOC_ARRAY(stack1, BTNode*, cap);
	BTNode** stack2 = NULL;
	ALLOC_ARRAY(stack2, BTNode*, cap);
	MEM_SIZE_T top1 = 0, top2 = 0;

	stack1[top1++] = _this->root;

	WHILE(top1 > 0)
	{
		BTNode* curr = stack1[--top1];
		stack2[top2++] = curr;
		IF(curr->left)
		{
			stack1[top1++] = curr->left;
		}
		END_IF;
		IF(curr->right != NULL)
		{
			stack1[top1++] = curr->right;
		}
		END_IF;
	}
	END_LOOP;

	WHILE(top2 > 0)
	{
		BTNode* curr = stack2[--top2];
		IF(curr && curr->value)
		{
			MFUN(_this, __print_value), (const void*)curr->value CALL;
		}
		END_IF;
	}
	END_LOOP;

	FREE(stack1);
	FREE(stack2);

}
END_FUN

MEM_FUN_IMPL(GenericBinaryTree, print, BT_VisitOrder order)
{
	IF(order != PRE && order != IN && order != POST)
	{
		THROW_MSG("Invalid visit order");
	}
	END_IF;

	IF(order == PRE)
	{
		MFUN(_this, traverse_pre) CALL;
	}
	ELSE_IF(order == IN)
	{
		MFUN(_this, traverse_in) CALL;
	}
	ELSE /* POST */
	{
		MFUN(_this, traverse_post) CALL;
	}
	END_IF;
	printf("\n");
}
END_FUN

MEM_FUN_IMPL(GenericBinaryTree, begin_inorder, Iterator** out_iter)
{
	BTInOrderIterator* it = NULL;
	ALLOC(it, BTInOrderIterator);
	INITIALIZE_INSTANCE(BTInOrderIterator, (*it)) CALL;
	MFUN(it, init_begin), _this CALL;
	*out_iter = (Iterator*)it;
}
END_FUN

MEM_FUN_IMPL(GenericBinaryTree, end_inorder, Iterator** out_iter)
{
	BTInOrderIterator* it = NULL;
	ALLOC(it, BTInOrderIterator);
	INITIALIZE_INSTANCE(BTInOrderIterator, (*it)) CALL;
	MFUN(it, init_end), _this CALL;
	*out_iter = (Iterator*)it;
}
END_FUN

MEM_FUN_IMPL(GenericBinaryTree, it_destroy, Iterator* it)
{
	BTInOrderIterator* BTit = (BTInOrderIterator*)it;
	DESTROY(BTit);
	FREE(BTit);
}
END_FUN


INIT_CLASS(GenericBinaryTree);
BIND(GenericBinaryTree, get_size);
BIND(GenericBinaryTree, is_empty);

BIND(GenericBinaryTree, __insert_generic);
BIND(GenericBinaryTree, insert_int);
/* add more types */

BIND(GenericBinaryTree, __remove_generic);
BIND(GenericBinaryTree, remove_int);
/* add more types */

BIND(GenericBinaryTree, print);
BIND(GenericBinaryTree, traverse_pre);
BIND(GenericBinaryTree, traverse_in);
BIND(GenericBinaryTree, traverse_post);
BIND(GenericBinaryTree, __print_value);

BIND(GenericBinaryTree, begin_inorder);
BIND(GenericBinaryTree, end_inorder);
BIND(GenericBinaryTree, it_destroy);
END_INIT_CLASS(GenericBinaryTree)

/* ============ In-order Iterator implementation ============ */

DEF_DERIVED_CTOR(BTInOrderIterator, Iterator) SUPER, ITER_BIDIRECTIONAL ME
{
	_this->owner = NULL;
	_this->current = NULL;
}
END_DERIVED_CTOR

DEF_DERIVED_DTOR(BTInOrderIterator, Iterator)
{
	_this->owner = NULL;
	_this->current = NULL;
}
END_DERIVED_DTOR

MEM_FUN_IMPL(BTInOrderIterator, init_begin, GenericBinaryTree* owner)
{
	_this->owner = owner;

	BTNode* cur = owner->root;

	WHILE(cur && cur->left)
	{
		cur = cur->left;
	}
	END_LOOP;

	_this->current = cur;
}
END_FUN

MEM_FUN_IMPL(BTInOrderIterator, init_end, GenericBinaryTree* owner)
{
	_this->owner = owner;
	_this->current = NULL;
}
END_FUN

FUN_OVERRIDE_IMPL(BTInOrderIterator, Iterator, equals, object* other, bool* out_equal)
{
	BTInOrderIterator* o = (BTInOrderIterator*)other;
	*out_equal = (_this->owner == o->owner) && (_this->current == o->current);
}
END_FUN

FUN_OVERRIDE_IMPL(BTInOrderIterator, Iterator, next)
{
	IF(_this->current == NULL)
	{
		RETURN;
	}
	END_IF;

	BTNode* n = _this->current;

	IF(n->right != NULL)
	{
		BTNode* cur = n->right;
		WHILE(cur && cur->left)
		{
			cur = cur->left;
		}
		END_LOOP;
		_this->current = cur;
		RETURN;
	}
	END_IF;

	BTNode* cur = n;
	BTNode* p = n->parent;

	WHILE(p && p->right == cur)
	{
		cur = p;
		p = p->parent;
	}
	END_LOOP;

	_this->current = p;
}
END_FUN

FUN_OVERRIDE_IMPL(BTInOrderIterator, Iterator, prev)
{
	IF(_this->owner == NULL)
	{
		RETURN;
	}
	END_IF;

	IF(_this->current == NULL)
	{
		BTNode* cur = _this->owner->root;
		WHILE(cur && cur->right)
		{
			cur = cur->right;
		}
		END_LOOP;
		_this->current = cur;
		RETURN;
	}
	END_IF;

	BTNode* n = _this->current;

	IF(n->left != NULL)
	{
		BTNode* cur = n->left;
		WHILE(cur && cur->right)
		{
			cur = cur->right;
		}
		END_LOOP;
		_this->current = cur;
		RETURN;
	}
	END_IF;

	BTNode* cur = n;
	BTNode* p = n->parent;

	WHILE(p && p->left == cur)
	{
		cur = p;
		p = p->parent;
	}
	END_LOOP;

	_this->current = p;
}
END_FUN

FUN_OVERRIDE_IMPL(BTInOrderIterator, Iterator, get_ref, void** out_ptr)
{
	IF(_this->current == NULL)
	{
		*out_ptr = NULL;
		RETURN;
	}
	END_IF;

	*out_ptr = (void*)(_this->current->value);
}
END_FUN

FUN_OVERRIDE_IMPL(BTInOrderIterator, Iterator, get_cref, const void** out_ptr)
{
	IF(_this->current == NULL)
	{
		*out_ptr = NULL;
		RETURN;
	}
	END_IF;

	*out_ptr = (const void*)(_this->current->value);
}
END_FUN

FUN_OVERRIDE_IMPL(BTInOrderIterator, Iterator, distance, object* other, ptrdiff_t* out_dist)
{
	*out_dist = 0;
	BTInOrderIterator* o = (BTInOrderIterator*)other;

	IF(_this->owner != o->owner)
	{
		RETURN;
	}
	END_IF;

	BTInOrderIterator itBegin;
	INITIALIZE_INSTANCE(BTInOrderIterator, itBegin) CALL;
	BTInOrderIterator itEnd;
	INITIALIZE_INSTANCE(BTInOrderIterator, itEnd) CALL;

	MFUN(&itBegin, init_begin), _this->owner CALL;
	MFUN(&itEnd, init_end), _this->owner CALL;

	ptrdiff_t idxThis = 0;
	ptrdiff_t idxOther = 0;
	bool foundThis = false;
	bool foundOther = false;

	WHILE(1)
	{
		bool atEnd = false;
		MFUN(&itBegin, equals), (object*)&itEnd, & atEnd CALL;
		IF(atEnd)
		{
			BREAK;
		}
		END_IF;


		IF(!foundThis)
		{
			bool eq1 = false;
			MFUN(&itBegin, equals), (object*)_this, & eq1 CALL;
			IF(eq1)
			{
				foundThis = true;
			}
			ELSE
			{
				idxThis++;
			}
			END_IF;
		}
		END_IF;

		IF(!foundOther)
		{
			bool eq2 = false;
			MFUN(&itBegin, equals), (object*)o, & eq2 CALL;
			IF(eq2)
			{
				foundOther = true;
			}
			ELSE
			{
				idxOther++;
			}
			END_IF;
		}
		END_IF;

		IF(foundThis&& foundOther)
		{
			BREAK;
		}
		END_IF;

		MFUN(&itBegin, next) CALL;
	}
	END_LOOP;

	*out_dist = idxOther - idxThis;
	DESTROY(&itBegin);
	DESTROY(&itEnd);
}
END_FUN

FUN_OVERRIDE_IMPL(BTInOrderIterator, Iterator, advance, ptrdiff_t n)
{
	IF(n > 0)
	{
		FOR(ptrdiff_t i = 0; i < n; i++)
		{
			MFUN(_this, next) CALL;
		}
		END_LOOP;
	}
	ELSE_IF(n < 0)
	{
		FOR(ptrdiff_t i = 0; i < -n; i++)
		{
			MFUN(_this, prev) CALL;
		}
		END_LOOP;
	}
	END_IF;
}
END_FUN

INIT_DERIVED_CLASS(BTInOrderIterator, Iterator);
BIND(BTInOrderIterator, init_begin);
BIND(BTInOrderIterator, init_end);
BIND_OVERIDE(BTInOrderIterator, Iterator, equals);
BIND_OVERIDE(BTInOrderIterator, Iterator, next);
BIND_OVERIDE(BTInOrderIterator, Iterator, prev);
BIND_OVERIDE(BTInOrderIterator, Iterator, get_ref);
BIND_OVERIDE(BTInOrderIterator, Iterator, get_cref);
BIND_OVERIDE(BTInOrderIterator, Iterator, distance);
BIND_OVERIDE(BTInOrderIterator, Iterator, advance);
END_INIT_CLASS(BTInOrderIterator)

#define IMPL_SPECIFIC_BT_TYPE_xTORS(type, TAG) \
  DEF_DERIVED_CTOR(BTree_##type, GenericBinaryTree) SUPER, sizeof(type), TAG ME {} END_DERIVED_CTOR \
  DEF_DERIVED_DTOR(BTree_##type, GenericBinaryTree) {} END_DERIVED_DTOR

#define IMPL_SPECIFIC_BT_TYPE_FUNCS(type) \
MEM_FUN_IMPL(BTree_##type, is_empty, bool* out){ FUN_BASE(_this, is_empty), out CALL; } END_FUN \
MEM_FUN_IMPL(BTree_##type, get_size, MEM_SIZE_T* out){ FUN_BASE(_this, get_size), out CALL; } END_FUN \
MEM_FUN_IMPL(BTree_##type, insert, type val){ FUN_BASE(_this, insert_##type), val CALL; } END_FUN \
MEM_FUN_IMPL(BTree_##type, remove, type key, bool* out_removed){ FUN_BASE(_this, remove_##type), key, out_removed CALL; } END_FUN \
MEM_FUN_IMPL(BTree_##type, print, BT_VisitOrder order){ FUN_BASE(_this, print), order CALL; } END_FUN \
MEM_FUN_IMPL(BTree_##type, traverse_pre){ FUN_BASE(_this, traverse_pre) CALL; } END_FUN \
MEM_FUN_IMPL(BTree_##type, traverse_in){ FUN_BASE(_this, traverse_in) CALL; } END_FUN \
MEM_FUN_IMPL(BTree_##type, traverse_post){ FUN_BASE(_this, traverse_post) CALL; } END_FUN \
MEM_FUN_IMPL(BTree_##type, begin_inorder, object** out_iter){ FUN_BASE(_this, begin_inorder), (Iterator**)out_iter CALL; } END_FUN \
MEM_FUN_IMPL(BTree_##type, end_inorder, object** out_iter){ FUN_BASE(_this, end_inorder), (Iterator**)out_iter CALL; } END_FUN \
MEM_FUN_IMPL(BTree_ ##type, it_destroy, Iterator* it) { FUN_BASE(_this, it_destroy), it CALL; } END_FUN; \
INIT_DERIVED_CLASS(BTree_##type, GenericBinaryTree); \
BIND(BTree_##type, is_empty); \
BIND(BTree_##type, get_size); \
BIND(BTree_##type, insert); \
BIND(BTree_##type, remove); \
BIND(BTree_##type, print); \
BIND(BTree_##type, traverse_pre); \
BIND(BTree_##type, traverse_in); \
BIND(BTree_##type, traverse_post); \
BIND(BTree_##type, begin_inorder); \
BIND(BTree_##type, end_inorder); \
BIND(BTree_ ##type, it_destroy);                                         \
END_INIT_CLASS(BTree_##type)

///////////////////////////////////////////////////////////////

IMPL_SPECIFIC_BT_TYPE_xTORS(int, INT);
IMPL_SPECIFIC_BT_TYPE_FUNCS(int);
/* add more types */