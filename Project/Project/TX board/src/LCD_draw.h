/*
ECSE 426 - Wireless 3D printing machine project
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Nuri Ege Kozan    (ID: 260359680)
Genevieve Nantel  (ID: 260481768)
Group #4
Date:	April 14th, 2015
*/

#ifndef __LCD_DRAW__
#define __LCD_DRAW__

#include "stm32f429i_discovery_lcd.h"


/* Exported types ------------------------------------------------------------*/

// Sets of X,Y coordinates for the memory unit structure definition
typedef struct{
	uint16_t x1;
	uint16_t x2;
	uint16_t y1;
	uint16_t y2;	
}memory_unit;

// X and Y coordinates for digit drawing structure definition
typedef struct{
	uint8_t x;
	uint8_t y;
}digit_draw_xy;


/* Exported functions --------------------------------------------------------*/

void LCD_shape_clear(void);
void LCD_inital_screen(void);
digit_draw_xy get_draw_coor(uint8_t digit_location, uint8_t number);

#endif