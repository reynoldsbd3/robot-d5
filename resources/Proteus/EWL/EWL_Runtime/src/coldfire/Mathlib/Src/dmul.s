;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: dmul.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Double precision multiplication functions



	.include ColdFire_sfpe.pre.s

	.weak	__d_mul
	
	.public __fp_d_underflow
	.public __fp_d_overflow

	.ifdef FENV_SUPPORT
	.public __fp_d_directed_rounding
	.endif

        .text


;===============================================================================
; FUNCTION: __d_mul
; DESCRIPTION: Double multipliocation.
;
; INPUT: pres(a6) = ptr to result, xhi-xlo(a6) = double, yhi-ylo(a6) = double
; OUTPUT: a0 = pres(a6) = address of result of x * y

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

__d_mul:

		;Set-up the stack frame and save all non-volatile data registers
	D_START_STACK_FRAME

		;save x and y parameters (received through stack)

        movea.l	(Frame_dd2d_xhi,a6),a0	;save x original in (a0,d3)
        move.l	(Frame_dd2d_xlo,a6),d3

	move.l	a0,d0			;d0 = xhi

        movea.l	(Frame_dd2d_yhi,a6),a1	;save y original in (a1,d7)
        move.l	(Frame_dd2d_ylo,a6),d7

	move.l	a1,d4			;d4 = yhi

		;We need a lot of shifting with >8 positions, so will use a register
	moveq.l	#20,d6		;d6 = 20 positions to shift

		;Split y
	move.l	d4,d2		;d2 = yhi
	andi.l	#$000FFFFF,d2	;d2 = yMantisa_hi
	move.l	d4,d5		;d5 = yhi
	lsr.l	d6,d5		;d5 = y with right aligned exponent
	andi.l	#$7FF,d5	;d5 = yExponent

	cmpi.l  #$7FF,d5
	beq	@DMUL_yExp_Max		;yExponent is 3FF (Max) ?

		;Split x
	move.l	a0,d1		;d1 = x
	andi.l	#$000FFFFF,d1	;d1 = xMantisa_hi
	move.l	a0,d0		;d0 = x
	lsr.l	d6,d0		;d0 = (x>>23) ,with right aligned (sign-bit,exponent)
	andi.l	#$7FF,d0	;d0 = xExponent by clearing the sign bit

	cmpi.l  #$7FF,d0	;test if exponent is maxim
	beq	@DMUL_xExp_Max	;branch if xExponent is not maxim (#$FF)

		;No Nan(s) or Inf(s) or (Zero x Inf ) case or (Inf x Zero) case

		;x and y mantisa(53bits each = 52 bits of mantisa + possibly 1 hidden mantisa bit)
		; will be split in (xhi-most significant 21 bit) and (xlo - less significant 32 bits) and
		; (yhi,ylo) respectively


	tst.l	d0		;test if xExponent is 0
	bz	@DMUL_x_Zero_Or_Denorm ;if x is zero or denorm, do not set hidden 1 bit from mantisa
	 ori.l	#$00100000,d1	;put hidden 1 bit of mantisa
	@xMantisa_Aligned:

	tst.l	d5			;test if yExponent is 0
	bz	@DMUL_y_Zero_Or_Denorm	;if y is zero or denorm, do not set hidden 1 bit from mantisa
	 ori.l	#$00100000,d2		;put into mantisa the implicit bit
	@yMantisa_Aligned:

			;compute sum of exponents (biased)
	add.l	d0,d5		;d5 = biased exp of y + biased exp of x
	sub.l	#1023,d5	;d5 = final exponent(containing two bias-es) - bias(1023)

			;here, we have registers as follows
			;d5=current sum of x and y exponents
			;(d1,d3) = xMantisa, with most significant 1 bit on bit 21 of d1
			;(d2,d7) = yMantisa, with most significant 1 bit on bit 21 of d2
	move.l	a0,d6
	move.l	a1,d4
	eor.l	d4,d6
	andi.l	#$80000000,d6

	movea.l	d6,a0	;keep in a0 the final sign and free a data register
	movea.l	d5,a1	;keep in a1 current exponent and free a data register

			;(d1,d3) = xMantisa, with most significant 1 bit on bit 21 of d1
			;(d2,d7) = yMantisa, with most significant 1 bit on bit 21 of d2
			;all the remaining data registers are free

	lea	-12(a7),a7	;alloc stack space for saving 3 adress registers
	movem.l	a2-a4,(a7)	;save a2,a3,a4
	;---------------------------------------------------
	; WILL multiply xMantisa having set the 1 hidden bit with y Mantisa
	; represented as 4 half-words. x=(x0,x1,x2,x3), y=(y0,y1,y2,y3)


	movea.l	d1,a2	;keep xMantisa (d1,d3) in a2,a3 , freeing d1, d3 data regs
	movea.l	d3,a3
	movea.l	d2,a4	;keep yMantisa (d2,d7) in a4,d7, freeing d2 data reg

	clr.l	d2	;need it 0 for adding extended bit

			;d5 will be low 32bit sum
	move.l	d7,d5	;d5 = yMantisaLO(32bits)
	mulu.w	d3,d5	;d5 = yMantisaLO_LOW16 * yMantisaLO_LOW16 =x3*y3

	tst.w	d5	;test lowest 16 bits that will be shifted
	snz.b	d6	;set initial sticky as or of lowest 16 bits of result

	clr.w	d5
	swap.w	d5	;S=S>>16
	;-------------------------------------

	move.l	d7,d4	;d4 = y3
	swap.w	d3	;d3 = x2
	mulu.w	d4,d3	;y3*x2

	add.l	d3,d5	;S+= y3*x2

	swap.w	d4	; d4 = y2
	move.l	a3,d3	;d3=x3
	mulu.w	d4,d3	;y2*x3

	clr.l	d1	;we will use d1 for storing carries

	add.l	d3,d5	;S+= y2*x3
	addx.l	d2,d1	;add carry to d1

	tst.w	d5
	snz.b	d0
	or.l	d0,d6	;low 16 bits are sticky, or with existing sticky
	clr.w	d5
	swap.w	d5	;shift right S=S>>16

	swap.w	d1
	clr.w	d1	;align carry in position, shifting left carry with 16 bits
	add.l	d1,d5	;add carry
	clr.l	d1	;clear carry
	;----------------------------------------------

			;x2*y2
	move.l	a3,d3
	swap.w	d3	;d3 = x2
	mulu.w	d4,d3	;d4 = y2


	add.l	d3,d5	;S+= x2*y2
	addx.l	d2,d1

			;y3*x1
	move.l	a2,d3	;d3 = x1
	move.l	d7,d4	;d4 = y3
	mulu.w	d3,d4


	add.l	d4,d5	;S+= y3*x1
	addx.l	d2,d1

			;y1*x3
	move.l	a3,d3	;x3
	move.l	a4,d4	;y1
	mulu.w	d3,d4

	add.l	d4,d5	;S+=y1*x3
	addx.l	d2,d1

	tst.w	d5
	snz.b	d0
	or.l	d0,d6	;low 16 bits are sticky, or with existing sticky
	clr.w	d5
	swap.w	d5	;shift right S=S>>16

	swap.w	d1
	clr.w	d1	;align carry in position, shifting left carry with 16 bits
	add.l	d1,d5	;add carry
	clr.l	d1	;clear carry
	;--------------------------------------------

		;x0*y3
	move.l	a2,d3	;d3=x1
	swap.w	d3	;d3=x0
	move.l	d7,d4	;d4=y3
	mulu.w	d4,d3	;x0*y3

	add.l	d3,d5	;S+=x0*y3
	addx.l	d1,d1

		;y2*x1
	swap.w	d4	;d4=y2
	move.l	a2,d3	;d3=x1
	mulu.w	d3,d4	;

	add.l	d4,d5
	addx.l	d2,d1

		;x2*y1
	move.l	a4,d3	;d3=y1
	move.l	a3,d4	;
	swap.w	d4	;d4=x2
	mulu.w	d3,d4

	add.l	d4,d5
	addx.l	d2,d1

		;x3*y0
	swap.w	d3	;d3 = y0
	move.l	a3,d4	;d4 =x3
	mulu.w	d3,d4

	add.l	d4,d5
	addx.l	d2,d1	;add carry

	move.l	d5,d2	;low 16 bits of result in low 16 bits of d2
	andi.l	#$FFFF,d2	;clear hi bits

	clr.w	d5
	swap.w	d5	;shift right S=S>>16
	swap.w	d1
	clr.w	d1	;align carry in position, shifting left carry with 16 bits
	add.l	d1,d5	;add carry
	clr.l	d1	;clear carry

	clr.l	d0	;we need a zero for adding carry rom extended bit
	;------------------------------------------------------
		;x1*y1
	move.l	a2,d3	;d3=x1
	move.l	a4,d4	;d4=y1
	mulu.w	d3,d4	;

	add.l	d4,d5
	addx.l	d0,d1	;add carry

		;x0*y2
	swap.w	d3	;d3=x0
	move.l	d7,d4	;
	swap.w	d4	;d4=y2
	mulu.w	d3,d4

	add.l	d4,d5
	addx.l	d0,d1	;add carry

		;x2*y0
	move.l	a3,d3
	swap.w	d3	;d3=x2
	move.l	a4,d4
	swap.w	d4	;d4=y0
	mulu.w	d3,d4

	add.l	d4,d5
	addx.l	d0,d1


	swap.w	d2	;put actual low 16 bits of result in high word
	move.w	d5,d2	;low 16 bits of result in low 16 bits of d2
	swap.w	d2	;now we have in d2 lowest 32bits of the result

	clr.w	d5
	swap.w	d5	;shift right S=S>>16
	swap.w	d1
	clr.w	d1	;align carry in position, shifting left carry with 16 bits
	add.l	d1,d5	;add carry
	clr.l	d1	;clear carry

	;-------------------------------------------

		;x0*y1
	move.l	a4,d3	;d3=y1
	move.l	a2,d4
	swap.w	d4	;d4=x0
	mulu.w	d3,d4

	add.l	d4,d5
	addx.l	d0,d1

		;x1*y0
	move.l	a2,d4	;d4=x1
	swap.w	d3	;d3=y0
	mulu.w	d3,d4

	add.l	d4,d5
	addx.l	d0,d1

	move.l	d5,d0	;save low word bits of high word of result

	clr.w	d5
	swap.w	d5	;shift right S=S>>16
	swap.w	d1
	clr.w	d1	;align carry in position, shifting left carry with 16 bits
	add.l	d1,d5	;add carry
	clr.l	d1	;clear carry

	;----------------------------------------------------------

		;x0*y0
	move.l	a2,d3
	move.l	a4,d4
	swap.w	d3
	swap.w	d4
	mulu.w	d3,d4

	add.l	d4,d5

	swap.w	d5
	clr.w	d5
	andi.l	#$FFFF,d0;clear hi half-word
	add.l	d5,d0	;put in in d2 to have all 32 bits of high word of result

	;in (d0,d2) we have the result, aligned with decimal point on bit16+ 8 (16+7 from a 0 scale) of d0


	;---------------------------------------------------
	movem.l	(a7),a2-a4	;restore the 4 saved address register
	lea	12(a7),a7	;regain allocated space on stack frame


	move.l	a1,d3	;d3 is exponent

		;we have in (d0,d2) result mantisa, d3 exponent, d6=sticky, a0=result sign
	andi.l	#1,d6	;keep only 1 bit as sticky
	move.l	d2,d1	;copy low word of result in d1
		;we will have mantisa in (d0,d1)
	clr.l	d4	;we will keep in d4 the round bit

	btst.l	#25,d0	;test bit 26(counting from 1)
	bz	@DMUL_NO_1SHIFT_RIGHT
		;if here, round bit is least significant 5-th bit
		;less significant 4 bits are sticky
		;(d0,d1)>>5
	 addi.l	#1,d3	;increment exponent (we are in the case where result is ww.xxxx), not w.xxxx
			;so shift right an extra position
	 andi.l	#$F,d2	;keep only less significant 4 bits
	 add.l	d2,d6	;add to sticky

	 lsr.l	#5,d1	;align low word of mantisa and round bit is in Carry and X bit
	 addx.l	d4,d4	;put round bit (the X-tended bit) in d4 the round bit

	 move.l	d0,d5	;put in d4 high word
	 move.l	#27,d7	;we need shifts
	 lsl.l	d7,d5	;put less significant 5 bits of HiWord of result mantisa
			;as the most significant 5 bits of d4
	 or.l	d5,d1	;put them in place, as high bits
	 lsr.l	#5,d0	;shift right d0 with 5 positions

	;----------------------------WE HAVE RESULT UN-PACKED


@DMUL_MANTISA_IN_POSITION:	;we branch here from @DMUL_NO_1SHIFT_RIGHT


		;NOW WE HAVE in d3=result exponent, in (d0,d1)=result mantisa;
		;d4=roundBit, d6=sticky, a0=result sign

	tst.w	d3			;test if exponent zero
	ble	@DMUL_Denormalization	;branch if greater than zero


@DMUL_Denormalized:
	cmpi.l	#$7FF,d3		;test if exponent max
	bge	@DMUL_prep_overflow	;branch if exponent is max or over max


		;pack result
	andi.l	#$FFFFF,d0	;clear hidden 1 mantisa bit
		;put exponent in the right position
	move.l	#20,d5
	lsl.l	d5,d3
	or.l	d3,d0


		;WE HAVE in (d0,d1)=result , d4=roundBit, d6=stickyBit

	;-------------------Rounding------------------------------------

@DMUL_Do_Rounding:
		;put sign of the result in the right position
	add.l	a0,d0

@DMUL_Really_Do_Rounding:

.ifndef	FENV_SUPPORT

        tst.l	d4	        ;  test round bit
        beq	@DMUL_End_Return	;  round bit is 0 --> truncate
	clr.l	d7	;we need a 0 register for adding carry
        tst.l	d6		;  test sticky
        bne	@DMUL_ADD_ONE         ;  nonzero stickies force roundup

			        ;  Halfway case: check LSB
	move.l	d1,d2			;simpler than check LSB, just add last bit of d0 to d0
	andi.l	#1,d2
	add.l	d2,d1
	addx.l	d7,d0
  @DMUL_End_Return:
	D_END_STACK_FRAME
  @DMUL_ADD_ONE:
	addi.l	#1,d1
	addx.l	d7,d0
	D_END_STACK_FRAME

.else

	move.l	d4,d2	;put in d2 round bit
	or.l	d6,d2	;or-ed with sticky
	bne	@DMUL_Not_Exact
		;if result is exact, no rounding needed
	 D_END_STACK_FRAME

	@DMUL_Not_Exact:

		;ROUND RESULT
	movea.l	d4,a0	;keep round bit in a volatile register
	movea.l	d6,a1	;keep sticky in a volatile register

	.ifdef	PID_PIC_FENV_SUPPORT
	move.l	a1,d3				;in pid mode, we need an adress register
	.endif						;which the macro Load/Store FPSCR does not used in
								;non-pid mode

	LoadFPSCR d2,a1   ; puts state bits in d2
	bset      #INEXACT_BIT,d2
	StoreFPSCR d2,a1

	.ifdef	PID_PIC_FENV_SUPPORT
	move.l	d3,a1
	.endif

	movem.l	(a7),d3-d7
	lea.l	20(a7),a7


	 btst      #DIRECTED_BIT,d2
	 bne       __fp_d_directed_rounding
	 btst      #CHOP_OR_UP_BIT,d2     ; toward 0 --> truncate
	bne       @DMUL_endround

        tst.l	a0
        beq	@DMUL_endround		  ;  round bit is 0 --> truncate

        tst.l	a1	;test sticky
        bne	@DMUL_roundup               ;  nonzero stickies force roundup

        btst	#0,d1                  ;  Halfway case: check LSB
        beq	@DMUL_endround              ;  is LSB nonzero?

	@DMUL_roundup:


	        clr.l	d2	;need d2=0 for addx
	        addq.l	#1,d1	; bump lsb
	        addx.l	d2,d0	; add carry to high mantisa word

		move.l	#$800fffff,d2  ; did we overflow? check for maxexp
		or.l   d0,d2    ; we got only 1s in case of maxexp
		not.l   d2
		beq   @DMUL_overflow_exit

	@DMUL_endround:


        movea.l   (Frame_dd2d_pres,a6),a0
        move.l    d0,(a0)
        move.l    d1,(4,a0)
        unlk      a6
        rts

@DMUL_overflow_exit:
	LoadFPSCR d2,a0
	ori.l	#OVERFLOW_OR_INEXACT_MASK,d2
        StoreFPSCR d2,a0

        movea.l   (Frame_dd2d_pres,a6),a0
        move.l    d0,(a0)
        move.l    d1,(4,a0)
        unlk      a6
        rts



.endif		;end IEEE compliance


@DMUL_Denormalization:
.ifndef	FENV_SUPPORT

	 cmpi.l	#-52,d3
	 blt	@DMUL_prep_d_underflow	;if exponent is less, even with round_up remains 0

	 cmpi.l	#-31,d3
	 blt	@DMUL_DENORM_64	;we have to shift more than 32 positions
	 beq	@DMUL_DENORM_32	;the number of positions to shift is 32
		;if here, the number of shiftes is less than 32
	 move.l	d0,d2	;save d0,d1 in d2,d5
	 move.l	d1,d5

	 addi.l	#-1,d3	;add 1 to number of positions to shift since the encoded exponent is 1-bias
			;not 0-bias
	 move.l	d3,d7	;save exponent

	 neg.l	d3	;exponent = -exponent
	 lsr.l	d3,d0	;d0 >> cnt(=d3)
	 lsr.l	d3,d1	;d1 >> cnt(=d3)

	 addi.l	#32,d7	;32-cnt
	 lsl.l	d7,d2	;put less significant bits in d0 in high significant positions
	 or.l	d2,d1	;put them in d1
		;now we have correct mantisa in

	 lsl.l	d7,d5	;we have stickies in d5

	 or.l	d4,d6	;old round bit is sticky now
	 clr.l	d4	;make round bit 0
	 lsl.l	#1,d5	;move into carry and X bits the most significant bit
	 addx.l	d4,d4	;put round bit in d4
	 tst.l	d5	;now we have in d5
	 snz.b	d5	;set in d5 lowest 8 bits accordingly
	 or.l	d5,d6	;update sticky

	 bra	@DMUL_Do_Rounding

     @DMUL_DENORM_32:
		;if here, the number of shifts is 32
	 or.l	d4,d6	;old round bit is sticky now
	 clr.l	d4	;make round bit 0

	 lsl.l	#1,d1	;move into carry and X bits the most significant bit
	 addx.l	d4,d4	;put round bit in d4

	 tst.l	d1	;now we have in d5
	 snz.b	d1	;set in d5 lowest 8 bits accordingly
	 or.l	d1,d6	;update sticky

	 move.l	d0,d1	;low mantisa word = high mantisa word
	 clr.l	d0

	 bra	@DMUL_Do_Rounding

     @DMUL_DENORM_64:
			;if here, the number of shifts is >32

	 or.l	d4,d6	;old round bit is sticky now
	 clr.l	d4	;make round bit 0

	addi.l	#31,d3	;add 32 to shift number

	tst.l	d1	;test
	snz.b	d5	;set accordingly the low byte of d5
	or.l	d5,d6	;or it with the other sticky

	move.l	d0,d1	;copy d0 in d1

	neg.l	d3	;make exponent positiv
	lsr.l	d3,d1	;we have in d1 the result
	addx.l	d4,d4	;last shifted bit is round
	move.l	#33,d5
	sub.l	d3,d5
	lsl.l	d5,d0	;in d0 we have remaining stickies
	snz.b	d5
	or.l	d5,d6	;or with the other stickies

	clr.l	d0	;clear d0

	 bra	@DMUL_Do_Rounding

@DMUL_prep_d_underflow:
	 add.l	a0,d0	;put sign in d0
	 movem.l	(a7),d3-d7
	 lea.l		20(a7),a7
	 bra	__fp_d_underflow

.else

		;we will have to shift it right 1 position more, since we have 1-bias (this is the
		;reason for 31 instead 32

	 cmpi.l	#-52,d3
	 blt	@DMUL_prep_d_underflow	;if exponent is less, even with round_up remains 0

	 cmpi.l	#-31,d3		;compare with 31 because in fact we have to account for adding
				;1 bit because the result is encoded as 1-bias, not as we have now
				;as 0-bias, so in reality 31 means 32
	 blt	@DMUL_DENORM_64	;we have to shift more than 32 positions
	 beq	@DMUL_DENORM_32	;the number of positions to shift is 32
		;if here, the number of shiftes is less than 32
	 move.l	d0,d2	;save d0,d1 in d2,d5
	 move.l	d1,d5

	 addi.l	#-1,d3	; add 1 to number of positions to shift, since the exponent is 1-bias, not 0-bias
			;since d3 is kept as a negative number, substract 1 instead
	 move.l	d3,d7	;save exponent

	 neg.l	d3	;exponent = -exponent
	 lsr.l	d3,d0	;d0 >> cnt(=d3)
	 lsr.l	d3,d1	;d1 >> cnt(=d3)

	 addi.l	#32,d7	;32-cnt
	 lsl.l	d7,d2	;put less significant bits in d0 in high significant positions
	 or.l	d2,d1	;put them in d1
		;now we have correct mantisa in

	 lsl.l	d7,d5	;we have stickies in d5

	 or.l	d4,d6	;old round bit is sticky now
	 clr.l	d4	;make round bit 0
	 lsl.l	#1,d5	;move into carry and X bits the most significant bit
	 addx.l	d4,d4	;put round bit in d4
	 tst.l	d5	;now we have in d5
	 snz.b	d5	;set in d5 lowest 8 bits accordingly
	 or.l	d5,d6	;update sticky

	 bra	@DMUL_TEST_INEXACT

     @DMUL_DENORM_32:
		;if here, the number of shifts is 32

	 or.l	d4,d6	;old round bit is sticky now
	 clr.l	d4	;make round bit 0

	 lsl.l	#1,d1	;move into carry and X bits the most significant bit
	 addx.l	d4,d4	;put round bit in d4

	 tst.l	d1	;now we have in d5
	 snz.b	d1	;set in d5 lowest 8 bits accordingly
	 or.l	d1,d6	;update sticky

	 move.l	d0,d1	;low mantisa word = high mantisa word
	 clr.l	d0

	 bra	@DMUL_TEST_INEXACT

     @DMUL_DENORM_64:
			;if here, the number of shifts is >32

	 or.l	d4,d6	;old round bit is sticky now
	 clr.l	d4	;make round bit 0

	addi.l	#31,d3	;add 32 to shift number (1 position comes for 1-bias instead of 0-bias)

	tst.l	d1	;test
	snz.b	d5	;set accordingly the low byte of d5
	or.l	d5,d6	;or it with the other sticky

	move.l	d0,d1	;copy d0 in d1

	neg.l	d3	;make exponent positiv
	lsr.l	d3,d1	;we have in d1 the result
	addx.l	d4,d4	;last shifted bit is round
	move.l	#33,d5
	sub.l	d3,d5
	lsl.l	d5,d0	;in d0 we have remaining stickies
	snz.b	d5
	or.l	d5,d6	;update stickies

	clr.l	d0	;clear d0

@DMUL_TEST_INEXACT:
	 tst.b	d6
	 snz.b	d6
	 move.l	d4,d5
	 or.l	d6,d5	;or sticky with round

	 bz	@DMUL_Denormalization_Exact


		;set underflow & inexact
	  LoadFPSCR d7,a1
	  ori.l	#UNDERFLOW_OR_INEXACT_MASK,d7
          StoreFPSCR d7,a1
		;go to normal rounding routine
	  bne	@DMUL_Do_Rounding	;not 0, need rounding

	@DMUL_Denormalization_Exact:
	add.l	a0,d0	;put sign in result
	 D_END_STACK_FRAME




@DMUL_prep_d_underflow:
	 add.l	a0,d0	;put sign in d0
	 movem.l	(a7),d3-d7
	 lea.l		20(a7),a7
	 bra	__fp_d_underflow

.endif

@DMUL_prep_overflow:
	move.l	a0,d0
	movem.l	(a7),d3-d7
	lea.l	20(a7),a7
	bra	__fp_d_overflow


@DMUL_NO_1SHIFT_RIGHT:
		;if here, round bit is least significant 4-th bit
		;less significant 3 bits are sticky
		;(d0,d1)>>4
	 andi.l	#$7,d2	;keep only less significant 4 bits
	 add.l	d2,d6	;add to sticky

	 lsr.l	#4,d1	;align low word of mantisa and round bit is in Carry and X bit
	 addx.l	d4,d4	;put round bit (the X-tended bit) in d4 the round bit

	 move.l	d0,d5	;put in d4 high word
	 move.l	#28,d7	;we need shifts
	 lsl.l	d7,d5	;put less significant 5 bits of HiWord of result mantisa
			;as the most significant 5 bits of d4
	 or.l	d5,d1	;put them in place, as high bits
	 lsr.l	#4,d0	;shift right d0 with 5 positions
	 bra	@DMUL_MANTISA_IN_POSITION


@DMUL_x_Zero_Or_Denorm:
	  tst.l	d3		;test if xMantisa is zero
	  snz.b	d6
	  or.l	d1,d6

	  bnz	@x_Not_Zero
		;if here, y is zero, so rezult is zero with sign=xsign^ysign
		move.l	a0,d0
		move.l	a1,d1
		eor.l	d1,d0
		andi.l	#$80000000,d0	;keep the sign and zero the rest
		clr.l	d1
		D_END_STACK_FRAME
	  @x_Not_Zero:
		;normalize xMantisa (d1,d3), diminishing the exponent
		;exponent is (1-bias) for denorms, not (0-bias), so add 1 to exponent
		addi.l	#1,d0
		;subi.l	#1,d0
	    clr.l	d6	;need a register with 0
	  @x_Normalized:
	  btst.l	#20,d1		;test bit 21 till it is 1
	  bnz	@xMantisa_Aligned
	  sub.l		#1,d0		;decrement exponent
	  lsl.l		#1,d1		;xMantisaHI shift left 1 position
	  lsl.l		#1,d3		;xMantisaLO shift left 1 position, set X bit with significant bit
	  addx.l	d6,d1
	  bra @x_Normalized

@DMUL_y_Zero_Or_Denorm:

		;test if yMantisa = 0 (denorm or zero)
	  tst.l	d7
	  snz.b	d6
	  or.l	d2,d6

	  bnz	@y_Not_Zero
		;if here, y is zero
		move.l	a0,d0
		move.l	a1,d1
		eor.l	d1,d0
		andi.l	#$80000000,d0
		clr.l	d1
		D_END_STACK_FRAME
	  @y_Not_Zero:
		;normalize yMantisa (d1), diminishing the exponent
		;exponent is 1-bias, not 0-bias, so add 1 to exponent
		addi.l	#1,d5
		; subi.l	#1,d5
		clr.l	d6	;need a register which is 0
	  @y_Normalized:
	  btst.l	#20,d2		;test bit 21 till it is 1
	  bnz		@yMantisa_Aligned
	  sub.l		#1,d5		;substract 1 from exponent
	  lsl.l		#1,d2		;shift left yMantisaHI
	  asl.l		#1,d7		;shift arithmetic left yMantisaLOW, shifted bit in Carry&Extend bits
	  addx.l	d6,d2
	  bra		@y_Normalized



@DMUL_yExp_Max:
			;here yExponent is 3FF(Max)
			;Now , mantisa does not matter except for if 0 or not 0, so make least significant
			;32 bits stickies and or-it to yMantisa_hi
	tst.l	d7	;test if low bits of yMantisa are zero
	snz.b	d6	;we know that bits 8-31 are 0 (d6 was previously 0x14)
		;andi.l	#1,d6
	or.l	d2,d6	;we have in d2 yMantisa_hi with lowest 8bits 1 if yMantisa_lo != 0
	beq	@DMUL_y_Inf_01
			;If here, y is Nan
		andi.l	#$00080000,d2		;test if Nan is Signaling (First mantisa bit 0)
		bz	@DMUL_y_Signaling_01
				;If here, y is qNan, but test if x is sNan
			clr.l	d6
			tst.l	d3
			snz.b	d6
			andi.l	#$000fffff,d0		;test if mantisa is zero (it can not be sNan)
				;andi.l	#1,d7
			or.l	d0,d6
			bz	@DMUL_x_Not_Snan

			move.l	a0,d0		;here we know that mantisa is not zero, so if exp Max
			andi.l	#$7ff80000,d0	;and test if x is sNan
			cmpi.l	#$7ff00000,d0

			beq	@DMUL_y_Signaling_01	;if x = sNan, set invalid and return qNan
				;else return qNan

			@DMUL_x_Not_Snan:
			move.l	#$7FF80000,d0		;RESULT (d0,d1) = qNan
			clr.l	d1
			D_END_STACK_FRAME

		@DMUL_y_Signaling_01:
				;If here,y is sNan
			.ifdef FENV_SUPPORT
			SET_INVALID_FLAG		;signal INVALID
			.endif
			move.l	#$7FF80000,d0		;RESULT (d0,d1) = qNan
			clr.l	d1
			D_END_STACK_FRAME
	@DMUL_y_Inf_01:
			  ;Test if x is 0
			tst.l	d3
			snz.b	d6
			andi.l	#$7FFFFFFF,d0	;d0 = xMantisa_hi
			or.l	d0,d6		;compare x with 0
		bnz @DMUL_x_Non_Zero_01		;branch if not zero
				;here we have INF x ZERO , so signal invalid
			.ifdef FENV_SUPPORT
			SET_INVALID_FLAG	;set Invalid Flag if IEEE compliance, alters d7
			.endif
			move.l	#$7FF80000,d0	;RESULT (d0,d1) = qNan
			clr.l	d1
			D_END_STACK_FRAME		;clear stack frame and return d0 which is qNan
		@DMUL_x_Non_Zero_01:
			;Test if x is Nan or Inf
		move	a0,d0
		andi.l	#$7FF00000,d0		;keep x Exponent
		cmpi.l	#$7FF00000,d0		;test if x has max exponent
		bne	@DMUL_x_Not_Zero_Nan_Inf_01
			;x is Nan or Inf
		move.l	a0,d0		;d0 = xHi
		tst.l	d3
		snz.b	d6
		andi.l	#$FF,d6
		andi.l	#$000FFFFF,d0	;d0 = xMantisa
		or.l	d0,d6
		bz	@DMUL_x_Inf_01	;if xMantisa==0 , x is Inf and branch
			  ;here x is Nan
			andi.l	#$00080000,d0	;keep first xMantisa bit in d0
			bz	@DMUL_x_Is_Signaling_01
				  ;here x is qNan
				move.l #$7FF80000,d0	;move (d0,d1)(x=qNan) as result
				clr.l	d1
				D_END_STACK_FRAME
			@DMUL_x_Is_Signaling_01:
				  ;here x is sNan
				.ifdef FENV_SUPPORT
				SET_INVALID_FLAG
				.endif
				move.l #$7ff80000,d0
				clr.l	d1
				D_END_STACK_FRAME
		@DMUL_x_Inf_01:
		    ;As long as y is Inf and x is not nan or zero is the same as x usual number
	@DMUL_x_Not_Zero_Nan_Inf_01:
		move.l	a0,d0
		eor.l	d4,d0		;d0 = yInitial ^ xInitial
		andi.l	#$80000000,d0
		ori.l	#$7FF00000,d0
		clr.l	d1
		D_END_STACK_FRAME



@DMUL_xExp_Max:
			;if here, xExponent is #$FF(Max)
	tst.l	d3		;since xExponent is Max, test if Mantisa is 0(nr. is Inf)
	snz.b	d6
	or.l	d1,d6
	beq	@DMUL_x_Inf_02	;branch if mantisa is zero (x is Inf)
			;If here, x is qNan or sNan
		andi.l	#$00080000,d1		;test if Nan is Signaling (First mantisa bit 0)
		bz	@DMUL_x_Signaling_02		;branch if signaling
				;If here, x=qNan and y<>sNan, so return  qNan
			move.l	#$7FF80000,d0	;RESULT (d0,d1) = qNan
			clr.l	d1
			D_END_STACK_FRAME		;clear stack frame and restore data registers
		@DMUL_x_Signaling_02:
				;If here,y is sNan
			.ifdef FENV_SUPPORT
			SET_INVALID_FLAG		;signal INVALID
			.endif
			move.l	#$7FF80000,d0		;RESULT (d0,d1) = qNan
			clr.l	d1
			D_END_STACK_FRAME
	@DMUL_x_Inf_02:
			;Test if y is 0, we know that y can't be Nan of Inf because
			;the program would have ended in @DMUL_yExp_Max
		andi.l	#$7FFFFFFF,d4	;test yMantisaHI
		snz.b	d6	;low 8 bits are 1's if d4 not zero
		or.l	d7,d6	;or with yMantisaLO


		bnz	@DMUL_y_Non_Zero_02		;branch if not zero
				;here we have INF x ZERO , so signal invalid
			.ifdef FENV_SUPPORT
			SET_INVALID_FLAG	;set Invalid Flag if IEEE compliance, alters d7
			.endif
			move.l	#$7FF80000,d0	;RESULT (d0,d1) = qNan
			clr.l	d1
			D_END_STACK_FRAME		;clear stack frame and return d0 which is qNan
		@DMUL_y_Non_Zero_02:
			;x is inf, y is not zero or Nan or Inf
			;As long as y in Inf and x is not nan or zero is the same as x usual number
		@DMUL_y_Not_Zero_Nan_Inf_02:
			move.l	a0,d0
			move.l	a1,d6		;d7= yInitial
			eor.l	d6,d0		;d0 = yInitial ^ xInitial
			andi.l	#$80000000,d0	;d0 = sign of the result
			ori.l	#$7FF00000,d0	;d0 = put Max exponent and so make Inf
			clr.l	d1
			D_END_STACK_FRAME

