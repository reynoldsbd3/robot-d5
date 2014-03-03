;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: dterminals.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Single precision common terminal sequences

	.include ColdFire_sfpe.pre.s

	.weak	__fp_d_rounding
	.weak	__fp_d_underflow
	.weak	__fp_d_overflow

	.ifdef FENV_SUPPORT
	.weak	__fp_d_directed_rounding
	.endif

        .text
;===============================================================================
; TERMINAL: __fp_d_rounding
; DESCRIPTION: Rounds a double result and returns
; INPUT:  d0:d1 = result
;	  a0    = rounding bits (MSB = rounding, rest = stickies)

__fp_d_rounding:
	.ifdef FENV_SUPPORT
	tst.l	  a0			; check if any rounding
	beq	  @endround

        LoadFPSCR d2,a1		 ; puts state bits in d2
        bset      #INEXACT_BIT,d2
        StoreFPSCR d2,a1
        btst      #DIRECTED_BIT,d2
        bne       __fp_d_directed_rounding
        btst      #CHOP_OR_UP_BIT,d2     ; toward 0 --> truncate
        bne       @endround
        .endif
        tst.l	  a0	                 ;  test round bit
        bpl       @endround	         ;  round bit is 0 --> truncate
        adda.l	  a0,a0
        tst.l	  a0
        bne       @roundup               ;  nonzero stickies force roundup
        btst      #0,d1                  ;  Halfway case: check LSB
        beq       @endround              ;  is LSB nonzero?
@roundup:
	clr.l	  d2
        addq.l    #1,d1			 ; bump lsb
        addx.l	  d2,d0
        .ifdef FENV_SUPPORT
	move.l	  #0x800fffff,d2	 ; did we overflow? check for maxexp
	or.l	  d0,d2			 ; we got only 1s in case of maxexp
	not.l	  d2
	beq	  @signal_overflow
	.endif
@endround:
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
	unlk	  a6
	rts

.ifdef FENV_SUPPORT

@signal_overflow:
	LoadFPSCR d2,a1
	bset	  #OVERFLOW_BIT,d2	; have the content of fpscr in d1
        StoreFPSCR d2,a1
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
        unlk	  a6
        rts
.endif

;===============================================================================
; TERMINAL: __fp_d_directed_rounding
; DESCRIPTION: Rounds a double result and returns, checking for overflow
; INPUT:  d0:d1 = result
;	  d2    = fpscr
; NOTE:   Saves and restores register d3 in a0

	.ifdef FENV_SUPPORT
__fp_d_directed_rounding:
	movea.l	  d3,a0			 ; SAVE d3
        btst      #CHOP_OR_UP_BIT,d2
        bne       @round_plus
        tst.l     d0		         ; Rounding down, so increment if result is negative.
        bmi       @roundup
        bra       @endround
@round_plus:
        tst.l     d0			 ; Rounding up, so increment if result is positive.
        bmi       @endround
@roundup:
        clr.l	  d3
        addq.l    #1,d1			 ; bump lsb
        addx.l    d3,d0			 ; bump msb if necessary
	move.l	  #0x800fffff,d3	 ; did we overflow? check for maxexp
	or.l	  d0,d3			 ; we got only 1s in case of maxexp
	not.l	  d3
	bne	  @endround
	bset	  #OVERFLOW_BIT,d2
@endround:
        StoreFPSCR d2,a1
	move.l	  a0,d3			; RESTORE d3

        movea.l   (Frame_d2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
        unlk	  a6
        rts
        .endif

;===============================================================================
; TERMINAL: __fp_d_underflow
; DESCRIPTION: Returns the result of an underflow according to the rounding mode
; INPUT:  d0.31 = result sign
; OUTPUT: a0 = pres(a6) = 64-bit packed result
__fp_d_underflow:
	.ifdef FENV_SUPPORT
        LoadFPSCR d2,a1		 ; puts state bits in d2
	.endif
	andi.l	  #0x80000000,d0	 ; d0 - +/- 0
	clr.l	  d1
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
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
        unlk	  a6
        rts
@roundup:
        addq.l    #1,d1			 ; return smallest denormalized number
@endround:
        .endif
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
        unlk	  a6
        rts

;===============================================================================
; TERMINAL: __fp_d_overflow
; DESCRIPTION: Returns the result of an overflow according to the rounding mode
; INPUT:  d0.31 = result sign
; OUTPUT: a0 = pres(a6) = 64-bit packed result
__fp_d_overflow:
	.ifdef FENV_SUPPORT
        LoadFPSCR d2,a1		 ; puts state bits in d2
	.endif
	andi.l	  #0x80000000,d0	 ; d0 - +/- infinity
	ori.l	  #0x7ff00000,d0
	clr.l	  d1
	.ifdef FENV_SUPPORT
        ori.l	  #OVERFLOW_OR_INEXACT_MASK,d2
        StoreFPSCR d2,a1

        btst      #DIRECTED_BIT,d2
        bne       @dir_round
        btst      #CHOP_OR_UP_BIT,d2     ; toward 0 --> subtract 1
        sne.b	  d1
        extb.l	  d1			 ; ffff ffff in case of chop
        add.l	  d1,d0			 ; subtract 1 in case of chop
        .endif
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
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
        extb.l	  d1			 ; ffff ffff in case of chop
        add.l	  d1,d0			 ; subtract 1 in case of chop
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
        unlk	  a6
        rts
@round_plus:
        tst.l     d0			 ; Rounding up, so increment if result is positive.
        smi.b	  d1
        extb.l	  d1			 ; ffff ffff in case of chop
        add.l	  d1,d0			 ; subtract 1 in case of chop
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
        unlk	  a6
        rts
        .endif
