#include "threads.h"


int threadCounterTask (struct k_sem sem, struct k_timer time, int *counter,int sleep ){
    k_sem_take(&sem, K_FOREVER);
    *counter = *counter + 1;
    printk("hello world from %s! Count %d\n", "thread", *counter);
    k_timer_start(&time, K_MSEC(sleep), K_NO_WAIT);
    k_timer_status_sync(&time);
    k_sem_give(&sem);
    return 0;
}