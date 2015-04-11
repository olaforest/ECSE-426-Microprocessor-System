
#include "LCD_draw.h"


void LCD_shape_clear(void){
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_DrawFullRect(0,110,240,100);
	LCD_DrawFullRect(40,110,160,100);
	LCD_FillTriangle(60, 120, 180, 210, 100, 210);
}

void LCD_inital_screen(void){
	/* Clear the LCD */ 
    LCD_Clear(LCD_COLOR_WHITE);
	
		LCD_SetFont(&Font12x12);
		
		LCD_SetTextColor(LCD_COLOR_BLACK);

		LCD_DisplayStringLine(LINE(10), (uint8_t*)"  Select the mode    ");
		LCD_DisplayStringLine(LINE(11), (uint8_t*)"  using letters:    ");
		LCD_DisplayStringLine(LINE(13), (uint8_t*)" A: Predefined Shapes");
		LCD_DisplayStringLine(LINE(14), (uint8_t*)" B: On the fly mode  ");
		LCD_DisplayStringLine(LINE(15), (uint8_t*)" C: Display numbers  ");
	
}

digit_draw_xy get_draw_coor(uint8_t digit_location, uint8_t number){
	digit_draw_xy data;
	
	
	
}
