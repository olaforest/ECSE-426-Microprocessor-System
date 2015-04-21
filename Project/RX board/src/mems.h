/*
ECSE 426 - Wireless 3D printing machine project
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Nuri Ege Kozan    (ID: 260359680)
Genevieve Nantel  (ID: 260481768)
Group #4
Date:	April 14th, 2015
*/ 

#ifndef _MEMS_
#define _MEMS_

#include <stdio.h>
#include "math.h"
#include "lis3dsh.h"
#include "kalman_filter.h"

#define M_PI 3.141592653589793
// exported functions
void config_LIS3DSH(void);
void config_ext_interrupt(void);
void get_accelerations(float * norm_acc_X, float * norm_acc_Y, float * norm_acc_Z);
float get_pitch_angle(void);
float get_roll_angle(void);
void mems_init(void);

#endif
