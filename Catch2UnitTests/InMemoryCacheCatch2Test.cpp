#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("MemManagement", "[MemManagement]")
{
	SECTION("New_WhenAllocatingBuffer_ThenReturnsRightAmountOfMemroy")
	{
		CHECK(New_WhenAllocatingBuffer_ThenReturnsRightAmountOfMemroy());
	}
	SECTION("New_WhenAllocatingClassInstance_ThenReturnsRightAmountOfMemroy")
	{
		CHECK(New_WhenAllocatingClassInstance_ThenReturnsRightAmountOfMemroy());
	}
	SECTION("Delete_WhenDeletingAllocMemory_ThenItFreesTheResource")
	{
		CHECK(Delete_WhenDeletingAllocMemory_ThenItFreesTheResource());
	}

}