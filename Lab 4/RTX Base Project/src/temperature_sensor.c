/*
ECSE 426 - Lab 2
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Group #3
Date: February 20th, 2015
*/

#include <math.h>
#include <stdlib.h>
#include "temperature_sensor.h"

// Configuration of TIM2
void config_TIM2(void){
	
	// Enable TIM2 clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM2_initStruct;
	NVIC_InitTypeDef NVIC_initStruct;
	
	// Configure TIM2 timer
	TIM2_initStruct.TIM_Prescaler			= TIM2_PRESCALAR; 
	TIM2_initStruct.TIM_CounterMode			= TIM_CounterMode_Up;
	TIM2_initStruct.TIM_Period				= TIM2_PERIOD; 
	TIM2_initStruct.TIM_ClockDivision		= TIM_CKD_DIV1;	
	TIM_TimeBaseInit(TIM2, &TIM2_initStruct);
	
	// Enable and set TIM2 Interrupt to the highest priority
	NVIC_initStruct.NVIC_IRQChannel						= TIM2_IRQn;
	NVIC_initStruct.NVIC_IRQChannelPreemptionPriority	= 0x00;
	NVIC_initStruct.NVIC_IRQChannelSubPriority			= 0x00;
	NVIC_initStruct.NVIC_IRQChannelCmd					= ENABLE;
	NVIC_Init(&NVIC_initStruct);
	
	TIM_UpdateRequestConfig(TIM2, TIM_UpdateSource_Regular);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

// initialization and setup of the GPIO used for the LEDs.
void LED_GPIO_init(){
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); // Enable clock for GPIOD
	
	GPIO_InitTypeDef gpio_initStruct;
	
	GPIO_StructInit(&gpio_initStruct);
	gpio_initStruct.GPIO_Pin 	= ALL_LED_PINS; 	// Map to all 4 LED pins
	gpio_initStruct.GPIO_Mode	= GPIO_Mode_OUT;	// Set as output
	gpio_initStruct.GPIO_Speed 	= GPIO_Speed_50MHz; 
	gpio_initStruct.GPIO_OType 	= GPIO_OType_PP; 	// Push-pull
	gpio_initStruct.GPIO_PuPd 	= GPIO_PuPd_NOPULL; // Not input, don't pull
	
	GPIO_Init(GPIOD, &gpio_initStruct);
}


// initialization and setup of the ADC used for the temperature sensor samples.
void ADC_init(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //Clock Gating
	
	ADC_CommonInitTypeDef adc_common_init_s;
	
	adc_common_init_s.ADC_Mode 				= ADC_Mode_Independent;			// independant mode (i.e. not multi-mode)
	adc_common_init_s.ADC_Prescaler 		= ADC_Prescaler_Div2;		
	adc_common_init_s.ADC_DMAAccessMode 	= ADC_DMAAccessMode_Disabled;	// disable DMA (not using multi-mode)
	adc_common_init_s.ADC_TwoSamplingDelay 	= ADC_TwoSamplingDelay_5Cycles; // delay between 2 sampling phases
	
	ADC_CommonInit(&adc_common_init_s); 	//Initialization
	
	ADC_InitTypeDef adc_init_s;
	adc_init_s.ADC_Resolution 				= ADC_Resolution_12b;				// 12-bit ADC resolution
	adc_init_s.ADC_ScanConvMode 			= DISABLE;							// using single mode
	adc_init_s.ADC_ContinuousConvMode 		= DISABLE;							// using single mode
	adc_init_s.ADC_ExternalTrigConvEdge 	= ADC_ExternalTrigConvEdge_None; 	// no external trigger edge
	adc_init_s.ADC_DataAlign 				= ADC_DataAlign_Right;				
	adc_init_s.ADC_NbrOfConversion 			= 1;
	
	ADC_Init(ADC1, &adc_init_s);
	
	ADC_Cmd(ADC1, ENABLE);				// enables the module
	ADC_TempSensorVrefintCmd(ENABLE);	// enable temperature sensor
	ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_480Cycles); // set sensor channel and ADC	
}

//  returns the voltage value correponding to the processor temperature based on the latest ADC conversion result.
int getADCVoltage(){
	
	//Starting Conversion, waiting for it to finish, clearing the flag, reading the result
	ADC_SoftwareStartConv(ADC1); 						   
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); 
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC); 					   //EOC: End Of Conversion
	
	return ADC_GetConversionValue(ADC1);
}

// conversion of a voltage value to temperature as per the formula provided in the board's manual.
float volt_to_celsius(uint_fast16_t voltage){
	return (((( (float)voltage / 4095) * V_MAX) - V_25) / AVG_SLOPE) + 25;
}

// various initializations.
void temp_sensor_init(){	
	config_TIM2();
	LED_GPIO_init();
	ADC_init();
}
