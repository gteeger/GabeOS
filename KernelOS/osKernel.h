#ifndef _OS_KERNEL_H
#define _OS_KERNEL_H

// 1) need to launch the kernel
// 2) need to add threads to the kernel  
// 3) need to initialize the kernel

#include <stdint.h>
#include "stm32f4xx.h"                  // Device header


void osSemaphoreInit(int32_t *semaphore, int32_t value);
void osSignalSet(int32_t *semaphore);
void osSignalWait(int32_t *semaphore);
void osKernelLaunch(uint32_t quanta);
void osKernelInit (void);
uint8_t osKernelAddThreads(void(*task1)(),void(*task2)(),void(*task3)() );
void osThreadYield(void);
void osRoundRobinScheduler();
void osPeriodicScheduler();
void Task3();
void Task4();

#endif