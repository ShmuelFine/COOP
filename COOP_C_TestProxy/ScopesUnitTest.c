#include "ScopesUnitTest.h"
#include "Cache.h"
#include "dynamic_memory_management.h"
#include "COOP.h"
#include "ScopeTester.h"

/////////////////////////////////////////////
//	See the first two tests for detailed   //
//	explanations of the testing approach   //
//	used throughout this file.			   //
/////////////////////////////////////////////

IMPL_FUN(SCOPE_END__WhenObjectsDefinedInsideScope_ThenAllGetFreed)
{
	char feedback[3] = { 0, 0, 0 };
	SCOPE_START;

	//Arrange
	
	CREATE_OBJECT(ScopeTester, s1), feedback + 0);
	CREATE_OBJECT(ScopeTester, s2), feedback + 1);
	CREATE_OBJECT(ScopeTester, s3), feedback + 2);
	
	// - The ctor of ScopeTester sets the feedback val:
	ASSERT(feedback[0] == 'A');
	ASSERT(feedback[1] == 'A');
	ASSERT(feedback[2] == 'A');
	
	// Act
	SCOPE_END;
	
	// Assert
	// - The dtor of ScopeTester sets the feedback vals again:
	ASSERT(feedback[0] == 0);
	ASSERT(feedback[1] == 0);
	ASSERT(feedback[2] == 0);
}
END_FUN

IMPL_FUN(LOCAL_SCOPE_END__WhenObjectsDefinedInside_InnerScope_ThenAllGetFreed)
{
	char feedback[4] = { 0, 0, 0, 0 };
	SCOPE_START;

	CREATE_OBJECT(ScopeTester, outer_scope_obj), feedback + 0);
	ASSERT(feedback[0] == 'A');

	bool is_get_into_if = true;
	if (is_get_into_if)
	{
		SCOPE_START;

		//Arrange
		CREATE_OBJECT(ScopeTester, inner_scope_obj1), feedback + 1);
		CREATE_OBJECT(ScopeTester, inner_scope_obj2), feedback + 2);
		CREATE_OBJECT(ScopeTester, inner_scope_obj3), feedback + 3);

		// - The ctor of ScopeTester sets the feedback val:
		ASSERT(feedback[1] == 'A');
		ASSERT(feedback[2] == 'A');
		ASSERT(feedback[3] == 'A');

		// Act
		SCOPE_END;

		// - The dtor of ScopeTester sets the feedback val again:
		ASSERT(feedback[1] == 0);
		ASSERT(feedback[2] == 0);
		ASSERT(feedback[3] == 0);

		ASSERT(feedback[0] == 'A');

	}

	SCOPE_END;

	// Assert
	ASSERT(feedback[0] == 0);

}
END_FUN

IMPL_FUN(LOCAL_SCOPE_END__DoesNotFreeUnrelatedObjects)
{
	char feedback[3] = { 0, 0, 0 };
	SCOPE_START;

	//Arrange

	CREATE_OBJECT(ScopeTester, outer_obj1), feedback + 0);
	CREATE_OBJECT(ScopeTester, outer_obj2), feedback + 1);

	//Act
	if (1)
	{
		SCOPE_START;
		CREATE_OBJECT(ScopeTester, inner_obj), feedback + 2);
		SCOPE_END;
	}

	//Assert
	// Outer objects were not yet freed:
	ASSERT(feedback[0] != 0);
	ASSERT(feedback[1] != 0);

	SCOPE_END;
}
END_FUN

IMPL_FUN(LOCAL_SCOPE_END__WhenMultipleNestedScopesExist_ThenFreesOnlyTheInnerMost)
{
	char feedback[4] = { 0, 0, 0, 0 };
	
	SCOPE_START;

	//Arrange 
	CREATE_OBJECT(ScopeTester, outer_scope_obj), feedback + 0);
	ASSERT(feedback[0] == 'A');

	//Act
	if (1) {
		SCOPE_START;
		CREATE_OBJECT(ScopeTester, inner_scope_1), feedback + 1);

		if (1)
		{
			SCOPE_START;
			CREATE_OBJECT(ScopeTester, inner_scope_2), feedback + 2);

			//Assert
			ASSERT(feedback[0] != 0);
			ASSERT(feedback[1] != 0);
			ASSERT(feedback[2] != 0);

			SCOPE_END;

			ASSERT(feedback[0] != 0);
			ASSERT(feedback[1] != 0);
			ASSERT(feedback[2] == 0);
		}
		
		SCOPE_END;

		ASSERT(feedback[0] != 0);
		ASSERT(feedback[1] == 0);
		ASSERT(feedback[2] == 0);

	}
	
	SCOPE_END;

	ASSERT(feedback[0] == 0);
	ASSERT(feedback[1] == 0);
	ASSERT(feedback[2] == 0);
}
END_FUN

IMPL_FUN(LOCAL_SCOPE_END__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash)
{
	char feedback[4] = { 0, 0, 0, 0 };

	SCOPE_START;

	//Arrange 
	CREATE_OBJECT(ScopeTester, outer_scope_obj), feedback + 0);
	ASSERT(feedback[0] == 'A');

	//Act
	if (1) {
		SCOPE_START;
		CREATE_OBJECT(ScopeTester, inner_scope_1), feedback + 1);
		if (1) {
			SCOPE_START;
			SCOPE_END;
		}

		// Assert
		ASSERT(feedback[1] != 0);
		SCOPE_END;
		ASSERT(feedback[1] == 0);
	}
	
	SCOPE_END;
}
END_FUN