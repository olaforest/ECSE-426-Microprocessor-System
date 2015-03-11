#ifndef _MEMS_
#define _MEMS_

#include "math.h"
#include "lis3dsh.h"
#include "kalman_state.h"

#define PI 3.141592653589793
#define GPIO_SEGMENT_PINS GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11

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


/** @defgroup DISPLAY_PINS_define_
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

/** @defgroup DIGIT_ON_define_
  * @{
  */ 
#define DIGIT1_ON GPIO_Pin_9
#define DIGIT2_ON GPIO_Pin_10
#define DIGIT3_ON GPIO_Pin_11



void config_LIS3DSH(void);
void config_ext_interupt(void);
void config_tim3(void);
void config_segment_display(void);
void get_accelerations(float * norm_acc_X, float * norm_acc_Y, float * norm_acc_Z);
float get_pitch_angle(void);
float kalmanFilter(float input, KalmanState* kstate);
void display_current_pitch(float pitch, int count);

#endif
