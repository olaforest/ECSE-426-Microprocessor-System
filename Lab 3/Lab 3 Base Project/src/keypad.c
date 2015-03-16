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

// returns the angle that the user inputs using the keypad
int get_target_angle(volatile uint_fast16_t * ready){
		
	char key;
	int target_angle = 0;
	int keys_pressed = 0;		// counter of the number of keys that were pressed (only for digits)
	int count = 0;
	while (keys_pressed < 3 && key != ENTER){
		while (!(*ready));
		*ready = 0;
		
		// check if a it's time to scan for input. If yes, scan and see if the a key was pressed.
		if (!(count++ % KEY_SCAN_CLK_DIV) && get_key(&key)){
			if (isdigit(key)){
				printf("Pressed key: %c\n", key);
				keys_pressed++;
				// convert key to int and "append" to target angle;
				target_angle = 10 * target_angle + (key - '0');  
			} else if (key != ENTER){
				printf("Please enter a digit (or press %c to confirm choice)\n", ENTER);
			}
		}
		
		// make sure that the user entered a valid target angle, otherwise prompt to re-enter the target angle.
		if (target_angle > 180 || (key == ENTER && keys_pressed == 0)){
			printf("You didn't choose a valid target angle (0 to 180°) before pressing enter. Please try again.\n");
			keys_pressed = 0;
			target_angle = 0;
			key = DUMMY_KEY;
		}
	}
	
	return target_angle;
}
