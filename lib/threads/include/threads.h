#pragma once
#include <stdio.h>
#include <zephyr.h>
#include <kernel.h>
#include <arch/cpu.h>
#include <sys/printk.h>

int do_loop(struct k_timer *timer,
            struct k_sem *semaphore,
            int *counter,
            char *src,
            k_timeout_t timeout);

int deadlock_thread(struct k_sem *semA, 
                    struct k_sem *semB);


int orphaned_lock(struct k_sem *semaphore, int *counter);
int un_orphaned_lock(struct k_sem *semaphore, int *counter);