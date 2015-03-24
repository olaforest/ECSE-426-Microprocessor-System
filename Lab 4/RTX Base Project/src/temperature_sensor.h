/*
ECSE 426 - Lab 4
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Group #3
Date:	March 23, 2015
*/ 

#ifndef _TEMP_SENSOR_
#define _TEMP_SENSOR_

#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "kalman_filter.h"

// function prototypes
void config_TIM2(void);
void LED_GPIO_init(void);
void ADC_init(void);
void temp_sensor_init(void);
int getADCVoltage(void);
float volt_to_celsius(uint_fast16_t voltage);

// various constant and parameters
#define V_25	  			0.76
#define AVG_SLOPE 			0.0025
#define V_MAX     			3
#define PULSE_WIDTH_PERIOD 	180
#define ALARM_THRESHOLD		45
#define ALL_LED_PINS		GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15

// Definition for rates (in Hz), prescalar and period for the TIM2 clock.
#define TIM2_COUNTER_CLK 1500
#define TIM2_PRESCALAR ((SystemCoreClock / 2) / TIM2_COUNTER_CLK) - 1
#define TIM2_DESIRED_RATE 50
#define TIM2_PERIOD (TIM2_COUNTER_CLK / TIM2_DESIRED_RATE) - 1
#endif
