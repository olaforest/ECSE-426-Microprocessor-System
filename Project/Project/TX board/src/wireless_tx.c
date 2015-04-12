#include "stdio.h"
#include "string.h"
#include "wireless_tx.h"


// Definition for rates (in Hz), prescalar and period for the TIM3 clock.
#define TIM3_COUNTER_CLK 1282
#define TIM3_PRESCALAR ((SystemCoreClock / 2) / TIM3_COUNTER_CLK) - 1
#define TIM3_DESIRED_RATE 1
#define TIM3_PERIOD (TIM3_COUNTER_CLK / TIM3_DESIRED_RATE) - 1

volatile int ready;
uint8_t data;


static void delay(__IO uint32_t nCount){
	__IO uint32_t index = 0; 
	for(index = 100000*nCount; index != 0; index--){}
}

// Handler routine for the TIM3 interrupts
void TIM3_IRQHandler(){
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET){
		ready = 1;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}


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

// Initialization of wireless
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
	
	uint8_t buffer[NUM_CONF_REG];
	
	CC2500_read(buffer, CC2500_REG_IOCFG2, NUM_CONF_REG);
	for (int i = 0; i < NUM_CONF_REG; i++){
		printf("Read by at address %X: %X\n", i, buffer[i]);
	}
}

void send_data(uint8_t *data){
	set_idle();
	flush_tx_fifo();
	tx_enable();
	
	for (int i = 0; i < 4; i++){
		write_tx_fifo(&(data[i]), 1);
		delay(10);
	}
}

