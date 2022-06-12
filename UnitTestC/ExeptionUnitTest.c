#include "ExeptionUnitTest.h"

FUN_IMPL(Exception_WhenThrown_ThenGoesStraightToScopeEnd, int* tester)
{
	*tester = 0;

	if (1)
	{
		SCOPE_START;
		THROW;
		(*tester)++;
		END_SCOPE;
	}
	(*tester)++;
}
END_FUN

FUN_IMPL(Exception_WhenThrownFromWithinFunction_ThenGoesStraightToScopeEnd, int* tester)
{
	(*tester) = 0;
	char feedback[3] = { 0, 0, 0 };

	CREATE(ScopeTester, s), feedback + 0);
	if (1)
	{
		SCOPE_START;
		// Cause throwing from within a func:
		FUN(&s, ThrowingIfEQ), 3, 3 CALL
		(*tester)++;
		END_SCOPE;
	}
	(*tester)++;
}
END_FUN

FUN_IMPL(CATCH_WhenExeptionCaught_Does_NOT_ContinueThrowing)
{
	//Arrange
	bool isCaught = false;
	bool isContinuedExecution = false;

	char feedback[3] = { 0, 0, 0 };
	{
		SCOPE_START;

		TRY
		{
			CREATE(ScopeTester,s),feedback + 0);
			// cause throwing from within a function:
			FUN(&s, ThrowingIfEQ), 3, 3 CALL
		}CATCH{
			isCaught = true;
		}END_TRY;

			isContinuedExecution = true;

			END_SCOPE;
	}

	TEST_ASSERT(isCaught);
	TEST_ASSERT(isContinuedExecution);
}
END_FUN

FUN_IMPL(Exception_WhenUsingTHROW_MSG_ThenTheMessageIsSaved, const char* whatToThrow, char* outThrowingMsg)
{

	TRY
	{
		THROW_MSG(whatToThrow);

	}CATCH{
		memcpy(outThrowingMsg, LAST_EXCEPTION_ERROR_MSG, strlen(LAST_EXCEPTION_ERROR_MSG) + 1);
	}END_TRY;

}
END_FUN

FUN_IMPL(BREAK_WhenDoneFromNastedLoop_ThenBreaksCorrectly)
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

	TEST_ASSERT(4509 == counter);
	END_SCOPE;
}
END_FUN

FUN_IMPL(BREAK_WhenDoneFromTRY_Catch_Block_ThenBreaksCorrectly)
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

	TEST_ASSERT(4509 == counter);
	END_SCOPE;
}
END_FUN

FUN_IMPL(BREAK_WhenDoneFromLoop_ThenFreesObjectsFromInnerScope)
{
	SCOPE_START;
	char feedback[3] = { 0, 0, 0 };

	for (int i = 0; i < 3; i++)
	{
		SCOPE_START;

		for (int j = 0; j < 100; j++)
		{
			SCOPE_START;

			CREATE(ScopeTester, s), feedback + i);

			if (true)
				BREAK;

			END_SCOPE;
		}
		END_SCOPE;
	}

	TEST_ASSERT(feedback[0] == 0);
	TEST_ASSERT(feedback[1] == 0);
	TEST_ASSERT(feedback[2] == 0);
	END_SCOPE;
}
END_FUN
