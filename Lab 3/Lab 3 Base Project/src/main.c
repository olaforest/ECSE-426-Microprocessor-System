#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "mems.h"
#include "segment_display.h"
#include "keypad.h"

static volatile uint_fast16_t data_ready, cycle_led;

// Handler routine for the EXTI0 interupts 
void EXTI0_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line0) == SET) {
		data_ready = 1;
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

// Handler routine for the TIM3 interrupts
void TIM3_IRQHandler(){
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET){
		cycle_led = 1;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

int main(){
	// Kalman filter state initilization
	KalmanState kstate;
	kstate.p = 0.0;
	kstate.k = 0.0;
	kstate.r = 50;
	kstate.q = 1;
	kstate.x = 0.0;
	
	config_LIS3DSH();
	config_ext_interupt();
	config_tim3();
	config_segment_display();
	
	/*keypad_init();
	
//	printf("%u\n", GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0));
//	printf("%u\n", GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1));
//	printf("%u\n", GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2));
//	printf("%u\n", GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_3));
//	printf("%u\n", GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4));
//	printf("%u\n", GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5));
//	printf("%u\n", GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6));
//	printf("%u\n", GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7));
//	printf("%u\n", read_cols());
//	printf("Found key: %d\n", get_key());
	get_keys();*/
	
	int target_angle = 80;
	
	int count = 0;
	
	while(1){
		
		while(!data_ready && !cycle_led);
		
		float pitch;
		
		if(data_ready){
			data_ready = 0;
			pitch = kalmanFilter(get_pitch_angle(), &kstate);
			//printf("Pitch angle: %.2f\n", pitch);
		}
		else if(cycle_led){
			cycle_led = 0;
			
			if(pitch > target_angle + 5){
				display_anim_larger(count);
			}
			else if(pitch < target_angle - 5){
				display_anim_smaller(count);
			}
			else {
				display_current_pitch(pitch, count);
			}
			
			count++;
		}
	}
	return 0;
}

