/*
ECSE 426 - Lab 4
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Group #3
Date:	March 23, 2015
*/ 

#include "threads.h"

// display thread handler
void segment_display(void const * arg){
	uint16_t LED_pins[4] = { GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15 };
	int count = 0;
	char key = DUMMY_KEY, led = '1';
	int mode = TEMP_MODE;
	float temperature = 50.0;
	float pitch = 0.0;
		
	display_init();
	
	float data; 
	
	while(1){
		osSignalWait(DISPLAY_READY, osWaitForever);
				
		// try to get the keypad message		
		if (receive_message(&data, keypad_queue)){
			key = (char)(((int)data) + '0');
			
			// figure out which type of key was pressed (i.e. mode or led) and adjust variables accordingly. 
			if (key == TEMP_MODE_KEY || key == MEMS_MODE_KEY){
				mode = key == TEMP_MODE_KEY ? TEMP_MODE : MEMS_MODE;
			} else {
				led = key;
			}			
		}
		
		// try to get the pitch message
		if (receive_message(&data, pitch_queue)){
			pitch = data;
		}
		
		// try to get the temperature message
		if (receive_message(&data, temp_queue)){
			temperature = data;
		}
				
		GPIO_ResetBits(GPIOD, GPIO_SEGMENT_PINS);
		GPIO_ResetBits(GPIOE, GPIO_DIGIT_SELECT_PINS);
	
		// run the display effect depending on whether or not an alarm is triggerd and the type of mode.
		if (temperature < ALARM_THRESHOLD || (count % (2 * TIM3_DESIRED_RATE)) < TIM3_DESIRED_RATE) {		
			if (mode == TEMP_MODE){
				GPIO_ResetBits(GPIOD, ALL_LED_PINS);
				display_value(temperature, count, TEMP_MODE); 
			} else {
				display_value(pitch, count, MEMS_MODE);
			}
		}
		
		if (mode == MEMS_MODE){
			display_LED(pitch, count, LED_pins[(led - '1')]);
		}
		
		count++;
	}
}

// keypad thread handler
void keypad(void const * arg){
	int count = 0;
	char key;
	
	keypad_init();
	
	while(1){
		osSignalWait(KEYPAD_READY, osWaitForever);
		if ((count++ % KEY_SCAN_CLK_DIV) == 0){
			key = get_input();
			
			// send a message to the display thread
			if (key != DUMMY_KEY){		
				send_message((float)(key - '0'), keypad_queue);
			}
		}
	}
}

// MEMS thread handler
void mems(void const * arg){
	// Kalman filter state initilization
	KalmanState kstate;
	kstate.p = 0.0;
	kstate.k = 0.0;
	kstate.r = 50;
	kstate.q = 0.5;
	kstate.x = 0.0;
	
	float pitch = 0.0;
	
	mems_init();
	
	while(1){
		osSignalWait(MEMS_READY, osWaitForever);
	
		pitch = kalmanFilter(get_pitch_angle(), &kstate);
		
		// send a message to the display thread
		send_message(pitch, pitch_queue);
	}
}

// temperature thread handler
void temp_sensor(void const * arg){

	// set the initial Kalman filter state.
	KalmanState kstate;
	kstate.p = 0.1;
	kstate.k = 0.0;
	kstate.r = 2.25;
	kstate.q = 0.01;
	kstate.x = 0.0;
	
	float current_temperature = 0.0;
	
	temp_sensor_init();
	
	while(1){
		osSignalWait(TEMP_SENSOR_READY, osWaitForever);
		
		// sample the current temperature of the processor.
		current_temperature = kalmanFilter(volt_to_celsius(getADCVoltage()), &kstate);
		
		// send a message to the display thread
		send_message(current_temperature, temp_queue);	
	}
}

// Send the 'data' in the Message 'queue' 
void send_message(float data, osMessageQId queue){
	Message* msg = osPoolAlloc(mem_pool);
	msg->data = data;
	osMessagePut(queue, (uint32_t)msg, 0);	
}

// Receive the 'data' in the Message 'queue'. Returns 1 if a message was received, 0 otherwise.
int receive_message(float * data, osMessageQId queue){
	osEvent evt = osMessageGet(queue, 0); 
	if (evt.status == osEventMessage) {
		Message* msg = ((Message *)evt.value.p);
		*data = msg->data;			
		osPoolFree(mem_pool, msg);
		
		return 1;
	}
	
	return 0;
}

// creates the memory pool and the message queues
void create_mem_pool_and_msg_queues(void){
	mem_pool = osPoolCreate(osPool(mem_pool)); 					  
	pitch_queue = osMessageCreate(osMessageQ(pitch_queue), NULL); 
	temp_queue = osMessageCreate(osMessageQ(temp_queue), NULL);
	keypad_queue = osMessageCreate(osMessageQ(keypad_queue), NULL);
}

// creates the threads
void create_threads(void){
	mems_thread = osThreadCreate(osThread(mems), NULL);
	temp_sensor_thread = osThreadCreate(osThread(temp_sensor), NULL);
	display_thread = osThreadCreate(osThread(segment_display), NULL);
	keypad_thread = osThreadCreate(osThread(keypad), NULL);
}
