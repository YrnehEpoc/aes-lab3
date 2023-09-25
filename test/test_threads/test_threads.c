
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


int main (void)
{
    UNITY_BEGIN();
    RUN_TEST(test_lock);
    return UNITY_END();
}