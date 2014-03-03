;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: ftod.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Format conversion functions: single to double precision
;

	.include ColdFire_sfpe.pre.s

	.weak	__f_ftod
	.weak	__f_ftoq

        .text


;===============================================================================
; FUNCTION: __f_ftoq
; DESCRIPTION: Convert float to double (== long double).
; INPUT: pres(a6) = ptr to result field, x(a6) = float
; OUTPUT: a0 = pres(a6) = location of computed (long) double.


__f_ftoq:
; fall through to __f_ftod


;===============================================================================
; FUNCTION: __f_ftod
; DESCRIPTION: Convert float to double.
; INPUT: pres(a6) = ptr to result field, x(a6) = float
; OUTPUT: a0 = pres(a6) = location of computed (long) double.
__f_ftod:
        link.w    a6,#Frame_f2d_localsize
        move.l    (Frame_f2d_x,a6),d0    ; the number

        move.l	  d0,d2			 ; get exponent
        andi.l	  #0x7f800000,d2
        beq	  @zero_exp		 ; zero / denorm
        cmpi.l    #0x7f800000,d2
        beq	  @max_exp		 ; inf / nan

        ; add bias to exponent and shift mantissa right 3 positions.
        move.l	  d0,d1                  ; shift mantissa into lower word
        moveq.l	  #29,d2
        lsl.l	  d2,d1

        move.l	  d0,d2			 ; compute higher word
        lsl.l	  #1,d2			 ; clear msb and shift right 3
        lsr.l	  #4,d2
        addi.l	  #0x38000000,d2	 ; add bias difference: (1023-127) << 20
        andi.l	  #0x80000000,d0
        or.l	  d2,d0			 ; 'and' with sign bit

        movea.l   (Frame_f2d_pres,a6),a0 ; put result in memory and return
        movem.l   d0-d1,(a0)

        unlk	  a6
        rts


@zero_exp:
	move.l	  d0,d1			; get mantissa
	andi.l	  #0x007fffff,d1
	bne	  @denorm

        movea.l   (Frame_f2d_pres,a6),a0 ; return 0
        clr.l	  d1			 ; lo word is 0
        movem.l   d0-d1,(a0)             ; hi word is the same
	unlk	  a6
	rts


@denorm:
	movea.l	  d3,a0			; SAVE d3

	; normalize the denorm...
	move.l	  #0x38000000,d2	; largest single precision denorm
					; represented as float (hi word)
	move.l	  d0,d1			; shift mantissa 9 positions left
	moveq.l	  #9,d3
	lsl.l	  d3,d1
	bmi	  @1
@2:
	subi.l	  #0x00100000,d2	; bump exponent
	add.l	  d1,d1			; left shift mantissa
	bpl	  @2
@1:
	andi.l	  #0x80000000,d0	; put exponent in place
	or.l	  d2,d0
	move.l	  d1,d3			; save hi word

	moveq.l	  #21,d2		; keep only the last two bits in the low word
	lsl.l	  d2,d1

        lsl.l	  #1,d3			; 'hide' the hidden "1" bit
	moveq.l	  #12,d2		; shift mantissa in the right place
	lsr.l	  d2,d3			; 12 places = 1 sign + 11 exp
	or.l	  d3,d0			; pack the mantissa

	move.l	  a0,d3			; RESTORE d3
        movea.l   (Frame_f2d_pres,a6),a0
        movem.l   d0-d1,(a0)
        unlk	  a6
        rts


@max_exp:
	move.l	  d0,d1			; get mantissa
	andi.l	  #0x007fffff,d1
	bne	  @nan			; inf or nan?

	andi.l	  #0x80000000,d0	; get exponent
        ori.l	  #0x7ff00000,d0	; make an inf
        movea.l   (Frame_f2d_pres,a6),a0
        clr.l	  d1
        movem.l   d0-d1,(a0)
        unlk	  a6
        rts


@nan:
	.ifdef FENV_SUPPORT
        btst      #22,d1                 ;   test Q bit
        bne       @11
        LoadFPSCR d2,a1
        bset      #INVALID_BIT,d2
        StoreFPSCR d2,a1

@11:
	.endif

        moveq.l	  #29,d2		 ; this part of mantissa goes into lower word
        lsl.l	  d2,d1

        move.l	  d0,d2			 ; compute higher word
        lsr.l	  #3,d2			 ; clear msb and shift right 3
        andi.l	  #0x0007ffff,d2	 ; keep mantissa bits only
        ori.l	  #0x7ff80000,d2	 ; put a quiet NaN
	andi.l	  #0x80000000,d0	 ; keep exponent only
        or.l	  d2,d0			 ; put all together

        movea.l   (Frame_f2d_pres,a6),a0
        movem.l   d0-d1,(a0)
        unlk	  a6
        rts
