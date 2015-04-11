#include "stm32f429i_discovery_lcd.h"




typedef struct{
	uint16_t x1;
	uint16_t x2;
	uint16_t y1;
	uint16_t y2;	
	
	
}memory_unit;

typedef struct{
	uint8_t x;
	uint8_t y;
}digit_draw_xy;

void LCD_shape_clear(void);
void LCD_inital_screen(void);

digit_draw_xy get_draw_coor(uint8_t digit_location, uint8_t number);

