#include "ExeptionTest.h"
#include "ScopeTest.h"
#include "Globals.h"

int WhenexeptionIsThrown_ThenGoesStriehtToScopeEnd()
{
	int check = 0;
	SCOPE_START;
	LOCAL_SCOPE_START;
	/*_CurrScope_Idx++;
	if (!setjmp(SCOPE_FALLBACK_ADDR[_CurrScope_Idx])) {

			object object1; object1.vTable = NULL; object1._next = NULL;
			_scope_obj_list_add(&_scope_obj_list, &object1);*/
	THROW;
	check++;


	LOCAL_SCOPE_END;
	check++;


	SCOPE_END;

	return(0 == check);
}

#define CREATE_OBJECT4TEST(type, instance_name, ...)		        \
	if (! is_ ##type ##VirtualTable__initialized) type ##_init();   \
	instance_name.vTable=&type ##VTable;							\
	instance_name.vTable->_ctor(&instance_name, __VA_ARGS__);		\
	instance_name._next= NULL;										\
	REGISTER_OBJECT(&instance_name)


int WhenexeptionIsThrownWithInAFunctionCall_ThenGoesStriehtToScopeEnd()
{
	int check = 0;
	ScopeTest s;
	CreateGlobalCache(100, "GlobalCache", IN_MEMORY_CACHE_);

	SCOPE_START;
	CREATE_OBJECT4TEST(ScopeTest, s, 20);
	LOCAL_SCOPE_START;

	CALL(ThrowingIfEQ, s, 3, 3);

	check++;
	FreeMostInnerScope(&_scope_obj_list); _CurrScope_Idx--;

	LOCAL_SCOPE_END;
	check++;
	SCOPE_END;

	DestroyGlobalCache();
	return(0 == check && NULL == s.buff);
}

int WhenCatchingExeption_DoesntContinueThrowing()
{
	//Arrange
	int check = 0;
	ScopeTest s;

	SCOPE_START;
	LOCAL_SCOPE_START;
	TRY{
		CREATE_OBJECT4TEST(ScopeTest,s,20);
	CALL(ThrowingIfEQ,s, 3, 3);
	}CATCH; END_TRY;
	
	check++;
	
	LOCAL_SCOPE_END;
	
	check++;
	
	return(2 == check);

	SCOPE_END;
}