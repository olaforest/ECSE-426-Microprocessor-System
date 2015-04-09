/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"                  // Device header

#include "stm32f4xx_conf.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
//#include "stm32f429i_discovery_l3gd20.h"

#include "keypad.h"
#include "LCD_draw.h"

#include <stdio.h>
#include <string.h>
#include <Math.h>

//mode select is used to switch between predefined shape and on the fly mode
//'A' is for predefined shapes, 'B' is for on the fly
//initialized to C so its not one of the above
char mode_select = 'C';
//predefined shape selection: 0 for on the fly mode, 1 for square, 2 for rect, 3 for triangle
//initialized to 10 so that it is not one of the above options to start with
uint8_t shape_select = 10;
//keyboard input value
char key;

//new draw line params.
int x1 = 0;
int x2 = 0;
int y1 = 0;
int y2 = 0;


int lcd_memory_count = 0;
memory_unit lcd_draw_memory[20]; 





osThreadId keypad_thread;
osThreadId LCD_control_thread;
osThreadId Data_send_thread;


static void delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = 100000*nCount; index != 0; index--)
  {
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
  * @brief    Illustartes a simple shape draw and fill, and string dsiplay
  * @function This function draws concentrated colour-filled circles. It also draw a square and a triangle. Some text at two
              different font sizes is displayed.
  * @param    None
  * @retval   None
  */

void example_1a(void const *argument){
	while(1){
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
		
		osDelay(250);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//bitmap method deleted
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
  * @brief    Illustartes a simple animation program
  * @function This function draws concentrated circles emanating from the center towards the LCD edge in
              an animated fashion. It will look as a sonar or radar display. Then it simulates locking 
              onto a target by flashing a small red circle and displaying the text "Object located"
  * @param    None
  * @retval   None
  */

void example_1c(void const *argument){
	while(1){
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
		
		osDelay(250);
	}
}

void keypad(void const * arg){

	char last_key = DUMMY_KEY;
	
	int second1 = 1;
	int second3 = 1;
	int second7 = 1;
	int second9 = 1;
	
	
	double degree = 15.0;
	
	
	
	keypad_init();
	
	while(1){
		key = get_input();
			
		
		
		if (last_key == DUMMY_KEY && key !=DUMMY_KEY){		
			// do something "with key"
			printf("%c\n", key);
			
			
			
			//select the mode
			if(key == 'A'){
				mode_select = 'A';
			}else if(key == 'B'){
				mode_select = 'B';
			}
			
			//# as reset button
			if(key == '#'){
				x1 = 0;
				x2 = 0;
				y1 = 0;
				y2 = 0;
			}
			
			if(key == '1'){
				x2 = x1 - 40*(cos((second1 * degree) * (M_PI / 180.0)));
				y2 = y1 + 40*(sin((second1 * degree) * (M_PI / 180.0)));
				second1++;
				if(second1 == 6){
					second1 = 1;
				}
			}else if(key == '2'){
				second1 = 1;
				second3 = 1;
				second7 = 1;
				second9 = 1;
				
				x2 = x1;
				y2 = y1 + 40;
			}else if(key == '3'){
				x2 = x1 + 40*(cos((second3 * degree) * (M_PI / 180.0)));
				y2 = y1 + 40*(sin((second3 * degree) * (M_PI / 180.0)));
				second3++;
				if(second3 == 6){
					second3 = 1;
				}
			}else if(key == '4'){
				second1 = 1;
				second3 = 1;
				second7 = 1;
				second9 = 1;
				
				x2 = x1 - 40;
				y2 = y1;
			}else if(key == '6'){
				second1 = 1;
				second3 = 1;
				second7 = 1;
				second9 = 1;
				
				x2 = x1 + 40;
				y2 = y1;
			}else if(key == '7'){
				x2 = x1 - 40*(cos((second7 * degree) * (M_PI / 180.0)));
				y2 = y1 - 40*(sin((second7 * degree) * (M_PI / 180.0)));
				second7++;
				if(second7 == 6){
					second7 = 1;
				}
			}else if(key == '8'){
				second1 = 1;
				second3 = 1;
				second7 = 1;
				second9 = 1;
				
				x2 = x1;
				y2 = y1 - 40;
			}else if(key == '9'){
				x2 = x1 + 40*(cos((second9 * degree) * (M_PI / 180.0)));
				y2 = y1 - 40*(sin((second9 * degree) * (M_PI / 180.0)));
				second9++;
				if(second9 == 6){
					second9 = 1;
				}
				
			}
			
			
			
			
			
			osSignalSet(LCD_control_thread, 0x01);
			
			//enter key to send to draw
			if(key == 'D'){
				
				if(shape_select == 0){
					LCD_SetTextColor(LCD_COLOR_RED);
					LCD_DrawUniLine(x1, 280 - y1, x2, 280 - y2);
					lcd_draw_memory[lcd_memory_count].x1 = x1;
					lcd_draw_memory[lcd_memory_count].x2 = x2;
					lcd_draw_memory[lcd_memory_count].y1 = y1;
					lcd_draw_memory[lcd_memory_count].y2 = y2;
					lcd_memory_count++;
					x1 = x2;
					y1 = y2;
					LCD_SetFont(&Font12x12);
					LCD_SetTextColor(LCD_COLOR_BLACK);			
					LCD_DisplayStringLine(LINE(25), (uint8_t*)"     Please wait      ");
					
			

					
				}else{
					LCD_Clear(LCD_COLOR_WHITE);
					LCD_SetFont(&Font12x12);	
					LCD_SetTextColor(LCD_COLOR_BLACK);
					LCD_DisplayStringLine(LINE(10), (uint8_t*)"  Please wait while   ");
					LCD_DisplayStringLine(LINE(11), (uint8_t*)"  drawing the shape   ");
					
				}
				
				//send signal to a thread to send data wirelessly
				osSignalSet(Data_send_thread, 0x01);
				
				
				
				
				
			}
			
			

		}
		
		if(mode_select == 'B'){
			osSignalSet(LCD_control_thread, 0x01);
		}
		
		
		
		last_key = key;
		osDelay(80);	
	}
}

void LCD_control(void const * arg){
	LCD_inital_screen();
	
	char last_mode = 'C';
	int counter;

	
	while(1){
		osSignalWait(0x01, osWaitForever);

////////////////////////////////////////////////////////////////////////////////////////////////
//Predefined shape mode		
		if(mode_select == 'A'){
			if(last_mode != 'A'){
				/* Clear the LCD */ 
				LCD_Clear(LCD_COLOR_WHITE);
			}
			
			LCD_Clear(LCD_COLOR_WHITE);
			LCD_SetFont(&Font12x12);
		
			LCD_SetTextColor(LCD_COLOR_BLACK);

			LCD_DisplayStringLine(LINE(2), (uint8_t*)"  Select the shape  ");
			LCD_DisplayStringLine(LINE(3), (uint8_t*)"  using keys 1,2,3  ");
			
			LCD_SetFont(&Font8x12);
			LCD_DisplayStringLine(LINE(24), (uint8_t*)"     Press 'D' key to send    ");

			if(key == '1'){
				LCD_shape_clear();
				LCD_SetTextColor(LCD_COLOR_RED);
				LCD_DrawFullRect(70,110,100,100);
				shape_select = 1;
			}else if(key == '2'){
				LCD_shape_clear();
				LCD_SetTextColor(LCD_COLOR_RED);
				LCD_DrawFullRect(40,110,160,100);
				shape_select = 2;
			}else if(key == '3'){
				LCD_shape_clear();
				LCD_SetTextColor(LCD_COLOR_RED);
				LCD_FillTriangle(60, 120, 180, 210, 100, 210);
				shape_select = 3;
			}
		
			
			last_mode = 'A';
		
////////////////////////////////////////////////////////////////////////////////////////////////
// on the fly mode
		}else if(mode_select == 'B'){
			if(last_mode != 'B'){
				/* Clear the LCD */ 
				LCD_Clear(LCD_COLOR_WHITE);
				x1 = 0;
				x2 = 0;
				y1 = 0;
				y2 = 0;
			}
			shape_select = 0;
		
			
			//blinking animation
			//LCD_SetLayer(LCD_FOREGROUND_LAYER);
			LCD_SetTextColor(LCD_COLOR_RED);
			for(counter = 0; counter < lcd_memory_count; counter++){
				LCD_DrawUniLine(lcd_draw_memory[counter].x1, 280 - lcd_draw_memory[counter].y1, lcd_draw_memory[counter].x2, 280 - lcd_draw_memory[counter].y2);
			}
			LCD_SetTextColor(LCD_COLOR_RED);
			LCD_DrawUniLine(x1, 280 - y1, x2, 280 - y2);
			delay(20);
			LCD_SetTextColor(LCD_COLOR_WHITE);
			LCD_DrawUniLine(x1, 280 - y1, x2, 280 - y2);

			//LCD_SetLayer(LCD_BACKGROUND_LAYER);
			
			
			
			
			
			
			
			last_mode = 'B';
		}
		

		
	}
	
	
	
}

void Data_send(void const * arg){
	while(1){
		osSignalWait(0x01, osWaitForever);
		printf("Sending data to the controller board!\n");
		
		
		//send data wirelessly here
		//always send shape select, if it is 0, it means we are in on the fly mode
		//if it is 1,2 or 3, draw one of the predefined shapes
		
		if(shape_select){
			
			
			//erase "please wait" message
			delay(50);
			osSignalSet(LCD_control_thread, 0x01);
		}else{
			
			
			//erase "please wait" text after the delay of drawing
			delay(50);
			LCD_SetTextColor(LCD_COLOR_WHITE);
			LCD_DrawFullRect(0,281,240,39);
		}
		

		
		

	}
}



//osThreadDef(example_1a, osPriorityNormal, 1, 0);
//osThreadDef(example_1b, osPriorityNormal, 1, 0);
//osThreadDef(example_1c, osPriorityNormal, 1, 0);

osThreadDef(keypad, osPriorityBelowNormal, 1, 0);
osThreadDef(LCD_control, osPriorityNormal, 1, 0);
osThreadDef(Data_send, osPriorityAboveNormal, 1, 0);

// ID for theads
//osThreadId example_1a_thread;
//osThreadId example_1b_thread;
//osThreadId example_1c_thread;



/*
 * main: initialize and start the system
 */
int main (void) {
  osKernelInitialize ();                    // initialize CMSIS-RTOS
	
  // initialize peripherals here
	/* LCD initiatization */
  LCD_Init();
  
  /* LCD Layer initiatization */
  LCD_LayerInit();
    
  /* Enable the LTDC controler */
  LTDC_Cmd(ENABLE);
  
  /* Set LCD foreground layer as the current layer */
  LCD_SetLayer(LCD_FOREGROUND_LAYER);
	
	
	
  // create 'thread' functions that start executing,
  // example: tid_name = osThreadCreate (osThread(name), NULL);
	
	/*******************************************************
	         Uncomment the example you want to see
	example_1a: Simple shape draw, fill and text display
	example_1b: bitmap image display
	example_1c: Simple animation
	********************************************************/
	
	//example_1a_thread = osThreadCreate(osThread(example_1a), NULL);
	//example_1b_thread = osThreadCreate(osThread(example_1b), NULL);
	//example_1c_thread = osThreadCreate(osThread(example_1c), NULL);
	
	
	keypad_thread = osThreadCreate(osThread(keypad), NULL);
	LCD_control_thread = osThreadCreate(osThread(LCD_control), NULL);
	Data_send_thread = osThreadCreate(osThread(Data_send), NULL);
	
	osKernelStart ();                         // start thread execution 
}


