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
	
	EXTI_InitTypeDef EXTI_initStruct;
	NVIC_InitTypeDef NVIC_initStruct;
	//GPIO_InitTypeDef GPIO_InitStructure;

  // Enable GPIOE clock
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		
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



void get_accelerations(float * norm_acc_X, float * norm_acc_Y, float * norm_acc_Z){
	uint8_t buffer[6];
	float acc_X, acc_Y, acc_Z;
	
	float factor = 10000.0;
	
	// this is a scaled calibration matrix (factor of 10000)
	float cm[4][3] = {	 0.5880,  -0.0094,   0.0274,
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
	
	// calculate and return the pitch angle.
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
