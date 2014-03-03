;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; FILE: fmul.s
;;;;
;;;; Copyright ©1997-2003 Motorola, Metrowerks,   All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;-------------------------------------------------------------------------------
; ColdFire floating point software emulation library
; Single precision multiplication functions


	.include ColdFire_sfpe.pre.s

	.weak	__f_mul

        .text

END_STACK_FRAME:	.macro
		;clear stack frame, return should be in d0
	movem.l   (a7),d3-d7
	lea       20(a7),a7
        unlk      a6
        rts
.endm

START_STACK_FRAME:	.macro
	 	;start stack frame
        link.w    a6,#Frame_ff2f_localsize
	lea       -20(a7),a7
	movem.l   d3-d7,(a7)
.endm



;===============================================================================
; FUNCTION: __f_mul
; DESCRIPTION: Float multiply.
;
; INPUT: x(a6), y(a6), floats
; OUTPUT: d0 = result

__f_mul:
		;Set-up the stack frame and save all non-volatile data registers
	START_STACK_FRAME

		;save x and y parameters (received through stack)
	movem.l (Frame_ff2f_x,a6),d0-d1	 ;d0=x, d1=y
	movea.l	d0,a0		;save x original in a0
	movea.l d1,a1		;save y original in a1

		;We need a lot of shifting with >8 positions, so will use a register
	moveq.l	#23,d6		;d6 = 23 positions to shift

		;Split y
	move.l	a1,d2		;d2=y
	andi.l	#$007FFFFF,d2	;d2 = yMantisa
	move.l	a1,d1		;d1 = y
	lsr.l	d6,d1		;d1 = y with right aligned exponent
	andi.l	#$FF,d1		;d1 = yExponent

	cmpi.l  #$FF,d1
	beq	FMUL_yExponent_Max_01

		;if we got here, (d1,d2) will be (yExponent,yMantisa)

		;Split x
	move.l	a0,d0		;d2 = x
	andi.l	#$007FFFFF,d0	;d2 = xMantisa
	move.l	a0,d5		;d5 = x
	lsr.l	d6,d5		;d5 = (x>>23) ,with right aligned (sign-bit,exponent)
	andi.l	#$FF,d5		;d5 = xExponent by clearing the sign bit

	cmpi.l  #$FF,d5			;test if exponent is maxim
	beq	FMUL_xExponent_Max_02


	xExponent_Not_Max_02:
		;No Nan(s) or Inf(s) or (Zero x Inf ) case or (Inf x Zero) case
	NO_NANs_OR_INFs:


		;Here is for normal numbers
		;x and y manitisa(24bits each = 23 bits of mantisa + possibly 1 hidden mantisa bit)
		; will be split in (xhi-most significant 8 bit) and (xlo - less significant 16 bit) and
		; (yhi,ylo) respectively



		;d0 = x fraction with hidden 1 bit

		;move.l	a0,d0		;d0 = x
		;andi.l	#$007FFFFF,d0	;d0 = xMantisa
		;move.l	a0,d5		;d5 = x
		;lsr.l	d6,d5		;d5 = (sign;biasedExponent)
		;andi.l	#$FF,d5		;d5 = (biasedExponent)


	;(d5,d0) = (xExponent,x mantisa)
	;(d1,d2) = (yExponent,y mantisa)

	tst.l	d5	;test if xExponent is 0
	bz	FMUL_x_Zero_Or_Denorm ;if x is zero or denorm, do not set hidden 1 bit from mantisa
	 ori.l	#$00800000,d0	;put hidden 1 bit of mantisa
	FMUL_xMantisa_Aligned:

	tst.l	d1		;test if yExponent is 0
	bz	FMUL_y_Zero_Or_Denorm ;if y is zero or denorm, do not set hidden 1 bit from mantisa
		;if here, y exponent is non-zero
	 ori.l	#$00800000,d2		;put into mantisa the implicit bit
	FMUL_yMantisa_Aligned:

			;compute sum of exponents (biased)
	add.l	d1,d5		;d5 = biased exp of y + biased exp of x
	sub.l	#127,d5		;d5 = final exponent(containing two bias-es) - bias(127)

			;compute the resulting mantisa
	clr.l	d7		;d7 will be sticky flag from now on, sticky is 0
	move.l	d0,d3		;d3 = xMantisa
	move.l	d2,d4		;d4 = yMantisa

	mulu.w	d0,d2		;d1=xlo * ylo
	tst.w	d2		;all low order 16 bits of (xlo*ylo) are sticky

	beq	Sticky_Is_Not_1
	  addq.l	#1,d7	;sticky is 1

	Sticky_Is_Not_1:

	moveq.l	#16,d6		;we need 16 bit shifts, so we use a register

	lsr.l	d6,d2		;shift result with 16 bits(8+8), d2 = HI16bits(xlo*ylo)
				;will have here first 32bit of mantisa of
				;the 24bit x 24bit full multiplication result

	move.l	d3,d0		;d0 = x
	move.l	d4,d1		;d1 = y
	lsr.l	d6,d1		;d1 = (y>>16) = yhi
	mulu.w	d0,d1		;d1 = xlo*yhi
	add.l	d1,d2		;add xlo*yhi to result mantisa (HI16bits(xlo*ylo)))
	lsr.l	d6,d0		;d0 = (x>>16) = xhi
	move.l	d4,d1		;d1 = y
	mulu.w	d0,d1		;d1 = xhi*ylo
	add.l	d1,d2		;add ylo*xhi to result mantisa (in fact is shifted with 16)
	move.l	d4,d1		;d2 = y
	lsr.l	d6,d1		;d2 = (y>>16) = yhi
	mulu.w	d1,d0		;d0 = yhi*xhi
	lsl.l	d6,d0		;d0 = (xhi*yhi)<<16	(in fact is shifted with 32positions)
	add.l	d0,d2		;add (xhi*yhi) properly shifted to the result mantisa

		;test if result is zero (that means that at least one of the numbers is zero
	move.l	d2,d1		;move first 32 bits of results from mantisa multiplications
	or.l	d7,d2		;or first 32 bits result mantisa with sticky from the dismissed 16 bits

	bnz	Result_Not_Zero
		;if here, one of the operands was 0, so return 0 with the correct sign
	  move.l	a0,d0		;d0 = x
	  move.l	a1,d1		;d1 = y
	  eor.l		d1,d0		;d0 = x^y (we do that for the xor of signs)
	  andi.l	#$80000000,d0
	  END_STACK_FRAME


	Result_Not_Zero:


	btst	#31,d1			;test if msb is 1 (binary point is bit 46!)
	bz	No_Shifting
		; put result mantisa in place
	  or.l d1,d7	; or the shifted bit together with sticky
	  andi.l #$1,d7	; keep only least significant bit

	  addi.l	#1,d5		;add 1 to exponent because we need to right shift mantisa with 1 position
	  lsr.l		#1,d1		;align mantisa with 1 as the least significant exponent bit

	No_Shifting:

	move.l	d1,d2	;keep mantisa in d2 and use d1

	  ;set in  d3 round bit
	clr.l	d3	;initialize roundBit to 0
	btst	#6,d1			;test if bit nr.8 (round bit) equals 1
	bz	Rounding_Bit_Is_Zero	;if bit is 0, do not set the bit
	 moveq.l	#1,d3		;set roundBit to 1
	Rounding_Bit_Is_Zero:

	  ;test remaining sticky bits
	andi.l	#$3F,d1			;test if one of the remaining sticky bits is 1
	bz	Do_Nothing_With_Current_Sticky	;sticky(d7) remains the same as now
	 moveq.l	#1,d7		;make sticky 1
	Do_Nothing_With_Current_Sticky:



	  lsr.l		#7,d2		;align mantisa with 1 as the least significant exponent bit

	  move.l	a0,d0		;d0 = initial x
	  move.l	a1,d4		;d2 = initial y
	  eor.l		d4,d0		;d0 = d0 ^ d2 (make sign of result=xor of sign bits)
	  andi.l	#$80000000,d0	;d0 = keep only sign bit


		;NOW WE HAVE in d5=result exponent, in d2=result mantisa;
		;d3=roundBit, d7=stickyBit, d0=sign

	tst.w	d5			;test if exponent zero
	bgt	No_Denormalization	;branch if greater than zero

.ifndef	FENV_SUPPORT
	 cmpi.l	#-23,d5
	 blt	fmul_underflow		;if exponent is less, even with round_up remains 0
	 beq	Test_Very_Tiny		;test if result is less than smallest denorm, but round-ing up
					;could make it the smallest representable denorm

		;if here, the result will be a denorm
	 move.l	#$FFFFFFFF,d6	;use it to generate a mask for remaining sticky bits
	 neg.l	d5		;d5 = abs(d5)  (= 0-d5 )
	 lsl.l	d5,d6		;d6 = 1..110...00, with d5 zeros and 32-d5 ones
	 not.l	d6		;d6 = change all bits for the correct mask

	 and.l	d2,d6		;d6 = apply mask to mantisa
	 beq	Sticky_Unchanged
	 moveq.l #1,d7		;set sticky (d7=1)
	Sticky_Unchanged:
	 or.l	d3,d7		;old round is sticky now, so update sticky
	 lsr.l	d5,d2		;shift right mantisa
	 move.l	d2,d3
	 andi.l	#1,d3		;d3 = new round bit
	 lsr.l	#1,d2		;shift right one position, since denorm exp is not -bias, is 1-bias
	 or.l	d2,d0		;put mantisa in d0
	 bra	Do_Rounding

	Test_Very_Tiny:
		;if here, the result might round up to the smallest denorm
	 neg.l	d5		;d5 = abs(d5)  (= 0-d5 )
	 btst.l	#23,d2		;test round bit (bit 24)
	 beq	Exit_Test_Very_Tiny
	 or.l	d7,d2		;or old sticky with current sticky
	 andi.l	#$7FFFFF,d2	;remaining stickies
	 beq	Exit_Test_Very_Tiny
	 ori.l	#1,d0
	Exit_Test_Very_Tiny:
	 END_STACK_FRAME
.else

	 cmpi.l	#-23,d5
	 blt	fmul_underflow		;if exponent is less, even with round_up remains 0

		;if here, the result will be a denorm
	 move.l	#$FFFFFFFF,d6	;use it to generate a mask for remaining sticky bits
	 neg.l	d5		;d5 = abs(d5)  (= 0-d5 )
	 lsl.l	d5,d6		;d6 = 1..110...00, with d5 zeros and 32-d5 ones
	 not.l	d6		;d6 = change all bits for the correct mask

	 and.l	d2,d6		;d6 = apply mask to mantisa
	 beq	Sticky_Unchanged
	 moveq.l #1,d7		;set sticky (d7=1)
	Sticky_Unchanged:
	 or.l	d3,d7		;old round is sticky now, so update sticky
	 lsr.l	d5,d2		;shift right mantisa
	 move.l	d2,d3
	 andi.l	#1,d3		;d3 = new round bit
	 lsr.l	#1,d2		;shift right one position, since denorm exp is not -bias, is 1-bias
	 or.l	d2,d0		;put mantisa in d0

	 move.l	d3,d6		;round & stickies or-ed to set inexact
	 or.l	d7,d6
	 beq Denormalization_Exact

		;set underflow & inexact
	  LoadFPSCR d6,a1
	  ori.l	#UNDERFLOW_OR_INEXACT_MASK,d6
          StoreFPSCR d6,a1
		;go to normal rounding routine
	  bne	Do_Rounding	;not 0, need rounding

	Denormalization_Exact:
	 END_STACK_FRAME


.endif

       No_Denormalization:
	cmpi.l	#$FF,d5		;test if exponent max
	bge	fmul_overflow	;branch if exponent is max or over max

	  moveq.l	#23,d4		;put exponent in place
	  lsl.l		d4,d5		;put resulting exponent in place
	  andi.l	#$007FFFFF,d2	;clear hidden 1 mantisa bits
	  or.l	d5,d0			;put exponent into result
	  or.l	d2,d0			;put mantisa into result



		;WE HAVE in d5=result exponent, in d2=result mantisa;
		;d3=roundBit, d7=stickyBit, d0=sign

	;-------------------Rounding------------------------------------
	Do_Rounding:

.ifndef	FENV_SUPPORT

        tst.l	d3	        ;  test round bit
        beq	End_Return	;  round bit is 0 --> truncate
        tst.l	d7		;  test sticky
        bne	ADD_ONE         ;  nonzero stickies force roundup
			        ;  Halfway case: check LSB

	move.l	d0,d1			;simpler than check LSB, just add last bit of d0 to d0
	andi.l	#1,d1
	add.l	d1,d0
  End_Return:
	END_STACK_FRAME
  ADD_ONE:
	addi.l	#1,d0
	END_STACK_FRAME

.else

	move.l	d3,d2	;put in d2 round bit
	or.l	d7,d2	;or-ed with sticky
	bne	Not_Exact
	 ;if result is exact, no rounding needed
	 END_STACK_FRAME

	Not_Exact:
		;ROUND RESULT
	        LoadFPSCR d2,a1   ; puts state bits in d2
	bset      #INEXACT_BIT,d2
	        btst      #DIRECTED_BIT,d2
	        bne       fmul_directed_rounding
	        btst      #CHOP_OR_UP_BIT,d2     ; toward 0 --> truncate
	        bne       @endround_01

        tst.l	d3
        beq	@endround_01		  ;  round bit is 0 --> truncate

        tst.l d7	;test sticky
        bne	@roundup_01               ;  nonzero stickies force roundup

        btst	#0,d0                  ;  Halfway case: check LSB
        beq	@endround_01            ;  is LSB nonzero?

	@roundup_01:
	        addq.l    #1,d0    ; bump lsb

		move.l   #0x807fffff,d1  ; did we overflow? check for maxexp
		or.l   d0,d1    ; we got only 1s in case of maxexp
		not.l   d1
		beq   fmul_overflow_exit

	@endround_01:
		StoreFPSCR d2,a1
	END_STACK_FRAME

.endif		;end IEEE compliance

;===============================================================================
; TERMINAL: fmul_overflow
; DESCRIPTION: Returns the result of an overflow according to the rounding mode
; INPUT:  d0.31 = result sign
; OUTPUT: d0 = 32-bit packed result
fmul_overflow:
	.ifdef FENV_SUPPORT
        LoadFPSCR d2,a1		 ; puts state bits in d2
	.endif
	andi.l	  #0x80000000,d0	 ; d0 - +/- infinity
	ori.l	  #0x7f800000,d0
	.ifdef FENV_SUPPORT
        ori.l	  #OVERFLOW_OR_INEXACT_MASK,d2
        StoreFPSCR d2,a1

        btst      #DIRECTED_BIT,d2
        bne       @dir_round_02
        btst      #CHOP_OR_UP_BIT,d2     ; toward 0 --> subtract 1
        sne.b	  d1
        andi.l	  #1,d1			 ; select 0 / 1
        sub.l	  d1,d0
        .endif
	END_STACK_FRAME

	.ifdef FENV_SUPPORT
@dir_round_02:
	; d0 - hi word of result
	; d2 - floating point state
        btst      #CHOP_OR_UP_BIT,d2
        bne       @round_plus_02
        tst.l     d0		         ; Rounding down, so increment if result is negative.
        spl.b	  d1
        andi.l	  #1,d1			 ; select 0 / 1
        sub.l	  d1,d0
	END_STACK_FRAME
@round_plus_02:
        tst.l     d0			 ; Rounding up, so increment if result is positive.
        smi.b	  d1
        andi.l	  #1,d1			 ; select 0 / 1
        sub.l	  d1,d0
	END_STACK_FRAME
        .endif

;===============================================================================
; TERMINAL: fmul_overflow_exit
; DESCRIPTION: Signals overflow, writes fpscr and exits
; INPUT:  d2 = fpscr
; OUTPUT: saves d2 into fpscr
	.ifdef FENV_SUPPORT
fmul_overflow_exit:
        ori.l	#OVERFLOW_OR_INEXACT_MASK,d2
        StoreFPSCR d2,a1
	END_STACK_FRAME
	.endif

;===============================================================================
; TERMINAL: fmul_underflow
; DESCRIPTION: Returns the result of an underflow according to the rounding mode
; INPUT:  d0.31 = result sign
; OUTPUT: d0 = 32-bit packed result
fmul_underflow:
	.ifdef FENV_SUPPORT
        LoadFPSCR d2,a1		 ; puts state bits in d2
	.endif
	andi.l	  #0x80000000,d0	 ; d0 - +/- 0
	.ifdef FENV_SUPPORT
        ori.l	  #UNDERFLOW_OR_INEXACT_MASK,d2
        StoreFPSCR d2,a1

        btst      #DIRECTED_BIT,d2
        beq       @endround_03
        btst      #CHOP_OR_UP_BIT,d2

        bne       @round_plus_03
        tst.l     d0		         ; Rounding down, so increment if result is negative.
        bmi       @roundup_03
	END_STACK_FRAME
@round_plus_03:
        tst.l     d0			 ; Rounding up, so increment if result is positive.
        bpl       @roundup_03
	END_STACK_FRAME
@roundup_03:
        addq.l    #1,d0			 ; return smallest denormalized number
@endround_03:
        .endif
	END_STACK_FRAME

;===============================================================================
; TERMINAL: fmul_directed_rounding
; DESCRIPTION: Rounds a float result and returns, checking for overflow
; INPUT:  d0 = result
;	  d2 = fpscr
	.ifdef FENV_SUPPORT
fmul_directed_rounding:
        btst      #CHOP_OR_UP_BIT,d2
        bne       @round_plus_04
        tst.l     d0		         ; Rounding down, so increment if result is negative.
        bmi       @roundup_04
        bra       @endround_04
@round_plus_04:
        tst.l     d0			 ; Rounding up, so increment if result is positive.
        bmi       @endround_04
@roundup_04:
        addq.l    #1,d0			 ; bump lsb
	move.l	  #0x807fffff,d1	 ; did we overflow? check for maxexp
	or.l	  d0,d1			 ; we got only 1s in case of maxexp
	not.l	  d1
	beq	  fmul_overflow_exit
@endround_04:
        StoreFPSCR d2,a1
	END_STACK_FRAME
        .endif


FMUL_x_Zero_Or_Denorm:
	  tst.l	d0		;test if xMantisa is zero
	  bnz	@x_Not_Zero
		;if here, y is zero, so rezult is zero with sign=xsign^ysign
		move.l	a0,d0
		move.l	a1,d1
		eor.l	d1,d0
		andi.l	#$80000000,d0	;keep the sign and zero the rest
		END_STACK_FRAME
	  @x_Not_Zero:
		;normalize xMantisa (d0), diminishing the exponent
		;exponent is 1-bias, not 0-bias, so add 1 to exponent
		addi.l	#1,d5
	  @x_Normalized:
	  btst.l	#23,d0		;test bit 24 till it is 1
	  bnz	FMUL_xMantisa_Aligned
	  sub.l	#1,d5
	  lsl.l	#1,d0
	  bra @x_Normalized	;x_Zero_Or_Denorm

FMUL_y_Zero_Or_Denorm:
		;test if mantisa = 0 (denorm or zero)
	  tst.l	d2
	  bnz	@y_Not_Zero
		;if here, y is zero
		move.l	a0,d0
		move.l	a1,d1
		eor.l	d1,d0
		andi.l	#$80000000,d0
		END_STACK_FRAME
	  @y_Not_Zero:
		;normalize yMantisa (d1), diminishing the exponent
		;exponent is 1-bias, not 0-bias, so add 1 to exponent
		addi.l	#1,d5
	  @y_Normalized:
	  btst.l	#23,d2		;test bit 24 till it is 1
	  bnz		FMUL_yMantisa_Aligned
	  sub.l		#1,d5
	  lsl.l		#1,d2	;error d0
	  bra		@y_Normalized



FMUL_yExponent_Max_01:	;yExponent is FF (Max) ?
			;yExponent is FF(Max)
		tst.l	d2		;since yExponent is Max, test if Mantisa is 0(nr. is Inf)
		beq	y_Inf_01
				;If here, y is Nan
			andi.l	#$00400000,d2		;test if Nan is Signaling (First mantisa bit 0)
			bz	y_Signaling_01
					;If here, y is qNan, but test if x is sNan
				move.l	a0,d0		;d0 = x
				andi.l	#$007FFFFF,d0	;test if mantisa 0 (if zero, it can not be neither qNan or sNan)
				bz	x_Not_Snan

				move.l	a0,d0
				andi.l	#$7fC00000,d0	;test if x is sNan (exp. Max and first mantisa bit 0)
				cmpi.l	#$7f800000,d0

				beq	y_Signaling_01	;if x = sNan, set invalid and return qNan
					;else return qNan

				x_Not_Snan:
				move.l	#$7FC00000,d0		;RESULT (d0) = qNan
				END_STACK_FRAME

			y_Signaling_01:
					;If here,y is sNan
				.ifdef FENV_SUPPORT
				SET_INVALID_FLAG		;signal INVALID
				.endif
				move.l	#$7FC00000,d0		;RESULT (d0) = qNan
				END_STACK_FRAME
		y_Inf_01:
				;Test if x is 0
			move.l	a0,d0
			andi.l	#$7FFFFFFF,d0		;compare x with 0
			bnz x_Non_Zero_01		;branch if not zero
					;here we have INF x ZERO , so signal invalid
				.ifdef FENV_SUPPORT
				SET_INVALID_FLAG	;set Invalid Flag if IEEE compliance, alters d7
				.endif
				move.l	#$7FC00000,d0	;RESULT (d0) = qNan
				END_STACK_FRAME		;clear stack frame and return d0 which is qNan
			x_Non_Zero_01:
				;Test if x is Nan or Inf
			move	a0,d0
			andi.l	#$7F800000,d0		;keep x Exponent
			cmpi.l	#$7F800000,d0		;test if x has max exponent
			bne	x_Not_Zero_Nan_Inf_01
					;x is Nan or Inf
				move.l	a0,d0		;d0 = x
				andi.l	#$007FFFFF,d0	;d0 = xMantisa
				bz	x_Inf_01	;if xMantisa==0 , x is Inf and branch
					  ;here x is Nan
					andi.l	#$00400000,d0	;keep first xMantisa bit in d0
					bz	x_Is_Signaling_01
						  ;here x is qNan
						move.l #$7FC00000,d0	;move a0(x=qNan) as result
						END_STACK_FRAME
					x_Is_Signaling_01:
						  ;here x is sNan
						.ifdef FENV_SUPPORT
						SET_INVALID_FLAG
						.endif
						move.l #$7fC00000,d0
						END_STACK_FRAME
				x_Inf_01:
				    ;As long as y is Inf and x is not nan or zero is the same as x usual number
			x_Not_Zero_Nan_Inf_01:
				move.l	a0,d0
				move.l	a1,d7		;d7= yInitial
				eor.l	d7,d0		;d0 = yInitial ^ xInitial
				andi.l	#$80000000,d0	;d0 = sign of the result
				ori.l	#$7F800000,d0	;d0 = put Max exponent and so make Inf
				END_STACK_FRAME

FMUL_xExponent_Max_02:	;branch if xExponent is not maxim (#$FF)
				;if here, xExponent is #$FF(Max)
		tst.l	d0		;since xExponent is Max, test if Mantisa is 0(nr. is Inf)
		beq	x_Inf_02	;branch if mantisa is zero (x is Inf)
				;If here, x is qNan or sNan
			andi.l	#$00400000,d0		;test if Nan is Signaling (First mantisa bit 0)
			bz	x_Signaling_02		;branch if signaling
					;If here, x=qNan and y<>sNan, so return  qNan
				move.l	#$7FC00000,d0	;RESULT (d0) = qNan
				END_STACK_FRAME		;clear stack frame and restore data registers
			x_Signaling_02:
					;If here,y is sNan
				.ifdef FENV_SUPPORT
				SET_INVALID_FLAG		;signal INVALID
				.endif
				move.l	#$7FC00000,d0		;RESULT (d0) = qNan
				END_STACK_FRAME
		x_Inf_02:
				;Test if y is 0
			move.l	a1,d0
			andi.l	#$7FFFFFFF,d0		;compare y with 0
			bnz y_Non_Zero_02		;branch if not zero
					;here we have INF x ZERO , so signal invalid
				.ifdef FENV_SUPPORT
				SET_INVALID_FLAG	;set Invalid Flag if IEEE compliance, alters d7
				.endif
				move.l	#$7FC00000,d0	;RESULT (d0) = qNan
				END_STACK_FRAME		;clear stack frame and return d0 which is qNan
			y_Non_Zero_02:
				;x is inf, y is not zero or Nan or Inf
				;As long as y in Inf and x is not nan or zero is the same as x usual number
			y_Not_Zero_Nan_Inf_02:
				move.l	a0,d0
				move.l	a1,d7		;d7= yInitial
				eor.l	d7,d0		;d0 = yInitial ^ xInitial
				andi.l	#$80000000,d0	;d0 = sign of the result
				ori.l	#$7F800000,d0	;d0 = put Max exponent and so make Inf
				END_STACK_FRAME



;----------------------END _f_mul ------------------------------------
