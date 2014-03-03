;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: dtoi.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Format conversion functions: double precision to int
;

	.include ColdFire_sfpe.pre.s

	.weak	__d_dtoi

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
; FUNCTION: __d_dtoi
; DESCRIPTION: Convert double to integer.
; INPUT: xhi-xlo(a6) = double
; OUTPUT: d0 = int

__d_dtoi:
        link.w    a6,#Frame_d2f_localsize
        move.l    (Frame_d2f_xhi,a6),d0

	movea.l	  d3,a0
	movea.l	  d4,a1			; SAVE d3,d4
	move.l	  d0,d3			; save the original input
	andi.l	  #0x7fffffff,d0	; d0 - absolute value

	cmpi.l	  #0x3ff00000,d0
	blt	  @underflow		; < exponent of 2^0 ?

	move.l	  d0,d1			; extract exponent in d1
	moveq.l	  #20,d2
	lsr.l	  d2,d1

	move.l	  #0x41e,d2		; 31 + bias
	sub.l	  d1,d2			; compute # of shifts
	ble	  @overflow		; negative # of shifts? overflow!

        move.l    (Frame_d2f_xlo,a6),d1	; Normalization:
        moveq.l	  #11,d4		; shift left hi word w/ 11 positions
        lsl.l	  d4,d0
        moveq.l	  #21,d4		; add 11 bits from the lower word
        lsr.l	  d4,d1
        or.l	  d1,d0
	ori.l	  #0x80000000,d0	; add hidden implicit '1' bit

	.ifdef FENV_SUPPORT
	move.l	  d0,d1			; save chopped bits
	.endif

	lsr.l	  d2,d0			; get the integer
	tst.l	  d3			; if the input is negative, negate
	bpl	  @positive
	neg.l	  d0
@positive:

	move.l	  a1,d4                 ; RESTORE d4
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

	move.l	  a0,d3			; RESTORE d3
	unlk	  a6
	rts

@underflow:
	move.l	  a1,d4                 ; RESTORE d4
	.ifdef FENV_SUPPORT
	or.l	  (Frame_d2f_xlo,a6),d0	  ; don't set inexact if there's a true 0
	beq	  @exact_zero
        LoadFPSCR d2,a1
        bset      #INEXACT_BIT,d2
        StoreFPSCR d2,a1
@exact_zero:
	.endif

	move.l	  a0,d3			; RESTORE d3
	clr.l	  d0			; return 0
	unlk	  a6
	rts


@overflow:
	move.l	  a1,d4			; RESTORE d4
	.ifdef FENV_SUPPORT
	cmpi.l	  #0xc1e00000,d3	; don't set invalid if there is a true minint
	bne	  @not_minint
	move.l	  (Frame_d2f_xlo,a6),d1
	beq	  @true_minint
	andi.l    #0xffe00000,d1        ; set inexact if there is a rounded minint
	bne       @not_minint
        LoadFPSCR d2,a1
        bset      #INEXACT_BIT,d2
        StoreFPSCR d2,a1
        bra       @true_minint
@not_minint:
        LoadFPSCR d2,a1
        bset      #INVALID_BIT,d2
        StoreFPSCR d2,a1
@true_minint:
	.endif

	move.l    #0x80000000,d2
	tst.l     d3
	bmi       @return_minint        ; negative? MININT

	cmpi.l	  #0x7ff00000,d0	; NaN ? MININT
	blo       @return_maxint
	bhi       @return_minint
	tst.l	  (Frame_d2f_xlo,a6)
	bne       @return_minint
@return_maxint:
        subq.l    #1,d2
@return_minint:
	move.l    d2,d0

	move.l	  a0,d3			; RESTORE d3
        unlk	  a6
        rts
