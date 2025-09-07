#include "IteratorUnitTest.h"
#include "ExportDefs.h"
#include "ScopeTester.h"


TEST_FUN_IMPL(IteratorForTest, foreach_SumsAll)
{
    CREATE(Vector_int, v) CALL;

    FOR(int i = 1; i <= 5; ++i) {
        MFUN(&v, push_back), i CALL;
    } END_LOOP

    int sum = 0;
    ITER_FOR(void*, val, &v)
    {
       sum += (int)val;
    } END_ITER_FOR

    NTEST_ASSERT(sum == 1 + 2 + 3 + 4 + 5);
}END_FUN;

TEST_FUN_IMPL(IteratorForTest, foreach_ContinueSkips)
{
    CREATE(Vector_int, vec) CALL;
    FOR(int i = 0; i < 5; i++) {
        MFUN(&vec, push_back), i CALL;
    } END_LOOP;

    int sum = 0;

    ITER_FOR(int, val, &vec)
    {
        IF(val == 2 || val == 4) {
            ITER_CONTINUE;
		} END_IF

         sum += val;
    }END_ITER_FOR
    ASSERT(sum == (0 +1+3)); 
}END_FUN;

TEST_FUN_IMPL(IteratorForTest, foreach_BreakStopsEarly)
{
    CREATE(Vector_int, vec) CALL;
    FOR(int i = 0; i < 10; i++) {
        MFUN(&vec, push_back), i CALL;
    } END_LOOP;

    int sum = 0;


	ITER_FOR(int, val, &vec)
    {
        IF(val >= 4) {
           BREAK;
        } END_IF

        sum += val;
    }END_ITER_FOR
  

    ASSERT(sum == (0 + 1 + 2 + 3));
}END_FUN;


TEST_FUN_IMPL(IteratorForTest, foreach_EmptyRange)
{
    CREATE(Vector_int, vec) CALL;

    int count = 0;

    ITER_FOR(int, val,&vec)
    {
        count++;
    }END_ITER_FOR


    ASSERT(count == 0);
}END_FUN;




INIT_TEST_SUITE(IteratorForTest)
	BIND_TEST(IteratorForTest, foreach_SumsAll);
	BIND_TEST(IteratorForTest, foreach_ContinueSkips);
    BIND_TEST(IteratorForTest, foreach_BreakStopsEarly);
	BIND_TEST(IteratorForTest, foreach_EmptyRange);
END_INIT_TEST_SUITE(IteratorForTest)

