	AREA testbench, CODE, READONLY
	EXPORT kalman_filter_testbench
	IMPORT kalmanFilter_asm

	ALIGN 4
FilteredData SPACE 4 * 5
InputData DCFS 0x40d66666, 0x411ccccd, 0x40f91eb8, 0x4107fa1a, 0x40e00000 ; [6.7, 9.8, 7.785, 8.49856, 7.0]
KalmanState DCFS 0x3dcccccd, 0x3dcccccd, 0x0, 0x3dcccccd, 0x0 ; q = 0.1, r = 0.1, x = 0, p = 0.1, k =0

kalman_filter_testbench
	LDR R0, =InputData
	LDR R1, =FilteredData
	MOV R2, #2
	LDR R3, =KalmanState
	LDR R5, =kalmanFilter_asm
	BLX R5
	ADD R1, R1, R2
	
	END
	