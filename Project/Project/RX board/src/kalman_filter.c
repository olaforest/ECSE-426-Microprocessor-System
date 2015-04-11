/*
ECSE 426 - Lab 4
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Group #3
Date:	March 23, 2015
*/ 

#include "kalman_filter.h"

// Kalman filter to filter an input. Based on the Kalman filter that was used in Labs 1 and 2.
float kalmanFilter(float input, KalmanState* kstate){
	float filtered;
	kstate->p = kstate->p + kstate->q;
	kstate->k = kstate->p / (kstate->p + kstate->r);
	filtered = kstate->x = kstate->x + kstate->k * (input - kstate->x);
	kstate->p = (1 - kstate->k) * kstate->p;

	return filtered;
}
