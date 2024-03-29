#include "ScopesUnitTest.h"

FUN_IMPL(ChangeValueWithScopeTester, char* whoToChange)
{
	CREATE(ScopeTester, outer_scope_obj), whoToChange CALL;
	if (*whoToChange != 'A')
	{
		THROW_MSG("BUG in ScopeTester");
	}
}
END_FUN;

FUN_IMPL(Call_Inner_ChangeValueWithScopeTester, char* whoToChange)
{
	ChangeValueWithScopeTester(whoToChange);
}
END_FUN;


TEST_FUN_IMPL(Infra_ScopesTest, SCOPE_END__WhenObjectsDefinedInsideScope_ThenAllGetFreed)
{
	//Arrange
	char feedback[3] = { 0, 0, 0 };

	// Act
	ChangeValueWithScopeTester(feedback + 0);
	ChangeValueWithScopeTester(feedback + 1);
	ChangeValueWithScopeTester(feedback + 2);

	// Assert
	// - The dtor of ScopeTester sets the feedback vals again:
	NTEST_ASSERT(feedback[0] == 0);
	NTEST_ASSERT(feedback[1] == 0);
	NTEST_ASSERT(feedback[2] == 0);
}
END_FUN

TEST_FUN_IMPL(Infra_ScopesTest, LOCAL_SCOPE__WhenObjectsDefinedInside_InnerScope_ThenAllGetFreed)
{
	// Arrange:
	char feedback[4] = { 0, 0, 0, 0 };

	// Sanity:
	CREATE(ScopeTester, outer_scope_obj), feedback + 0 CALL;

	// Act:
	Call_Inner_ChangeValueWithScopeTester(feedback);
	ChangeValueWithScopeTester(feedback + 1);
	ChangeValueWithScopeTester(feedback + 2);
	ChangeValueWithScopeTester(feedback + 3);

	// Assert
	// - The dtor of ScopeTester sets the feedback val to zero again:
	NTEST_ASSERT(feedback[0] == 0);
	NTEST_ASSERT(feedback[1] == 0);
	NTEST_ASSERT(feedback[2] == 0);
	NTEST_ASSERT(feedback[3] == 0);

}
END_FUN;

TEST_FUN_IMPL(Infra_ScopesTest, LOCAL_SCOPE__DoesNotFreeUnrelatedObjects)
{
	char feedback[3] = { 0, 0, 0 };

	IF(true) {

		//Arrange
		CREATE(ScopeTester, outer_obj1), feedback + 0 CALL;
		CREATE(ScopeTester, outer_obj2), feedback + 1 CALL;

		//Act
		IF(1)
		{
			CREATE(ScopeTester, inner_obj), feedback + 2 CALL;
		}END_IF;

		//Assert
		// Outer objects were not yet freed:
		NTEST_ASSERT(feedback[0] != 0);
		NTEST_ASSERT(feedback[1] != 0);

	}END_IF;
}
END_FUN

TEST_FUN_IMPL(Infra_ScopesTest, LOCAL_SCOPE__WhenMultipleNestedScopesExist_ThenFreesOnlyTheInnerMost)
{
	char feedback[4] = { 0, 0, 0, 0 };

	//Arrange 
	IF(0 == 0) {
		CREATE(ScopeTester, outer_scope_obj), feedback + 0 CALL;
		NTEST_ASSERT(feedback[0] == 'A');

		//Act
		IF(1 == 1) {

			CREATE(ScopeTester, inner_scope_1), feedback + 1 CALL;
			NTEST_ASSERT(feedback[1] == 'A');

			FOR(int i = 2; i < 3; i++)
			{
				CREATE(ScopeTester, inner_scope_2), feedback + 2 CALL;
				NTEST_ASSERT(feedback[2] == 'A');

				//Assert
				NTEST_ASSERT(feedback[0] != 0);
				NTEST_ASSERT(feedback[1] != 0);
				NTEST_ASSERT(feedback[2] != 0);
			}END_IF;

			NTEST_ASSERT(feedback[0] != 0);
			NTEST_ASSERT(feedback[1] != 0);
			NTEST_ASSERT(feedback[2] == 0);


		}END_IF;
		NTEST_ASSERT(feedback[0] != 0);
		NTEST_ASSERT(feedback[1] == 0);
		NTEST_ASSERT(feedback[2] == 0);

	}END_IF;

	NTEST_ASSERT(feedback[0] == 0);
	NTEST_ASSERT(feedback[1] == 0);
	NTEST_ASSERT(feedback[2] == 0);
}
END_FUN

TEST_FUN_IMPL(Infra_ScopesTest, LOCAL_SCOPE__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash)
{
	char feedback[4] = { 0, 0, 0, 0 };

	//Arrange 
	CREATE(ScopeTester, outer_scope_obj), feedback + 0 CALL;
	NTEST_ASSERT(feedback[0] == 'A');

	//Act
	IF(true) {

		CREATE(ScopeTester, inner_scope_1), feedback + 1 CALL;
		IF(true) {


		}END_IF;

		// Assert
		NTEST_ASSERT(feedback[1] != 0);

	}END_IF;

	NTEST_ASSERT(feedback[1] == 0);
}
END_FUN

TEST_FUN_IMPL(Infra_ScopesTest, LOCAL_SCOPE__LoopScopeSanityTest)
{
	char feedback[4] = { 0, 0, 0, 0 };

	FOR(int i = 0; i < 100; i++)
	{
		CREATE(ScopeTester, inner_scope_1), feedback + i % 4 CALL;

	}END_LOOP;


	FOR(int i = 0; i < 4; i++)
		NTEST_ASSERT(feedback[i] == 0);
	END_LOOP;
}
END_FUN

INIT_TEST_SUITE(Infra_ScopesTest);
BIND_TEST(Infra_ScopesTest, SCOPE_END__WhenObjectsDefinedInsideScope_ThenAllGetFreed);
BIND_TEST(Infra_ScopesTest, LOCAL_SCOPE__WhenObjectsDefinedInside_InnerScope_ThenAllGetFreed);
BIND_TEST(Infra_ScopesTest, LOCAL_SCOPE__DoesNotFreeUnrelatedObjects);
BIND_TEST(Infra_ScopesTest, LOCAL_SCOPE__WhenMultipleNestedScopesExist_ThenFreesOnlyTheInnerMost);
BIND_TEST(Infra_ScopesTest, LOCAL_SCOPE__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash);
BIND_TEST(Infra_ScopesTest, LOCAL_SCOPE__LoopScopeSanityTest);
END_INIT_TEST_SUITE(Infra_ScopesTest);
