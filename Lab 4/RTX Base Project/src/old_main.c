///*
//ECSE 426 - Lab 3
//Maxim Goukhshtein (ID: 260429739)
//Olivier Laforest  (ID: 260469066)
//Group #3
//Date:	March 16, 2015
//*/ 

//#include <stdio.h>
//#include "stm32f4xx.h"                  // Device header
//#include "stm32f4xx_conf.h"
//#include "mems.h"
//#include "segment_display.h"
//#include "keypad.h"

//static volatile uint_fast16_t data_ready, cycle_led;

//// Handler routine for the EXTI0 interrupts 
//void EXTI0_IRQHandler(){
//	if(EXTI_GetITStatus(EXTI_Line0) == SET) {
//		data_ready = 1;
//		EXTI_ClearITPendingBit(EXTI_Line0);
//	}
//}

//// Handler routine for the TIM3 interrupts
//void TIM3_IRQHandler(){
//	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET){
//		cycle_led = 1;
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
//	}
//}

//int main(){
//	// Kalman filter state initilization
//	KalmanState kstate;
//	kstate.p = 0.0;
//	kstate.k = 0.0;
//	kstate.r = 50;
//	kstate.q = 0.5;
//	kstate.x = 0.0;
//	
//	int count = 0;
//	
//	// initialization routines
//	config_LIS3DSH();
//	config_ext_interupt();
//	config_tim3();
//	config_segment_display();
//	keypad_init();
//	
//	// get the target angle input from the keypad
//	int target_angle = get_target_angle(&cycle_led);
//	printf("You chose a target angle of %d°\n", target_angle);
//			
//	// main loop
//	while(1){
//		
//		while(!data_ready && !cycle_led);
//		
//		float pitch;
//		
//		// If data is ready, get the angle and filter it. If the display is ready to change, make appropriate adjustments.
//		if(data_ready){
//			data_ready = 0;
//			pitch = kalmanFilter(get_pitch_angle(), &kstate);
//		} else if(cycle_led) {
//			cycle_led = 0;
//			display_routine(pitch, target_angle, count);
//			count++;
//		}
//	}
//	return 0;
//}

