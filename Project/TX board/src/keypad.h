/*
ECSE 426 - Wireless 3D printing machine project
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Nuri Ege Kozan    (ID: 260359680)
Genevieve Nantel  (ID: 260481768)
Group #4
Date:	April 14th, 2015
*/

#ifndef _KEYPAD_
#define _KEYPAD_

#define KEYPAD_READY	  0x08

#define M_PI 3.14159265358979323846

#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "math.h"
#include <ctype.h>

#define ALL_ROW_PINS GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 //rows are GPIOE
#define ALL_COL_PINS GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 //columns are GPIOD
#define ROW_1_PINS GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5
#define ROW_2_PINS GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5
#define ROW_3_PINS GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5
#define ROW_4_PINS GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4
#define COL_1_VALUE 0xE
#define COL_2_VALUE 0xD
#define COL_3_VALUE 0xB
#define COL_4_VALUE 0x7
#define NUM_ROWS 4
#define NUM_COLS 4
#define A_KEY 'A'
#define B_KEY 'B'
#define C_KEY 'C'
#define D_KEY 'D'
#define DUMMY_KEY 'x'
#define KEY_SCAN_FREQ 4
#define KEY_SCAN_CLK_DIV (TIM3_DESIRED_RATE / KEY_SCAN_FREQ)


/* Exported functions --------------------------------------------------------*/

void keypad_init(void);
uint8_t read_cols(void);
int get_key(char * key);
char get_input(void);

#endif
