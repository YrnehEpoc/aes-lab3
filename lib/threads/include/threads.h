#pragma once
#include <stdio.h>
#include <zephyr.h>
#include <arch/cpu.h>
#include <sys/printk.h>

int threadCounterTask (struct k_sem sem, struct k_timer time, int counter,int sleep );

