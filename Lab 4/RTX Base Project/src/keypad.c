/*
ECSE 426 - Lab 3
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Group #3
Date:	March 16, 2015
*/ 

#include "keypad.h"

uint16_t row_pins[NUM_ROWS] = {ROW_1_PINS, ROW_2_PINS, ROW_3_PINS, ROW_4_PINS};
uint8_t col_values[NUM_COLS] = {COL_1_VALUE, COL_2_VALUE, COL_3_VALUE, COL_4_VALUE};
char keys[NUM_ROWS][NUM_COLS] = { '1', '2', '3', 'A',
								  '4', '5', '6', 'B',
								  '7', '8', '9', 'C',
								  '*', '0', '#', 'D' };

// initialize the keypad
void keypad_init(void){
		
	GPIO_InitTypeDef GPIO_InitStruct;	
	
	//Give clock to the GPIO
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	//Initialize the rows as push-up output
	GPIO_InitStruct.GPIO_Pin   = ALL_ROW_PINS;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(KEYPAD_GPIO, &GPIO_InitStruct);
	
	// Initialize the columns as input
	GPIO_InitStruct.GPIO_Pin   = ALL_COL_PINS;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_Init(KEYPAD_GPIO, &GPIO_InitStruct);
}

// read and appeand the bits corresponding to the column values.
uint8_t read_cols(void){
	return  GPIO_ReadInputDataBit(KEYPAD_GPIO, GPIO_Pin_11) |
		   (GPIO_ReadInputDataBit(KEYPAD_GPIO, GPIO_Pin_12) << 1) |
		   (GPIO_ReadInputDataBit(KEYPAD_GPIO, GPIO_Pin_13) << 2) |
		   (GPIO_ReadInputDataBit(KEYPAD_GPIO, GPIO_Pin_14) << 3);
}

// Get the pressed key. returns 1 if a key was found to be pressed and 0 otherwise.
int get_key(char * key){	
	int i, j;
	for (i = 0; i < NUM_ROWS; i++){
		// reset the row bits and then set to a new possible row pattern.
		GPIO_ResetBits(KEYPAD_GPIO, ALL_ROW_PINS);
		GPIO_SetBits(KEYPAD_GPIO, row_pins[i]);
		
		uint8_t col_value = read_cols();	
		
		// iterate through all the possible column values (or stop early if a match was found).
		for (j = 0; j < NUM_COLS; j++){
			if (col_values[j] == col_value){
				*key = keys[i][j];	
				
				return 1;
			}	
		}
	}	
	*key = DUMMY_KEY;
	
	return 0;
}

char get_input(void){
	char key;
	if (get_key(&key)){
		if (key == '1' || key == '2' || key == '3' || key == '4' || key == TEMP_MODE_KEY || key == MEMS_MODE_KEY){
			return key;
		}
	}
	
	return DUMMY_KEY;
}
