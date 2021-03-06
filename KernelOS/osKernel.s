	AREA |.text|, CODE, READONLY, ALIGN=2
	THUMB 
	;instructs assembler to interpret subsequent instructions as Thumb instructions
	;external variable, points to the thread control block of the current thread which has exhausted its quanta
	;since context is changing, we need to save the context of this thread. 

	PRESERVE8 ;preserve 8-bit alignment
	EXTERN currentPt
	EXPORT SysTick_Handler
	EXPORT osSchedulerLaunch
	IMPORT osPeriodicScheduler
		
;perform context switch when a systick interrupt occurs 		
SysTick_Handler ;this saves r0-r3,r12,lr,c,psr
	;disable interrupts
	CPSID I
	;manually push remaining registers onto the stack
	PUSH {R4-R11}
	;R0 pointes to currentPt
	LDR R0, =currentPt
	;load what currentPt is pointing to into R1
	LDR R1, [R0]
	;store stack pointer into R1 which equals currentPt
	STR SP,[R1]
	;choose next thread in cirucular linked list
	;navigate to second entry in the thread control block
	;LDR R1, [R1, #4] ;R1 = currentPt->next
	;currentPt = R1
	;STR R1, [R0] 
	PUSH {R0, LR}
	;BL osRoundRobinScheduler
	BL osPeriodicScheduler
	POP	{R0,LR}
	LDR R1, [R0]
	;SP = currentPt->stackPt
	LDR SP,[R1] 
	POP {R4-R11}
	;enable interrupts again
	CPSIE I
	;the 8 registers that were automatically saved will be restored
	BX LR
	
;set the value of the cortex-m stack pointer to the value of the first thread	
osSchedulerLaunch
	LDR R0, =currentPt
	;R2 = currentPt
	LDR R2,[R0] 
	;SP = currentPt->stackPt
	LDR SP,[R2] 
	POP {R4-R11}
	POP {R0-R3}
	POP {R12}
	ADD SP, SP, #4
	POP {LR}
	ADD SP, SP, #4
	CPSIE I
	BX LR

	ALIGN
	END