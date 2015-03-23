#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "background16bpp.h"

/* Private function prototypes -----------------------------------------------*/

static void example_1a(void);
static void example_1b(void);
static void example_1c(void);
static void delay(__IO uint32_t nCount);

int main ()
{	
	/* LCD initiatization */
  LCD_Init();
  
  /* LCD Layer initiatization */
  LCD_LayerInit();
    
  /* Enable the LTDC controler */
  LTDC_Cmd(ENABLE);
  
  /* Set LCD foreground layer as the current layer */
  LCD_SetLayer(LCD_FOREGROUND_LAYER);
	
	/*******************************************************
	         Uncomment the example you want to see
	example_1a: Simple shape draw, fill and text display
	example_1b: bitmap image display
	example_1c: Simple animation
	********************************************************/
	
	//example_1a();
	//example_1b();
	
	while(1){	
	//example_1c();
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
  * @brief    Illustartes a simple shape draw and fill, and string dsiplay
  * @function This function draws concentrated colour-filled circles. It also draw a square and a triangle. Some text at two
              different font sizes is displayed.
  * @param    None
  * @retval   None
  */

static void example_1a(void){
	
	 /* Clear the LCD */ 
    LCD_Clear(LCD_COLOR_WHITE);
	
	  //The files source and header files implement drawing characters (drawing strings)
	  //using different font sizes, see the file font.h for the four sizes
    LCD_SetFont(&Font8x8);
	  //The number of string lines avaialble is dependant on the font height:
	  //A font height of 8 will result in 320 / 8 = 40 lines
    LCD_DisplayStringLine(LINE(1), (uint8_t*)"      Welcome to uP lab     ");
    LCD_DisplayStringLine(LINE(2), (uint8_t*)"          Good Luck         ");
	  
	  //The stm32f429i_discovery_lcd.h file offers functions which allows to draw various shapes
	  //in either border or filled with colour. You can draw circles, rectangles, triangles, lines,
	  //ellipses, and polygons. You can draw strings or characters, change background/foreground 
	  //colours.
	
	  LCD_DrawLine(0, 32, 240, LCD_DIR_HORIZONTAL);
	  LCD_DrawLine(0, 34, 240, LCD_DIR_HORIZONTAL);
	  LCD_SetTextColor(LCD_COLOR_BLUE2); 
	  LCD_DrawFullCircle(120, 160, 100);
	  LCD_SetTextColor(LCD_COLOR_CYAN); 
	  LCD_DrawFullCircle(120, 160, 90);
	  LCD_SetTextColor(LCD_COLOR_YELLOW); 
	  LCD_DrawFullCircle(120, 160, 80);
	  LCD_SetTextColor(LCD_COLOR_RED); 
	  LCD_DrawFullCircle(120, 160, 70);
	  LCD_SetTextColor(LCD_COLOR_BLUE); 
	  LCD_DrawFullCircle(120, 160, 60);
	  LCD_SetTextColor(LCD_COLOR_GREEN); 
	  LCD_DrawFullCircle(120, 160, 50);
	  LCD_SetTextColor(LCD_COLOR_BLACK); 
	  LCD_DrawFullCircle(120, 160, 40);
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_DrawRect(90,130,60,60);
		LCD_SetTextColor(LCD_COLOR_MAGENTA);
		LCD_FillTriangle(90, 120, 150, 130, 180, 130);
		LCD_SetFont(&Font12x12);
		LCD_DisplayStringLine(LINE(15), (uint8_t*)"      Success!    ");
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
  * @brief    Displays a bitmap image
  * @function This function copies a bitmap image converted by STM32 Imager into an array "background16bpp.h" and stored
in flash memory into the active buffer in SDRAM. The SDRAM has two layer buffer:
              ->Background layer at address LCD_FRAME_BUFFER = 0xD0000000
              ->Foreground layer at address LCD_FRAME_BUFFER + BUFFER_OFFSET = 0xD0000000 + 0x00050000
              memcpy is a processor intiated and managed transfer. A more efficient way is to use DMA2D ChromeART acccelerator
              Once the image is copied into the active buffer (which we set by LCD_SetLayer(LCD_FOREGROUND_LAYER) command ), the
              LTDC updates the display when it continously refreshes the output display
  * @param    None
  * @retval   None
  */

static void example_1b(void){
	memcpy ( (void *)(LCD_FRAME_BUFFER + BUFFER_OFFSET), (void *) &background, sizeof(background));	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
  * @brief    Illustartes a simple animation program
  * @function This function draws concentrated circles emanating from the center towards the LCD edge in
              an animated fashion. It will look as a sonar or radar display. Then it simulates locking 
              onto a target by flashing a small red circle and displaying the text "Object located"
  * @param    None
  * @retval   None
  */
static void example_1c(void){
	/* Clear the LCD */ 
  LCD_Clear(LCD_COLOR_WHITE);
	LCD_SetFont(&Font8x8);
	LCD_DisplayStringLine(LINE(1), (uint8_t*)"  Radar Scanning for Object  ");
	
	LCD_SetTextColor(LCD_COLOR_BLUE2);
	LCD_DrawLine(10, 160, 220, LCD_DIR_HORIZONTAL);
	LCD_DrawLine(120, 50, 220, LCD_DIR_VERTICAL );
	
	LCD_SetTextColor(LCD_COLOR_BLUE2);
  LCD_DrawCircle(120, 160, 10);	
	delay(35);
	LCD_DrawCircle(120, 160, 20);	
	delay(35);
	LCD_DrawCircle(120, 160, 30);	
	delay(35);
	LCD_DrawCircle(120, 160, 40);	
	delay(35);
	LCD_DrawCircle(120, 160, 50);	
	delay(35);
	LCD_DrawCircle(120, 160, 60);	
	delay(35);
	LCD_DrawCircle(120, 160, 70);	
	delay(35);
	LCD_DrawCircle(120, 160, 80);	
	delay(35);
	LCD_DrawCircle(120, 160, 90);	
	delay(35);
	LCD_DrawCircle(120, 160, 100);	
	delay(35);
	LCD_SetTextColor(LCD_COLOR_RED);
	LCD_DisplayStringLine(LINE(36), (uint8_t*)"        Object Located    ");
	LCD_DrawFullRect(90,130,10,10);
	delay(25);
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_DrawFullRect(90,130,10,10);
	delay(25);
	LCD_SetTextColor(LCD_COLOR_RED);
	LCD_DrawFullRect(90,130,10,10);
	delay(25);
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_DrawFullRect(90,130,10,10);
	delay(25);
	LCD_SetTextColor(LCD_COLOR_RED);
	LCD_DrawFullRect(90,130,10,10);
	delay(25);
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_DrawFullRect(90,130,10,10);
	delay(25);
	LCD_SetTextColor(LCD_COLOR_RED);
	LCD_DrawFullRect(90,130,10,10);
	delay(25);
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_DrawFullRect(90,130,10,10);
	delay(25);
	LCD_SetTextColor(LCD_COLOR_RED);
	LCD_DrawFullRect(90,130,10,10);
	delay(25);
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
static void delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = 100000*nCount; index != 0; index--)
  {
  }
}