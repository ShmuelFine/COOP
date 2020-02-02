#include "ExeptionUnitTest.h"
#include "ScopeTester.h"
#include "dynamic_memory_management.h"
#include <string.h>

FUN_IMPL(Exception_WhenThrown_ThenGoesStraightToScopeEnd, int * tester)
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

FUN_IMPL(Exception_WhenThrownFromWithinFunction_ThenGoesStraightToScopeEnd, int * tester)
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
	
	ASSERT(isCaught);
	ASSERT(isContinuedExecution);
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
