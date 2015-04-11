/*
ECSE 426 - Lab 4
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Group #3
Date:	March 23, 2015
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
