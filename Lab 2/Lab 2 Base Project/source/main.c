/*
ECSE 426 - Lab 2
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Group #3
Date:	February 20th, 2015
*/ 

#include "temperature_sensor.h"

static volatile uint_fast16_t ticks;

// interrupt handler for system tick
void SysTick_Handler(){
	ticks = 1;
}

int main(){
	// declaration/initialization of various variables. 
	KalmanState kstate;
	uint16_t LED_pins[4] = { GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15 };
	int duty_cycle = 0;
	int increasing = 1;
	int ticks_count = 0;
	
	ticks = 0;
	
	// set the initial Kalman filter state.
	kstate.p = 0.1;
	kstate.k = 0.0;
	kstate.r = 2.25;
	kstate.q = 0.01;
	kstate.x = 0.0;
	
	// initialize various components (ADC, GPIO and SysTick).
	init();
			
	// main loop
	while(1){
		
		// wait until SysTick handler changes toggles tick value.
		while(!ticks);
		ticks = 0;
		
		// sample the current temperature of the processor.
		float current_temperature = kalmanFilter(volt_to_celsius(getADCVoltage()), &kstate);
		
		printf("Temperature = %.2f °C", current_temperature);
		
		// make a decision of whether to run the normal or alarm routines, 
		// based on the current temperature and chosen alarm thershold.
		if (current_temperature < ALARM_THRESHOLD) {
			printf(" (NORMAL)\n");
			normal_operation((int) current_temperature, LED_pins);
		} else {
			printf(" (ALARM)\n");
			alarm_operation(&ticks_count, &increasing, &duty_cycle);
		}			
	}
	
	return 0;
}
