#include "stm32f4xx.h"
//#include "cmsis_os.h"
#include "servermotor.h"
#include <stdio.h>

int dutycycle = 0;
int max_value = 20;
int angle = 30;
int motor;
/*!
 @brief Program entry point
 */
int main (void) {
	
	motor_gpio_init();
	timer_init();
	pwm_angle_init();
	
	
	double angle[2];
	
	convert_to_angle(angle, 100, 55.0);
	
//	double angle1 = *angle;
//	double angle2 = *(angle + 1);
//	
//	printf("Angle motor 1 %f\n", *angle);
//	printf("Angle motor 2 %f\n", *(angle + 1));
	
//	dutycycle	=  angle1 + 65;
	pwm_set_duty(1, *angle);
	pwm_set_duty(2, *(angle + 1));
	pwm_set_duty(3, 150);
	
/*	motor = 2;
  //initlilization all component
	motor_gpio_init();
	timer_init();
	pwm_angle_init();
	if (motor == 1) {
		dutycycle	=  angle + 65;
		pwm_set_duty(1, dutycycle);

	}else if (motor == 2) {
		dutycycle	=  angle + 55;
		pwm_set_duty(2, dutycycle);
		
	} else {
		dutycycle	=  angle + 65;
		pwm_set_duty(3, dutycycle);
		
	}
	
	*/
}

