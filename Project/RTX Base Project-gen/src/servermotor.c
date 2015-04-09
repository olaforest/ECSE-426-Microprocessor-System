#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "servermotor.h"
#include <math.h>

TIM_OCInitTypeDef  TIM_OCInitStructure;

void motor_gpio_init(void) {
	GPIO_InitTypeDef gpio_init_s;
	// GPIOC and GPIOB clock enabled
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB, ENABLE);
	
	 /* GPIOC Configuration: TIM3 CH1 (PC6) and TIM3 CH2 (PC7) */
	gpio_init_s.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;
	gpio_init_s.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_s.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init_s.GPIO_OType = GPIO_OType_PP;
	gpio_init_s.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOC, &gpio_init_s); 

	/* GPIOB Configuration:  TIM3 CH3 (PB0) */
	gpio_init_s.GPIO_Pin = GPIO_Pin_0;
	gpio_init_s.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_s.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init_s.GPIO_OType = GPIO_OType_PP;
	gpio_init_s.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOB, &gpio_init_s);
	
	/* Connect TIM3 pins to AF2 */ 
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3); 
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
}

void timer_init(void)
{
	//enable tim 3 clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	uint16_t prescaler, period;
	TIM_TimeBaseInitTypeDef TIM_timebase_init;
	
	/*
	 * TIM3 counter clock frequency = 315000
	 * Prescaler = (TIM3CLK / TIM3 counter clock frequency) - 1
     * with TIM_period = 2100, the PWM output frequency is
     * TIM3 counter clock frequency / TIM_period = 315000 / 6300 = 50Hz
	 */
	prescaler = 840;
    period = 1000;

    /* Time base configuration */
    TIM_timebase_init.TIM_Period = period;
    TIM_timebase_init.TIM_Prescaler = prescaler;
    /* The ration between clock frequency and the sampleing frequency, not used */
    TIM_timebase_init.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_timebase_init.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_timebase_init);
	TIM_Cmd(TIM3, ENABLE);
}


TIM_OCInitTypeDef TIM_OCStruct;

void pwm_angle_init(void){
	
    /* Common settings */
    
    /* PWM mode 2 = Clear on compare match */
    /* PWM mode 1 = Set on compare match */
    TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
    
    //pulse_length = ((TIM_Period + 1) * dutycycle) / 100 - 1
    
 /* where DutyCycle is in percent, between 0 and 100%
    25% duty cycle:     pulse_length = ((8399 + 1) * 25) / 100 - 1 = 2099
    50% duty cycle:     pulse_length = ((8399 + 1) * 50) / 100 - 1 = 4199
    75% duty cycle:     pulse_length = ((8399 + 1) * 75) / 100 - 1 = 6299
    100% duty cycle:    pulse_length = ((8399 + 1) * 100) / 100 - 1 = 8399
*/
    TIM_OCStruct.TIM_Pulse = 0;
    TIM_OC1Init(TIM3, &TIM_OCStruct);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    
    TIM_OCStruct.TIM_Pulse = 0;
    TIM_OC2Init(TIM3, &TIM_OCStruct);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    
    TIM_OCStruct.TIM_Pulse = 0;
    TIM_OC3Init(TIM3, &TIM_OCStruct);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
   
}

void pwm_set_duty(int channel, double angle){

	if (channel == 1) {
		//TIM3->CCR1 = dutycycle;
		TIM_OCStruct.TIM_Pulse = 250 - angle *190/180;
		TIM_OC1Init(TIM3, &TIM_OCStruct);
	}
	else if (channel == 2){
		//TIM3->CCR2 = dutycycle;
		TIM_OCStruct.TIM_Pulse = 244 - angle *205/185;
		TIM_OC2Init(TIM3, &TIM_OCStruct);
	}
	else{
		//TIM3->CCR3 = dutycycle;
		TIM_OCStruct.TIM_Pulse = 0;
		TIM_OC3Init(TIM3, &TIM_OCStruct);
	}
}

void convert_to_angle(double *angle, double x, double y){
	
	double l, h1, h2, gamma_1, gamma_2, alpha;
	
	l = sqrt(pow(x, 2) + pow(y, 2));
	
	gamma_1 = acos(x / l);
	
	h1 = sqrt(pow(D,2)/4 + pow(l,2) + D*x);
	h2 = sqrt(pow(D,2)/4 + pow(l,2) - D*x);
	
	if (x < D/2)
		gamma_2 = asin(y/h2);
	else
		gamma_2 = PI - asin(y/h2);
	
	if (x > -D/2)
		alpha = asin(y/h1);
	else
		alpha = PI - asin(y/h1);
	
	*angle = acos((pow(L1,2) + pow(h1,2) - pow(L2,2))/(2*L1*h1)) + alpha;
	*(angle + 1) = PI - gamma_2 - acos((pow(L1,2) + pow(h2,2) - pow(L2,2))/(2*L1*h2));
	
//	double angle1 = *angle * 180/PI;
//	double angle2 = *(angle + 1) * 180/PI;
	
	*angle *= 180/PI;
	*(angle + 1) *= 180/PI;
	
	if (*angle > 180)
		*angle = 180;
	else if (*angle < 25)
		*angle = 25;
	
	if (*(angle + 1) > 155)
		*(angle + 1) = 155;
	else if (*(angle + 1) < 0)
		*(angle + 1)= 0;
}


