;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _f_ftoi.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



;===============================================================================

	EXPORT _f_ftoi

	EXPORT _ffix                   ; FPLIB for _f_ftoi



	#ifdef VFP_AND_NOT_IEEE_STD

	EXPORT _vfp_support_ffix

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

; FUNCTION: _f_ftoi

; DESCRIPTION: Convert float to integer.

; INPUT:

;       fx = float

; OUTPUT:

;       fx = signed word

	AREA |.text|,CODE,READONLY

#ifdef VFP

_ffix

_f_ftoi

        fmsr    s0, r0

        ftosizs s0, s0

        fmrs    r0, s0

        Return



_vfp_support_ffix

#else

_ffix									; FPLIB entry point

_f_ftoi

#endif



	#ifdef NOT_VFP_OR_NOT_IEEE_STD



        bic     sreg01,fx,#0x80000000   ; sreg01 = absolute value

        mov     sreg02,#0x9e            ; sreg02 = # of shifts ( 31 + bias - exp)

        subs    sreg02,sreg02,sreg01,LSR #23

        ble     ftoi_overflow           ; negative shifts? overflow



  #ifdef IEEE_FULL

        cmp     sreg02,#32              ; >=32 ? zero

        bge     ftoi_zero

  #endif

        mov     sreg01,sreg01,LSL #8    ; sreg01 = left-aligned mantissa

        orr     sreg01,sreg01,#0x80000000  ; add implicit "1" bit

        cmp     fx,#0                   ; check if input is negative

        mov     fx,sreg01,LSR sreg02    ; fx = result

        rsbmi   fx,fx,#0                ; negate result if input was negative

  #ifdef IEEE_FULL        

        rsb     sreg02,sreg02,#32 

        movs    sreg01,sreg01,LSL sreg02  ; any round bits?

        Return  eq



        LFPState sreg03,sreg02          ; if any, signal inexact

        SetInexact sreg03

        StFPState sreg03,sreg02 

  #endif  

        Return



  #ifdef IEEE_FULL        

ftoi_zero

        movs    fx,sreg01               ; is it a true +/- zero?

        Return  eq

        mov     fx,#0

        LFPState sreg03,sreg01

        SetInexact sreg03

        StFPState sreg03,sreg01 

        Return

  #endif



ftoi_overflow

	#ifdef IEEE_FULL_OR_FIXED

        cmp     fx,#0xcf000000          ; don't signal invalid for a true MININT

        beq     ftoi_minint

        LFPState sreg03,sreg02

        SetInvalidE sreg03

        StFPState sreg03,sreg02 

ftoi_minint

  #endif

        mvn     fx,fx,ASR #31           ; fx = -1 if positive, 0 if negative

        add     fx,fx,#0x80000000       ; MININT if -, MAXINT if +

        Return



	#ifdef IEEE_FULL_OR_FIXED

	IMPORT __fp_handler_32_to_32



; invalid exception handler

__wrap_invalid_handler

        ldr     r1, =(FE_EX_INVALID :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_INT :OR: FE_EX_FN_CVT :OR: FE_EX_ROUND_ZERO)

        stmfd   sp!,{lr}

        bl      __fp_handler_32_to_32

        ldmfd   sp!,{lr}

        Return

  #endif



  #ifdef IEEE_FULL

; inexact exception wrapper

__wrap_inexact_handler

        ldr     r1, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_INT :OR: FE_EX_FN_CVT :OR: FE_EX_ROUND_ZERO)

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
