/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                 // define objects in main module
#include "osObjects.h"                  // RTOS object definitions
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include <stdio.h>
#include "mems.h"
#include "temperature_sensor.h"
#include "segment_display.h"
#include "keypad.h"
#include "kalman_filter.h"

#define MEMS_READY		  0x01 
#define TEMP_SENSOR_READY 0x02
#define DISPLAY_READY	  0x04
#define KEYPAD_READY	  0x08

float temperature = 50.0;
float t;
float t2;

osThreadId mems_thread, temp_sensor_thread, display_thread, keypad_thread;

typedef struct{
	float data;
} Message;

osPoolDef(mem_pool, 16, Message);			// Define a memory pool
osPoolId mem_pool;
osMessageQDef(pitch_queue, 1, Message); 	// Define message queue for pitch angles
osMessageQId pitch_queue;
osMessageQDef(temp_queue, 1, Message);		// Define message queue for temperatures
osMessageQId temp_queue;
osMessageQDef(keypad_queue, 1, Message);	// Define message queue for the keypad inputs
osMessageQId keypad_queue;

// Handler routine for the EXTI0 interrupts 
void EXTI0_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line0) == SET) {
		EXTI_ClearITPendingBit(EXTI_Line0);
		osSignalSet(mems_thread, MEMS_READY);
	}
}

// Handler routine for the TIM2 interrupts
void TIM2_IRQHandler(){
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		osSignalSet(temp_sensor_thread, TEMP_SENSOR_READY);
	}
}

// Handler routine for the TIM3 interrupts
void TIM3_IRQHandler(){
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET){
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		osSignalSet(keypad_thread, KEYPAD_READY);
		osSignalSet(display_thread, DISPLAY_READY);
	}
}

void keypad(void const * arg){
	int count = 0;
	char key;
	int mode = TEMP_MODE;
	
	while(1){
		osSignalWait(KEYPAD_READY, osWaitForever);
		if ((count++ % KEY_SCAN_CLK_DIV) == 0){
			key = get_input();
			if (key != DUMMY_KEY){			
				Message* msg = osPoolAlloc(mem_pool);
				msg->data = (float)(key - '0');
				osMessagePut(keypad_queue, (uint32_t)msg, osWaitForever);
			}
		}
	}
}

void segment_display(void const * arg){
	uint16_t LED_pins[4] = { GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15 };
	int count = 0;
	char key = DUMMY_KEY, led = '1';
	int mode = TEMP_MODE;
	//float temperature = 50.0;
	float pitch = 0.0;
		
	while(1){
		osSignalWait(DISPLAY_READY, osWaitForever);
					
		osEvent keypad_evt = osMessageGet(keypad_queue, 0); // wait for message
		if (keypad_evt.status == osEventMessage) {
			Message* msg = ((Message *)keypad_evt.value.p);
			key = (char)(((int)msg->data) + '0');			
			osPoolFree(mem_pool, msg); // free memory allocated for message
			
			if (key == TEMP_MODE_KEY || key == MEMS_MODE_KEY){
				mode = key == TEMP_MODE_KEY ? TEMP_MODE : MEMS_MODE;
			} else {
				led = key;
			}
		}			
		
		osEvent pitch_evt = osMessageGet(pitch_queue, 0); // wait for message
		if (pitch_evt.status == osEventMessage) {
			Message* msg = ((Message *)pitch_evt.value.p);
			pitch = msg->data;			
			osPoolFree(mem_pool, msg); // free memory allocated for message
		}
		
//		osEvent temp_evt = osMessageGet(temp_queue, 0); // wait for message
//		if (temp_evt.status == osEventMessage) {
//			Message* msg = ((Message *)temp_evt.value.p);
//			temperature = msg->data;			
//			osPoolFree(mem_pool, msg); // free memory allocated for message
//		}
		
		GPIO_ResetBits(GPIOD, GPIO_SEGMENT_PINS);
		GPIO_ResetBits(GPIOB, GPIO_DIGIT_SELECT_PINS);
		
		if (temperature < ALARM_THRESHOLD || (count % (2 * TIM3_DESIRED_RATE)) < TIM3_DESIRED_RATE) {
			if (mode == TEMP_MODE){
				display_value(temperature, count, TEMP_MODE); 
			} else {
				display_value(pitch, count, MEMS_MODE); 
				//display_LED(pitch, count, LED_pins[(led - '1')]);
			}
		}
		
		if (mode == MEMS_MODE){
			//display_value(pitch, count, MEMS_MODE); 
			display_LED(pitch, count, LED_pins[(led - '1')]);
		}
		count++;
	}
}

void mems(void const * arg){
	// Kalman filter state initilization
	KalmanState kstate;
	kstate.p = 0.0;
	kstate.k = 0.0;
	kstate.r = 50;
	kstate.q = 0.5;
	kstate.x = 0.0;
	
	float pitch = 0.0;
	
	while(1){
		osSignalWait(MEMS_READY, osWaitForever);
		pitch = kalmanFilter(get_pitch_angle(), &kstate);
		
		Message* msg = osPoolAlloc(mem_pool);
		msg->data = pitch;
		osMessagePut(pitch_queue, (uint32_t)msg, osWaitForever);
	}
}

void temp_sensor(void const * arg){

	// set the initial Kalman filter state.
	KalmanState kstate;
	kstate.p = 0.1;
	kstate.k = 0.0;
	kstate.r = 2.25;
	kstate.q = 0.01;
	kstate.x = 0.0;
	
	float current_temperature = 0.0;
	
	while(1){
		osSignalWait(TEMP_SENSOR_READY, osWaitForever);
		
		t2 = volt_to_celsius(getADCVoltage());
		t =  kalmanFilter(t2, &kstate);
		
		// sample the current temperature of the processor.
		current_temperature = kalmanFilter(volt_to_celsius(getADCVoltage()), &kstate);
		temperature = kalmanFilter(volt_to_celsius(getADCVoltage()), &kstate);		
		
//		Message* msg = osPoolAlloc(mem_pool);
//		//msg->data = current_temperature;
//		msg->data = t;
//		osMessagePut(temp_queue, (uint32_t)msg, 0);		
	}
}

osThreadDef(temp_sensor, osPriorityHigh, 1, 400);
osThreadDef(mems, osPriorityAboveNormal, 1, 1000);
osThreadDef(keypad, osPriorityNormal, 1, 400);
osThreadDef(segment_display, osPriorityBelowNormal, 1, 400);

/*
 * main: initialize and start the system
 */
int main (void) {
	// initialize CMSIS-RTOS
	osKernelInitialize();                    
	
	mems_init();
	temp_sensor_init();
	display_init();
	keypad_init();
	
	mem_pool = osPoolCreate(osPool(mem_pool)); 					  
	pitch_queue = osMessageCreate(osMessageQ(pitch_queue), NULL); 
	temp_queue = osMessageCreate(osMessageQ(temp_queue), NULL);
	keypad_queue = osMessageCreate(osMessageQ(keypad_queue), NULL);
	
	mems_thread = osThreadCreate(osThread(mems), NULL);
	temp_sensor_thread = osThreadCreate(osThread(temp_sensor), NULL);
	display_thread = osThreadCreate(osThread(segment_display), NULL);
	keypad_thread = osThreadCreate(osThread(keypad), NULL);
		
	// start thread execution 
	osKernelStart();                         
}
