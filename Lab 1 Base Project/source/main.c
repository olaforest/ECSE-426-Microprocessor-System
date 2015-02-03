#include <stm32f4xx.h>
#include <stdio.h>
#include <math.h>
#include "arm_math.h"
#include "kalman_state.h"

extern int kalmanFilter_asm(float* inputArray, float* outputArray, int length, KalmanState* kstate);

// Kalman filtering of the input found in the 'inputArray' containing 'length' measurements, with the initial
// Kalman state in 'kstate'. The output is written into 'outputArray'.
int kalmanFilter_C(float* inputArray, float* outputArray, KalmanState* kstate, int length){
	int i;
	for (i = 0; i < length; i++){
		kstate->p = kstate->p + kstate->q;
		kstate->k = kstate->p / (kstate->p + kstate->r);
		outputArray[i] = kstate->x = kstate->x + kstate->k * (inputArray[i] - kstate->x);
		kstate->p = (1 - kstate->k) * kstate->p;
	}
	
	return 0;
}

// calculation of the difference between the inputs to and the outputs of the Kalman filter
void result_diff(float* inputArray, float* outputArray, float* diff, int length){
	int i;
	for (i = 0; i < length; i++){
		diff[i] = inputArray[i] - outputArray[i];
	}
}

// calcualate the average and standard deviation of the difference between the inputs and outputs
void avg_and_std(float* data, float* avg, float* std, int length){
	*avg = 0.0;
	*std = 0.0;
	
	// average calculation
	int i;
	for (i = 0; i < length; i++){
		*avg += data[i];
	}
	*avg /= length;
	
	// standard deviation calculation
	for (int i = 0; i < length; i++){
		*std += pow(data[i] - *avg, 2);
	}
	*std = sqrt(*std / (length - 1));
}

// (Pearson) correlation between the input and output vectors.
float correlation(float* x, float* y, int length){
	float avg_x, avg_y, std_x, std_y, corr = 0.0;
	
	avg_and_std(x, &avg_x, &std_x, length);
	avg_and_std(y, &avg_y, &std_y, length);
	
	int i;
	for (i = 0; i < length; i++){
		corr += (x[i] - avg_x) * (y[i] - avg_y);
	}
	corr /= (length - 1) * std_x * std_y;
	
	return corr;
}	

// returns the result of the convolution of f and g in r
void convolution(float* f, float* g, float* r, int length){
	int i, j;
	for (i = 0; i < 2*length - 1; i++){
		r[i] = 0.0;
		for (j = 0; j < length; j++){
			int n = i - j;
			if (n < 0) {
				break;
			} else if (n < length) {
				r[i] += f[j] * g[n];
			}
		}
	}
}	

int main(){
	// setup problem inputs
	const int measurements = 5;
	float inputData[measurements] = { 6.7, 9.8, 7.785, 8.49856, 7.0 };
	float outputData[measurements];
	
	KalmanState state;
	state.q = state.r = state.p = 0.1;
	state.x = state.k = 0.0;
		
	// Do Kalman filtering
	int status_c = kalmanFilter_C(inputData, outputData, &state, measurements);
	//int status_asm = kalmanFilter_asm(inputData, outputData, measurements, &state);
		
	float avg, avg2, std, std2;
	float diff[measurements], diff2[measurements];
	float corr[2*measurements - 1], corr2[2*measurements - 1];;
	float conv[2*measurements - 1], conv2[2*measurements - 1];
	
	// Analysis using our own C implementation
	result_diff(inputData, outputData, diff, measurements);
	avg_and_std(diff, &avg, &std, measurements);	
	correlation(inputData, outputData, measurements);	
	convolution(inputData, outputData, conv, measurements);
	
	// Analysis using the CMSIS-DSP library	
	arm_sub_f32(inputData, outputData, diff2, measurements);
	arm_mean_f32(diff2, measurements, &avg2);
	arm_std_f32(diff2, measurements, &std2);
	arm_correlate_f32(inputData, measurements, outputData, measurements, corr2);
	arm_conv_f32(inputData, measurements, outputData, measurements, conv2);
	
	return 0;
}
