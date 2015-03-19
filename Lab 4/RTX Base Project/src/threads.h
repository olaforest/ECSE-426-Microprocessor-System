#ifndef _THREADS_
#define _THREADS_

#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "osObjects.h"                  // RTOS object definitions
#include "mems.h"
#include "temperature_sensor.h"
#include "segment_display.h"
#include "keypad.h"
#include "kalman_filter.h"

// signals
#define MEMS_READY		  0x01 
#define TEMP_SENSOR_READY 0x02
#define DISPLAY_READY	  0x04
#define KEYPAD_READY	  0x08

// thread IDs
extern osThreadId mems_thread, temp_sensor_thread, display_thread, keypad_thread;

// structure for messages to be passed between the threads 
typedef struct{
	float data;
} Message;

// Define a memory pool
osPoolDef(mem_pool, 10, Message);			
extern osPoolId mem_pool;

// message queue definitions
osMessageQDef(pitch_queue, 1, Message); 	// Define message queue for pitch angles
osMessageQDef(temp_queue, 1, Message);		// Define message queue for temperatures
osMessageQDef(keypad_queue, 1, Message);	// Define message queue for the keypad inputs
extern osMessageQId temp_queue, pitch_queue, keypad_queue;

// thread handlers
void temp_sensor(void const * arg);
void mems(void const * arg);
void keypad(void const * arg);
void segment_display(void const * arg);

// other function prototypes
void send_message(float data, osMessageQId queue);
int receive_message(float * data, osMessageQId queue);
void create_mem_pool_and_msg_queues(void);
void create_threads(void);

// thread definitions
osThreadDef(temp_sensor, osPriorityHigh, 1, 400);
osThreadDef(mems, osPriorityAboveNormal, 1, 1000);
osThreadDef(keypad, osPriorityNormal, 1, 400);
osThreadDef(segment_display, osPriorityBelowNormal, 1, 400);

#endif
