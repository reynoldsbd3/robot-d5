;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: dtou.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Format conversion functions: double precision to unsigned int
;

	.include ColdFire_sfpe.pre.s

	.weak	__d_dtou

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
; FUNCTION: __d_dtou
; DESCRIPTION: Convert double to unsigned integer.
; INPUT: xhi-xlo(a6) = double
; OUTPUT: d0 = unsigned int

__d_dtou:
        link.w    a6,#Frame_d2f_localsize
        move.l    (Frame_d2f_xhi,a6),d0
	bmi	  @negative		    ; negative? 0 or NaN

	cmpi.l	  #0x3ff00000,d0
	blt	  @underflow		    ; < exponent of 2^0 ?

	move.l	  d0,d1			    ; extract exponent in d1
	moveq.l	  #20,d2
	lsr.l	  d2,d1

	move.l	  #0x41e,d2		    ; 31 + bias
	sub.l	  d1,d2			    ; compute # of shifts
	blt	  @overflow		    ; negative # of shifts? overflow!

	movea.l	  d3,a0			; SAVE d3
        move.l    (Frame_d2f_xlo,a6),d1	; Normalization:
        moveq.l	  #11,d3		; shift left hi word w/ 11 positions
        lsl.l	  d3,d0
        moveq.l	  #21,d3		; add 11 bits from the lower word
        lsr.l	  d3,d1
        or.l	  d1,d0
	ori.l	  #0x80000000,d0	; add hidden implicit '1' bit
	move.l	  a0,d3			; RESTORE d3

	.ifdef FENV_SUPPORT
	move.l	  d0,d1			; save chopped bits
	.endif
	lsr.l	  d2,d0			; compute the result

	.ifdef FENV_SUPPORT
	subi.l	  #32,d2		; keep only chopped bits
	neg.l	  d2
	lsl.l	  d2,d1
	move.l    (Frame_d2f_xlo,a6),d2 ; get the double low word bits
	andi.l	  #0x001fffff,d2
	or.l	  d2,d1
	beq	  @exact		; signal inexact if chopped

        LoadFPSCR d2,a1
        bset      #INEXACT_BIT,d2
        StoreFPSCR d2,a1
@exact:
	.endif

	unlk	  a6
	rts

@underflow:
	.ifdef FENV_SUPPORT
	or.l	  (Frame_d2f_xlo,a6),d0	  ; don't set inexact if there's a true 0
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
	cmpi.l	  #0xfff00000,d0
	bhi	  @overflow		; check for NaN
	blo	  @zero
	tst.l	  (Frame_d2f_xlo,a6)
	beq	  @zero

@overflow:
	moveq.l	  #-1,d0		; return MAXINT and set invalid
	.ifdef FENV_SUPPORT
        LoadFPSCR d2,a1
        bset      #INVALID_BIT,d2
        StoreFPSCR d2,a1
	.endif
        unlk	  a6
        rts
