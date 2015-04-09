#include "stm32f429i_discovery_lcd.h"


void LCD_shape_clear(void);
void LCD_inital_screen(void);

typedef struct{
	uint16_t x1;
	uint16_t x2;
	uint16_t y1;
	uint16_t y2;	
	
	
}memory_unit;