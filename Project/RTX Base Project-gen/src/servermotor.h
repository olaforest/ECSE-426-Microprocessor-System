#ifndef MOTOR_H
#define MOTOR_H

#define D 33.3375
#define L1 65.0875
#define L2 71

#define PI 3.14159265358979323846

#include "stm32f4xx.h"
void motor_gpio_init(void);
void timer_init(void);
void pwm_angle_init(void);
void pwm_set_duty(int channel, double dutycycle);
void convert_to_angle(double *angle, double x, double y);

#endif