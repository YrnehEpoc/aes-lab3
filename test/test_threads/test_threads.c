
#include <stdio.h>
#include <zephyr.h>
#include <arch/cpu.h>
#include <sys/printk.h>
#include <unity.h>
#include "threads.h"


void setUp(void) {}

void tearDown(void) {}

void test_lock(void)
{

    // Take semaphore without giving, then
    // try and take semaphore
    struct k_sem sem;
    int counter = 0;
	struct k_timer timer;
	k_timer_init(&timer, NULL, NULL);
    k_sem_init(&sem, 1, 1);
    k_sem_take(&sem, K_FOREVER);

    int result = do_loop(&timer, &sem, &counter, "main", K_MSEC(100));

    TEST_ASSERT_EQUAL(1,result);
    TEST_ASSERT_EQUAL(0, counter);
}
void test_loop_runs(void)
{

    // Take semaphore for 500 Msec, try to take during lock and after given

    struct k_sem sem;
    int counter = 0;
	struct k_timer timer;
	k_timer_init(&timer, NULL, NULL);
    k_sem_init(&sem, 1, 1);

    int result = do_loop(&timer, &sem, &counter, "main", K_MSEC(100));

    TEST_ASSERT_EQUAL(0,result);
    TEST_ASSERT_EQUAL(1, counter);
}



#define STACK_SIZE 500
K_THREAD_STACK_DEFINE(thread2_stack, STACK_SIZE); //worker 1
K_THREAD_STACK_DEFINE(thread3_stack, STACK_SIZE); //worker 2

struct k_thread thread2, thread3;

void test_deadlock(void)
{
    struct k_sem semA;
    struct k_sem semB;
    k_sem_init(&semA, 1, 1);
    k_sem_init(&semB, 1, 1);

    //creates first worker thread
    k_tid_t k_id1 = k_thread_create(&thread2, thread2_stack, STACK_SIZE, 
                                    (k_thread_entry_t) deadlock_thread, &semA, &semB, 
                                    NULL, K_PRIO_COOP(7), 0, K_NO_WAIT);

    //creates second worker thread
    k_tid_t k_id2 = k_thread_create(&thread3, thread3_stack, STACK_SIZE, 
                                    (k_thread_entry_t) deadlock_thread, &semB, &semA, 
                                    NULL, K_PRIO_COOP(7), 0, K_NO_WAIT);

    int thread_status1 = k_thread_join(k_id1, K_MSEC(800));
    int thread_status2 = k_thread_join(k_id2, K_MSEC(800));
    
    TEST_ASSERT_EQUAL(-EAGAIN, thread_status1);
    TEST_ASSERT_EQUAL(-EAGAIN, thread_status2);

    k_thread_abort(k_id1);
    k_thread_abort(k_id2);
}

void test_orphaned_lock(void)
{
    struct k_sem sem;
    k_sem_init(&sem, 1, 1);
    int count = 1; 

    int orphan_status = orphaned_lock(&sem, &count);
    TEST_ASSERT_EQUAL(2, count);
    TEST_ASSERT_EQUAL(2, orphan_status);

    orphan_status = orphaned_lock(&sem, &count);
    TEST_ASSERT_EQUAL(3, count);
    TEST_ASSERT_EQUAL(0, orphan_status);

    orphan_status = orphaned_lock(&sem, &count);
    TEST_ASSERT_EQUAL(3, count);
    TEST_ASSERT_EQUAL(1, orphan_status);
}

void test_un_orphaned_lock(void)
{
    struct k_sem sem;
    k_sem_init(&sem, 1, 1);
    int count = 1; 

    int un_orphan_status = un_orphaned_lock(&sem, &count);
    TEST_ASSERT_EQUAL(2, count);
    TEST_ASSERT_EQUAL(2, un_orphan_status);

    un_orphan_status = un_orphaned_lock(&sem, &count);
    TEST_ASSERT_EQUAL(3, count);
    TEST_ASSERT_EQUAL(0, un_orphan_status);

    un_orphan_status = un_orphaned_lock(&sem, &count);
    TEST_ASSERT_EQUAL(4, count);
    TEST_ASSERT_EQUAL(2, un_orphan_status);

    k_sem_take(&sem, K_FOREVER);
    un_orphan_status = un_orphaned_lock(&sem, &count);
    TEST_ASSERT_EQUAL(4, count);
    TEST_ASSERT_EQUAL(1, un_orphan_status);

}

int main (void)
{
    UNITY_BEGIN();
    RUN_TEST(test_lock);
    RUN_TEST(test_loop_runs);
    RUN_TEST(test_deadlock);
    RUN_TEST(test_orphaned_lock);
    RUN_TEST(test_un_orphaned_lock);
    return UNITY_END();
}