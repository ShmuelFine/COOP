#include "ExeptionUnitTest.h"

TEST_FUN_IMPL(ExceptionTests, WhenThrown_ThenGoesStraightToScopeEnd)
{
	int exception_tester = 0;

	EXPECT_THROW;
	THROW;
	exception_tester++;
	ASSERT_THROW;

	NTEST_ASSERT(exception_tester == 0);
}
END_FUN

TEST_FUN_IMPL(ExceptionTests, WhenThrownFromWithinFunction_ThenGoesStraightToScopeEnd)
{
	int exception_tester = 0;

	char feedback[3] = { 0, 0, 0 };

	CREATE(ScopeTester, s), feedback + 0 CALL;

	EXPECT_THROW;
	// Cause throwing from within a func:
	FUN(&s, ThrowingIfEQ), 3, 3 CALL;
	exception_tester++;
	ASSERT_THROW;

	NTEST_ASSERT(exception_tester == 0);
}
END_FUN

TEST_FUN_IMPL(ExceptionTests, CATCH_WhenExeptionCaught_Does_NOT_ContinueThrowing)
{
	//Arrange
	bool isCaught = false;
	bool isContinuedExecution = false;

	char feedback[3] = { 0, 0, 0 };
	{
		TRY
		{
			CREATE(ScopeTester,s), (feedback + 0) CALL;
			// cause throwing from within a function
			FUN(&s, ThrowingIfEQ), 3, 3 CALL;
		} CATCH {
			isCaught = true;
		}END_TRY;

		isContinuedExecution = true;

	}

	NTEST_ASSERT(isCaught);
	NTEST_ASSERT(isContinuedExecution);
}
END_FUN

TEST_FUN_IMPL(ExceptionTests, WhenUsingTHROW_MSG_ThenTheMessageIsSaved)
{
	char* whatToThrow = "ERROR! Help!";
	char* expected = whatToThrow;
	char actual[100] = { 0 };
	TRY
	{
		THROW_MSG(whatToThrow);

	}CATCH{
		memcpy(actual, LAST_EXCEPTION_ERROR_MSG, strlen(LAST_EXCEPTION_ERROR_MSG) + 1);
	}END_TRY;

	NTEST_ASSERT(0 == strcmp(actual, expected));
}
END_FUN

TEST_FUN_IMPL(ExceptionTests, BREAK_WhenDoneFromNastedLoop_ThenBreaksCorrectly)
{
	SCOPE_START;

	int counter = 0;
	for (int i = 0; i < 9; i++)
	{
		SCOPE_START;

		counter++;
		for (int j = 0; j < 10; j++)
		{
			SCOPE_START;

			counter += 100;

			if (5 <= (counter % 10))
				BREAK;


			END_SCOPE;
		}

		END_SCOPE;
	}

	NTEST_ASSERT(4509 == counter);
	END_SCOPE;
}
END_FUN

TEST_FUN_IMPL(ExceptionTests, BREAK_WhenDoneFromTRY_Catch_Block_ThenBreaksCorrectly)
{
	SCOPE_START;

	int counter = 0;
	for (int i = 0; i < 9; i++)
	{
		SCOPE_START;

		counter++;
		for (int j = 0; j < 10; j++)
		{
			SCOPE_START;
			counter += 100;
			TRY
			{
			if (5 <= (counter % 10))
				BREAK;
			} CATCH{
				counter = 100000;
			}END_TRY;

			END_SCOPE;
		}

		END_SCOPE;
	}

	NTEST_ASSERT(4509 == counter);
	END_SCOPE;
}
END_FUN

TEST_FUN_IMPL(ExceptionTests, BREAK_WhenDoneFromLoop_ThenFreesObjectsFromInnerScope)
{
	SCOPE_START;
	char feedback[3] = { 0, 0, 0 };

	for (int i = 0; i < 3; i++)
	{
		SCOPE_START;

		for (int j = 0; j < 100; j++)
		{
			SCOPE_START;

			CREATE(ScopeTester, s), feedback + i CALL;

			if (true)
				BREAK;

			END_SCOPE;
		}
		END_SCOPE;
	}

	NTEST_ASSERT(feedback[0] == 0);
	NTEST_ASSERT(feedback[1] == 0);
	NTEST_ASSERT(feedback[2] == 0);
	END_SCOPE;
}
END_FUN

INIT_TEST_SUITE(ExceptionTests)
BIND_TEST(ExceptionTests, WhenThrown_ThenGoesStraightToScopeEnd);
BIND_TEST(ExceptionTests, WhenThrownFromWithinFunction_ThenGoesStraightToScopeEnd);
BIND_TEST(ExceptionTests, CATCH_WhenExeptionCaught_Does_NOT_ContinueThrowing);
BIND_TEST(ExceptionTests, WhenUsingTHROW_MSG_ThenTheMessageIsSaved);
BIND_TEST(ExceptionTests, BREAK_WhenDoneFromNastedLoop_ThenBreaksCorrectly);
BIND_TEST(ExceptionTests, BREAK_WhenDoneFromTRY_Catch_Block_ThenBreaksCorrectly);
BIND_TEST(ExceptionTests, BREAK_WhenDoneFromLoop_ThenFreesObjectsFromInnerScope);
END_INIT_TEST_SUITE(ExceptionTests);