#include "stdio.h"
#include "string.h"
#include "wireless_rx.h"

// Configuration of TIM3
void config_tim3(void){
	
	// Enable TIM3 clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM3_initStruct;
	NVIC_InitTypeDef NVIC_initStruct;
	
	// Configure TIM3 timer
	TIM3_initStruct.TIM_Prescaler			= TIM3_PRESCALAR; 	//old: 0xFFFF;
	TIM3_initStruct.TIM_CounterMode		= TIM_CounterMode_Up;
	TIM3_initStruct.TIM_Period				= TIM3_PERIOD; 		//old: 0x0008;
	TIM3_initStruct.TIM_ClockDivision	= TIM_CKD_DIV1;	
	TIM_TimeBaseInit(TIM3, &TIM3_initStruct);
	
	// Enable and set TIM3 Interrupt to the highest priority
	NVIC_initStruct.NVIC_IRQChannel										= TIM3_IRQn;
	NVIC_initStruct.NVIC_IRQChannelPreemptionPriority	= 0x01;
	NVIC_initStruct.NVIC_IRQChannelSubPriority				= 0x00;
	NVIC_initStruct.NVIC_IRQChannelCmd								= ENABLE;
	NVIC_Init(&NVIC_initStruct);
	
	TIM_UpdateRequestConfig(TIM3, TIM_UpdateSource_Regular);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

void wireless_init(){
	config_tim3();
	CC2500_init();
}

void transmit(){
	uint8_t addr = 0x10;
	uint8_t byte = 0x7;
	
	CC2500_write(&byte, addr, 1);
	printf("Writing to address %u: %u\n", addr, byte);
}

void receive(){
//	uint8_t addr = 0x10;
//	uint8_t byte;
//	
//	CC2500_read(&byte, addr, 1);
//	printf("Read by at address %u: %u\n", addr, byte);
	
	
//	for (int i = 0; i < NUM_CONF_REG; i++){
//		CC2500_read(&byte, (uint8_t)i, 1);
//		printf("Read by at address %X: %X\n", i, byte);
//	}	
	
	uint8_t buffer[NUM_CONF_REG];
	CC2500_read(buffer, CC2500_REG_IOCFG2, NUM_CONF_REG);
	for (int i = 0; i < NUM_CONF_REG; i++){
		printf("Read by at address %X: %X\n", i, buffer[i]);
	}
}


