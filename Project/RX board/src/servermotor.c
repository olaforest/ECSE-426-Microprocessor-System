/*
ECSE 426 - Wireless 3D printing machine project
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Nuri Ege Kozan    (ID: 260359680)
Genevieve Nantel  (ID: 260481768)
Group #4
Date:	April 14th, 2015
*/ 

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "servermotor.h"
#include <math.h>

static void delay(__IO uint32_t nCount){
	__IO uint32_t index = 0; 
	for(index = 100000*nCount; index != 0; index--){}
}

// initialize output compare for pwm
TIM_OCInitTypeDef  TIM_OCInitStructure;

// initialize motor gpio with specific pwm tim3 pins
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
	
	TIM_TimeBaseInitTypeDef TIM_timebase_init;

	/* Time base configuration */
	TIM_timebase_init.TIM_Period = PERIOD;
	TIM_timebase_init.TIM_Prescaler = PRESCALER;
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
	// set pwm chanel 1 with pulse of 0
    TIM_OCStruct.TIM_Pulse = 0;
    TIM_OC1Init(TIM3, &TIM_OCStruct);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    // set pwm channel 2 with pulse 0
    TIM_OCStruct.TIM_Pulse = 0;
    TIM_OC2Init(TIM3, &TIM_OCStruct);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    // set pwm channel 3 with pulse of 0
    TIM_OCStruct.TIM_Pulse = 0;
    TIM_OC3Init(TIM3, &TIM_OCStruct);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
   
}

// for each channel we will set the pulse according to the angles. This is our dutycycle
// we tested that the motor pulse goes from 0 to 250 approximatly and calibrated it accordingly
void pwm_set_duty(int channel, double angle){
	// for motor 1 set pulse with proper calibrated dutycycle
	if (channel == 1) {
		//TIM3->CCR1 = dutycycle;
		TIM_OCStruct.TIM_Pulse = 250 - angle *190/180;
		TIM_OC1Init(TIM3, &TIM_OCStruct);
	}// for motor 2 set pulse with proper calibrated dutycycle
	else if (channel == 2){
		//TIM3->CCR2 = dutycycle;
		TIM_OCStruct.TIM_Pulse = 244 - angle *205/185;
		TIM_OC2Init(TIM3, &TIM_OCStruct);
	}// for motor 3 set pulse with proper calibrated dutycycle
	else{
		//TIM3->CCR3 = dutycycle;
		TIM_OCStruct.TIM_Pulse = 150 + angle * 25;
		TIM_OC3Init(TIM3, &TIM_OCStruct);
	}
}
// convert the x and y values to an angle value for each motor
void convert_to_angle(double *angle, double x, double y){
	
	double l, h1, h2, gamma_1, gamma_2, alpha;
	
	// set the initial x and y position on the marker to be down left corner of the whiteboard
	y = y + Y_OFFSET;
	x = x + X_OFFSET;
	
	// calculate the length of l which is the length from 0 to the marker pivot
	l = sqrt(pow(x, 2) + pow(y, 2));
	
	// calculate gamma 1
	gamma_1 = acos(x / l);
	
	// cosinus law to calculate h1 and h2 
	h1 = sqrt(pow(D,2)/4 + pow(l,2) + D*x);
	h2 = sqrt(pow(D,2)/4 + pow(l,2) - D*x);
	
	// if motor is on the right side of origin and is further than the motor center then we set gamma two accordingly
	// if it is on the right side but within the the rang between center and motor center we adjust acordingly
	// same procedure for left side, those calculations were found using the sinus law
	if (x < D/2)
		gamma_2 = asin(y/h2);
	else
		gamma_2 = PI - asin(y/h2);
	
	if (x > -D/2)
		alpha = asin(y/h1);
	else
		alpha = PI - asin(y/h1);
	
	// using cosine law again we find the angle for each motor
	*angle = acos((pow(L1,2) + pow(h1,2) - pow(L2,2))/(2*L1*h1)) + alpha;
	*(angle + 1) = PI - gamma_2 - acos((pow(L1,2) + pow(h2,2) - pow(L2,2))/(2*L1*h2));
	
	//convert the angle to degrees
	*angle *= 180/PI;
	*(angle + 1) *= 180/PI;
	
	//we set protection for the motor arms 
	if (*angle > 180)
		*angle = 180;
	else if (*angle < 25)
		*angle = 25;
	
	if (*(angle + 1) > 155)
		*(angle + 1) = 155;
	else if (*(angle + 1) < 0)
		*(angle + 1)= 0;
}
//we move the arms according to the x and y values received from the transmitter board
void move_arms(uint8_t * data, uint8_t toDelay){
			
	double angle[2];
	
	convert_to_angle(angle, data[0], data[1]);
	
	pwm_set_duty(3, data[2]);
	
	if (toDelay)
		delay(50);
	
	pwm_set_duty(1, *angle);
	pwm_set_duty(2, *(angle + 1));	
}

