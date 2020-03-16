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

FUN_IMPL(SCOPE_END__WhenObjectsDefinedInsideScope_ThenAllGetFreed)
{
	char feedback[3] = { 0, 0, 0 };
	SCOPE_START;

	//Arrange
	
	CREATE(ScopeTester, s1), feedback + 0);
	CREATE(ScopeTester, s2), feedback + 1);
	CREATE(ScopeTester, s3), feedback + 2);
	
	// - The ctor of ScopeTester sets the feedback val:
	ASSERT(feedback[0] == 'A');
	ASSERT(feedback[1] == 'A');
	ASSERT(feedback[2] == 'A');
	
	// Act
	END_SCOPE;
	
	// Assert
	// - The dtor of ScopeTester sets the feedback vals again:
	ASSERT(feedback[0] == 0);
	ASSERT(feedback[1] == 0);
	ASSERT(feedback[2] == 0);
}
END_FUN

FUN_IMPL(LOCAL_SCOPE__WhenObjectsDefinedInside_InnerScope_ThenAllGetFreed)
{
	char feedback[4] = { 0, 0, 0, 0 };
	SCOPE_START;

	CREATE(ScopeTester, outer_scope_obj), feedback + 0);
	ASSERT(feedback[0] == 'A');

	bool is_get_into_if = true;
	if (is_get_into_if)
	{
		SCOPE_START;

		//Arrange
		CREATE(ScopeTester, inner_scope_obj1), feedback + 1);
		CREATE(ScopeTester, inner_scope_obj2), feedback + 2);
		CREATE(ScopeTester, inner_scope_obj3), feedback + 3);

		// - The ctor of ScopeTester sets the feedback val:
		ASSERT(feedback[1] == 'A');
		ASSERT(feedback[2] == 'A');
		ASSERT(feedback[3] == 'A');

		// Act
		END_SCOPE;

		// - The dtor of ScopeTester sets the feedback val again:
		ASSERT(feedback[1] == 0);
		ASSERT(feedback[2] == 0);
		ASSERT(feedback[3] == 0);

		ASSERT(feedback[0] == 'A');

	}

	END_SCOPE;

	// Assert
	ASSERT(feedback[0] == 0);

}
END_FUN

FUN_IMPL(LOCAL_SCOPE__DoesNotFreeUnrelatedObjects)
{
	char feedback[3] = { 0, 0, 0 };
	SCOPE_START;

	//Arrange

	CREATE(ScopeTester, outer_obj1), feedback + 0);
	CREATE(ScopeTester, outer_obj2), feedback + 1);

	//Act
	if (1)
	{
		SCOPE_START;
		CREATE(ScopeTester, inner_obj), feedback + 2);
		END_SCOPE;
	}

	//Assert
	// Outer objects were not yet freed:
	ASSERT(feedback[0] != 0);
	ASSERT(feedback[1] != 0);

	END_SCOPE;
}
END_FUN

FUN_IMPL(LOCAL_SCOPE__WhenMultipleNestedScopesExist_ThenFreesOnlyTheInnerMost)
{
	char feedback[4] = { 0, 0, 0, 0 };
	
	SCOPE_START;

	//Arrange 
	CREATE(ScopeTester, outer_scope_obj), feedback + 0);
	ASSERT(feedback[0] == 'A');

	//Act
	if (1) {
		SCOPE_START;
		CREATE(ScopeTester, inner_scope_1), feedback + 1);

		if (1)
		{
			SCOPE_START;
			CREATE(ScopeTester, inner_scope_2), feedback + 2);

			//Assert
			ASSERT(feedback[0] != 0);
			ASSERT(feedback[1] != 0);
			ASSERT(feedback[2] != 0);

			END_SCOPE;

			ASSERT(feedback[0] != 0);
			ASSERT(feedback[1] != 0);
			ASSERT(feedback[2] == 0);
		}
		
		END_SCOPE;

		ASSERT(feedback[0] != 0);
		ASSERT(feedback[1] == 0);
		ASSERT(feedback[2] == 0);

	}
	
	END_SCOPE;

	ASSERT(feedback[0] == 0);
	ASSERT(feedback[1] == 0);
	ASSERT(feedback[2] == 0);
}
END_FUN

FUN_IMPL(LOCAL_SCOPE__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash)
{
	char feedback[4] = { 0, 0, 0, 0 };

	SCOPE_START;

	//Arrange 
	CREATE(ScopeTester, outer_scope_obj), feedback + 0);
	ASSERT(feedback[0] == 'A');

	//Act
	if (1) {
		SCOPE_START;
		CREATE(ScopeTester, inner_scope_1), feedback + 1);
		if (1) {
			SCOPE_START;
			END_SCOPE;
		}

		// Assert
		ASSERT(feedback[1] != 0);
		END_SCOPE;
		ASSERT(feedback[1] == 0);
	}
	
	END_SCOPE;
}
END_FUN

FUN_IMPL(LOCAL_SCOPE__LoopScopeSanityTest)
{
	char feedback[4] = { 0, 0, 0, 0 };

	for (int i = 0; i < 100; i++)
	{
		SCOPE_START;
		CREATE(ScopeTester, inner_scope_1), feedback + i % 4);
		END_SCOPE;
	}

	for (int i=0 ; i < 4; i++)
		ASSERT(feedback[i] == 0);

}
END_FUN

#include "Shared_ptr.h"
FUN_IMPL(Shared_ptr__CopyTo__PointsOnTheSameValue)
{
	// Arrange
	CREATE(MemoryManager, memManager), sizeof(int) * 20, STACK_BASED_MEMORY);


	CREATE(Shared_ptr, ptr));
	CREATE(Shared_ptr, ptr2));
	
	int* i = NULL;
	NEW(i, int);
	*i = 110;

	int* j= NULL;
	NEW(j, int);
	*j = 300;

	FUN(&ptr, Reset), i CALL;//110
	FUN(&ptr2, Reset), j CALL;//300

	ASSERT(*ptr2.px == 300)

	// Act
	FUN(&ptr, CopyTo), & ptr2 CALL;
	

	FUN(&ptr2, CopyFrom), & ptr CALL;

	// Assert
	ASSERT(*ptr2.px == 110);

	*i = 220;
	ASSERT(*ptr.px  == 220);
	ASSERT(*ptr2.px == 220);

	bool out;
	FUN(&ptr, IsEmpty), & out CALL;
	ASSERT(!(out == 0));

	FUN(&ptr, Release) CALL;


}
END_FUN
