;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: ftoull.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Format conversion functions: single precision to unsigned long long
;

	.include ColdFire_sfpe.pre.s

	.weak	__f_ftoull

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
; FUNCTION: __f_ftoull
; DESCRIPTION: Convert float to unsigned long long integer.
; INPUT: pres(a6) = ptr to result field, x(a6) = float
; OUTPUT: a0 = pres(a6) = address of converted long long

__f_ftoull:
        link.w    a6,#Frame_f2d_localsize
        move.l    (Frame_f2d_x,a6),d0
	bmi	  @negative		    ; negative? 0 or NaN

	cmpi.l	  #0x3f800000,d0
	blt	  @underflow		    ; < exponent of 2^0 ?

	move.l	  d0,d1			    ; extract exponent in d1
	moveq.l	  #23,d2
	lsr.l	  d2,d1

	move.l	  #0xbe,d2		    ; 31 + bias
	sub.l	  d1,d2			    ; compute # of shifts
	blt	  @overflow		    ; negative # of shifts? overflow!

	lsl.l	  #8,d0			    ; left justify mantissa
	ori.l	  #0x80000000,d0	    ; add hidden implicit '1' bit

	cmpi.l	  #32,d2		    ; more than 32 shifts?
	blt	  @64bits

	subi.l	  #32,d2		    ; 32 bit case: same as ftol

	.ifdef FENV_SUPPORT
	move.l	  d0,d1			    ; save chopped bits
	.endif
	lsr.l	  d2,d0

	.ifdef FENV_SUPPORT
	subi.l	  #32,d2		    ; keep only chopped bits
	neg.l	  d2
	lsl.l	  d2,d1
	beq	  @exact		    ; signal inexact if chopped

        LoadFPSCR d2,a0
        bset      #INEXACT_BIT,d2
        StoreFPSCR d2,a0
@exact:
	.endif

        movea.l   (Frame_f2d_pres,a6),a0
        clr.l	  (a0)
        move.l	  d0,4(a0)
	unlk	  a6
	rts

@64bits:
	move.l	  d0,d1			    ; save lower word bits
	lsr.l	  d2,d0			    ; shift higher word

	subi.l	  #32,d2		    ; shift lower word bits
	neg.l	  d2
	lsl.l	  d2,d1

        movea.l   (Frame_f2d_pres,a6),a0
        movem.l   d0-d1,(a0)
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
        movea.l   (Frame_f2d_pres,a6),a0
	clr.l	  (a0)
	clr.l	  4(a0)
	unlk	  a6
	rts

@negative:
	cmpi.l	  #0xff800000,d0
	bls	  @zero			    ; check for NaN

@overflow:
        movea.l   (Frame_f2d_pres,a6),a0
	moveq.l	  #-1,d0		    ; return MAXINT and set invalid
	move.l	  d0,d1
        movem.l   d0-d1,(a0)
	.ifdef FENV_SUPPORT
        LoadFPSCR d2,a1
        bset      #INVALID_BIT,d2
        StoreFPSCR d2,a1
	.endif
        unlk	  a6
        rts
