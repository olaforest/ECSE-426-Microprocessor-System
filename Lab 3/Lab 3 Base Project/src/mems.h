#ifndef _MEMS_
#define _MEMS_

#include "math.h"
#include "lis3dsh.h"
#include "kalman_state.h"

#define PI 3.141592653589793

void config_LIS3DSH(void);
void config_ext_interupt(void);
void get_accelerations(float * norm_acc_X, float * norm_acc_Y, float * norm_acc_Z);
float get_pitch_angle(void);
float kalmanFilter(float input, KalmanState* kstate);

#endif
