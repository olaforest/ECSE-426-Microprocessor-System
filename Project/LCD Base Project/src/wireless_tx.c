#include "stdio.h"
#include "string.h"
#include "wireless_tx.h"

volatile int ready;

void wireless_init(){
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
	rx_enable();
//	tx_enable();
	
	while (1){
		//while(!ready);
		//ready = 0;
	
		uint8_t buffer[255];
		memset(buffer, 0, 255);
		uint8_t buff_size = get_rx_buffer_size();
		
//		for (int i = 0; i < 5; i++){		
//			uint8_t crap = ((uint8_t)0x00);
//			printf("TX FIFO FREE: %u\n", get_tx_buffer_size());
//			write_tx_fifo(&crap, 1);
//		}	
				
		if (buff_size >= 0){
			read_rx_fifo(buffer, buff_size);
			
			printf("Count: %c\n", (buffer[0] - '0'));
			
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
