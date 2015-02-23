/*
ECSE 426 - Lab 2
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Group #3
Date:	February 20th, 2015
*/ 

#ifndef _TEMP_SENSOR_
#define _TEMP_SENSOR_

#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "kalman_state.h"

// function prototypes
void GPIO_init(void);
void ADC_init(void);
void init(void);
int getADCVoltage(void);
float volt_to_celsius(uint_fast16_t voltage);
void alarm_operation(int * ticks_count, int * increasing, int * duty_cycle);
void normal_operation(int temperature, uint16_t LED_pins[]);
float kalmanFilter(float input, KalmanState* kstate);

// various constant and parameters
#define V_25	  			0.76
#define AVG_SLOPE 			0.0025
#define V_MAX     			3
#define PULSE_WIDTH_PERIOD 	4
#define ALARM_THRESHOLD		46
#define ALL_LED_PINS		GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15

#endif
