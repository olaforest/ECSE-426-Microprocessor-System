#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "mems.h"
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
	kstate.r = 25;
	kstate.q = 1;
	kstate.x = 0.0;
	
	uint16_t digit_pins[10] = {DISPLAY_0, DISPLAY_1, DISPLAY_2, DISPLAY_3, DISPLAY_4, DISPLAY_5, DISPLAY_6, DISPLAY_7, DISPLAY_8, DISPLAY_9};
	uint16_t digit_select[3] = {DIGIT1_ON, DIGIT2_ON, DIGIT3_ON};
	
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
	
	int count = 0;
	
	while(1){
		
		while(!data_ready && !cycle_led);
		
		float pitch;
		
		if(data_ready){
			data_ready = 0;
			pitch = kalmanFilter(get_pitch_angle(), &kstate);
			printf("Pitch angle: %.2f\n", pitch);
			//printf("trunc: %d\n", (int)(pitch*100));
			
		}
		else if(cycle_led){
			cycle_led = 0;
			
			int display_digit = (int)(pitch * 100);
			
			int i = 1;
			
			while(display_digit/i != 0){i *= 10;}
			
			switch(count % 3){
				case 0:
					display_digit = (display_digit * 10)/i;
					//printf("%d", display_digit);
					break;
				case 1:
					display_digit = ((display_digit % (i/10)) * 100)/i;
					//printf("%d", display_digit);
					break;
				case 2:
					display_digit = ((display_digit % (i/100))* 1000)/i;
					//printf("%d\n", display_digit);
					break;
			}
			
			GPIO_ResetBits(GPIOD, GPIO_SEGMENT_PINS);
			GPIO_SetBits(GPIOD, digit_select[count % 3]);
			GPIO_SetBits(GPIOD, digit_pins[display_digit]);
			
			//printf("Count: %d\n", count);
			count++;
		}
	}
	
	return 0;
}
