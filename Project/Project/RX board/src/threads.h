#ifndef _THREADS_
#define _THREADS_

#define osObjectsPublic 
#include "stm32f4xx_conf.h"
#include "osObjects.h"                  // RTOS object definitions
#include <stdio.h>
#include <string.h>
#include "wireless_rx.h"
#include "servermotor.h"
#include "mems.h"

#define RX_READY   0x01 
#define DATA_READY 0x01
#define MEMS_READY 0x01
#define MEMS_MODE   254

// structure for messages to be passed between the threads (either for position data or difference in x/y corredintes from the MEMS).
typedef struct{
	uint8_t x;    // used for difference in x when using MEMS
	uint8_t y;		// used for difference in y when using MEMS
	uint8_t z;		// not set when using MEMS
} Message;

extern osPoolId mem_pool;

extern osMessageQId data_queue, mems_queue;

extern osThreadId rx_tid, servomotor_tid, mems_tid;

void rx_thread(void const * arg);
void servomotor_thread(void const * arg);
void mems_thread(void const * arg);

// other function prototypes
void send_message(uint8_t * data, osMessageQId queue);
int receive_message(uint8_t * data, osMessageQId queue);
void create_threads(void);

#endif
