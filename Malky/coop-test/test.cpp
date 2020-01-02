#include "pch.h"
#include "../coop/coop.h"
#include "..//coop/coop.c"


TEST(coop, Test_init)
{
	//Arrange
	int num = 5;
	node* expected = (node*)malloc(sizeof(node));
	expected->num = num;
	expected->next = NULL;

	//Act
	node* head = init(num);

	//Assert
	EXPECT_EQ(expected, head);
}

TEST(coop, Test_remove)
{
	//Arrange
	int num = 5;
	node* expected = init(num);
	free(expected);

	//Act
	node* head = init(num);
	remove(head);

	//Assert
	EXPECT_EQ(expected, head);
}

TEST(coop, Test_add)
{
	//Arrange
	node* node1 = init(5);
	node* node2 = init(8);
	node* node3 = init(9);
	node1->next = node2;
	node2->next = node3;

	//Act
	node* n1 = init(5);
	node* n3 = init(9);
	n1->next = n3;
	int num = 8;
	add(n1,num);

	//Assert
	EXPECT_EQ(node1, n1);
}
