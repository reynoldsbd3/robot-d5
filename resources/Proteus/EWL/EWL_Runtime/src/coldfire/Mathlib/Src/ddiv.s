;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: ddiv.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Double precision division function

	.include ColdFire_sfpe.pre.s

	.weak	__d_div
	
	.public __fp_d_rounding
	.public __fp_d_underflow
	.public __fp_d_overflow
	
	.ifdef FENV_SUPPORT
	.public __fp_d_directed_rounding
	.endif


	.ifdef NO_HARDWARE_DIVIDE
	.public __fp_PA_divu16
	.endif

        .text


;===============================================================================
; FUNCTION: __d_div
; DESCRIPTION: Double divide.
;
; INPUT: pres(a6) = ptr to result, xhi-xlo(a6) = double, yhi-ylo(a6) = double
; OUTPUT: a0 = pres(a6) = address of result of x / y


__d_div:
; Compute (-1)^x.sign * 2^x.exp-bias * 1.xxxx divided by
; (-1)^y.sign * 2^y.exp-bias * 1.yyyy --->
; result.sign		= xor of x and y signs
; result.exp (biased)	= x.exp - y.exp + bias
; result.bits		= w.zzzzz before normalization, rounding, etc.
; If the leading w bit is 0, then must decrement the exponent by 1
; and realign.


        link.w    a6,#Frame_dd2d_localsize
        lea	  -28(a7),a7
        movem.l	  d3-d7/a2-a3,(a7)	    ; SAVE all registers
        movem.l   (Frame_dd2d_xhi,a6),d0-d3 ; d0:d1 = x, d2:d3 = y

	move.l	  d2,d4
	eor.l	  d0,d4
	and.l	  #0x80000000,d4
	movea.l	  d4,a1			; a1.31 = x.sign ^ y.sign

	and.l	  #0x7fffffff,d0	; classify first number
	move.l	  d0,d4
	or.l	  d1,d4
	beq	  @x_zero		; 0.0 / x?
	cmpi.l	  #0x7ff00000,d0
	bhs	  @x_MaxExp		; NaN or Inf / x?

	and.l	  #0x7fffffff,d2	; classify 2nd number
	move.l	  d2,d4
	or.l	  d3,d4
	beq	  @division_by_zero	; x / 0.0 ?
	cmpi.l	  #0x7ff00000,d2
	bhs	  @y_MaxExp		; x / NaN or Inf?

	move.l	  d0,d4			; isolate exponent of x
	andi.l	  #0x7ff00000,d4
	beq	  @x_denorm		; x is denormalized
	lsr.l	  #4,d4			; don't need to shift the exponent all the 23 positions!
	moveq.l	  #12,d7
	lsl.l	  d7,d0			; align x to 0x7fff ffff ffff fC00
	lsr.l     #2,d0
	moveq.l	  #10,d7
	move.l	  d1,d6
	lsl.l	  d7,d1
	moveq.l	  #22,d7
	lsr.l	  d7,d6
	or.l	  d6,d0
	ori.l	  #0x40000000,d0	; add implicit 1
@x_denorm_return:

	move.l	  d2,d5			; isolate exponent of y
	andi.l	  #0x7ff00000,d5
	beq	  @y_denorm		; x is denormalized
	lsr.l	  #4,d5			; don't need to shift the exponent all the 23 positions!
	moveq.l	  #11,d7
	lsl.l	  d7,d2			; align y to ffff ffff ffff f800
	move.l	  d3,d6
	lsl.l	  d7,d3
	moveq.l	  #21,d7
	lsr.l	  d7,d6
	or.l	  d6,d2
	ori.l	  #0x80000000,d2	; add implicit 1
@y_denorm_return:

	sub.l	  d5,d4			; exp = x.exp - y.exp + bias - 1
	addi.l	  #0x3fe0000,d4
	movea.l	  d4,a0			; save exp in a0.hi

;
;	128 bit / 64 bit unsigned division routine using 32/16 divu.w
;
;       INPUT: x = d0:d1:0:0 / y = d2:d3
;       OUTPUT: quotient = d5:d4, remainder = d0:d1
;       PRECONDITIONS: MSB of x == 0; MSB of y == 1
;
;       Algorithm (adapted from Knuth, Vol. 2): division using 16-bit "digits"
;       (Note: Digits of X are X(n)X(n-1)...X(0); of Y : Y(3)...Y(0) )
;
;          Repeat for each quotient digit Qi ( from Q(3) to Q(0) )
;             Qi := [X(n)X(n-1)] / Y(3)		 # guess quotient digit ~Qi
;                                                # according to Knuth, Qi <= ~Qi <= Qi+2
;             if Qi >= 0x10000 then Qi := 0xFFFF
;             X := X -  Y * (Qi << 16*i)         # compute partial remainder X
;             if X < 0 then Qi := Qi-1; X := X + (Y << 16*i)
;             if X < 0 then Qi := Qi-1; X := X + (Y << 16*i)
;
;       Registers used: d0-d7, a2-a3
;       X = d0:d1 (only the top 64 bits of X are stored at any moment)
;       Y = d2:d3
;       Q = d4:?? (iterations #1-2), a3:d4 (iterations #3-4)
;	a2 = indicates partial remainder overflow
;       d5,d6,d7 are scratch registers
;
	suba.l	  a3,a3			; clear a3,d4 - use it to detect end of loop
	clr.l	  d4
@divu_loop:
;
;	Qi := [X(n)X(n-1)] / Y(3)
;
        swap.w	  d2
.ifndef NO_HARDWARE_DIVIDE
        divu.w    d2,d0                 ;  d0.w = quo, d0.hi = rem
.else
	move.l    d0,-(a7)
	move.w    d2,-(a7)
	clr.w     -(a7)
	jsr       __fp_PA_divu16
	bvs.b     @_PA_4372_
	move.l    4(a7),d0
@_PA_4372_:
	lea       8(a7),a7
.endif
;       if Qi >= 0x10000 then Qi := 0xFFFF
        bvs       @divu_overflow
	move.w	  d0,d4
        swap.w	  d1
        move.w	  d1,d0			; d0 = [ Ri X(n-2) ]
        clr.w	  d1			; d1 = [ X(n-1) 0 ]
        suba.l	  a2,a2			; a2 = 0 (partial remainder <= 64-bits)
@divu_return_overflow:
	swap.w	  d2
;       X := X -  Y * (Qi << 16*i)
;          = [Ri X(n-2)..X(0)] - [Y(2)Y(1)Y(0)] * (Qi << 16*i)

	move.l	  d4,d5			; d5 = Y(0) * Qi
	mulu.w	  d3,d5

	move.l	  d3,d6			; d7:d6 = (Y(1) * Qi) << 16
	swap.w	  d6
	mulu.w	  d4,d6
	swap.w	  d6
	clr.l	  d7
	move.w	  d6,d7
	clr.w	  d6

	add.l	  d6,d5			; d7:d5 = [Y(1)Y(0)] * Qi
	clr.l	  d6
	addx.l	  d6,d7

	move.l	  d4,d6			; d7:d5 = [Y(2)Y(1)Y(0)] * Qi
	mulu.w	  d2,d6
	add.l	  d6,d7

	move.l	  a2,d6			; d6 == -1 ? X = 1:d1:d0 else X= 0:d1:d0
	sub.l	  d5,d1			; X := X - (Y * Qi)
	subx.l	  d7,d0
	negx.l	  d6			; Jump if d6==-1 or not carry
        bpl       @divu_post_fixup
;
;       { if X < 0 then Qi := Qi-1; X := X + (Y << 16*i) } - 2 times
;
        subq.l	  #1,d4
        add.l	  d3,d1
        addx.l	  d2,d0
        bcs       @divu_post_fixup  ;     if carries out, then reconciled the borrow
        subq.l	  #1,d4
        add.l	  d3,d1
        addx.l	  d2,d0
@divu_post_fixup:
  	move.l	  a3,d5
	cmpi.l	  #0x40000000,d5
	bhs	  @divu_finished    ; is the quotient complete? return it in d5:d4
	swap.w	  d5		    ; make room for the next quotient digit
	swap.w	  d4		    ; a3:d4 <<= 16
	move.w	  d4,d5
	movea.l	  d5,a3
	bra	  @divu_loop
@divu_finished:
	; Got the quotient in a3:d4 and remainder in d0:d1
	move.l	  a0,d7			; get exp + bias - 1 in d7.hi
	move.l	  a3,d3			; first quotient digit may be 0
	bmi	  @quotient_aligned
	lsl.l	  #1,d3
	clr.l	  d5
	lsl.l	  #1,d4
	addx.l	  d5,d3
	subi.l	  #0x00010000,d7	; align and decrement exponent
@quotient_aligned:

	clr.l	  d6
	or.l	  d0,d1
	sne.b	  d6			; 'or' stickies with the last 11 bits of d4
	or.l	  d6,d4			; (the rounding bits)

	move.l	  d7,d1			; min exponent: -1022
	blt	  @underflow
	cmpi.l	  #0x07fe0000,d1	; max exponent: +1023
	bge	  @overflow

	move.l	  d3,d0			; pack mantissa
	move.l	  d4,d2			; save stickies

	moveq.l	  #11,d7		; shift mantissa right 11 bits (2-word shift)
	lsr.l	  d7,d0
	lsr.l	  d7,d2
	lsl.l	  #4,d1			; pack exponent
	add.l	  d1,d0			; add exp+bias-1 to the implicit "1" bit
	move.l	  a1,d1			; pack sign bit
	or.l	  d1,d0			; upper word is ready

	moveq.l	  #21,d7
	lsl.l	  d7,d3			; make lower word
	move.l	  d2,d1
	or.l	  d3,d1			; lower word is ready

	lsl.l	  d7,d4
	movea.l	  d4,a0			; save rounding bits

        movem.l	  (a7),d3-d7/a2-a3	; RESTORE all registers
        lea	  28(a7),a7
        bra       __fp_d_rounding

;;;;;;;;;;;;;; Overflows during 128 : 64 division

; The overflows appear when X(n) == Y(3);
;       if Qi >= 0x10000 then
;            Qi := 0xFFFF;
;            Ri := X(n)X(n-1) - X(n) * (2^16-1)
;                = X(n) + X(n-1)

@divu_overflow:
; IN: d0 = [X(n)X(n-1)] (AB), d1 = [X(n-2)X(n-3)] (CD)
; OUT: d0 = [Ri.lo X(n-2)] (A+B|C), d1 = X(n-3) << 16 (D0), a2 = -1 if A+B carries

	move.w	  #0xffff,d4
	swap.w	  d1		    ; d1 = DC
	move.w	  d0,d5
	swap.w	  d5
	clr.w	  d5		    ; d5 = B0
	move.w	  d1,d0		    ; d0 = AC
	clr.w	  d1		    ; d1 = D0
	add.l	  d5,d0		    ; d0 = A+B|C
	scs.b	  d6
	extb.l	  d6
	movea.l	  d6,a2		    ; A+B carries? A2 := -1
	bra	  @divu_return_overflow

;;;;;;;;;;;;;; Special cases


@x_zero:
	andi.l	  #0x7fffffff,d2
	move.l	  d3,d4
	or.l	  d2,d4
	beq	  @invalid_op		; 0 / 0 is invalid
	cmpi.l	  #0x7ff00000,d2
	bhi	  @y_NaN		; NaN ? return NaN
	blo	  @x_zero_return
	tst.l	  d3
	bne	  @y_NaN
@x_zero_return:
        movem.l	  (a7),d3-d7/a2-a3	; RESTORE all registers
        lea	  28(a7),a7
	move.l	  a1,d0			; return signed 0
	clr.l	  d1
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
	unlk	  a6
	rts


;;;;;;;;;;;;;;


@y_MaxExp:
	bne	  @y_NaN		; last instruction was cmpi.l #0x7ff00000,d2
	tst.l	  d3
	bne	  @y_NaN
        movem.l	  (a7),d3-d7/a2-a3	; RESTORE all registers
        lea	  28(a7),a7
	move.l	  a1,d0			; x / inf = 0
	clr.l	  d1
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
	unlk	  a6
	rts

;;;;;;;;;;;;;;


@y_NaN:
	btst.l	  #19,d2		; signaling? return Invalid
	beq	  @invalid_op
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d2-d3,(a0)		; return 2nd NaN
        movem.l	  (a7),d3-d7/a2-a3	; RESTORE all registers
        lea	  28(a7),a7
	unlk	  a6
	rts

;;;;;;;;;;;;;;


@x_MaxExp:
	bne	  @x_NaN		; last instruction was cmpi.l #0x7ff00000,d0
	tst.l	  d1
	beq	  @x_Inf
@x_NaN:
	btst.l	  #19,d0		; signaling? return Invalid
	beq	  @invalid_op
	andi.l	  #0x7fffffff,d2
	cmpi.l	  #0x7ff00000,d2	; is y NaN?
	blo	  @return_d0_d1
	bhi	  @x_QNaN_y_NaN
	tst.l	  d3
	beq	  @return_d0_d1
@x_QNaN_y_NaN:
	btst.l	  #19,d2		; y signaling NaN? return Invalid
	beq	  @invalid_op
@return_d0_d1:
        movem.l	  (a7),d3-d7/a2-a3	; RESTORE all registers
        lea	  28(a7),a7
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
	unlk	  a6
	rts

;;;;;;;;;;;;;;


@x_Inf:
	andi.l	  #0x7fffffff,d2
	cmpi.l	  #0x7ff00000,d2
	blo	  @x_Inf_y_normal
	bhi	  @y_NaN		; is y NaN? return y
	tst.l	  d3
	beq	  @invalid_op		; inf / inf is invalid;
	bra	  @y_NaN
@x_Inf_y_normal:
        movem.l	  (a7),d3-d7/a2-a3	; RESTORE all registers
        lea	  28(a7),a7
	move.l	  a1,d2			; return signed inf
	or.l	  d2,d0
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
	unlk	  a6
	rts

;;;;;;;;;;;;;;


@invalid_op:
; Signal invalid and return the default NaN.
; COMPATIBILITY: 68K documentation says that the default float
; NaN is 0x7fffffff, regardless of the intended result sign.
        movem.l	  (a7),d3-d7/a2-a3	; RESTORE all registers
        lea	  28(a7),a7
      	.ifdef FENV_SUPPORT
        LoadFPSCR d2,a0
        bset      #INVALID_BIT,d2
        StoreFPSCR d2,a0
	.endif
        movea.l   (Frame_dd2d_pres,a6),a0
	move.l	  #0x7ff80000,d0
	clr.l	  d1
        movem.l	  d0-d1,(a0)
	unlk	  a6
	rts

;;;;;;;;;;;;;;


@division_by_zero:
        movem.l	  (a7),d3-d7/a2-a3	; RESTORE all registers
        lea	  28(a7),a7
	.ifdef FENV_SUPPORT
        LoadFPSCR d2,a0
        bset      #DIVBYZERO_BIT,d2
        StoreFPSCR d2,a0
        .endif
        move.l	  a1,d0			; return signed infinity
        ori.l	  #0x7ff00000,d0
        clr.l	  d1
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
        unlk	  a6
        rts

;;;;;;;;;;;;;;


@x_denorm:
	moveq.l	  #11,d4		; shift counter for the lower bits
	moveq.l	  #12,d7
	lsl.l	  d7,d0			; left align number (12 bits+)
	beq	  @x_denorm_low
	bmi	  @x_aligned
@x_align:
	addq.l	  #1,d4			; compute total # of shifts
	lsl.l	  #1,d0			; align
	bpl	  @x_align
@x_aligned:
	lsr.l	  #1,d0			; align hi word to 7fff f800
	move.l	  d1,d7
	lsl.l	  d4,d1			; shift the lower word
	subi.l	  #32,d4		; shift bits from lo to hi word
	neg.l	  d4
	lsr.l	  d4,d7
	or.l	  d7,d0

	swap.w	  d4			; d6 == 21 + biased exponent. Adjust d4.hi
	subi.l	  #0x150000,d4
	bra	  @x_denorm_return


@x_denorm_low:
	move.l	  #0xffec0000,d4	; biased exponent -20 in d4.hi
	tst.l	  d1			; the number is in the lower word
	bmi	  @x_aligned_low
@x_align_low:
	subi.l	  #0x10000,d4		; compute the exponent
	lsl.l	  #1,d1			; align
	bpl	  @x_align_low
@x_aligned_low:
	move.l	  d1,d0
	lsr.l	  #1,d0			; align number to 7fff ffff 8000 0000
	moveq.l	  #31,d7
	lsl.l	  d7,d1
	bra	  @x_denorm_return

;;;;;;;;;;;;;;


@y_denorm:
	moveq.l	  #12,d5		; shift counter for the lower bits
	lsl.l	  d5,d2			; left align number (12 bits+)
	beq	  @y_denorm_low
	bmi	  @y_aligned
@y_align:
	addq.l	  #1,d5			; compute total # of shifts
	lsl.l	  #1,d2			; align
	bpl	  @y_align
@y_aligned:
	move.l	  d3,d7
	lsl.l	  d5,d3			; shift the lower word
	subi.l	  #32,d5		; shift bits from lo to hi word
	neg.l	  d5
	lsr.l	  d5,d7
	or.l	  d7,d2

	swap.w	  d5			; d6 == 20 + biased exponent. Adjust d5.hi
	subi.l	  #0x140000,d5
	bra	  @y_denorm_return


@y_denorm_low:
	move.l	  #0xffec0000,d5	; biased exponent -20 in d5.hi
	tst.l	  d3			; the number is in the lower word
	bmi	  @y_aligned_low
@y_align_low:
	subi.l	  #0x10000,d5		; compute the exponent
	lsl.l	  #1,d3			; align
	bpl	  @y_align_low
@y_aligned_low:
	move.l	  d3,d2
	clr.l	  d3
	bra	  @y_denorm_return

;;;;;;;;;;;;;;


@underflow:
	cmpi.l	  #0xffcb0000,d1	; min exponent: -1074. d1.hi = (exp+bias)-1
	blt	  @true_underflow
	beq	  @verytiny

	subi.l	  #0x000B0000,d1	; # of shifts: 12 - (exp+bias)
	neg.l	  d1			; compute number of mantissa shifts
	swap	  d1
	cmpi.l	  #32,d1
	blt	  @underflow_64bit	; is hi word 0?
	beq       @underflow_copy       ; have exactly 32 shifts, copy hi word into low word
	subi.l	  #32,d1

	move.l	  d3,d5			; d5:d3 = d3:0 >> d1
	lsr.l	  d1,d5
	subi.l	  #32,d1
	neg.l	  d1
	lsl.l	  d1,d3

	clr.l	  d2			; add sticky bits from lower word to d3
	tst.l	  d4
	sne.b	  d2
	or.l	  d2,d3

	move.l	  a1,d0			; put sign and prepare result in d0:d1
	move.l	  d5,d1
	movea.l	  d3,a0			; save rounding bits
	bra	  @underflow_round


@underflow_copy:
        move.l    a1,d0                 ; result in d0:d1
        move.l    d3,d1
        movea.l   d4,a0                 ; rounding bits
	bra	  @underflow_round


@underflow_64bit:
	move.l	  d3,d0			; d3:d4:d2 =  d3:d4:0 >> d1
	move.l	  d4,d2
	lsr.l	  d1,d3
	lsr.l	  d1,d4
	subi.l	  #32,d1
	neg.l	  d1
	lsl.l	  d1,d0
	lsl.l	  d1,d2
	or.l	  d0,d4

	move.l	  a1,d0			; put sign bit
	or.l	  d3,d0
	move.l	  d4,d1			; prepare result in d0:d1
	move.l	  d2,a0			; and save rounding bits in a0

@underflow_round:
        movem.l	  (a7),d3-d7/a2-a3	; RESTORE all registers
        lea	  28(a7),a7

	.ifdef FENV_SUPPORT
	tst.l	  a0
	beq	  @underflow_return	; quit if no rounding

        LoadFPSCR d2,a1		; puts state bits in d2
        ori.l	  #UNDERFLOW_OR_INEXACT_MASK,d2
        StoreFPSCR d2,a1
        btst      #DIRECTED_BIT,d2
        bne       __fp_d_directed_rounding
        btst      #CHOP_OR_UP_BIT,d2    ; toward 0 --> truncate
        bne       @underflow_return
        .endif
        tst.l	  a0			;  test round bit
        bpl       @underflow_return     ;  round bit is 0 --> truncate
        adda.l	  a0,a0			;  shift left to check the other bits
        tst.l	  a0
        bne       @underflow_roundup    ;  nonzero stickies force roundup
        btst      #0,d1                 ;  Halfway case: check LSB
        beq       @underflow_return     ;  is LSB nonzero?
@underflow_roundup:
        clr.l     d2
        addq.l    #1,d1
        addx.l    d2,d0
@underflow_return:
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
        unlk	  a6
        rts

;;;;;;;;;;;;;;


@true_underflow:
	move.l	  a1,d0			; return signed 0
        movem.l	  (a7),d3-d7/a2-a3	; RESTORE all registers
        lea	  28(a7),a7
        bra	  __fp_d_underflow

;;;;;;;;;;;;;;


@verytiny:
	; exponent: -1075; the result may round up to the smallest denorm
	move.l    #0x7fffffff,d1
	and.l	  d3,d1			; get rounding bits in d1
	or.l	  d4,d1			; and lower word
	move.l	  d1,a0			; save stickies
	move.l	  a1,d0			; return signed 0
	clr.l	  d1

        movem.l	  (a7),d3-d7/a2-a3	; RESTORE all registers
        lea	  28(a7),a7

	.ifdef FENV_SUPPORT
        LoadFPSCR d2,a1		; puts state bits in d2
        ori.l	  #UNDERFLOW_OR_INEXACT_MASK,d2
        btst      #DIRECTED_BIT,d2
        bne       __fp_d_directed_rounding
        btst      #CHOP_OR_UP_BIT,d2    ; toward 0 --> truncate
        bne       @vtiny_endround
        .endif
        tst.l	  a0			; any rounding bits?
	beq	  @vtiny_endround	; no rounding? write fpscr and quit
@vtiny_roundup:
        addq.l    #1,d1			; bump lsb
@vtiny_endround:
	.ifdef FENV_SUPPORT
        StoreFPSCR d2,a1
        .endif
@vtiny_return:
        movea.l   (Frame_dd2d_pres,a6),a0
        movem.l	  d0-d1,(a0)
        unlk	  a6
        rts

;;;;;;;;;;;;;;


@overflow:
	move.l	  a1,d0			; sign bit
        movem.l	  (a7),d3-d7/a2-a3	; RESTORE all registers
        lea	  28(a7),a7
	bra	  __fp_d_overflow




