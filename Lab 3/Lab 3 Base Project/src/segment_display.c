#include "segment_display.h"
#include "stm32f4xx_conf.h"

// Configuration of TIM3
void config_tim3(void){
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM3_initStruct;
	NVIC_InitTypeDef NVIC_initStruct;
	
	TIM3_initStruct.TIM_Prescaler					= 0xFFFF;
	TIM3_initStruct.TIM_CounterMode				= TIM_CounterMode_Up;
	TIM3_initStruct.TIM_Period						= 0x0008;
	TIM3_initStruct.TIM_ClockDivision			= TIM_CKD_DIV1;
	
	TIM_TimeBaseInit( TIM3, &TIM3_initStruct);
	
	// Enable and set TIM3 Interrupt to the lowest priority
	NVIC_initStruct.NVIC_IRQChannel										= TIM3_IRQn;
	NVIC_initStruct.NVIC_IRQChannelPreemptionPriority	= 0x00;
	NVIC_initStruct.NVIC_IRQChannelSubPriority				= 0x00;
	NVIC_initStruct.NVIC_IRQChannelCmd								= ENABLE;
	NVIC_Init(&NVIC_initStruct);
	
	TIM_UpdateRequestConfig(TIM3, TIM_UpdateSource_Regular);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

void config_segment_display(void){
	
	// Enable GPIOD clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_SEGMENT_PINS;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
}

void display_current_pitch(float pitch, int count){
	
	uint16_t digit_pins[10] = {DISPLAY_0, DISPLAY_1, DISPLAY_2, DISPLAY_3, DISPLAY_4, DISPLAY_5, DISPLAY_6, DISPLAY_7, DISPLAY_8, DISPLAY_9};
	uint16_t digit_select[3] = {DIGIT1_ON, DIGIT2_ON, DIGIT3_ON};
	
	GPIO_ResetBits(GPIOD, GPIO_SEGMENT_PINS);
		
		int display_digit, decimal_point_location;
		
		if((int)(pitch)/100 != 0){
			display_digit = (int) pitch;
			decimal_point_location = 2;
		}
		else if((int)(pitch)/10 != 0){
			display_digit = (int) (pitch * 10);
			decimal_point_location = 1;
		}
		else{
			display_digit = (int) (pitch * 100);
			decimal_point_location = 0;
		}
		
		switch(count % 3){
			case 0:
				display_digit = display_digit/100;
			
				if(decimal_point_location == 0)
					GPIO_SetBits(GPIOD, SEGMENT_PIN_DP);
				
				break;
			case 1:
				display_digit = (display_digit % 100)/10;
			
				if(decimal_point_location == 1)
					GPIO_SetBits(GPIOD, SEGMENT_PIN_DP);
				
				break;
			case 2:
				display_digit = (display_digit % 10)/1;
				break;
		}
		
		GPIO_SetBits(GPIOD, digit_select[count % 3]);
		GPIO_SetBits(GPIOD, digit_pins[display_digit] | SEGMENT_PIN_DEGREE_SIGN);
}

void display_anim_larger(int count){
	
	uint16_t digit_pins[2] = {DISPLAY_d, DISPLAY_n};
	uint16_t digit_select[2] = {DIGIT1_ON, DIGIT2_ON};
	
	GPIO_ResetBits(GPIOD, GPIO_SEGMENT_PINS);
	GPIO_SetBits(GPIOD, digit_select[count % 2]);
	GPIO_SetBits(GPIOD, digit_pins[count % 2]);
}

void display_anim_smaller(int count){
	
	uint16_t digit_pins[2] = {DISPLAY_U, DISPLAY_P};
	uint16_t digit_select[2] = {DIGIT1_ON, DIGIT2_ON};
	
	GPIO_ResetBits(GPIOD, GPIO_SEGMENT_PINS);
	GPIO_SetBits(GPIOD, digit_select[count % 2]);
	GPIO_SetBits(GPIOD, digit_pins[count % 2]);
}