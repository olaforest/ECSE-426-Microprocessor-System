/*
ECSE 426 - Wireless 3D printing machine project
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Nuri Ege Kozan    (ID: 260359680)
Genevieve Nantel  (ID: 260481768)
Group #4
Date:	April 14th, 2015
*/

#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f4xx.h"

// define lengths that were measured on the mechanical 3d parts
#define D 33.3375
#define L1 65.0875
#define L2 71

#define X_OFFSET -35.0
#define Y_OFFSET 65.0

#define PI 3.14159265358979323846
#define DELAY    1
#define NO_DELAY 0

/*
 * TIM3 counter clock frequency = 315000
 * Prescaler = (TIM3CLK / TIM3 counter clock frequency) - 1
	* TIM3 counter clock frequency / TIM_period = 315000 / 6300 = 50Hz
	* Define prescaler and period which were adjusted by trial and error with motors
 */
#define PERIOD		1000
#define PRESCALER	840


/* Exported functions --------------------------------------------------------*/
void motor_gpio_init(void);
void timer_init(void);
void pwm_angle_init(void);
void pwm_set_duty(int channel, double dutycycle);
void convert_to_angle(double *angle, double x, double y);
void move_arms(uint8_t * data, uint8_t toDelay);

#endif
