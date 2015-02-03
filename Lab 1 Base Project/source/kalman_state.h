#ifndef KALMAN
#define KALMAN

// Kalman state structure
typedef struct {
	float q;
	float r;
	float x;
	float p;
	float k;
} KalmanState;

#endif
