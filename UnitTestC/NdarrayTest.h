#include "test_defs.h"
#include "../COOP/COOP.h"
#include "../COOP/ExportDefs.h"
#include "../COOP/Ndarray.h"
#include "ScopeTester.h"

DEF_TEST_SUITE(NDarray_Tests);
ADD_TEST(NDarray_Tests, init_sanity_case);
ADD_TEST(NDarray_Tests, init_invalid_dims);
ADD_TEST(NDarray_Tests, init_invalid_shape);
ADD_TEST(NDarray_Tests, get_location_3D);
ADD_TEST(NDarray_Tests, set_sanity_case);
ADD_TEST(NDarray_Tests, set_index_out_of_range);
ADD_TEST(NDarray_Tests, at_sanity_case);
ADD_TEST(NDarray_Tests, at_index_out_of_range);
ADD_TEST(NDarray_Tests, fill_sanity_case);
ADD_TEST(NDarray_Tests, contains_when_true);
ADD_TEST(NDarray_Tests, contains_when_false);
ADD_TEST(NDarray_Tests, min_sanity_case);
ADD_TEST(NDarray_Tests, max_sanity_case);
ADD_TEST(NDarray_Tests, general_test);
END_TEST_SUITE(NDarray_Tests);