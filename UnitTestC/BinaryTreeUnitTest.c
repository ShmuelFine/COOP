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
    MFUN(&bt, is_empty), & isEmpty CALL;
    NTEST_ASSERT(!isEmpty);

    MEM_SIZE_T sz = 0;
    MFUN(&bt, get_size), & sz CALL;
    NTEST_ASSERT(sz == (MEM_SIZE_T)num);

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
    MFUN(&bt, remove), 5, & removed CALL;

    // Assert
    NTEST_ASSERT(removed == true);

    MEM_SIZE_T sz = 999;
    MFUN(&bt, get_size), & sz CALL;
    NTEST_ASSERT(sz == 4);

    /* Another attempt to delete the same value should return false */
    removed = true;
    MFUN(&bt, remove), 5, & removed CALL;
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
    MFUN(&bt, remove), 1, & removed CALL;

    // Assert
    NTEST_ASSERT(removed == true);

    MEM_SIZE_T sz = 0;
    MFUN(&bt, get_size), & sz CALL;
    NTEST_ASSERT(sz == 2);

    /* After deleting the root, another deletion of 1 should fail */
    removed = true;
    MFUN(&bt, remove), 1, & removed CALL;
    NTEST_ASSERT(removed == false);

} END_FUN

TEST_FUN_IMPL(BinaryTreeTest, remove_onEmpty_tree)
{
    // Arrange
    CREATE(BTree_int, bt) CALL;

    // Act
    bool removed = true; 
    MFUN(&bt, remove), 42, & removed CALL;

    // Assert
    NTEST_ASSERT(removed == false);

}
END_FUN

TEST_FUN_IMPL(BinaryTreeTest, iterators_beginEqualsEnd_onEmpty)
{
    // Arrange
    CREATE(BTree_int, bt) CALL;

    // Act
    object* b = NULL;
    object* e = NULL;
    MFUN(&bt, begin_inorder), & b CALL;
    MFUN(&bt, end_inorder), & e CALL;

    bool eq = false;
    MFUN((Iterator*)b, equals), e, & eq CALL;

    // Assert
    NTEST_ASSERT(eq == true);

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
    object* e = NULL;
    MFUN(&bt, end_inorder), & e CALL;
    MFUN((Iterator*)e, prev) CALL;

    void* p = NULL;
    MFUN((Iterator*)e, get_ref), & p CALL;

    // Assert
    THROW_MSG_UNLESS(p, "Iterator ref must not be NULL");
    NTEST_ASSERT(*(int*)p == 3);
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
    object* it = NULL;
    MFUN(&bt, begin_inorder), & it CALL;

    void* p = NULL;
    MFUN((Iterator*)it, get_ref), & p CALL;
    THROW_MSG_UNLESS(p, "Iterator ref must not be NULL");

    int before = *(int*)p;
    *(int*)p += 100;

    /* call again to make sure the change survives */
    p = NULL;
    MFUN((Iterator*)it, get_ref), & p CALL;
    THROW_MSG_UNLESS(p, "Iterator ref must not be NULL (2)");
    int after = *(int*)p;

    // Assert
    NTEST_ASSERT(after == before + 100);
} END_FUN

TEST_FUN_IMPL(BinaryTreeTest, iterator_advance_WorksAndReachesEnd)
{
    // Arrange
    CREATE(BTree_int, bt) CALL;
    const int N = 8;
    FOR(int i = 1; i <= N; ++i) { MFUN(&bt, insert), i CALL; } END_LOOP;

    object* b = NULL;
    object* e = NULL;
    MFUN(&bt, begin_inorder), & b CALL;
    MFUN(&bt, end_inorder), & e CALL;

    // Act + Assert
    /* advance(0) doesn't change anything */
    MFUN((Iterator*)b, advance), 0 CALL;
    bool eq = false;
    MFUN((Iterator*)b, equals), e, & eq CALL;
    NTEST_ASSERT(eq == false);

    /* advance(N - 1) not yet at the end */
    MFUN((Iterator*)b, advance), (N - 1) CALL;
    MFUN((Iterator*)b, equals), e, & eq CALL;
    NTEST_ASSERT(eq == false);

    /* one more step comes to an end */
    MFUN((Iterator*)b, advance), 1 CALL;
    MFUN((Iterator*)b, equals), e, & eq CALL;
    NTEST_ASSERT(eq == true);
}
END_FUN


TEST_FUN_IMPL(BinaryTreeTest, iterator_distance_BeginToEndEqualsSize)
{
    // Arrange
    CREATE(BTree_int, bt) CALL;
    const int N = 6;
    FOR(int i = 1; i <= N; ++i) { MFUN(&bt, insert), i CALL; } END_LOOP;

    object* b = NULL;
    object* e = NULL;
    MFUN(&bt, begin_inorder), & b CALL;
    MFUN(&bt, end_inorder), & e CALL;

    long d = -12345;

    // Act + Assert
    /* distance(begin, begin) == 0 */
    MFUN((Iterator*)b, distance), b, & d CALL;
    NTEST_ASSERT(d == 0);

    /* distance(begin, end) == N */
    d = -1;
    MFUN((Iterator*)b, distance), e, & d CALL;
    NTEST_ASSERT(d == (long)N);

    /* distance(end, end) == 0 */
    d = -1;
    MFUN((Iterator*)e, distance), e, & d CALL;
    NTEST_ASSERT(d == 0);
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
    MFUN(&bt, remove), 3, & removed CALL;
    MFUN(&bt, remove), 1, & removed CALL;

    // Act (no output check here; just that the calls don't throw)
    MFUN(&bt, traverse_pre) CALL;  printf("\n");
	MFUN(&bt, print), PRE CALL;    printf("\n");

    MFUN(&bt, traverse_in) CALL;   printf("\n");
	MFUN(&bt, print), IN CALL;     printf("\n");

    MFUN(&bt, traverse_post) CALL; printf("\n");
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
    FUN(init_global_memory) sizeof(int) * 1000, LIMITED_SIZE_MEMORY CALL;

    MEM_SIZE_T free_start = 0, free_end = 0;
    FUN(get_total_free_bytes)& free_start CALL;
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


    MFUN(TheGlobalCache, reset) CALL;
    FUN(get_total_free_bytes)& free_end CALL;
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