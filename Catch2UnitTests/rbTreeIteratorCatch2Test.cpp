#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("rbTreeIterator", "[rbTreeIterator]")
{
	SECTION("redBlackTreeIterator_increment_sanityTest")
	{
		CHECK_FALSE(redBlackTreeIterator_increment_sanity());
	}
	SECTION("redBlackTreeIterator_decrement_sanityTest")
	{
		CHECK_FALSE(redBlackTreeIterator_decrement_sanity());
	}
}