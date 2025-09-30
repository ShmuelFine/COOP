#ifndef BT_PURE_H
#define BT_PURE_H

#include <stddef.h>
#include <stdbool.h>

/* pointers to generic functions */
typedef int  (*BT_CompareFn)(const void* a, const void* b);
typedef void (*BT_PrintFn)(const void* value, size_t element_size);

typedef enum {
    BT_PRE = 0,
    BT_IN = 1,
    BT_POST = 2
} BT_VisitOrder;

typedef struct BTNode {
    void* value;
    struct BTNode* left;
    struct BTNode* right;
    struct BTNode* parent;
} BTNode;

typedef struct {
    BTNode* root;
    size_t size;
    size_t element_size;
    BT_CompareFn compare_fn;   /* if NULL: compare in memcmp(element_size) */
    BT_PrintFn   print_fn;     /* if NULL: print in hex by element_size */
} BT_Tree;

void init(BT_Tree* tree, size_t element_size, BT_CompareFn compare_fn, BT_PrintFn print_fn);
void destroy(BT_Tree* tree);

bool is_empty(const BT_Tree* tree);
size_t get_size(const BT_Tree* tree);

void insert(BT_Tree* tree, const void* value);
bool bt_remove(BT_Tree* tree, const void* key);

void traverse_pre(BT_Tree* owner, void (*action)(BT_Tree*, const void*));
void traverse_in(BT_Tree* owner, void (*action)(BT_Tree*, const void*));
void traverse_post(BT_Tree* owner, void (*action)(BT_Tree*, const void*));

void print(BT_Tree* tree, BT_VisitOrder order);

void action_print_value(BT_Tree* owner, const void* value);

#endif
