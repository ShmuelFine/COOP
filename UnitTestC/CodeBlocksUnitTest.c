#include "CodeBlocksUnitTest.h"
#include "BaseClassExample.h"

FUN_IMPL(inner_createObjInFunc)
{
	CREATE(BaseClassExample, example), 14, 14 CALL;

}END_FUN;

TEST_FUN_IMPL(CodeBlocksTest, functionScope_FreesMemory) {
	// Arrange
	MEM_SIZE_T start_size = 0, end_size = 0;
	FUN(get_total_free_bytes)& start_size CALL;

	// Act
	FUN(inner_createObjInFunc) CALL;

	// Assert
	FUN(get_total_free_bytes)& end_size CALL;
	NTEST_ASSERT(start_size == end_size);

} END_FUN;

TEST_FUN_IMPL(CodeBlocksTest, forLoop_FreesMemory)
{
	// Arrange
	MEM_SIZE_T start_size = 0, end_size = 0;
	FUN(get_total_free_bytes)& start_size CALL;

	// Act

	FOR(int i = 0; i < 10; i++)
	{
		CREATE(BaseClassExample, example), 14, 14 CALL;
		// just make sure it's a valid object:
		example.hight = 456;

	}END_LOOP;

	// Assert
	FUN(get_total_free_bytes)& end_size CALL;
	NTEST_ASSERT(start_size == end_size);

} END_FUN;

TEST_FUN_IMPL(CodeBlocksTest, nestedLoop_FreesMemory)
{
	// Arrange
	MEM_SIZE_T start_size = 0, end_size = 0;
	FUN(get_total_free_bytes)& start_size CALL;

	// Act
	FOR(int i = 0; i < 10; i++)
	{
		CREATE(BaseClassExample, example), 14, 14 CALL;

		FOR(int j = 0; j < 2; j++)
		{
			CREATE(BaseClassExample, other), 14, 14 CALL;
		}END_LOOP;

	}END_LOOP;

	// Assert
	FUN(get_total_free_bytes)& end_size CALL;
	NTEST_ASSERT(start_size == end_size);
} END_FUN;

TEST_FUN_IMPL(CodeBlocksTest, forLoopWith_BREAK_FreesMemory)
{
	// Arrange
	MEM_SIZE_T start_size = 0, end_size = 0;
	FUN(get_total_free_bytes)& start_size CALL;
	int counter = 0;

	// Act
	FOR(int i = 0; i < 10; i++)
	{
		CREATE(BaseClassExample, example), 14, 14 CALL;

		IF(i == 8)
		{
			BREAK;
		}
		END_IF;

		counter++;
		CREATE(BaseClassExample, other), 14, 14 CALL;

	}END_LOOP;

	// Assert
	FUN(get_total_free_bytes)& end_size CALL;
	NTEST_ASSERT(start_size == end_size);
	NTEST_ASSERT(counter == 8);

} END_FUN;


FUN_IMPL(inner_ForWithReturn, int* counter)
{
	FOR(int i = 0; i < 10; i++)
	{
		CREATE(BaseClassExample, example), 14, 14 CALL;

		IF(i == 8)
		{
			RETURN;
		}
		END_IF;

		(*counter)++;
		CREATE(BaseClassExample, other), 14, 14 CALL;

	}END_LOOP;

}END_FUN;


TEST_FUN_IMPL(CodeBlocksTest, forLoopWith_RETURN_FreesMemory)
{
	// Arrange
	MEM_SIZE_T start_size = 0, end_size = 0;
	FUN(get_total_free_bytes)& start_size CALL;
	int counter = 0;

	// Act
	FUN(inner_ForWithReturn)& counter CALL;

	// Assert
	FUN(get_total_free_bytes)& end_size CALL;
	NTEST_ASSERT(start_size == end_size);
	NTEST_ASSERT(counter == 8);
} END_FUN;



FUN_IMPL(inner_NestedForWithReturn, int* counter)
{
	FOR(int i = 0; i < 10; i++)
	{
		CREATE(BaseClassExample, example), 14, 14 CALL;
		FOR(int k = 0; k < 2; k++)
		{
			CREATE(BaseClassExample, other), 14, 14 CALL;

			IF(i == 8)
			{
				RETURN;
			}
			END_IF;
		}END_LOOP;

		(*counter)++;

	}END_LOOP;

}END_FUN;



TEST_FUN_IMPL(CodeBlocksTest, nestedForLoopWith_RETURN_FreesMemory)
{
	// Arrange
	MEM_SIZE_T start_size = 0, end_size = 0;
	FUN(get_total_free_bytes)& start_size CALL;
	int counter = 0;

	// Act
	FUN(inner_NestedForWithReturn)& counter CALL;

	// Assert
	FUN(get_total_free_bytes)& end_size CALL;
	NTEST_ASSERT(start_size == end_size);
	NTEST_ASSERT(counter == 8);
} END_FUN;


INIT_TEST_SUITE_WITH_SPECIFIC_MEM_SPACE(CodeBlocksTest, STACK_BASED_MEMORY, 1000 * sizeof(int))
BIND_TEST(CodeBlocksTest, functionScope_FreesMemory);
BIND_TEST(CodeBlocksTest, forLoop_FreesMemory);
BIND_TEST(CodeBlocksTest, nestedLoop_FreesMemory);
BIND_TEST(CodeBlocksTest, forLoopWith_BREAK_FreesMemory);
BIND_TEST(CodeBlocksTest, forLoopWith_RETURN_FreesMemory);
BIND_TEST(CodeBlocksTest, nestedForLoopWith_RETURN_FreesMemory);
END_INIT_TEST_SUITE(CodeBlocksTest)