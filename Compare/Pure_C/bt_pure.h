#ifndef BT_PURE_H
#define BT_PURE_H

#include <stddef.h>
#include <stdbool.h>

/* Visit order for print/traversal */
typedef enum {
    PRE = 0,
    IN = 1,
    POST = 2
} BT_VisitOrder;

typedef void (*BT_Action)(int value);

typedef struct BTNode {
    int value;
    struct BTNode* left;
    struct BTNode* right;
    struct BTNode* parent;
} BTNode;

typedef struct {
    BTNode* root;
    size_t  size;   /* number of nodes in the tree */
} GenericBinaryTree;


void   bt_init(GenericBinaryTree* tree);
void   bt_destroy(GenericBinaryTree* tree);

bool   is_empty(const GenericBinaryTree* tree);
size_t get_size(const GenericBinaryTree* tree);

void   insert(GenericBinaryTree* tree, int value);
bool   bt_remove(GenericBinaryTree* tree, int key);

void   traverse_pre(GenericBinaryTree* tree, BT_Action action);
void   traverse_in(GenericBinaryTree* tree, BT_Action action);
void   traverse_post(GenericBinaryTree* tree, BT_Action action);

void   print(GenericBinaryTree* tree, BT_VisitOrder order);

/* ===== Internal helpers ===== */
void   __find_node_recursive(GenericBinaryTree* tree, BTNode* current, int key, BTNode** out_node_found);
void   __get_parent_for_position(GenericBinaryTree* tree, size_t position_index, BTNode** out_parent_node, int* out_lsb_dir);

#endif
