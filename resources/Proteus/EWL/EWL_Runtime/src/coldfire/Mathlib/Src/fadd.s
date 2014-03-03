;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: fadd.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Single precision addition and subtraction functions

	.include ColdFire_sfpe.pre.s

        .text

	.weak	__f_add
	.weak	__f_sub
		
	.ifdef FENV_SUPPORT
	.public __fp_f_overflow
	.public __fp_f_directed_rounding
	.endif

; Add and subtract share common code.  Subtract is identical to add
; but with y's sign flipped.
;
; Compute (-1)^x.sign * 2^x.exp-bias * 1.xxxx plus/minus
; (-1)^y.sign * 2^y.exp-bias * 1.yyyy --->
; result.sign		= sign of larger operand, though in case
;	of zero result from operands of different signs, the result
;	sign depends on the rounding mode: + for all except toward -INF
; result.exp (biased)	= larger of two exps, subject to normalization
; result.bits		= rounded sum or difference
;
; Many SPECIAL CASES apply:
; Subnorms need not be normalized -- just adjust their exp to 1 and be
;	sure not to artificially set the implicit bit to 1.
; Magnitude add is benign. The worst that can happen is a 1-bit carry-out,
;	and even when the operands are aligned there can be rounding from
;	the Round (1st) bit.
; Magnitude subtract when exps are equal can lead to massive cancellation,
;	but never a rounding error.
; Magnitude subtract when exps differ by one can lead to massive cancellation,
;	and possible rounding from Round bit.
; Magnitude subtract when exps differ by more than one never suffers
;	cancellation by more than 1 bit, and requires general rounding.
; Tiny results can arise from the sum or difference of subnormals, or
;	from the difference of normals.  Must be careful when realigning
;	after cancellation to be ready to back off to subnormal.  The results
;	are always exact (i.e. no bit can lie to the right of the LSB of
;	a subnormal.
; The benign nature of subnormals (tinies) in add/sub means some care must
;	be taken in packing the result back up to fudge the exponent
;	(compare mul/div, where tiny results can be trickier).

D_END_STACK_FRAME:	.macro
		;clear stack frame, return should be in d0
        movea.l   (Frame_dd2d_pres,a6),a0
        move.l    d0,(a0)
        move.l    d1,(4,a0)
	movem.l   (a7),d3-d7
	lea       20(a7),a7
        unlk      a6
        rts

.endm

D_START_STACK_FRAME:	.macro
	 	;start stack frame
	link.w    a6,#Frame_dd2d_localsize
	lea       -20(a7),a7
	movem.l   d3-d7,(a7)
.endm



SMALL_END_STACK_FRAME:	.macro
		;clear stack frame, return should be in d0
	movem.l   (a7),d3-d5
	lea       12(a7),a7
        unlk      a6
        rts
.endm

SMALL_START_STACK_FRAME:	.macro
	 	;start stack frame
        link.w    a6,#Frame_ff2f_localsize
	lea       -12(a7),a7
	movem.l   d3-d5,(a7)
.endm


;===============================================================================
; FUNCTION: __f_add, __fsub
; DESCRIPTION: Float add/subtract.
;
; INPUT: x(a6) = x, y(a6) = y, floats
; OUTPUT: d0 = result of x + y / x - y

__f_sub:
	SMALL_START_STACK_FRAME
	movem.l	(Frame_ff2f_x,a6),d0-d1
	eor.l	#$80000000,d1		;flip y's sign
	bra FADD_COMMON			;branch to add routine
__f_add:
		;Set-up the stack frame and save all non-volatile data registers
	SMALL_START_STACK_FRAME
		;save x and y parameters (received through stack)
	movem.l (Frame_ff2f_x,a6),d0-d1	 ;d0=x, d1=y

FADD_COMMON:
		;save original operands
	movea.l	d0,a0		;save x original in a0
	movea.l d1,a1		;save y original in a1
		;keep the xor of signs
	move.l	d0,d3
	eor.l	d1,d3		;we have the xor of sign in most significant bit, junk in rest
		;We need a lot of shifting with >8 positions, so will use a register
	moveq.l	#23,d4		;d4 = 23 positions to shift

		;Split y
	move.l	a1,d2		;d2=y
	andi.l	#$007FFFFF,d2	;d2 = yMantisa
	lsr.l	d4,d1		;d1 = yExponent = y with right aligned exponent
	andi.l	#$FF,d1		;clear sign bit, keep only exponent
	bz	FADD_y_Zero_Or_Denorm	;branch if y denorm or zero
	cmpi.l  #$FF,d1		;test if exponent is maxim
	beq	FADD_yExponent_Max_01	;branch if exponent is maxim
	ori.l	#$00800000,d2		;put hidden 1 bit of mantisa
FADD_yMantisa_Aligned:

		;Split x
	andi.l	#$007FFFFF,d0	;d0 = xMantisa
	move.l	a0,d5		;d5 = x
	lsr.l	d4,d5		;d5 = (x>>23) ,with right aligned (sign-bit,exponent)
	andi.l	#$FF,d5		;clear sign bit, keep only exponent
	bz	FADD_x_Zero_Or_Denorm	;branch if denorm or zero
	cmpi.l  #$FF,d5		;test if exponent is maxim
	beq	FADD_xExponent_Max_02	;branch if exponent is maxim
	ori.l	#$00800000,d0		;put into mantisa hidden 1 bit
FADD_xMantisa_Aligned:

		;here we have in (d5,d0) (xExponent,xMantisa with eventual hidden bit set)
		;(d1,d2) will be (yExponent,yMantisa with eventual hidden bit set)
		;d3 has in msbit the xor of signs

	lsl.l	#1,d3		;test if we really have addition or substraction(difference in sign or not)
	bcs	FSUB_REALLY	;jump to substract subroutine (the sign of operands are different)


FADD_REALLY:		;if here, we really have addition (same sign of operands)
		;Suppose that y has a larger exponent
	move.l	a1,d3		;d3(sign and exponent of the result) = sign and exponent of y for now
	sub.l	d5,d1		;d1 = yExponent - xExponent =(number of positions to shift)
	beq	FADD_ALIGNED	;if equal, jump to routine for aligned
	bpl	FADD_NO_OPERAND_SWITCH
	neg.l	d1	;if negative, it means xExponent > yExponent, so
	move.l	d0,d3	;make positive the number of positions to shift
	move.l	d2,d0	;exchange d0 with d2 trashing d3 whose value is not needed
	move.l	d3,d2
	move.l	a0,d3	;d3(sign and exponent of the result) = sign of x now
FADD_NO_OPERAND_SWITCH:
		;here we have in d2 the mantisa of larger number, d0 mantisa of smaller number,
		;in d1 number of position to shift smaller mantisa,in d3 sign and exponent of the result
	cmpi.l	#24,d1		;compare difference in exponents
	bgt	FADD_SHIFT25	;if here, the result mantisa is d2, d0 are all stickies
		;if here , 24 >= number of shifts >= 1
	move.l	d0,d5		;d5 = mantisa of smaller number
	lsr.l	d1,d0		;d0 = d0 >> (exponent difference)
	add.l	d2,d0		;add d0 mantisa correctly shifted to d2 and store result in d0
	neg.l	d1		;prepare number of shifts for computing 32-nr.shifts by negating nr.shifts
	addi.l	#32,d1		;add 31 to (-nr.shifts) = 32 - nr.shifts
	lsl.l	d1,d5		;d5 = d5 << (32- exponent difference ) = RoundBit And Stickies aligned at msb
	btst.l	#24,d0		;test bit 25(24 counting from zero) (we might have carry from mantisa addition)
	bz	FADD_NOEXTRASHIFT
	lsr.l	#1,d5	;make room for another bit in extra bits register
	lsr.l	#1,d0	;shift right d0
	bcc	@FADD_NOEXTRASHIFT_Round0	;test if extra bit was 0
	ori.l	#$80000000,d5	;put 1 in most significant bit position since shifted bit was 1
@FADD_NOEXTRASHIFT_Round0:
	addi.l	#$00800000,d3	;increment result exponent
FADD_NOEXTRASHIFT:

	;------------ Do Rounding -----------------
	.ifdef	FENV_SUPPORT
		;here we are in IEEE mode
		  ;pack result
	andi.l	#$007FFFFF,d0
	andi.l	#$FF800000,d3
	add.l	d3,d0

	lsl.l	#1,d3	;align left mantisa
	cmpi.l	#$FF000000,d3
	bhs	FADD_prep_overflow

	move.l	d5,d1	;copy d5 in a non-volatile register

	movem.l	(a7),d3-d5 ;restore registers
	lea.l	12(a7),a7



	tst.l	d1	;test if result exact
	bz	FADD_RESULT_EXACT_001	;if exact, no rounding needed

	      ;test which rounding mode
	LoadFPSCR d2,a1	;load flags
	      ;set inexact
	bset.l	#INEXACT_BIT,d2 ;inexact
	StoreFPSCR d2,a1	;store flags

	btst.l	#DIRECTED_BIT,d2
	bne	__fp_f_directed_rounding
	btst	#CHOP_OR_UP_BIT,d2     ; toward 0 --> truncate
	bne	FADD_ROUND_DOWN

	      ;here we have round to nearest
	lsl.l	#1,d1		 ;we have round bit in carry
	bcc	FADD_ROUND_DOWN ;round bit is zero, so round down

FADD_ROUNDBIT_ONE:	;result is truncated
		;test stickies
	tst.l	d1
	bnz		FADD_ROUNDUP
	  ;if here, we have a tie, so add to d0 it's lowest bit
	move.l	d0,d1	;copy result in d1
	andi.l	#1,d1	;keep only least significant bit
	add.l	d1,d0	;add to result

	move.l	d0,d1	;make copy of result
	lsl.l	#1,d1	;shift left to align exponent left

	cmpi.l	#$FF000000,d1	;test if overflow
	bhs	__fp_f_overflow

	unlk	a6	;return
	rts

FADD_ROUNDUP:	;here, we have to round up
	addi.l	#1,d0

	move.l	d0,d1		;copy result in d1
	lsl.l	#1,d1		;make exponent left aligned
	cmpi.l	#$FF000000,d1	;test if overflow
	bhs	__fp_f_overflow

	unlk	a6
	rts

FADD_ROUND_DOWN:
FADD_RESULT_EXACT_001:
	unlk	a6
	rts

FADD_prep_overflow:
	movem.l	(a7),d3-d5 ;restore registers
	lea.l	12(a7),a7
	bra	__fp_f_overflow

	.else
		;if here, non-IEEE mode, round to nearest
		  ;pack result
	andi.l	#$007FFFFF,d0
	andi.l	#$FF800000,d3
	add.l	d3,d0
		;test for overflow
	lsl.l	#1,d3		;align left exponent
	cmpi.l	#$FF000000,d3	;test if inf
	bhs	FADD_prep_overflow

	lsl.l	#1,d5		 ;we have round bit in carry
	bcc	FADD_ROUND_DOWN ;round bit is zero, so round down

FADD_ROUNDBIT_ONE:	;result is truncated
			;test stickies
	tst.l	d5
	bnz	FADD_ROUNDUP
	    ;if here, we have a tie, so add to d0 it's lowest bit
	move.l	d0,d2
	andi.l	#1,d2
	add.l	d2,d0
	move.l	d0,d1
	lsl.l	#1,d1
	cmpi.l	#$FF000000,d1	;test if overflow
	bls	@FADD_No_Overflow
	;if here, overflow
	andi.l	#$FF800000,d0	;keep sign and FF exponent
	@FADD_No_Overflow:
FADD_ROUND_DOWN:
	SMALL_END_STACK_FRAME

FADD_ROUNDUP:	;here, we have to round up
	addi.l	#1,d0

	move.l	d0,d2		;copy d0 in d2
	lsl.l	#1,d2		;make exponent left aligned
	cmpi.l	#$FF000000,d2	;test if overflow
	bls	@FADD_No_Overflow
	    ;if here, overflow
	andi.l	#$FF800000,d0	;keep sign and FF exponent
@FADD_No_Overflow:
	SMALL_END_STACK_FRAME

FADD_prep_overflow:
	and.l	#$FF800000,d0	;keep only sign and exponent FF
	SMALL_END_STACK_FRAME

	.endif

FADD_SHIFT25:		;we get here when the exponent difference is >24
			;if here, bigger number is normal,result may overflow
	.ifdef	FENV_SUPPORT
			;pack result
	move.l	d2,d0
	andi.l	#$007FFFFF,d0
	andi.l	#$FF800000,d3
	or.l	d3,d0

		;clear stack
	movem.l	(a7),d3-d5
	lea	12(a7),a7

		;here, we know that result is inexact
	LoadFPSCR d2,a1
	bset.l	#INEXACT_BIT,d2
	StoreFPSCR d2,a1

	btst.l	#DIRECTED_BIT,d2	;directed rounding depends on the sign
	bne	__fp_f_directed_rounding

	   ;if here, we have round to nearest or zero and with rounding bit 0 -> no rounding
	unlk	a6
	rts
	.else

	    ;In round to nearest, the result is the bigger number, which is a normal number
	move.l	d2,d0
	andi.l	#$007FFFFF,d0
	andi.l	#$FF800000,d3
	or.l	d3,d0
	SMALL_END_STACK_FRAME

	.endif

FADD_ALIGNED:		;add aligned when operand exponents are the same
				;here we might have denorm + denorm, or normal+normal
	add.l	d2,d0		;add the 2 mantisa
	cmpi.l	#$00FFFFFF,d0	;test if result is a normal number (if denorm+denorm)
	ble	FADD_RESULT_DENORM_EXACT

	.ifdef	FENV_SUPPORT
		;here for IEEE mode
	move.l	a0,d4		;we do an or with d3(sign,exp of y) because we might have the case
	or.l	d4,d3		;of norm with exp 1 and denorm which obtains exp 1, so to really keep 1 as exp, and not zero
	addi.l	#$00800000,d3	 ;increment final exponent with one because of the shift

	move.l	d3,d4
	lsl.l	#1,d4
	cmpi.l	#$FF000000,d4
	bhs	FADD_ALIGNED_overflow

	lsr.l	#1,d0	;we need to shift right one position because we surely have carry
	bcc	FADD_ALIGNED_RESULTEXACT
	      	;pack result in d0
	andi.l	#$007FFFFF,d0
	andi.l	#$FF800000,d3
	add.l	d3,d0
		;if here, we have roundbit 1 and stickies 0, so
		;test which rounding mode
	LoadFPSCR d2,a1	;load flags
		;set inexact
	bset.l	#INEXACT_BIT,d2 ;inexact
	StoreFPSCR d2,a1	;store flags

	movem.l	(a7),d3-d5	;regain allocated space on the stack
	lea.l	12(a7),a7

	btst.l	#DIRECTED_BIT,d2
	bne	__fp_f_directed_rounding
	btst	#CHOP_OR_UP_BIT,d2     ; toward 0 --> truncate
	bne	FADD_ROUND_DOWN_002


	move.l	d0,d2		;add less significant bit of d0 to d0
	andi.l	#1,d2
	add.l	d2,d0
		;test if overflow and if overflow, set inf as result
	move.l	d0,d2		;move sign+exponent of result in d2
	lsl.l	#1,d2		;get sign out, result exponent aligned left
	cmpi.l	#$FF000000,d2
	bhi	__fp_f_overflow

FADD_ROUND_DOWN_002:
	unlk	a6
	rts

FADD_ALIGNED_RESULTEXACT:
	andi.l	#$007FFFFF,d0
	andi.l	#$FF800000,d3
	add.l	d3,d0
	SMALL_END_STACK_FRAME

FADD_ALIGNED_overflow:
	andi.l	#$FF800000,d3
	move.l	d3,d0
	movem.l	(a7),d3-d5
	lea	12(a7),a7
	bra	__fp_f_overflow

	.else

	;here for NON-IEEE mode, just round to nearest
	move.l	a0,d4		;we do an or with d3(sign,exp of y) because we might have the case
	or.l	d4,d3		;of norm with exp 1 and denorm which obtains exp 1, so to really keep 1 as exp, and not zero
	addi.l	#$00800000,d3	 ;increment final exponent with one because of the shift
	lsr.l	#1,d0	;we need to shift right one position because we surely have carry
	bcc	FADD_ALIGNED_RESULTEXACT
			;if here, we have roundbit 1 and stickies 0, so add less significant bit to operand
        move.l	d0,d2		;add less significant bit of d0 to d0
        andi.l	#1,d2
        add.l	d2,d0
		;test if overflow and if overflow, set inf as result
        move.l	d3,d2		;move sign+exponent of result in d2
        lsl.l	#1,d2		;get sign out, result exponent aligned left
        cmpi.l	#$FF000000,d2
        bls	@FADD_ALIGNED_No_Overflow_01
	          ;if here, overflow
        andi.l	#$FF800000,d0	;keep sign and FF exponent
@FADD_ALIGNED_No_Overflow_01:

FADD_ALIGNED_RESULTEXACT:
	andi.l	#$007FFFFF,d0
	andi.l	#$FF800000,d3
	add.l	d3,d0

	move.l	d0,d2
	lsl.l	#1,d2
	cmpi.l	#$FF000000,d2
	bls	@FADD_ALIGNED_No_Overflow_02
	  ;if here, overflow
	andi.l	#$FF800000,d0	;keep sign and FF exponent
@FADD_ALIGNED_No_Overflow_02:

	SMALL_END_STACK_FRAME

	.endif

FADD_RESULT_DENORM_EXACT:	;here the result is a denorm, operands are denorms
	andi.l	#$80000000,d3	;keep only sign as exponent is already 0
	or.l		d3,d0		;put sign into result
	SMALL_END_STACK_FRAME

FSUB_REALLY:
		;Suppose that y has a larger exponent
	move.l	a1,d3		;d3(sign and exponent of the result) = sign and exponent of y for now
	sub.l	d5,d1		;d1 = yExponent - xExponent =(number of positions to shift)
	beq	FSUB_ALIGNED	;if equal, jump to routine for aligned
	bpl	FSUB_NO_OPERAND_SWITCH
	neg.l		d1	;if negative, it means xExponent > yExponent, so
	move.l	d0,d3	;make positive the number of positions to shift
	move.l	d2,d0	;exchange d0 with d2 trashing d3 whose value is not needed
	move.l	d3,d2
	move.l	a0,d3	;d3(sign and exponent of the result) = sign of x now

FSUB_NO_OPERAND_SWITCH:
		;here we have in d2 the mantisa of larger number, d0 mantisa of smaller number,
		;in d1 number of position to shift smaller mantisa,in d3 sign and exponent of the result
	cmpi.l	#24,d1		;compare difference in exponents
	bgt	FSUB_SHIFT25	;if here, the result mantisa is d2, d0 are all stickies
	cmpi.l	#1,d1		;test if exponent difference is 1, which is special since we can have massive cancellation
	beq	FSUB_SHIFT1
		;if here , 24 >= number of shifts >= 1
	move.l	d0,d5		;d5 = mantisa of smaller number
	lsr.l	d1,d0		;d0 = d0 >> (exponent difference )
	neg.l	d1		;prepare number of shifts for computing 32-nr.shifts by negating nr.shifts

	sub.l	d0,d2		;substract d0 mantisa correctly shifted from d2

	clr.l	d4		;we need a register to be zero
	addi.l	#32,d1		;add 31 to (-nr.shifts) = 32 - nr.shifts
	lsl.l	d1,d5		;d5 = d5 << (32- exponent difference ) = RoundBit And Stickies aligned at msb
	neg.l	d5		;we had a substraction, so negate round and stickies and substract carry from result
	subx.l	d4,d2		;substract from result mantisa carry (borrow in this case)

	btst.l	#23,d2		;test bit 24(23 counting from zero) (we might have to renormalize 1 position)
	bnz	FSUB_NOEXTRASHIFT	;branch if hidden mantisa 1 bit remained 1
	lsl.l		#1,d2	;make room in result for extra bit
	lsl.l		#1,d5	;shift left out of round and stickies the most significant bit
	addx.l	d4,d2	;put it into result (d4 = 0)

	subi.l	#$00800000,d3 ;decrement result exponent
FSUB_NOEXTRASHIFT:

	move.l	d2,d0		;put result in d0

	;------------ Do Rounding -----------------
	.ifdef	FENV_SUPPORT
		;here we are in IEEE mode
		  ;pack result
	andi.l	#$007FFFFF,d0
	andi.l	#$FF800000,d3
	add.l		d3,d0

	move.l	d5,d1	;copy d5 in a non-volatile register

	movem.l	(a7),d3-d5 ;restore registers
	lea.l		12(a7),a7

	tst.l	d1	;test if result exact
	bz	FSUB_RESULT_EXACT_001	;if exact, no rounding needed

	      ;test which rounding mode
	LoadFPSCR d2,a1	;load flags
	      ;set inexact
	bset.l	#INEXACT_BIT,d2 ;inexact
	StoreFPSCR d2,a1	;store flags

	btst.l	#DIRECTED_BIT,d2
	bne	__fp_f_directed_rounding
	btst	#CHOP_OR_UP_BIT,d2     ; toward 0 --> truncate
	bne	FSUB_ROUND_DOWN

	      ;here we have round to nearest
	lsl.l	#1,d1		 ;we have round bit in carry
	bcc	FSUB_ROUND_DOWN ;round bit is zero, so round down

FSUB_ROUNDBIT_ONE:	;result is truncated
		;test stickies
	tst.l	d1
	bnz	FSUB_ROUNDUP
		;if here, we have a tie, so add to d0 it's lowest bit
	move.l	d0,d1	;copy result in d1
	andi.l	#1,d1	;keep only least significant bit
	add.l	d1,d0	;add to result

	move.l	d0,d1	;make copy of result
	lsl.l	#1,d1	;shift left to align exponent left

	cmpi.l	#$FF000000,d1	;test if overflow
	bhi	__fp_f_overflow

	unlk	a6	;return
	rts

FSUB_ROUNDUP:	;here, we have to round up
	addi.l	#1,d0

	move.l	d0,d1		;copy result in d1
	lsl.l	#1,d1		;make exponent left aligned
	cmpi.l	#$FF000000,d1	;test if overflow
	bhi	__fp_f_overflow

	unlk	a6
	rts

FSUB_ROUND_DOWN:
FSUB_RESULT_EXACT_001:
	unlk	a6
	rts

	.else
		;if here, non-IEEE mode, round to nearest
		  ;pack result
	move.l	d2,d0
	andi.l	#$007FFFFF,d0
	andi.l	#$FF800000,d3
	add.l	d3,d0

	lsl.l	#1,d5		 ;we have round bit in carry
	bcc	FADD_ROUND_DOWN ;round bit is zero, so round down

FSUB_ROUNDBIT_ONE:	;result is truncated
			;test stickies
	tst.l	d5
	bnz	FSUB_ROUNDUP
	;if here, we have a tie, so add to d0 it's lowest bit
	move.l	d0,d2
	andi.l	#1,d2
	add.l	d2,d0
	move.l	d0,d1
	lsl.l	#1,d1
	cmpi.l	#$FF000000,d1	;test if overflow
	bls	@FSUB_No_Overflow
	;if here, overflow
	andi.l	#$FF800000,d0	;keep sign and FF exponent

@FSUB_No_Overflow:
FSUB_ROUND_DOWN:
	SMALL_END_STACK_FRAME

FSUB_ROUNDUP:	;here, we have to round up
	addi.l	#1,d0

	move.l	d0,d2		;copy d0 in d2
	lsl.l	#1,d2		;make exponent left aligned
	cmpi.l	#$FF000000,d2	;test if overflow
	bls	@FSUB_No_Overflow
	;if here, overflow
	andi.l	#$FF800000,d0	;keep sign and FF exponent

@FSUB_No_Overflow:
	SMALL_END_STACK_FRAME

	.endif


FSUB_SHIFT25:		;we get here when the exponent difference is >24
				;if here, bigger number is normal,result may generate tiny, d2>d0
	.ifdef	FENV_SUPPORT

	;pack result
	cmp.l	#$00800000,d2	;we have to test the special case of 1.0 x 2^y - 0.01 ulp (round1, sticky 1)
	beq	FSUB_SHIFT25_SPECIALCASE

	subi.l	#1,d2		;if not, we substract 1 from d2 ; which we know it won't denormalize number

	move.l	d2,d0
	andi.l	#$007FFFFF,d0
	andi.l	#$FF800000,d3
	or.l	d3,d0

FSUB_SHIFT25_COMEBACK_FROM_SPECIAL:
	;clear stack
	movem.l	(a7),d3-d5
	lea	12(a7),a7

	;here, we know that result is inexact
	LoadFPSCR d2,a1
	bset.l	#INEXACT_BIT,d2
	StoreFPSCR d2,a1

	btst.l	#DIRECTED_BIT,d2	;directed rounding depends on the sign
	bne	__fp_f_directed_rounding
	btst.l	#CHOP_OR_UP_BIT,d2
	bne	FSUB_SHIFT25_ROUNDZERO
	;if here, we have round to nearest with rounding bit 1 and stickies 1, so round up which
	;equals the original number
	addi.l #1,d0		;shift up, since round is 1 and sticky is 1

FSUB_SHIFT25_ROUNDZERO:
	;if round to zero, just return
	unlk	 a6
	rts
        ;here we have

FSUB_SHIFT25_SPECIALCASE:
	;here, we have the special case expained at jump point
	;in d1 we have the difference in exponents, test if is exactly 25

	subi.l	#$00800000,d3	;sub 1 from exponent
	ori.l		#$007FFFFF,d3	;mantisa of 1.000000..00 - 00.00..001 = 7FFFFF
	move.l	d0,d2
	move.l	d3,d0
	cmpi.l	#25,d1
	bne	FSUB_SHIFT25_COMEBACK_FROM_SPECIAL

	    ;here, depends on smallest number, which is now in d2
	cmpi.l	#$00800000,d2	;compare with smallest normal number
	ble	FSUB_SHIFT25_COMEBACK_FROM_SPECIAL
	    ;if here, round bit is surely 0
	movem.l	(a7),d3-d5
	lea		12(a7),a7

	LoadFPSCR d2,a1
	bset.l	#INEXACT_BIT,d2
	StoreFPSCR d2,a1

	btst.l	#DIRECTED_BIT,d2	;directed rounding depends on the sign
	bne	__fp_f_directed_rounding
	    ;if round to nearest or chop, and we know round is zero, just return rezult
	unlk a6
	rts


	.else
		;In round to nearest, the result is the bigger number, which is a normal number
           cmp.l	#$00800000,d2	;we have to test the special case of 1.0 x 2^y - 0.01 ulp (round1, sticky 1)
	   beq	FSUB_SHIFT25_SPECIALCASE

	  FSUB_SHIFT25_COMEBACK_FROM_SPECIAL:
	   move.l	d2,d0
	   andi.l	#$007FFFFF,d0
	   andi.l	#$FF800000,d3
	   or.l	d3,d0
	   SMALL_END_STACK_FRAME

	  FSUB_SHIFT25_SPECIALCASE:
		;here, we have the special case expained at jump point
		;in d1 we have the difference in exponents, test if is exactly 25

	   cmpi.l	#25,d1				;test if shift number >25 (not equal with25)
	   bne	FSUB_SHIFT25_COMEBACK_FROM_SPECIAL

	   cmpi.l	#$00800000,d0	;compare with smallest normal number
	   ble	FSUB_SHIFT25_COMEBACK_FROM_SPECIAL

		;if here , round is zero, so keep the number - 1ulp

	   subi.l	#$00800000,d3	;sub 1 from exponent
	   ori.l	#$007FFFFF,d3	;mantisa of 1.000000..00 - 00.00..001 = 7FFFFF
	   move.l	d3,d0
	   SMALL_END_STACK_FRAME

	.endif

	FSUB_ALIGNED:		;add aligned when operand exponents are the same
				;here we might have denorm + denorm, or normal+normal
		;We will have no rounding, here result is always exact
	  cmp.l		d0,d2	;compare d2 with d0
	  bge		FSUB_ALIGNED_NO_SWITCH	;if d2 mantisa is greater or equal with d0 mantisa, no need to shift
		move.l	a0,d3	;put sign of d0 in d3
		move.l	d0,d4	;exchange d0-d2
		move.l	d2,d0
		move.l	d4,d2
	  FSUB_ALIGNED_NO_SWITCH:

	  sub.l		d0,d2		;substract d0 from d2
	  bz		FSUB_ALIGNED_RESULT_ZERO ;special case when result 0
	  cmpi.l	#$00FFFFFF,d2	;test if result didn't borrow(carry)
	  bgt		FSUB_RESULT_EXACT ;if greater, we have normal-denorm which remains correctly aligned

	  move.l	d3,d4
	  and.l		#$7F800000,d4	;keep in d4 only exponent
	  bz		FSUB_ALIGNED_DENORM ; if exponent of bigger number was 0, both were denorm , so branch

	  FSUB_ALIGNED_START_NORMALIZATION:
	    subi.l	#$00800000,d4		;substract 1 from exponent
	    bz		FSUB_ALIGNED_DENORM	;if exponent zero, the result is denorm
	    lsl.l	#1,d2
	    bclr.l	#23,d2		;test bit 24(23 counting from zero) and clear (because it is hidden 1)
	    bz		FSUB_ALIGNED_START_NORMALIZATION	;branch if result not aligned yet

		;here, result is a normal number, so pack
	    and.l	#$80000000,d3	;keep sign of result
	    or.l	d4,d3
	    or.l	d3,d2		;put is result exponent and sign (the hidden bit is already cleared)
	    move.l	d2,d0
	    SMALL_END_STACK_FRAME

	  FSUB_ALIGNED_DENORM:		;if here, result is denorm and correctly aligned, just return it
	    andi.l	#$80000000,d3
	    or.l	d3,d2
	    move.l	d2,d0		;put result in d0
	    SMALL_END_STACK_FRAME


	  FSUB_RESULT_EXACT:	;here the result is exact
	    andi.l	#$FF8000000,d3	;keep only sign and exponent
	    or.l	d3,d2		;put sign into result
	    move.l	d2,d0		;put result
	    SMALL_END_STACK_FRAME

	  FSUB_ALIGNED_RESULT_ZERO:
	  .ifdef	FENV_SUPPORT
		LoadFPSCR d2,a1
		andi.l	#$30,d2	;keep only rounding mode
		cmpi.l	#$20,d2
		bne	FSUB_ALIGNED_RESULT_PLUS
		  move.l  #$80000000,d0
		  SMALL_END_STACK_FRAME
		FSUB_ALIGNED_RESULT_PLUS:
	  .endif
		;in round to nearest, return +0 always
		clr.l	d0
		SMALL_END_STACK_FRAME


	FSUB_SHIFT1:
		;the numbers have a difference in exponents of 1 (so better shift both left with 1 and account for that)

	  lsl.l		#1,d2	;shift with one position larger number's mantisa
	  sub.l		d0,d2	;substract d0 from d2

	  .ifdef	FENV_SUPPORT
		  ;fully compliant IEEE rounding

		cmp.l	#$01000000,d2	;test if result is normalized
		bge	FSUB_SHIFT1_NORMALIZED
			;if here, we already shifted with 1 and have an exact result, just substract 1 from exponent

		  move.l	d3,d4
		  and.l		#$7F800000,d4	;keep in d4 only exponent
		  subi.l	#$00800000,d4	;exponent - 1 because we already shifted left 1 position

		  bclr.l	#23,d2
		  bnz		FSUB_SHIFT1_NoNormalizationRequired
		 FSUB_SHIFT1_START_NORMALIZATION:
		  subi.l	#$00800000,d4	;exponent=exponent-1
		  bz		FSUB_SHIFT1_DENORM	;if exponent zero, the result is denorm
		  lsl.l		#1,d2
		  bclr.l	#23,d2		;test bit 24(23 counting from zero) and clear (because it is hidden 1)
		  bz		FSUB_SHIFT1_START_NORMALIZATION	;branch if result not aligned yet

			;here, result is a normal number, so pack
		 FSUB_SHIFT1_NoNormalizationRequired:
		  and.l		#$80000000,d3	;keep sign of result
		  or.l		d4,d3
		  or.l		d3,d2	;put is result exponent and sign (the hidden bit is already cleared)
		  move.l	d2,d0
		  SMALL_END_STACK_FRAME

		 FSUB_SHIFT1_DENORM:	;if here, result is denorm and correctly aligned, just return it
		    andi.l	#$80000000,d3
		    or.l	d3,d2
		    move.l	d2,d0		;put result in d0
		    SMALL_END_STACK_FRAME


	        FSUB_SHIFT1_NORMALIZED:
			;if here, the result is a normalized number
			;we have 1 round bit and no stickies in this case, so test round bit
		  lsr.l	#1,d2	;put round bit in cc
		  bcc	FSUB_SHIFT1_RESULTEXACT
			;if here, round bit is 1, no stickies
		    move.l	d2,d0		;put result in d0
		    andi.l	#$007FFFFF,d0	;clear hidden 1 from mantisa
		    and.l	#$FF800000,d3	;keep only sign and exponent
		    or.l	d3,d0		;put sign and exponent in result

			;clear stack frame
		    movem.l	(a7),d3-d5
		    lea.l	12(a7),a7
			;load fpscr
		    LoadFPSCR d2,a1	;load flags
			;set inexact
		    bset.l	#INEXACT_BIT,d2 ;inexact
		    StoreFPSCR d2,a1	;store flags

		    btst.l	#DIRECTED_BIT,d2
		    bne		__fp_f_directed_rounding
		    btst.l	#CHOP_OR_UP_BIT,d2     ; toward 0 --> truncate
		    bne		FSUB_SHIFT1_ROUND_DOWN

			;here we have round to nearest, and carry is 1 and no stickies, so add less significant bit
			move.l	d0,d2	;make a copy of result mantisa
			andi.l	#1,d2	;keep only less significant bit
			add.l	d2,d0	;add least significant bit of result to result

	  	    FSUB_SHIFT1_ROUND_DOWN:  ;rounding to zero, so round down
			unlk	a6
			rts

		  FSUB_SHIFT1_RESULTEXACT:
				;if here, result is exact
		    andi.l	#$007FFFFF,d2	;clear hidden bit
		    andi.l	#$FF800000,d3	;keep only sign and mantisa of result
		    or.l	d3,d2		;pack
		    move.l	d2,d0		;put result in d0
		    SMALL_END_STACK_FRAME


	  .else
		  ;non-IEEE mode, round to nearest

		cmp.l	#$01000000,d2	;test if result is normalized
		bge	FSUB_SHIFT1_NORMALIZED
			;if here, we already shifted with 1 and have an exact result, just substract 1 from exponent

		  move.l	d3,d4
		  and.l		#$7F800000,d4	;keep in d4 only exponent
		  subi.l	#$00800000,d4	;exponent - 1 because we already shifted left 1 position

		  bclr.l	#23,d2
		  bnz		FSUB_SHIFT1_NoNormalizationRequired
		 FSUB_SHIFT1_START_NORMALIZATION:
		  subi.l	#$00800000,d4	;exponent=exponent-1
		  bz		FSUB_SHIFT1_DENORM	;if exponent zero, the result is denorm
		  lsl.l		#1,d2
		  bclr.l	#23,d2		;test bit 24(23 counting from zero) and clear (because it is hidden 1)
		  bz		FSUB_SHIFT1_START_NORMALIZATION	;branch if result not aligned yet

			;here, result is a normal number, so pack
		 FSUB_SHIFT1_NoNormalizationRequired:
		  and.l		#$80000000,d3	;keep sign of result
		  or.l		d4,d3
		  or.l		d3,d2	;put is result exponent and sign (the hidden bit is already cleared)
		  move.l	d2,d0
		  SMALL_END_STACK_FRAME

		 FSUB_SHIFT1_DENORM:	;if here, result is denorm and correctly aligned, just return it
		    andi.l	#$80000000,d3
		    or.l	d3,d2
		    move.l	d2,d0		;put result in d0
		    SMALL_END_STACK_FRAME


	        FSUB_SHIFT1_NORMALIZED:
			;if here, the result is a normalized number
			;we have 1 round bit and no stickies in this case, so test round bit
		  lsr.l	#1,d2	;put round bit in cc
		  bcc	FSUB_SHIFT1_RESULTEXACT
			;if here, round bit is 1, no stickies
		    move.l	d2,d0		;put result in d0
		    andi.l	#$007FFFFF,d0	;clear hidden 1 from mantisa
		    and.l	#$FF800000,d3	;keep only sign and exponent
		    or.l	d3,d0		;put sign and exponent in result

			;clear stack frame
		    movem.l	(a7),d3-d5
		    lea.l	12(a7),a7

			;here we have round to nearest, and carry is 1 and no stickies, so add less significant bit
		    move.l	d0,d2	;make a copy of result mantisa
		    andi.l	#1,d2	;keep only less significant bit
		    add.l	d2,d0	;add least significant bit of result to result

		    unlk	a6
		    rts

		  FSUB_SHIFT1_RESULTEXACT:
				;if here, result is exact
		    andi.l	#$007FFFFF,d2	;clear hidden bit
		    andi.l	#$FF800000,d3	;keep only sign and mantisa of result
		    or.l	d3,d2		;pack
		    move.l	d2,d0		;put result in d0
		    SMALL_END_STACK_FRAME

	  .endif


FADD_x_Zero_Or_Denorm:
	tst.l	d0		;test if xMantisa is zero
	bnz	@x_Not_Zero
		;if here, x is zero, and we know that y isn't zero, so return y
		move.l	a1,d0
		SMALL_END_STACK_FRAME
	@x_Not_Zero:
		;exponent is 1-bias, not 0-bias, so add 1 to exponent
	addi.l	#1,d5
	bra	FADD_xMantisa_Aligned

FADD_y_Zero_Or_Denorm:
	tst.l	d2		;test if mantisa = 0 (denorm or zero)
	bnz	FADD_y_Not_Zero
		;if here, y is zero, so rezult is x or test if x is 0 also
	        move.l	a0,d4	;put x into d4
	        lsl.l	#1,d4	;get sign out of d4
	        bnz	FADD_RETURN_IS_X
			;if here, x and y are zero
			;we have in d3 xor of signs
			lsl.l	#1,d3
			bcc	FADD_Zeroes_With_Same_Sign
				;here,signs differ (0 + (-0)) or (-0 + 0)
			.ifdef	FENV_SUPPORT
				;IEEE-mode, test if round to -inf (when sign is -),else sign is +
			  LoadFPSCR d2,a1
			  andi.l	#$00000030,d2
			  cmp.l		#$20,d2
			  bne		FADD_y_Plus_Zero	;if rounding not -inf return +0
			    ;if here, in -inf rounding mode, return -0
			    move.l	#$80000000,d0
			    SMALL_END_STACK_FRAME
			  FADD_y_Plus_Zero:
			.endif
				;non-IEEE mode, round to nearest
			  clr.l	d0	;result is +0
			  SMALL_END_STACK_FRAME

			FADD_Zeroes_With_Same_Sign:
			move.l	a0,d0
			SMALL_END_STACK_FRAME
	        FADD_RETURN_IS_X:
			;if here, y is zero and we return x, just test for x nan, snan
	          cmpi.l #$FF000000,d4	;test if exponent >=max (inf or nan case)
	          bhi	@FADD_TEST_Nan
	            move.l a0,d0
	            SMALL_END_STACK_FRAME
	          @FADD_TEST_Nan:
		    move.l #$7fc00000,d0
		    .ifdef	FENV_SUPPORT
		      and.l   #$00800000,d4	;test if snan
		      bnz	@FADD_INVALID_NOT_SET
		        LoadFPSCR d4,a1
		        bset.l	#INVALID_BIT,d4
		        StoreFPSCR d4,a1
		      @FADD_INVALID_NOT_SET:
		    .endif
		    SMALL_END_STACK_FRAME
	FADD_y_Not_Zero:
		;normalize yMantisa (d1), diminishing the exponent
		;exponent is 1-bias, not 0-bias, so add 1 to exponent
	addi.l	#1,d1
	bra	FADD_yMantisa_Aligned

FADD_yExponent_Max_01:	;yExponent is FF (Max) ?
			;yExponent is FF(Max)
		tst.l	d2		;since yExponent is Max, test if Mantisa is 0(nr. is Inf)
		beq	@FADD_y_Inf_01
				;If here, y is Nan
			andi.l	#$00400000,d2		;test if Nan is Signaling (First mantisa bit 0)
			bz	@FADD_y_Signaling_01
					;If here, y is qNan, but test if x is sNan
				move.l	a0,d0		;d0 = x
				andi.l	#$007FFFFF,d0	;test if mantisa 0 (if zero, it can not be neither qNan or sNan)
				bz	@FADD_x_Not_Snan

				move.l	a0,d0
				andi.l	#$7fC00000,d0	;test if x is sNan (exp. Max and first mantisa bit 0)
				cmpi.l	#$7f800000,d0

				beq	@FADD_y_Signaling_01	;if x = sNan, set invalid and return qNan
					;else return qNan

				@FADD_x_Not_Snan:
				move.l	#$7FC00000,d0		;RESULT (d0) = qNan
				SMALL_END_STACK_FRAME

			@FADD_y_Signaling_01:
					;If here,y is sNan
				.ifdef FENV_SUPPORT
				SET_INVALID_FLAG		;signal INVALID
				.endif
				move.l	#$7FC00000,d0		;RESULT (d0) = qNan
				SMALL_END_STACK_FRAME
		@FADD_y_Inf_01:
				;Test if x is Nan or Inf
			move	a0,d0
			andi.l	#$7F800000,d0		;keep x Exponent
			cmpi.l	#$7F800000,d0		;test if x has max exponent
			bne	@FADD_x_Not_Zero_Nan_Inf_01
					;x is Nan or Inf
				move.l	a0,d0		;d0 = x
				andi.l	#$007FFFFF,d0	;d0 = xMantisa
				bz	@FADD_x_Inf_01	;if xMantisa==0 , x is Inf and branch
					  ;here x is Nan
					andi.l	#$00400000,d0	;keep first xMantisa bit in d0
					bz	@FADD_x_Is_Signaling_01
						  ;here x is qNan
						move.l #$7FC00000,d0	;move a0(x=qNan) as result
						SMALL_END_STACK_FRAME
					@FADD_x_Is_Signaling_01:
						  ;here x is sNan
						.ifdef FENV_SUPPORT
						SET_INVALID_FLAG
						.endif
						move.l #$7fC00000,d0
						SMALL_END_STACK_FRAME
				@FADD_x_Inf_01:
					;test here if we have Inf-Inf (inf+(-inf)), which is invalid
				    move.l	a0,d0
				    move.l	a1,d1
				    eor.l	d1,d0
				    cmp.l	#$80000000,d0
				    bne		@FADD_x_Not_Zero_Nan_Inf_01
				     .ifdef FENV_SUPPORT
				     SET_INVALID_FLAG		;we have the addition of infinities with
				     .endif
				     move.l	#$7fC00000,d0	;oposite site, so signal invalid and return
				     SMALL_END_STACK_FRAME		;qNan
			@FADD_x_Not_Zero_Nan_Inf_01:
				 move.l	a1,d0	;y is infinity,x isn't inf or nan, so result is y
				SMALL_END_STACK_FRAME

FADD_xExponent_Max_02:	;branch if xExponent is not maxim (#$FF)
				;if here, xExponent is #$FF(Max)
		tst.l	d0	;since xExponent is Max, test if Mantisa is 0(nr. is Inf)
		beq	@FADD_x_Inf_02	;branch if mantisa is zero (x is Inf)
				;If here, x is qNan or sNan
			andi.l	#$00400000,d0		;test if Nan is Signaling (First mantisa bit 0)
			bz	@FADD_x_Signaling_02		;branch if signaling
					;If here, x=qNan and y<>sNan, so return  qNan
				move.l	#$7FC00000,d0	;RESULT (d0) = qNan
				SMALL_END_STACK_FRAME		;clear stack frame and restore data registers
			@FADD_x_Signaling_02:
					;If here,y is sNan
				.ifdef FENV_SUPPORT
				SET_INVALID_FLAG		;signal INVALID
				.endif
				move.l	#$7FC00000,d0		;RESULT (d0) = qNan
				SMALL_END_STACK_FRAME
		@FADD_x_Inf_02:
				;if here, we know that y in not inf or nan and x is inf, so return x
			@FADD_y_Not_Zero_Nan_Inf_02:
				 move.l	a0,d0	;x is infinity,y isn't inf or nan, so result is x
				SMALL_END_STACK_FRAME

