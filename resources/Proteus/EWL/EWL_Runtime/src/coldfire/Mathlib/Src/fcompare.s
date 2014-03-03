;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: fcompare.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Single precision comparison functions

	.include ColdFire_sfpe.pre.s

	.weak	__f_cmp
	.weak	__f_cmpe
	.weak	__f_fun
	.weak	__f_for
	.weak	__f_feq
	.weak	__f_fne
	.weak	__f_flt
	.weak	__f_fle
	.weak	__f_fgt
	.weak	__f_fge

        .text

;===============================================================================
; FUNCTION: __f_cmp, __f_cmpe
;	__f_feq, __f_fne, __f_flt, __f_fgt, __f_fge, __f_fle
;	__f_fun, __f_for
; DESCRIPTION: Comparison functions, all described here because of commonality.
;
; INPUT: x(a6) = float, y(a6) = float
; OUTPUT: d0 = result code:
;	__f_cmp  --> 0-eq, 1-lt, 2-gt, 3-un (invalid if an SNaN)
;	__f_cmpe --> 0-eq, 1-lt, 2-gt, 3-un (invalid if unordered)
;	__f_fXX   --> 0-relation XX is false, 1-relation XX is true
;			XX=eq, ne, un, or --> invalid if an SNaN
;			XX=lt, le, gt, ge --> invalid if unordered
; INCIDENTALLY: un --> unordered; NaNs are unordered with EVERYTHING,
; or --> ordered; all pairs of numbers, finite or infinite, are ordered
; as lt, eq, or gt.
; IMPLEMENTATION: To share code across all routines, put the relation
; to be tested in bits 0x3ff of d0. Masks take care of myriad cases, including
; setting of Invalid on unordered.
; The comparison codes are 0-eq, 1-lt, 2-gt, 3-un, just as with __f_cmp.
; The operation codes are:

OP_EQ             .equ $001
OP_NE             .equ $002
OP_OR             .equ $004
OP_UN             .equ $008
OP_LT             .equ $010
OP_LE             .equ $020
OP_GT             .equ $040
OP_GE             .equ $080
OP_CMP            .equ $100
OP_CMPE		  .equ $200

; these masks select the relations that return 1 on different conditions:
OP_1NAN		  .equ (OP_NE + OP_UN)
OP_1EQ		  .equ (OP_EQ + OP_OR + OP_LE + OP_GE)
OP_1LT		  .equ (OP_CMP + OP_CMPE + OP_NE + OP_OR + OP_LT + OP_LE)
OP_1GT		  .equ (OP_NE + OP_OR + OP_GT + OP_GE)

; this mask selects CMPx operations
OP_CMPANY	  .equ (OP_CMP + OP_CMPE)

; this mask selects which operations set invalid when receiving a QNaN
OP_QNAN		  .equ (OP_CMPE + OP_LT + OP_LE + OP_GT + OP_GE)

__f_cmp:
        move.l   #OP_CMP,d0
        bra       f_compare
__f_cmpe:
        move.l   #OP_CMPE,d0
        bra       f_compare
__f_feq:
        moveq.l   #OP_EQ,d0
        bra       f_compare
__f_fne:
        moveq.l   #OP_NE,d0
        bra       f_compare
__f_flt:
        moveq.l   #OP_LT,d0
        bra       f_compare
__f_fle:
        moveq.l   #OP_LE,d0
        bra       f_compare
__f_fgt:
        moveq.l   #OP_GT,d0
        bra       f_compare
__f_fge:
        move.l   #OP_GE,d0
        bra       f_compare
__f_fun:
        moveq.l   #OP_UN,d0
        bra       f_compare
__f_for:
        moveq.l   #OP_OR,d0

f_compare:
        link.w    a6,#Frame_ff2f_localsize
	movem.l	  (Frame_ff2f_x,a6),d1/a0   ; d0=x, a0=y
        move.l	  #0x7fffffff,d2	    ; is it NaN ?
        and.l	  d1,d2
        cmpi.l	  #0x7f800000,d2
        bhi	  @x_NaN
        tst.l	  d1			    ; if x is negative, negate
        bpl	  @x_positive
        neg.l	  d2
@x_positive:        			    ; x is in d2

        move.l	  a0,d1
        andi.l	  #0x7fffffff,d1	    ; is it NaN ?
        cmpi.l	  #0x7f800000,d1
        bhi	  @y_NaN
        tst.l	  a0			    ; if y is negative, negate
        bpl	  @y_positive
        neg.l	  d1
@y_positive:        			    ; y is in d1

	cmp.l	  d1,d2			    ; x > y ?
	bgt	  @greater
	blt	  @less

;;;;;;;;;;;;;;;;;; case #0: ==
	andi.l	  #OP_1EQ,d0		    ; set 1 if necessary
	sne.b	  d0
	neg.l	  d0
	extb.l	  d0
	unlk	  a6
	rts

;;;;;;;;;;;;;;;;;; case #1: <
@less:
	andi.l	  #OP_1LT,d0		    ; set 1 if necessary
	sne.b	  d0
	neg.l	  d0
	extb.l	  d0
	unlk	  a6
	rts

;;;;;;;;;;;;;;;;;; case #2: >
@greater:
	move.l	  d0,d2
	andi.l	  #OP_CMPANY,d2		    ; is it a cmp?
	bne	  @greater_cmp
	andi.l	  #OP_1GT,d0		    ; set 1 if necessary
	sne.b	  d0
	neg.l	  d0
	extb.l	  d0
	unlk	  a6
	rts
@greater_cmp:
	moveq.l	  #2,d0
	unlk	  a6
	rts

;;;;;;;;;;;;;;;;;; case #3: NaN

@x_NaN:
	move.l	  d0,d2
	andi.l	  #OP_CMPANY,d2		    ; is it a cmp?
	bne	  @xnan_cmp
	.ifdef FENV_SUPPORT
	move.l	  d0,d2
	.endif
	andi.l	  #OP_1NAN,d0		    ; set 1 if necessary
	sne.b	  d0
	neg.l	  d0
	extb.l	  d0
	.ifdef FENV_SUPPORT
	btst.l	  #22,d1		    ; signaling NaN?
	beq	  @set_invalid
	andi.l	  #OP_QNAN,d2		    ; always set invalid?
	beq	  @xnan_check_y
@set_invalid:
	LoadFPSCR d1,a1
        bset      #INVALID_BIT,d1
        StoreFPSCR d1,a1
	.endif
	unlk	  a6
	rts
@xnan_cmp:
	.ifdef FENV_SUPPORT
	move.l	  d0,d2
	.endif
	moveq.l	  #3,d0
	.ifdef FENV_SUPPORT
	andi.l	  #OP_QNAN,d2		    ; always set invalid?
	bne	  @set_invalid
	btst.l	  #22,d1		    ; signaling NaN?
	beq	  @set_invalid
@xnan_check_y:
        move.l    a0,d1			    ; check whether y is SNaN
        andi.l	  #0x7fffffff,d1
        cmpi.l	  #0x7f800000,d1	    ; is it a NaN?
        bls	  @return
	btst.l	  #22,d1		    ; signaling NaN?
	beq	  @set_invalid
        .endif
@return:
	unlk	  a6
	rts

;;;;;;;;;;;;;;;;;;

@y_NaN:
	move.l	  d0,d2
	andi.l	  #OP_CMPANY,d2		    ; is it a cmp?
	bne	  @ynan_cmp
	.ifdef FENV_SUPPORT
	move.l	  d0,d2
	.endif
	andi.l	  #OP_1NAN,d0		    ; set 1 if necessary
	sne.b	  d0
	neg.l	  d0
	extb.l	  d0
	.ifdef FENV_SUPPORT
	btst.l	  #22,d1		    ; signaling NaN?
	beq	  @set_invalid
	andi.l	  #OP_QNAN,d2		    ; always set invalid?
	bne	  @set_invalid
	.endif
	unlk	  a6
	rts
@ynan_cmp:
	.ifdef FENV_SUPPORT
	move.l	  d0,d2
	.endif
	moveq.l	  #3,d0
	.ifdef FENV_SUPPORT
	andi.l	  #OP_QNAN,d2		    ; always set invalid?
	bne	  @set_invalid
	btst.l	  #22,d1		    ; signaling NaN?
	beq	  @set_invalid
        .endif
	unlk	  a6
	rts
