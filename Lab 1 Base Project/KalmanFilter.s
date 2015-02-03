	AREA KalmanFilter, CODE, READONLY
	EXPORT kalmanFilter_asm
	
kalmanFilter_asm
	; load the initial Kalman state
	VLDR.F32 S1, [R3]		; q
	VLDR.F32 S2, [R3, #4]	; r
	VLDR.F32 S3, [R3, #8]   ; x
	VLDR.F32 S4, [R3, #12]	; p
	VLDR.F32 S5, [R3, #16] 	; k

update
	; do the Kalman filter updates
    CMP 	 R2, #0	        ; Loop through all the array 
    BEQ 	 exit           ; If so, leave the loop, otherwise continue 
	VLDR.F32 S6, [R0], #4	; measurement
	VADD.F32 S4, S4, S1		; p = p + q
	VADD.F32 S7, S4, S2		; temp = p + r
	VDIV.F32 S5, S4, S7		; k = p / temp = p / (p + r)	
	VSUB.F32 S7, S6, S3		; temp = measurment - x
	VMLA.F32 S3, S5, S7		; x = x + k * temp = x + k * (measurment - x)
	VMLS.F32 S4, S4, S5		; p = (1 - k) * p = p - p * k
	VSTR.F32 S3, [R1], #4	; store x in the filtered (output) array
    SUB 	 R2, R2, #1     ; R2 = R2 - 1
    B 		 update     	; Go to the beginning of the loop 

exit
	VMRS	 R1, FPSCR		; copy FPSCR to APSR
	AND 	 R0, R1, #159	; apply mask to status register to extract the floating-point exceptions. R0 = 0 if no exception.
	
	; store the final Kalman state
	VSTR.F32 S1, [R3]		; store q
	VSTR.F32 S2, [R3, #4]	; store r
	VSTR.F32 S3, [R3, #8]	; store x
	VSTR.F32 S4, [R3, #12]	; store p
	VSTR.F32 S5, [R3, #16]  ; store k
	
	BX 		 LR
	END