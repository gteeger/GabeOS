# GabeOS
A real time operating system developed on an ARM STM32F4 Microcontroller, but can work on any Cortex M4 Microcontroller. 

•	Includes Round-Robin and Periodic schedulers, and thread control block.  

•	The time quanta of each process is linked to the Systick Timer. Processes are pre-empted after each Systick interrupt.

•	At predefined intervals, the schduler can schedule certain tasks like toggleing GPIO pins to create a custom PWMs. GPIO pins are accessed through the board's hardware abstraction layer (HAL).

•	Semaphores ensure thread synchronization.

•	Developed in Keil uVision, written in C and assembly.
