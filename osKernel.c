// need a scheduler
// need a thread control block- a stack to hold threads

#include "osKernel.h"
#define MAX_NUM_THREADS (3)

//each thread requires a stack for its context
#define STACK_SIZE 100
#define BUS_FREQ 16000000 //16 mHz

//to make systick lowest priority, we need to access the register directly
//this is the same register on all cortex m4 MCUs
#define SYSPRI3 (*((volatile uint32_t *)0xE000ED20)) 
//address of the interrupt control register
#define INTCTRL (*((volatile uint32_t *)0xE000ED04))

uint32_t MILLIS_PRESCALER;
 
void osSchedulerLaunch(); 
	
struct tcb{
	int32_t *stackPt;
	struct tcb *nextPt;
};

typedef struct tcb tcbType;
	
tcbType tcbs[MAX_NUM_THREADS];
tcbType *currentPt;

int32_t TCB_STACK[MAX_NUM_THREADS][STACK_SIZE];

//i is the thread number
void osKernelStackInit(int i){
	//stack pointer of each thread points to the top of the stack of that thread
	tcbs[i].stackPt = &TCB_STACK[i][STACK_SIZE-16];
	
	//initialize the xPSR register to be in thumb mode
	TCB_STACK[i][STACK_SIZE-1] = 0x01000000;
	
}

uint8_t osKernelAddThreads(void(*task0)(),void(*task1)(),void(*task2)() ){
	
	//thread priority 
	//create a circular linked list between tasks
	__disable_irq();
	tcbs[0].nextPt = &tcbs[1];
	tcbs[1].nextPt = &tcbs[2];
	tcbs[2].nextPt = &tcbs[0];
	
	//initialize PC registers
	osKernelStackInit(0);
	TCB_STACK[0][STACK_SIZE-2] = (uint32_t)(task0); //STACK_SIZE-2 points to pc register
	osKernelStackInit(1);
	TCB_STACK[1][STACK_SIZE-2] = (uint32_t)(task1);
	osKernelStackInit(2);
	TCB_STACK[2][STACK_SIZE-2] = (uint32_t)(task2);
	//point currentPT to the thread control block of the first thread
	currentPt = &tcbs[0];
	__enable_irq();
	
	return 1;
}
	
void osKernelInit(){
	
	__disable_irq();
	//set ms prescaler
	MILLIS_PRESCALER = BUS_FREQ/1000; //16ms
	//__enable_irq();
	
}

void osKernelLaunch(uint32_t quanta){
	//initialize systick, set it lowest priority (7)
	SysTick->CTRL = 0;
	SysTick->VAL = 0;
	SysTick->LOAD = quanta*MILLIS_PRESCALER -1;
	SYSPRI3 =(SYSPRI3&0x00FFFFFF)|0xE0000000; 

	//set the clock source to be the bus clock
	SysTick->CTRL = 0x00000007;
	//jump to scheduler, which saves conext and loads a new context
	osSchedulerLaunch();
}

//this function will tell the scheduler to preempt before the time quanta has expired
void osThreadYield(void){
	//trigger systick
	INTCTRL = 0x04000000; 
	
}
