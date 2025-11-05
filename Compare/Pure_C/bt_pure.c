#include "bt_pure.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


static BTNode* __node_create(int value, BTNode* parent)
{
	BTNode* node = (BTNode*)malloc(sizeof(BTNode));
	if (!node)
		return NULL;

	node->value = value;
	node->left = NULL;
	node->right = NULL;
	node->parent = parent;
	return node;
}

static void __node_destroy(BTNode* node)
{
	free(node);
}

void bt_init(GenericBinaryTree* tree)
{
	if (!tree)
		return;
	tree->root = NULL;
	tree->size = 0U;
}

/* Post-order iterative destruction using two stacks (safe for deep trees) */
void bt_destroy(GenericBinaryTree* tree)
{
	if (!tree || !tree->root) {
		if (tree) tree->size = 0U; return;
	}

	size_t capacity = (tree->size == 0U ? 1U : tree->size);
	BTNode** stack1 = (BTNode**)malloc(sizeof(BTNode*) * capacity);
	if (!stack1) {
		return;
	}
	BTNode** stack2 = (BTNode**)malloc(sizeof(BTNode*) * capacity);
	if (!stack2) {
		free(stack1);
		return;
	}

	size_t top1 = 0U, top2 = 0U;
	stack1[top1++] = tree->root;

	while (top1 > 0U) {
		BTNode* current = stack1[--top1];
		stack2[top2++] = current;
		if (current->left)
			stack1[top1++] = current->left;
		if (current->right)
			stack1[top1++] = current->right;
	}
	while (top2 > 0U) {
		__node_destroy(stack2[--top2]);
	}
	free(stack1);
	free(stack2);

	tree->root = NULL;
	tree->size = 0U;
}

bool is_empty(const GenericBinaryTree* tree)
{
	return (!tree) || (tree->size == 0U);
}

size_t get_size(const GenericBinaryTree* tree)
{
	return (!tree) ? 0U : tree->size;
}


void insert(GenericBinaryTree* tree, int value)
{
	if (!tree)
		return;

	if (tree->root == NULL) {
		BTNode* root = __node_create(value, NULL);
		if (!root)
			return;
		tree->root = root;
		tree->size = 1U;
		return;
	}

	size_t next_position = tree->size + 1U;

	/* Find MSB index for next_position */
	size_t msb_index = 0U, tmp = next_position;
	while ((tmp >>= 1U) != 0U) {
		msb_index++; 
	}

	BTNode* parent = tree->root;
	for (size_t bit = msb_index -1; bit > 0U; --bit) {
		int direction = (int)((next_position >> bit) & 1U);
		parent = (direction == 0) ? parent->left : parent->right;
		assert(parent && "Structure inconsistent with recorded size");
		if (!parent)
			return;
	}

	int lsb_direction = (int)(next_position & 1U);
	BTNode* new_node = __node_create(value, parent);
	if (!new_node)
		return;

	if (lsb_direction == 0)
		parent->left = new_node;
	else
		parent->right = new_node;

	tree->size++;
}

void __find_node_recursive(GenericBinaryTree* tree, BTNode* current, int key, BTNode** out_node_found)
{
	(void)tree;
	if (out_node_found)
		*out_node_found = NULL;
	if (!current || !out_node_found)
		return;

	if (current->value == key) {
		*out_node_found = current;
		return;
	}

	if (current->left) {
		__find_node_recursive(tree, current->left, key, out_node_found);
		if (*out_node_found) 
			return;
	}
	if (current->right) {
		__find_node_recursive(tree, current->right, key, out_node_found);
		if (*out_node_found)
			return;
	}
}

void __get_parent_for_position(GenericBinaryTree* tree, size_t position_index, BTNode** out_parent_node, int* out_lsb_dir)
{
	if (out_parent_node)
		*out_parent_node = NULL;
	if (!tree || !tree->root || position_index <= 1U || !out_parent_node) 
		return;

	size_t msb_index = 0U, tmp = position_index;
	while ((tmp >>= 1U) != 0U) {
		msb_index++; 
	}

	BTNode* parent = tree->root;
	for (size_t bit = msb_index - 1; bit > 0U; --bit) {
		int direction = (int)((position_index >> bit) & 1U);
		parent = (direction == 0) ? parent->left : parent->right;
		assert(parent && "Structure inconsistent with recorded size");
		if (!parent)
			return;
	}

	if (out_lsb_dir)
		*out_lsb_dir = (int)(position_index & 1U);
	*out_parent_node = parent;
}

bool bt_remove(GenericBinaryTree* tree, int key)
{
	if (!tree || tree->size == 0U)
		return false;

	/* Single-node tree */
	if (tree->size == 1U) {
		if (tree->root->value != key)
			return false;
		__node_destroy(tree->root);
		tree->root = NULL;
		tree->size = 0U;
		return true;
	}

	/* 1) Find target node (DFS) */
	BTNode* target_node = NULL;
	__find_node_recursive(tree, tree->root, key, &target_node);
	if (!target_node) 
		return false;

	/* 2) Find parent + direction for the "last" node (position == size) */
	size_t last_position = tree->size;
	BTNode* parent_of_last = NULL;
	int last_dir_lsb = 0;
	__get_parent_for_position(tree, last_position, &parent_of_last, &last_dir_lsb);
	assert(parent_of_last);

	BTNode* last_node = (last_dir_lsb == 0) ? parent_of_last->left : parent_of_last->right;
	assert(last_node);

	/* 3) Detach and destroy last; copy value if needed */
	if (target_node == last_node) {
		if (last_dir_lsb == 0)
			parent_of_last->left = NULL;
		else
			parent_of_last->right = NULL;
		__node_destroy(last_node);
		tree->size--;
		return true;
	}

	/* Copy last value into target, then remove the last node */
	target_node->value = last_node->value;
	if (last_dir_lsb == 0)
		parent_of_last->left = NULL;
	else                
		parent_of_last->right = NULL;
	__node_destroy(last_node);
	tree->size--;
	return true;
}

static void __traverse_pre_recursive(BTNode* current, BT_Action action)
{
	if (!current)
		return;
	action(current->value);
	__traverse_pre_recursive(current->left, action);
	__traverse_pre_recursive(current->right, action);
}

void traverse_pre(GenericBinaryTree* tree, BT_Action action)
{
	if (!tree || !action || !tree->root)
		return;
	__traverse_pre_recursive(tree->root, action);
}

static void __traverse_post_recursive(BTNode* current, BT_Action action)
{
	if (!current)
		return;
	__traverse_post_recursive(current->left, action);
	__traverse_post_recursive(current->right, action);
	action(current->value);
}

void traverse_post(GenericBinaryTree* tree, BT_Action action)
{
	if (!tree || !action || !tree->root)
		return;
	__traverse_post_recursive(tree->root, action);
}

/* In-order: iterative */
void traverse_in(GenericBinaryTree* tree, BT_Action action)
{
	if (!tree || !action || !tree->root)
		return;

	size_t capacity = (tree->size == 0U ? 1U : tree->size);
	BTNode** stack = (BTNode**)malloc(sizeof(BTNode*) * capacity);
	if (!stack)
		return;

	size_t top = 0U;

	for (BTNode* current = tree->root; (current != NULL) || (top > 0U); ) {

		/* Descend left as far as possible, pushing ancestors */
		while (current != NULL) {
			stack[top++] = current;
			current = current->left;
		}

		/* Visit the node at the top */
		BTNode* visit = stack[--top];
		action(visit->value);

		/* Move to the right subtree; loop header condition will re-check */
		current = visit->right;
	}

	free(stack);
}

static void __print_action(int value)
{
	printf("%d ", value);
}

void print(GenericBinaryTree* tree, BT_VisitOrder order)
{
	if (!tree) return;

	switch (order) {
	case PRE:  traverse_pre(tree, __print_action);
		break;
	case IN:   traverse_in(tree, __print_action);
		break;
	case POST: traverse_post(tree, __print_action);
		break;
	default: return;
	}
	printf("\n");
}
