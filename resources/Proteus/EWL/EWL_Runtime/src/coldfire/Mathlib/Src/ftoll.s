;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: ftoll.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Format conversion functions: single precision to long long
;

	.include ColdFire_sfpe.pre.s

	.weak	__f_ftoll

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
; FUNCTION: __f_ftoll
; DESCRIPTION: Convert float to long long integer.
; INPUT: pres(a6) = ptr to result field, x(a6) = float
; OUTPUT: a0 = pres(a6) = address of converted long long

__f_ftoll:
        link.w    a6,#Frame_f2d_localsize
        move.l    (Frame_f2d_x,a6),d0
	movea.l	  d3,a0			; SAVE d3
	move.l	  d0,d3			; save the original input
	andi.l	  #0x7fffffff,d0	; d0 - absolute value

	cmpi.l	  #0x3f800000,d0
	blt	  @underflow		; < exponent of 2^0 ?

	move.l	  d0,d1			; extract exponent in d1
	moveq.l	  #23,d2
	lsr.l	  d2,d1

	move.l	  #0xbe,d2		; 31 + bias
	sub.l	  d1,d2			; compute # of shifts
	ble	  @overflow		; negative # of shifts? overflow!

	lsl.l	  #8,d0			    ; left justify mantissa
	ori.l	  #0x80000000,d0	    ; add hidden implicit '1' bit

	cmpi.l	  #32,d2		    ; more than 32 shifts?
	blt	  @64bits

	subi.l	  #32,d2		    ; 32 bit case: same as ftol
	.ifdef FENV_SUPPORT
	move.l	  d0,d1			    ; save chopped bits
	.endif
	lsr.l	  d2,d0
	tst.l	  d3			; if the input is negative, negate
	bpl	  @positive
	neg.l	  d0
@positive:

	.ifdef FENV_SUPPORT
	subi.l	  #32,d2		    ; keep only chopped bits
	neg.l	  d2
	lsl.l	  d2,d1
	beq	  @exact		    ; signal inexact if chopped

        LoadFPSCR d2,a1
        bset      #INEXACT_BIT,d2
        StoreFPSCR d2,a1
@exact:
	.endif

        movea.l   (Frame_f2d_pres,a6),a1
        moveq.l	  #31,d2
        asr.l	  d2,d3			; hi word - same as sign bit of d3
        move.l	  d3,(a1)
        move.l	  d0,4(a1)
	move.l	  a0,d3			; RESTORE d3
	movea.l	  a1,a0			; return pointer to result in a0
 	unlk	  a6
	rts

@64bits:
	move.l	  d0,d1			    ; save lower word bits
	lsr.l	  d2,d0			    ; shift higher word

	subi.l	  #32,d2		    ; shift lower word bits
	neg.l	  d2
	lsl.l	  d2,d1

	tst.l	  d3			; if the input is negative, negate
	bpl	  @64positive
	neg.l	  d1
	negx.l	  d0
@64positive:

	move.l	  a0,d3			; RESTORE d3
        movea.l   (Frame_f2d_pres,a6),a0
        movem.l   d0-d1,(a0)
	unlk	  a6
	rts

@underflow:
	.ifdef FENV_SUPPORT
	tst.l	  d0			; don't set inexact if there's a true 0
	beq	  @exact_zero
        LoadFPSCR d2,a1
        bset      #INEXACT_BIT,d2
        StoreFPSCR d2,a1
@exact_zero:
	.endif

	move.l	  a0,d3			; RESTORE d3
        movea.l   (Frame_f2d_pres,a6),a0
	clr.l	  (a0)			; return 0
	clr.l	  4(a0)
	unlk	  a6
	rts


@overflow:
	.ifdef FENV_SUPPORT
	cmpi.l	  #0xdf000000,d3	; don't set invalid if there is a true minint
	beq	  @true_minint
        LoadFPSCR d2,a1
        bset      #INVALID_BIT,d2
        StoreFPSCR d2,a1
@true_minint:
	.endif

	cmpi.l	  #0x7f800000,d0
	sls.b	  d0			; set if not NaN
	tst.l	  d3
	spl.b	  d1			; set if positive
	and.l	  d1,d0			; return !NaN & positive ? MAXINT : MININT
	extb.l	  d0
	move.l	  d0,d1
	addi.l	  #0x80000000,d0

	move.l	  a0,d3			; RESTORE d3
        movea.l   (Frame_f2d_pres,a6),a0
        movem.l   d0-d1,(a0)
        unlk	  a6
        rts
