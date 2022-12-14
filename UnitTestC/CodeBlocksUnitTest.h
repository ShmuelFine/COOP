#ifndef __COOP_CODE_BLOCKS_UNIT_TEST_H__
#define __COOP_CODE_BLOCKS_UNIT_TEST_H__

#include "UnitTestsInfra.h"
#include "COOP.h"

DEF_TEST_SUITE(CodeBlocksTest);
ADD_TEST(CodeBlocksTest, functionScope_FreesMemory);
ADD_TEST(CodeBlocksTest, forLoop_FreesMemory);
ADD_TEST(CodeBlocksTest, nestedLoop_FreesMemory);
ADD_TEST(CodeBlocksTest, forLoopWith_BREAK_FreesMemory);
ADD_TEST(CodeBlocksTest, forLoopWith_RETURN_FreesMemory);
ADD_TEST(CodeBlocksTest, nestedForLoopWith_RETURN_FreesMemory);
END_TEST_SUITE(CodeBlocksTest);

#endif // !__COOP_CODE_BLOCKS_UNIT_TEST_H__
