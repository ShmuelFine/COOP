#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct node_t
{
	int value;
	struct node_t* left;
	struct node_t* right;
	bool isActive;
} tree_node;

typedef struct node_tt
{
	int num;
	struct node_t* next;
} node;

typedef struct list_t
{
	node* head;
	int length;
	int* pointers_array;
	int size_arr;
} list;

//linked list
int getLenght(node* head);
node* getMiddle(node* head, int lenght);
node* searchIfExist(int num, node* head, int lenght);
node* _getPrev(node* head, node* ptr);
void insert(node* head, int num);

//binary tree
tree_node* init(int num);
tree_node* add(tree_node* head, int num);
void removeNode(tree_node* toRemove);
tree_node* getPlaceToAdd(tree_node* head, int num);
void free_tree(tree_node* tree);


