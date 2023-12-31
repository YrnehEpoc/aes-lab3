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

int deadlock_thread(struct k_sem *semA, struct k_sem *semB){
// There are two locks
    struct k_timer timer;
	k_timer_init(&timer, NULL, NULL);
    k_sem_take(semA, K_FOREVER);
    
    k_yield();
    k_timer_start(&timer, K_MSEC(100), K_NO_WAIT);
    k_timer_status_sync(&timer);
    k_sem_take(semB, K_FOREVER);
    k_sem_give(semB);

    k_sem_give(semA);
    return 0;

}
//One thread has lock A, and is waiting for a lock B. 
//The other thread holds lock B and is waiting for resource A.





int orphaned_lock(struct k_sem *semaphore, int *counter)
{
    if(k_sem_take(semaphore, K_MSEC(600)))
    {
        return 1; //return 1 when semaphore is already taken
    }
    
    *counter = *counter +1;

    if(*counter % 2)
    {
        return 0; //return 0 when remainder =/= 0 and cause lock to orphan
    }

    printk("Count %d\n", *counter);
    k_sem_give(semaphore);

    return 2;  //completed with proper sem take and give
    
}


int un_orphaned_lock(struct k_sem *semaphore, int *counter)
{
    if(k_sem_take(semaphore, K_MSEC(600)))
    {
        return 1; //return 1 when semaphore is already taken
    }
    else
    {
        *counter = *counter +1;
        if(*counter % 2)
        {
            printk("Count %d\n", *counter);
            k_sem_give(semaphore);
            return 0; //return 0 with proper sem take and give when checking condition
        }
    }

    printk("Count %d\n", *counter);
    k_sem_give(semaphore);
    
    return 2; //completed with proper sem take and give
}








