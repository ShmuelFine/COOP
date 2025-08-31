#ifndef __COOP__BINARY_TREE__H_
#define __COOP__BINARY_TREE__H_


#include "COOP.h"
#include "Iterator.h"

DEF_CLASS(GenericBinaryTree);
object* root;        /* BTNode */
MEM_SIZE_T size;
MEM_SIZE_T elementSize;
END_DEF(GenericBinaryTree);

FUNCTIONS(GenericBinaryTree, MEM_SIZE_T elementSize);

MEM_FUN_DECL(GenericBinaryTree, is_empty, bool* out);
MEM_FUN_DECL(GenericBinaryTree, get_size, MEM_SIZE_T* out);

MEM_FUN_DECL(GenericBinaryTree, __insert_generic, const void* src);
MEM_FUN_DECL(GenericBinaryTree, insert_int, int val);
/* add more types */

MEM_FUN_DECL(GenericBinaryTree, __remove_generic, const void* key, bool* out_removed);
MEM_FUN_DECL(GenericBinaryTree, remove_int, int key, bool* out_removed);
/* add more types */

MEM_FUN_DECL(GenericBinaryTree, __print_pre_generic);
MEM_FUN_DECL(GenericBinaryTree, print_pre_int);
/* add more types */

MEM_FUN_DECL(GenericBinaryTree, __print_in_generic);
MEM_FUN_DECL(GenericBinaryTree, print_in_int);
/* add more types */

MEM_FUN_DECL(GenericBinaryTree, __print_post_generic);
MEM_FUN_DECL(GenericBinaryTree, print_post_int);
/* add more types */

MEM_FUN_DECL(GenericBinaryTree, begin_inorder, object** out_iter);
MEM_FUN_DECL(GenericBinaryTree, end_inorder, object** out_iter);

END_FUNCTIONS(GenericBinaryTree);

///////////////////////////////////////////////////////////////

/* ============ Inner In-Order Iterator ============ */

DEF_DERIVED_CLASS(BTInOrderIterator, Iterator);
GenericBinaryTree* owner;
object* current;  /* BTNode* */
END_DEF_DERIVED(BTInOrderIterator);

DERIVED_FUNCTIONS(BTInOrderIterator, Iterator);
MEM_FUN_DECL(BTInOrderIterator, init_begin, GenericBinaryTree* owner);
MEM_FUN_DECL(BTInOrderIterator, init_end, GenericBinaryTree* owner);

/* override functions */
MEM_FUN_DECL(BTInOrderIterator, equals, object* other, bool* out_equal);
MEM_FUN_DECL(BTInOrderIterator, next);
MEM_FUN_DECL(BTInOrderIterator, prev);
MEM_FUN_DECL(BTInOrderIterator, get_ref, void** out_ptr);
MEM_FUN_DECL(BTInOrderIterator, get_cref, const void** out_ptr);
MEM_FUN_DECL(BTInOrderIterator, distance, object* other, ptrdiff_t* out_dist);
MEM_FUN_DECL(BTInOrderIterator, advance, ptrdiff_t n);
END_DERIVED_FUNCTIONS(BTInOrderIterator);

///////////////////////////////////////////////////////////////

#define DECLARE_SPECIFIC_BT_TYPE(type)                                \
DEF_DERIVED_CLASS(BTree_ ##type, GenericBinaryTree);                  \
END_DEF_DERIVED(BTree_ ##type);                                       \
																	  \
DERIVED_FUNCTIONS(BTree_ ##type, GenericBinaryTree);                  \
MEM_FUN_DECL(BTree_ ##type, insert, type val);                        \
MEM_FUN_DECL(BTree_ ##type, remove, type key, bool* out_removed);     \
MEM_FUN_DECL(BTree_ ##type, print_in);                                \
MEM_FUN_DECL(BTree_ ##type, print_pre);                               \
MEM_FUN_DECL(BTree_ ##type, print_post);                              \
MEM_FUN_DECL(BTree_ ##type, begin_inorder, object** out_iter);        \
MEM_FUN_DECL(BTree_ ##type, end_inorder  , object** out_iter);        \
END_DERIVED_FUNCTIONS(BTree_ ##type);

////////////////////////////////////////////////

DECLARE_SPECIFIC_BT_TYPE(int);
/* add more types */

#endif 