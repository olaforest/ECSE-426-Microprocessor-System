/*
ECSE 426 - Lab 3
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Group #3
Date:	March 16, 2015
*/ 

#include "segment_display.h"
#include "temperature_sensor.h"

// Configuration of TIM3
void config_tim3(void){
	
	// Enable TIM3 clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM3_initStruct;
	NVIC_InitTypeDef NVIC_initStruct;
	
	// Configure TIM3 timer
	TIM3_initStruct.TIM_Prescaler			= TIM3_PRESCALAR; 
	TIM3_initStruct.TIM_CounterMode			= TIM_CounterMode_Up;
	TIM3_initStruct.TIM_Period				= TIM3_PERIOD; 		
	TIM3_initStruct.TIM_ClockDivision		= TIM_CKD_DIV1;	
	TIM_TimeBaseInit(TIM3, &TIM3_initStruct);
	
	// Enable and set TIM3 Interrupt to the highest priority
	NVIC_initStruct.NVIC_IRQChannel						= TIM3_IRQn;
	NVIC_initStruct.NVIC_IRQChannelPreemptionPriority	= 0x02;
	NVIC_initStruct.NVIC_IRQChannelSubPriority			= 0x00;
	NVIC_initStruct.NVIC_IRQChannelCmd					= ENABLE;
	NVIC_Init(&NVIC_initStruct);
	
	TIM_UpdateRequestConfig(TIM3, TIM_UpdateSource_Regular);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

// configure the 7-segment display
void config_segment_display(void){
	
	// Enable GPIOD clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// Configure GPIOD pins that operate the segments on the display.
	GPIO_InitStructure.GPIO_Pin		= GPIO_SEGMENT_PINS;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_DOWN;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	// Configure GPIOE pins that selects the digit of the display.
	GPIO_InitStructure.GPIO_Pin		= GPIO_DIGIT_SELECT_PINS;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void display_init(void){
	config_tim3();
	config_segment_display();
}

// Display the current pitch angle (i.e. when the current pitch angle is within 5 degrees from the target angle).
void display_value(float value, int count, int mode){
	
	// Look up table which maps the appropriate GPIO pins to the desired number to be displayed.
	uint16_t digit_pins[10] = {DISPLAY_0, DISPLAY_1, DISPLAY_2, DISPLAY_3, DISPLAY_4, DISPLAY_5, DISPLAY_6, DISPLAY_7, DISPLAY_8, DISPLAY_9};
	
	// Look up table which maps the appropriate GPIO pins to the 
	// desired digit on the segment display to be lit.
	uint16_t digit_select[4] = {DIGIT1_ON, DIGIT2_ON, DIGIT3_ON, DIGIT4_ON};
			
	int display_digit, decimal_point_location;
	
	// Correctly formats the value angle in order to retrieve the the first 3 digits for display
	// and selects the decimal point location depending on magnitude of the value angles.
	if((int)(value)/100 != 0){
		display_digit = (int) value;
		decimal_point_location = 2;
	}
	else if((int)(value)/10 != 0){
		display_digit = (int) (value * 10);
		decimal_point_location = 1;
	}
	else{
		display_digit = (int) (value * 100);
		decimal_point_location = 0;
	}
	
	// Sets the appropriate pins and selects the digit to be displayed based on the current 
	// count of the timer.
	switch(count % 4){
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
	
	GPIO_SetBits(GPIOE, digit_select[count % 4]);
	
	if (count % 4 == 3) {
		
		if(mode == 0) 
			GPIO_SetBits(GPIOD, DISPLAY_C);
		else
			GPIO_SetBits(GPIOD, DISPLAY_DEGREE_SIGN);
	}
	else {
		GPIO_SetBits(GPIOD, digit_pins[display_digit]);
	}
}

// Varies the brightness of a selected LED based on the detected pitch angle (0 - off, 180 - brightest)
void display_LED(float pitch, int count, uint32_t LED_Pin){	
	int duty_cycle = (int)((pitch / 180) * PULSE_WIDTH_PERIOD);
	
	// check if we're currently in the duty cycle (or remaning) time and turn LEDs on (off).
	if ((count % PULSE_WIDTH_PERIOD) < duty_cycle)
		GPIO_SetBits(GPIOD, LED_Pin);
	else 
		GPIO_ResetBits(GPIOD, ALL_LED_PINS);
}

