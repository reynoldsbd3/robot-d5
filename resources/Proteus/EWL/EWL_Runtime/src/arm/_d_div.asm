;===============================================================================
;===
;===	ARM Floating Point Emulation Library 
;===	Copyright  2009 Freescale Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:23 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _d_div.s

;===============================================================================

;

;  Uses umull and umlal instructions. 

;  The division routine for the ARM xM architecture are found in _d_div_xm.s

;

#ifndef ARM_xM  

	#include "ARMsfpe.pre.asm"



;===============================================================================



        EXPORT _d_div

        EXPORT _ddiv                   ; FPLIB for _d_div

        EXPORT _drdiv



#ifdef VFP_AND_NOT_IEEE_STD
      	EXPORT _vfp_support_ddiv
#endif


;===============================================================================
; FUNCTION: double _drdiv (double, double)
; DESCRIPTION: Reversed double division
; INPUT: 
; x=dxh/dxl,
; y=dyh/dyl, doubles
; OUTPUT: 
; dxh/dxl = result
;===============================================================================

; FUNCTION: _d_div
; DESCRIPTION: Double divide.
; INPUT: dxh/dxl = x, dyh/dyl = y
; OUTPUT: dxh/dxl = result of x / y
;

	AREA |.text|,CODE,READONLY	


#ifdef VFP


_ddiv

_d_div

        fmdhr   d0, dxh
        fmdlr   d0, dxl
        fmdhr   d1, dyh
        fmdlr   d1, dyl
        fdivd   d0, d0, d1
        fmrdh   dxh, d0
        fmrdl   dxl, d0

#ifdef THUMB

        bx      lr

#else

        mov     pc,lr

#endif 
       


_drdiv

        fmdhr   d0, dxh
        fmdlr   d0, dxl
        fmdhr   d1, dyh
        fmdlr   d1, dyl
        fdivd   d0, d1, d0
        fmrdh   dxh, d0
        fmrdl   dxl, d0

#ifdef THUMB

        bx      lr

#else

        mov     pc,lr

#endif        



_vfp_support_ddiv

#else

_drdiv

; swap operands

        eor     dxh, dxh, dyh
        eor     dyh, dxh, dyh
        eor     dxh, dxh, dyh
        eor     dxl, dxl, dyl
        eor     dyl, dxl, dyl
        eor     dxl, dxl, dyl

; and fall through _d_div


_ddiv

_d_div
; FPLIB entry point

#endif



#ifdef NOT_VFP_OR_NOT_IEEE_STD



; Compute (-1)^x.sign * 2^x.exp-bias * 1.xxxx divided by
; (-1)^y.sign * 2^y.exp-bias * 1.yyyy --->
; result.sign           = eor of x and y signs
; result.exp (biased)   = x.exp - y.exp + bias
; result.bits           = w.zzzzz before normalization, rounding, etc.
; If the leading w bit is 0, then must decrement the exponent by 1
; and realign.



#ifdef IEEE_FULL_OR_FIXED

#ifdef USING_ARMASM
#define Clrstk(x) CLRSTK #x 
#else
#define Clrstk(x) CLRSTK ##x
#endif


	MACRO
	DDIV_SAVE

        stmfd   sp!, {r4-r6,r7, sreg08, dxh, dxl, dyh, dyl}

	MEND


#else

#define Clrstk(x)

	MACRO
	DDIV_SAVE

        stmfd   sp!, {r4-r6,r7,sreg08}

       	MEND 

#endif                


; && !defined(ARM5)
#ifdef THUMB

	MACRO
	DDIV_RETURN

        Clrstk(al)

        ldmfd   sp!,{r4-r6,r7,sreg08}
        bx      lr

	MEND


	MACRO
	DDIV_RETURN_EQ

        Clrstk(eq)

        ldmeqfd sp!,{r4-r6,r7,sreg08}
        bxeq    lr

	MEND


	MACRO
	DDIV_RETURN_NE

        Clrstk(ne)

        ldmnefd sp!,{r4-r6,r7,sreg08}
        bxne    lr

	MEND


	MACRO
	DDIV_RETURN_CC

        Clrstk(cc)

        ldmccfd sp!,{r4-r6,r7,sreg08}
        bxcc    lr

	MEND


	MACRO
	Ret_restore

        ldmfd   sp!, {r4-r6,r7,sreg08}
        bx      lr

	MEND



#else

	MACRO
	DDIV_RETURN

        Clrstk(al)

        ldmfd   sp!,{r4-r6,r7,pc}

	MEND


	MACRO
	DDIV_RETURN_EQ

        Clrstk(eq)

        ldmeqfd sp!,{r4-r6,r7,pc}

	MEND



	MACRO
	DDIV_RETURN_NE

        Clrstk(ne)

        ldmnefd sp!,{r4-r6,r7,pc}

	MEND



	MACRO
	DDIV_RETURN_CC

        Clrstk(cc)

        ldmccfd sp!,{r4-r6,r7,pc}

	MEND



	MACRO
	Ret_restore

        ldmfd   sp!, {r4-r6,r7,pc}

	MEND

#endif  



        DDIV_SAVE

        ldr     sreg08,=0xffe               ; Exponent mask
        eor     sreg04,dxh,dyh          ; MSB[sreg04] = result sign
        ands    sreg12,sreg08,dxh,LSR #19   ; r12 = exp of X << 1
        cmpne   sreg12,sreg08               ; check for x - nan, inf
        beq.w     ddiv_xSpecial
        bic     dxh,dxh,sreg08,LSL #20      ; clear exponent bits
        orr     dxh,dxh,#0x00100000     ; add lead 1 bit
        add     sreg12,sreg12,sreg04,LSR #31  ; LSB[sreg12] = result sign       

ddiv_xDenormReturn

        ands    sreg04,sreg08,dyh,LSR #19   ; r4 = exp of Y << 1
        cmpne   sreg04,sreg08               ; check for y - nan, inf
        beq.w     ddiv_ySpecial
        bic     dyh,dyh,sreg08,LSL #20      ; clear exponent bits
        orr     dyh,dyh,#0x00100000     ; add lead 1 bit

ddiv_yDenormReturn

        sub     sreg12,sreg12,sreg04    ; exp = expX - expY
        cmp     dxh,dyh                 ; align x so that x/y is in [1..2)
        cmpeq   dxl,dyl                 ; test ( x < y )
        bhs     ddiv_start              ; if ( x < y )
        adds    dxl,dxl,dxl             ;   x <<= 1
        adc     dxh,dxh,dxh
        sub     sreg12,sreg12,#2        ;   exp --

; The Newton-Raphson division algorithm:
; --------------------------------------
;  x0 = approximately 1/y - computed using an 8-bit lookup table
;  x1 = x0 * (2 + (-y)*x0)
;  x2 = x1 + x1 * (1 + (-y)*x1)
;  q2 = x2 * x
;   q = q2 + x2 * (x + (-y) * q2)
; --------------------------------------
; C fixed point implementation:
;
;  if (x<y) 
;    {
;    x <<= 1;
;    exp --;
;    }
;
;  x0 = table[(hi(y) & 0x000fffff) >> (20 - TABLEBITS)]
;
;  mY = -y
;  k1 = 0x20000000 + x0 * hi(mY)
;  x1 = x0 * (k1 >> 7) >> 13
;
;  k2 = x1 * hi(mY << 10)
;  k3 = x1 * (k2 >> 16)
;  x2 = (x1 << 14) + (k3 >> 16)
;
;  q2 = hi(x2 * hi(x<<10))
;  k4 = lo(mY * q2 >> 26)
;  k5 = (lo(x) << 2) + k4
;  k6:rbits = k5 * x2
;  sigQ = (q2<<24) + k6
;
; Registers @ this point
;
;  dxh/dxl = x
;  dyh/dyl = y
;  r12 = exp / sign


ddiv_start

#ifdef IEEE_FULL_OR_FIXED

;;;        add     sreg04,pc,#(rtable - . - 8 - 256)
;;;        add.w     sreg04,pc,#(rtable - . - 4 - 256)
	adr.w	sreg04,rtable
	sub.w 	sreg04,sreg04,#0x100

#else

;;;     add.w     sreg04,pc,#(rtable - . - 4 - 256)
	adr.w	sreg04,rtable
	sub.w 	sreg04,sreg04,#0x100

#endif        

;;;        ldrb    sreg08,[sreg04,dyh,LSR #12]  ; x0 = table[(hi(y) & 0x000fffff) >> (20 - 8)] 
	mov	sreg08,dyh,LSR #12
	ldrb	sreg08,[sreg04,sreg08]

        rsbs    dyl,dyl,#0              ; mY = -y
;;;        rsc     dyh,dyh,#0
	rsb	dyh,dyh,#0
	subcc	dyh,dyh,#1	; substract 1 more if Carry Clear from rsbs

        mov     sreg04,#0x20000000      ; k1 = 0x20000000 + x0 * hi(mY)
        mla     sreg05,sreg08,dyh,sreg04
        mov     sreg06,dyh,LSL #10      ; ...computing hi(mY << 10)
        mov     sreg05,sreg05,LSR #7    ; x1 = x0 * (k1 >> 7) >> 13     
        mul     sreg08,sreg05,sreg08
        orr     sreg06,sreg06,dyl,LSR #22  ; ...computing hi(mY << 10)
        mov     sreg08,sreg08,LSR #13
        mul     sreg05,sreg08,sreg06        ; k2 = x1 * hi(mY << 10)
        mov     sreg06,dxh,LSL #10      ; ...computing hi(x<<10)
        orr     sreg06,sreg06,dxl,LSR #22
        mov     sreg05,sreg05,LSR #16   ; k3 = x1 * (k2 >> 16)
        mul     sreg05,sreg08,sreg05
        mov     sreg08,sreg08,LSL #14           ; x2 = (x1 << 14) + (k3 >> 16)
        add     sreg08,sreg08,sreg05,LSR #16
        umull   sreg05,sreg06,sreg08,sreg06  ; q2 = hi(x2 * hi(x<<10))
        umull   sreg04,sreg05,sreg06,dyl  ; k4 = lo(mY * q2 >> 26)
        mla     sreg05,dyh,sreg06,sreg05
        mov     sreg04,sreg04,LSR #26
        orr     sreg04,sreg04,sreg05,LSL #6
        add     sreg04,sreg04,dxl,LSL #2  ; k5 = (lo(x) << 2) + k4
        umull   sreg08,sreg05,sreg04,sreg08     ; k6:rbits = k5 * x2
        mov     sreg04,#0
        adds    sreg05,sreg05,sreg06,LSL #24  ; sigQ = (q2<<24) + k6
        adc     sreg04,sreg04,sreg06,LSR #8

;
; Registers @ this point
;
;  dxh:dxl = -x
;  dyh:dyl = y
;  r4:r5 = q (mantissa, not rounded)
;  r12 = exp / sign
;  sreg08 -> MSB is round bit

        cmp     sreg12,#0x800
        bge.w     ddiv_overflow
        add     sreg12,sreg12,#0x7f0    ; exp += 1023 - 1
        adds    sreg12,sreg12,#0x00C
        bmi.w     ddiv_underflow 
        orr     dxh,sreg04,sreg12,LSL #31  ; pack sign
        bic     sreg12,sreg12,#1        ; clear sign (sreg12 = exp << 1)
        add     dxh,dxh,sreg12,LSL #19  ; add exp

#ifdef IEEE_FULL

        rsbs    dyl,dyl,#0              ; compute y = -mY
        mul     sreg08,dyl,sreg04           ; compute remainder: x - y * q
        mov     sreg06,#0
        umlal   sreg06,sreg08,dyl,sreg05
;;;        rsc     dyh,dyh,#0
	rsb	dyh,dyh,#0
	subcc	dyh,dyh,#1

        mla     sreg08,sreg05,dyh,sreg08        ; sreg08:r6 = y * q 
        cmp     sreg08,dxl,LSL #20          ; remainder == 0 ? exact
        cmpeq   sreg06,#0
        beq     ddiv_nobump
        adds    sreg04,sreg06,dyl       ; r12:r4 = y * (q + 1 ulp)
        adc     sreg12,sreg08,dyh
        cmp     sreg12,dxl,LSL #20
        bmi     ddiv_plus1ulp
        cmpeq   sreg04,#0
        beq     ddiv_bump               ; exact ? return
        b       ddiv_inexact


ddiv_plus1ulp

        mov     sreg08,sreg12               ; the real chopped q is 1ulp larger
        mov     sreg06,sreg04
        adds    sreg05,sreg05,#1        ; increment also the result
        adc     dxh,dxh,#0 

ddiv_inexact

        adds    sreg06,sreg06,sreg06    ; dyh:dyl = b + (q * 0.5 ulp)
        adc     sreg08,sreg08,sreg08
        adds    sreg06,sreg06,dyl
        adc     sreg08,sreg08,dyh
        LFPState sreg03, sreg02
        SetInexact sreg03
        StFPState sreg03,sreg02 
        tst     sreg03, CHOP_OR_DOWN_BIT_MASK
        bne.w     ddiv_chopDownRound
        tst     sreg03, UP_OR_NEAR_BIT_MASK 
        bne.w     ddiv_upRound 
        cmp     sreg08,dxl,LSL #21          ; bump if remainder is positive
        bmi     ddiv_bump 

 #else

        tst     sreg08,#0x80000000          ; test round bit
        bne     ddiv_bump               ; if 1 - increment      
        rsbs    dyl,dyl,#0              ; compute y = -mY
        mov     sreg04,sreg04,LSL #1    ; compute remainder: x - y * (q + 0.5 ulp)
        add     sreg04,sreg04,sreg05,LSR #31
        mul     sreg08,dyl,sreg04
        mov     sreg06,#0
        mov     sreg04,sreg05,LSL #1
        orr     sreg04,sreg04,#1
        umlal   sreg06,sreg08,sreg04,dyl
;;;        rsc     dyh,dyh,#0
	rsb	dyh,dyh,#0
	subcc	dyh,dyh,#1

        mla     sreg08,sreg04,dyh,sreg08        ; sreg08:r6 = y * (q + 0.5 ulp)
        cmp     sreg08,dxl,LSL #21          ; bump if remainder is positive
        bmi     ddiv_bump 

#endif



ddiv_nobump

        mov     dxl,sreg05

        DDIV_RETURN

ddiv_bump

        adds    dxl,sreg05,#1

        adc     dxh,dxh,#0 

   #ifdef IEEE_FULL_OR_FIXED

        mov     sreg02,dxh,LSL #1       ; look for overflow

        cmn     sreg02,#0x00200000 

        DDIV_RETURN_CC                  ; exit if overflow did not occur  

        LFPState sreg03,sreg12

        SetOverflow sreg03

        StFPState sreg03,sreg12

    #endif        

        DDIV_RETURN



; initial aproximation for reciprocal (256 entries)  

rtable

        DCB   255, 255, 254, 253, 252, 251, 250, 249

        DCB   248, 247, 246, 245, 244, 243, 242, 241

        DCB   240, 240, 239, 238, 237, 236, 235, 234

        DCB   234, 233, 232, 231, 230, 230, 229, 228

        DCB   227, 226, 226, 225, 224, 223, 223, 222

        DCB   221, 220, 220, 219, 218, 217, 217, 216

        DCB   215, 215, 214, 213, 212, 212, 211, 210

        DCB   210, 209, 208, 208, 207, 206, 206, 205

        DCB   204, 204, 203, 203, 202, 201, 201, 200

        DCB   200, 199, 198, 198, 197, 197, 196, 195

        DCB   195, 194, 194, 193, 192, 192, 191, 191

        DCB   190, 190, 189, 189, 188, 188, 187, 186

        DCB   186, 185, 185, 184, 184, 183, 183, 182

        DCB   182, 181, 181, 180, 180, 179, 179, 178

        DCB   178, 177, 177, 176, 176, 175, 175, 175

        DCB   174, 174, 173, 173, 172, 172, 171, 171

        DCB   170, 170, 170, 169, 169, 168, 168, 167

        DCB   167, 167, 166, 166, 165, 165, 164, 164

        DCB   164, 163, 163, 162, 162, 162, 161, 161

        DCB   160, 160, 160, 159, 159, 158, 158, 158

        DCB   157, 157, 157, 156, 156, 155, 155, 155

        DCB   154, 154, 154, 153, 153, 153, 152, 152

        DCB   152, 151, 151, 150, 150, 150, 149, 149

        DCB   149, 148, 148, 148, 147, 147, 147, 146

        DCB   146, 146, 145, 145, 145, 145, 144, 144

        DCB   144, 143, 143, 143, 142, 142, 142, 141

        DCB   141, 141, 140, 140, 140, 140, 139, 139

        DCB   139, 138, 138, 138, 138, 137, 137, 137

        DCB   136, 136, 136, 136, 135, 135, 135, 134

        DCB   134, 134, 134, 133, 133, 133, 133, 132

        DCB   132, 132, 131, 131, 131, 131, 130, 130

        DCB   130, 130, 129, 129, 129, 129, 128, 128



    #ifdef IEEE_FULL

ddiv_chopDownRound

        tst     sreg03, ZERO_OR_DOWN_BIT_MASK 

        bne     ddiv_nobump

        tst     dxh,#0x80000000

        bne     ddiv_bump

        b       ddiv_nobump



ddiv_upRound

        tst     dxh,#0x80000000         ; positive? bump

        beq     ddiv_bump

        b       ddiv_nobump

    #endif



ddiv_overflow

    #ifdef IEEE_FULL_OR_FIXED

        LFPState sreg03, sreg02

        SetOverflowL sreg03, __wrap_overflow_handler_pack

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

;;;        tst     sreg06, FE_IEEE_MASK_UNDERFLOW
	mov 	sreg07,#1
	tst	sreg06, sreg07, LSL #11

        bne.w     __wrap_underflow_handler

    #endif

    #ifndef IEEE_STD

        mvn     sreg06,sreg12,ASR #1    ; r6 = # of shifts - 1

        cmp     sreg06,#52

        bgt.w     ddiv_trueUnderflow

        beq.w     ddiv_veryTiny



        cmp     sreg06,#20

        bge     ddiv_smallDenorm



        rsb     sreg06,sreg06,#19       ; r6 = 20 - #

        mov     sreg08,dxl,LSL sreg06       ; r12:sreg08 = x << (52 - #)



        rsb     sreg06,sreg06,#20       ; r6 = #

        mov     dxl,sreg05,LSR sreg06   ; r4:dxl = sigQ >> #

        rsb     sreg06,sreg06,#32

;;;	orr	dxl,dxl,sreg04,LSL sreg06
;;;	use dxh (r1) as scratch here

	mov	dxh,sreg04,LSL sreg06
        orr     dxl,dxl,dxh

        rsb     sreg06,sreg06,#32

        mov     sreg04,sreg04,LSR sreg06



        orr     dxh,sreg04,sreg12,LSL #31  ; pack sign

        mov     sreg12,sreg08

        mov     sreg08,#0

        b       ddiv_denormRound 



ddiv_smallDenorm

        rsb     sreg06,sreg06,#51       ; r6 = 52 - #

        mov     sreg08,dxh,LSL sreg06       ; r12:sreg08 = x << (52 - #)        

        mov     dxh,sreg12,LSL #31      ; put sign      

        rsb     sreg06,sreg06,#32       ; r6 = # - 20   

;;; ARM only        orr     sreg12,sreg08,dxl,LSR sreg06

	mov 	sreg07,dxl,LSR sreg06
        orr     sreg12,sreg08,sreg07

        rsb     sreg06,sreg06,#32       ; r6 = 52 - #

        mov     sreg08,dxl,LSL sreg06 



        mov     sreg05,sreg05,LSR #21   ; r4:dxl = sigQ >> #

        orr     sreg05,sreg05,sreg04,LSL #11

        rsb     sreg06,sreg06,#31       ; r6 = # - 21

        mov     dxl,sreg05,LSR sreg06

        mov     sreg04,#0



ddiv_denormRound

        rsbs    dyl,dyl,#0              ; compute y = -mY

        mul     sreg04,dyl,sreg04       ; compute remainder: x - y * q

        mov     sreg05,#0

        umlal   sreg05,sreg04,dyl,dxl

;;;        rsc     dyh,dyh,#0
	rsb	dyh,dyh,#0
	subcc	dyh,dyh,#1

        mla     sreg04,dxl,dyh,sreg04   ; r4:r5 = rem = y * q   



        cmp     sreg04,sreg12           ; if (rem == x) return

        cmpeq   sreg05,sreg08

        DDIV_RETURN_EQ



        adds    sreg05,sreg05,dyl       ; rem += y

        adc     sreg04,sreg04,dyh

        cmp     sreg04,sreg12           ; if (rem == x) return q+1

        bmi     ddiv_incrementDenorm    ; if (rem < x) q+=1;

        bne     ddiv_denormRestore

        cmp     sreg05,sreg08

        beq     ddiv_incrementReturn

        blo     ddiv_incrementDenorm

ddiv_denormRestore

        subs    sreg05,sreg05,dyl       ; else rem -= y

        sbc     sreg04,sreg04,dyh



ddiv_setUnderflow

        adds    sreg05,sreg05,sreg05    ; rem += y/2

        adc     sreg04,sreg04,sreg04    ; (actually rem = rem * 2 + y)

        adds    sreg05,sreg05,dyl

        adc     sreg04,sreg04,dyh



      #ifdef IEEE_FULL_OR_FIXED

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



        adds    sreg08,sreg08,sreg08                ; round to nearest even

        adc     sreg12,sreg12,sreg12 

        cmp     sreg04,sreg12           ; if (rem == x) return q+1

        bmi     ddiv_incrementReturn

        DDIV_RETURN_NE

        cmp     sreg05,sreg08

        blo     ddiv_incrementReturn

        DDIV_RETURN_NE

        tst     dxl,#1                  ; if midpoint - round to even

        DDIV_RETURN_EQ



ddiv_incrementReturn

        adds    dxl,dxl,#1

        adc     dxh,dxh,#0

        DDIV_RETURN



ddiv_incrementDenorm

        adds    dxl,dxl,#1

        adc     dxh,dxh,#0

        b       ddiv_setUnderflow



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

        rsbs    dyl,dyl,#0              ; round to nearest - return 1 if initial numbers are different

;;;        rsc     dyh,dyh,#0
	rsb	dyh,dyh,#0
	subcc	dyh,dyh,#1

        cmp     dxh,dyh

        cmpeq   dxl,dyl



        mov     dxh,sreg12,LSL #31      ; return the lowest denorm or 0

        mov     dxl,#0

        movne   dxl,#1



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

      #endif

        DDIV_RETURN

      #ifdef IEEE_FULL

ddiv_vTinyUpRound

        mov     dxl,dxh,LSR #31         ; return 1 if sign bit == 0

        eor     dxl,dxl,#1

        DDIV_RETURN

ddiv_vTinyDownRound

        tst     sreg03, ZERO_OR_DOWN_BIT_MASK 

        mov     dxl,#0

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

    #ifdef IEEE_STD

        cmp     sreg12,#0

        beq     ddiv_xZero

    #else

        orrs    sreg05,dxl,dxh,LSL #1   ; check for zero

        beq.w     ddiv_xZero

        cmp     sreg12,sreg08               ; check for Inf/NaN

        beq     ddiv_xMaxExp



; Denormalized X

        movs    dxh,dxh,LSL #12

        beq     ddiv_xSmallDenorm 



        Normalize_a5 dxh,sreg12,sreg05  ; sreg12 - exponent

        add     sreg05,sreg12,#31

        mov     dxh,dxh,LSR #11

;;;        orr     dxh,dxh,dxl,LSR sreg05
	mov	sreg07,dxl,LSR sreg05
	orr	dxh,dxh,sreg07

        rsb     sreg05,sreg05,#32

        mov     dxl,dxl,LSL sreg05



        mov     sreg12,sreg12,LSL #1    ; LSB[r12] = sign

        orr     sreg12,sreg12,sreg04,LSR #31 

        b       ddiv_xDenormReturn



ddiv_xSmallDenorm

        mvn     sreg12,#19              ;  normalize the denorm and return to main

        Normalize_a5 dxl,sreg12,sreg05

        mov     dxh,dxl,LSR #11

        mov     dxl,dxl,LSL #21



        mov     sreg12,sreg12,LSL #1    ; LSB[r12] = sign

        orr     sreg12,sreg12,sreg04,LSR #31 

        b       ddiv_xDenormReturn

    #endif        

ddiv_xMaxExp

        orrs    sreg05,dxl,dxh,LSL #12  ; check for NaN

        bne.w     ddiv_xNaN



        bic     sreg05,dyh,#0x80000000

        cmp     sreg05,sreg08,LSL #19       ; check if y is Inf / NaN

        bhs     ddiv_xInf_yMax



        and     sreg05,dyh,#0x80000000

        eor     dxh,sreg05,dxh          ; Inf / x -> Inf (signed)

        DDIV_RETURN



ddiv_xInf_yMax

        orrs    sreg05,dyl,dyh,LSL #12  ; check if y is NaN

        bne.w     ddiv_yNaN

        b       ddiv_invalid            ; Inf / Inf -> invalid



ddiv_ySpecial

    #ifdef IEEE_STD

        cmp     sreg04,#0

        beq     ddiv_byZero

    #else

        orrs    sreg05,dyl,dyh,LSL #1   ; check for x / 0

        beq     ddiv_byZero 

        cmp     sreg04,sreg08

        beq     ddiv_yMaxExp



; Denormalized Y

        movs    dyh,dyh,LSL #12

        beq     ddiv_ySmallDenorm 



        Normalize_a5 dyh,sreg04,sreg05  ; r4 - exponent

        add     sreg05,sreg04,#31

        mov     dyh,dyh,LSR #11

;;;        orr     dyh,dyh,dyl,LSR sreg05
	mov	sreg07,dyl,LSR sreg05
	orr	dyh,dyh,sreg07

        rsb     sreg05,sreg05,#32

        mov     dyl,dyl,LSL sreg05 

        mov     sreg04,sreg04,LSL #1    ; 

        b       ddiv_yDenormReturn



ddiv_ySmallDenorm

        mvn     sreg04,#19              ;  normalize the denorm and return to main

        Normalize_a5 dyl,sreg04,sreg05

        mov     dyh,dyl,LSR #11

        mov     dyl,dyl,LSL #21

        mov     sreg04,sreg04,LSL #1

        b       ddiv_yDenormReturn 

    #endif

ddiv_yMaxExp

        orrs    sreg05,dyl,dyh,LSL #12  ; check for NaN

        bne     ddiv_yNaN

        mov     dxh,sreg12,LSL #31      ; x / Inf = 0 (signed)

        mov     dxl,#0

        DDIV_RETURN



ddiv_byZero

    #ifdef IEEE_FULL_OR_FIXED

        LFPState sreg03, sreg02

        SetDivByZero sreg03

        StFPState sreg03,sreg02 

    #endif

        mov     dxh,sreg12,LSL #31      ; x / 0 -> signed Inf

        orr     dxh,dxh,sreg08,LSL #19

        mov     dxl,#0

        DDIV_RETURN



ddiv_xZero

    #ifdef IEEE_STD

        tst     sreg08,dyh,LSR #19

    #else 

        orrs    sreg05,dyl,dyh,LSL #1   ; check 0 / 0 -> invalid

    #endif        

        beq     ddiv_invalid

        bic     sreg05,dyh,#0x80000000

        cmp     sreg05,sreg08,LSL #19       ; check 0 / NaN

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

        cmp     sreg05,sreg08,LSL #19       ; check if y is NaN

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

        LFPState sreg03, sreg02

        SetInvalidE sreg03

        StFPState sreg03,sreg02 

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

        Clrstk(al)

        and     dxh,dxh,#0x80000000

        orr     dxh,dxh,#0x10000000

        orr     dxh,dxh,#0x0ff00000

        mov     dxl,#0

        ldr     r2, =(FE_EX_OVERFLOW :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_DIV)

        bl      __fp_handler_64_to_64

        Ret_restore



; overflow exception handler with pack

__wrap_overflow_handler_pack

        Clrstk(al)

        sub     sreg12, sreg12, #512

        sub     sreg12, sreg12, #2      ; coerce exponent       

        orr     dxh, sreg04, sreg12, LSL #31  ; pack sign

        bic     sreg12, sreg12,#1       ; clear sign (sreg12 = exp << 1)

        add     dxh,dxh,sreg12,LSL #19  ; add exp

        mov     dxl, sreg05

        ldr     r2, =(FE_EX_OVERFLOW :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_DIV)

        bl      __fp_handler_64_to_64

        Ret_restore



; underflow exception handler with pack

__wrap_underflow_handler

        Clrstk(al)

        add     sreg12, sreg12, #1536

        orr     dxh, sreg04, sreg12, LSL #31  ; pack sign

        bic     sreg12, sreg12,#1       ; clear sign (sreg12 = exp << 1)

        add     dxh,dxh,sreg12,LSL #19  ; add exp

        mov     dxl, sreg05

        ldr     r2, =(FE_EX_UNDERFLOW :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_DIV)

        bl      __fp_handler_64_to_64

        Ret_restore



; division by zero exception handler

__wrap_divbyzero_handler

        ldmfd   sp!, {dxh, dxl, dyh, dyl}

        ldr     r12, =(FE_EX_DIVBYZERO :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_DIV)

        stmfd   sp!, {sreg12}

        bl      __fp_handler_binary_64_to_64

        add     sp, sp, #4

        Ret_restore



; invalid exception handler

__wrap_invalid_handler

        ldmfd   sp!, {dxh, dxl, dyh, dyl}

        ldr     r12, =(FE_EX_INVALID :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_DIV)

        stmfd   sp!, {sreg12}

        bl      __fp_handler_binary_64_to_64

        add     sp, sp, #4 

        Ret_restore



    #endif        



    #ifdef IEEE_FULL

; inexact exception wrapper

__wrap_inexact_handler

        Clrstk(al)

        ldr     r2, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_DIV)

        bl      __fp_handler_64_to_64

        Ret_restore

    #endif



;-------------------------------------------------------------------------------

  #endif

#endif
	END

/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 * jrusso Oct 4, 2009. ADS compatibilty
 */
 
