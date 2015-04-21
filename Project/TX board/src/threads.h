/*
ECSE 426 - Wireless 3D printing machine project
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Nuri Ege Kozan    (ID: 260359680)
Genevieve Nantel  (ID: 260481768)
Group #4
Date:	April 14th, 2015
*/

#ifndef _THREADS_
#define _THREADS_

#define osObjectsPublic 
#include "stm32f4xx_conf.h"
#include "osObjects.h"

#include "wireless_tx.h"
#include "keypad.h"
#include "LCD_draw.h"

#define DATA_READY		0x01
#define LCD_READY		0x01
#define NON_MEMS_MODE   255
#define MEMS_MODE		254

extern osThreadId keypad_tid, LCD_control_tid, Data_send_tid;

/* Exported functions --------------------------------------------------------*/
void keypad_thread(void const * arg);
void LCD_control_thread(void const * arg);
void Data_send_thread(void const * arg);
void create_threads(void);


#endif
