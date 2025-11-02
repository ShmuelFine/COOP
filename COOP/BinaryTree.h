#ifndef __COOP__BINARY_TREE__H_
#define __COOP__BINARY_TREE__H_


#include "COOP.h"
#include "Iterator.h"
#include "SharedObjPtr.h"

/* ====== Element kind (type-tag) for printing ====== */
typedef enum {
	INT,
	FLOAT,
	CHAR,
	OBJ_SPTR,
	RAW_BYTES   /* default: hex dump of elemSize bytes */
} BT_ElementType;

/* ====== Visit order for single print() API ====== */
typedef enum {
	PRE = 0,
	IN = 1,
	POST = 2
} BT_VisitOrder;


/* ====== BTNode class  ======== */

DEF_CLASS(BTNode);
void *value;
BTNode *left;
BTNode *right;
BTNode *parent;
END_DEF(BTNode);

FUNCTIONS(BTNode, MEM_SIZE_T elementSize, const void *src_bytes, BTNode *parent);
END_FUNCTIONS(BTNode);

/* ============ Inner In-Order Iterator ============ */

DEF_DERIVED_CLASS(BTInOrderIterator, Iterator);
BTNode *current;  
END_DEF_DERIVED(BTInOrderIterator);

DERIVED_FUNCTIONS(BTInOrderIterator, Iterator, void* container_ptr);
FUN_OVERRIDE(Iterator, equals, Iterator *other, bool *out_equal);
FUN_OVERRIDE(Iterator, next);
FUN_OVERRIDE(Iterator, prev);
FUN_OVERRIDE(Iterator, get_ref, void **out_ptr);
FUN_OVERRIDE(Iterator, get_cref, const void **out_ptr);
FUN_OVERRIDE(Iterator, distance, Iterator *other, ptrdiff_t *out_dist);
FUN_OVERRIDE(Iterator, advance, ptrdiff_t n);
MEM_FUN_DECL(BTInOrderIterator, __find_begin); 
END_DERIVED_FUNCTIONS(BTInOrderIterator);

/* ====== GenericBinaryTree class ====== */

DEF_CLASS(GenericBinaryTree);
BTNode *root;
MEM_SIZE_T size;
MEM_SIZE_T elementSize;
BT_ElementType BT_type;
END_DEF(GenericBinaryTree);

/* Callback function type for traversal */
typedef void (*BT_Action)(GenericBinaryTree *owner, const void *value);

FUNCTIONS(GenericBinaryTree, MEM_SIZE_T elementSize, BT_ElementType BT_type);

MEM_FUN_DECL(GenericBinaryTree, is_empty, bool *out);
MEM_FUN_DECL(GenericBinaryTree, get_size, MEM_SIZE_T *out);

MEM_FUN_DECL(GenericBinaryTree, __insert_generic, const void *src);
MEM_FUN_DECL(GenericBinaryTree, insert_int, int value);
MEM_FUN_DECL(GenericBinaryTree, insert_char, char value);
MEM_FUN_DECL(GenericBinaryTree, insert_float, float value);
MEM_FUN_DECL(GenericBinaryTree, insert_objSPtr, objSPtr value);

MEM_FUN_DECL(GenericBinaryTree, __find_node_recursive, BTNode* currentNode, const void* key, BTNode** outNodeFound);
MEM_FUN_DECL(GenericBinaryTree, __get_parent_for_position, MEM_SIZE_T positionIndex, BTNode** outParentNode, int* outLeastSignificantBit);

MEM_FUN_DECL(GenericBinaryTree, __remove_generic, const void *key, bool *out_removed);
MEM_FUN_DECL(GenericBinaryTree, remove_int, int key, bool *out_removed);
MEM_FUN_DECL(GenericBinaryTree, remove_char, char key, bool *out_removed);
MEM_FUN_DECL(GenericBinaryTree, remove_float, float key, bool *out_removed);
MEM_FUN_DECL(GenericBinaryTree, remove_objSPtr, objSPtr key, bool *out_removed);

MEM_FUN_DECL(GenericBinaryTree, print, BT_VisitOrder order);

MEM_FUN_DECL(GenericBinaryTree, __traverse_pre_recursive, BTNode* currentNode, BT_Action action);
MEM_FUN_DECL(GenericBinaryTree, __traverse_post_recursive, BTNode* currentNode, BT_Action action);
MEM_FUN_DECL(GenericBinaryTree, traverse_pre, BT_Action action);
MEM_FUN_DECL(GenericBinaryTree, traverse_in, BT_Action action);
MEM_FUN_DECL(GenericBinaryTree, traverse_post, BT_Action action);

MEM_FUN_DECL(GenericBinaryTree, __print_value, const void *value);

MEM_FUN_DECL(GenericBinaryTree, begin, Iterator **out_it);
MEM_FUN_DECL(GenericBinaryTree, end, Iterator **out_it);

END_FUNCTIONS(GenericBinaryTree);


///////////////////////////////////////////////////////////////

#define DECLARE_SPECIFIC_BT_TYPE(type)                                \
DEF_DERIVED_CLASS(BTree_ ##type, GenericBinaryTree);                  \
END_DEF_DERIVED(BTree_ ##type);                                       \
																	  \
DERIVED_FUNCTIONS(BTree_ ##type, GenericBinaryTree);                  \
MEM_FUN_DECL(BTree_ ##type, is_empty, bool *out);                     \
MEM_FUN_DECL(BTree_ ##type, get_size, MEM_SIZE_T *out);               \
MEM_FUN_DECL(BTree_ ##type, insert, type value);                      \
MEM_FUN_DECL(BTree_ ##type, remove, type key, bool *out_removed);     \
MEM_FUN_DECL(BTree_ ##type, print, BT_VisitOrder order);              \
MEM_FUN_DECL(BTree_ ##type, traverse_pre, BT_Action action);          \
MEM_FUN_DECL(BTree_ ##type, traverse_in, BT_Action action);           \
MEM_FUN_DECL(BTree_ ##type, traverse_post, BT_Action action);         \
MEM_FUN_DECL(BTree_ ##type, begin, Iterator **out_it);                \
MEM_FUN_DECL(BTree_ ##type, end, Iterator **out_it);                  \
END_DERIVED_FUNCTIONS(BTree_ ##type);

////////////////////////////////////////////////

DECLARE_SPECIFIC_BT_TYPE(int);
DECLARE_SPECIFIC_BT_TYPE(char);
DECLARE_SPECIFIC_BT_TYPE(float);
DECLARE_SPECIFIC_BT_TYPE(objSPtr);

/* Helper function to run __print_value as BT_Action */
FUN_DECL(BT_action_print_value, GenericBinaryTree *owner, const void *value);

#endif 