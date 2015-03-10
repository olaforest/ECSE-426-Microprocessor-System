#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "keypad.h"
#include <stdio.h>
#include <ctype.h>

#define ALL_ROW_PINS GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3
#define ALL_COL_PINS GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7
#define ROW_1_PINS GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3
#define ROW_2_PINS GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3
#define ROW_3_PINS GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3
#define ROW_4_PINS GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2
#define COL_1_VALUE 0xE
#define COL_2_VALUE 0xD
#define COL_3_VALUE 0xB
#define COL_4_VALUE 0x7
#define NUM_ROWS 4
#define NUM_COLS 4
#define ENTER '#'

uint16_t row_pins[NUM_ROWS] = {ROW_1_PINS, ROW_2_PINS, ROW_3_PINS, ROW_4_PINS};
uint8_t col_values[NUM_COLS] = {COL_1_VALUE, COL_2_VALUE, COL_3_VALUE, COL_4_VALUE};
char keys[NUM_ROWS][NUM_COLS] = { '1', '2', '3', 'A',
	                               '4', '5', '6', 'B',
								   '7', '8', '9', 'C',
	                               '*', '0', '#', 'D' };

void keypad_init(void){
		
	GPIO_InitTypeDef GPIO_InitStruct;	
	
	//Give clock to the GPIO
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	//Initialize the rows as push-up output
	GPIO_InitStruct.GPIO_Pin   = ALL_ROW_PINS;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// Initialize the columns as input
	GPIO_InitStruct.GPIO_Pin   = ALL_COL_PINS;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
	//GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3);
}

uint8_t read_cols(void){
	return  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) |
		   (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) << 1) |
		   (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) << 2) |
		   (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) << 3);
}

int get_key(char * key){	
	int i, j;
	for (i = 0; i < NUM_ROWS; i++){
		GPIO_ResetBits(GPIOA, ALL_ROW_PINS);
		GPIO_SetBits(GPIOA, row_pins[i]);
		uint8_t col_value = read_cols();	
		for (j = 0; j < NUM_COLS; j++){
			if (col_values[j] == col_value){
				*key = keys[i][j];	
				return 1;
			}	
		}
	}
	
	*key = 'x';
	return 0;
}

uint8_t get_keys(void){
	int num_digits = 0;
	int max_presses = 1000;
	char last_key = 'x', key;
	int pressed = 0;
	while (num_digits < 3 && key != ENTER){
		//while(!(pressed = get_key(&key));
				
		if (get_key(&key)){
			if (isdigit(key)){
				if (key != last_key){
					last_key = key;
					pressed = 0;
				}
				
				if (pressed == max_presses){
					num_digits++;
					printf("Key: %c\n", key);
				} else if (pressed < max_presses){
					pressed++;
				}
			} 
		} else if (key == ENTER){
			// DONE getting input
		}
//		} else { 
//			printf("Not a digit: %c\n", key);
//		}
	}
	
	return 0;
}
