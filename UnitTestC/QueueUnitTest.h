#ifndef __COOP_TESTS_QUEUE__H__
#define __COOP_TESTS_QUEUE__H__

#include "UnitTestsInfra.h"
#include "Queue.h"
#include <stdio.h>

DEF_TEST_SUITE(QueueTest);
ADD_TEST(QueueTest, Queue_fifo_order);
ADD_TEST(QueueTest, empty_transitions_on_new_enqueue_clear);
ADD_TEST(QueueTest, size_reflects_enqueue_dequeue);
ADD_TEST(QueueTest, front_reflects_fifo_order);
ADD_TEST(QueueTest, Queue_clear_resets);
ADD_TEST(QueueTest, Queue_iterator_foreach);
ADD_TEST(QueueTest, Queue_destroy_frees_memory);
END_TEST_SUITE(QueueTest);

#endif 