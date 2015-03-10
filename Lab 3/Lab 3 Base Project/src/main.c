#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "mems.h"

static volatile uint_fast16_t data_ready;

// Handler routine for the EXTI0 interupts 
void EXTI0_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line0) == SET) {
		data_ready = 1;
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

int main(){
	// Kalman filter state initilization
	KalmanState kstate;
	kstate.p = 0.0;
	kstate.k = 0.0;
	kstate.r = 25;
	kstate.q = 1;
	kstate.x = 0.0;
	
	config_LIS3DSH();
	config_ext_interupt();
	
	while(1){		
		while(!data_ready);
		data_ready = 0;
		float pitch = kalmanFilter(get_pitch_angle(), &kstate);
		printf("Pitch angle: %.2f\n", pitch);
	}
	
	return 0;
}


