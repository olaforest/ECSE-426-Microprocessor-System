/*
ECSE 426 - Lab 4
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Group #3
Date:	March 23, 2015
*/ 

#include "interrupt_service_routines.h"

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
