#ifndef _OS_KERNEL_H
#define _OS_KERNEL_H

// 1) need to launch the kernel
// 2) need to add threads to the kernel  
// 3) need to initialize the kernel

#include <stdint.h>
#include "stm32f4xx.h"                  // Device header

void osKernelLaunch(uint32_t quanta);
void osKernelInit (void);
uint8_t osKernelAddThreads(void(*task1)(),void(*task2)(),void(*task3)() );
void osThreadYield(void);



#endif