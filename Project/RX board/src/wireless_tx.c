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
	uint8_t byte = 0x9;
//	
//	CC2500_read(&byte, addr, 1);
//	printf("Read by at address %u: %u\n", addr, byte);
	
	for (int i = 0; i < NUM_CONF_REG; i++){
		CC2500_read(&byte, (uint8_t)i, 1);
		printf("Read by at address %X: %X\n", i, byte);
	}	
}

void read_beacons(){
	ready = 0;
	set_idle();
	send_strobe(0x3A);
	rx_enable();

	while (1){
		//while(!ready);
		//ready = 0;
	
		while(!ready);
		ready = 0;
				
		uint8_t buffer[64];
		memset(buffer, 0, 64);
		
		rx_enable();		
//		uint8_t buff_size = get_rx_buffer_size();
			
		uint8_t buff_size;
		//CC2500_read(&buff_size, 0xFB, 1);
//		printf("Buffer size: %u\n", buff_size);
		
		if (buff_size >= 0){
			read_rx_fifo(buffer, 1);
			data = buffer[0];
//			printf("Count: %c\n", (buffer[0] - '0'));
			
//			for (int i = 0; i < buff_size; i++){				
//				printf("Count: %c\n", (buffer[i] - '0'));
//				printf("TX FIFO FREE: %u\n", get_tx_buffer_size());
//				write_tx_fifo(&buffer[i], 1);
//			}			
		}
	}
}

void EXTI0_IRQHandler(){
	if (EXTI_GetITStatus(CC2500_SPI_INT1_EXTI_LINE) == SET){
		EXTI_ClearITPendingBit(CC2500_SPI_INT1_EXTI_LINE);
		ready = 1;
	}
}
