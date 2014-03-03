;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: int_to_d.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Format conversion functions: integer to double precision
;

	.include ColdFire_sfpe.pre.s

	.weak	__d_utod
	.weak	__d_itod
	.weak	__d_ulltod
	.weak	__d_lltod
	
	.ifdef FENV_SUPPORT
	.public __fp_d_directed_rounding
	.endif

        .text


;===============================================================================
; STRATEGY: Converting integral types to floating types requires alignment and
; then coercion.


;===============================================================================
; FUNCTION: __d_utod
; DESCRIPTION: Convert unsigned word to double.
; INPUT: pres(a6) = ptr to result, x(a6) = unsigned int
; OUTPUT: a0 = pres(a6) = ptr to double result


__d_utod:
    	link.w    a6,#Frame_f2d_localsize
    	move.l	  #0x41d00000,d2	; double of 2^30
        move.l    (Frame_f2d_x,a6),d0
utod_common_z:
    	beq	  utod_return_zero	; 0 ? return 0.0f
utod_common_m:
  	bmi	  utod_aligned		; top bit is 1
utod_common_p:
@cntzero:
	subi.l	  #0x00100000,d2	; subtract exponent
	lsl.l	  #1,d0
	bpl	  @cntzero		; loop for all leading zero bits
utod_aligned:
	move.l	  d0,d1			; save lower word bits
	lsr.l	  #8,d0			; put mantissa into position (8+3=11)
	lsr.l	  #3,d0
	add.l	  d2,d0			; put exponent (d2 + 1)
	moveq.l	  #21,d2
	lsl.l	  d2,d1			; put lower word bits in position: shift (32-11)

        movea.l   (Frame_f2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
	unlk	  a6
	rts


utod_return_zero:
        movea.l   (Frame_f2d_pres,a6),a0
        clr.l	  d1			; return 0.0d
        movem.l	  d0-d1,(a0)
	unlk	  a6
	rts

;===============================================================================
; FUNCTION: __d_itod
; DESCRIPTION: Convert signed word to double.
; INPUT: pres(a6) = ptr to result, x(a6) = int
; OUTPUT: a0 = pres(a6) = ptr to double result
; OPTIMIZATION: uses _d_utod


__d_itod:
    	link.w    a6,#Frame_f2d_localsize
    	move.l	  #0x41d00000,d2	; double of 2^30
    	move.l	  (Frame_f2d_x,a6),d0
    	beq	  utod_return_zero	; zero? return 0.0d (same as 0x0) !
    	bpl	  utod_common_p		; if positive - utod case
    	ori.l	  #0x80000000,d2	; set the sign bit in the result
    	neg.l	  d0			; get absolute value
	bra	  utod_common_m     	; we are in utod case

;===============================================================================
; FUNCTION: __d_ulltod
; DESCRIPTION: Convert unsigned double-word to double.
; INPUT: pres(a6) = ptr to result, xhix-xlo(a6) = unsigned long long
; OUTPUT: a0 = pres(a6) = ptr to double result
; OPTIMIZATION: Uses signed conversion, with faked positive sign.


__d_ulltod:
        link.w    a6,#Frame_d2d_localsize
	move.l    (Frame_d2d_xhi,a6),d0	    ; if hi word is 0 -> go to utod case
	beq	  ulltod_32bits

ulltod_common:
	move.l	  #0x43d00000,d2	    ; exponent of 2 ^ 62
	move.l    (Frame_d2d_xlo,a6),d1	    ; load low word

lltod_common:
	movea.l	  d3,a0			; SAVE d3,d4
	movea.l	  d4,a1

	moveq.l	  #32,d3		; low word shift count in d3
	tst.l     d0
	bmi	  @aligned
@cntzero:
	subq.l	  #1,d3			; compute (32 - shift count) in d3
	lsl.l	  #1,d0  		; shift high word
	bpl	  @cntzero		; loop for all leading zero bits
@aligned:
	move.l	  d1,d4			; align lower word too:
	lsr.l	  d3,d4			; d0 |= d1 >> 32 - shift count
	or.l	  d4,d0

	subi.l	  #32,d3		; d1 <<= shift count
	neg.l	  d3
	lsl.l	  d3,d1

	move.l	  #20,d4		; adjust exponent with shift count
	lsl.l	  d4,d3
	sub.l	  d3,d2

	move.l	  d1,d4			; save rounding bits in d4

	clr.l	  d3			; put mantissa in position; d0:d1 >>= 11
	move.w	  d0,d3
	swap.w	  d3
	lsl.l	  #5,d3			; d3 = d0 << 21
	lsr.l	  #8,d1
	lsr.l	  #3,d1
	or.l	  d3,d1			; d1 = (d1 >> 11) | (d0 << 32-11)
	lsr.l	  #8,d0
	lsr.l	  #3,d0			; d0 >>= 11

	add.l	  d2,d0			; put the exponent

;	move.l    a0,d3                 ; RESTORE d3
	andi.l	  #0x7ff,d4		; any rounding bits?
	beq	  @return

	.ifdef FENV_SUPPORT
        LoadFPSCR d2,a0		 ; puts state bits in d2
        bset      #INEXACT_BIT,d2
        btst      #DIRECTED_BIT,d2
        bne       @directed_rounding
        btst      #CHOP_OR_UP_BIT,d2     ; toward 0 --> truncate
        bne       @endround
    .endif
    btst      #10,d4                 ;  test round bit
    beq       @endround	         ;  round bit is 0 --> truncate
    andi.l	  #0x3ff,d4		 ;  check other bits
    bne       @roundup               ;  nonzero stickies force roundup
    btst      #0,d1                  ;  Halfway case: check LSB
    beq       @endround              ;  is LSB nonzero?
@roundup:
	clr.l	  d3
    addq.l    #1,d1			 ; bump lsb
    addx.l	  d3,d0			 ; bump msb if carry; no overflow possible
@endround:
	.ifdef FENV_SUPPORT
        StoreFPSCR d2,a0
    .endif

@return:
	move.l    a0,d3                 ; RESTORE d3
	move.l	  a1,d4                 ; RESTORE d4

    movea.l   (Frame_d2d_pres,a6),a0
    movem.l	  d0-d1,(a0)
    unlk	  a6
    rts

	.ifdef FENV_SUPPORT
@directed_rounding:
	move.l	  a1,d4                 ; RESTORE d4
	bra	  __fp_d_directed_rounding
    .endif

;===============================================================================
; FUNCTION: __d_lltod
; DESCRIPTION: Convert signed double-word to double.
; INPUT: pres(a6) = ptr to result, xhix-xlo(a6) = long long int
; OUTPUT: a0 = pres(a6) = ptr to double result


__d_lltod:
	link.w    a6,#Frame_d2d_localsize
	move.l    (Frame_d2d_xhi,a6),d0
	beq	  ulltod_32bits		    ; if hi word zero - go to utof case
	bpl	  ulltod_common		    ; if positive - use ulltof

	move.l	  (Frame_d2d_xlo,a6),d1
	neg.l	  d1			    ; if negative - negate number
	negx.l	  d0
	tst.l     d0
	beq	  @32bits		    ; if top word is 0 - set sign bit and utof

	move.l	  #0xc3d00000,d2	    ; exponent of - 2 ^ 62
	bra	  lltod_common


@32bits:
    	move.l	  #0xc1d00000,d2	    ; double of -2^30
	move.l    d1,d0			    ; utod on lower word
	bra	  utod_common_m


ulltod_32bits:
    	move.l	  #0x41d00000,d2	    ; double of 2^30
        move.l	  (Frame_d2d_xlo,a6),d0
        bra	  utod_common_z
