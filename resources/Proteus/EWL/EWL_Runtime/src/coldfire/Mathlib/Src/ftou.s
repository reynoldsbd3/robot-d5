;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: ftou.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Format conversion functions: single precision to unsigned int
;

	.include ColdFire_sfpe.pre.s

	.weak	__f_ftou

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
; FUNCTION: __f_ftou
; DESCRIPTION: Convert float to unsigned integer.
; INPUT: x(a6) = float
; OUTPUT: d0 = unsigned int

__f_ftou:
        link.w    a6,#Frame_f2f_localsize
        move.l    (Frame_f2f_x,a6),d0
	bmi	  @negative		    ; negative? 0 or NaN

	cmpi.l	  #0x3f800000,d0
	blt	  @underflow		    ; < exponent of 2^0 ?

	move.l	  d0,d1			    ; extract exponent in d1
	moveq.l	  #23,d2
	lsr.l	  d2,d1

	move.l	  #0x9e,d2		    ; 31 + bias
	sub.l	  d1,d2			    ; compute # of shifts
	blt	  @overflow		    ; negative # of shifts? overflow!

	lsl.l	  #8,d0			    ; left justify mantissa
	ori.l	  #0x80000000,d0	    ; add hidden implicit '1' bit

	.ifdef FENV_SUPPORT
	move.l	  d0,d1			    ; save chopped bits
	.endif
	lsr.l	  d2,d0

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

	unlk	  a6
	rts

@underflow:
	.ifdef FENV_SUPPORT
	tst.l	  d0			    ; don't set inexact if there's a true 0
	beq	  @zero
        LoadFPSCR d2,a1
        bset      #INEXACT_BIT,d2
        StoreFPSCR d2,a1
	.endif
@zero:
	clr.l	  d0
	unlk	  a6
	rts

@negative:
	cmpi.l	  #0xff800000,d0
	bls	  @zero			    ; check for NaN

@overflow:
	moveq.l	  #-1,d0		    ; return MAXINT and set invalid
	.ifdef FENV_SUPPORT
        LoadFPSCR d2,a1
        bset      #INVALID_BIT,d2
        StoreFPSCR d2,a1
	.endif
        unlk	  a6
        rts
