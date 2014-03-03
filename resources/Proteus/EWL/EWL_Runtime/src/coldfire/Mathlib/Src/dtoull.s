;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: dtoull.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Format conversion functions: double precision to unsigned long long
;

	.include ColdFire_sfpe.pre.s

	.weak	__d_dtoull

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
; FUNCTION: __d_dtoull
; DESCRIPTION: Convert double to unsigned long long integer.
; INPUT: pres(a6) = ptr to result field, xhi-xlo(a6) = double
; OUTPUT: a0 = pres(a6) = address of converted long long

__d_dtoull:
        link.w    a6,#Frame_d2d_localsize
        move.l    (Frame_d2d_xhi,a6),d0
	bmi	  @negative		    ; negative? 0 or NaN

	cmpi.l	  #0x3ff00000,d0
	blt	  @underflow		    ; < exponent of 2^0 ?

	move.l	  d0,d1			    ; extract exponent in d1
	moveq.l	  #20,d2
	lsr.l	  d2,d1

	move.l	  #0x43e,d2		    ; 31 + bias
	sub.l	  d1,d2			    ; compute # of shifts
	blt	  @overflow		    ; negative # of shifts? overflow!

	movea.l	  d3,a0			; SAVE d3,d4
	movea.l	  d4,a1
        move.l    (Frame_d2d_xlo,a6),d1	; Normalization:
        move.l	  d1,d4
        moveq.l	  #11,d3		; shift left hi word w/ 11 positions
        lsl.l	  d3,d0
        lsl.l	  d3,d4
        moveq.l	  #21,d3		; add 11 bits from the lower word
        lsr.l	  d3,d1
        or.l	  d1,d0			; get the number in d0:d4
	ori.l	  #0x80000000,d0	; add hidden implicit '1' bit

	cmpi.l	  #32,d2	        ; more than 32 shifts?
	blt	  @64bits

	subi.l	  #32,d2		; 32 bit case: same as dtou
	.ifdef FENV_SUPPORT
	move.l	  d0,d1			; save chopped bits
	.endif
	lsr.l	  d2,d0

	move.l	  a0,d3			; RESTORE d3
	.ifdef FENV_SUPPORT
	subi.l	  #32,d2		; keep only chopped bits
	neg.l	  d2
	lsl.l	  d2,d1
	or.l	  d1,d4
	beq	  @exact		; signal inexact if chopped

        LoadFPSCR d2,a0
        bset      #INEXACT_BIT,d2
        StoreFPSCR d2,a0
@exact:
	.endif

	move.l	  a1,d4			; RESTORE d4
        movea.l   (Frame_d2d_pres,a6),a0
        clr.l	  (a0)
        move.l	  d0,4(a0)
	unlk	  a6
	rts

@64bits:
	move.l	  d0,d1			; save lower word bits
	lsr.l	  d2,d0			; shift higher word

	.ifdef FENV_SUPPORT
	move.l	  d4,d3			; save chopped bits in d3
	.endif
	lsr.l	  d2,d4			; shift lower word

	subi.l	  #32,d2		; make lower word
	neg.l	  d2
	lsl.l	  d2,d1
	or.l	  d4,d1

	move.l	  a1,d4                 ; RESTORE d4
	.ifdef FENV_SUPPORT
	lsl.l	  d2,d3			; keep only chopped bits
	beq	  @64exact		; signal inexact if chopped

        LoadFPSCR d2,a1
        bset      #INEXACT_BIT,d2
        StoreFPSCR d2,a1
@64exact:
	.endif

	move.l	  a0,d3			; RESTORE d3
        movea.l   (Frame_d2d_pres,a6),a0
        movem.l   d0-d1,(a0)
	unlk	  a6
	rts

@underflow:
	.ifdef FENV_SUPPORT
	or.l	  (Frame_d2d_xlo,a6),d0	  ; don't set inexact if there's a true 0
	beq	  @zero
        LoadFPSCR d2,a1
        bset      #INEXACT_BIT,d2
        StoreFPSCR d2,a1
	.endif
@zero:
        movea.l   (Frame_d2d_pres,a6),a0
	clr.l	  (a0)
	clr.l	  4(a0)
	unlk	  a6
	rts

@negative:
	cmpi.l	  #0xfff00000,d0
	bhi	  @overflow		; check for NaN
	blo	  @zero
	tst.l	  (Frame_d2d_xlo,a6)
	beq	  @zero

@overflow:
        movea.l   (Frame_d2d_pres,a6),a0
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
