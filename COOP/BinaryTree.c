#include "BinaryTree.h"
#include "DynamicMemoryManagement.h"
#include "SharedObjPtr.h"
#include "InMemoryCache.h"

FUN_IMPL(BT_action_print_value, GenericBinaryTree *owner, const void *value)
{
	MFUN(owner, __print_value), value CALL;
}
END_FUN


/* ====== BTNode implementation ====== */

DEF_CTOR(BTNode, MEM_SIZE_T elementSize, const void *src_bytes, BTNode *parent)
{
	_this->left = NULL;
	_this->right = NULL;
	_this->parent = NULL;
	_this->value = NULL;

	IF(elementSize > (MEM_SIZE_T)0)
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
	IF(_this->value)
	{ 
		FREE(_this->value); 
		_this->value = NULL;
	} 
	END_IF;
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

	THROW_MSG_UNLESS(_this->size > 0, "size must be positive and accurate");

	MEM_SIZE_T capacity = _this->size;
	BTNode **stack1 = NULL;
	ALLOC_ARRAY(stack1, BTNode*, capacity);
	BTNode **stack2 = NULL;
	ALLOC_ARRAY(stack2, BTNode*, capacity);
	MEM_SIZE_T top1 = 0, top2 = 0;

	stack1[top1++] = _this->root;

	/* Step 1: Fill stack2 in reverse order */
	WHILE(top1 > 0)
	{
		BTNode *current = stack1[--top1];
		stack2[top2++] = current;

		IF(current->left)
		{
			stack1[top1++] = current->left;
		}
		END_IF;
		IF(current->right)
		{
			stack1[top1++] = current->right;
		}
		END_IF;
	}
	END_LOOP;

	/* Step 2: Pop from stack2 — this is where the actual release is done */
	WHILE(top2 > 0)
	{
		BTNode *current = stack2[--top2];

		IF(_this->BT_type == OBJ_SPTR && current && current->value)
		{
			DESTROY((objSPtr*)current->value);
		}
		END_IF;

		DELETE(current);
	}
	END_LOOP;

	FREE(stack1);
	FREE(stack2);

	_this->root = NULL;
	_this->size = 0;
}
END_DTOR

MEM_FUN_IMPL(GenericBinaryTree, is_empty, bool *out)
{
	*out = (_this->size == (MEM_SIZE_T)0);
}
END_FUN

MEM_FUN_IMPL(GenericBinaryTree, get_size, MEM_SIZE_T *out)
{
	*out = _this->size;
}
END_FUN

/* The next insertion position is derived from (size + 1). Each bit of this position
encodes a left (0) or right (1) step from the root to the parent of the new node.*/
MEM_FUN_IMPL(GenericBinaryTree, __insert_generic, const void *src)
{
	//printf("sizeof(MEM_SIZE_T) = %zu\n", sizeof(MEM_SIZE_T));

	/* Empty tree: create a root and return */
	IF(_this->root == NULL)
	{
		BTNode* newNode = NULL;
		ALLOC(newNode, BTNode);
		INITIALIZE_INSTANCE(BTNode, (*newNode)), _this->elementSize, (const void*)src, (BTNode*)NULL CALL;
		_this->root = newNode;
		_this->size = (MEM_SIZE_T)1;
		RETURN;
	}
	END_IF;

	MEM_SIZE_T nextPositionIndex = _this->size + (MEM_SIZE_T)1;
	MEM_SIZE_T mostSignificantBitIndex = 0U;
	MEM_SIZE_T positionCopy = nextPositionIndex;
	WHILE((positionCopy >>= 1U) != 0U)
	{
		mostSignificantBitIndex++;
	}
	END_LOOP;

	BTNode* parentNode = _this->root;

	FOR(MEM_SIZE_T bitIndex = mostSignificantBitIndex; bitIndex-- > 1U; )
	{
		int directionBit = (int)((nextPositionIndex >> bitIndex) & (MEM_SIZE_T)1U);
		parentNode = (directionBit == 0) ? parentNode->left : parentNode->right;
		THROW_MSG_UNLESS(parentNode != NULL, "BinaryTree structure is inconsistent with its recorded size");
	}
	END_LOOP;

	int lastDirectionBit = (int)(nextPositionIndex & (MEM_SIZE_T)1U);

	BTNode* newNode = NULL;
	ALLOC(newNode, BTNode);
	INITIALIZE_INSTANCE(BTNode, (*newNode)), _this->elementSize, (const void*)src, parentNode CALL;

	IF(lastDirectionBit == 0)
	{
		parentNode->left = newNode;
	}
	ELSE
	{
		parentNode->right = newNode;
	}
	END_IF;

	_this->size++;
}
END_FUN

#define IMPL_INSERT_OF_TYPE(type)\
MEM_FUN_IMPL(GenericBinaryTree, insert_ ##type, type value) {\
	MFUN(_this, __insert_generic), (const void*)&value CALL;\
}\
END_FUN;

IMPL_INSERT_OF_TYPE(int);
IMPL_INSERT_OF_TYPE(char);
IMPL_INSERT_OF_TYPE(float);
IMPL_INSERT_OF_TYPE(objSPtr);

/* ===== Helpers functions for remove ===== */
MEM_FUN_IMPL(GenericBinaryTree, __find_node_recursive, BTNode* currentNode, const void* key, BTNode** outNodeFound)
{
	*outNodeFound = NULL;

	IF(currentNode == NULL)
	{
		RETURN;
	}
	END_IF;

	/* compare current node */
	bool isMatch = false;
	IF(_this->BT_type == OBJ_SPTR)
	{
		bool isEquals = false;
		MFUN((objSPtr*)currentNode->value, equals), (objSPtr*)key, &isEquals CALL;
		isMatch = isEquals;
	}
	ELSE
	{
		isMatch = (memcmp(currentNode->value, key, _this->elementSize) == 0);
	}
	END_IF;

	IF(isMatch)
	{
		*outNodeFound = currentNode;
		RETURN;
	}
	END_IF;

	/* recurse left */
	BTNode* foundLeft = NULL;
	MFUN(_this, __find_node_recursive), currentNode->left, key, &foundLeft CALL;
	IF(foundLeft != NULL)
	{
		*outNodeFound = foundLeft;
		RETURN;
	}
	END_IF;

	/* recurse right */
	BTNode* foundRight = NULL;
	MFUN(_this, __find_node_recursive), currentNode->right, key, &foundRight CALL;
	IF(foundRight != NULL)
	{
		*outNodeFound = foundRight;
		RETURN;
	}
	END_IF;
}
END_FUN

MEM_FUN_IMPL(GenericBinaryTree, __get_parent_for_position, MEM_SIZE_T positionIndex, BTNode** outParentNode, int* outLeastSignificantBit)
{
	*outParentNode = NULL;

	IF((_this->root == NULL) || (positionIndex <= (MEM_SIZE_T)1))
	{
		RETURN;
	}
	END_IF;

	MEM_SIZE_T mostSignificantBitIndex = (MEM_SIZE_T)0;
	MEM_SIZE_T tmpIndex = positionIndex;
	WHILE((tmpIndex >>= 1U) != 0U)
	{
		mostSignificantBitIndex++;
	}
	END_LOOP;

	BTNode* parentNode = _this->root;

	FOR(MEM_SIZE_T bitIndex = mostSignificantBitIndex; bitIndex-- > 1U; )
	{
		int directionBit = (int)((positionIndex >> bitIndex) & (MEM_SIZE_T)1U);
		parentNode = (directionBit == 0) ? parentNode->left : parentNode->right;
		THROW_MSG_UNLESS(parentNode != NULL, "BinaryTree structure is inconsistent with its recorded size");
	}
	END_LOOP;

	IF(outLeastSignificantBit != NULL)
	{
		*outLeastSignificantBit = (int)(positionIndex & (MEM_SIZE_T)1U);
	}
	END_IF;

	*outParentNode = parentNode;
}
END_FUN

MEM_FUN_IMPL(GenericBinaryTree, __remove_generic, const void *key, bool *outRemoved)
{
	*outRemoved = false;

	IF(_this->size == (MEM_SIZE_T)0)
	{
		RETURN;
	}
	END_IF;

	/* Single-node tree: compare and delete root if it matches */
	IF(_this->size == (MEM_SIZE_T)1)
	{
		bool isMatch = false;

		IF(_this->BT_type == OBJ_SPTR)
		{
			bool isEquals = false;
			MFUN((objSPtr*)_this->root->value, equals), (objSPtr*)key, &isEquals CALL;
			isMatch = isEquals;
		}
		ELSE
		{
			isMatch = (memcmp(_this->root->value, key, _this->elementSize) == 0);
		}
		END_IF;

		IF(!isMatch)
		{
			RETURN;
		}
		END_IF;

		DELETE(_this->root);
		_this->root = NULL;
		_this->size = (MEM_SIZE_T)0;
		*outRemoved = true;
		RETURN;
	}
	END_IF;

	/* Step 1: recursive DFS search for the target */
	BTNode* targetNode = NULL;
	MFUN(_this, __find_node_recursive), _this->root, key, & targetNode CALL;
	IF(targetNode == NULL)
	{
		RETURN;
	}
	END_IF;

	/* Step 2: locate the last node */
	MEM_SIZE_T positionIndex = _this->size;
	BTNode* parentOfLast = NULL;
	int leastSignificantBit = 0;
	MFUN(_this, __get_parent_for_position), positionIndex, & parentOfLast, & leastSignificantBit CALL;
	THROW_MSG_UNLESS(parentOfLast != NULL, "BinaryTree structure is inconsistent with its recorded size");

	BTNode* lastNode = (leastSignificantBit == 0) ? parentOfLast->left : parentOfLast->right;
	THROW_MSG_UNLESS(lastNode != NULL, "BinaryTree structure is inconsistent: last node is missing");

	/* Step 3a: if the target is already the last node */
	IF(targetNode == lastNode)
	{
		IF(leastSignificantBit == 0)
		{
			parentOfLast->left = NULL;
		}
		ELSE
		{
			parentOfLast->right = NULL;
		}
		END_IF;

		DELETE(lastNode);
		_this->size--;
		*outRemoved = true;
		RETURN;
	}
	END_IF;

	/* Step 3b: copy last->value into target->value, then detach + delete last */
	memcpy(targetNode->value, lastNode->value, (size_t)_this->elementSize);

	IF(leastSignificantBit == 0)
	{
		parentOfLast->left = NULL;
	}
	ELSE
	{
		parentOfLast->right = NULL;
	}
	END_IF;

	DELETE(lastNode);
	_this->size--;
	*outRemoved = true;
}
END_FUN

#define IMPL_REMOVE_OF_TYPE(type)\
MEM_FUN_IMPL(GenericBinaryTree, remove_ ##type, type key, bool *out_removed) {\
	MFUN(_this, __remove_generic), (const void*)&key, out_removed CALL;\
}END_FUN;

IMPL_REMOVE_OF_TYPE(int);
IMPL_REMOVE_OF_TYPE(char);
IMPL_REMOVE_OF_TYPE(float);
IMPL_REMOVE_OF_TYPE(objSPtr);

MEM_FUN_IMPL(GenericBinaryTree, __print_value, const void *value)
{
	IF(_this->BT_type == INT)
	{
		printf("%d ", *(const int*)value);
	}
	ELSE_IF(_this->BT_type == FLOAT)
	{
		printf("%f ", *(const float*)value);
	}
	ELSE_IF(_this->BT_type == CHAR)
	{
		printf("%c ", *(const char*)value);
	}
	ELSE_IF(_this->BT_type == OBJ_SPTR)
	{
		MFUN((objSPtr*)value, print) CALL;
		printf(" ");
	}
	ELSE /* RAW_BYTES */
	{
		const unsigned char *bytes = (const unsigned char*)value;
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

MEM_FUN_IMPL(GenericBinaryTree, __traverse_pre_recursive, BTNode* currentNode, BT_Action action)
{
	IF(currentNode == NULL)
	{
		RETURN;
	} 
	END_IF;

	IF(currentNode->value != NULL)
	{ 
		action(_this, currentNode->value); 
	}
	END_IF;

	MFUN(_this, __traverse_pre_recursive), currentNode->left, action CALL;
	MFUN(_this, __traverse_pre_recursive), currentNode->right, action CALL;
}
END_FUN

MEM_FUN_IMPL(GenericBinaryTree, __traverse_post_recursive, BTNode* currentNode, BT_Action action)
{
	IF(currentNode == NULL) 
	{
		RETURN;
	} 
	END_IF;

	MFUN(_this, __traverse_post_recursive), currentNode->left, action CALL;
	MFUN(_this, __traverse_post_recursive), currentNode->right, action CALL;

	IF(currentNode->value != NULL) 
	{
		action(_this, currentNode->value); 
	}
	END_IF;
}
END_FUN


MEM_FUN_IMPL(GenericBinaryTree, traverse_pre, BT_Action action)
{
	IF((_this == NULL) || (action == NULL) || (_this->root == NULL))
	{
		RETURN;
	}
	END_IF;

	MFUN(_this, __traverse_pre_recursive), _this->root, action CALL;
}
END_FUN

MEM_FUN_IMPL(GenericBinaryTree, traverse_in, BT_Action action)
{
	IF(_this->root == NULL)
	{
		RETURN;
	}
	END_IF;

	ITER_FOR(void*, value, _this)
	{
		action(_this, (const void*)&value);
	}
	END_ITER_FOR;

}
END_FUN


MEM_FUN_IMPL(GenericBinaryTree, traverse_post, BT_Action action)
{
	IF((_this == NULL) || (action == NULL) || (_this->root == NULL))
	{
		RETURN;
	}
	END_IF;

	MFUN(_this, __traverse_post_recursive), _this->root, action CALL;
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
		MFUN(_this, traverse_pre), BT_action_print_value  CALL;
	}
	ELSE_IF(order == IN)
	{
		MFUN(_this, traverse_in), BT_action_print_value  CALL;
	}
	ELSE /* POST */
	{
		MFUN(_this, traverse_post), BT_action_print_value  CALL;
	}
	END_IF;
	printf("\n");
}
END_FUN

MEM_FUN_IMPL(GenericBinaryTree, begin, Iterator** out_it)
{
	THROW_MSG_UNLESS(out_it, "out_it is NULL");

	BTInOrderIterator* bt_iter = NULL;
	ALLOC_ARRAY(bt_iter, BTInOrderIterator, 1);
	INITIALIZE_INSTANCE(BTInOrderIterator, (*bt_iter)), _this CALL;
	MFUN(bt_iter, __find_begin) CALL;

	*out_it = (Iterator*)bt_iter;
}
END_FUN

MEM_FUN_IMPL(GenericBinaryTree, end, Iterator** out_it)
{
	THROW_MSG_UNLESS(out_it, "out_it is NULL");

	BTInOrderIterator* bt_iter = NULL;
	ALLOC_ARRAY(bt_iter, BTInOrderIterator, 1);
	INITIALIZE_INSTANCE(BTInOrderIterator, (*bt_iter)), _this CALL;
	bt_iter->current = NULL;

	*out_it = (Iterator*)bt_iter;
}
END_FUN


INIT_CLASS(GenericBinaryTree)
BIND(GenericBinaryTree, get_size);
BIND(GenericBinaryTree, is_empty);

BIND(GenericBinaryTree, __insert_generic);
BIND(GenericBinaryTree, insert_int);
BIND(GenericBinaryTree, insert_char);
BIND(GenericBinaryTree, insert_float);
BIND(GenericBinaryTree, insert_objSPtr);

BIND(GenericBinaryTree, __find_node_recursive);
BIND(GenericBinaryTree, __get_parent_for_position);

BIND(GenericBinaryTree, __remove_generic);
BIND(GenericBinaryTree, remove_int);
BIND(GenericBinaryTree, remove_char);
BIND(GenericBinaryTree, remove_float);
BIND(GenericBinaryTree, remove_objSPtr);

BIND(GenericBinaryTree, print);
BIND(GenericBinaryTree, __traverse_pre_recursive);
BIND(GenericBinaryTree, __traverse_post_recursive);
BIND(GenericBinaryTree, traverse_pre);
BIND(GenericBinaryTree, traverse_in);
BIND(GenericBinaryTree, traverse_post);
BIND(GenericBinaryTree, __print_value);

BIND(GenericBinaryTree, begin);
BIND(GenericBinaryTree, end);
END_INIT_CLASS(GenericBinaryTree)

/* ============ In-order Iterator implementation ============ */

DEF_DERIVED_CTOR(BTInOrderIterator, Iterator, void* container_ptr) SUPER, ITER_BIDIRECTIONAL, container_ptr ME
{
	_this->current = NULL;
}
END_DERIVED_CTOR

DEF_DERIVED_DTOR(BTInOrderIterator, Iterator)
{
	_this->current = NULL;
}
END_DERIVED_DTOR

FUN_OVERRIDE_IMPL(BTInOrderIterator, Iterator, equals, Iterator *other, bool *out_equal)
{
	*out_equal = 0;
	IF(other)
	{
		bool is_base_equals = false;
		FUN_BASE(_this, equals), other, & is_base_equals CALL;
		*out_equal = is_base_equals && (_this->current == ((BTInOrderIterator*)other)->current);
	}
	END_IF;
}
END_FUN

FUN_OVERRIDE_IMPL(BTInOrderIterator, Iterator, next)
{
	IF(_this->current == NULL)
	{
		RETURN;
	}
	END_IF;

	BTNode *current_node = _this->current;

	IF(current_node->right != NULL)
	{
		BTNode *successor_candidate = current_node->right;
		WHILE(successor_candidate && successor_candidate->left)
		{
			successor_candidate = successor_candidate->left;
		}
		END_LOOP;
		_this->current = successor_candidate;
		RETURN;
	}
	END_IF;

	BTNode *child = current_node;
	BTNode *parent_node = current_node->parent;

	WHILE(parent_node && parent_node->right == child)
	{
		child = parent_node;
		parent_node = parent_node->parent;
	}
	END_LOOP;

	_this->current = parent_node;
}
END_FUN

FUN_OVERRIDE_IMPL(BTInOrderIterator, Iterator, prev)
{

	IF(_this->current == NULL)
	{
		GenericBinaryTree* owner = (GenericBinaryTree*)_this->_base.container_ptr;
		BTNode* rightmost_node = owner ? owner->root : NULL;
		WHILE(rightmost_node && rightmost_node->right)
		{
			rightmost_node = rightmost_node->right;
		}
		END_LOOP;
		_this->current = rightmost_node;
		RETURN;
	}
	END_IF;

	BTNode* current_node = _this->current;

	IF(current_node->left != NULL)
	{
		BTNode* predecessor_candidate = current_node->left;
		WHILE(predecessor_candidate && predecessor_candidate->right)
		{
			predecessor_candidate = predecessor_candidate->right;
		}
		END_LOOP;
		_this->current = predecessor_candidate;
		RETURN;
	}
	END_IF;

	BTNode* child = current_node;
	BTNode* parent_node = current_node->parent;

	WHILE(parent_node && parent_node->left == child)
	{
		child = parent_node;
		parent_node = parent_node->parent;
	}
	END_LOOP;

	_this->current = parent_node;
}
END_FUN


FUN_OVERRIDE_IMPL(BTInOrderIterator, Iterator, get_ref, void **out_ptr)
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

FUN_OVERRIDE_IMPL(BTInOrderIterator, Iterator, get_cref, const void **out_ptr)
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

FUN_OVERRIDE_IMPL(BTInOrderIterator, Iterator, distance, Iterator *other, ptrdiff_t *out_dist)
{
	THROW_MSG_UNLESS(other != NULL, "Null other iterator");

	BTInOrderIterator* other_iter = (BTInOrderIterator*)other;
	THROW_MSG_UNLESS(other_iter->_base.container_ptr == _this->_base.container_ptr, "Iterators of different trees");

	BTInOrderIterator iter_begin;
	INITIALIZE_INSTANCE(BTInOrderIterator, iter_begin), _this->_base.container_ptr CALL;
	iter_begin._base.container_ptr = _this->_base.container_ptr;
	MFUN(&iter_begin, __find_begin) CALL;

	BTInOrderIterator iter_end;
	INITIALIZE_INSTANCE(BTInOrderIterator, iter_end), _this->_base.container_ptr CALL;
	iter_end._base.container_ptr = _this->_base.container_ptr;
	iter_end.current = NULL;

	ptrdiff_t index_this = 0;
	ptrdiff_t index_other = 0;
	bool found_this = false;
	bool found_other = false;

	WHILE(1)
	{
		bool at_end = false;
		MFUN(&iter_begin, equals), (Iterator*)&iter_end, &at_end CALL;
		IF(at_end)
		{
			BREAK;
		}
		END_IF;


		IF(!found_this)
		{
			bool is_equals1 = false;
			MFUN(&iter_begin, equals), (Iterator*)_this, & is_equals1 CALL;
			IF(is_equals1)
			{
				found_this = true;
			}
			ELSE
			{
				index_this++;
			}
			END_IF;
		}
		END_IF;

		IF(!found_other)
		{
			bool is_equals2 = false;
			MFUN(&iter_begin, equals), (Iterator*)((BTInOrderIterator*)other), &is_equals2 CALL;
			IF(is_equals2)
			{
				found_other = true;
			}
			ELSE
			{
				index_other++;
			}
			END_IF;
		}
		END_IF;

		IF(found_this && found_other)
		{
			BREAK;
		}
		END_IF;

		MFUN(&iter_begin, next) CALL;
	}
	END_LOOP;

	*out_dist = index_other - index_this;
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

MEM_FUN_IMPL(BTInOrderIterator, __find_begin)
{
	THROW_MSG_UNLESS(_this->_base.container_ptr != NULL, "Iterator not bound");
	GenericBinaryTree* owner = (GenericBinaryTree*)_this->_base.container_ptr;

	BTNode *current_node = owner ? owner->root : NULL;

	WHILE(current_node && current_node->left)
	{
		current_node = current_node->left;
	}
	END_LOOP;

	_this->current = current_node;
}
END_FUN

INIT_DERIVED_CLASS(BTInOrderIterator, Iterator);
BIND_OVERIDE(BTInOrderIterator, Iterator, equals);
BIND_OVERIDE(BTInOrderIterator, Iterator, next);
BIND_OVERIDE(BTInOrderIterator, Iterator, prev);
BIND_OVERIDE(BTInOrderIterator, Iterator, get_ref);
BIND_OVERIDE(BTInOrderIterator, Iterator, get_cref);
BIND_OVERIDE(BTInOrderIterator, Iterator, distance);
BIND_OVERIDE(BTInOrderIterator, Iterator, advance);
BIND(BTInOrderIterator, __find_begin);
END_INIT_CLASS(BTInOrderIterator)

#define IMPL_SPECIFIC_BT_TYPE_xTORS(type, type_name) \
  DEF_DERIVED_CTOR(BTree_##type, GenericBinaryTree) SUPER, sizeof(type), type_name ME {} END_DERIVED_CTOR \
  DEF_DERIVED_DTOR(BTree_##type, GenericBinaryTree) {} END_DERIVED_DTOR

#define IMPL_SPECIFIC_BT_TYPE_FUNCS(type) \
MEM_FUN_IMPL(BTree_##type, is_empty, bool *out){ FUN_BASE(_this, is_empty), out CALL; } END_FUN \
MEM_FUN_IMPL(BTree_##type, get_size, MEM_SIZE_T *out){ FUN_BASE(_this, get_size), out CALL; } END_FUN \
MEM_FUN_IMPL(BTree_##type, insert, type value){ FUN_BASE(_this, insert_##type), value CALL; } END_FUN \
MEM_FUN_IMPL(BTree_##type, remove, type key, bool *out_removed){ FUN_BASE(_this, remove_##type), key, out_removed CALL; } END_FUN \
MEM_FUN_IMPL(BTree_##type, print, BT_VisitOrder order){ FUN_BASE(_this, print), order CALL; } END_FUN \
MEM_FUN_IMPL(BTree_##type, traverse_pre, BT_Action action){ FUN_BASE(_this, traverse_pre), action CALL; } END_FUN \
MEM_FUN_IMPL(BTree_##type, traverse_in, BT_Action action){ FUN_BASE(_this, traverse_in), action CALL; } END_FUN \
MEM_FUN_IMPL(BTree_##type, traverse_post, BT_Action action){ FUN_BASE(_this, traverse_post), action CALL; } END_FUN \
MEM_FUN_IMPL(BTree_##type, begin, Iterator **out_it);{FUN_BASE(_this, begin), out_it CALL; } END_FUN \
MEM_FUN_IMPL(BTree_##type, end, Iterator **out_it);{FUN_BASE(_this, end), out_it CALL; } END_FUN \
INIT_DERIVED_CLASS(BTree_##type, GenericBinaryTree); \
BIND(BTree_##type, is_empty); \
BIND(BTree_##type, get_size); \
BIND(BTree_##type, insert); \
BIND(BTree_##type, remove); \
BIND(BTree_##type, print); \
BIND(BTree_##type, traverse_pre); \
BIND(BTree_##type, traverse_in); \
BIND(BTree_##type, traverse_post); \
BIND(BTree_##type, begin); \
BIND(BTree_##type, end); \
END_INIT_CLASS(BTree_##type)

///////////////////////////////////////////////////////////////

IMPL_SPECIFIC_BT_TYPE_xTORS(int, INT);
IMPL_SPECIFIC_BT_TYPE_FUNCS(int);
IMPL_SPECIFIC_BT_TYPE_xTORS(char, CHAR);
IMPL_SPECIFIC_BT_TYPE_FUNCS(char);
IMPL_SPECIFIC_BT_TYPE_xTORS(float, FLOAT);
IMPL_SPECIFIC_BT_TYPE_FUNCS(float);
IMPL_SPECIFIC_BT_TYPE_xTORS(objSPtr, OBJ_SPTR);
IMPL_SPECIFIC_BT_TYPE_FUNCS(objSPtr);