;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:23 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _d_arith.s

;===============================================================================

;

;  The division routine for the ARM xM architecture 

;

#ifdef ARM_xM   



	#include "ARMsfpe.pre.asm"



;===============================================================================



	EXPORT _d_div

	EXPORT _ddiv                   ; FPLIB for _d_div

	EXPORT _drdiv



;===============================================================================

; FUNCTION: double _drdiv (double, double)

; DESCRIPTION: Reversed double division

; INPUT: 

; x=dxh/dxl,

; y=dyh/dyl, doubles

; OUTPUT: 

; dxh/dxl = result

	AREA |.text|,CODE,READONLY

_drdiv

; swap operands

        eor     dxh, dxh, dyh

        eor     dyh, dxh, dyh

        eor     dxh, dxh, dyh

        eor     dxl, dxl, dyl

        eor     dyl, dxl, dyl

        eor     dxl, dxl, dyl

; and fall through _d_div



;===============================================================================

; FUNCTION: _d_div

; DESCRIPTION: Double divide.

; INPUT: dxh/dxl = x, dyh/dyl = y

; OUTPUT: dxh/dxl = result of x / y

;



_ddiv                                  ; FPLIB entry point

_d_div

; Compute (-1)^x.sign * 2^x.exp-bias * 1.xxxx divided by

; (-1)^y.sign * 2^y.exp-bias * 1.yyyy --->

; result.sign           = eor of x and y signs

; result.exp (biased)   = x.exp - y.exp + bias

; result.bits           = w.zzzzz before normalization, rounding, etc.

; If the leading w bit is 0, then must decrement the exponent by 1

; and realign.



	MACRO
	DDIV_SAVE

        stmfd   sp!,{r4-r8,r10,sreg11}

	MEND



  #ifdef THUMB

	MACRO
	DDIV_RETURN

        ldmfd   sp!,{r4-r8,r10,sreg11}

        bx      lr

	MEND



	MACRO
	DDIV_RETURN_EQ

        ldmeqfd sp!,{r4-r8,r10,sreg11}

        bxeq    lr

	MEND



	MACRO
	DDIV_RETURN_NE

        ldmnefd sp!,{r4-r8,r10,sreg11}

        bxne    lr

	MEND



	MACRO
	DDIV_RETURN_CC

        ldmccfd sp!,{r4-r8,r10,sreg11}

        bxcc    lr

	MEND

  #else

	MACRO
	DDIV_RETURN

        ldmfd   sp!,{r4-r8,r10,pc}

	MEND



	MACRO
	DDIV_RETURN_EQ

        ldmeqfd sp!,{r4-r8,r10,pc}

	MEND



	MACRO
	DDIV_RETURN_NE

        ldmnefd sp!,{r4-r8,r10,pc}

	MEND



	MACRO
	DDIV_RETURN_CC

        ldmccfd sp!,{r4-r8,r10,pc}

	MEND

  #endif  



        DDIV_SAVE

        ldr     sreg11,=0xffe               ; Exponent mask

        eor     sreg04,dxh,dyh          ; MSB[sreg04] = result sign



	#ifdef IEEE_FULL_OR_FIXED

        mov     sreg08,dxh              ; save the original x - for user def'd handlers

        mov     sreg10,dxl

  #endif



        ands    sreg12,sreg11,dxh,LSR #19   ; r12 = exp of X << 1

        cmpne   sreg12,sreg11               ; check for x - nan, inf

        beq     ddiv_xSpecial



        bic     dxh,dxh,sreg11,LSL #20      ; clear exponent bits

        orr     dxh,dxh,#0x00100000     ; add lead 1 bit

        add     sreg12,sreg12,sreg04,LSR #31  ; LSB[sreg12] = result sign       

ddiv_xDenormReturn



        ands    sreg04,sreg11,dyh,LSR #19   ; r4 = exp of Y << 1

        cmpne   sreg04,sreg11               ; check for y - nan, inf

        beq     ddiv_ySpecial



        bic     dyh,dyh,sreg11,LSL #20      ; clear exponent bits

        orr     dyh,dyh,#0x00100000     ; add lead 1 bit

ddiv_yDenormReturn



        sub     sreg12,sreg12,sreg04    ; exp = expX - expY



        cmp     dxh,dyh                 ; align x so that x/y is in [1..2)

        cmpeq   dxl,dyl                 ; test ( x < y )



        mov     dyh,dyh,LSL #11         ; y <<= 11 (left-align y)

        orr     dyh,dyh,dyl,LSR #21

        mov     dyl,dyl,LSL #11



;

;       128 bit / 64 bit unsigned division routine using 32/16 divisions

;

;       INPUT: x = dxh:dxl:0:0 / y = dyh:dyl

;       OUTPUT: quotient = r4:r5, remainder = dxh:dxl

;       PRECONDITIONS: MSB of x == 0; MSB of y == 1

;

;       Algorithm (adapted from Knuth, Vol. 2): division using 16-bit "digits"

;       (Note: Digits of X are X(n)X(n-1)...X(0); of Y : Y(3)...Y(0) )

;

;          Repeat for each quotient digit Qi ( from Q(3) to Q(0) )

;             Qi := [X(n)X(n-1)] / Y(3)          # guess quotient digit ~Qi

;                                                # according to Knuth, Qi <= ~Qi <= Qi+2

;             if Qi >= 0x10000 then Qi := 0xFFFF

;             X := X -  Y * (Qi << 16*i)         # compute partial remainder X

;             if X < 0 then Qi := Qi-1; X := X + (Y << 16*i)

;             if X < 0 then Qi := Qi-1; X := X + (Y << 16*i)

;

;       Registers used: r0-r8, sreg11

;       X = dxh:dxl (only the top 64 bits of X are stored at any moment)

;       Y = r6[16..31]:dyh[16..31]:dyl

;       Q = r5:?? (iterations #1-2), r4:r5 (iterations #3-4)

;       r10 = current 16 bit quotient

;       sreg11 = indicates partial remainder overflow

;       r7-r8 = scratch registers

;

        mov     sreg04,#0               ; Q = 0

        mov     sreg05,#0

        mov     sreg06,dyh,LSR #16      ; r6 =  bits 0..15 of y

        eor     dyh,dyh,sreg06,LSL #16  ; dyh = bits 16..31 of y



        mov     sreg10,#0

        mov     sreg11,#0 



        bhs     ddiv_loopStart2         ; if ( x < y )

        adds    dxl,dxl,dxl             ;   x <<= 1

        adc     dxh,dxh,dxh

        sub     sreg12,sreg12,#2        ;   exp --

        b       ddiv_loopStart1         ; first 9 bits of Q are always zero



ddiv_loop

        mov     sreg11,#0 

        cmp     dxh,sreg06,LSL #16

        bhs     ddiv_overflow16

        mov     sreg10,#0

; Unrolled 32:16 division:  dxh/sreg06 -> q in sreg05, r in dxh 

        cmp     dxh,sreg06,LSL #15

        subhs   dxh,dxh,sreg06,LSL #15

        orrhs   sreg10,sreg10,#0x8000

        cmp     dxh,sreg06,LSL #14

        subhs   dxh,dxh,sreg06,LSL #14

        orrhs   sreg10,sreg10,#0x4000

        cmp     dxh,sreg06,LSL #13

        subhs   dxh,dxh,sreg06,LSL #13

        orrhs   sreg10,sreg10,#0x2000

        cmp     dxh,sreg06,LSL #12

        subhs   dxh,dxh,sreg06,LSL #12

        orrhs   sreg10,sreg10,#0x1000

        cmp     dxh,sreg06,LSL #11

        subhs   dxh,dxh,sreg06,LSL #11

        orrhs   sreg10,sreg10,#0x0800

        cmp     dxh,sreg06,LSL #10

        subhs   dxh,dxh,sreg06,LSL #10

        orrhs   sreg10,sreg10,#0x0400

        cmp     dxh,sreg06,LSL #9

        subhs   dxh,dxh,sreg06,LSL #9

        orrhs   sreg10,sreg10,#0x0200

        cmp     dxh,sreg06,LSL #8

        subhs   dxh,dxh,sreg06,LSL #8

        orrhs   sreg10,sreg10,#0x0100

        cmp     dxh,sreg06,LSL #7

        subhs   dxh,dxh,sreg06,LSL #7

        orrhs   sreg10,sreg10,#0x0080

ddiv_loopStart1

        cmp     dxh,sreg06,LSL #6

        subhs   dxh,dxh,sreg06,LSL #6

        orrhs   sreg10,sreg10,#0x0040

ddiv_loopStart2

        cmp     dxh,sreg06,LSL #5

        subhs   dxh,dxh,sreg06,LSL #5

        orrhs   sreg10,sreg10,#0x0020

        cmp     dxh,sreg06,LSL #4

        subhs   dxh,dxh,sreg06,LSL #4

        orrhs   sreg10,sreg10,#0x0010

        cmp     dxh,sreg06,LSL #3

        subhs   dxh,dxh,sreg06,LSL #3

        orrhs   sreg10,sreg10,#0x0008

        cmp     dxh,sreg06,LSL #2

        subhs   dxh,dxh,sreg06,LSL #2

        orrhs   sreg10,sreg10,#0x0004

        cmp     dxh,sreg06,LSL #1

        subhs   dxh,dxh,sreg06,LSL #1

        orrhs   sreg10,sreg10,#0x0002

        cmp     dxh,sreg06

        subhs   dxh,dxh,sreg06

        orrhs   sreg10,sreg10,#0x0001



        mov     dxh,dxh,LSL #16         ; dxh = [ Ri X(n-2) ]

        orr     dxh,dxh,dxl,LSR #16     ; dxl = [ X(n-1) 0 ]

        mov     dxl,dxl,LSL #16



ddiv_overflowReturn

;       X := X -  Y * (Qi << 16*i) 

;          = [Ri X(n-2)..X(0)] - [Y(2)Y(1)Y(0)] * (Qi << 16*i) 



; r7:r8 = [Y(2)Y(1)Y(0)] * q    

; Q = Q << 16 | q



        mov     sreg07,dyl,LSR #16      ; r7 = Y(1)

        sub     sreg08,dyl,sreg07,LSL #16  ; r8 = Y(0)

        mul     sreg08,sreg10,sreg08    ; r8 = Y(0) * q



        mov     sreg04,sreg04,LSL #16   ; Q = Q << 16 



        mul     sreg07,sreg10,sreg07    ; r7 = Y(1) * q



        orr     sreg04,sreg04,sreg05,LSR #16

        orr     sreg05,sreg10,sreg05,LSL #16  ; Q = Q | q



        adds    sreg08,sreg08,sreg07,LSL #16  ; r7:r8 = [Y(1)Y(0)] * q

        addcs   sreg07,sreg07,#0x10000

        mov     sreg07,sreg07,LSR #16 



        mla     sreg07,dyh,sreg10,sreg07  ; r7:r8 = [Y(2)Y(1)Y(0)] * q





; X := X - (Y * q)

        subs    dxl,dxl,sreg08 

        sbcs    dxh,dxh,sreg07

        sbcs    sreg11,sreg11,#0 

;       

;       { if X < 0 then Qi := Qi-1; X := X + (Y << 16*i) } - 2 times

;

        bpl     ddiv_postFix

        sub     sreg05,sreg05,#1

        adds    dxl,dxl,dyl

        add     sreg10,dyh,sreg06,LSL #16  ; restore dy

        adcs    dxh,dxh,sreg10

        bcs     ddiv_postFix

        sub     sreg05,sreg05,#1

        adds    dxl,dxl,dyl

        adc     dxh,dxh,sreg10

ddiv_postFix

        tst     sreg04,#0x00200000      ; is the loop over yet?

        beq     ddiv_loop



        cmp     sreg12,#0x800

        bge     ddiv_overflow

        add     sreg12,sreg12,#0x7f0    ; exp += 1023 - 1

        adds    sreg12,sreg12,#0x00C

        bmi     ddiv_underflow



        orrs    sreg06,dxh,dxl          ; save stickies



        mov     dxl,sreg05,LSR #1       ; pack everything

        orr     dxl,dxl,sreg04,LSL #31

        mov     dxh,sreg12,LSL #31      ; sign

        bic     sreg12,sreg12,#1        ; clear sign (sreg12 = exp << 1)

        orr     dxh,dxh,sreg04,LSR #1   ; mantissa

        add     dxh,dxh,sreg12,LSL #19  ; add exp



  #ifdef IEEE_FULL

        tsteq   sreg05,#1               ; check round bit

        DDIV_RETURN_EQ



        LFPState sreg03, sreg02

        SetInexact sreg03

        StFPState sreg03,sreg02 



        tst     sreg03, CHOP_OR_DOWN_BIT_MASK

        bne     ddiv_chopDownRound

        tst     sreg03, UP_OR_NEAR_BIT_MASK 

        bne     ddiv_upRound 



        cmp     sreg06,#0

  #endif

        tsteq   dxl,#1                  ; check LSB

        tstne   sreg05,#1               ; check round bit

        DDIV_RETURN_EQ

ddiv_bump

        adds    dxl,dxl,#1

        adc     dxh,dxh,#0 

	#ifdef IEEE_FULL_OR_FIXED

        mov     sreg02,dxh,LSL #1       ; look for overflow

        cmn     sreg02,#0x00200000 

        DDIV_RETURN_CC                  ; exit if overflow did not occur  

        SetOverflow sreg03

        StFPState sreg03,sreg02 

  #endif        

        DDIV_RETURN



  #ifdef IEEE_FULL

ddiv_chopDownRound

        tst     sreg03, ZERO_OR_DOWN_BIT_MASK 

        DDIV_RETURN_NE

        tst     dxh,#0x80000000

        bne     ddiv_bump

        DDIV_RETURN



ddiv_upRound

        tst     dxh,#0x80000000         ; positive? bump

        beq     ddiv_bump

        DDIV_RETURN



  #endif



;;;;;;;;;;;;;; Overflows during 128 : 64 division



; The overflows appear when X(n) == Y(3); 

;       if Qi >= 0x10000 then 

;            Qi := 0xFFFF; 

;            Ri := X(n)X(n-1) - X(n) * (2^16-1) 

;                = X(n) + X(n-1)



; IN: dxh = [X(n)X(n-1)] (AB), dxl = [X(n-2)X(n-3)] (CD)

; OUT: dxh = [Ri.lo X(n-2)] (A+B|C), dxl = X(n-3) << 16 (D0), sreg11 = 1 if A+B carries

ddiv_overflow16

        mov     sreg10,#0x00FF          ; q = 0xFFFF

        orr     sreg10,sreg10,#0xFF00

        adds    dxh,dxh,dxh,LSL #16     ; dxh = A+B|B

        adc     sreg11,sreg11,#0                ; save carry

        bic     dxh,dxh,#0x00FF 

        bic     dxh,dxh,#0xFF00

        orr     dxh,dxh,dxl,LSR #16     ; dxh = A+B|C

        mov     dxl,dxl,LSL #16         ; dxl = D0

        b       ddiv_overflowReturn



ddiv_overflow

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg03, sreg02

        SetOverflowL sreg03,__wrap_overflow_handler_pack

        StFPState sreg03,sreg02 

  #endif

        movs    dxh,sreg12,LSL #31      ; 

        orr     dxh,dxh,#0x7F000000     ; return signed Inf

        orr     dxh,dxh,#0x00F00000

        mov     dxl,#0

  #ifdef IEEE_FULL

        bmi     ddiv_overflowMinus

        tst     sreg03,CHOP_OR_DOWN_BIT_MASK

        DDIV_RETURN_EQ

        subs    dxl,dxl,#1

        sbc     dxh,dxh,#0

        DDIV_RETURN



ddiv_overflowMinus

        tst     sreg03,CHOP_OR_UP_BIT_MASK

        DDIV_RETURN_EQ

        subs    dxl,dxl,#1

        sbc     dxh,dxh,#0

  #endif        

        DDIV_RETURN



ddiv_underflow

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg06, sreg06

        tst     sreg06, FE_IEEE_MASK_UNDERFLOW

        bne     __wrap_underflow_handler

  #endif

  #ifndef IEEE_STD

        mvn     sreg06,sreg12,ASR #1    ; r6 = # of shifts - 2

        cmp     sreg06,#52

        bgt     ddiv_trueUnderflow

        beq     ddiv_veryTiny



        orrs    dxh,dxl,dxh             ; keep only 1 sticky bit

        orrne   sreg05,sreg05,#1

        mov     dxh,sreg12,LSL #31      ; put sign into high word



        cmp     sreg06,#20

        bgt     ddiv_smallDenorm



        add     sreg06,sreg06,#2        ; r6 = #shifts

        orr     dxh,dxh,sreg04,LSR sreg06  ; dxh:dxl |= r4:r5 >> r6

        mov     dxl,sreg05,LSR sreg06

        rsb     sreg06,sreg06,#32

        orr     dxl,dxl,sreg04,LSL sreg06 

        mov     sreg04,sreg05,LSL sreg06  ; r4 = round | stickies

        b       ddiv_denormRound



ddiv_smallDenorm

        rsb     sreg06,sreg06,#52       ; r6 = 32 - #shifts

        mov     dxl,sreg04,LSL #10

        orr     dxl,dxl,sreg05,LSR #22

        movs    sreg05,sreg05,LSL #10   ; put leftover stickies in r5

        movne   sreg05,#0x80000000

        mov     sreg04,dxl,LSL sreg06   ; r4 = round | stickies

        rsb     sreg06,sreg06,#32

        mov     dxl,dxl,LSR sreg06

        orr     sreg04,sreg04,sreg05,LSR sreg06  ; add the leftover stickies



ddiv_denormRound

	#ifdef IEEE_FULL_OR_FIXED

        cmp     sreg04,#0

        DDIV_RETURN_EQ

        LFPState sreg03, sreg02

        SetUnderflow sreg03

        StFPState sreg03,sreg02 

    #endif      

    #ifdef IEEE_FULL

        tst     sreg03, CHOP_OR_DOWN_BIT_MASK

        bne     ddiv_denormDownRound

        tst     sreg03, UP_OR_NEAR_BIT_MASK 

        bne     ddiv_denormUpRound 

    #endif

        tst     sreg04,#0x80000000

        DDIV_RETURN_EQ

        movs    sreg04,sreg04,LSL #1

        tsteq   dxl,#1

        DDIV_RETURN_EQ

        adds    dxl,dxl,#1

        adc     dxh,dxh,#0

        DDIV_RETURN

    #ifdef IEEE_FULL

ddiv_denormUpRound

        eor     sreg04,dxh,#0x80000000

        adds    dxl,dxl,sreg04,LSR #31

        adc     dxh,dxh,#0

        DDIV_RETURN

ddiv_denormDownRound

        tst     sreg03, ZERO_OR_DOWN_BIT_MASK 

        DDIV_RETURN_NE

        adds    dxl,dxl,dxh,LSR #31

        adc     dxh,dxh,#0

        DDIV_RETURN

    #endif



ddiv_veryTiny

        orrs    sreg06,dxh,dxl          ; remainder !=0 ? return denorm

        orreqs  sreg06,sreg05,sreg04,LSL #11  ; quotient != 2^-52 ? return denorm

        mov     dxh,sreg12,LSL #31      ; return the lowest denorm or 0

        mov     dxl,#0

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg03, sreg02

        SetUnderflow sreg03

        StFPState sreg03,sreg02 

    #endif

    #ifdef IEEE_FULL

        tst     sreg03, CHOP_OR_DOWN_BIT_MASK

        bne     ddiv_vTinyDownRound

        tst     sreg03, UP_OR_NEAR_BIT_MASK 

        bne     ddiv_vTinyUpRound

        cmp     sreg06,#0 

    #endif

        movne   dxl,#1

        DDIV_RETURN

    #ifdef IEEE_FULL

ddiv_vTinyUpRound

        mov     dxl,dxh,LSR #31         ; return 1 if sign bit == 0

        eor     dxl,dxl,#1

        DDIV_RETURN

ddiv_vTinyDownRound

        tst     sreg03, ZERO_OR_DOWN_BIT_MASK 

        moveq   dxl,dxh,LSR #31         ; return 1 if directed and sign bit == 1

        DDIV_RETURN

    #endif





ddiv_trueUnderflow

        mov     dxh,sreg12,LSL #31      ; return signed 0

        mov     dxl,#0

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg03, sreg02

        SetUnderflow sreg03

        StFPState sreg03,sreg02 

    #endif

    #ifdef IEEE_FULL

        and     sreg03,sreg03,ROUND_MODE_MASK  ; handle directed modes

        cmp     sreg03,FE_TONEAREST

        cmpne   sreg03,FE_TOWARDZERO

        DDIV_RETURN_EQ

        cmp     sreg03,FE_UPWARD

        tsteq   dxh,#0x80000000

        addeq   dxl,dxl,#1              ; increment if directed up and positive

        cmp     sreg03,FE_UPWARD

        tstne   dxh,#0x80000000

        addne   dxl,dxl,#1              ; increment if directed down and negative

    #endif

        DDIV_RETURN

  #else

        mov     dxh,sreg12,LSL #31      ; return signed 0 in flush-to-zero mode

        mov     dxl,#0

        DDIV_RETURN

  #endif        



ddiv_xSpecial

; dxh:dxl, dyh:dyl = original numbers

; MSB[sreg04] = result sign

; r12 = exp of X << 1

; sreg11 = 0xffe

  #ifdef IEEE_STD

        cmp     sreg12,#0

        beq     ddiv_xZero

  #else

        orrs    sreg05,dxl,dxh,LSL #1   ; check for zero

        beq     ddiv_xZero

        cmp     sreg12,sreg11               ; check for Inf/NaN

        beq     ddiv_xMaxExp



; Denormalized X

        movs    dxh,dxh,LSL #12

        beq     ddiv_xSmallDenorm 



        Normalize dxh,sreg12            ; sreg12 - exponent

        add     sreg05,sreg12,#31

        mov     dxh,dxh,LSR #11

        orr     dxh,dxh,dxl,LSR sreg05

        rsb     sreg05,sreg05,#32

        mov     dxl,dxl,LSL sreg05



        mov     sreg12,sreg12,LSL #1    ; LSB[r12] = sign

        orr     sreg12,sreg12,sreg04,LSR #31 

        b       ddiv_xDenormReturn



ddiv_xSmallDenorm

        mvn     sreg12,#19              ;  normalize the denorm and return to main

        Normalize dxl,sreg12

        mov     dxh,dxl,LSR #11

        mov     dxl,dxl,LSL #21



        mov     sreg12,sreg12,LSL #1    ; LSB[r12] = sign

        orr     sreg12,sreg12,sreg04,LSR #31 

        b       ddiv_xDenormReturn

  #endif        

ddiv_xMaxExp

        orrs    sreg05,dxl,dxh,LSL #12  ; check for NaN

        bne     ddiv_xNaN



        bic     sreg05,dyh,#0x80000000

        cmp     sreg05,sreg11,LSL #19       ; check if y is Inf / NaN

        bhs     ddiv_xInf_yMax



        and     sreg05,dyh,#0x80000000

        eor     dxh,sreg05,dxh          ; Inf / x -> Inf (signed)

        DDIV_RETURN



ddiv_xInf_yMax

        orrs    sreg05,dyl,dyh,LSL #12  ; check if y is NaN

        bne     ddiv_yNaN

        b       ddiv_invalid            ; Inf / Inf -> invalid



ddiv_ySpecial

  #ifdef IEEE_STD

        cmp     sreg04,#0

        beq     ddiv_byZero

  #else

        orrs    sreg05,dyl,dyh,LSL #1   ; check for x / 0

        beq     ddiv_byZero 

        cmp     sreg04,sreg11

        beq     ddiv_yMaxExp



; Denormalized Y

        movs    dyh,dyh,LSL #12

        beq     ddiv_ySmallDenorm 



        Normalize dyh,sreg04            ; r4 - exponent

        add     sreg05,sreg04,#31

        mov     dyh,dyh,LSR #11

        orr     dyh,dyh,dyl,LSR sreg05

        rsb     sreg05,sreg05,#32

        mov     dyl,dyl,LSL sreg05 

        mov     sreg04,sreg04,LSL #1    ; 

        b       ddiv_yDenormReturn



ddiv_ySmallDenorm

        mvn     sreg04,#19              ;  normalize the denorm and return to main

        Normalize dyl,sreg04

        mov     dyh,dyl,LSR #11

        mov     dyl,dyl,LSL #21

        mov     sreg04,sreg04,LSL #1

        b       ddiv_yDenormReturn 

  #endif

ddiv_yMaxExp

	#ifdef IEEE_FULL_OR_FIXED

        mov     dxh,sreg08

        mov     dxl,sreg10

  #endif

        orrs    sreg05,dyl,dyh,LSL #12  ; check for NaN

        bne     ddiv_yNaN

        mov     dxh,sreg12,LSL #31      ; x / Inf = 0 (signed)

        mov     dxl,#0

        DDIV_RETURN



ddiv_byZero

	#ifdef IEEE_FULL_OR_FIXED

        mov     dxh,sreg08

        mov     dxl,sreg10

        LFPState sreg04, sreg05

        SetDivByZero sreg04

        StFPState sreg04,sreg05 

  #endif

        mov     dxh,sreg12,LSL #31      ; x / 0 -> signed Inf

        orr     dxh,dxh,sreg11,LSL #19

        mov     dxl,#0

        DDIV_RETURN



ddiv_xZero

  #ifdef IEEE_STD

        tst     sreg11,dyh,LSR #19

  #else 

        orrs    sreg05,dyl,dyh,LSL #1   ; check 0 / 0 -> invalid

  #endif        

        beq     ddiv_invalid

        bic     sreg05,dyh,#0x80000000

        cmp     sreg05,sreg11,LSL #19       ; check 0 / NaN

        cmpeq   dyl,#0

        bhi     ddiv_yNaN

        eor     dxh,dxh,dyh             ; 0 / x = 0 (signed)

        and     dxh,dxh,#0x80000000

  #ifdef IEEE_STD

        mov     dxl,#0

  #endif        

        DDIV_RETURN



ddiv_xNaN

        tst     dxh,#0x00080000         ; signaling? invalid

        beq     ddiv_invalid

        bic     sreg05,dyh,#0x80000000

        cmp     sreg05,sreg11,LSL #19       ; check if y is NaN

        cmpeq   dyl,#0

        bhi     ddiv_yNaN

        DDIV_RETURN



ddiv_yNaN

        tst     dyh,#0x00080000         ; signaling? invalid

        beq     ddiv_invalid

        mov     dxh,dyh                 ; return y

        mov     dxl,dyl

        DDIV_RETURN





ddiv_invalid

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg04, sreg05

        SetInvalidE sreg04

        StFPState sreg04,sreg05 

  #endif

ddiv_qnan

        orr     dxh,dxh,#0x7f000000

        orr     dxh,dxh,#0x00f80000     ; return a quiet NaN

        DDIV_RETURN



	#ifdef IEEE_FULL_OR_FIXED

	IMPORT __fp_handler_64_to_64

	IMPORT __fp_handler_binary_64_to_64



; overflow exception handler

__wrap_overflow_handler

        and     dxh,dxh,#0x80000000

        orr     dxh,dxh,#0x10000000

        orr     dxh,dxh,#0x0ff00000

        mov     dxl,#0

        ldr     r2, =(FE_EX_OVERFLOW :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_DIV)

        bl      __fp_handler_64_to_64

        DDIV_RETURN



; overflow exception handler with pack

__wrap_overflow_handler_pack

        sub     sreg12, sreg12, #0x400

        sub     sreg12, sreg12, #0x4    ; coerce exponent       



        mov     dxl,sreg05,LSR #1       ; pack everything

        orr     dxl,dxl,sreg04,LSL #31

        mov     dxh,sreg12,LSL #31      ; sign

        bic     sreg12,sreg12,#1        ; clear sign (sreg12 = exp << 1)

        orr     dxh,dxh,sreg04,LSR #1   ; mantissa

        add     dxh,dxh,sreg12,LSL #19  ; add exp



        ldr     r2, =(FE_EX_OVERFLOW :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_DIV)

        bl      __fp_handler_64_to_64

        DDIV_RETURN



; underflow exception handler with pack

__wrap_underflow_handler

        add     sreg12, sreg12, #0xC00



        mov     dxl,sreg05,LSR #1       ; pack everything

        orr     dxl,dxl,sreg04,LSL #31

        mov     dxh,sreg12,LSL #31      ; sign

        bic     sreg12,sreg12,#1        ; clear sign (sreg12 = exp << 1)

        orr     dxh,dxh,sreg04,LSR #1   ; mantissa

        add     dxh,dxh,sreg12,LSL #19  ; add exp



        ldr     r2, =(FE_EX_UNDERFLOW :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_DIV)

        bl      __fp_handler_64_to_64

        DDIV_RETURN



; division by zero exception handler

__wrap_divbyzero_handler

        ldr     r12, =(FE_EX_DIVBYZERO :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_DIV)

        stmfd   sp!, {sreg12}

        bl      __fp_handler_binary_64_to_64

        add     sp, sp, #4

        DDIV_RETURN



; invalid exception handler

__wrap_invalid_handler

        ldr     r12, =(FE_EX_INVALID :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_DIV)

        stmfd   sp!, {sreg12}

        bl      __fp_handler_binary_64_to_64

        add     sp, sp, #4 

        DDIV_RETURN



  #endif        



  #ifdef IEEE_FULL

; inexact exception wrapper

__wrap_inexact_handler

        ldr     r2, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_DIV)

        bl      __fp_handler_64_to_64

        DDIV_RETURN

  #endif



#endif
	END

/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */
