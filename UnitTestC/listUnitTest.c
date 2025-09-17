#include "listUnitTest.h"
#include "ExportDefs.h"
#include "List.h"
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
END_INIT_TEST_SUITE(ListTest)
