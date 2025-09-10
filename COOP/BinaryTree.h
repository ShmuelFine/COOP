#ifndef __COOP__BINARY_TREE__H_
#define __COOP__BINARY_TREE__H_


#include "COOP.h"
#include "Iterator.h"

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
BTNode* left;
BTNode* right;
BTNode* parent;
void* value;
END_DEF(BTNode);

FUNCTIONS(BTNode, MEM_SIZE_T elementSize, const void* src_bytes, BTNode* parent);
END_FUNCTIONS(BTNode);

/* ============ Inner In-Order Iterator ============ */

DEF_DERIVED_CLASS(BTInOrderIterator, Iterator);
void* owner;
BTNode* current;  
END_DEF_DERIVED(BTInOrderIterator);

DERIVED_FUNCTIONS(BTInOrderIterator, Iterator);
MEM_FUN_DECL(BTInOrderIterator, init_begin, void* owner);
MEM_FUN_DECL(BTInOrderIterator, init_end, void* owner);
FUN_OVERRIDE(Iterator, equals, Iterator* other, bool* out_equal);
FUN_OVERRIDE(Iterator, next);
FUN_OVERRIDE(Iterator, prev);
FUN_OVERRIDE(Iterator, get_ref, void** out_ptr);
FUN_OVERRIDE(Iterator, get_cref, const void** out_ptr);
FUN_OVERRIDE(Iterator, distance, Iterator* other, ptrdiff_t* out_dist);
FUN_OVERRIDE(Iterator, advance, ptrdiff_t n);
END_DERIVED_FUNCTIONS(BTInOrderIterator);

/* ====== GenericBinaryTree class ====== */

DEF_CLASS(GenericBinaryTree);
BTNode* root;
MEM_SIZE_T size;
MEM_SIZE_T elementSize;
BT_ElementType BT_type;
BTInOrderIterator begin_iter;
BTInOrderIterator end_iter;
END_DEF(GenericBinaryTree);

/* Callback function type for traversal */
typedef int (*BT_Action)(GenericBinaryTree* owner, const void* elem);

FUNCTIONS(GenericBinaryTree, MEM_SIZE_T elementSize, BT_ElementType BT_type);

MEM_FUN_DECL(GenericBinaryTree, is_empty, bool* out);
MEM_FUN_DECL(GenericBinaryTree, get_size, MEM_SIZE_T* out);

MEM_FUN_DECL(GenericBinaryTree, __insert_generic, const void* src);
MEM_FUN_DECL(GenericBinaryTree, insert_int, int val);
/* add more types */

MEM_FUN_DECL(GenericBinaryTree, __remove_generic, const void* key, bool* out_removed);
MEM_FUN_DECL(GenericBinaryTree, remove_int, int key, bool* out_removed);
/* add more types */

MEM_FUN_DECL(GenericBinaryTree, print, BT_VisitOrder order);

MEM_FUN_DECL(GenericBinaryTree, traverse_pre, BT_Action action);
MEM_FUN_DECL(GenericBinaryTree, traverse_in, BT_Action action);
MEM_FUN_DECL(GenericBinaryTree, traverse_post, BT_Action action);

MEM_FUN_DECL(GenericBinaryTree, __print_value, const void* p);

END_FUNCTIONS(GenericBinaryTree);


///////////////////////////////////////////////////////////////

#define DECLARE_SPECIFIC_BT_TYPE(type)                                \
DEF_DERIVED_CLASS(BTree_ ##type, GenericBinaryTree);                  \
END_DEF_DERIVED(BTree_ ##type);                                       \
																	  \
DERIVED_FUNCTIONS(BTree_ ##type, GenericBinaryTree);                  \
MEM_FUN_DECL(BTree_ ##type, is_empty, bool* out);                     \
MEM_FUN_DECL(BTree_ ##type, get_size, MEM_SIZE_T* out);               \
MEM_FUN_DECL(BTree_ ##type, insert, type val);                        \
MEM_FUN_DECL(BTree_ ##type, remove, type key, bool* out_removed);     \
MEM_FUN_DECL(BTree_ ##type, print, BT_VisitOrder order);              \
MEM_FUN_DECL(BTree_ ##type, traverse_pre, BT_Action action);                            \
MEM_FUN_DECL(BTree_ ##type, traverse_in, BT_Action action);                             \
MEM_FUN_DECL(BTree_ ##type, traverse_post, BT_Action action);                           \
END_DERIVED_FUNCTIONS(BTree_ ##type);

////////////////////////////////////////////////

DECLARE_SPECIFIC_BT_TYPE(int);
/* add more types */

/* פונקציית עזר להפעלת __print_value כ־BT_Action */
FUN_DECL(BT_action_print_value, GenericBinaryTree* owner, const void* p);

#endif 