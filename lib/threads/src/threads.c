#include "threads.h"

#define SLEEPTIME 1000

int do_loop(struct k_timer *timer,struct k_sem *semaphore, int *counter, 
            char *src,
            k_timeout_t timeout){
                if(k_sem_take(semaphore, timeout) != 0){
                    return 1;   //fail if semaphore is unable to be taken
                }
                k_sem_take(semaphore, timeout);
                *counter = *counter + 1;
                printk("hello world from %s! Count %d\n", src, *counter);
                k_timer_start(timer, K_MSEC(SLEEPTIME), K_NO_WAIT);
                k_timer_status_sync(timer);
                k_sem_give(semaphore);
                return 0;
            }
