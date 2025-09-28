#include "UnitTestsInfra.h"
#include "MathUtils.h"
#include <stdio.h>
#include "QueueUnitTest.h"


/* ============================
 *   BASIC QUEUE BEHAVIOR
 * ============================ */
TEST_FUN_IMPL(QueueTest, Queue_fifo_order)
{
    CREATE(Queue_int, q) CALL;

    MFUN(&q, enqueue), 1 CALL;
    MFUN(&q, enqueue), 2 CALL;
    MFUN(&q, enqueue), 3 CALL;

    int val;
    MFUN(&q, dequeue), & val CALL; ASSERT(1 == val);
    MFUN(&q, dequeue), & val CALL; ASSERT(2 == val);
    MFUN(&q, dequeue), & val CALL; ASSERT(3 == val);
}
END_FUN

/* ============================
 *   FRONT / SIZE / EMPTY
 * ============================ */
    TEST_FUN_IMPL(QueueTest, Queue_front_size_empty)
{
    CREATE(Queue_int, q) CALL;

    bool is_empty = false;
    MFUN(&q, empty), & is_empty CALL;
    ASSERT(is_empty == 1);

    MFUN(&q, enqueue), 42 CALL;

    MEM_SIZE_T sz = 0;
    MFUN(&q, size), & sz CALL;
    ASSERT(1 == (int)sz);

    int* p = NULL;
    MFUN(&q, front), & p CALL;
    ASSERT_NOT_NULL(p);
    ASSERT(42 == *p);
}
END_FUN

/* ============================
 *   CLEAR BEHAVIOR
 * ============================ */
    TEST_FUN_IMPL(QueueTest, Queue_clear_resets)
{
    CREATE(Queue_int, q) CALL;

    FOR(int i = 0; i < 5; ++i) {
        MFUN(&q, enqueue), i CALL;
    } END_LOOP;

    MFUN(&q, clear) CALL;

    bool is_empty = true;
    MFUN(&q, empty), & is_empty CALL;
    ASSERT(is_empty == 1);

    MEM_SIZE_T sz = 123;
    MFUN(&q, size), & sz CALL;
    ASSERT(0 == (int)sz);

    /* iteration after clear should do nothing */
    int sum = 0;
    ITER_FOR(int, v, (GenericQueue*)&q) {
        sum += v;
    } END_ITER_FOR
        ASSERT(0 == sum);
}
END_FUN

/* ============================
 *   ITERATOR FOREACH
 * ============================ */
    TEST_FUN_IMPL(QueueTest, Queue_iterator_foreach)
{
    CREATE(Queue_int, q) CALL;

    int data[] = { 10, 20, 30 };
    FOR(int i = 0; i < 3; ++i) {
        MFUN(&q, enqueue), data[i] CALL;
    } END_LOOP;

    int sum = 0;
    ITER_FOR(int, v, (GenericQueue*)&q) {
        sum += v;
    } END_ITER_FOR
        ASSERT(60 == sum);

    /* test continue: skip 20 */
   // int sum2 = 0;
   // ITER_FOR(int, v, (GenericQueue*)&q) {
   //     IF(v == 20)
   //         ITER_CONTINUE;
   //      END_IF
   //         sum2 += v;
   // } END_ITER_FOR
   //     ASSERT(40 == sum2);

   // /* test break: stop at first 30 */
   // int sum3 = 0;
   //ITER_FOR(int, v, (GenericQueue*)&q) {
   //     IF(v == 30)
   //     {
   //         BREAK;
   //     } END_IF
   //         sum3 += v;
   // } END_ITER_FOR
   //    ASSERT(30 == sum3);
}
END_FUN

TEST_FUN_IMPL(QueueTest, Queue_destroy_frees_memory)
{

    MEM_SIZE_T free_bytes_at_start = 0, free_bytes_at_end = 0;
    FUN(get_total_free_bytes)& free_bytes_at_start CALL;

    FOR(int k = 0; k < 10; k++)
    {
        CREATE(Queue_int, q) CALL;

        FOR(int i = 0; i < 5; ++i) {
            MFUN(&q, enqueue), i CALL;
        } END_LOOP;

    } END_LOOP;

    FUN(get_total_free_bytes)& free_bytes_at_end CALL;
    NTEST_ASSERT(free_bytes_at_end == free_bytes_at_start);
}
END_FUN
/* ============================
 *   SUITE
 * ============================ */
    INIT_TEST_SUITE(QueueTest)
    BIND_TEST(QueueTest, Queue_fifo_order);
BIND_TEST(QueueTest, Queue_front_size_empty);
BIND_TEST(QueueTest, Queue_clear_resets);
BIND_TEST(QueueTest, Queue_iterator_foreach);
BIND_TEST(QueueTest, Queue_destroy_frees_memory);
END_INIT_TEST_SUITE(QueueTest)