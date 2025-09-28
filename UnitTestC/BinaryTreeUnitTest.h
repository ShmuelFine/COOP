#ifndef __COOP_TESTS__BINARY_TREE_TEST__H__
#define __COOP_TESTS__BINARY_TREE_TEST__H__

#include "UnitTestsInfra.h"
#include "COOP.h"

DEF_TEST_SUITE(BinaryTreeTest);
ADD_TEST(BinaryTreeTest, insert_SanityTest);
ADD_TEST(BinaryTreeTest, removeLeaf_SanityTest);
ADD_TEST(BinaryTreeTest, removeRoot_SanityTest);
ADD_TEST(BinaryTreeTest, iterators_beginEqualsEnd_onEmpty);
ADD_TEST(BinaryTreeTest, prev_from_end_returns_rightmost);
ADD_TEST(BinaryTreeTest, iterator_modifyThroughRef_persists);
ADD_TEST(BinaryTreeTest, traversals_DoNotCrash);
ADD_TEST(BinaryTreeTest, iterator_advance_WorksAndReachesEnd);
ADD_TEST(BinaryTreeTest, iterator_distance_BeginToEndEqualsSize);
ADD_TEST(BinaryTreeTest, remove_onEmpty_tree);
ADD_TEST(BinaryTreeTest, print_onEmpty);
ADD_TEST(BinaryTreeTest, dtor_freesAllMemory);
END_TEST_SUITE(BinaryTreeTest);

#endif
