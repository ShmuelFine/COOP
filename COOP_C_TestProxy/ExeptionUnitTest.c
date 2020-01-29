#include "ExeptionTest.h"
#include "ScopeTester.h"
#include "dynamic_memory_management.h"

int Exception_WhenThrown_ThenGoesStraightToScopeEnd(int * tester)
{
	*tester = 0;
	SCOPE_START;

	if (1)
	{
		LOCAL_SCOPE_START;
		THROW;
		(*tester)++;
		LOCAL_SCOPE_END;
	}
	(*tester)++;

	SCOPE_END;
}

int Exception_WhenThrownFromWithinFunction_ThenGoesStraightToScopeEnd(int * tester)
{
	(*tester) = 0;
	char feedback[3] = { 0, 0, 0 };
	ScopeTester s;

	SCOPE_START;

	CREATE_OBJECT(ScopeTester, s), feedback + 0);
	if (1)
	{
		LOCAL_SCOPE_START;
		// Cause throwing from within a func:
		CALL(ThrowingIfEQ, s, 3, 3);
		(*tester)++;
		LOCAL_SCOPE_END;
	}
	(*tester)++;

	SCOPE_END;
}

int CATCH_WhenExeptionCaught_Does_NOT_ContinueThrowing()
{
	//Arrange
	bool isCaught = false;
	bool isContinuedExecution = false;

	SCOPE_START;
	
	char feedback[3] = { 0, 0, 0 };
	ScopeTester s;
	{
		LOCAL_SCOPE_START;
		
		TRY
		{
			CREATE_OBJECT(ScopeTester,s),feedback + 0);
			// cause throwing from within a function:
			CALL(ThrowingIfEQ,s, 3, 3);
		}CATCH{
			isCaught = true;
		}END_TRY;

		isContinuedExecution = true;

		LOCAL_SCOPE_END;
	}
	
	ASSERT(isCaught);
	ASSERT(isContinuedExecution);

	SCOPE_END;

}