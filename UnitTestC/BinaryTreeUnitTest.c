#include "BinaryTreeUnitTest.h"
#include "ExportDefs.h"
#include "BinaryTree.h"
#include "DynamicMemoryManagement.h"
#include "Iterator.h"


TEST_FUN_IMPL(BinaryTreeTest, insert_SanityTest)
{
    // Arrange
    CREATE(BTree_int, bt) CALL;

    // Act
	int num = 7;
    FOR(int i = 1; i <= num; ++i)
    {
        MFUN(&bt, insert), i CALL;
    }
    END_LOOP;

    // Assert
    bool isEmpty = true;
    MFUN(&bt, is_empty), &isEmpty CALL;
    NTEST_ASSERT(!isEmpty);

    MEM_SIZE_T size= 0;
    MFUN(&bt, get_size), &size CALL;
    NTEST_ASSERT(size == (MEM_SIZE_T)num);

} END_FUN


TEST_FUN_IMPL(BinaryTreeTest, removeLeaf_SanityTest)
{
    // Arrange
    CREATE(BTree_int, bt) CALL;
    FOR(int i = 1; i <= 5; ++i)
    {
        MFUN(&bt, insert), i CALL;
    }
    END_LOOP;

    // Act
    bool removed = false;
    MFUN(&bt, remove), 5, &removed CALL;

    // Assert
    NTEST_ASSERT(removed == true);

    MEM_SIZE_T size = 999;
    MFUN(&bt, get_size), &size CALL;
    NTEST_ASSERT(size == 4);

    /* Another attempt to delete the same value should return false */
    removed = true;
    MFUN(&bt, remove), 5, &removed CALL;
    NTEST_ASSERT(removed == false);

} END_FUN


TEST_FUN_IMPL(BinaryTreeTest, removeRoot_SanityTest)
{
    // Arrange
    CREATE(BTree_int, bt) CALL;
    FOR(int i = 1; i <= 3; ++i)
    {
        MFUN(&bt, insert), i CALL;
    }
    END_LOOP; /* 1; 2,3 */

    // Act
    bool removed = false;
    MFUN(&bt, remove), 1, &removed CALL;

    // Assert
    NTEST_ASSERT(removed == true);

    MEM_SIZE_T size = 0;
    MFUN(&bt, get_size), &size CALL;
    NTEST_ASSERT(size == 2);

    /* After deleting the root, another deletion of 1 should fail */
    removed = true;
    MFUN(&bt, remove), 1, &removed CALL;
    NTEST_ASSERT(removed == false);

} END_FUN

TEST_FUN_IMPL(BinaryTreeTest, remove_onEmpty_tree)
{
    // Arrange
    CREATE(BTree_int, bt) CALL;

    // Act
    bool removed = true; 
    MFUN(&bt, remove), 42, &removed CALL;

    // Assert
    NTEST_ASSERT(removed == false);

}
END_FUN

TEST_FUN_IMPL(BinaryTreeTest, iterators_beginEqualsEnd_onEmpty)
{
    // Arrange
    CREATE(BTree_int, bt) CALL;

    // Act
    Iterator *begin = (Iterator*)&bt._base.begin_iter;
    Iterator *end = (Iterator*)&bt._base.end_iter;

    bool is_equals = false;
    MFUN(begin, equals), end, & is_equals CALL;

    // Assert
    NTEST_ASSERT(is_equals == true);

} END_FUN


TEST_FUN_IMPL(BinaryTreeTest, prev_from_end_returns_rightmost)
{
    // Arrange
    CREATE(BTree_int, bt) CALL;
    FOR(int i = 1; i <= 5; ++i)
    {
        MFUN(&bt, insert), i CALL;
    }
    END_LOOP; /* expected inorder: 4,2,5,1,3 <- rightmost 3 */

    // Act
    Iterator *end = (Iterator*)&bt._base.end_iter;
    MFUN(end, prev) CALL;

    void *last_value = NULL;
    MFUN(end, get_ref), &last_value CALL;

    // Assert
    THROW_MSG_UNLESS(last_value, "Iterator ref must not be NULL");
    NTEST_ASSERT(*(int*)last_value == 3);
} END_FUN


TEST_FUN_IMPL(BinaryTreeTest, iterator_modifyThroughRef_persists)
{
    // Arrange
    CREATE(BTree_int, bt) CALL;
    FOR(int i = 1; i <= 5; ++i)
    {
        MFUN(&bt, insert), i CALL;
    }
    END_LOOP; /* inorder starts at the first value (4) */

    // Act
    Iterator *iter = (Iterator*)&bt._base.begin_iter;

    void *iter_value = NULL;
    MFUN(iter, get_ref), &iter_value CALL;
    THROW_MSG_UNLESS(iter_value, "Iterator ref must not be NULL");

    int before = *(int*)iter_value;
    *(int*)iter_value += 100;

    /* fetch again to ensure persistence */
    iter_value = NULL;
    MFUN(iter, get_ref), & iter_value CALL;
    THROW_MSG_UNLESS(iter_value, "Iterator ref must not be NULL (2)");
    int after = *(int*)iter_value;

    // Assert
    NTEST_ASSERT(after == before + 100);
} END_FUN

TEST_FUN_IMPL(BinaryTreeTest, iterator_advance_WorksAndReachesEnd)
{
    // Arrange
    CREATE(BTree_int, bt) CALL;
    const int N = 8;
    FOR(int i = 1; i <= N; ++i) 
    {
        MFUN(&bt, insert), i CALL; 
    }
    END_LOOP;

    Iterator *begin = (Iterator*)&bt._base.begin_iter;
    Iterator *end = (Iterator*)&bt._base.end_iter;

    // Act + Assert
    /* advance(0) doesn't change anything */
    MFUN(begin, advance), 0 CALL;
    bool is_equals = false;
    MFUN(begin, equals), end, &is_equals CALL;
    NTEST_ASSERT(is_equals == false);

    /* advance(N - 1) not yet at the end */
    MFUN(begin, advance), (N - 1) CALL;
    MFUN(begin, equals), end, &is_equals CALL;
    NTEST_ASSERT(is_equals == false);

    /* one more step comes to an end */
    MFUN(begin, advance), 1 CALL;
    MFUN(begin, equals), end, &is_equals CALL;
    NTEST_ASSERT(is_equals == true);
}
END_FUN


TEST_FUN_IMPL(BinaryTreeTest, iterator_distance_BeginToEndEqualsSize)
{
    // Arrange
    CREATE(BTree_int, bt) CALL;
    const int N = 6;
    FOR(int i = 1; i <= N; ++i) 
    {
        MFUN(&bt, insert), i CALL; 
    }
    END_LOOP;

    Iterator *begin = (Iterator*)&bt._base.begin_iter;
    Iterator *end = (Iterator*)&bt._base.end_iter;

    long dist = -12345;

    // Act + Assert
    /* distance(begin, begin) == 0 */
    MFUN(begin, distance), begin, &dist CALL;
    NTEST_ASSERT(dist == 0);

    /* distance(begin, end) == N */
    dist = -1;
    MFUN(begin, distance), end, &dist CALL;
    NTEST_ASSERT(dist == (long)N);

    /* distance(end, end) == 0 */
    dist = -1;
    MFUN(end, distance), end, &dist CALL;
    NTEST_ASSERT(dist == 0);
}
END_FUN



TEST_FUN_IMPL(BinaryTreeTest, traversals_DoNotCrash)
{
    // Arrange
    CREATE(BTree_int, bt) CALL;
    FOR(int i = 1; i <= 8; ++i)
    {
        MFUN(&bt, insert), i CALL;
    }
    END_LOOP;

	bool removed = false;
    MFUN(&bt, remove), 3, &removed CALL;
    MFUN(&bt, remove), 1, &removed CALL;

    // Act (no output check here; just that the calls don't throw)
	MFUN(&bt, print), PRE CALL;    printf("\n");
	MFUN(&bt, print), IN CALL;     printf("\n");
	MFUN(&bt, print), POST CALL;   printf("\n");

    // Assert
    NTEST_ASSERT(1);

} END_FUN

TEST_FUN_IMPL(BinaryTreeTest, print_onEmpty)
{
    // Arrange
    CREATE(BTree_int, bt) CALL;

    // Act – just make sure there is no crash/exception
    MFUN(&bt, print), PRE CALL;
    MFUN(&bt, print), IN  CALL;
    MFUN(&bt, print), POST CALL;

    // Assert
    NTEST_ASSERT(1);
}
END_FUN

TEST_FUN_IMPL(BinaryTreeTest, dtor_freesAllMemory)
{
    FUN(init_global_memory) sizeof(int)  *1000, LIMITED_SIZE_MEMORY CALL;

    MEM_SIZE_T free_start = 0, free_end = 0;
    FUN(get_total_free_bytes) &free_start CALL;

    FOR(int k = 0; k < 10; ++k)
    {
        CREATE(BTree_int, bt) CALL;

        FOR(int i = 0; i < 17; ++i)
        {
            MFUN(&bt, insert), i CALL;
        }
        END_LOOP;

    }
    END_LOOP;


    FUN(get_total_free_bytes) &free_end CALL;
    NTEST_ASSERT(free_end == free_start);
}
END_FUN


/* ========= Suite Binding ========= */

INIT_TEST_SUITE(BinaryTreeTest)
BIND_TEST(BinaryTreeTest, insert_SanityTest);
BIND_TEST(BinaryTreeTest, removeLeaf_SanityTest);
BIND_TEST(BinaryTreeTest, removeRoot_SanityTest);
BIND_TEST(BinaryTreeTest, iterators_beginEqualsEnd_onEmpty);
BIND_TEST(BinaryTreeTest, prev_from_end_returns_rightmost);
BIND_TEST(BinaryTreeTest, iterator_modifyThroughRef_persists);
BIND_TEST(BinaryTreeTest, traversals_DoNotCrash);
BIND_TEST(BinaryTreeTest, dtor_freesAllMemory);
BIND_TEST(BinaryTreeTest, iterator_advance_WorksAndReachesEnd);
BIND_TEST(BinaryTreeTest, iterator_distance_BeginToEndEqualsSize);
BIND_TEST(BinaryTreeTest, remove_onEmpty_tree);
BIND_TEST(BinaryTreeTest, print_onEmpty);
END_INIT_TEST_SUITE(BinaryTreeTest)