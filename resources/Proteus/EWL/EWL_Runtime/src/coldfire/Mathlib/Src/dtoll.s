;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: dtoll.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Format conversion functions: double precision to long long
;

	.include ColdFire_sfpe.pre.s

	.weak	__d_dtoll

        .text

;===============================================================================
; A NOTE ABOUT CONVERSIONS TO INTEGRAL FORMATS
;=============================================
;
; Conversions from float and double to the 32- and 64-bit signed and unsigned
; integral formats follow a number of conventions.
; 1) An negative number converts to unsigned 0, without signaling an exception.
; 2) Otherwise, any fractional part is truncated, regardless of rounding mode.
; 3) If a nonzero fraction is truncated, and the value doesn't overflow,
;	inexact is set.
; 4) If a value overflows the integer destination, invalid (not overflow) is set
;	and the result is set to the most negative value for signed destinations
;	and to the biggest value for unsigned destinations.


;===============================================================================
; FUNCTION: __d_dtoll
; DESCRIPTION: Convert double to long long integer.
; INPUT: pres(a6) = ptr to result field, xhi-xlo(a6) = double
; OUTPUT: a0 = pres(a6) = address of converted long long

__d_dtoll:
        link.w    a6,#Frame_d2d_localsize
        move.l    (Frame_d2d_xhi,a6),d0
	movea.l	  d3,a0			; SAVE d3,d4
	movea.l	  d4,a1
	move.l	  d0,d3			; save the original input
	andi.l	  #0x7fffffff,d0	; d0 - absolute value

	cmpi.l	  #0x3ff00000,d0
	blt	  @underflow		; < exponent of 2^0 ?

	move.l	  d0,d1			; extract exponent in d1
	moveq.l	  #20,d2
	lsr.l	  d2,d1

	move.l	  #0x43e,d2		; 31 + bias
	sub.l	  d1,d2			; compute # of shifts in d2
	ble	  @overflow		; negative # of shifts? overflow!

        move.l    (Frame_d2d_xlo,a6),d1
        move.l	  d1,d4
        lsl.l	  #8,d0			; Normalization:
        lsr.l	  #8,d1			; d0:d4 = d0:d1 << 11
        lsl.l	  #8,d4
        lsl.l	  #3,d0			; use immediate shifts to avoid saving
        lsr.l	  #8,d1			; and restoring a register on the stack
        lsl.l	  #3,d4
        lsr.l	  #5,d1
        or.l	  d1,d0
        ori.l     #0x80000000,d0

	cmpi.l	  #32,d2		; more than 32 shifts?
	blt	  @64bits

	subi.l	  #32,d2		; 32 bit case: same as dtoi

	.ifdef FENV_SUPPORT
	move.l	  d0,d1			; save chopped bits
	.endif
	lsr.l	  d2,d0
	tst.l	  d3			; if the input is negative, negate
	bpl	  @positive
	neg.l	  d0
@positive:

	.ifdef FENV_SUPPORT
	subi.l	  #32,d2		; keep only chopped bits
	neg.l	  d2
	lsl.l	  d2,d1
	or.l	  d4,d1			; any bits in the low word?
	move.l	  a1,d4			; RESTORE d4
	tst.l     d1
	beq	  @exact		; signal inexact if chopped

        LoadFPSCR d2,a1
        bset      #INEXACT_BIT,d2
        StoreFPSCR d2,a1
@exact:
        .else
	move.l	  a1,d4			; RESTORE d4
	.endif

        movea.l   (Frame_d2d_pres,a6),a1
        moveq.l	  #31,d2
        asr.l	  d2,d3			; hi word - same as sign bit of d3
        move.l	  d3,(a1)
        move.l	  d0,4(a1)
	move.l	  a0,d3			; RESTORE d3
	movea.l	  a1,a0			; return pointer to result in a0
 	unlk	  a6
	rts

@64bits:
	move.l	  d0,d1			; (d0:d1:d4)  =  (d0:d4:00) >> d2
	lsr.l	  d2,d0			; shift higher word

	.ifdef FENV_SUPPORT
	move.l	  d5,-(a7)		; SAVE d5
	move.l	  d4,d5
	.endif
	lsr.l	  d2,d4			; shift lower word

	subi.l	  #32,d2		; make lower word
	neg.l	  d2
	lsl.l	  d2,d1
	or.l	  d4,d1
	move.l	  a1,d4			; RESTORE d4

	.ifdef FENV_SUPPORT
	lsl.l	  d2,d5			; keep only chopped bits
	beq	  @64exact		; signal inexact if chopped
        LoadFPSCR d2,a1
        bset      #INEXACT_BIT,d2
        StoreFPSCR d2,a1
@64exact:
	move.l	  (a7)+,d5		; RESTORE d5
	.endif

	tst.l	  d3			; if the input is negative, negate
	bpl	  @64positive
	neg.l	  d1
	negx.l	  d0
@64positive:

	move.l	  a0,d3			; RESTORE d3
        movea.l   (Frame_d2d_pres,a6),a0
        movem.l   d0-d1,(a0)
	unlk	  a6
	rts

@underflow:
	move.l	  a0,d3			; RESTORE d3,d4
	move.l	  a1,d4
	.ifdef FENV_SUPPORT
	or.l	  (Frame_d2d_xlo,a6),d0	  ; don't set inexact if there's a true 0
	beq	  @exact_zero
        LoadFPSCR d2,a1
        bset      #INEXACT_BIT,d2
        StoreFPSCR d2,a1
@exact_zero:
	.endif

        movea.l   (Frame_d2d_pres,a6),a0
	clr.l	  (a0)			; return 0
	clr.l	  4(a0)
	unlk	  a6
	rts


@overflow:
	move.l	  a1,d4			;  RESTORE d4
	.ifdef FENV_SUPPORT
	cmpi.l	  #0xc3e00000,d3	; don't set invalid if there is a true minint
	bne	  @not_minint
	move.l	  (Frame_d2d_xlo,a6),d1
	beq	  @true_minint
@not_minint:
        LoadFPSCR d2,a1
        bset      #INVALID_BIT,d2
        StoreFPSCR d2,a1
@true_minint:
	.endif

	move.l    #0x80000000,d2
	clr.l	  d1
	tst.l     d3
	bmi       @return_minint        ; negative? MININT

	cmpi.l	  #0x7ff00000,d0	; NaN ? MININT
	blo       @return_maxint
	bhi       @return_minint
	tst.l	  (Frame_d2d_xlo,a6)
	bne       @return_minint
@return_maxint:
        subq.l    #1,d2			; transform 0x8000... -> 0x7fff...
        subq.l	  #1,d1
@return_minint:
	move.l    d2,d0

	move.l	  a0,d3			; RESTORE d3
        movea.l   (Frame_d2d_pres,a6),a0
        movem.l   d0-d1,(a0)
        unlk	  a6
        rts
