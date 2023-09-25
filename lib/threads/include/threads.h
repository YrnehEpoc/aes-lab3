#pragma once
#include <stdio.h>
#include <zephyr.h>
#include <arch/cpu.h>
#include <sys/printk.h>

int do_loop(struct k_timer *timer,
            struct k_sem *semaphore,
            int *counter,
            char *src,
            k_timeout_t timeout);