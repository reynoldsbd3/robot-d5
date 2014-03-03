	#include "ARMsfpe.pre.asm"

	EXPORT _f_ftoh


;======================================================================================

; FP16 format: IEEE

; FUNCTION: _f_ftoh

; DESCRIPTION: Convert single precision floating-point to half precision floating-point.

; INPUT:

;       fx = single precision fp
;	sreg01 = 1 to enable ieee format

; OUTPUT:

;       fx = half precision fp

;=====================================================================================

	AREA |.text|,CODE,READONLY
	
_f_ftoh

	push	{sreg03-sreg05,lr}
	
	mov	sreg01, #1	;enable IEEE fp16 format
	
	lsr	sreg02, fx, #16
	
	movw	sreg12, #65535
	
	and	sreg03, sreg02, #32768
	
	ubfx	sreg02, fx,#23, #8
	
	movt	sreg12, #127
	
	and	sreg12, fx, sreg12
	
	cmp	sreg02, #255
	
	bne	label_01
	
	cbz	sreg01, label_02
	
	orr	fx, sreg03, #32256
	
	orr	fx, fx, sreg12, lsr #13
	
	uxth	sreg03, fx

label_02

	mov	fx, sreg03

	pop	{sreg03-sreg05, pc}

label_01

	cbnz	sreg02, label_03

	cmp	sreg12, #0

	it	eq

	moveq	fx, sreg03

	beq	label_04

label_03

	sub	sreg02, #127

	mvn	fx, #13

	orr	sreg12, sreg12, #8388608

	cmp	sreg02, fx

	bge	label_05

	movw	sreg04, #65535

	mvn	fx, #24

	movt	sreg04, #127

	cmp	sreg02, fx

	it	lt

	mvnlt	sreg02, #25

	blt	label_06

	beq	label_06

	add	fx, sreg02, #24

	lsrs	sreg04, fx

	b	label_06

label_05

	movw	sreg04, #8191

label_06

	ands	sreg05, sreg12, sreg04

	beq	label_07

	adds	fx, sreg04, #1

	lsr	fx, fx, #1

	cmp	fx, sreg05

	it	eq

	andeq	fx, sreg12, fx, lsl #1

	add	sreg12, fx

	cmp	sreg12, #16777216

	itt	cs

	movcs	sreg12, sreg12, lsr #1

	addcs	sreg02, sreg02, #1

label_07

	cbz	sreg01, label_08

	cmp	sreg02, #15

	ble	label_09

	orr	fx, sreg03, #31744

	uxth	fx, fx

	pop	{sreg03-sreg05, pc}

label_08

	cmp	sreg02, #16

	ble	label_09

	movw	fx, #32767

	orrs	fx, sreg03

	uxth	fx, fx

	pop	{sreg03-sreg05, pc}

label_09

	mvn	fx, #23

	cmp	sreg02, fx

	it	lt

	movlt	fx, sreg03

	blt	label_04

	mvn	fx, #13

	cmp	sreg02, fx

	bge	label_10

	subs	fx, fx, sreg02

	mov	sreg02, #13

	lsr	sreg12, sreg12, fx

	mvns	sreg02, sreg02

label_10

	add	sreg01, sreg02, #14

	mov	fx, sreg12, lsr #13

	add	fx, fx, sreg01, lsl #10

	orrs	fx, sreg03

	uxth	fx, fx

label_04

	pop	{sreg03-sreg05, pc}
