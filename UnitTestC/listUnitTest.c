#include "listUnitTest.h"
#include "ExportDefs.h"
#include "List.h"
#include "Iterator.h"
#include "ScopeTester.h"


TEST_FUN_IMPL(ListTest, push_back_SanityTest)
{
    CREATE(List_int, lst_int) CALL;
    CREATE(List_char, lst_char) CALL;
    int numElements = 54;

    FOR(int i = 0; i < numElements; i++)
    {
        MFUN(&lst_int, push_back), i CALL;
        MFUN(&lst_char, push_back), i CALL;

    } END_LOOP;

    MEM_SIZE_T curr_size = 0;
    MFUN(&lst_int, size), & curr_size CALL;
    MFUN(&lst_char, size), & curr_size CALL;
    NTEST_ASSERT(curr_size == numElements);

    int v = -1;
    MFUN(&lst_int, front), & v CALL; NTEST_ASSERT(v == 0);
    MFUN(&lst_char, front), & v CALL; NTEST_ASSERT(v == 0);
    v = -1;
    MFUN(&lst_int, back), & v CALL; NTEST_ASSERT(v == (numElements - 1));
    MFUN(&lst_char, back), & v CALL; NTEST_ASSERT(v == (numElements - 1));
} END_FUN


TEST_FUN_IMPL(ListTest, pop_back_SanityTest)
{
    CREATE(List_int, lst) CALL;
    int numElements = 54;
    FOR(int i = 0; i < numElements; i++) { MFUN(&lst, push_back), i CALL; } END_LOOP;

    FOR(int i = 0; i < numElements; i++)
    {
        int val = 0;
        MFUN(&lst, pop_back), & val CALL;
        NTEST_ASSERT(val == (numElements - 1) - i);
    } END_LOOP;

    MEM_SIZE_T sz = (MEM_SIZE_T)-1;
    MFUN(&lst, size), & sz CALL;
    NTEST_ASSERT(sz == 0);
} END_FUN


TEST_FUN_IMPL(ListTest, push_front_SanityTest)
{
    CREATE(List_int, lst) CALL;

    FOR(int i = 0; i < 10; i++)
    {
        MFUN(&lst, push_front), i CALL;
    } END_LOOP;

    MEM_SIZE_T sz = 0;
    MFUN(&lst, size), & sz CALL; NTEST_ASSERT(sz == 10);

    int v = -1;
    MFUN(&lst, front), & v CALL; NTEST_ASSERT(v == 9);
    v = -1;
    MFUN(&lst, back), & v CALL; NTEST_ASSERT(v == 0);
} END_FUN


TEST_FUN_IMPL(ListTest, pop_front_SanityTest)
{
    CREATE(List_int, lst) CALL;
    FOR(int i = 0; i < 8; i++) { MFUN(&lst, push_back), i CALL; } END_LOOP;

    FOR(int i = 0; i < 8; i++)
    {
        int val = 0;
        MFUN(&lst, pop_front), & val CALL;
        NTEST_ASSERT(val == i);
    } END_LOOP;

    MEM_SIZE_T sz = (MEM_SIZE_T)-1;
    MFUN(&lst, size), & sz CALL; NTEST_ASSERT(sz == 0);
} END_FUN


TEST_FUN_IMPL(ListTest, front_back_SanityTest)
{
    CREATE(List_int, lst) CALL;

    MFUN(&lst, push_back), 10 CALL;
    MFUN(&lst, push_back), 20 CALL;
    MFUN(&lst, push_front), 5 CALL;

    int v = -1;
    MFUN(&lst, front), & v CALL; NTEST_ASSERT(v == 5);
    v = -1;
    MFUN(&lst, back), & v CALL; NTEST_ASSERT(v == 20);
} END_FUN


TEST_FUN_IMPL(ListTest, size_empty_clear_SanityTest)
{
    CREATE(List_int, lst) CALL;

    bool empty = false;
    MFUN(&lst, empty), & empty CALL; NTEST_ASSERT(empty == true);

    FOR(int i = 0; i < 5; ++i) { MFUN(&lst, push_back), i CALL; } END_LOOP;

    MEM_SIZE_T sz = 0;
    MFUN(&lst, size), & sz CALL;     NTEST_ASSERT(sz == 5);
    MFUN(&lst, empty), & empty CALL; NTEST_ASSERT(empty == false);

    MFUN(&lst, clear) CALL;

    MFUN(&lst, size), & sz CALL;     NTEST_ASSERT(sz == 0);
    MFUN(&lst, empty), & empty CALL; NTEST_ASSERT(empty == true);
} END_FUN


TEST_FUN_IMPL(ListTest, front_onEmpty_Throws)
{
    CREATE(List_int, lst) CALL;

    int v = 0;
    EXPECT_THROW;
    MFUN(&lst, front), & v CALL;
    ASSERT_THROW;
} END_FUN


TEST_FUN_IMPL(ListTest, pop_front_onEmpty_Throws)
{
    CREATE(List_int, lst) CALL;

    int v = 0;
    EXPECT_THROW;
    MFUN(&lst, pop_front), & v CALL;
    ASSERT_THROW;
} END_FUN


TEST_FUN_IMPL(ListTest, pop_back_onEmpty_Throws)
{
    CREATE(List_int, lst) CALL;

    int v = 0;
    EXPECT_THROW;
    MFUN(&lst, pop_back), & v CALL;
    ASSERT_THROW;
} END_FUN


TEST_FUN_IMPL(ListTest, dtor_freesAllMemory)
{
    FUN(init_global_memory) sizeof(int) * 1000, LIMITED_SIZE_MEMORY CALL;

    MEM_SIZE_T free_bytes_at_start = 0, free_bytes_at_end = 0;
    FUN(get_total_free_bytes)& free_bytes_at_start CALL;

    FOR(int k = 0; k < 10; k++)
    {
        CREATE(List_int, lst) CALL;
        FOR(int i = 0; i < 5; i++) { MFUN(&lst, push_back), i CALL; } END_LOOP;
    } END_LOOP;

    FUN(get_total_free_bytes)& free_bytes_at_end CALL;
    NTEST_ASSERT(free_bytes_at_end == free_bytes_at_start);
} END_FUN

TEST_FUN_IMPL(ListTest, print_SanityTest)
{
    CREATE(List_int, lst_int) CALL;
    CREATE(List_char, lst_char) CALL;
    CREATE(List_float, lst_float) CALL;
    int numElements = 10;

    FOR(int i = 0; i < numElements; i++)
    {
        MFUN(&lst_int, push_back), i CALL;
        MFUN(&lst_char, push_back), i*1.2 CALL;
        MFUN(&lst_float, push_back), i * 1.2 CALL;
    } END_LOOP;

    MFUN(&lst_int, print) CALL;
    MFUN(&lst_char, print) CALL;
    MFUN(&lst_float, print) CALL;

} END_FUN

TEST_FUN_IMPL(ListTest, iter_nextPrev_MoveOK_andPrevThrowsAtBegin)
{
    CREATE(List_int, lst) CALL;
    FOR(int i = 0; i < 5; i++) { MFUN(&lst, push_back), i CALL; } END_LOOP;

    Iterator* it = (Iterator*)&(lst._base.begin_iter);
    Iterator* end = (Iterator*)&(lst._base.end_iter);

    /* prev at begin must throw */
    EXPECT_THROW;
    MFUN(it, prev) CALL;
    ASSERT_THROW;

    /* move 2 steps with next; not yet at end */
    MFUN(it, next) CALL;
    MFUN(it, next) CALL;
    bool eq = false;
    MFUN(it, equals), end, & eq CALL;
    NTEST_ASSERT(!eq);

    /* advance one more */
    MFUN(it, next) CALL;
    MFUN(it, equals), end, & eq CALL;
    NTEST_ASSERT(!eq);
} END_FUN


TEST_FUN_IMPL(ListTest, iter_getRef_getCref_PointsToCurrent)
{
    CREATE(List_int, lst) CALL;
    FOR(int i = 0; i < 5; i++) { MFUN(&lst, push_back), i CALL; } END_LOOP;

    Iterator* it = (Iterator*)&(lst._base.begin_iter);

    /* at begin -> value 0 */
    const void* cptr = NULL;
    MFUN(it, get_cref), & cptr CALL;
    NTEST_ASSERT(cptr != NULL);
    NTEST_ASSERT(*(const int*)cptr == 0);

    /* advance to index 2, expect value 2 */
    MFUN(it, advance), (ptrdiff_t)2 CALL;
    void* ptr = NULL;
    MFUN(it, get_ref), & ptr CALL;
    NTEST_ASSERT(ptr != NULL);
    NTEST_ASSERT(*(int*)ptr == 2);
} END_FUN


TEST_FUN_IMPL(ListTest, iter_distance_And_Advance_Bounds)
{
    CREATE(List_int, lst) CALL;
    FOR(int i = 0; i < 6; i++) { MFUN(&lst, push_back), i CALL; } END_LOOP;

    Iterator* b = (Iterator*)&(lst._base.begin_iter);
    Iterator* e = (Iterator*)&(lst._base.end_iter);

    /* distance(begin, end) == size */
    ptrdiff_t dist = -999;
    MFUN(b, distance), e, & dist CALL;
    NTEST_ASSERT(dist == 6);

    /* advance 3 -> remaining distance 3 */
    MFUN(b, advance), (ptrdiff_t)3 CALL;
    dist = -999;
    MFUN(b, distance), e, & dist CALL;
    NTEST_ASSERT(dist == 3);

} END_FUN



INIT_TEST_SUITE(ListTest)
BIND_TEST(ListTest, push_back_SanityTest);
BIND_TEST(ListTest, pop_back_SanityTest);
BIND_TEST(ListTest, push_front_SanityTest);
BIND_TEST(ListTest, pop_front_SanityTest);
BIND_TEST(ListTest, front_back_SanityTest);
BIND_TEST(ListTest, size_empty_clear_SanityTest);
BIND_TEST(ListTest, print_SanityTest);
BIND_TEST(ListTest, front_onEmpty_Throws);
BIND_TEST(ListTest, pop_front_onEmpty_Throws);
BIND_TEST(ListTest, pop_back_onEmpty_Throws);
BIND_TEST(ListTest, dtor_freesAllMemory);
BIND_TEST(ListTest, iter_nextPrev_MoveOK_andPrevThrowsAtBegin);
BIND_TEST(ListTest, iter_getRef_getCref_PointsToCurrent);
BIND_TEST(ListTest, iter_distance_And_Advance_Bounds);
END_INIT_TEST_SUITE(ListTest)
