#include "pch.h"
#include "rbTreeUnitTest.h"

TEST(redBlackTree, rbTree_insert_sanity)
{
	ASSERT_FALSE(rbTree_insert_SanityTest());
}

TEST(redBlackTree, rbTree_find_sanity)
{
	ASSERT_FALSE(rbTree_find_SanityTest());
}

TEST(redBlackTree, rbTree_delete_sanity)
{
	ASSERT_FALSE(rbTree_delete_SanityTest());
}

TEST(redBlackTree, rbTree_iteration_sanity)
{
	ASSERT_FALSE(rbTree_iteration_SanityTest());
}

TEST(redBlackTree, rbTree_rectangle_as_node_data_insert_sanity)
{
	ASSERT_FALSE(rbTree_rectangle_as_node_data_insert_SanityTest());
}

TEST(redBlackTree, rbTree_rectangle_as_node_data_find_sanity)
{
	ASSERT_FALSE(rbTree_rectangle_as_node_data_find_SanityTest());
}

TEST(redBlackTree, rbTree_rectangle_as_node_data_delete_sanity)
{
	ASSERT_FALSE(rbTree_rectangle_as_node_data_delete_SanityTest());
}