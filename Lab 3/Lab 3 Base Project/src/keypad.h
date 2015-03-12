#ifndef _KEYPAD_
#define _KEYPAD_

#include "math.h"
#include "segment_display.h"

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
#define DUMMY_KEY 'x'
#define KEY_SCAN_FREQ 10
#define KEY_SCAN_CLK_DIV (TIM3_DESIRED_RATE / KEY_SCAN_FREQ)

void keypad_init(void);
uint8_t read_cols(void);
int get_key(char * key);
int get_target_angle(volatile uint_fast16_t * ready);

#endif
