/*
ECSE 426 - Lab 3
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Group #3
Date:	March 16, 2015
*/ 

#ifndef _SEGMENT_DISPLAY_
#define _SEGMENT_DISPLAY_

#include "stm32f4xx_conf.h"

#define TEMP_MODE 0
#define MEMS_MODE 1

#define GPIO_SEGMENT_PINS GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8// | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12
#define GPIO_DIGIT_SELECT_PINS GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8

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
//#define SEGMENT_PIN_DEGREE_SIGN GPIO_Pin_12


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
#define DISPLAY_C SEGMENT_PIN_A | SEGMENT_PIN_D | SEGMENT_PIN_E | SEGMENT_PIN_F
#define DISPLAY_U SEGMENT_PIN_B | SEGMENT_PIN_C | SEGMENT_PIN_D | SEGMENT_PIN_E | SEGMENT_PIN_F
#define DISPLAY_P SEGMENT_PIN_A | SEGMENT_PIN_B | SEGMENT_PIN_E | SEGMENT_PIN_F | SEGMENT_PIN_G
#define DISPLAY_d SEGMENT_PIN_B | SEGMENT_PIN_C | SEGMENT_PIN_D | SEGMENT_PIN_E | SEGMENT_PIN_G
#define DISPLAY_n SEGMENT_PIN_C | SEGMENT_PIN_E | SEGMENT_PIN_G

// Degree Sign pins
#define DISPLAY_DEGREE_SIGN SEGMENT_PIN_A | SEGMENT_PIN_B | SEGMENT_PIN_F | SEGMENT_PIN_G


/** @defgroup DIGIT_ON_define_
  * @{
  */ 
#define DIGIT1_ON GPIO_Pin_4
#define DIGIT2_ON GPIO_Pin_5
#define DIGIT3_ON GPIO_Pin_7
#define DIGIT4_ON GPIO_Pin_8

//// Definition for rates (in Hz), prescalar and period for the TIM3 clock.
#define TIM3_COUNTER_CLK 72000 //1282
#define TIM3_PRESCALAR ((SystemCoreClock / 2) / TIM3_COUNTER_CLK) - 1
#define TIM3_DESIRED_RATE 7200
#define TIM3_PERIOD (TIM3_COUNTER_CLK / TIM3_DESIRED_RATE) - 1

/* Exported functions --------------------------------------------------------*/

// Configuration of TIM3 timer and GPIOs used to operate the 7 segments display.
void config_tim3(void);
void config_segment_display(void);
void display_init(void);

// Function which properly format display output for the three possible modes of operation.
void display_value(float value, int count, int mode);
void display_LED(float pitch, int count, uint32_t LED_Pin);

#endif
