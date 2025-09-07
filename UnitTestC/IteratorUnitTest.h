#ifndef __COOP_TESTS__ITERATOR_FOR_TEST__H__
#define __COOP_TESTS__ITERATOR_FOR_TEST__H__

#include "UnitTestsInfra.h"
#include "COOP.h"
#include "vector.h"
#include "iterator.h"

DEF_TEST_SUITE(IteratorForTest);
	ADD_TEST(IteratorForTest, foreach_SumsAll);
	ADD_TEST(IteratorForTest, foreach_ContinueSkips);
	ADD_TEST(IteratorForTest, foreach_BreakStopsEarly);
	ADD_TEST(IteratorForTest, foreach_EmptyRange);
END_TEST_SUITE(IteratorForTest);

#endif 
