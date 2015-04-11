/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                 // define objects in main module
#include <stdio.h>
#include <string.h>
#include "osObjects.h"                  // RTOS object definitions
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "threads.h"

osThreadId rx_tid, servomotor_tid, mems_tid;
osMessageQId data_queue, mems_queue;
osPoolId mem_pool;

/*
 * main: initialize and start the system
 */
int main (void) {
	// initialize CMSIS-RTOS
	osKernelInitialize(); 
	
	create_threads();

	// start thread execution 
	osKernelStart();
}


