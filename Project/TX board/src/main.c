/*
ECSE 426 - Wireless 3D printing machine project
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Nuri Ege Kozan    (ID: 260359680)
Genevieve Nantel  (ID: 260481768)
Group #4
Date:	April 14th, 2015
*/

#define osObjectsPublic                 // define objects in main module
#include "osObjects.h"                  // RTOS object definitions
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "threads.h"

osThreadId keypad_tid, LCD_control_tid, Data_send_tid;

/*
 * main: initialize and start the system
 */
int main (void) {

	osKernelInitialize ();				// initialize CMSIS-RTOS
	
	// initialize peripherals here
	/* LCD initiatization */
	LCD_Init();
	
	/* LCD Layer initiatization */
	LCD_LayerInit();
	
	/* Enable the LTDC controler */
	LTDC_Cmd(ENABLE);
	
	/* Set LCD foreground layer as the current layer */
	LCD_SetLayer(LCD_FOREGROUND_LAYER);
	
	create_threads();
	
	osKernelStart ();					// start thread execution 
}
