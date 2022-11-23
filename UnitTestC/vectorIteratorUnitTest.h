#include "test_defs.h"
#include "../COOP/COOP.h"
#include "../COOP/ExportDefs.h"
#include "../COOP/vectorIterator.h"
#include "../COOP/vector.h"


DEF_TEST_SUITE(VectorIteratorTest);
ADD_TEST(VectorIteratorTest, getContentsOf_Sanity);
ADD_TEST(VectorIteratorTest, increment_Sanity);
END_TEST_SUITE(VectorIteratorTest);