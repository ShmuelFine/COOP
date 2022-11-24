#ifndef __COOP_TESTS__DEMO_CLASS_TEST___H__
#define __COOP_TESTS__DEMO_CLASS_TEST___H__

#include "test_defs.h"
#include "../COOP/COOP.h"

DEF_TEST_SUITE(DemoClassTest);
ADD_TEST(DemoClassTest, Class_CallSimpleFunction);
ADD_TEST(DemoClassTest, Rectangle_SanityTest);
END_TEST_SUITE(DemoClassTest);

#endif