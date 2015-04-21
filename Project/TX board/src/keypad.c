/*
ECSE 426 - Wireless 3D printing machine project
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Nuri Ege Kozan    (ID: 260359680)
Genevieve Nantel  (ID: 260481768)
Group #4
Date:	April 14th, 2015
*/

#include "keypad.h"

// Sets up look up tables for row pin values, column pin values and key values
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
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	
	//Initialize the rows as push-up output
	GPIO_InitStruct.GPIO_Pin   = ALL_ROW_PINS;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	// Initialize the columns as input
	GPIO_InitStruct.GPIO_Pin   = ALL_COL_PINS;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
}

// read and appeand the bits corresponding to the column values.
uint8_t read_cols(void){
	return  GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2) |
		   (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_4) << 1) |
		   (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5) << 2) |
		   (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7) << 3);
}

// Get the pressed key. returns 1 if a key was found to be pressed and 0 otherwise.
int get_key(char * key){	
	
	int i, j;
	
	for (i = 0; i < NUM_ROWS; i++){
		// reset the row bits and then set to a new possible row pattern.
		GPIO_ResetBits(GPIOE, ALL_ROW_PINS);
		GPIO_SetBits(GPIOE, row_pins[i]);
		
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

// Gets an input from the keypad. Returns the pressed key, or a dummy charachter if no (valid) key was pressed.
char get_input(void){
	char key;
	if(get_key(&key))return key;
	return DUMMY_KEY;
}
