/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                 // define objects in main module
#include "osObjects.h"                  // RTOS object definitions
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include <stdio.h>
#include "threads.h"
#include "interrupt_service_routines.h"

osThreadId mems_thread, temp_sensor_thread, display_thread, keypad_thread;
osPoolId mem_pool;
osMessageQId temp_queue, pitch_queue, keypad_queue;

/*
 * main: initialize and start the system
 */
int main (void) {
	// initialize CMSIS-RTOS
	osKernelInitialize();                    
		
	create_mem_pool_and_msg_queues();
	create_threads();
	
	// start thread execution 
	osKernelStart();                         
}
