#include "osKernel.h"
#include "stm32_rtos_bsp.h"
#define QUANTA (1) //1ms

uint32_t count0, count1; 

int32_t semaphore1, semaphore2;

void Task0(void){
	while(1){
		osSignalWait(&semaphore2);
		count0++;
		osSignalSet(&semaphore1);
	}
}
void Task1(void){
	while(1){
		osSignalWait(&semaphore1);
		count1++;
		osSignalSet(&semaphore2);
	}
}

void Task2(void){
	while(1){}
}

void Task3(){
	Probe_CH2(); //pwm on PC2
}
	
void Task4(){
	Probe_CH3(); //pwm on PC3
}

int main(){
	osSemaphoreInit(&semaphore1, 1);
	osSemaphoreInit(&semaphore2, 1);
	HAL_Init();
	Probe_Init();
	TIM3_Init_Start(); //an automatic timer that toggles PC0 when expired
	osKernelInit();
	osKernelAddThreads(&Task0, &Task1, &Task2);
	osKernelLaunch(QUANTA);
	
}
