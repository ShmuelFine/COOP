#include "coop.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

//static list myList;
//get length of list
int getLenght(node* head)
{
	int count = 0; // Initialize count  
	node* current = head; // Initialize current

	while (current )
	{
		count++;
		current = current->next;
	}
	return count;
}

//get middle node
node* getMiddle(node* head, int lenght)
{
	node* ptr = head;
	for (int i = 0; i < lenght / 2;i++)
	{
		ptr = ptr->next;
	}
	return ptr;
}

node* _getPrev(node* head, node* ptr)
{
	node* h = head;
	while (h->next != ptr)
		h = h->next;
	return h;
}

//search if exist - return the exist node or the previous
node* searchIfExist(node* head,int num)
{
	int length = getLenght(head);
	node* ptr = head;
	int len = length;
	node* check = getMiddle(head, len);
	node* prev;
	while (len>2)
	{
		if (check->num == num)
			return check;
		if (check->num > num)
			check=getMiddle(ptr, len/2);
		else
		{
			if(len%2==0)
				check=getMiddle(check->next, (len/2)-1);
			else
				check=getMiddle(check->next, len / 2);
			ptr = check->next;
		}
		len /= 2;
	}
	if (check->num == num || check->num < num)
		return check;
	else
	{
		if (check == head)
			return check;
		return _getPrev(head, check);
	}
}

node* list_init(int num)
{
	node* _new=(node*)malloc(sizeof(node));
	_new->num = num;
	_new->next = NULL;
	
	return _new;
}

node* list_add(node* head, int num)
{
	node* check = searchIfExist(head, num);// check -> placeToInsert
	node* _new = list_init(num); // _new -> toAdd
	node* ptr = check; // not needed
	_new->next = ptr->next;
	ptr->next = _new;
	return ptr;
}

void simple_add(node* head, int num)
{
	node* _new = list_init(num);
	node* ptr = head;
	while (ptr->next != NULL)
	{
		ptr = ptr->next;
	}
	ptr->next = _new;
}

void remove____(node* head)
{
	if (head != NULL)
	{
		remove____(head->next);
		free(head);
		head = NULL;
	}
}


int add__(int a, int b)
{
	int c = a + b;
	//printf("Got %d", c);
	return c;
}

bool checkEQ(node* l1, node* l2)
{
	node* p1 = l1;
	node* p2 = l2;
	while (p1 && p2)
	{
		if (p1->num != p2->num)
			return 0;
		p1 = p1->next;
		p2 = p2->next;
	}
	return 1;
}


//int main()
//{
//	//TEST INIT A LIST
//	// Arrange
//	int num = 5;
//	node* list= (node*)malloc(sizeof(node));
//	list->num = num;
//	list->next = NULL;
//
//	// Act
//	node* head = list_init(num);
//	// Assert
//	//if (safe_malloc((node*)malloc(sizeof(node))))
//	if(getLenght(head)==getLenght(list))
//		printf("Inint-OK\n");
//
//	/*
//	// TEST (simple)ADD NEW LINK TO EMPTY LIST
//	//Arrange
//	node* list2 = (node*)malloc(sizeof(node));
//	list2->num = num;
//	list2->next = NULL;
//	node* n2 = init(3);
//
//	list2->next = n2;
//	//Act
//	node* head2 = init(num);
//	simple_add(head2, 3);
//	//Assert
//	if (getLenght(head2) == getLenght(list2))
//		printf("OK");*/
//
//	// TEST ADD NEW LINK TO EMPTY LIST
//	//Arrange
//	node* list4 = (node*)malloc(sizeof(node));
//	list4->num = num;
//	list4->next = NULL;
//	node* node2 = list_init(3);
//	node2->next = list4;
//	//Act
//	node* head4 = list_init(num);
//	node * _new_ =list_add(head4, 3);
//	//Assert
//	if (getLenght(head4) == getLenght(_new_)&&checkEQ(head4,_new_))
//		printf("Add-OK\n");
//
//	// TEST REMOVE A LIST
//	//Arrange
//	node* list3 = (node*)malloc(sizeof(node));
//	list3->num = num;
//	list3->next = NULL;
//	free(list3);
//	list3 = NULL;
//	//Act
//	node* head3 = list_init(num);
//	remove____(head3);
//	//Assert
//	while (head3);
//	printf("Remove-OK\n");
//	return 0;
//}
/*
void* safe_malloc(size_t size)
{
	void* ptr = malloc(size);

	if (!ptr && (size > 0)) {
		perror("malloc failed!");
		exit(EXIT_FAILURE);
	}

	return ptr;
}*/