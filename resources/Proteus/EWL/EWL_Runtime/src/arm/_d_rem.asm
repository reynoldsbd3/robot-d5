;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _f_rem.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



;===============================================================================



	EXPORT _drem



;===============================================================================

; FUNCTION: _drem

; DESCRIPTION:  Function that performs the IEEE754 remainder operation on double

;               precision. (C name: remainder()). This is defined to take two

;               numbers, x and y, and return a number z such that z = x - n * y

;               where n is an integer. To return an exactly correct result, n is

;               chosen so that z is no bigger that half of x (so that z might be

;               negative even if both x and y are positive). The IEEE 754 remainder

;               function is not the same as the operation performed by the fmod()

;               where z has always the same sign as x. Where the above specification

;               gives two acceptable choices of n, the even one is chosen. This

;               behavior occurs independently of the rounding mode.

; INPUT: dxh/dxl = x, dyh/dyl = y

; OUTPUT: dxh/dxl = result

;



	#ifdef IEEE_FULL_OR_FIXED

#define ClearStack add sp,sp,#8

#else

#define ClearStack 

#endif          



	#ifdef IEEE_FULL_OR_FIXED

	MACRO
	DREM_SAVE

        stmfd   sp!,{dxh,dxl,r4,r5,r6,r7,sreg08}

	MEND

#else

	MACRO
	DREM_SAVE

        stmfd   sp!,{r4,r5,r6,r7,sreg08}

	MEND

#endif



#if defined(THUMB)
; && !defined(ARM5)

	MACRO
	DREM_RETURN

        ClearStack

        ldmfd   sp!,{r4,r5,r6,r7,sreg08}

        bx      lr

	MEND

#else

	MACRO
	DREM_RETURN

        ClearStack

        ldmfd   sp!,{r4,r5,r6,r7,pc}

	MEND

#endif  

	AREA |.text|,CODE,READONLY

_drem

        DREM_SAVE

;;;        ldr     sreg04,=0x7ff
	mov sreg04,#0x7ff

        and     sreg12,dxh,#0x80000000  ; r12 = sign of x (sign)

        ands    sreg08,sreg04,dxh,LSR #20   ; sreg08 = exponent of x (ex)

        beq.w     drem_xzero 

        cmp     sreg08,sreg04

        beq.w     drem_xhuge

        orr     dxh,dxh,#0x00100000     ; add implicit 1

        bic     dxh,dxh,sreg04,LSL #21  ; clear sign and exponent

drem_xzero_return



        mov     sreg05,sreg04

        ands    sreg04,sreg04,dyh,LSR #20  ; r4 = exponent of y (ey)

#ifdef IEEE_STD

        beq.w     drem_invalid

#else

        beq.w     drem_yzero

#endif  

        cmp     sreg04,sreg05

        beq.w     drem_yhuge 

        orr     dyh,dyh,#0x00100000     ; add implicit 1

        bic     dyh,dyh,sreg05,LSL #21  ; clear sign and exponent

drem_yzero_return



        subs    sreg08,sreg08,sreg04            ; sreg08 = ex - ey (ediff)

        blt.w     drem_xless

        beq     drem_loopend 

drem_loop                              ; while (ediff > 0)

        cmp     dxh,dyh

        cmpeq   dxl,dyl                 ;   if (fx >= fy)

        blo     drem_loop_next

        subs    dxl,dxl,dyl             ;     fx -= fy;

        sbc     dxh,dxh,dyh

drem_loop_next

        adds    dxl,dxl,dxl             ;   fx <<= 1;

        adc     dxh,dxh,dxh

        subs    sreg08,sreg08,#1                ;   ediff --;

        bne     drem_loop 

drem_loopend



; fx, fy have the same exponent now             



        mov     sreg08,sreg12               ; sreg08 = sign

        cmp     dxh,dyh                 ; if (fx >= fy) -> quotient is even

        cmpeq   dxl,dyl

        blo     drem_check_half

        subs    dxl,dxl,dyl             ;   fx -= fy;

        sbc     dxh,dxh,dyh

        eor     sreg08,sreg12,#0x80000000   ;   sreg08 = - sign

drem_check_half



        mov     sreg05,dxh,LSL #1

        orr     sreg05,sreg05,dxl,LSR #31

        cmp     dyh,sreg05              ; if (fx == fy / 2)

        cmpeq   dyl,dxl,LSL #1

        moveq   sreg12,sreg08               ;   sign = sreg08 (round to even quotient)

        bhs     drem_normalize          ; else if (fx > fy / 2)

        eor     sreg12,sreg12,#0x80000000  ; sign = -sign

        subs    dxl,dyl,dxl             ;   fx = fy - fx

        sbc     dxh,dyh,dxh



drem_normalize

        cmp     dxh,#0

        beq     drem_half_zero



        Normalize_num dxh,sreg05,#43

        mov     dxh,dxh,LSR #11

;;;        orr     dxh,dxh,dxl,LSR sreg05

	mov	sreg06,dxl,LSR sreg05
	orr	dxh,dxh,sreg06	

        rsb     sreg05,sreg05,#32

        mov     dxl,dxl,LSL sreg05

        sub     sreg04,sreg04,sreg05

        b       drem_pack



drem_half_zero

        cmp     dxl,#0                  ; division is exact? return signed 0.0

        beq.w     drem_xtruezero



        sub     sreg04,sreg04,#21 

        Normalize_a5 dxl,sreg04,sreg05

        mov     dxh,dxl,LSR #11

        mov     dxl,dxl,LSL #21

drem_pack

        cmp     sreg04,#0

#ifdef IEEE_STD

        ble     drem_xtruezero

#else   

        ble     drem_result_denorm

#endif  

        bic     dxh,dxh,#0x00100000     ; clear implicit 1 bit

        orr     dxh,dxh,sreg12          ; join sign and mantissa

        orr     dxh,dxh,sreg04,LSL #20  ; pack exponent

        DREM_RETURN



#ifndef IEEE_STD        

drem_result_denorm

; dxh/dxl: unpacked mantissa

; MSB of sreg12 - sign

; sreg04: exponent (negative or zero)   

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg02, sreg02

        tst     sreg02, FE_IEEE_MASK_UNDERFLOW

        bne.w     __wrap_underflow_handler

  #endif

        cmn     sreg04,#20              ; exp <= -20 ?

        ble     drem_result_low 



        add     sreg04,sreg04,#31       ; high word is non-zero

        mov     sreg05,dxh,LSL sreg04

        rsb     sreg04,sreg04,#32

;;;        orr     dxh,sreg12,dxh,LSR sreg04
	mov	sreg07,dxh,LSR sreg04
	orr	dxh,sreg12,sreg07
	

;;;        orr     dxl,sreg05,dxl,LSR sreg04
	mov	sreg07,dxl,LSR sreg04
	orr	dxl,sreg05,sreg07

        DREM_RETURN



drem_result_low

        mov     dxl,dxl,LSR #21

        orr     dxl,dxl,dxh,LSL #11

        mov     dxh,sreg12              ; high word is zero

        mvn     sreg05,#19

        sub     sreg04,sreg05,sreg04

        mov     dxl,dxl,LSR sreg04

        DREM_RETURN

#endif



; exp of x is less than exp of y



drem_xless

        cmn     sreg08,#1

        add     sreg04,sreg08,sreg04        ; result exponent: ex

        bne     drem_pack               ; if exp difference == -1 return



        cmp     dxh,dyh                 ; if (y/2 < x < y)

        cmpeq   dxl,dyl

        bls     drem_pack



        eor     sreg12,sreg12,#0x80000000

        rsbs    dxl,dxl,dyl,LSL #1      ;     rem(x,y) = x - y  

;;;        rsc     dxh,dxh,dyh,LSL #1

	rsb	 dxh,dxh,dyh,LSL #1
	subcc	dxh,dxh,#1

        add     dxh,dxh,dyl,LSR #31

        b       drem_normalize



; x is zero/denorm, y may be anything



drem_xzero

        orrs    sreg05,dyl,dyh,LSL #1

        beq     drem_invalid            ; rem(x,0) => invalid

        ands    sreg05,sreg04,dyh,LSR #20

        cmp     sreg05,sreg04

        beq.w     drem_yhuge

#ifndef IEEE_STD        

        movs    dxh,dxh,LSL #12

        beq     drem_xzero_high 



        Normalize_num dxh,sreg08,#0

        add     sreg05,sreg08,#31

        mov     dxh,dxh,LSR #11

;;;        orr     dxh,dxh,dxl,LSR sreg05
	mov	sreg07,dxl,LSR sreg05
	orr	dxh,dxh,sreg07

        rsb     sreg05,sreg05,#32

        mov     dxl,dxl,LSL sreg05

        b       drem_xzero_return



drem_xzero_high

        cmp     dxl,#0                  ;  check zero / denorm

        beq     drem_xtruezero



        mvn     sreg08,#19                  ;  normalize the denorm and return to main

        Normalize_a5 dxl,sreg08,sreg05

        mov     dxh,dxl,LSR #11

        mov     dxl,dxl,LSL #21

        b       drem_xzero_return

#endif  

drem_xtruezero

        mov     dxh,sreg12              ; rem(+/-0,x) = +/-0

        mov     dxl,#0

        DREM_RETURN



; x is inf/nan, y may be anything



drem_xhuge

	#ifdef IEEE_FULL_OR_FIXED

        orrs    sreg05,dxl,dxh,LSL #12

        beq     drem_xinf

        tst     dxh,#0x00080000

        beq     drem_invalid            ; rem(sNaN,any) => invalid

        tst     dyh,#0x00080000

        bne     drem_xqnan              ; rem(qNaN,non sNaN) => qNaN    

        mov     sreg12,dyh,LSL #1 

        cmn     sreg12,#0x00200000

        cmpeq   dyl,#0

        bls     drem_xqnan              ; rem(qNaN,non NaN) => qNaN

        b       drem_invalid            ; rem(qNaN,sNaN) => invalid



drem_xinf

        mov     sreg12,dyh,LSL #1

        cmn     sreg12,#0x00100000

        bhs     drem_xqnan              ; rem(Inf,qNaN) => qNaN

#endif

drem_invalid

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg04, sreg05

        SetInvalidE sreg04

        StFPState sreg04,sreg05 

#endif

drem_xqnan

        orr     dxh,dxh,#0x7f000000

        orr     dxh,dxh,#0x00f80000     ; return a quiet NaN

        DREM_RETURN



; x is already normalized, y is zero/denorm



#ifndef IEEE_STD        

drem_yzero

        movs    sreg04,dyh,LSL #12

        beq     drem_yzero_high



        mov     dyh,sreg04

        Normalize_num dyh,sreg04,#0

        add     sreg05,sreg04,#31

        mov     dyh,dyh,LSR #11

;;;        orr     dyh,dyh,dyl,LSR sreg05
	mov	sreg07,dyl,LSR sreg05
	orr	dyh,dyh,sreg07

        rsb     sreg05,sreg05,#32

        mov     dyl,dyl,LSL sreg05

        b       drem_yzero_return



drem_yzero_high

        cmp     dyl,#0                  ; check zero / denorm

        beq     drem_invalid            ; rem(non NaN,0) => invalid



        mvn     sreg04,#19              ;  normalize the denorm and return to main

        Normalize_a5 dyl,sreg04,sreg05

        mov     dyh,dyl,LSR #11

        mov     dyl,dyl,LSL #21

        b       drem_yzero_return

#endif  



; x is 0 or unpacked, y is inf/nan



drem_yhuge

        movs    sreg04,dyh,LSL #12

	#ifdef IEEE_FULL_OR_FIXED

        bmi     drem_xqnan              ; rem(any,qNaN) => invalid

#endif

        cmpeq   dyl,#0

        bne     drem_invalid            ; rem(any,sNaN) => invalid



#ifdef IEEE_STD 

        cmp     sreg08,#0                   ; flush-to-zero the denorm

        moveq   dxh,#0

        moveq   dxl,#0

#endif

        bic     dxh,dxh,#0x00100000     ; clear implicit "1" bit

        orr     dxh,dxh,sreg08,LSL #20

	#ifdef IEEE_FULL_OR_FIXED

        cmp     sreg08,#0                   ; raise an exception in case of denorm

        bne     drem_yhuge_return



        LFPState sreg02, sreg02

        tst     sreg02, FE_IEEE_MASK_UNDERFLOW

        beq     drem_yhuge_return



        movs    dxh,dxh,LSL #12

        beq     drem_xzero_high 

        cmp     dxh,#0

        beq     drem_0byInf_zero_high 

        Normalize_num dxh,sreg04,#0

        add     sreg05,sreg04,#31

        mov     dxh,dxh,LSR #11

;;;        orr     dxh,dxh,dxl,LSR sreg05
	mov	sreg07,dxl,LSR sreg05
	orr	dxh,dxh,sreg07

        rsb     sreg05,sreg05,#32

        mov     dxl,dxl,LSL sreg05

        orr     dxh,dxh,sreg12          ; put sign

        b       __wrap_underflow_handler



drem_0byInf_zero_high

        cmp     dxl,#0                  ;  check zero / denorm

        beq     drem_yhuge_return



        mvn     sreg04,#19              ; normalize the denorm and go to handler

        Normalize_a5 dxl,sreg04,sreg05

        mov     dxh,dxl,LSR #11

        mov     dxl,dxl,LSL #21

        orr     dxh,dxh,sreg12          ; put sign

        b       __wrap_underflow_handler



drem_yhuge_return

#endif

        orr     dxh,dxh,sreg12          ; put sign

        DREM_RETURN





	#ifdef IEEE_FULL_OR_FIXED

	IMPORT __fp_handler_64_to_64

	IMPORT __fp_handler_binary_64_to_64



; invalid exception handler

__wrap_invalid_handler

        ldmfd   sp, {dxh,dxl} 

        ldr     r12, =(FE_EX_INVALID :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_REM)

        stmfd   sp!, {sreg12}

        bl      __fp_handler_binary_64_to_64

        add     sp,sp,#4

        DREM_RETURN



; underflow exception handler with pack

; dxh/dxl: unpacked mantissa

; MSB of sreg12 - sign

; sreg04: exponent (negative or zero)   

__wrap_underflow_handler

        bic     dxh,dxh,#0x00100000     ; clear implicit 1 bit

        orr     dxh,dxh,sreg12          ; join sign and mantissa

        add     sreg04,sreg04,#0x600    ; underflow correction

        orr     dxh,dxh,sreg04,LSL #20  ; pack exponent

        ldr     r2, =(FE_EX_UNDERFLOW :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_REM)

        bl      __fp_handler_64_to_64

        DREM_RETURN

#endif
	END

/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */
