;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright � 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _f_ftoull.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



;===============================================================================

	EXPORT _f_ftoull

	EXPORT _ll_ufrom_f             ; FPLIB for _f_ftoull



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

; FUNCTION: _f_ftoull

; DESCRIPTION: Convert float to unsigned long long integer.

; INPUT:

;       fx = float

; OUTPUT:

;       llh/lll = signed long long word

;
	AREA |.text|,CODE,READONLY
	
_ll_ufrom_f							; FPLIB entry point

_f_ftoull

        tst     fx,#0x80000000          ; NaN or <= 0

        bne     ftoull_negative

        mov     sreg02,#0xbe            ; sreg02 = # of shifts ( 31 + bias - exp)

        subs    sreg02,sreg02,fx,LSR #23

        blt     ftoull_overflow         ; negative? overflow



#ifdef IEEE_FULL        

        cmp     sreg02,#64              ; >=32 ? zero

        bge     ftoull_zero

#endif  



        mov     sreg03,fx,LSL #8        ; sreg03 = left aligned mantissa

        orr     sreg03,sreg03,#0x80000000  ; add implicit 1



        cmp     sreg02,#32              ; >32 shifts? same code as ftou

        bgt     ftoull_32bit

        mov     llh,sreg03,LSR sreg02   ; llh = sreg03 >> #shifts

        rsb     sreg02,sreg02,#32

        mov     lll,sreg03,LSL sreg02   ; lll = sreg02 << (32 - #shifts)

        Return



ftoull_32bit

        sub     sreg02,sreg02,#32

        mov     llh,#0

        mov     lll,sreg03,LSR sreg02   ; lll = sreg03 >> #shifts

#ifdef IEEE_FULL        

        rsb     sreg02,sreg02,#32

        movs    sreg03,sreg03,LSL sreg02  ; round bits = sreg02 << (32 - #shifts)

        Return eq 

        LFPState sreg03,sreg02          ; if any, signal inexact

        SetInexact sreg03

        StFPState sreg03,sreg02 

#endif  

        Return



#ifdef IEEE_FULL        

ftoull_zero

        cmp     fx,#0                   ; is it a true zero?

        mov     llh,#0                  ; return 0 

        mov     lll,#0

        Return eq

        LFPState sreg03,sreg02

        SetInexact sreg03

        StFPState sreg03,sreg02 

        Return

#endif



ftoull_negative

        mov     sreg02,#0xff000000      ; check for NaN

        cmp     sreg02,fx,LSL #1

        blo     ftoull_overflow

        mov     llh,#0                  ; return 0 if negative and not NaN

        mov     lll,#0

        Return



ftoull_overflow

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg03,sreg02          ; set invalid, not overflow

        SetInvalidE sreg03

        StFPState sreg03,sreg02 

#endif

        mvn     llh,#0                  ; return MAXUINT if overflow

        mvn     lll,#0

        Return



	#ifdef IEEE_FULL_OR_FIXED

	IMPORT __fp_handler_32_to_64



; invalid exception handler

__wrap_invalid_handler

        ldr     r1, =(FE_EX_INVALID :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_ULONGLONG :OR: FE_EX_FN_CVT :OR: FE_EX_ROUND_ZERO)

        stmfd   sp!,{lr}

        bl      __fp_handler_32_to_64

        ldmfd   sp!,{lr}

        Return

#endif



#ifdef IEEE_FULL

	IMPORT __fp_handler_64_to_64



; inexact exception wrapper

__wrap_inexact_handler

        ldr     r2, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_ULONGLONG :OR: FE_EX_FN_CVT :OR: FE_EX_ROUND_ZERO)

        stmfd   sp!,{lr}

        bl      __fp_handler_64_to_64

        ldmfd   sp!,{lr}

        Return

#endif
	END

/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */
