#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("Class CallSimpleFunction SanityTest", "[Class]") {

	CHECK_FALSE(Class_CallSimpleFunction_SanityTest());
}