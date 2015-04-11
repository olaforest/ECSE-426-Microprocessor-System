#include "threads.h"

uint8_t mems_mode = 0;

osThreadDef(rx_thread, osPriorityNormal, 1, 0);
osThreadDef(servomotor_thread, osPriorityNormal, 1, 0);
osThreadDef(mems_thread, osPriorityNormal, 1, 0);

// Define a memory pool
osPoolDef(mem_pool, 20, Message);	

// message queue definitions
osMessageQDef(data_queue, 10, Message); 	
osMessageQDef(mems_queue, 10, Message); 

// Handler routine for the TIM3 interrupts
void TIM3_IRQHandler(){
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET){
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		osSignalSet(rx_tid, RX_READY);
	}
}

// Handler routine for the EXTI0 interrupts (MEMS sensor)
void EXTI0_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line0) == SET) {
		EXTI_ClearITPendingBit(EXTI_Line0);
		osSignalSet(mems_tid, MEMS_READY);
	}
}

void mems_thread(void const * arg){
	// Kalman filter state initilization
	KalmanState kstate_pitch, kstate_roll;
	kstate_pitch.p = kstate_roll.p =0.0;
	kstate_pitch.k = kstate_roll.k =0.0;
	kstate_pitch.r = kstate_roll.r =50;
	kstate_pitch.q = kstate_roll.q =0.5;
	kstate_pitch.x = kstate_roll.x =0.0;
	
	float pitch = 0.0;
	float roll  = 0.0;
	
	mems_init();
	
	while(1){
		osSignalWait(MEMS_READY, osWaitForever);	
		
		if (mems_mode){	
			pitch = kalmanFilter(get_pitch_angle(), &kstate_pitch);
			roll  = kalmanFilter(get_roll_angle(), &kstate_roll);
			if (roll < 45.0) {
				roll = 45.0;
			} else if (roll > 135.0){
				roll = 135.0;
			}
				
			float x = 90 - pitch / 2;
			float y = 55.0 - (roll - 45.0) * (55.0 / 90.0);
			
			uint8_t position[4];
			position[0] = 255;
			position[1] = x;
			position[2] = y;
			position[3] = 1.0;
			send_message(position, mems_queue);
					
			osSignalSet(servomotor_tid, DATA_READY);
		}
	}
}

void servomotor_thread(void const * arg){	
	motor_gpio_init();
	timer_init();
	pwm_angle_init();
	
	uint8_t data[3];
	
	while (1){
		osSignalWait(DATA_READY, osWaitForever);
		
		if (receive_message(data, data_queue)){
			move_arms(data, DELAY);
		} else if (receive_message(data, mems_queue)){
			move_arms(data, NO_DELAY);
		} else {
			printf("No data was received\n");
		}
	}
}

void rx_thread(void const * arg){
	wireless_init();
	
	set_idle();
	flush_rx_fifo();
	rx_enable();

	while (1){		

		osSignalWait(RX_READY, osWaitForever);
			
		uint8_t buffer[64];
		memset(buffer, 0, 64);
		
		rx_enable();
		
		uint8_t buff_size;
		buff_size = get_rx_buffer_size();		
		if (buff_size > 3 && buff_size < 64){
			read_rx_fifo(buffer, buff_size);
						
			for (int i = 0; i < buff_size; i++){				
				printf("Count: %u\n", buffer[i]);
			}	
			
			if (buffer[0] == (uint8_t) MEMS_MODE) {
				mems_mode = 1;
			} else {
				mems_mode = 0;			
				send_message(buffer, data_queue);
				osSignalSet(servomotor_tid, DATA_READY);	
			}
		}
	}
}

// Send the 'data' in the Message 'queue' 
void send_message(uint8_t * data, osMessageQId queue){
	Message* msg = osPoolAlloc(mem_pool);
	msg->x = data[1];
	msg->y = data[2];
	msg->z = data[3];
	osMessagePut(queue, (uint32_t)msg, osWaitForever);	
}

// Receive the 'data' in the Message 'queue'. Returns 1 if a message was received, 0 otherwise.
int receive_message(uint8_t * data, osMessageQId queue){
	osEvent evt = osMessageGet(queue, 0); 
	if (evt.status == osEventMessage) {
		Message* msg = ((Message *)evt.value.p);
		data[0] = msg->x;
		data[1] = msg->y;
		data[2] = msg->z;
		osPoolFree(mem_pool, msg);
		
		return 1;
	}
	
	return 0;
}


void create_threads(void){
	mem_pool = osPoolCreate(osPool(mem_pool)); 
	data_queue = osMessageCreate(osMessageQ(data_queue), NULL); 	
	mems_queue = osMessageCreate(osMessageQ(mems_queue), NULL); 	
	
	rx_tid = osThreadCreate(osThread(rx_thread), NULL);
	servomotor_tid = osThreadCreate(osThread(servomotor_thread), NULL);
	mems_tid = osThreadCreate(osThread(mems_thread), NULL);
}
