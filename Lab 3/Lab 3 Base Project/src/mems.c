#include <stdio.h>
#include "mems.h"

// configure the MEMS sensor
void config_LIS3DSH(void){
	
	LIS3DSH_InitTypeDef lis3dsh_initStruct;
	
	lis3dsh_initStruct.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_100;
	lis3dsh_initStruct.Axes_Enable				  			= LIS3DSH_X_ENABLE | LIS3DSH_Y_ENABLE | LIS3DSH_Z_ENABLE;
	lis3dsh_initStruct.Continous_Update			  		= LIS3DSH_ContinousUpdate_Disabled;
	lis3dsh_initStruct.AA_Filter_BW				  			= LIS3DSH_AA_BW_800;
	lis3dsh_initStruct.Full_Scale				  				= LIS3DSH_FULLSCALE_2;
	lis3dsh_initStruct.Self_Test			      			= LIS3DSH_SELFTEST_NORMAL;
	
	LIS3DSH_Init(&lis3dsh_initStruct);
		
	LIS3DSH_DRYInterruptConfigTypeDef lis3dsh_interruptConfigStruct;
	
	lis3dsh_interruptConfigStruct.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;
	lis3dsh_interruptConfigStruct.Interrupt_signal	  = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;
	lis3dsh_interruptConfigStruct.Interrupt_type	  	= LIS3DSH_INTERRUPT_REQUEST_PULSED;
	
	LIS3DSH_DataReadyInterruptConfig(&lis3dsh_interruptConfigStruct);
}

// Configure the external interupt (EXTI and NVIC)
void config_ext_interupt(void){
	
	//GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_initStruct;
	NVIC_InitTypeDef NVIC_initStruct;

    // Enable GPIOE clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	// Enable SYSCFG clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	// Configure pin PE0
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStructure.GPIO_Pin  = LIS3DSH_SPI_INT1_PIN;		// Pin 0
	//GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	// Connect EXTI Line0 to PE0 pin
	SYSCFG_EXTILineConfig(LIS3DSH_SPI_INT1_EXTI_PORT_SOURCE, LIS3DSH_SPI_INT1_EXTI_PIN_SOURCE);
	
	// Configure EXTI Line0
	EXTI_initStruct.EXTI_Line	 		= EXTI_Line0;
	EXTI_initStruct.EXTI_Mode	 		= EXTI_Mode_Interrupt;
	EXTI_initStruct.EXTI_Trigger	= EXTI_Trigger_Rising;
	EXTI_initStruct.EXTI_LineCmd	= ENABLE;	
	EXTI_Init(&EXTI_initStruct);
	
	// Enable and set EXTI Line0 Interrupt to the lowest priority
	NVIC_initStruct.NVIC_IRQChannel										= EXTI0_IRQn;
	NVIC_initStruct.NVIC_IRQChannelPreemptionPriority	= 0x00;
	NVIC_initStruct.NVIC_IRQChannelSubPriority				= 0x00;
	NVIC_initStruct.NVIC_IRQChannelCmd								= ENABLE;
	NVIC_Init(&NVIC_initStruct);
}

// Configuration of TIM3
void config_tim3(void){
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM3_initStruct;
	NVIC_InitTypeDef NVIC_initStruct;
	
	TIM3_initStruct.TIM_Prescaler					= 0xFFFF;
	TIM3_initStruct.TIM_CounterMode				= TIM_CounterMode_Up;
	TIM3_initStruct.TIM_Period						= 0x0008;
	TIM3_initStruct.TIM_ClockDivision			= TIM_CKD_DIV1;
	
	TIM_TimeBaseInit( TIM3, &TIM3_initStruct);
	
	// Enable and set TIM3 Interrupt to the lowest priority
	NVIC_initStruct.NVIC_IRQChannel										= TIM3_IRQn;
	NVIC_initStruct.NVIC_IRQChannelPreemptionPriority	= 0x00;
	NVIC_initStruct.NVIC_IRQChannelSubPriority				= 0x00;
	NVIC_initStruct.NVIC_IRQChannelCmd								= ENABLE;
	NVIC_Init(&NVIC_initStruct);
	
	TIM_UpdateRequestConfig(TIM3, TIM_UpdateSource_Regular);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

void config_segment_display(void){
	
	// Enable GPIOD clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_SEGMENT_PINS;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
}

void get_accelerations(float * norm_acc_X, float * norm_acc_Y, float * norm_acc_Z){
	uint8_t buffer[6];
	float acc_X, acc_Y, acc_Z;
	
	float factor = 10000.0;
	
	// this is a scaled calibration matrix (factor of 10000)
	float cm[4][3] = {    0.5880,  -0.0094,   0.0274,
						 -0.0145,   0.6154,   0.0335,
						  0.0016,  -0.0028,   0.6008,
					   -247.9509,  63.3592, 268.4256 };
	
	// reading the LSB/MSB of the x,y and z acceleration values.
	LIS3DSH_Read(&buffer[0], LIS3DSH_OUT_X_L, 1);
	LIS3DSH_Read(&buffer[1], LIS3DSH_OUT_X_H, 1);
	LIS3DSH_Read(&buffer[2], LIS3DSH_OUT_Y_L, 1);
	LIS3DSH_Read(&buffer[3], LIS3DSH_OUT_Y_H, 1);
	LIS3DSH_Read(&buffer[4], LIS3DSH_OUT_Z_L, 1);
	LIS3DSH_Read(&buffer[5], LIS3DSH_OUT_Z_H, 1);
	
	// concatenate the MSB and LSB readings into single 16-bit acceleration
	// readings (stored in a 32-bit float for further calculations)
	acc_X = (float) ((int16_t) (buffer[1] << 8 | buffer[0]));
	acc_Y = (float) ((int16_t) (buffer[3] << 8 | buffer[2]));
	acc_Z = (float) ((int16_t) (buffer[5] << 8 | buffer[4]));
	
	*norm_acc_X = (acc_X * cm[0][0] + acc_Y * cm[1][0] + acc_Z * cm[2][0] + cm[3][0]) / factor;
	*norm_acc_Y = (acc_X * cm[0][1] + acc_Y * cm[1][1] + acc_Z * cm[2][1] + cm[3][1]) / factor;
	*norm_acc_Z = (acc_X * cm[0][2] + acc_Y * cm[1][2] + acc_Z * cm[2][2] + cm[3][2]) / factor;
}

float get_pitch_angle(void){
	float norm_acc_X, norm_acc_Y, norm_acc_Z;
		
	// get the (normalized) acceleration values
	get_accelerations(&norm_acc_X, &norm_acc_Y, &norm_acc_Z);
	
	// caluculate and return the pitch angle.
	return atan(norm_acc_X / sqrt(norm_acc_Y * norm_acc_Y + norm_acc_Z * norm_acc_Z)) * 180 / PI + 90;	
}

// Kalman filter to filter an input. Based on the Kalman filter that was used in Labs 1 and 2.
float kalmanFilter(float input, KalmanState* kstate){
	float filtered;
	kstate->p = kstate->p + kstate->q;
	kstate->k = kstate->p / (kstate->p + kstate->r);
	filtered = kstate->x = kstate->x + kstate->k * (input - kstate->x);
	kstate->p = (1 - kstate->k) * kstate->p;

	return filtered;
}

void display_current_pitch(float pitch, int count){
	
	uint16_t digit_pins[10] = {DISPLAY_0, DISPLAY_1, DISPLAY_2, DISPLAY_3, DISPLAY_4, DISPLAY_5, DISPLAY_6, DISPLAY_7, DISPLAY_8, DISPLAY_9};
	uint16_t digit_select[3] = {DIGIT1_ON, DIGIT2_ON, DIGIT3_ON};
	
	GPIO_ResetBits(GPIOD, GPIO_SEGMENT_PINS);
		
		int display_digit, decimal_point_location;
		
		if((int)(pitch)/100 != 0){
			display_digit = (int) pitch;
			decimal_point_location = 2;
		}
		else if((int)(pitch)/10 != 0){
			display_digit = (int) (pitch * 10);
			decimal_point_location = 1;
		}
		else{
			display_digit = (int) (pitch * 100);
			decimal_point_location = 0;
		}
		
		switch(count % 3){
			case 0:
				display_digit = display_digit/100;
			
				if(decimal_point_location == 0)
					GPIO_SetBits(GPIOD, SEGMENT_PIN_DP);
				
				printf("Pitch angle: %.2f\t", pitch);
				printf("%d", display_digit);
				break;
			case 1:
				display_digit = (display_digit % 100)/10;
			
				if(decimal_point_location == 1)
					GPIO_SetBits(GPIOD, SEGMENT_PIN_DP);
			
				printf("%d", display_digit);
				break;
			case 2:
				display_digit = (display_digit % 10)/1;
				printf("%d\n", display_digit);
				break;
		}
		
		GPIO_SetBits(GPIOD, digit_select[count % 3]);
		GPIO_SetBits(GPIOD, digit_pins[display_digit]);
}
