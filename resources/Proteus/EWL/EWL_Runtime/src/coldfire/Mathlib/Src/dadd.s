;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: dadd.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Double precision addition and subtraction functions


	.include ColdFire_sfpe.pre.s

	.weak	__d_add
	.weak	__d_sub
	
	.public __fp_d_rounding
	.public __fp_d_overflow

        .text

;===============================================================================
; FUNCTION: __d_add
; DESCRIPTION: Double add.
;
; INPUT: pres(a6) = ptr to result, xhi-xlo(a6) = double, yhi-ylo(a6) = double
; OUTPUT: a0 = pres(a6) = address of result of x + y

__d_add:
        link.w    a6,#Frame_dd2d_localsize
        lea	  -16(a7),a7
        movem.l	  d3-d6,(a7)		    ; SAVE all registers
        movem.l   (Frame_dd2d_xhi,a6),d0-d3 ; d0:d1 = x, d2:d3 = y

	move.l	  d0,d5			    ; x and y have different signs? sub
	eor.l	  d2,d5
	bmi	  dsub_common

dadd_common:
	move.l	  d0,d5			    ; save sign of x in a1
	andi.l	  #0x80000000,d5
	movea.l	  d5,a1
	eor.l	  d5,d0			    ; and set MSB of x to 0

	move.l	  d0,d4			; classify 1st number
	or.l	  d1,d4
	beq	  @x_zero		; 0.0 + x?
	cmpi.l	  #0x7ff00000,d0
	bhs	  @x_MaxExp		; NaN or Inf + x?

	and.l	  #0x7fffffff,d2	; classify 2nd number
	move.l	  d2,d4
	or.l	  d3,d4
	beq	  @return_signed_x	; x + 0.0 = x
	cmpi.l	  #0x7ff00000,d2
	bhs	  @y_MaxExp		; x + NaN or Inf?

	move.l	  d0,d4			; isolate exponent of x
	andi.l	  #0x7ff00000,d4
	beq	  @x_denorm		; x is denormalized
	eor.l	  d4,d0			; zero out the exponent
	ori.l	  #0x00100000,d0	; put hidden 1 bit
@x_denorm_return:

	move.l	  d2,d5			; isolate exponent of y
	andi.l	  #0x7ff00000,d5
	beq	  @y_denorm		; y is denormalized
	eor.l	  d5,d2			; zero out the exponent
	ori.l	  #0x00100000,d2	; put hidden 1 bit
@y_denorm_return:

	cmp.l	  d4,d5			; if exp y > exp x --> swap x and y
	bgt	  @swap_x_y
	sub.l	  d5,d4			; d4 = exp x - exp y
	add.l	  d4,d5			; d5 = exp y = (exp x - exp y) + exp y
@swap_x_y_return:

	lsr.l	  #4,d4
	swap.w	  d4			; d4 = (exp x - exp y) >> 20
	cmpi.l	  #53,d4
	bgt	  @y_too_small          ; Go to case: y is much smaller than x

	cmpi.l	  #32,d4
	beq	  @32bit_add		; case: exp difference == 32
	bgt	  @loword_add	        ; case: exp difference > 32
	move.l	  d2,d6			; denormalize y with (exp x - exp y)
	movea.l	  d3,a0			; save sticky bits
	lsr.l	  d4,d2
	lsr.l	  d4,d3
	subi.l	  #32,d4
	neg.l	  d4
	lsl.l	  d4,d6
	or.l	  d6,d3

	move.l	  a0,d6
	lsl.l	  d4,d6			; d6 contains rounding bits only

	add.l	  d3,d1			; make the addition
	addx.l	  d2,d0
	bra	  @check_carry

@32bit_add:
	move.l	  d3,d6			; low word contains only rounding bits
	clr.l	  d3
	add.l	  d2,d1
	addx.l	  d3,d0
	bra	  @check_carry

@loword_add:
	subi.l	  #32,d4
	move.l	  d3,a0			; save stickies from d3
	move.l	  d2,d6

	clr.l	  d3
	lsr.l	  d4,d2	    		; d0:d1 += d2:d3 >> (exp x - exp y)
	add.l	  d2,d1
	addx.l	  d3,d0

	subi.l	  #32,d4		; compute rounding bits in d6
	neg.l	  d4
	lsl.l	  d4,d6
	clr.l	  d3
	tst.l	  a0
	sne.b	  d3			; set sticky bits if low word != 0
	or.l	  d3,d6

@check_carry:
	btst.l	  #21,d0		; result carried out of the 21 bit mantissa?
	beq	  @check_denorm	        ; carried into bit #21 ? shift right mantissa

	moveq.l	  #31,d4		; shift right mantissa
	move.l	  d0,d2
	move.l	  d1,d3			; saving round bit
	lsr.l	  #1,d0
	lsl.l	  d4,d2
	lsr.l	  #1,d1
	or.l	  d2,d1

	addi.l	  #0x00100000,d5	; bump exponent
	cmpi.l	  #0x7ff00000,d5	; check for overflow
	bge	  @overflow

	lsl.l	  d4,d3			; have rounding bits in d3
	clr.l	  d4
	tst.l	  d6
	sne.b	  d4			; add stickies to the rounding bits
	or.l	  d4,d3
	move.l    d3,d6

@check_denorm:
	btst.l	  #20,d0		; is the hidden 1 bit set?
	beq	  @denorm		; got a denormalized number

	andi.l	  #0x000fffff,d0	; make room for the exponent
	move.l	  a1,d2
	or.l	  d2,d0			; put the sign
	or.l	  d5,d0			; put the exponent

	movea.l	  d6,a0			; save rounding bits
        movem.l	  (a7),d3-d6		; RESTORE all registers
        lea	  16(a7),a7
	bra	  __fp_d_rounding

;;; Special cases

@denorm:
	; denorm is only the sum of 2 denorms -> no rounding!
	andi.l	  #0x000fffff,d0	; set zero exponent
	move.l	  a1,d2
	or.l	  d2,d0			; put the sign

	movea.l	  d3,a0			; save rounding bits
        movem.l	  (a7),d3-d6		; RESTORE all registers
        lea	  16(a7),a7
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
	unlk	  a6
	rts

;;; addition: X + small = X + 0..1 ulp

@y_too_small:
        andi.l    #0x000fffff,d0        ; clear hidden 1 bit
	or.l	  d5,d0			; pack exponent
	move.l	  a1,d5
	or.l	  d5,d0			; pack sign
	movea.l	  #1,a0			; set stickies in a0

        movem.l	  (a7),d3-d6		; RESTORE all registers
        lea	  16(a7),a7
	bra	  __fp_d_rounding

;;;

@x_zero:
	andi.l	  #0x7fffffff,d2
	cmpi.l	  #0x7ff00000,d2
	bhi	  dadd_y_NaN		; y is NaN ? return NaN
	blo	  @return_signed_y
	tst.l	  d3
	bne	  dadd_y_NaN
@return_signed_y:
	move.l	  a1,d0
	or.l	  d0,d2
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d2-d3,(a0)
        movem.l	  (a7),d3-d6		; RESTORE all registers
        lea	  16(a7),a7
	unlk	  a6
	rts

;;;

@x_MaxExp:
	bne	  dadd_x_NaN		; last instruction was cmpi.l #0x7ff00000,d0
	tst.l	  d1
	bne	  dadd_x_NaN

	andi.l	  #0x7fffffff,d2
	cmpi.l	  #0x7ff00000,d2	; is y NaN?
	bhi	  dadd_y_NaN
	blo	  @return_signed_x	; inf + positive = inf
	tst.l	  d3
	bne	  dadd_y_NaN
@return_signed_x:
	move.l	  a1,d4			; put sign back
	or.l	  d4,d0
        movem.l	  (a7),d3-d6		; RESTORE all registers
        lea	  16(a7),a7
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
	unlk	  a6
	rts

;;;

@y_MaxExp:
	bne	  dadd_y_NaN		; last instruction was cmpi.l #0x7ff00000,d2
	tst.l	  d3
	bne	  dadd_y_NaN
	bra	  @return_signed_y	; x + inf = inf

;;;

@x_denorm:
	move.l	  #0x00100000,d4	; denorms have exponent 1
	bra	  @x_denorm_return


@y_denorm:
	move.l	  #0x00100000,d5	; denorms have exponent 1
	bra	  @y_denorm_return

;;;

@swap_x_y:
	; return exp y in d5, exp y - exp x in d4
	sub.l	  d5,d4
	neg.l	  d4

	move.l	  d0,d6			; if (x<y) swap x <=> y
	move.l	  d2,d0
	move.l	  d6,d2

	move.l	  d1,d6
	move.l	  d3,d1
	move.l	  d6,d3
	bra	  @swap_x_y_return
;;;
@overflow:
	move.l	  a1,d0			; sign bit
        movem.l	  (a7),d3-d6		; RESTORE all registers
        lea	  16(a7),a7
	bra	  __fp_d_overflow

;===============================================================================
; FUNCTION: __d_sub
; DESCRIPTION: Double subtract.
;
; INPUT: pres(a6) = ptr to result, xhi-xlo(a6) = double, yhi-ylo(a6) = double
; OUTPUT: a0 = pres(a6) = address of result of x - y

__d_sub:
        link.w    a6,#Frame_dd2d_localsize
        lea	  -16(a7),a7
        movem.l	  d3-d6,(a7)		    ; SAVE all registers
        movem.l   (Frame_dd2d_xhi,a6),d0-d3 ; d0:d1 = x, d2:d3 = y

	move.l	  d0,d5			    ; x and y have different signs? add
	eor.l	  d2,d5
	bmi	  dadd_common
dsub_common:

	move.l	  d0,d5			; save sign of x in a1
	andi.l	  #0x80000000,d5
	movea.l	  d5,a1
	eor.l	  d5,d0			; and set MSB of x to 0

	move.l	  d0,d4			; classify first number
	or.l	  d1,d4
	beq	  @x_zero		; 0.0 - y?
	cmpi.l	  #0x7ff00000,d0
	bhs	  @x_MaxExp		; NaN or Inf - y?

	and.l	  #0x7fffffff,d2	; classify 2nd number
	move.l	  d2,d4
	or.l	  d3,d4
	beq	  @return_signed_x	; x - 0.0 = x
	cmpi.l	  #0x7ff00000,d2
	bhs	  @y_MaxExp		; x - NaN or Inf?

	move.l	  d0,d4			; isolate exponent of x
	andi.l	  #0x7ff00000,d4
	beq	  @x_denorm		; x is denormalized
	eor.l	  d4,d0			; zero out the exponent
	ori.l	  #0x00100000,d0	; put hidden 1 bit
@x_denorm_return:

	move.l	  d2,d5			; isolate exponent of y
	andi.l	  #0x7ff00000,d5
	beq	  @y_denorm		; y is denormalized
	eor.l	  d5,d2			; zero out the exponent
	ori.l	  #0x00100000,d2	; put hidden 1 bit
@y_denorm_return:

	cmp.l	  d4,d5
	beq	  @same_exp		; Go to case: exp x == exp y
	bgt	  @swap_x_y		; if exp y > exp x --> swap x and y
	sub.l	  d5,d4			; d4 = exp x - exp y
	add.l	  d4,d5			; d5 = exp y = (exp x - exp y) + exp y
@swap_x_y_return:

	lsr.l	  #4,d4
	swap.w	  d4			; d4 = (exp x - exp y) >> 20

	cmpi.l	  #1,d4
	beq	  @diff_1_exp		; Go to case: exp x - exp y == 1

	cmpi.l	  #55,d4		; case: exp x - exp y >=55 ; x - very small == x (adjusted by rounding)
	bge	  @shift_out_y
	cmpi.l	  #32,d4		; case: exp x - exp y in 33..54 ; shift hi word of y into lower word
	bgt	  @shift_hi_y
	beq	  @shift_32_y		; case: exp x - exp y == 32 ; hi word of y becomes lower word

	movea.l	  d5,a0			; SAVE d5
	move.l	  d2,d5			; [d2:d3:d6] = [d2:d3:00] >> (exp x - exp y)
	move.l	  d3,d6
	lsr.l	  d4,d2
	lsr.l	  d4,d3
	subi.l	  #32,d4
	neg.l	  d4
	lsl.l	  d4,d5
	lsl.l	  d4,d6
	or.l	  d5,d3
	move.l	  a0,d5			; RESTORE d5
	bra	  @do_subtract

@shift_out_y:
	moveq.l	  #1,d6			; have y < 1/2 ulp of x -> clear y, set the sticky bit
	clr.l	  d2
	clr.l	  d3
	bra	  @do_subtract

@shift_32_y:
	move.l	  d3,d6			; this is a 32-bit shift of [d2:d3:d6]
	move.l	  d2,d3
	clr.l	  d2
	bra	  @do_subtract

@shift_hi_y:
	subi.l	  #32,d4

	movea.l	  d5,a0			; SAVE d5
	move.l	  d2,d5			; [d2:d3:d6] = [d2:d3:00] >> (exp x - exp y)
	move.l	  d3,d6
	lsr.l	  d4,d2
	lsr.l	  d4,d3
	subi.l	  #32,d4
	neg.l	  d4
	lsl.l	  d4,d5
	lsl.l	  d4,d6
	or.l	  d5,d3
	move.l	  a0,d5			; RESTORE d5

        tst.l     d6                    ; use d6 to set sticky bits in d3
        sne.b     d6
        extb.l    d6
        lsr.l     #2,d6                 ; d6 = stickies ? 3fff ffff : 0

        or.l      d3,d6
        move.l    d2,d3
        clr.l     d2

@do_subtract:
	neg.l	  d6			; [d0:d1:d6] = [d0:d1:00] - [d2:d3:d6]
	subx.l	  d3,d1
	subx.l	  d2,d0

	btst.l	  #20,d0		; any borrow from the hidden "1" bit?
	bne	  @round_result

	clr.l	  d4
	lsl.l	  #1,d0			; if so, [d0:d1:d6] <<= 1
	lsl.l	  #1,d1
	addx.l	  d4,d0
	lsl.l	  #1,d6
	addx.l	  d4,d1
	subi.l	  #0x00100000,d5	; decrease the exponent

@round_result:
	; have number in d0:d1, rounding bits in d6, exponent in d5, sign in a1
	andi.l	  #0x000fffff,d0	; make room for the exponent
	move.l	  a1,d2
	or.l	  d2,d0			; put the sign
	or.l	  d5,d0			; put the exponent

	movea.l	  d6,a0			; save rounding bits
        movem.l	  (a7),d3-d6		; RESTORE all registers
        lea	  16(a7),a7
	bra	  __fp_d_rounding

@diff_1_exp:
	clr.l	  d6
	lsl.l	  #1,d0			; x<<=1 (shift left [d0:d1])
	lsl.l	  #1,d1
	addx.l	  d6,d0

	sub.l	  d3,d1			; do the subtraction
	subx.l	  d2,d0

	btst.l	  #21,d0		; has any cancellation happened?
	bne	  @no_cancellation
	subi.l	  #0x00100000,d5	; adjust the exponent
	btst.l	  #20,d0
	bne	  @sub_aligned

@cancellation:
        tst.l	  d0			; check if hi 32 bits are 0
        beq	  @hi_word_cancelled

        clr.l	  d4
@canc_align:
	addq.l	  #1,d4
	lsl.l	  #1,d0
	btst.l	  #20,d0		; align to 001f ffff...
	beq	  @canc_align

	move.l	  d1,d6			; shift also the lower word
	lsl.l	  d4,d1
	sub.l	  #32,d4
	neg.l	  d4
	lsr.l	  d4,d6
	or.l	  d6,d0

	swap.w	  d4
	lsl.l	  #4,d4
	add.l	  d4,d5			; exponent -= #shifts  ( d4 == 32 - #shifts )
	subi.l    #0x02000000,d5
	bra	  @sub_aligned

@hi_word_cancelled:
        subi.l    #0x01500000,d5        ; exp -= 21 (cancel 21 mantissa positions)
	tst.l	  d1
	bmi	  @canc_aligned_low
@canc_align_low:
	subi.l	  #0x00100000,d5	; adjust the exponent
	lsl.l	  #1,d1			; align
	bpl	  @canc_align_low
@canc_aligned_low:
	move.l	  d1,d0
	moveq.l	  #11,d6
	lsr.l	  d6,d0			; align number to 001f ffff ffe0 0000
	moveq.l	  #21,d6
	lsl.l	  d6,d1

@sub_aligned:
	tst.l	  d5			; exponent <= 0? we have a denorm!
	ble	  @underflow

	andi.l	  #0x000fffff,d0	; make room for the exponent
	move.l	  a1,d2
	or.l	  d2,d0			; put the sign
	or.l	  d5,d0			; put the exponent

        movem.l	  (a7),d3-d6		; RESTORE all registers
        lea	  16(a7),a7
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
	unlk	  a6
	rts

@no_cancellation:
	moveq.l	  #31,d3		; x>>=1 (shift back [d0:d1])
	move.l	  d0,d2
	move.l	  d1,d6
	lsr.l	  #1,d0
	lsr.l	  #1,d1
	lsl.l	  d3,d2
	or.l	  d2,d1
	lsl.l	  d3,d6			; have the rounding bit in MSB of d6
	bra	  @round_result

@same_exp:
	sub.l	  d3,d1			; do the subtraction
	subx.l	  d2,d0
	blt	  @sub_negative		; x<y ? swap x and y
	move.l	  d0,d6
	or.l	  d1,d6
	beq       @return_exact_zero    ; x == y ? return exactly 0
@sub_negative_return:
	btst.l    #20,d0                ; has any cancellation occured?
	beq	  @cancellation
	move.l    a1,d2
	or.l      d2,d0                 ; put sign
	or.l      d5,d0                 ; put exponent
        movem.l	  (a7),d3-d6		; RESTORE all registers
        lea	  16(a7),a7
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
	unlk	  a6
	rts

@sub_negative:
	neg.l	  d1			; if x < y - negate result, flip sign in a1
	negx.l	  d0
	move.l	  a1,d6
	eor.l	  #0x80000000,d6
	move.l	  d6,a1
	bra	  @sub_negative_return

@return_exact_zero:
        movem.l	  (a7),d3-d6		; RESTORE all registers
        lea	  16(a7),a7
	clr.l	  d0
	clr.l	  d1
	.ifdef FENV_SUPPORT
        LoadFPSCR d2,a1		; puts state bits in d2
        andi.l	  #(ROUND_MODE_MASK << ROUND_MODE_SHIFT),d2
        cmpi.l	  #(2 << ROUND_MODE_SHIFT),d2
        bne	  @result_plus_zero
        move.l	  #0x80000000,d0
@result_plus_zero:
	.endif
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
	unlk	  a6
	rts


;;; Special cases

@swap_x_y:
	; return exp y in d5, exp y - exp x in d4
	sub.l	  d5,d4
	neg.l	  d4

	move.l	  d0,d6			; if (x<y) swap x <=> y
	move.l	  d2,d0
	move.l	  d6,d2

	move.l	  d1,d6
	move.l	  d3,d1
	move.l	  d6,d3

	move.l	  a1,d6			; if (x<y) flip sign!
	eori.l	  #0x80000000,d6
	movea.l	  d6,a1
	bra	  @swap_x_y_return

@x_zero:
	andi.l	  #0x7fffffff,d2
	beq       @x_zero_y_zero
	cmpi.l	  #0x7ff00000,d2
	bhi	  dadd_y_NaN		; 0.0 - NaN ? return NaN
	blo	  @return_signed_y
	tst.l	  d3
	bne	  dadd_y_NaN
@return_signed_y:
	move.l	  a1,d0
	eori.l	  #0x80000000,d0	; flip sign of x
	or.l	  d0,d2
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d2-d3,(a0)
        movem.l	  (a7),d3-d6		; RESTORE all registers
        lea	  16(a7),a7
	unlk	  a6
	rts

@x_zero_y_zero:
        tst.l     d3
        beq       @return_exact_zero    ; sign of 0 - 0 depends on rounding mode
        bra       @return_signed_y

;;;

@x_MaxExp:
	bne	  dadd_x_NaN		; last instruction was cmpi.l #0x7ff00000,d0
	tst.l	  d1
	bne	  dadd_x_NaN

	andi.l	  #0x7fffffff,d2
	cmpi.l	  #0x7ff00000,d2	; is y NaN/Inf?
	bhi	  dadd_y_NaN
	blo	  @return_signed_x	; inf - finite positive = inf
	tst.l	  d3
	bne	  dadd_y_NaN
	bra	  dadd_invalid_op	; inf - inf = NaN (invalid operation)

@return_signed_x:
	move.l	  a1,d4			; put sign back
	or.l	  d4,d0
        movem.l	  (a7),d3-d6		; RESTORE all registers
        lea	  16(a7),a7
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
	unlk	  a6
	rts

;;;

@y_MaxExp:
	bne	  dadd_y_NaN		; last instruction was cmpi.l #0x7ff00000,d2
	tst.l	  d3
	bne	  dadd_y_NaN
	bra	  @return_signed_y	; x - inf = -inf

;;;

@x_denorm:
	move.l	  #0x00100000,d4	; denorms have exponent 1
	bra	  @x_denorm_return


@y_denorm:
	move.l	  #0x00100000,d5	; denorms have exponent 1
	bra	  @y_denorm_return


;;;

@underflow:
	; have number in d0:d1, exponent in d5, sign in a1
	; the result of subtraction is always exact if it is denormalized
	subi.l	  #0x00100000,d5	; # of shifts = 1 - d5 >> 20
	neg.l	  d5
	lsr.l	  #4,d5
	swap.w	  d5
	cmpi.l	  #32,d5
	bge	  @denorm_loword	; 32 shifts or more?

	move.l	  d0,d2			; shift d0:d1, saving stickies
	lsr.l	  d5,d0
	lsr.l	  d5,d1
	subi.l	  #32,d5
	neg.l	  d5
	lsl.l	  d5,d2
	or.l	  d2,d1

	move.l	  a1,d2
	or.l	  d2,d0			; pack sign

        movem.l	  (a7),d3-d6		; RESTORE all registers
        lea	  16(a7),a7
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
	unlk	  a6
	rts

@denorm_loword:
	subi.l	  #32,d5
	move.l	  d0,d1			; hi word - shifted to lo word
	lsr.l	  d5,d1
	move.l	  a1,d0			; restore sign

        movem.l	  (a7),d3-d6		; RESTORE all registers
        lea	  16(a7),a7
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
	unlk	  a6
	rts

;;; COMMON dadd and dsub terminals

dadd_x_NaN:
	btst.l	  #19,d0		; signaling? return Invalid
	beq	  dadd_invalid_op
	andi.l	  #0x7fffffff,d2
	cmpi.l	  #0x7ff00000,d2	; is y NaN?
	blo	  @return_d0_d1
	bhi	  @x_QNaN_y_NaN
	tst.l	  d3
	beq	  @return_d0_d1
@x_QNaN_y_NaN:
	btst.l	  #19,d2		; y signaling NaN? return Invalid
	beq	  dadd_invalid_op
@return_d0_d1:
        movem.l	  (a7),d3-d6		; RESTORE all registers
        lea	  16(a7),a7
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
	unlk	  a6
	rts

;;;

dadd_y_NaN:
	btst.l	  #19,d2		; signaling? return Invalid
	beq	  dadd_invalid_op
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d2-d3,(a0)		; return 2nd NaN
        movem.l	  (a7),d3-d6		; RESTORE all registers
        lea	  16(a7),a7
	unlk	  a6
	rts

;;;

dadd_invalid_op:
; Signal invalid and return the default NaN.
        movem.l	  (a7),d3-d6		; RESTORE all registers
        lea	  16(a7),a7
      	.ifdef FENV_SUPPORT
        LoadFPSCR d2,a1
        bset      #INVALID_BIT,d2
        StoreFPSCR d2,a1
	.endif
        movea.l   (Frame_dd2d_pres,a6),a0
	move.l	  #0x7ff80000,d0
	clr.l	  d1
        movem.l	  d0-d1,(a0)
	unlk	  a6
	rts
