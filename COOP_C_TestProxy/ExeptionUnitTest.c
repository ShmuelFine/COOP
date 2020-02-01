#include "ExeptionUnitTest.h"
#include "ScopeTester.h"
#include "dynamic_memory_management.h"

IMPL_FUN(Exception_WhenThrown_ThenGoesStraightToScopeEnd, int * tester)
{
	*tester = 0;

	if (1)
	{
		SCOPE_START;
		THROW;
		(*tester)++;
		SCOPE_END;
	}
	(*tester)++;
}
END_FUN

IMPL_FUN(Exception_WhenThrownFromWithinFunction_ThenGoesStraightToScopeEnd, int * tester)
{
	(*tester) = 0;
	char feedback[3] = { 0, 0, 0 };

	CREATE_OBJECT(ScopeTester, s), feedback + 0);
	if (1)
	{
		SCOPE_START;
		// Cause throwing from within a func:
		FUN(ThrowingIfEQ, &s), 3, 3 CALL
		(*tester)++;
		SCOPE_END;
	}
	(*tester)++;
}
END_FUN

IMPL_FUN(CATCH_WhenExeptionCaught_Does_NOT_ContinueThrowing)
{
	//Arrange
	bool isCaught = false;
	bool isContinuedExecution = false;

	char feedback[3] = { 0, 0, 0 };
	{
		SCOPE_START;
		
		TRY
		{
			CREATE_OBJECT(ScopeTester,s),feedback + 0);
			// cause throwing from within a function:
			FUN(ThrowingIfEQ, &s), 3, 3 CALL
		}CATCH{
			isCaught = true;
		}END_TRY;

		isContinuedExecution = true;

		SCOPE_END;
	}
	
	ASSERT(isCaught);
	ASSERT(isContinuedExecution);
}
END_FUN

