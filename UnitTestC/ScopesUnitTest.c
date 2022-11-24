#include "ScopesUnitTest.h"

FUN_IMPL(SCOPE_END__WhenObjectsDefinedInsideScope_ThenAllGetFreed)
{
	char feedback[3] = { 0, 0, 0 };
	SCOPE_START;

	//Arrange

	CREATE(ScopeTester, s1), feedback + 0 CALL;
	CREATE(ScopeTester, s2), feedback + 1 CALL;
	CREATE(ScopeTester, s3), feedback + 2 CALL;

	// - The ctor of ScopeTester sets the feedback val:
	NTEST_ASSERT(feedback[0] == 'A');
	NTEST_ASSERT(feedback[1] == 'A');
	NTEST_ASSERT(feedback[2] == 'A');

	// Act
	END_SCOPE;

	// Assert
	// - The dtor of ScopeTester sets the feedback vals again:
	NTEST_ASSERT(feedback[0] == 0);
	NTEST_ASSERT(feedback[1] == 0);
	NTEST_ASSERT(feedback[2] == 0);
}
END_FUN

FUN_IMPL(LOCAL_SCOPE__WhenObjectsDefinedInside_InnerScope_ThenAllGetFreed)
{
	char feedback[4] = { 0, 0, 0, 0 };
	SCOPE_START;

	CREATE(ScopeTester, outer_scope_obj), feedback + 0 CALL;
	NTEST_ASSERT(feedback[0] == 'A');

	bool is_get_into_if = true;
	if (is_get_into_if)
	{
		SCOPE_START;

		//Arrange
		CREATE(ScopeTester, inner_scope_obj1), feedback + 1 CALL;
		CREATE(ScopeTester, inner_scope_obj2), feedback + 2 CALL;
		CREATE(ScopeTester, inner_scope_obj3), feedback + 3 CALL;

		// - The ctor of ScopeTester sets the feedback val:
		NTEST_ASSERT(feedback[1] == 'A');
		NTEST_ASSERT(feedback[2] == 'A');
		NTEST_ASSERT(feedback[3] == 'A');

		// Act
		END_SCOPE;

		// - The dtor of ScopeTester sets the feedback val again:
		NTEST_ASSERT(feedback[1] == 0);
		NTEST_ASSERT(feedback[2] == 0);
		NTEST_ASSERT(feedback[3] == 0);

		NTEST_ASSERT(feedback[0] == 'A');

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

	CREATE(ScopeTester, outer_obj1), feedback + 0 CALL;
	CREATE(ScopeTester, outer_obj2), feedback + 1 CALL;

	//Act
	if (1)
	{
		SCOPE_START;
		CREATE(ScopeTester, inner_obj), feedback + 2 CALL;
		END_SCOPE;
	}

	//Assert
	// Outer objects were not yet freed:
	NTEST_ASSERT(feedback[0] != 0);
	NTEST_ASSERT(feedback[1] != 0);

	END_SCOPE;
}
END_FUN

FUN_IMPL(LOCAL_SCOPE__WhenMultipleNestedScopesExist_ThenFreesOnlyTheInnerMost)
{
	char feedback[4] = { 0, 0, 0, 0 };

	SCOPE_START;

	//Arrange 
	CREATE(ScopeTester, outer_scope_obj), feedback + 0 CALL;
	NTEST_ASSERT(feedback[0] == 'A');

	//Act
	if (1) {
		SCOPE_START;
		CREATE(ScopeTester, inner_scope_1), feedback + 1 CALL;

		if (1)
		{
			SCOPE_START;
			CREATE(ScopeTester, inner_scope_2), feedback + 2 CALL;

			//Assert
			NTEST_ASSERT(feedback[0] != 0);
			NTEST_ASSERT(feedback[1] != 0);
			NTEST_ASSERT(feedback[2] != 0);

			END_SCOPE;

			NTEST_ASSERT(feedback[0] != 0);
			NTEST_ASSERT(feedback[1] != 0);
			NTEST_ASSERT(feedback[2] == 0);
		}

		END_SCOPE;

		NTEST_ASSERT(feedback[0] != 0);
		NTEST_ASSERT(feedback[1] == 0);
		NTEST_ASSERT(feedback[2] == 0);

	}

	END_SCOPE;

	NTEST_ASSERT(feedback[0] == 0);
	NTEST_ASSERT(feedback[1] == 0);
	NTEST_ASSERT(feedback[2] == 0);
}
END_FUN

FUN_IMPL(LOCAL_SCOPE__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash)
{
	char feedback[4] = { 0, 0, 0, 0 };

	SCOPE_START;

	//Arrange 
	CREATE(ScopeTester, outer_scope_obj), feedback + 0 CALL;
	NTEST_ASSERT(feedback[0] == 'A');

	//Act
	if (1) {
		SCOPE_START;
		CREATE(ScopeTester, inner_scope_1), feedback + 1 CALL;
		if (1) {
			SCOPE_START;
			END_SCOPE;
		}

		// Assert
		NTEST_ASSERT(feedback[1] != 0);
		END_SCOPE;
		NTEST_ASSERT(feedback[1] == 0);
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
		CREATE(ScopeTester, inner_scope_1), feedback + i % 4 CALL;
		END_SCOPE;
	}

	for (int i = 0; i < 4; i++)
		NTEST_ASSERT(feedback[i] == 0);

}
END_FUN

#include "../COOP/Shared_ptr.h"
FUN_IMPL(Shared_ptr__CopyTo__PointsOnTheSameValue)
{
	// Arrange
	init_global_memory( sizeof(int) * 20, STACK_BASED_MEMORY );


	CREATE(Shared_ptr, ptr) CALL;
	CREATE(Shared_ptr, ptr2) CALL;

	int* i = NULL;
	NEW(i, int);
	*i = 110;

	int* j = NULL;
	NEW(j, int);
	*j = 300;



	FUN(&ptr, Reset), i CALL;//110
	FUN(&ptr2, Reset), j CALL;//300

	//int* actualValue = ((int*)(ptr2.px));
	NTEST_ASSERT(*GET(ptr2, int) == 300)

		// Act
		FUN(&ptr, CopyTo), & ptr2 CALL;


	FUN(&ptr2, CopyFrom), & ptr CALL;

	// Assert
	NTEST_ASSERT(*GET(ptr2, int) == 110);

	*i = 220;
	NTEST_ASSERT(*GET(ptr, int) == 220);
	NTEST_ASSERT(*GET(ptr2, int) == 220);

	bool out;
	FUN(&ptr, IsEmpty), & out CALL;
	NTEST_ASSERT(!(out == 0));

	FUN(&ptr, Release) CALL;


}
END_FUN


