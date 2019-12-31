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




int getLenght(node* head);
node* getMiddle(node* head, int lenght);
node* searchIfExist(int num, node* head, int lenght);
node* find_previous(node* head, node* ptr);
void insert(node* head, int num);