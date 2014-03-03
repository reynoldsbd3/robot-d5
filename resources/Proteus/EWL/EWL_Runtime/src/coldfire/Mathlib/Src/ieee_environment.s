;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: ieee_environment.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Access to the floating point status and control register

	.include ColdFire_sfpe.pre.s

.ifdef FENV_SUPPORT

	.weak	__fp_round
	.weak	__fp_get_fpscr
	.weak	__fp_set_fpscr

	.text

;===============================================================================
; FUNCTION: __fp_round
; DESCRIPTION: Set the rounding mode, according to the code in x.
; INPUT: x = 2-bit code, aligned for (low half-word of) fpscr.
; OUTPUT: d0 = selected rounding mode

__fp_round:
	link.w    a6,#Frame_f2f_localsize
	LoadFPSCR d2,a0             ; d2.w = state bits,a
	moveq.l   #ROUND_MODE_MASK,d0    ;      mask off two rounding bits
	move.l    d0,d1
	not.l     d1  			 ;      mask all but rounding bits
	and.l     d1,d2			 ;      kill off current rounding mode
	and.l     (Frame_f2f_x,a6),d0    ;      get argument, and keep only round bits
	lsl.l     #ROUND_MODE_SHIFT,d0
	or.l      d0,d2			 ;      set new bits
        StoreFPSCR d2,a0
	unlk      a6
	rts

;===============================================================================
; FUNCTION: __fp_get_fpscr
; DESCRIPTION: Return the fpscr in d0.
; INPUT: none
; OUTPUT: d0 = fpscr

__fp_get_fpscr:
	LoadFPSCR d0,a0
	rts


;===============================================================================
; FUNCTION: __fp_set_fpscr
; DESCRIPTION: Return the fpscr in d3.
; INPUT: d3 = new fpscr value
; OUTPUT: none

__fp_set_fpscr:
	move.l    4(a7),d0
	StoreFPSCR d0,a0
	rts

.endif
