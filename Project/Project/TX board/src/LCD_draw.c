
#include "LCD_draw.h"


void LCD_shape_clear(void){
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_DrawFullRect(70,110,100,100);
	LCD_DrawFullRect(40,110,160,100);
	LCD_FillTriangle(60, 120, 180, 210, 100, 210);
}

void LCD_inital_screen(void){
	/* Clear the LCD */ 
    LCD_Clear(LCD_COLOR_WHITE);
	
		LCD_SetFont(&Font12x12);
		
		LCD_SetTextColor(LCD_COLOR_BLACK);

		LCD_DisplayStringLine(LINE(10), (uint8_t*)"  Select the mode   ");
		LCD_DisplayStringLine(LINE(11), (uint8_t*)"  using keys A or B   ");
}
