	#include "ARMsfpe.pre.asm"

	EXPORT _f_htof_alt


;======================================================================================

; FP16 format: ALTERNATIVE

; FUNCTION: _f_htof_alt

; DESCRIPTION: Convert half precision floating-point to single precision floating-point.

; INPUT:

;       fx = half precision fp
;	sreg01 = 1 to enable alternative format

; OUTPUT:

;       fx = single precision fp

;=====================================================================================

	AREA |.text|,CODE,READONLY
	
_f_htof_alt

	push	{sreg03-sreg04, lr}
	
	mov	sreg01, #0	;enable ALTERNATIVE fp16 format

	and	sreg02, fx, #32768

	lsl	sreg02, sreg02, #16

	ubfx	sreg03, fx, #10, #5

	movw	sreg04, #1023

	and 	sreg04, fx, sreg04

	cmp 	sreg03, #31

	bne	label_01

	cbz   	sreg01, label_01

	orr   	fx, sreg02, #2139095040

	orr   	fx, fx, sreg04, lsl #13

	pop   	{sreg03-sreg04, pc}

label_01

	cbnz  	sreg03, label_03

	cbnz  	sreg04, label_02

	mov   	fx, sreg02

	pop   	{sreg03-sreg04, pc}

label_02

	clz	fx, sreg04

	sub   	fx, #21

	lsls  	sreg04, fx

	neg  	sreg03, fx

label_03

	add   	sreg01, sreg03, #112

	lsl   	fx, sreg04, #13

	add   	fx, fx, sreg01, lsl #23

	orrs  	fx, sreg02

	pop   	{sreg03-sreg04, pc}
