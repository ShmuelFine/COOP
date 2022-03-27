#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("rbTreeIterator", "[rbTreeIterator]")
{
	SECTION("rbTree_insert_sanity")
	{
		CHECK_FALSE(rbTree_insert_SanityTest());
	}
	SECTION("rbTree_find_sanity")
	{
		CHECK_FALSE(rbTree_find_SanityTest());
	}
	SECTION("rbTree_delete_sanity")
	{
		CHECK_FALSE(rbTree_delete_SanityTest());
	}
	SECTION("rbTree_iteration_sanity")
	{
		CHECK_FALSE(rbTree_iteration_SanityTest());
	}
	SECTION("rbTree_rectangle_as_node_data_insert_sanity")
	{
		CHECK_FALSE(rbTree_rectangle_as_node_data_insert_SanityTest());
	}
	SECTION("rbTree_rectangle_as_node_data_find_sanity")
	{
		CHECK_FALSE(rbTree_rectangle_as_node_data_find_SanityTest());
	}
	SECTION("rbTree_rectangle_as_node_data_delete_sanity")
	{
		CHECK_FALSE(rbTree_rectangle_as_node_data_delete_SanityTest());
	}
	
	
}