/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

//#define osObjectsPublic                 // define objects in main module
//#include "osObjects.h"                  // RTOS object definitions
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"

#include "wireless_tx.h"

#include <stdio.h>
#include <string.h>


/*
 * main: initialize and start the system
 */

int main (void) {

	wireless_init();
	send_beacons();
//	transmit();
//	receive();	
}


