;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: fterminals.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Single precision common terminal sequences

	.include ColdFire_sfpe.pre.s

	.weak	__fp_f_overflow
	.weak	__fp_f_underflow

	.ifdef FENV_SUPPORT
	.weak	__fp_f_directed_rounding
	.endif

        .text

;===============================================================================
; TERMINAL: __fp_f_overflow
; DESCRIPTION: Returns the result of an overflow according to the rounding mode
; INPUT:  d0.31 = result sign
; OUTPUT: d0 = 32-bit packed result
__fp_f_overflow:
	.ifdef FENV_SUPPORT
        LoadFPSCR d2,a1		 ; puts state bits in d2
	.endif
	andi.l	  #0x80000000,d0	 ; d0 - +/- infinity
	ori.l	  #0x7f800000,d0
	.ifdef FENV_SUPPORT
        ori.l	  #OVERFLOW_OR_INEXACT_MASK,d2
        StoreFPSCR d2,a1
        btst      #DIRECTED_BIT,d2
        bne       @dir_round
        btst      #CHOP_OR_UP_BIT,d2     ; toward 0 --> subtract 1
        sne.b	  d1
        andi.l	  #1,d1			 ; select 0 / 1
        sub.l	  d1,d0
        .endif
        unlk	  a6
        rts

	.ifdef FENV_SUPPORT
@dir_round:
	; d0 - hi word of result
	; d2 - floating point state
        btst      #CHOP_OR_UP_BIT,d2
        bne       @round_plus
        tst.l     d0		         ; Rounding down, so increment if result is negative.
        spl.b	  d1
        andi.l	  #1,d1			 ; select 0 / 1
        sub.l	  d1,d0
        unlk	  a6
        rts
@round_plus:
        tst.l     d0			 ; Rounding up, so increment if result is positive.
        smi.b	  d1
        andi.l	  #1,d1			 ; select 0 / 1
        sub.l	  d1,d0
        unlk	  a6
        rts
        .endif

;===============================================================================
; TERMINAL: __fp_f_underflow
; DESCRIPTION: Returns the result of an underflow according to the rounding mode
; INPUT:  d0.31 = result sign
; OUTPUT: d0 = 32-bit packed result
__fp_f_underflow:
	.ifdef FENV_SUPPORT
        LoadFPSCR d2,a1		 ; puts state bits in d2
	.endif
	andi.l	  #0x80000000,d0	 ; d0 - +/- 0
	.ifdef FENV_SUPPORT
        ori.l	  #UNDERFLOW_OR_INEXACT_MASK,d2
        StoreFPSCR d2,a1

        btst      #DIRECTED_BIT,d2
        beq       @endround
        btst      #CHOP_OR_UP_BIT,d2

        bne       @round_plus
        tst.l     d0		         ; Rounding down, so increment if result is negative.
        bmi       @roundup
        unlk	  a6
        rts
@round_plus:
        tst.l     d0			 ; Rounding up, so increment if result is positive.
        bpl       @roundup
        unlk	  a6
        rts
@roundup:
        addq.l    #1,d0			 ; return smallest denormalized number
@endround:
        .endif
        unlk	  a6
        rts

;===============================================================================
; TERMINAL: __fp_f_directed_rounding
; DESCRIPTION: Rounds a float result and returns, checking for overflow
; INPUT:  d0 = result
;	  d2 = fpscr
	.ifdef FENV_SUPPORT
__fp_f_directed_rounding:
        btst      #CHOP_OR_UP_BIT,d2
        bne       @round_plus
        tst.l     d0		         ; Rounding down, so increment if result is negative.
        bmi       @roundup
        bra       @endround
@round_plus:
        tst.l     d0			 ; Rounding up, so increment if result is positive.
        bmi       @endround
@roundup:
        addq.l    #1,d0			 ; bump lsb
	move.l	  #0x807fffff,d1	 ; did we overflow? check for maxexp
	or.l	  d0,d1			 ; we got only 1s in case of maxexp
	not.l	  d1
	beq	  @signal_overflow_exit
@endround:
        StoreFPSCR d2,a1
    	unlk	  a6
    	rts

@signal_overflow_exit:
	bset	  #OVERFLOW_BIT,d2
        StoreFPSCR d2,a1
        unlk	  a6
        rts
        .endif

