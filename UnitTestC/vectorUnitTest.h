#include "test_defs.h"
#include "../COOP/COOP.h"
#include "../COOP/ExportDefs.h"
#include "../COOP/vectorIterator.h"
#include "ScopeTester.h"

// #include "../COOP_C_TestProxy/ScopeTester.h"
#include "../COOP/vector.h"

DEF_TEST_SUITE(VectorTest);
ADD_TEST(VectorTest, push_back_SanityTest);
ADD_TEST(VectorTest, at_throws_when_idx_is_outOfRange);
ADD_TEST(VectorTest, iteration_SanityTest);
END_TEST_SUITE(VectorTest);