;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: fdiv.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Single precision division function


	.include ColdFire_sfpe.pre.s

	.weak	__f_div
	
	.public __fp_f_overflow
	.public __fp_f_underflow
	
	.ifdef FENV_SUPPORT
	.public __fp_f_directed_rounding
	.endif

	.ifdef NO_HARDWARE_DIVIDE
	.public __fp_PA_divu16
	.endif

        .text

;===============================================================================
; FUNCTION: __f_div
; DESCRIPTION: Float divide.
;
; INPUT: x(a6) = x, y(a6) = y, floats
; OUTPUT: d0 = result of x / y


__f_div:
; Compute (-1)^x.sign * 2^x.exp-bias * 1.xxxx divided by
; (-1)^y.sign * 2^y.exp-bias * 1.yyyy --->
; result.sign		= xor of x and y signs
; result.exp (biased)	= x.exp - y.exp + bias
; result.bits		= w.zzzzz before normalization, rounding, etc.
; If the leading w bit is 0, then must decrement the exponent by 1
; and realign.

; REGISTER ALLOCATION:
;   d0 = x (aligned as 0x7fffff80)
;   d1 = y (aligned as 0xffffff00)
;   a0.hi = result.exponent;
;   a1.31 = result sign.

        link.w    a6,#Frame_ff2f_localsize

	movem.l	  (Frame_ff2f_x,a6),d0-d1   ;d0=x, d1=y
	move.l	  d1,d2
	eor.l	  d0,d2
	and.l	  #0x80000000,d2
	movea.l	  d2,a1			; a1.31 = x.sign ^ y.sign

	and.l	  #0x7fffffff,d0	; classify first number
	beq	  @x_zero		; 0.0 / x?
	cmpi.l	  #0x7f800000,d0
	bhs	  @x_MaxExp		; NaN or Inf / x?

	and.l	  #0x7fffffff,d1	; classify 2nd number
	beq	  @division_by_zero	; x / 0.0 ?
	cmpi.l	  #0x7f800000,d1
	bhs	  @y_MaxExp		; x / NaN or Inf?

	lea       -12(a7),a7
	movem.l   d3-d5,(a7)		; SAVE d3-d5

	move.l	  d0,d2			; isolate exponent of x
	andi.l	  #0x7f800000,d2
	beq	  @x_denorm		; x is denormalized
	lsr.l	  #7,d2			; don't need to shift the exponent all the 23 positions!
	lsl.l	  #8,d0			; align x to 0x7fffff80
	lsr.l     #1,d0
	ori.l	  #0x40000000,d0	; add implicit 1
@x_denorm_return:

	move.l	  d1,d3			; isolate exponent of y
	andi.l	  #0x7f800000,d3
	beq	  @y_denorm		; x is denormalized
	lsr.l	  #7,d3			; don't need to shift the exponent all the 23 positions!
	lsl.l	  #8,d1			; align y to 0xffffff00
	ori.l	  #0x80000000,d1	; add implicit 1
@y_denorm_return:

	sub.l	  d3,d2			; exp = x.exp - y.exp + bias - 1
	addi.l	  #0x7e0000,d2
	movea.l	  d2,a0			; save exp in a0.hi

; Divide d1 into d0:00 leaving remainder in d0 and quotient in d2.
;
; Have to divide d1 into d0-d2 to produce a quotient in d2, possibly
; unnormalized by one bit, and a remainder (sticky bits) in d0.
; Restriction to 32/16-->16 division reduces the problem to that
; of double division using divu.l, i.e. 2-digit division,
; where in this case the digits are 16 bits wide.
;
; In terms of 16-bit digits, the division is just
;
;            X Y
;       --------
;  A B | C D 0 0.
;
; Knuth, Vol. 2, explains how it goes.  First, A is normalized
; because its leading bit is one.  Next, the algorithms are designed
; so that C is at most equal to A, in which case the special guess
; 0xffff is made for X.  The large-scale divide is set up so that
; in the very first step, C is guaranteed less than A
;
; Knuth shows that the guess for X is at worst 2 units too high, so
; we watch for adjustment.
;
;            X ?
;       --------
;  A B | C D 0 0.
;        X*A B
;          E F 0
;
; All that's known is that, after X is corrected, AB < EF.
; It's possible for E to be = A, in which case the guess for Y
; is the maximum "digit", in this case 0xffff.  Correcting Y
; as before yields an unnormalized quotient (by 0 or 1 bit) and
; a remainder (that contributes to the sticky bits).
;
; Anyone reading this who wants some interesting examples using 8-bit
; digits is invited to try
;
; 7f000000 / 80ff  -- 0xfe is the guess for X, and it's the full 2
;	units too large.
;
; 7ffc0000 / 80ff  -- the guesses for X (0xff) and Y (0xff) are both
;	2 units too large.
;
; 77e00000 / 80f1  -- the guess for X (0xef) is two units too large,
;	and then the guess for Y must be forced to be 0xff.
;	The trick is that the quotient is 0xedff with a large remainder,
;	0x72f1.  It suggests that one looks for sample cases of the form
;	80f? * ????? = abcd00 + epsilon.  The value abcd00 is constrained
;	by the preconditioning rule. The "epsilon" suggests that when
;	it's truncated, there'll be a large remainder after the first
;	quotient digit is established.  The divisor form 80f? is prone
;	to overestimation of the quotient digits.
;
        swap      d1                     ;     d1.w = lead "digit" A
.ifndef NO_HARDWARE_DIVIDE
        divu.w    d1,d0                  ;  d0.w = quo, d0.hi = rem
.else
	move.l    d0,-(a7)
	move.w    d1,-(a7)
	clr.w     -(a7)
	jsr       __fp_PA_divu16
	bvs.b     @_PA_4359_
	move.l    4(a7),d0
@_PA_4359_:
	lea       8(a7),a7
.endif

; No overflow since C is less than A in the first step

        swap      d1                    ;     restore AB
        move.w    d0,d3                 ;     quo guess
	clr.w	  d0			;     remainder from first small div
	swap      d0

; Here's what's happened so far
;                        d3.l        <-- quotient may be 2 units too high
;            +--------------------
;  d1.h d1.l | d0.h d0.l 0000 0000
;              xxxx xxxx
;             ----------
;              0000 d0.l 0000 0000   <-- partial partial remainder
; Now must compute true partial remainder by subtracting
;           -  0000 d5.l d2.h 0000   <-- <d5.l d2.h> = B * X, d5.h = d2.l = 0
;             ---------------------
;                   d0.l d2.h d2.l   <-- partial partial rem, might have borrowed
        move.w    d1,d2                  ;     B
        mulu.w    d3,d2                  ;     B * X
        swap      d2
        moveq.l   #0,d5
        move.w    d2,d5                  ;     d5 = 0000 <high part of product>
        clr.w	  d2			 ;     d2 = <low part of product> 0000
	neg.l     d2
        subx.l    d5,d0                  ;  trial partial remainder, possibly borrowing...
        bcc       @divu_1st_post_fixup
; If the subtraction to produce the partial remainder borrows, add back aligned
; AB and decrement X, once or twice.
;             ---------------------
;                   d0.l d2.h d2.l   <-- partial partial rem, might have borrowed
;            + 0000   A    B  0000   <-- <d4.l d5.h> = <d1.h d1.l> = A B
;            ---------------------
        subq.l	  #1,d3			;     decrement X
        move.l    d1,d5
        swap      d5                    ;     align for split
        moveq.l   #0,d4
        move.w    d5,d4                 ;     0000 A
        clr.w	  d5			;     B 0000
        add.l     d5,d2
        addx.l    d4,d0
        bcs       @divu_1st_post_fixup  ;     if carries out, then reconciled the borrow
	subq.l    #1,d3			;     decrement X
        add.l     d5,d2
        addx.l    d4,d0
@divu_1st_post_fixup:
        swap      d3                    ;     make room for next quo digit in d3.w
; Here's how far we've gotten.
;                        d3.h ????
;            +--------------------
;  d1.h d1.l | d0.h d0.l 0000 0000.
;             -xxxx xxxx xxxx
;             --------------------
;                   d0.l d2.h d2.l  <-- d0.h = 0
; Must guess second quotient digit by aligning d0.l and d2.h
        move.l    d2,d4
        move.w    d0,d4
        swap      d4                     ;     <d0.l d2.h>
        swap      d1                     ;     isolate A
.ifndef NO_HARDWARE_DIVIDE
        divu.w    d1,d4
.else
	move.l    d4,-(a7)
	move.w    d1,-(a7)
	clr.w     -(a7)
	jsr       __fp_PA_divu16
	bvs.b     @_PA_4418_
	move.l    4(a7),d4
@_PA_4418_:
	lea       8(a7),a7
.endif
        bvs       @divu_2nd_overflow
        swap      d1                     ;     restore A B
        move.w    d4,d3                  ;     guess for Y
        move.w    d2,d4                  ;     partial partial remainder
        move.l    d4,d2
        moveq.l   #0,d0
@divu_2nd_return_overflow:
; Here's the situation after the guess of the next quo digit.
;                        d3.h d3.l <-- quotient may be 2 units too high
;            +--------------------
;  d1.h d1.l | d0.h d0.l 0000 0000.
;             -xxxx xxxx xxxx
;             --------------------
;                   d0.l d2.h d2.l
;                 - xxxx xxxx
;                 ----------------
;                   d0.l d2.h d2.l  <-- d0.h = 0, d0.l = small, at most
; Compute the full partial remainder by multiplying B and Y
        moveq.l   #0,d4
        move.w    d1,d5
        mulu.w    d3,d5
        sub.l     d5,d2
        subx.l    d4,d0
        bcc       @divu_2nd_post_fixup
; If there's a borrow add the divisor back once or twice. Note d4 = 0.
;              -------------------
;              d0.h d0.l d2.h d2.l  <-- borrowed
;            + 0000 0000 d1.h d1.l
;           ----------------------  <-- will this carry out and cancel borrow?
	subq.l    #1,d3
        add.l     d1,d2
        bcs       @divu_2nd_post_fixup
	subq.l    #1,d3
        add.l     d1,d2
@divu_2nd_post_fixup:
	; Got the quotient in d3 and remainder in d2
	move.l	  a0,d1			; get exp + bias - 1 in d1.hi
	tst.l	  d3			; first quotient digit may be 0
	bmi	  @quotient_aligned
	lsl.l	  #1,d3
	subi.l	  #0x00010000,d1	; align and decrement exponent
@quotient_aligned:
	tst.l	  d1			; min exponent: -126
	blt	  @underflow
	cmpi.l	  #0x00fe0000,d1	; max exponent: +127
	bge	  @overflow

	move.l	  d3,d0			; pack mantissa
	lsr.l	  #8,d0
	lsl.l	  #7,d1			; pack exponent
	add.l	  d1,d0			; add exp+bias-1 to the implicit "1" bit
	move.l	  a1,d1			; pack sign bit
	or.l	  d1,d0

	.ifdef FENV_SUPPORT
	move.l	  d2,d1			; check if any rounding
	sne.b	  d1			; remainder <> 0 ?
	or.l	  d3,d1			; last 8 bits of d3 are rounding bits
	extb.l	  d1
	beq	  @restore_return

        LoadFPSCR d1,a1			 ; puts state bits in d1
        bset      #INEXACT_BIT,d1
        btst      #DIRECTED_BIT,d1
        bne       @directed_rounding
        btst      #CHOP_OR_UP_BIT,d1     ; toward 0 --> truncate
        bne       @endround
        .endif
        btst      #7,d3	                 ;  test round bit
        beq       @endround	         ;  round bit is 0 --> truncate
        andi.l	  #0x0000007f,d3	 ;  check sticky bits
        or.l	  d2,d3
        bne       @roundup               ;  nonzero stickies force roundup
        btst      #0,d0                  ;  Halfway case: check LSB
        beq       @endround              ;  is LSB nonzero?
@roundup:
        addq.l    #1,d0			 ; bump lsb
        .ifdef FENV_SUPPORT
	move.l	  #0x807fffff,d2	 ; did we overflow? check for maxexp
	or.l	  d0,d2			 ; we got only 1s in case of maxexp
	not.l	  d2
	beq	  @signal_overflow
	.endif
@endround:
	.ifdef FENV_SUPPORT
        StoreFPSCR d1,a1
        .endif
@restore_return:
	movem.l   (a7),d3-d5		;  RESTORE d3-d5
	lea       12(a7),a7
	unlk	  a6
        rts

;;;;;;;;;;;;;; Code for exceptional cases


@divu_2nd_overflow:
	; Again, set up trial quo digit 0xffff and compute partial remainder.
        swap      d1                     ;     restore A B
        move.w    #$ffff,d3
        clr.w	  d4			 ;     Compute CD - C * (2^16-1) == C + D
        clr.l	  d0
        add.l	  d4,d2
        addx.l	  d0,d0
        bra       @divu_2nd_return_overflow

;;;;;;;;;;;;;;


@x_zero:
	andi.l	  #0x7fffffff,d1
	beq	  @invalid_op		; 0 / 0 is invalid
	cmpi.l	  #0x7f800000,d1
	bhi	  @y_NaN		; NaN ? return NaN
	move.l	  a1,d0			; return signed 0
	unlk	  a6
	rts

;;;;;;;;;;;;;;


@y_NaN:
	cmpi.l	  #0x7fc00000,d1	; signaling? return Invalid
	blo	  @invalid_op
	move.l	  d1,d0			; return 2nd NaN
	unlk	  a6
	rts

;;;;;;;;;;;;;;


@x_MaxExp:
	beq	  @x_Inf		; last instruction was cmpi.l #0x7f800000,d0
	cmpi.l	  #0x7fc00000,d0	; signaling? return Invalid
	blo	  @invalid_op
	andi.l	  #0x7fffffff,d1
	cmpi.l	  #0x7f800000,d1	; is y NaN?
	bls	  @return_d0
	cmpi.l	  #0x7fc00000,d1	; y signaling NaN? return Invalid
	blo	  @invalid_op
@return_d0:
	unlk	  a6
	rts

;;;;;;;;;;;;;;


@x_Inf:
	andi.l	  #0x7fffffff,d1
	cmpi.l	  #0x7f800000,d1
	beq	  @invalid_op		; inf / inf is invalid;
	bhi	  @y_NaN		; is y NaN? return y
	move.l	  a1,d1			; return signed inf
	or.l	  d1,d0
	unlk	  a6
	rts

;;;;;;;;;;;;;;


@division_by_zero:
	.ifdef FENV_SUPPORT
        LoadFPSCR d2,a0
        bset      #DIVBYZERO_BIT,d2
        StoreFPSCR d2,a0
        .endif
        move.l	  a1,d0			; return signed infinity
        ori.l	  #0x7f800000,d0
        unlk	  a6
        rts

;;;;;;;;;;;;;;


@y_MaxExp:
	bne	  @y_NaN		; last instruction was cmpi.l #0x7f800000,d1
	move.l	  a1,d0			; x / inf = 0
	unlk	  a6
	rts

;;;;;;;;;;;;;;


@x_denorm:
	lsl.l	  #8,d0			; left align number (9 bits+)
	lsl.l	  #1,d0
	bmi	  @x_aligned
@x_align:
	subi.l	  #0x10000,d2		; expecting the biased exponent in d2.hi
	lsl.l	  #1,d0			; align
	bpl	  @x_align
@x_aligned:
	lsr.l	  #1,d0			; align number to 7fffff80
	bra	  @x_denorm_return

;;;;;;;;;;;;;;


@y_denorm:
	lsl.l	  #8,d1			; left align number (9 bits+)
	lsl.l	  #1,d1
	bmi	  @y_aligned
@y_align:
	subi.l	  #0x10000,d3		; expecting the biased exponent in d3.hi
	lsl.l	  #1,d1			; align
	bpl	  @y_align
@y_aligned:
	bra	  @y_denorm_return

;;;;;;;;;;;;;;


@overflow:
	move.l	  a1,d0			; sign bit
	movem.l   (a7),d3-d5		;  RESTORE d3-d5
	lea       12(a7),a7
	bra	  __fp_f_overflow

;;;;;;;;;;;;;;

@invalid_op:
; Signal invalid and return the default NaN.
      	.ifdef FENV_SUPPORT
        LoadFPSCR d2,a1
        bset      #INVALID_BIT,d2
        StoreFPSCR d2,a1
	.endif
	move.l	  #0x7fc00000,d0
	unlk	  a6
	rts

@underflow:
	cmpi.l	  #0xffe80000,d1	; min exponent: -149. d1.hi = (exp+bias)-1
	blt	  @true_underflow
	beq	  @verytiny

	subi.l	  #0x00080000,d1	; # of shifts: 9 - (exp+bias)
	neg.l	  d1			; compute number of mantissa shifts
	swap	  d1
	move.l	  d3,d4			; save rounding bits in d4
	lsr.l	  d1,d3			; denormalize left aligned number in d3
	move.l	  a1,d0			; merge the result with sign bit
	or.l	  d3,d0

	; compute rounding + sticky bits in d1
	moveq.l	  #32,d3		; move chopped d1 bits so that rounding bit
	sub.l	  d1,d3			; goes into position #31
	lsl.l	  d3,d4			; now d4 contains only chopped bits
	clr.l	  d1
	tst.l	  d2			; d2 contains the remainder
	sne.b	  d1			; any remainder? set last byte of d1
	or.l	  d4,d1			; add left aligned chopped bits

	movem.l   (a7),d3-d5		;  RESTORE d3-d5
	lea       12(a7),a7

	.ifdef FENV_SUPPORT
	tst.l	  d1
	beq	  @vtiny_return		; quit if no rounding

        LoadFPSCR d2,a1		; puts state bits in d2
        ori.l	  #UNDERFLOW_OR_INEXACT_MASK,d2
        btst      #DIRECTED_BIT,d2
        bne       __fp_f_directed_rounding
        btst      #CHOP_OR_UP_BIT,d2    ; toward 0 --> truncate
        bne       @vtiny_endround
        .endif
        tst.l	  d1			;  test round bit
        bpl       @vtiny_endround       ;  round bit is 0 --> truncate
        lsl.l	  #1,d1			;  check other bits
        bne       @vtiny_roundup              ;  nonzero stickies force roundup
        btst      #0,d0                 ;  Halfway case: check LSB
        beq       @vtiny_endround       ;  is LSB nonzero?
	bra	  @vtiny_roundup

;;;;;;;;;;;;;;


@true_underflow:
	move.l	  a1,d0			; return signed 0
	movem.l   (a7),d3-d5		;  RESTORE d3-d5
	lea       12(a7),a7
	bra	  __fp_f_underflow

;;;;;;;;;;;;;;


@verytiny:
	; exponent: -150; the result may round up to the smallest denorm
	move.l    #0x7fffffff,d1
	and.l	  d3,d1			; get rounding bits in d1
	or.l      d2,d1			; any stickies from the remainder
	move.l	  a1,d0			; return signed 0

	movem.l   (a7),d3-d5		;  RESTORE d3-d5
	lea       12(a7),a7

	.ifdef FENV_SUPPORT
        LoadFPSCR d2,a1		; puts state bits in d2
        ori.l	  #UNDERFLOW_OR_INEXACT_MASK,d2
        btst      #DIRECTED_BIT,d2
        bne       __fp_f_directed_rounding
        btst      #CHOP_OR_UP_BIT,d2    ; toward 0 --> truncate
        bne       @vtiny_endround
        .endif
        tst.l	  d1			; any rounding bits?
	beq	  @vtiny_endround	; no rounding? write fpscr and quit
@vtiny_roundup:
        addq.l    #1,d0			; bump lsb
@vtiny_endround:
	.ifdef FENV_SUPPORT
        StoreFPSCR d2,a1
        .endif
@vtiny_return:
        unlk	  a6
        rts

;;;;;;;;;;;;;;

.ifdef FENV_SUPPORT

@directed_rounding:
	move.l	  d1,d2			; content of fpscr
	movem.l   (a7),d3-d5		;  RESTORE d3-d5
	lea       12(a7),a7
	bra	  __fp_f_directed_rounding


@signal_overflow:
	bset	  #OVERFLOW_BIT,d1	; have the content of fpscr in d1
        StoreFPSCR d1,a1
	movem.l   (a7),d3-d5		;  RESTORE d3-d5
	lea       12(a7),a7
        unlk	  a6
        rts
.endif
