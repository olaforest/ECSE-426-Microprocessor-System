#ifndef _SEGMENT_DISPLAY_
#define _SEGMENT_DISPLAY_

#include "stm32f4xx_conf.h"

#define GPIO_SEGMENT_PINS GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12


/* Exported constants --------------------------------------------------------*/

/** @defgroup SEGMENT_Exported_Constants
  * @{
  */ 
	
/** @defgroup SEGMENT_PINS_define_
  * @{
  */ 
#define SEGMENT_PIN_A GPIO_Pin_0
#define SEGMENT_PIN_B GPIO_Pin_1
#define SEGMENT_PIN_C GPIO_Pin_2
#define SEGMENT_PIN_D GPIO_Pin_3
#define SEGMENT_PIN_E GPIO_Pin_4
#define SEGMENT_PIN_F GPIO_Pin_6
#define SEGMENT_PIN_G GPIO_Pin_7
#define SEGMENT_PIN_DP GPIO_Pin_8
#define SEGMENT_PIN_DEGREE_SIGN GPIO_Pin_12


/** @defgroup DISPLAY_NUMBER_PINS_define_
  * @{
  */ 
#define DISPLAY_0 SEGMENT_PIN_A | SEGMENT_PIN_B | SEGMENT_PIN_C | SEGMENT_PIN_D | SEGMENT_PIN_E | SEGMENT_PIN_F
#define DISPLAY_1 SEGMENT_PIN_B | SEGMENT_PIN_C
#define DISPLAY_2 SEGMENT_PIN_A | SEGMENT_PIN_B | SEGMENT_PIN_G | SEGMENT_PIN_D | SEGMENT_PIN_E
#define DISPLAY_3 SEGMENT_PIN_A | SEGMENT_PIN_B | SEGMENT_PIN_C | SEGMENT_PIN_D | SEGMENT_PIN_G
#define DISPLAY_4 SEGMENT_PIN_B | SEGMENT_PIN_C | SEGMENT_PIN_G | SEGMENT_PIN_F
#define DISPLAY_5 SEGMENT_PIN_A | SEGMENT_PIN_C | SEGMENT_PIN_D | SEGMENT_PIN_G | SEGMENT_PIN_F
#define DISPLAY_6 SEGMENT_PIN_A | SEGMENT_PIN_C | SEGMENT_PIN_D | SEGMENT_PIN_E | SEGMENT_PIN_F | SEGMENT_PIN_G
#define DISPLAY_7 SEGMENT_PIN_A | SEGMENT_PIN_B | SEGMENT_PIN_C
#define DISPLAY_8 SEGMENT_PIN_A | SEGMENT_PIN_B | SEGMENT_PIN_C | SEGMENT_PIN_D | SEGMENT_PIN_E | SEGMENT_PIN_F | SEGMENT_PIN_G
#define DISPLAY_9 SEGMENT_PIN_A | SEGMENT_PIN_B | SEGMENT_PIN_C | SEGMENT_PIN_D | SEGMENT_PIN_F | SEGMENT_PIN_G


/** @defgroup DISPLAY_LETTER_PINS_define_
  * @{
  */ 
#define DISPLAY_U SEGMENT_PIN_B | SEGMENT_PIN_C | SEGMENT_PIN_D | SEGMENT_PIN_E | SEGMENT_PIN_F
#define DISPLAY_P SEGMENT_PIN_A | SEGMENT_PIN_B | SEGMENT_PIN_E | SEGMENT_PIN_F | SEGMENT_PIN_G
#define DISPLAY_d SEGMENT_PIN_B | SEGMENT_PIN_C | SEGMENT_PIN_D | SEGMENT_PIN_E | SEGMENT_PIN_G
#define DISPLAY_n SEGMENT_PIN_C | SEGMENT_PIN_E | SEGMENT_PIN_G


/** @defgroup DIGIT_ON_define_
  * @{
  */ 
#define DIGIT1_ON GPIO_Pin_9
#define DIGIT2_ON GPIO_Pin_10
#define DIGIT3_ON GPIO_Pin_11

// Definition for rates (in Hz), prescalar and period for the TIM3 clock.
#define TIM3_COUNTER_CLK 1282
#define TIM3_PRESCALAR ((SystemCoreClock / 2) / TIM3_COUNTER_CLK) - 1
#define TIM3_DESIRED_RATE 160
#define TIM3_PERIOD (TIM3_COUNTER_CLK / TIM3_DESIRED_RATE) - 1

/* Exported functions --------------------------------------------------------*/

// Configuration of TIM3 timer and GPIOs used to operate the 7 segments display.
void config_tim3(void);
void config_segment_display(void);

// Function which properly format display output for the three possible mode of operation.
void display_current_pitch(float pitch, int count);
void display_anim_larger(int count);
void display_anim_smaller(int count);
void display_routine(float pitch, int target_angle, int count);

#endif
