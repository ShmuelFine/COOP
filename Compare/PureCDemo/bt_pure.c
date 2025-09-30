#include "bt_pure.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* ---------- Internal helpers ---------- */
static BTNode* bt_node_create(size_t element_size, const void* source_bytes, BTNode* parent_node) {
    BTNode* node = (BTNode*)malloc(sizeof(BTNode));
    if (node == NULL)
        return NULL;

    node->left = NULL;
    node->right = NULL;
    node->parent = parent_node;
    node->value = NULL;

    if (element_size > 0U) {
        node->value = malloc(element_size);
        if (node->value == NULL) {
            free(node);
            return NULL;
        }
        if (source_bytes != NULL) {
            memcpy(node->value, source_bytes, element_size);
        }
        else {
            memset(node->value, 0, element_size);
        }
    }
    return node;
}

static void bt_node_destroy(BTNode* node) {
    if (node == NULL)
        return;
    free(node->value);
    free(node);
}

/* Default comparison when there is no compare_fn */
static int bt_compare_default(const void* a, const void* b, size_t length) {
    return memcmp(a, b, length);
}

/* Simple queue for BFS */
typedef struct {
    BTNode** buffer;
    size_t   head;
    size_t   tail;
    size_t   capacity;
} BT_NodeQueue;

static BT_NodeQueue queue_create(size_t capacity) {
    BT_NodeQueue queue;
    queue.buffer = (BTNode**)malloc(sizeof(BTNode*) * (capacity == 0U ? 1U : capacity));
    queue.head = 0U;
    queue.tail = 0U;
    queue.capacity = (queue.buffer == NULL) ? 0U : (capacity == 0U ? 1U : capacity);
    return queue;
}
static void queue_destroy(BT_NodeQueue* queue) {
    if (queue == NULL)
        return;
    free(queue->buffer);
    queue->buffer = NULL;
    queue->capacity = 0U;
    queue->head = 0U;
    queue->tail = 0U;
}
static int  queue_is_empty(const BT_NodeQueue* queue) {
    return (queue->head >= queue->tail);
}
static int  queue_push(BT_NodeQueue* queue, BTNode* node) {
    if (queue->tail >= queue->capacity)
        return 0;
    queue->buffer[queue->tail++] = node;
    return 1;
}
static BTNode* queue_pop(BT_NodeQueue* queue) {
    if (queue_is_empty(queue)) return NULL;
    return queue->buffer[queue->head++];
}

/* Finds both the target for deletion (target) by key and the "last" in the BFS scan (last_node) */
static BTNode* bt_find_target_and_last(BT_Tree* tree, const void* key, BTNode** out_last_node) {
    if ((tree == NULL) || (tree->root == NULL)) {
        if (out_last_node != NULL)
            *out_last_node = NULL;
        return NULL;
    }

    BT_NodeQueue queue = queue_create(tree->size == 0U ? 1U : tree->size);
    if (queue.buffer == NULL) {
        if (out_last_node != NULL)
            *out_last_node = NULL;
        return NULL;
    }
    (void)queue_push(&queue, tree->root);

    BTNode* target_node = NULL;
    BTNode* last_node = NULL;

    while (!queue_is_empty(&queue)) {
        BTNode* current_node = queue_pop(&queue);
        last_node = current_node;

        int compare_result = (tree->compare_fn != NULL)
            ? tree->compare_fn(current_node->value, key)
            : bt_compare_default(current_node->value, key, tree->element_size);

        if ((target_node == NULL) && (compare_result == 0)) {
            target_node = current_node;
        }

        if (current_node->left != NULL)
            (void)queue_push(&queue, current_node->left);
        if (current_node->right != NULL)
            (void)queue_push(&queue, current_node->right);
    }

    queue_destroy(&queue);
    if (out_last_node != NULL)
        *out_last_node = last_node;
    return target_node;
}

static void traverse_in_internal(BT_Tree* tree, void (*action)(BT_Tree*, const void*)) {
    if ((tree == NULL) || (tree->root == NULL))
        return;

    BTNode* current_node = tree->root;
    BTNode** node_stack = (BTNode**)malloc(sizeof(BTNode*) * (tree->size == 0U ? 1U : tree->size));
    if (node_stack == NULL)
        return;

    size_t top_index = 0U;

    while ((current_node != NULL) || (top_index > 0U)) {
        while (current_node != NULL) {
            node_stack[top_index++] = current_node;
            current_node = current_node->left;
        }
        BTNode* visit_node = node_stack[--top_index];
        if (visit_node->value != NULL)
            action(tree, visit_node->value);
        current_node = visit_node->right;
    }

    free(node_stack);
}

static void traverse_pre_internal(BT_Tree* tree, void (*action)(BT_Tree*, const void*)) {
    if ((tree == NULL) || (tree->root == NULL))
        return;

    BTNode** node_stack = (BTNode**)malloc(sizeof(BTNode*) * (tree->size == 0U ? 1U : tree->size));
    if (node_stack == NULL)
        return;

    size_t top_index = 0U;
    node_stack[top_index++] = tree->root;

    while (top_index > 0U) {
        BTNode* visit_node = node_stack[--top_index];
        if (visit_node->value != NULL)
            action(tree, visit_node->value);
        if (visit_node->right != NULL)
            node_stack[top_index++] = visit_node->right;
        if (visit_node->left != NULL)
            node_stack[top_index++] = visit_node->left;
    }

    free(node_stack);
}

static void traverse_post_internal(BT_Tree* tree, void (*action)(BT_Tree*, const void*)) {
    if ((tree == NULL) || (tree->root == NULL))
        return;

    BTNode** stack_first = (BTNode**)malloc(sizeof(BTNode*) * (tree->size == 0U ? 1U : tree->size));
    BTNode** stack_second = (BTNode**)malloc(sizeof(BTNode*) * (tree->size == 0U ? 1U : tree->size));
    if ((stack_first == NULL) || (stack_second == NULL)) {
        free(stack_first);
        free(stack_second);
        return;
    }

    size_t top_first = 0U;
    size_t top_second = 0U;

    stack_first[top_first++] = tree->root;

    while (top_first > 0U) {
        BTNode* node = stack_first[--top_first];
        stack_second[top_second++] = node;
        if (node->left != NULL)
            stack_first[top_first++] = node->left;
        if (node->right != NULL)
            stack_first[top_first++] = node->right;
    }

    while (top_second > 0U) {
        BTNode* visit_node = stack_second[--top_second];
        if (visit_node->value != NULL)
            action(tree, visit_node->value);
    }

    free(stack_first);
    free(stack_second);
}

/* ---------- API ---------- */

void init(BT_Tree* tree, size_t element_size, BT_CompareFn compare_fn, BT_PrintFn print_fn) {
    assert(tree != NULL);
    tree->root = NULL;
    tree->size = 0U;
    tree->element_size = element_size;
    tree->compare_fn = compare_fn;
    tree->print_fn = print_fn;
}

void destroy(BT_Tree* tree) {
    if ((tree == NULL) || (tree->root == NULL)) {
        if (tree != NULL)
            tree->size = 0U;
        return;
    }

    /* Clearing a full tree in iteration (postorder with two stacks) */
    BTNode** stack_first = (BTNode**)malloc(sizeof(BTNode*) * (tree->size == 0U ? 1U : tree->size));
    BTNode** stack_second = (BTNode**)malloc(sizeof(BTNode*) * (tree->size == 0U ? 1U : tree->size));
    if ((stack_first == NULL) || (stack_second == NULL)) {
        free(stack_first);
        free(stack_second);
        return;
    }

    size_t top_first = 0U;
    size_t top_second = 0U;

    stack_first[top_first++] = tree->root;

    while (top_first > 0U) {
        BTNode* node = stack_first[--top_first];
        stack_second[top_second++] = node;
        if (node->left != NULL)
            stack_first[top_first++] = node->left;
        if (node->right != NULL)
            stack_first[top_first++] = node->right;
    }

    while (top_second > 0U) {
        BTNode* node = stack_second[--top_second];
        bt_node_destroy(node);
    }

    free(stack_first);
    free(stack_second);

    tree->root = NULL;
    tree->size = 0U;
}

bool is_empty(const BT_Tree* tree) {
    return (tree == NULL) || (tree->size == 0U);
}

size_t get_size(const BT_Tree* tree) {
    return (tree == NULL) ? 0U : tree->size;
}

void insert(BT_Tree* tree, const void* value) {
    if (tree == NULL) return;

    if (tree->root == NULL) {
        BTNode* new_node = bt_node_create(tree->element_size, value, NULL);
        if (new_node == NULL) return;
        tree->root = new_node;
        tree->size = 1U;
        return;
    }

    BT_NodeQueue queue = queue_create(tree->size + 1U);
    if (queue.buffer == NULL)
        return;

    (void)queue_push(&queue, tree->root);

    while (!queue_is_empty(&queue)) {
        BTNode* current_node = queue_pop(&queue);

        if (current_node->left == NULL) {
            BTNode* child_node = bt_node_create(tree->element_size, value, current_node);
            if (child_node == NULL){
                queue_destroy(&queue); 
                return;
            }
            current_node->left = child_node;
            tree->size++;
            queue_destroy(&queue);
            return;
        }
        else {
            (void)queue_push(&queue, current_node->left);
        }

        if (current_node->right == NULL) {
            BTNode* child_node = bt_node_create(tree->element_size, value, current_node);
            if (child_node == NULL) {
                queue_destroy(&queue);
                return;
            }
            current_node->right = child_node;
            tree->size++;
            queue_destroy(&queue);
            return;
        }
        else {
            (void)queue_push(&queue, current_node->right);
        }
    }

    queue_destroy(&queue);
}

bool bt_remove(BT_Tree* tree, const void* key) {
    if ((tree == NULL) || (tree->size == 0U))
        return false;

    BTNode* last_node = NULL;
    BTNode* target_node = bt_find_target_and_last(tree, key, &last_node);
    if ((target_node == NULL) || (last_node == NULL))
        return false;

    /* Single tree case (one node) */
    if ((last_node == target_node) && (target_node->parent == NULL) && (target_node->left == NULL) && (target_node->right == NULL)) {
        bt_node_destroy(target_node);
        tree->root = NULL;
        tree->size = 0U;
        return true;
    }

    memcpy(target_node->value, last_node->value, tree->element_size);

    /* Detach last_node from its parent and destroy it */
    BTNode* parent_node = last_node->parent;
    if (parent_node != NULL) {
        if (parent_node->left == last_node) {
            parent_node->left = NULL;
        }
        else {
            parent_node->right = NULL;
        }
    }
    else {
        tree->root = NULL;
    }

    bt_node_destroy(last_node);
    tree->size--;
    return true;
}

void traverse_pre(BT_Tree* owner, void (*action)(BT_Tree*, const void*)) {
    if ((owner == NULL) || (action == NULL))
        return;
    traverse_pre_internal(owner, action);
}
void traverse_in(BT_Tree* owner, void (*action)(BT_Tree*, const void*)) {
    if ((owner == NULL) || (action == NULL))
        return;
    traverse_in_internal(owner, action);
}
void traverse_post(BT_Tree* owner, void (*action)(BT_Tree*, const void*)) {
    if ((owner == NULL) || (action == NULL))
        return;
    traverse_post_internal(owner, action);
}

void action_print_value(BT_Tree* owner, const void* value) {
    if ((owner == NULL) || (value == NULL))
        return;

    if (owner->print_fn != NULL) {
        owner->print_fn(value, owner->element_size);
        return;
    }

    const unsigned char* bytes = (const unsigned char*)value;
    for (size_t i = 0U; i < owner->element_size; ++i) {
        printf("%02X ", bytes[i]);
    }
}

void print(BT_Tree* tree, BT_VisitOrder order) {
    if (tree == NULL)
        return;
    switch (order) {
    case BT_PRE:  traverse_pre(tree, action_print_value);
        break;
    case BT_IN:   traverse_in(tree, action_print_value);
        break;
    case BT_POST: traverse_post(tree, action_print_value);
        break;
    default: return;
    }
    printf("\n");
}
