#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"

int main(){
		
	// This will allow use of the GPIO pins in bank A (PA0, PA1, ....)
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef gpio_init_s;
	
	//GPIO_StructInit(&gpio_init_s);
	gpio_init_s.GPIO_Pin = GPIO_Pin_All; // Select pin 4
	gpio_init_s.GPIO_Mode = GPIO_Mode_AN; // Set as output
	gpio_init_s.GPIO_Speed = GPIO_Speed_100MHz; // Don't limit slew rate
	gpio_init_s.GPIO_OType = GPIO_OType_PP; // Push-pull
	gpio_init_s.GPIO_PuPd = GPIO_PuPd_NOPULL; // Not input, don't pull

	// Actually configure that pin
	GPIO_Init(GPIOA, &gpio_init_s);
	
	//GPIO_SetBits(GPIOA, GPIO_Pin_4);
	//GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	//GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET); //(or Bit_RESET)
	//GPIO_Write(GPIOA, 0x16);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //Clock Gating
	
	ADC_CommonInitTypeDef adc_common_init_s;
	
	adc_common_init_s.ADC_Mode = ADC_Mode_Independent;
	adc_common_init_s.ADC_Prescaler = ADC_Prescaler_Div2;
	adc_common_init_s.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	adc_common_init_s.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	
	ADC_CommonInit(&adc_common_init_s); //Initialization
	
	ADC_InitTypeDef adc_init_s;
	adc_init_s.ADC_Resolution = ADC_Resolution_12b;
	adc_init_s.ADC_ScanConvMode = DISABLE;
	adc_init_s.ADC_ContinuousConvMode = DISABLE;
	adc_init_s.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	adc_init_s.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	adc_init_s.ADC_DataAlign = ADC_DataAlign_Right;
	adc_init_s.ADC_NbrOfConversion = 1;
	
	ADC_Init(ADC1, &adc_init_s);
	
	ADC_Cmd(ADC1, ENABLE);
	ADC_TempSensorVrefintCmd(ENABLE);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_480Cycles);
	
	ADC_SoftwareStartConv(ADC1); //Starting Conversion, waiting for it to finish, clearing the flag, reading the result
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); //Could be through interrupts (Later)
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC); //EOC means End Of Conversion
	ADC_GetConversionValue(ADC1);
	
	//SysTick_Config test;
	
	//RCC->AHB1ENR |= (1<<19);
	
	//GPIOD->MODER |= (1<<24);
	
	//GPIOD->ODR |= (1<<8);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitTypeDef gpio_init_s2;
	
	//GPIO_StructInit(&gpio_init_s);
	gpio_init_s2.GPIO_Pin = GPIO_Pin_12; // Select pin 4
	gpio_init_s2.GPIO_Mode = GPIO_Mode_OUT; // Set as output
	gpio_init_s2.GPIO_Speed = GPIO_Speed_100MHz; // Don't limit slew rate
	gpio_init_s2.GPIO_OType = GPIO_OType_PP; // Push-pull
	gpio_init_s2.GPIO_PuPd = GPIO_PuPd_NOPULL; // Not input, don't pull
	
	GPIO_Init(GPIOA, &gpio_init_s2);
	
	GPIO_SetBits(GPIOD, GPIO_Pin_12);
	
	while(1){
		
		for (int i = 0; i < 1000000; i++);
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		for (int i = 0; i < 1000000; i++);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
	}
	
	return 0;
}