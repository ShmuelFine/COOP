#ifndef __COOP_TESTS__RB_TREE_ITER_TEST__H__
#define __COOP_TESTS__RB_TREE_ITER_TEST__H__

#include "test_defs.h"
#include "../COOP/COOP.h"
#include "../COOP/redBlackTreeIterator.h"
#include "../COOP/redBlackTree.h"


DEF_TEST_SUITE(RedBlackTreeIteratorTest);
ADD_TEST(RedBlackTreeIteratorTest, redBlacktreeIterator_decrement_Sanity);
ADD_TEST(RedBlackTreeIteratorTest, redBlacktreeIterator_increment_Sanity);
END_TEST_SUITE(RedBlackTreeIteratorTest);

#endif