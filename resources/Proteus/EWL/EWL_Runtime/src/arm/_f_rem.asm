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



	EXPORT _frem



;===============================================================================

; FUNCTION: _frem

; DESCRIPTION:  Function that performs the IEEE754 remainder operation on single

;               precision float. (C name: remainderf()). This is defined to take

;               two numbers, x and y, and return a number z such that z = x - n * y

;               where n is an integer. To return an exactly correct result, n is

;               chosen so that z is no bigger that half of x (so that z might be

;               negative even if both x and y are positive). The IEEE 754 remainder

;               function is not the same as the operation performed by the fmod()

;               where z has always the same sign as x. Where the above specification

;               gives two acceptable choices of n, the even one is chosen. This

;               behavior occurs independently of the rounding mode.

; INPUT: fx=x, fy=y, floats

; OUTPUT: fr = result

;



	#ifdef IEEE_FULL_OR_FIXED

	MACRO
	FREM_SAVE

        stmfd   sp!,{lr}

        mov     lr,fx

	MEND



  #if defined(THUMB)
; && !defined(ARM5)

	MACRO
	FREM_RETURN $cc

        ldm$cc.fd sp!,{lr}

        bx$cc  lr

	MEND

  #else

	MACRO
	FREM_RETURN  $cc

        ldm$cc.fd sp!,{pc}

	MEND

  #endif  

#else

#define FREM_SAVE

  #if defined(THUMB)
; && !defined(ARM5)

	MACRO
	FREM_RETURN $cc

        bx$cc  lr

	MEND

  #else

	MACRO
	FREM_RETURN $cc

        mov$cc pc,lr

	MEND

  #endif  

#endif


	AREA |.text|,CODE,READONLY

_frem

        FREM_SAVE

        mov     sreg03,#0xff

        ands    sreg02,sreg03,fx,LSR #23  ; r2 = exponent of x (ex)

        beq     frem_xzero

        cmp     sreg02,#0xff

        beq.w     frem_xhuge

        and     sreg12,fx,#0x80000000   ; r12 = sign of x (sign)

        orr     fx,fx,#0x00800000       ; add implicit 1

        bic     fx,fx,#0xff000000       ; clear sign and exponent               

frem_xzero_return

        ands    sreg03,sreg03,fy,LSR #23  ; r3 = exponent of y (ey)

#ifdef IEEE_STD

        beq     frem_invalid

#else

        beq.w     frem_yzero

#endif  

        cmp     sreg03,#0xff

        beq.w     frem_yhuge 

        orr     fy,fy,#0x00800000       ; add implicit 1

        bic     fy,fy,#0xff000000       ; clear sign and exponent

frem_yzero_return

        subs    sreg02,sreg02,sreg03    ; sreg02 = ex - ey (ediff)

        blt     frem_xless

        beq     frem_loopend 

frem_loop                              ; while (ediff > 0)

        cmp     fx,fy                   ;   if (fx >= fy)

        subhs   fx,fx,fy                ;     fx -= fy;

        mov     fx,fx,LSL #1            ;   fx <<= 1;

        subs    sreg02,sreg02,#1        ;   ediff --;

        bne     frem_loop 

frem_loopend



; fx, fy have the same exponent now     



        cmp     fx,fy                   ; if (fx >= fy) -> quotient is odd

        subhs   fx,fx,fy                ;   fx -= fy;

        eorhs   sreg02,sreg12,#0x80000000  ; r2 = -sign

        movlo   sreg02,sreg12           ; else r2 = sign



        cmp     fy,fx,LSL #1            ; if (fx > fy / 2)

        sublo   fx,fy,fx                ;   fx = fy - fx

        eorlo   sreg12,sreg12,#0x80000000  ; sign = -sign

        moveq   sreg12,sreg02           ; else if (fx == fy / 2) sign = r2

;          (round to even quotient)

frem_normalize

        cmp     fx,#0                   ; division is exact? return signed 0.0

        beq     frem_xtruezero

        add     sreg03,sreg03,#8

        Normalize_a5 fx,sreg03,sreg02

        cmp     sreg03,#0

#ifdef IEEE_STD

        ble     frem_xtruezero

#else   

        ble     frem_result_denorm

#endif  

        bic     fx,fx,#0x80000000       ; clear implicit 1 bit

        orr     fx,sreg12,fx,LSR #8     ; join sign and mantissa

        orr     fx,fx,sreg03,LSL #23    ; pack exponent

        FREM_RETURN



#ifndef IEEE_STD        

frem_result_denorm

; fx - result mantissa, left aligned

; MSB of sreg12 - result sign

; sreg03 - exponent

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg02, sreg02

        tst     sreg02, FE_IEEE_MASK_UNDERFLOW

        bne.w     __wrap_underflow_handler

  #endif        

        rsb     sreg03,sreg03,#9        ; fx >>= (8 + 1 - exp)

;;;        orr     fx,sreg12,fx,LSR sreg03
	mov	sreg03,fx,LSR sreg03
	orr	fx,sreg12,sreg03

        FREM_RETURN

#endif



; exp of x is less than exp of y



frem_xless

        cmn     sreg02,#1

        add     sreg03,sreg02,sreg03    ; result exponent: ex

        bne     frem_normalize          ; if exp difference == -1 return

        cmp     fx,fy 

        eorhi   sreg12,sreg12,#0x80000000  ; if (y/2 < x < y)

        rsbhi   fx,fx,fy,LSL #1         ;     rem(x,y) = x - y  

        b       frem_normalize



; x is zero/denorm, y may be anything



frem_xzero

        bics    sreg12,fy,#0x80000000

        beq     frem_invalid            ; rem(x,0) => invalid

        cmp     sreg03,sreg12,LSR #23

        beq     frem_yhuge

        and     sreg12,fx,#0x80000000   ; r12 = sign of x (sign)

#ifndef IEEE_STD        

        movs    fx,fx,LSL #9            ; check zero / denorm

        beq     frem_xtruezero

        Normalize_num fx,sreg02,#0

        mov     fx,fx,LSR #8

        b       frem_xzero_return

#endif  

frem_xtruezero

        mov     fx,sreg12               ; rem(+/-0,x) = +/-0

        FREM_RETURN



; x is inf/nan, y may be anything



frem_xhuge

	#ifdef IEEE_FULL_OR_FIXED

        movs    fx,fx,LSL #9

        beq     frem_xinf

        bpl     frem_invalid            ; rem(sNaN,any) => invalid

        tst     fy,#0x00400000

        bne     frem_xqnan              ; rem(qNaN,non sNaN) => qNaN

        mov     sreg12,fy,LSL #1 

        cmn     sreg12,#0x01000000

        bls     frem_xqnan              ; rem(qNaN,non NaN) => qNaN

        b       frem_invalid            ; rem(qNaN,sNaN) => invalid



frem_xinf

        mov     sreg12,fy,LSL #1 

        cmn     sreg12,#0x00800000

        bhs     frem_xqnan              ; rem(Inf,qNaN) => qNaN

; rem(Inf,non NaN) => invalid

#endif

frem_invalid

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg03, sreg02

        SetInvalidE sreg03

        StFPState sreg03,sreg02 

#endif

frem_xqnan

        orr     fx,fx,#0x7f000000

        orr     fx,fx,#0x00c00000       ; return a quiet NaN

        FREM_RETURN



; x is already normalized, y is zero/denorm



#ifndef IEEE_STD        

frem_yzero

        movs    sreg03,fy,LSL #9        ; check zero / denorm

        beq     frem_invalid            ; rem(non NaN,0) => invalid

        mov     fy,sreg03

        Normalize_num fy,sreg03,#0

        mov     fy,fy,LSR #8

        b       frem_yzero_return

#endif  



; x is 0 or unpacked, y is inf/nan



frem_yhuge

        movs    sreg03,fy,LSL #9



	#ifdef IEEE_FULL_OR_FIXED

        bmi     frem_xqnan              ; rem(any,qNaN) => invalid

#endif

        bne     frem_invalid            ; rem(any,sNaN) => invalid

        cmp     sreg02,#0

#ifdef IEEE_STD 

        andeq   fx,fx,#0x80000000       ; flush-to-zero the denorm

#endif

	#ifdef IEEE_FULL_OR_FIXED

        beq     frem_xdenorm_yinf

#else

        FREM_RETURN eq                  ; rem(0,Inf) => 0

#endif  

; rem(a,Inf) => a

        bic     fx,fx,#0x00800000       ; clear implicit "1" bit

        orr     fx,fx,sreg02,LSL #23

        orr     fx,fx,sreg12

        FREM_RETURN



	#ifdef IEEE_FULL_OR_FIXED

frem_xdenorm_yinf

        bics    sreg02,fx,#0x80000000   ; is x zero?

        FREM_RETURN eq                  ; rem(0,Inf) => 0



        LFPState sreg03, sreg03

        tst     sreg03, FE_IEEE_MASK_UNDERFLOW

        FREM_RETURN eq                  ; rem(Denorm,Inf) => Denorm



        Normalize_num sreg02,sreg03,#9

        and     sreg12,fx,#0x80000000

        mov     fx,sreg02

        b       __wrap_underflow_handler

#endif        



	#ifdef IEEE_FULL_OR_FIXED

	IMPORT __fp_handler_32_to_32

	IMPORT __fp_handler_binary_32_to_32



; invalid exception handler

__wrap_invalid_handler

        mov     fx,lr

        ldr     r2, =(FE_EX_INVALID :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_REM)

        bl      __fp_handler_binary_32_to_32 

        FREM_RETURN



; underflow exception handler with pack

; fx - result mantissa, left aligned

; MSB of sreg12 - result sign

; sreg03 - exponent

__wrap_underflow_handler

        bic     fx,fx,#0x80000000       ; clear implicit 1 bit

        orr     fx,sreg12,fx,LSR #8     ; join sign and mantissa

        add     sreg03,sreg03,#0xC0     ; adjust exponent for underflow exception

        orr     fx,fx,sreg03,LSL #23    ; pack exponent

        ldr     r1, =(FE_EX_UNDERFLOW :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_REM)

        bl      __fp_handler_32_to_32 

        FREM_RETURN

#endif
	END

/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */
