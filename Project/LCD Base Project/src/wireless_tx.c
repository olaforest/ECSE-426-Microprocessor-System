#include "stdio.h"
#include "wireless_tx.h"

void wireless_init(){
	CC2500_Init();
}

void transmit(){
	uint8_t addr = 0x10;
	uint8_t byte = 0x7;
	
	CC2500_Write(&byte, addr, 1);
	printf("Writing to address %h: %u\n", addr, byte);
}

void receive(){
	uint8_t addr = 0x10;
	uint8_t byte = 0x9;
	
	CC2500_Read(&byte, addr, 1);
	
	printf("Read by at address %h: %u\n", addr, byte);
}
