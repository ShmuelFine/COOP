#ifndef __COOP_TESTS__TENSOR_TEST__H__
#define __COOP_TESTS__TENSOR_TEST__H__

#include "UnitTestsInfra.h"
#include "COOP.h"

DEF_TEST_SUITE(TensorTest);
ADD_TEST(TensorTest, at_ThrowsWhenIdxIsOutOfRange);
ADD_TEST(TensorTest, get_set_SanityTest);
ADD_TEST(TensorTest, reshape_SanityTest);
ADD_TEST(TensorTest, zero_all_SanityTest);
END_TEST_SUITE(TensorTest);

#endif
