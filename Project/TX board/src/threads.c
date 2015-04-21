/*
ECSE 426 - Wireless 3D printing machine project
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Nuri Ege Kozan    (ID: 260359680)
Genevieve Nantel  (ID: 260481768)
Group #4
Date:	April 14th, 2015
*/

#include "threads.h"

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

// Thread Definitions
osThreadDef(keypad_thread, osPriorityBelowNormal, 1, 0);
osThreadDef(LCD_control_thread, osPriorityNormal, 1, 0);
osThreadDef(Data_send_thread, osPriorityAboveNormal, 1, 0);

// Keypad thread handler
void keypad_thread(void const * arg){

	char last_key = DUMMY_KEY;
	
	// Packet data buffer, the first byte is a synchronizing byte
	uint8_t data[4];
	data[0] = NON_MEMS_MODE;
	
	uint8_t second1 = 1;
	uint8_t second3 = 1;
	uint8_t second7 = 1;
	uint8_t second9 = 1;
	
	// Degree increment for on the fly mode
	double degree = 15.0;
	
	keypad_init();
	
	while(1){
		key = get_input();
		
		if (last_key == DUMMY_KEY && key !=DUMMY_KEY){
			
			// Key '5' toggles on and off the 'crazy mode' in which the mems sensor of the receiving 
			// board is turned on and off and its pitch and roll can be used to move the printer
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
			
			// Sets the selected key for the digit mode operation
			if (key >= '0' && key <= '9')
				key_C = key;
				
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
			
			// The '#' key is used as a reset button
			if(key == '#'){
				
				// Resets the draw line parameters
				x1 = 0;
				x2 = 0;
				y1 = 0;
				y2 = 0;
				
				// Resets the motor positions as well
				data[1] = 0;
				data[2] = 0;
				data[3] = 0;
			
				send_data(data);
			}
			
			// Sets the draw line parameters when a numerical key between '1' and '9' is pressed
			// for the on-the-fly mode.
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
			
			// Sends signal to LCD_control thread to display the appropriate screen
			osSignalSet(LCD_control_tid, LCD_READY);
			
			// The key 'D' sends the users choice to be drawn by the printer
			if(key == 'D'){
				
				if(shape_select == 0){
					LCD_SetTextColor(LCD_COLOR_RED);
					
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
				
				//send signal to Data_send_thread to send data wirelessly
				osSignalSet(Data_send_tid, DATA_READY);
			}
		}
		
		if(mode_select == 'B')
			osSignalSet(LCD_control_tid, LCD_READY);
		
		last_key = key;
		osDelay(50);	
	}
}

// LCD thread handler
void LCD_control_thread(void const * arg){
	LCD_inital_screen();
	
	char last_mode = 'D';
	int counter;
	
	// Packet data buffer, the first byte is a synchronizing byte
	uint8_t data[4];
	data[0] = NON_MEMS_MODE;
	
	while(1){
		osSignalWait(LCD_READY, osWaitForever);

////////////////////////////////////////////////////////////////////////////////////////////////
//Predefined shape mode		
		if(mode_select == 'A'){
			
			/* Clear the LCD */ 
			if(last_mode != 'A')
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
			LCD_SetTextColor(LCD_COLOR_RED);
			for(counter = 0; counter < lcd_memory_count; counter++){
				LCD_DrawUniLine(lcd_draw_memory[counter].x1, 280 - lcd_draw_memory[counter].y1, lcd_draw_memory[counter].x2, 280 - lcd_draw_memory[counter].y2);
			}
			LCD_SetTextColor(LCD_COLOR_RED);
			LCD_DrawUniLine(x1, 280 - y1, x2, 280 - y2);
			osDelay(20);
			LCD_SetTextColor(LCD_COLOR_WHITE);
			LCD_DrawUniLine(x1, 280 - y1, x2, 280 - y2);
			
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

// Wireless transimission thread handler
void Data_send_thread(void const * arg){
	
	// Packet data buffer, the first byte is a synchronizing byte
	uint8_t data[4];
	data[0] = NON_MEMS_MODE;
	uint8_t digit_location = 0;
	
	uint8_t xCoord[3][6] = {{25,65,65,25,25,0},
							{15,75,75,15,15,0},
							{25,45,65,25,0,0}};
	
	uint8_t yCoord[3][6] = {{5,5,45,45,5,0},
							{5,5,45,45,5,0},
							{5,45,5,5,0,0}};
	
	uint8_t zCoord[3][6] = {{0,1,1,1,1,0},
							{0,1,1,1,1,0},
							{0,1,1,1,0,0}};
	
	uint8_t xDigitCoord[10][8] = {	{2,13,13,2,2,0,0,0},
									{8,8,6,0,0,0,0,0},
									{2,13,2,13,0,0,0,0},
									{2,13,13,2,13,3,0,0},
									{2,2,13,13,13,0,0,0},
									{2,13,13,2,2,13,0,0},
									{13,2,2,13,13,2,0,0},
									{2,13,2,0,0,0,0,0},
									{2,13,13,2,2,2,13,0},
									{2,13,13,2,2,13,0,0}};
	
	uint8_t yDigitCoord[10][8] = {	{5,5,45,45,5,0,0,0},
									{5,45,40,0,0,0,0,0},
									{45,45,5,5,0,0,0,0},
									{5,5,45,45,23,23,0,0},
									{45,23,23,45,5,0,0,0},
									{5,5,23,23,45,45,0,0},
									{45,45,5,5,23,23,0,0},
									{5,45,45,0,0,0,0,0},
									{5,5,45,45,5,23,23,0},
									{5,5,45,45,23,23,0,0}};
								
	uint8_t zDigitCoord[10][8] = {	{0,1,1,1,1,0,0,0},
									{0,1,1,0,0,0,0,0},
									{0,1,1,1,0,0,0,0},
									{0,1,1,1,0,1,0,0},
									{0,1,1,0,1,0,0,0},
									{0,1,1,1,1,1,0,0},
									{0,1,1,1,1,1,0,0},
									{0,1,1,0,0,0,0,0},
									{0,1,1,1,1,0,1,0},
									{0,1,1,1,1,1,0,0}};
		
	wireless_init();

	while(1){
		osSignalWait(DATA_READY, osWaitForever);

		//send data wirelessly here
		//shape select, if it is 0, it means we are in on the fly mode
		//if it is 1,2 or 3, draw one of the predefined shapes
		if(shape_select){
			
			for (int i = 0; i < 6; i++) {
				
				data[1] = xCoord[shape_select - 1][i];
				data[2] = yCoord[shape_select - 1][i];
				data[3] = zCoord[shape_select - 1][i];
				
				if (xCoord[shape_select - 1][i] == 0 && yCoord[shape_select - 1][i] == 0 && zCoord[shape_select - 1][i] == 0)
					break;
				
				send_data(data);
				delay(60);
			}
			
			delay(150);
			//erase "please wait" message
			LCD_shape_clear();
			osSignalSet(LCD_control_tid, 0x01);
			
		// Crazy mode selected
		}else if(mode_select == 'B'){
			if(MEMS_activate){
				
				data[0] = MEMS_MODE;
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
				
				data[0] = NON_MEMS_MODE;
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
		
		// Digit printing mode selected
		if(mode_select == 'C'){
			
			for (int i = 0; i < 8; i++) {
				
				data[1] = xDigitCoord[key_C - '0'][i] + digit_location*15;
				data[2] = yDigitCoord[key_C - '0'][i];
				data[3] = zDigitCoord[key_C - '0'][i];
			
				send_data(data);
				
				if (xDigitCoord[key_C - '0'][i] == 0 && yDigitCoord[key_C - '0'][i] == 0 && zDigitCoord[key_C - '0'][i] == 0)
					break;
				
				delay(60);
			}
			digit_location++;
		}
	}
}

// create all the necessary threads
void create_threads(void){	
	keypad_tid = osThreadCreate(osThread(keypad_thread), NULL);
	LCD_control_tid = osThreadCreate(osThread(LCD_control_thread), NULL);
	Data_send_tid = osThreadCreate(osThread(Data_send_thread), NULL);
}
