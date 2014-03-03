;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: int_to_f.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Format conversion functions: integer to single precision
;

	.include ColdFire_sfpe.pre.s

	.weak	__f_utof
	.weak	__f_itof
	.weak	__f_ulltof
	.weak	__f_lltof
	
	.ifdef FENV_SUPPORT
	.public __fp_f_directed_rounding
	.endif

        .text


;===============================================================================
; STRATEGY: Converting integral types to floating types requires alignment and
; then coercion.

;===============================================================================
; FUNCTION: __f_utof
; DESCRIPTION: Convert unsigned word to float.
; CASES: Zero maps to +0. Large values are rounded according to the current  mode.
; INPUT: x(a6) = unsigned word
; OUTPUT: d0 = float

__f_utof:
    	link.w    a6,#Frame_f2f_localsize
    	move.l	  #0x4e800000,d2	; float of 2^30
    	move.l	  (Frame_f2f_x,a6),d0
itof_common_z:
    	beq	  utof_return		; 0 ? return 0.0f
itof_common_m:
  	bmi	  utof_aligned		; top bit is 1
itof_common_p:
@cntzero:
	subi.l	  #0x00800000,d2	; subtract exponent
	lsl.l	  #1,d0
	bpl	  @cntzero		; loop for all leading zero bits
utof_aligned:
	move.l	  d0,d1			; save rounding bits
	lsr.l	  #8,d0			; put mantissa into position
	add.l	  d2,d0			; put exponent (d2 + 1)
	andi.l	  #0xff,d1		; any rounding?
	beq	  utof_return

	FloatRounding 7,0x7f,0		; round (no overflow check needed);

utof_return:
    	unlk	  a6
    	rts



;===============================================================================
; FUNCTION: __f_itof
; DESCRIPTION: Convert signed word to float.
; INPUT: x(a6) = signed word
; OUTPUT: d0 = float
; OPTIMIZATION: Uses unsigned conversion, with faked positive sign.

__f_itof:
    	link.w    a6,#Frame_f2f_localsize
    	move.l	  #0x4e800000,d2	; float of 2^30
    	move.l	  (Frame_f2f_x,a6),d0
    	beq	  utof_return		; zero? return 0.0f (same as 0x0) !
    	bpl	  itof_common_p		; if positive - utof case
    	ori.l	  #0x80000000,d2	; set the sign bit in the result
    	neg.l	  d0			; get absolute value
	bra	  itof_common_m     	; we are in utof case

;===============================================================================
; FUNCTION: __f_ulltof
; DESCRIPTION: Convert unsigned long long to float.
; INPUT: xhi-xlo(a6) = arg
; OUTPUT: d0 = float
; OPTIMIZATION: Uses utof if input < 2^32

__f_ulltof:
	link.w    a6,#Frame_d2f_localsize
	move.l    (Frame_d2f_xhi,a6),d0	    ; if hi word is 0 -> go to utof case
	beq	  ulltof_32bits

ulltof_common:
	movea.l	  d3,a0			; SAVE d3,d4
	movea.l	  d4,a1
	move.l	  #0x5e800000,d4	; exponent of 2 ^ 62 in d4
	move.l    (Frame_d2f_xlo,a6),d3 ; load low word

lltof_common:
	moveq.l	  #32,d2		; low word shift count in d2
	tst.l     d0
	bmi	  @aligned
@cntzero:
	subq.l	  #1,d2			; compute shift count
	lsl.l	  #1,d0  		; shift high word
	bpl	  @cntzero		; loop for all leading zero bits
@aligned:
	move.l	  d3,d1
	lsr.l	  d2,d1			; shift from low word into hi word
	or.l	  d1,d0

	clr.l	  d1
	move.b	  d0,d1			; save rounding bits from d0
	lsl.l	  #8,d1			; make room for low word stickies

	subi.l	  #32,d2		; shift low word with d2 = 32-count
	neg.l	  d2
	lsl.l	  d2,d3			; d3 - shifted low word
	sne.b	  d1			; any low word sticky bits? put the result in d1

	lsr.l	  #8,d0			; put mantissa into position
	add.l	  d4,d0			; put exponent of 2^31 ( exp 2 ^ 30 + 1 : the implicit bit )
	swap.w	  d2			; subtract d2 << 23
	lsl.l	  #7,d2
	sub.l	  d2,d0

	move.l	  a0,d3			; RESTORE d3,d4
	move.l	  a1,d4

	tst.l	  d1			; any rounding bits?
	beq	  @return		; return if none

	FloatRounding 15, 0x7fff, 0

@return:
    	unlk	  a6
    	rts

;===============================================================================
; FUNCTION: __f_lltof
; DESCRIPTION: Convert signed long long to float.
; INPUT: xhi-xlo(a6) = arg
; OUTPUT: d0 = float
; CASES: Zero maps to +0. Large values are rounded according to the current mode.
; OPTIMIZATION: call ulltof and negate if needed


__f_lltof:
	link.w    a6,#Frame_d2f_localsize
	move.l    (Frame_d2f_xhi,a6),d0
	beq	  ulltof_32bits		    ; if hi word zero - go to utof case
	bpl	  ulltof_common		    ; if positive - use ulltof

	move.l	  (Frame_d2f_xlo,a6),d1
	neg.l	  d1			    ; if negative - negate number
	negx.l	  d0
	tst.l     d0
	beq	  @32bits		    ; if top word is 0 - set sign bit and utof

	movea.l	  d3,a0			; SAVE d3,d4
	movea.l	  d4,a1
	move.l	  #0xde800000,d4	; exponent of - 2 ^ 30 in d4
	move.l    d1,d3			; load low word
	bra	  lltof_common


@32bits:
    	move.l	  #0xce800000,d2	    ; float of 2^30, negative
	move.l    d1,d0			    ; utof on lower word
	bra	  itof_common_m


ulltof_32bits:
    	move.l	  #0x4e800000,d2	    ; float of 2^30, positive
	move.l    (Frame_d2f_xlo,a6),d0	    ; utof on lower word
	bra	  itof_common_z
