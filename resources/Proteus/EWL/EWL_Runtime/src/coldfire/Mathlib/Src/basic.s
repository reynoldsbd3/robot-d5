;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: basic.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Simple and seldom used functions


	.include ColdFire_sfpe.pre.s

	.weak	__f_neg
	.weak	__d_neg

	.weak	__d_dtoq
	.weak	__d_qtod

        .text

;===============================================================================
; FUNCTION: __f_neg
; DESCRIPTION: Negate value (i.e. flip its sign bit).
;
; INPUT: x(a6) = float
; OUTPUT: d0 = result
__f_neg:
	move.l	  4(a7),d0
	eori.l	  #0x80000000,d0
        rts


;===============================================================================
; FUNCTION: __d_neg
; DESCRIPTION: Negate value (i.e. flip its sign bit).
;
; INPUT: pres(a6) = result ptr, xhi-xlo(a6) = double
; OUTPUT: a0 = pres(a6), location of result
__d_neg:
	movea.l	  4(a7),a0
	move.l	  8(a7),d0
        bchg      #31,d0
        move.l	  d0,(a0)
	move.l    12(a7),4(a0)
	rts


;===============================================================================
; FUNCTION: __d_dtoq
; DESCRIPTION: Convert between double and long double.
; INPUT: pres(a6) = ptr to result, xhi-xlo(a6) = double
; OUTPUT: (a0) = (pres(a6)) = result
; TRICK: just return input

__d_dtoq:				; fall through
;===============================================================================
; FUNCTION: __d_qtod
; DESCRIPTION: Convert between double and long double.
; INPUT: pres(a6) = ptr to result, x(a6) = ptr to arg
; OUTPUT: (a0) = (pres(a6)) = result
; TRICK: just return input

__d_qtod:
	movea.l  4(a7),a0
	move.l   8(a7),(a0)
	move.l   12(a7),4(a0)
	rts

