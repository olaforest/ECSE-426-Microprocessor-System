/*
ECSE 426 - Wireless 3D printing machine project
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Nuri Ege Kozan    (ID: 260359680)
Genevieve Nantel  (ID: 260481768)
Group #4
Date:	April 14th, 2015
*/ 

#ifndef _KALMAN_
#define _KALMAN_

// Kalman state structure
typedef struct {
	float q;
	float r;
	float x;
	float p;
	float k;
} KalmanState;

float kalmanFilter(float input, KalmanState* kstate);

#endif
