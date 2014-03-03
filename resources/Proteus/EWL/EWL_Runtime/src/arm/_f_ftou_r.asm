;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _f_ftou_r.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



;===============================================================================



	EXPORT _ffixu_r                ; Rounded _f_ftou



	#ifdef VFP_AND_NOT_IEEE_STD

	EXPORT _vfp_support_ffixu_r

#endif        


#ifdef THUMB

	MACRO
	Return $cc

        bx$cc  lr

	MEND

#else

	MACRO
	Return $cc

        mov$cc pc,lr

	MEND

#endif          



;===============================================================================

; A NOTE ABOUT CONVERSIONS TO INTEGRAL FORMATS

;=============================================

;

; Conversions from float and double to the 32- and 64-bit signed and unsigned

; integral formats follow a number of conventions.

; 1) A negative number converts to unsigned 0, without signaling an exception.

; 2) Otherwise, any fractional part is truncated, regardless of rounding mode.

;    _r versions round the integer part according to the current rounding mode

; 3) If a nonzero fraction is truncated, and the value does not overflow,

;       inexact is set.

; 4) If a value overflows the integer destination, invalid (not overflow) is set

;       and the result is set to the most negative value for signed destinations

;       and to the biggest value for unsigned destinations.



;===============================================================================

; FUNCTION: _ffixu_r

; DESCRIPTION: Convert float to unsigned integer, rounded

; INPUT:

;       fx = float

; OUTPUT:

;       fx = unsigned word

;

	AREA |.text|,CODE,READONLY

#ifdef VFP

_ffixu_r

        fmsr    s0, r0

        ftouis  s0, s0

        fmrs    r0, s0

        Return





_vfp_support_ffixu_r

#else

_ffixu_r                               ; FPLIB entry point

#endif



	#ifdef NOT_VFP_OR_NOT_IEEE_STD



        tst     fx,#0x80000000          ; NaN or <= 0

        bne     ftou_r_negative

        mov     sreg01,#0x9e            ; sreg01 = # of shifts ( 31 + bias - exp)

        subs    sreg01,sreg01,fx,LSR #23

        blt     ftou_r_overflow         ; negative? overflow



  #ifdef IEEE_FULL

        cmp     sreg01,#32              ; >32 ? zero

        bgt     ftou_r_zero

  #endif



        mov     sreg02,fx,LSL #8        ; sreg02 = left-aligned mantissa

        orr     sreg02,sreg02,#0x80000000  ; add implicit "1" bit

        mov     fx,sreg02,LSR sreg01    ; fx = result (not rounded)

        rsb     sreg01,sreg01,#32       ; sreg01 = shifts to keep round bits only

; if negative, shifting will clear sreg02

        movs    sreg02,sreg02,LSL sreg01  ; any round bits?

  #ifdef IEEE_FULL

        Return  eq



        LFPState sreg03,sreg01          ; if any, signal inexact

        SetInexact sreg03

        StFPState sreg03,sreg01 

        tst     sreg03,CHOP_OR_DOWN_BIT_MASK

        Return  ne                      ; round to zero / -inf chops round bits

        tst     sreg03,UP_OR_NEAR_BIT_MASK

        bne     ftou_r_up

  #endif



        tst     sreg02,#0x80000000      ; check rounding bit

        Return  eq                      ; 0 ? quit

        bics    sreg02,sreg02,#0x80000000  ; check sticky bits

        tsteq   fx,#1                   ; check LSB of the integer part

ftou_r_up

        addne   fx,fx,#1

        Return



; Common ftou functions



  #ifdef IEEE_FULL

ftou_r_zero

        cmp     fx,#0

        Return  eq

        mov     fx,#0

        LFPState sreg03,sreg01

        SetInexact sreg03

        StFPState sreg03,sreg01 



        and     sreg03,sreg03,ROUND_MODE_MASK  ; return 0 (1 in round up mode)

        cmp     sreg03,FE_UPWARD

        moveq   fx,#1

        Return

  #endif



ftou_r_negative

        mov     sreg02,#0xff000000      ; check for NaN

        cmp     sreg02,fx,LSL #1

        movhs   fx,#0

	#ifdef IEEE_FULL_OR_FIXED

        Return  hs

  #else

        mvnlo   fx,#0

        Return

  #endif



ftou_r_overflow

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg03,sreg02

        SetInvalidE sreg03

        StFPState sreg03,sreg02 

  #endif  

        mvn     fx,#0

        Return





	#ifdef IEEE_FULL_OR_FIXED



	IMPORT __fp_handler_32_to_32



; invalid exception handler

__wrap_invalid_handler

        ldr     r1, =(FE_EX_INVALID :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_UINT :OR: FE_EX_FN_CVT)

        stmfd   sp!,{lr}

        bl      __fp_handler_32_to_32

        ldmfd   sp!,{lr}

        Return

  #endif



  #ifdef IEEE_FULL

; inexact exception wrapper

__wrap_inexact_handler

        ldr     r1, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_UINT :OR: FE_EX_FN_CVT)

        stmfd   sp!,{lr}

        bl      __fp_handler_32_to_32

        ldmfd   sp!,{lr}

        Return

  #endif

#endif
	END

/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */
