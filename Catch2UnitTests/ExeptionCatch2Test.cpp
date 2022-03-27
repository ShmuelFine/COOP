#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("Exception_is_thrown_as_well", "[Exceptions]") 
	{
		int tester;
		SECTION("Exception_WhenThrown_ThenGoesStraightToScopeEnd") {
			auto ret = Exception_WhenThrown_ThenGoesStraightToScopeEnd(&tester);
			CHECK_THROW(IN_THROWING_VALUE, ret);
			CHECK(0, tester);
		}
		SECTION("Exception_WhenThrownFromWithinFunction_ThenGoesStraightToScopeEnd") {
			auto ret = Exception_WhenThrownFromWithinFunction_ThenGoesStraightToScopeEnd(&tester);
			CHECK_THROW(IN_THROWING_VALUE, ret);
			CHECK(0, tester);
		}
		SECTION("CATCH_WhenExeptionCaught_Does_NOT_ContinueThrowing") {
			auto ret = CATCH_WhenExeptionCaught_Does_NOT_ContinueThrowing();
			CHECK(SUCCESS_VALUE, ret);
		}
		SECTION("Exception_WhenUsingTHROW_MSG_ThenTheMessageIsSaved") {
			std::string errMsg = "This is a test error";
			char outMsg[100];
			Exception_WhenUsingTHROW_MSG_ThenTheMessageIsSaved(errMsg.c_str(), outMsg);
			std::string outMsgStr = outMsg;
			CHECK(errMsg, outMsgStr);
		}
		SECTION("BREAK_WhenDoneFromNastedLoop_ThenBreaksCorrectly") {
			CHECK_FALSE(Class_CallSimpleFunction_SanityTest());
		}
		SECTION("BREAK_WhenDoneFromTRY_Catch_Block_ThenBreaksCorrectly") {
			CHECK_FALSE(Class_CallSimpleFunction_SanityTest());
		}
		SECTION("BREAK_WhenDoneFromLoop_ThenFreesObjectsFromInnerScope") {
			CHECK_FALSE(Class_CallSimpleFunction_SanityTest());
		}
		
	}