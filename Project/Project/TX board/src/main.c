/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                 // define objects in main module
#include "osObjects.h"                  // RTOS object definitions
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"

#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
//#include "stm32f429i_discovery_l3gd20.h"

#include "wireless_tx.h"
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
char key_C;

int MEMS_activate = 0;

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

// Creates a delay based on the input value
static void delay(__IO uint32_t nCount){
	__IO uint32_t index = 0; 
	for(index = 100000*nCount; index != 0; index--){}
}

// Thread responsible for handling the user input throught the keypad
void keypad(void const * arg){

	char last_key = DUMMY_KEY;
	
	uint8_t data[4];
	data[0] = 255;
	
	uint8_t second1 = 1;
	uint8_t second3 = 1;
	uint8_t second7 = 1;
	uint8_t second9 = 1;
	
	double degree = 15.0;
	
	keypad_init();
	
	while(1){
		key = get_input();
		
		
		if (last_key == DUMMY_KEY && key !=DUMMY_KEY){		
			if(key == '5'){
			MEMS_activate = !MEMS_activate;
			if(MEMS_activate && mode_select == 'B'){
				LCD_SetFont(&Font12x12);
				LCD_SetTextColor(LCD_COLOR_BLACK);			
				LCD_DisplayStringLine(LINE(1), (uint8_t*)"     Activate      ");
			}else if(!MEMS_activate && mode_select == 'B'){
				LCD_SetFont(&Font12x12);
				LCD_SetTextColor(LCD_COLOR_BLACK);			
				LCD_DisplayStringLine(LINE(1), (uint8_t*)"     Deactivate      ");
			}				
		}
		
		if(key == '0'){
			key_C = '0';
		}else if (key == '1'){
			key_C = '1';
		}else if (key == '2'){
			key_C = '2';
		}else if (key == '3'){
			key_C = '3';
		}else if (key == '4'){
			key_C = '4';
		}else if (key == '5'){
			key_C = '5';
		}else if (key == '6'){
			key_C = '6';
		}else if (key == '7'){
			key_C = '7';
		}else if (key == '8'){
			key_C = '8';
		}else if (key == '9'){
			key_C = '9';
		}
		
			
		//Select the mode
		if(key == 'A'){
			MEMS_activate = 0;
			mode_select = 'A';
		}else if(key == 'B'){
			mode_select = 'B';
		}else if(key == 'C'){
			MEMS_activate = 0;
			mode_select = 'C';
		}
			
			//# as reset button
			if(key == '#'){
				x1 = 0;
				x2 = 0;
				y1 = 0;
				y2 = 0;
				
				//reset the motor positions as well
				data[1] = 0;
				data[2] = 0;
				data[3] = 0;
			
				send_data(data);
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
					//LCD_DrawUniLine(x1, 280 - y1, x2, 280 - y2);
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
		osDelay(50);	
	}
}

void LCD_control(void const * arg){
	LCD_inital_screen();
	
	char last_mode = 'D';
	int counter;
	
	
	
	uint8_t data[4];
	data[0] = 255;
	
	while(1){
		osSignalWait(0x01, osWaitForever);

////////////////////////////////////////////////////////////////////////////////////////////////
//Predefined shape mode		
		if(mode_select == 'A'){
			if(last_mode != 'A'){
				/* Clear the LCD */ 
				LCD_Clear(LCD_COLOR_WHITE);
			}
			

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
				LCD_SetTextColor(LCD_COLOR_BLACK);
				LCD_SetFont(&Font8x12);
				
				LCD_DisplayStringLine(LINE(5), (uint8_t*)"       On the fly mode.  ");
				LCD_DisplayStringLine(LINE(7), (uint8_t*)"       Please only draw  ");
				LCD_DisplayStringLine(LINE(8), (uint8_t*)"       within the box.   ");
				
				LCD_DrawUniLine(1, 147, 239, 147);
				LCD_DrawUniLine(1, 281, 239, 281);
				LCD_DrawUniLine(0, 147, 0, 281);
				LCD_DrawUniLine(239, 147, 239, 281);
				x1 = 0;
				x2 = 0;
				y1 = 0;
				y2 = 0;
				
				//reset the motor positions as well
				data[1] = 0;
				data[2] = 0;
				data[3] = 0;
			
				send_data(data);
				
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
			osDelay(20);
			LCD_SetTextColor(LCD_COLOR_WHITE);
			LCD_DrawUniLine(x1, 280 - y1, x2, 280 - y2);



			//LCD_SetLayer(LCD_BACKGROUND_LAYER);
			
			last_mode = 'B';
		}else if(mode_select == 'C'){
			if(last_mode != 'C'){
				/* Clear the LCD */ 
				LCD_Clear(LCD_COLOR_WHITE);
			}
			shape_select = 0;
			
			
			LCD_SetTextColor(LCD_COLOR_BLACK);
			LCD_SetFont(&Font12x12);
				
			LCD_DisplayStringLine(LINE(7), (uint8_t*)"      Numbers.  ");
			
			
			
			last_mode = 'C';
		}
	}
}

void Data_send(void const * arg){
	uint8_t data[4];
	data[0] = 255;
	uint8_t digit_location = 0;
	
	wireless_init();

	
	while(1){
		osSignalWait(0x01, osWaitForever);
		
		//printf("Sending data to the controller board!\n");
		

		//send data wirelessly here
		//shape select, if it is 0, it means we are in on the fly mode
		//if it is 1,2 or 3, draw one of the predefined shapes
		if(shape_select){
			//draw square
			if(shape_select == 1){
				data[1] = 25;
				data[2] = 5;
				data[3] = 0;
				
				send_data(data);
				delay(60);
				
				data[1] = 65;
				data[2] = 5;
				data[3] = 1;
				
				send_data(data);
				delay(60);
				
				data[1] = 65;
				data[2] = 45;
				data[3] = 1;
				
				send_data(data);
				delay(60);
				
				data[1] = 25;
				data[2] = 45;
				data[3] = 1;
				
				send_data(data);
				delay(60);
				
				data[1] = 25;
				data[2] = 5;
				data[3] = 1;
				
				send_data(data);
				delay(60);
				
				data[1] = 0;
				data[2] = 0;
				data[3] = 0;
				
				//draw rectangle
			}else if(shape_select == 2){
				data[1] = 15;
				data[2] = 5;
				data[3] = 0;
				
				send_data(data);
				delay(60);
				
				data[1] = 75;
				data[2] = 5;
				data[3] = 1;
				
				send_data(data);
				delay(60);
				
				data[1] = 75;
				data[2] = 45;
				data[3] = 1;
				
				send_data(data);
				delay(60);
				
				data[1] = 15;
				data[2] = 45;
				data[3] = 1;
				
				send_data(data);
				delay(60);
				
				data[1] = 15;
				data[2] = 5;
				data[3] = 1;
				
				send_data(data);
				delay(60);
				
				data[1] = 0;
				data[2] = 0;
				data[3] = 0;
				
				//draw triangle
			}else if(shape_select == 3){
				data[1] = 25;
				data[2] = 5;
				data[3] = 0;
				
				send_data(data);
				delay(60);
				
				data[1] = 45;
				data[2] = 45;
				data[3] = 1;
				
				send_data(data);
				delay(60);
				
				data[1] = 65;
				data[2] = 5;
				data[3] = 1;
				
				send_data(data);
				delay(60);
				
				data[1] = 25;
				data[2] = 5;
				data[3] = 1;
				
				send_data(data);
				delay(60);
				
				data[1] = 0;
				data[2] = 0;
				data[3] = 0;
			}
		
			delay(150);
			//erase "please wait" message
			LCD_shape_clear();
			osSignalSet(LCD_control_thread, 0x01);
		}else if(mode_select == 'B'){
			if(MEMS_activate){
				
				data[0] = 254;
				data[1] = 0;
				data[2] = 0;
				data[3] = 0;
				send_data(data);
				
				LCD_SetFont(&Font12x12);
				LCD_SetTextColor(LCD_COLOR_RED);
				LCD_DrawFullRect(0,282,240,38);
				LCD_SetTextColor(LCD_COLOR_BLACK);			
				LCD_DisplayStringLine(LINE(25), (uint8_t*)"     CRAZY MODE      ");
			}else{

			data[0] = 255;
			data[1] = x2/2.67;
			data[2] = y2/2.67;
			data[3] = 1;
			
			send_data(data);
			//erase "please wait" text after the delay of drawing
			delay(60);
			LCD_SetTextColor(LCD_COLOR_WHITE);
			LCD_DrawFullRect(0,282,240,38);
			}
			
			
		}
		
		if(mode_select == 'C'){
			if(key_C == '0'){
				
				data[1] = 2 + digit_location*15;
				data[2] = 5;
				data[3] = 0;
			
				send_data(data);
				delay(60);
				
				data[1] = 13 + digit_location*15;
				data[2] = 5;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 13 + digit_location*15;
				data[2] = 45;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 2 + digit_location*15;
				data[2] = 45;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 2 + digit_location*15;
				data[2] = 5;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 0;
				data[2] = 0;
				data[3] = 0;
				
				send_data(data);
				
				
				digit_location++;
			}else if(key_C == '1'){
				data[1] = 8 + digit_location*15;
				data[2] = 5;
				data[3] = 0;
			
				send_data(data);
				delay(60);
				
				data[1] = 8 + digit_location*15;
				data[2] = 45;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 6 + digit_location*15;
				data[2] = 40;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				
				data[1] = 0;
				data[2] = 0;
				data[3] = 0;
				
				send_data(data);
				
				
				digit_location++;
			}else if(key_C == '2'){
				data[1] = 2 + digit_location*15;
				data[2] = 45;
				data[3] = 0;
			
				send_data(data);
				delay(60);
				
				data[1] = 13 + digit_location*15;
				data[2] = 45;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 2 + digit_location*15;
				data[2] = 5;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 13 + digit_location*15;
				data[2] = 5;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 0;
				data[2] = 0;
				data[3] = 0;
				
				send_data(data);
				
				digit_location++;
			}else if(key_C == '3'){
				data[1] = 2 + digit_location*15;
				data[2] = 5;
				data[3] = 0;
			
				send_data(data);
				delay(60);
				
				data[1] = 13 + digit_location*15;
				data[2] = 5;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 13 + digit_location*15;
				data[2] = 45;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 2 + digit_location*15;
				data[2] = 45;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 13 + digit_location*15;
				data[2] = 23;
				data[3] = 0;
			
				send_data(data);
				delay(60);
				
				data[1] = 3 + digit_location*15;
				data[2] = 23;
				data[3] = 1;
				
				send_data(data);
				delay(60);
				
				data[1] = 0;
				data[2] = 0;
				data[3] = 0;
				
				send_data(data);
				
				
				digit_location++;
			}else if(key_C == '4'){
				data[1] = 2 + digit_location*15;
				data[2] = 45;
				data[3] = 0;
			
				send_data(data);
				delay(60);
				
				data[1] = 2 + digit_location*15;
				data[2] = 23;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 13 + digit_location*15;
				data[2] = 23;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 13 + digit_location*15;
				data[2] = 45;
				data[3] = 0;
			
				send_data(data);
				delay(60);
				
				data[1] = 13 + digit_location*15;
				data[2] = 5;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 0;
				data[2] = 0;
				data[3] = 0;
				
				send_data(data);
				
				digit_location++;
			}else if(key_C == '5'){
				data[1] = 2 + digit_location*15;
				data[2] = 5;
				data[3] = 0;
			
				send_data(data);
				delay(60);
				
				data[1] = 13 + digit_location*15;
				data[2] = 5;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 13 + digit_location*15;
				data[2] = 23;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 2 + digit_location*15;
				data[2] = 23;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 2 + digit_location*15;
				data[2] = 45;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 13 + digit_location*15;
				data[2] = 45;
				data[3] = 1;
				
				send_data(data);
				delay(60);
				
				data[1] = 0;
				data[2] = 0;
				data[3] = 0;
				
				send_data(data);
				
				digit_location++;
			}else if(key_C == '6'){
				data[1] = 13 + digit_location*15;
				data[2] = 45;
				data[3] = 0;
			
				send_data(data);
				delay(60);
				
				data[1] = 2 + digit_location*15;
				data[2] = 45;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 2 + digit_location*15;
				data[2] = 5;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 13 + digit_location*15;
				data[2] = 5;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 13 + digit_location*15;
				data[2] = 23;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 2 + digit_location*15;
				data[2] = 23;
				data[3] = 1;
				
				send_data(data);
				delay(60);
				
				data[1] = 0;
				data[2] = 0;
				data[3] = 0;
				
				send_data(data);
				
				
				digit_location++;
			}else if(key_C == '7'){
				data[1] = 2 + digit_location*15;
				data[2] = 5;
				data[3] = 0;
			
				send_data(data);
				delay(60);
				
				data[1] = 13 + digit_location*15;
				data[2] = 45;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 2 + digit_location*15;
				data[2] = 45;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 0;
				data[2] = 0;
				data[3] = 0;
				
				send_data(data);
				
				
				digit_location++;
			}else if(key_C == '8'){
				data[1] = 2 + digit_location*15;
				data[2] = 5;
				data[3] = 0;
			
				send_data(data);
				delay(60);
				
				data[1] = 13 + digit_location*15;
				data[2] = 5;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 13 + digit_location*15;
				data[2] = 45;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 2 + digit_location*15;
				data[2] = 45;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 2 + digit_location*15;
				data[2] = 5;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 2 + digit_location*15;
				data[2] = 23;
				data[3] = 0;
				
				send_data(data);
				
				delay(60);
				
				data[1] = 13 + digit_location;
				data[2] = 23;
				data[3] = 1;
				
				send_data(data);
				
				delay(60);
				
				data[1] = 0;
				data[2] = 0;
				data[3] = 0;
				
				send_data(data);
				
				
				digit_location++;
			}else if(key_C == '9'){
				data[1] = 2 + digit_location*15;
				data[2] = 5;
				data[3] = 0;
			
				send_data(data);
				delay(60);
				
				data[1] = 13 + digit_location*15;
				data[2] = 5;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 13 + digit_location*15;
				data[2] = 45;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 2 + digit_location*15;
				data[2] = 45;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 2 + digit_location*15;
				data[2] = 23;
				data[3] = 1;
			
				send_data(data);
				delay(60);
				
				data[1] = 13 + digit_location*15;
				data[2] = 23;
				data[3] = 1;
				
				send_data(data);
				delay(60);
				
				data[1] = 0;
				data[2] = 0;
				data[3] = 0;
				
				send_data(data);
				
				digit_location++;
			}
		}
		
		
	}
}

osThreadDef(keypad, osPriorityBelowNormal, 1, 0);
osThreadDef(LCD_control, osPriorityNormal, 1, 0);
osThreadDef(Data_send, osPriorityAboveNormal, 1, 0);


/*
 * main: initialize and start the system
 */

int main (void) {

	osKernelInitialize ();				// initialize CMSIS-RTOS
	
	// initialize peripherals here
	/* LCD initiatization */
	LCD_Init();
	
	/* LCD Layer initiatization */
	LCD_LayerInit();
	
	/* Enable the LTDC controler */
	LTDC_Cmd(ENABLE);
	
	/* Set LCD foreground layer as the current layer */
	LCD_SetLayer(LCD_FOREGROUND_LAYER);
	
	keypad_thread = osThreadCreate(osThread(keypad), NULL);
	LCD_control_thread = osThreadCreate(osThread(LCD_control), NULL);
	Data_send_thread = osThreadCreate(osThread(Data_send), NULL);
	
	osKernelStart ();					// start thread execution 
}


