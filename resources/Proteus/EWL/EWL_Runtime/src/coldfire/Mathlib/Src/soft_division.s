;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: soft_division.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;##################################################
; Software Divide Routines                        #
;##################################################
;
	.include ColdFire_sfpe.pre.s

        .text

.ifdef NO_HARDWARE_DIVIDE

	.weak	__fp_PA_divu16

__fp_PA_divu16:
	lea	-20(a7),a7
	movem.l d0/d1/d2/d3/d4,(a7)
	move	ccr,d4
	move.l  24+0(a7),d2
	and.l   #$0000ffff,d2
	beq	_PA_divide_by_zero
	move.l  24+4(a7),d0
	moveq	#32,d3
	clr.l	d1
_PA_div1:
	add.l	d0,d0
	addx.l	d1,d1
	bset	#0,d0
	sub.l	d2,d1
	bcc.s	_PA_div2
	add.l	d2,d1
	bclr	#0,d0
_PA_div2:
	subq.l	#1,d3
	bne.s	_PA_div1
	cmp.l	#$0000ffff,d0
	bls.b	_PA_div16_OK
	bset	#1,d4
	bls.b	_PA_div16_end
_PA_div16_OK:
	move.w  d0,24+6(a7)
	move.w  d1,24+4(a7)
_PA_div16_end:
	move	d4,ccr
	movem.l (a7),d0/d1/d2/d3/d4
	lea	20(a7),a7
; Should never get here directly
_PA_divide_by_zero:
	rts

.endif
