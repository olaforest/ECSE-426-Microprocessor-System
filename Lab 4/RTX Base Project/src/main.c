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

int mode = 1;
float pitch = 0.0;
float current_temperature = 0.0;

osThreadId mems_thread, temp_sensor_thread, display_thread, keypad_thread;

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
	while(1){
		osSignalWait(KEYPAD_READY, osWaitForever);
		get_target_angle();
	}
}

void segment_display(void const * arg){
	int count = 0;
	
	while(1){
		osSignalWait(DISPLAY_READY, osWaitForever);
		display_value(mode, pitch, 51.5, count++);
		//display_current_pitch(pitch, count++);
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
	
	while(1){
		osSignalWait(MEMS_READY, osWaitForever);
		pitch = kalmanFilter(get_pitch_angle(), &kstate);
		//printf("pitch: %.2f\n", pitch);
	}
}

void temp_sensor(void const * arg){
	
	uint16_t LED_pins[4] = { GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15 };
	int duty_cycle = 0;
	int increasing = 1;
	int ticks_count = 0;
	
	// set the initial Kalman filter state.
	KalmanState kstate;
	kstate.p = 0.1;
	kstate.k = 0.0;
	kstate.r = 2.25;
	kstate.q = 0.01;
	kstate.x = 0.0;
	
	while(1){
		osSignalWait(TEMP_SENSOR_READY, osWaitForever);
		
		// sample the current temperature of the processor.
		current_temperature = kalmanFilter(volt_to_celsius(getADCVoltage()), &kstate);		
		
		// make a decision of whether to run the normal or alarm routines, 
		// based on the current temperature and chosen alarm thershold.
		if (current_temperature < ALARM_THRESHOLD) {
			//normal_operation((int) current_temperature, LED_pins);
		} else {
			//alarm_operation(&ticks_count, &increasing, &duty_cycle);
		}	
		
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
	
	mems_thread = osThreadCreate(osThread(mems), NULL);
	//temp_sensor_thread = osThreadCreate(osThread(temp_sensor), NULL);
	display_thread = osThreadCreate(osThread(segment_display), NULL);
	//keypad_thread = osThreadCreate(osThread(keypad), NULL);
		
	// start thread execution 
	osKernelStart();                         
}


