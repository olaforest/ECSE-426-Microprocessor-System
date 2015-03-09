#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "lis3dsh.h"

static volatile uint_fast16_t measRdy;

void config_LIS3DSH(void){
	
	LIS3DSH_InitTypeDef lis3dsh_initStruct;
	
	lis3dsh_initStruct.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_100;
	lis3dsh_initStruct.Axes_Enable								= LIS3DSH_X_ENABLE | LIS3DSH_Y_ENABLE | LIS3DSH_Z_ENABLE;
	lis3dsh_initStruct.Continous_Update						= LIS3DSH_ContinousUpdate_Disabled;
	lis3dsh_initStruct.AA_Filter_BW								= LIS3DSH_AA_BW_800;
	lis3dsh_initStruct.Full_Scale									= LIS3DSH_FULLSCALE_16;
	lis3dsh_initStruct.Self_Test									= LIS3DSH_SELFTEST_NORMAL;
	
	LIS3DSH_Init(&lis3dsh_initStruct);
	
	
	LIS3DSH_DRYInterruptConfigTypeDef lis3dsh_interruptConfigStruct;
	
	lis3dsh_interruptConfigStruct.Dataready_Interrupt	= LIS3DSH_DATA_READY_INTERRUPT_ENABLED;
	lis3dsh_interruptConfigStruct.Interrupt_signal		= LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;
	lis3dsh_interruptConfigStruct.Interrupt_type			= LIS3DSH_INTERRUPT_REQUEST_PULSED;
	
	LIS3DSH_DataReadyInterruptConfig(&lis3dsh_interruptConfigStruct);
}

void config_PE0(void){
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	SYSCFG_EXTILineConfig(LIS3DSH_SPI_INT1_EXTI_PORT_SOURCE, LIS3DSH_SPI_INT1_EXTI_PIN_SOURCE);
	
	EXTI_InitTypeDef exti_initStruct;
	
	exti_initStruct.EXTI_Line			= EXTI_Line0;
	exti_initStruct.EXTI_Mode			= EXTI_Mode_Interrupt;
	exti_initStruct.EXTI_Trigger	= EXTI_Trigger_Rising;
	exti_initStruct.EXTI_LineCmd	= ENABLE;
	
	EXTI_Init(&exti_initStruct);
	
	
	
	
	NVIC_InitTypeDef nvic_initStruct;
	
	nvic_initStruct.NVIC_IRQChannel										= EXTI0_IRQn;
	nvic_initStruct.NVIC_IRQChannelPreemptionPriority	= 0x00;
	nvic_initStruct.NVIC_IRQChannelSubPriority				= 0x00;
	nvic_initStruct.NVIC_IRQChannelCmd								= ENABLE;
	
	NVIC_Init(&nvic_initStruct);
}

void EXTI0_IRQHandler(){
	measRdy = 1;
	EXTI_ClearITPendingBit(EXTI_Line0);
}

int main(){
	
	config_LIS3DSH();
	
	config_PE0();
	
	while(1){
		
		while(!measRdy);
		measRdy = 0;
		
	}
	
	return 0;
}


