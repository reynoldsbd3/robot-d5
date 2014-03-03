;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: dcompare.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Double precision comparison functions

	.include ColdFire_sfpe.pre.s

	.weak	__d_cmp
	.weak	__d_cmpe
	.weak	__d_feq
	.weak	__d_fne
	.weak	__d_flt
	.weak	__d_fle
	.weak	__d_fgt
	.weak	__d_fge
	.weak	__d_fun
	.weak	__d_for

        .text

;===============================================================================
; FUNCTION: __d_cmp, __d_cmpe
;	__d_feq, __d_fne, __d_flt, __d_fgt, __d_fge, __d_fle
;	__d_fun, __d_for
; DESCRIPTION: Comparison functions, all described here because of commonality.
;
; INPUT: xhi-xlo(a6) = double, yhi-ylo(a6) = double
; OUTPUT: d0 = result code:
;	__d_cmp  --> 0-eq, 1-lt, 2-gt, 3-un (invalid if an SNaN)
;	__d_cmpe --> 0-eq, 1-lt, 2-gt, 3-un (invalid if unordered)
;	__d_fXX   --> 0-relation XX is false, 1-relation XX is true
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


;===============================================================================
; FUNCTION ENTRY POINTS: __d_cmp, __d_cmpe
;	__d_feq, __d_fne, __d_flt, __d_fgt, __d_fge, __d_fle
;	__d_fun, __d_for
; DESCRIPTION: Comparison functions, described under __f_fxxx
;

__d_cmp:
        move.l   #OP_CMP,d0
        bra       d_compare
__d_cmpe:
        move.l   #OP_CMPE,d0
        bra       d_compare
__d_feq:
        moveq.l   #OP_EQ,d0
        bra       d_compare
__d_fne:
        moveq.l   #OP_NE,d0
        bra       d_compare
__d_flt:
        moveq.l   #OP_LT,d0
        bra       d_compare
__d_fle:
        moveq.l   #OP_LE,d0
        bra       d_compare
__d_fgt:
        moveq.l   #OP_GT,d0
        bra       d_compare
__d_fge:
        move.l   #OP_GE,d0
        bra       d_compare
__d_fun:
        moveq.l   #OP_UN,d0
        bra       d_compare
__d_for:
        moveq.l   #OP_OR,d0

d_compare:
        link.w    a6,#Frame_dd2f_localsize
        movem.l   (Frame_dd2f_xhi,a6),d1-d2/a0-a1 ; d1:d2 = x, a0-a1=y
	lea       -8(a7),a7
	movem.l   d3-d4,(a7)		    ; SAVE d3-d4
        move.l	  #0x7fffffff,d3	    ; is x NaN ?
        and.l	  d1,d3
        cmpi.l	  #0x7ff00000,d3
        bhi	  @x_NaN
        blo	  @x_ordered
        tst.l	  d2			    ; Hi word is 0. check lower word.
        bne	  @x_NaN
@x_ordered:
        tst.l	  d1			    ; if x is negative, negate
        bpl	  @x_positive
        neg.l	  d2
        negx.l	  d3
@x_positive:        			    ; x is in d3:d2

        move.l	  a0,d1
        andi.l	  #0x7fffffff,d1	    ; is y NaN ?
        cmpi.l	  #0x7ff00000,d1
        bhi	  @y_NaN
        blo	  @y_ordered
        tst.l	  a1
        bne	  @y_NaN
@y_ordered:
        move.l	  a1,d4
        tst.l	  a0			    ; if y is negative, negate
        bpl	  @y_positive
        neg.l	  d4
        negx.l	  d1
@y_positive:        			    ; y is in d1:d4

	sub.l	  d4,d2
	subx.l	  d1,d3			    ; x - y > 0 ?
	bgt	  @greater
	blt	  @less

;;;;;;;;;;;;;;;;;; case #0: ==
	movem.l   (a7),d3-d4		    ;  RESTORE d3-d4
	lea       8(a7),a7
	andi.l	  #OP_1EQ,d0		    ; set 1 if necessary
	sne.b	  d0
	neg.l	  d0
	extb.l	  d0
	unlk	  a6
	rts

;;;;;;;;;;;;;;;;;; case #1: <
@less:
	movem.l   (a7),d3-d4		    ;  RESTORE d3-d4
	lea       8(a7),a7
	andi.l	  #OP_1LT,d0		    ; set 1 if necessary
	sne.b	  d0
	neg.l	  d0
	extb.l	  d0
	unlk	  a6
	rts

;;;;;;;;;;;;;;;;;; case #2: >
@greater:
	movem.l   (a7),d3-d4		    ;  RESTORE d3-d4
	lea       8(a7),a7
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
	movem.l   (a7),d3-d4		    ;  RESTORE d3-d4
	lea       8(a7),a7
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
	btst.l	  #19,d1		    ; signaling NaN?
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
	btst.l	  #19,d1		    ; is x signaling NaN?
	beq	  @set_invalid
@xnan_check_y:
	move.l	  a0,d1
        andi.l	  #0x7fffffff,d1
        cmpi.l	  #0x7ff00000,d1	    ; is it a NaN?
        blo	  @return
        bhi	  @xnan_y_is_nan
        tst.l	  a1
        beq	  @return
@xnan_y_is_nan:
	btst.l	  #19,d1		    ; is y signaling NaN?
	beq	  @set_invalid
        .endif
@return:
	unlk	  a6
	rts

;;;;;;;;;;;;;;;;;;

@y_NaN:
	movem.l   (a7),d3-d4		    ;  RESTORE d3-d4
	lea       8(a7),a7
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
	btst.l	  #19,d1		    ; signaling NaN?
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
	btst.l	  #19,d1		    ; signaling NaN?
	beq	  @set_invalid
        .endif
	unlk	  a6
	rts

