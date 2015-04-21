/*
ECSE 426 - Wireless 3D printing machine project
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Nuri Ege Kozan    (ID: 260359680)
Genevieve Nantel  (ID: 260481768)
Group #4
Date:	April 14th, 2015
*/ 

#include "kalman_filter.h"

// Kalman filter to filter an input. Based on the lab 1 description and used in all the subsequents labs. 
float kalmanFilter(float input, KalmanState* kstate){
	float filtered;
	kstate->p = kstate->p + kstate->q;
	kstate->k = kstate->p / (kstate->p + kstate->r);
	filtered = kstate->x = kstate->x + kstate->k * (input - kstate->x);
	kstate->p = (1 - kstate->k) * kstate->p;

	return filtered;
}
