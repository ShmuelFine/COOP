#ifndef __COOP_TESTS_QUEUE__H__
#define __COOP_TESTS_QUEUE__H__

#include "UnitTestsInfra.h"
#include "COOP.h"
#include "Queue.h"

DEF_TEST_SUITE(QueueTest);
ADD_TEST(QueueTest, Queue_fifo_order);
ADD_TEST(QueueTest, Queue_front_size_empty);
ADD_TEST(QueueTest, Queue_clear_resets);
ADD_TEST(QueueTest, Queue_iterator_foreach);
ADD_TEST(QueueTest, Queue_destroy_frees_memory);
END_TEST_SUITE(QueueTest);

#endif 