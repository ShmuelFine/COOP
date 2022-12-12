#ifndef __COOP_TESTS__RB_TREE_TEST__H__
#define __COOP_TESTS__RB_TREE_TEST__H__

#include "UnitTestsInfra.h"
#include "COOP.h"
#include "RedBlackTree.h"
#include "Rectangle.h"

DEF_TEST_SUITE(RedBlackTreeTest);
ADD_TEST(RedBlackTreeTest, insert_SanityTest);
ADD_TEST(RedBlackTreeTest, find_SanityTest);
ADD_TEST(RedBlackTreeTest, delete_SanityTest);
ADD_TEST(RedBlackTreeTest, iteration_SanityTest);
ADD_TEST(RedBlackTreeTest, rectangle_as_node_data_insert_SanityTest);
ADD_TEST(RedBlackTreeTest, rectangle_as_node_data_find_SanityTest);
ADD_TEST(RedBlackTreeTest, rectangle_as_node_data_delete_SanityTest);
END_TEST_SUITE(RedBlackTreeTest);

#endif