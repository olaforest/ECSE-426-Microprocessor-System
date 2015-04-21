/*
ECSE 426 - Wireless 3D printing machine project
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Nuri Ege Kozan    (ID: 260359680)
Genevieve Nantel  (ID: 260481768)
Group #4
Date:	April 14th, 2015
*/ 
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
	TIM3_initStruct.TIM_Prescaler		= TIM3_PRESCALAR;
	TIM3_initStruct.TIM_CounterMode		= TIM_CounterMode_Up;
	TIM3_initStruct.TIM_Period			= TIM3_PERIOD;
	TIM3_initStruct.TIM_ClockDivision	= TIM_CKD_DIV1;	
	TIM_TimeBaseInit(TIM3, &TIM3_initStruct);
	
	// Enable and set TIM3 Interrupt to the highest priority
	NVIC_initStruct.NVIC_IRQChannel						= TIM3_IRQn;
	NVIC_initStruct.NVIC_IRQChannelPreemptionPriority	= 0x01;
	NVIC_initStruct.NVIC_IRQChannelSubPriority			= 0x00;
	NVIC_initStruct.NVIC_IRQChannelCmd					= ENABLE;
	NVIC_Init(&NVIC_initStruct);
	
	TIM_UpdateRequestConfig(TIM3, TIM_UpdateSource_Regular);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

// Initialization of wireless capabilities by configuring TIM3 hardware timer
// and initializing the CC2500 wireless board
void wireless_init(){
	config_tim3();
	CC2500_init();
}

//write to address 
void transmit(){
	uint8_t addr = 0x10;
	uint8_t byte = 0x7;
	
	CC2500_write(&byte, addr, 1);
	printf("Writing to address %u: %u\n", addr, byte);
}

// receive data from the buffer
void receive(){	
	
	uint8_t buffer[NUM_CONF_REG];
	CC2500_read(buffer, CC2500_REG_IOCFG2, NUM_CONF_REG);
	
	for (int i = 0; i < NUM_CONF_REG; i++)
		printf("Read by at address %X: %X\n", i, buffer[i]);
}


