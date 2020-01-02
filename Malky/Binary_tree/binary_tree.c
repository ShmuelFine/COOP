#include "coop.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

tree_node* getPlaceToAdd(tree_node* head, int num)
{
	tree_node* ptr = head;
	while (ptr != NULL)
	{
		if (ptr->value == num)
			return ptr;
		if (num > ptr->value)
		{
			if (ptr->right == NULL)
				return ptr;
			ptr = ptr->right;
		}
		else
		{
			if (ptr->left == NULL)
				return ptr;
			ptr = ptr->left;
		}
	}
	return ptr;
}


/*node* getPrevToRemove(node* head, node* toRemove)
{
	node* ptr = head;
	while (ptr != NULL)
	{
		if (ptr->right == toRemove || ptr->left == toRemove)
			return ptr;
		if (toRemove->value > ptr->value)
		{
			ptr = ptr->right;
		}
		else
		{
			ptr = ptr->left;
		}
	}
	return ptr;
}*/

//init tree
tree_node* init(int num)
{
	tree_node* _new = (tree_node*)malloc(sizeof(tree_node));
	_new->value = num;
	_new->right = NULL;
	_new->left = NULL;
	_new->isActive = true;
	return _new;
}

//add node
tree_node* add(tree_node* head, int num)
{
	tree_node* placeToAdd = getPlaceToAdd(head, num);
	if (placeToAdd->value != num)
	{
		tree_node* toAdd = init(num);
		if (placeToAdd->value < num)
			placeToAdd->right = toAdd;
		else
			placeToAdd->left = toAdd;
		return toAdd;
	}
	else
	{
		placeToAdd->isActive = true;
		return placeToAdd;
	}
}

//remove node
void removeNode(tree_node* toRemove)
{
	toRemove->isActive = false;
}

//free tree
void free_tree(tree_node ** tree)
{
	if (*tree != NULL)
	{
		free_tree(&((*tree)->right));
		free_tree(&((*tree)->left ));
		free(*tree);
		*tree = NULL;
	}
}

int main()
{
	//////TEST INIT
	// Arrange
	int num1 = 5;

	// Act
	tree_node* root = init(num1);

	// Assert
	if (root && root->value == num1 && root->right == NULL && root->left == NULL)
		printf("Init-OK\n");


	//////TEST ADD
	// Arrange
	int num2 = 7;

	// Act
	tree_node* root2 = init(num1);
	add(root2, num2);


	// Assert
	if (root2->right->value == num2)
		printf("Add-OK\n");


	//////TEST REMOVE
	// Arrange
	int num3 = 9;

	// Act
	tree_node* root3 = init(num1);
	tree_node* nd2 = add(root3, num2);
	tree_node* nd3 = add(root3, num3);
	removeNode(nd2);

	// Assert
	if (!nd2->isActive)
		printf("Remove-OK\n");

	//TEST-FREE
	tree_node* t = init(87);
	add(t, 50);
	add(t, 60);
	add(t, 90);
	add(t, 88);
	add(t, 95);
	add(t, 93);
	add(t, 100);

	free_tree(&t);
	//why it's not equal null??
	if (t==NULL)
		printf("Free-OK!!");
}