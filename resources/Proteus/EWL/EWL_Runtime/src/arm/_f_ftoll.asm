;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _f_ftoll.s

;===============================================================================



; @@@ All the code in this file was modified to use llh/lll instead of dxh/dxl

; @@@ for long long conversions in order to insure compatibility with the 

; @@@ SoftFPA mode



	#include "ARMsfpe.pre.asm"



;===============================================================================



	EXPORT _f_ftoll

	EXPORT _ll_sfrom_f             ; FPLIB for _f_ftoll



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

; FUNCTION: _f_ftoll

; DESCRIPTION: Convert float to long long integer.

; INPUT:

;       fx = float

; OUTPUT:

;       llh/lll = signed long long word

; OPERATION:

;       Negate the operand if necessary, then shift it to the right position

	AREA |.text|,CODE,READONLY
	
_ll_sfrom_f							; FPLIB entry point

_f_ftoll


        bic     sreg03,fx,#0x80000000   ; sreg03 = absolute value

        mov     sreg02,#0xbd            ; sreg02 = # of shifts ( 62 + bias - exp)

        subs    sreg02,sreg02,sreg03,LSR #23

        blt     ftoll_overflow          ; negative? overflow    



        cmp     sreg02,#63

        bge     ftoll_zero



        mov     sreg12,fx,ASR #31       ; sreg12 = 0 if positive, -1 if negative



        mov     fx,fx,LSL #8            ; left align mantissa

        orr     fx,fx,#0x80000000       ; add implicit 1 bit

        eor     sreg03,sreg12,fx,LSR #1  ; sreg03 = result (shifted, signed) 

        add     sreg03,sreg03,sreg12,LSR #31  ; r3 = ~r3 + 1 if sreg12 == -1



        cmp     sreg02,#32

        ble     ftoll_64bit             ; 32 / 64 bit result



        sub     sreg02,sreg02,#32

        mov     llh,sreg12              ; sign-extend result

        mov     lll,sreg03,ASR sreg02   ; lll = sreg03 >> #shifts

        rsb     sreg02,sreg02,#32

        movs    sreg03,sreg03,LSL sreg02  ; sreg03 = round bits

#ifdef IEEE_FULL        

        Return  eq

        add     lll,lll,sreg12,LSR #31  ; round-to-zero: increment if negative

        LFPState sreg03,sreg02          ; if any, signal inexact

        SetInexact sreg03

        StFPState sreg03,sreg02 

#else

        addne   lll,lll,sreg12,LSR #31  ; round-to-zero: increment if negative

#endif

        Return



ftoll_64bit

        mov     llh,sreg03,ASR sreg02   ; llh = sreg03 >> #shifts

        rsb     sreg02,sreg02,#32

        mov     lll,sreg03,LSL sreg02   ; lll = sreg03 << (32 - #shifts)

        Return



ftoll_zero

#ifdef IEEE_FULL        

        bics    fx,fx,#0x80000000       ; is it a true +/- zero?

        Return  eq

#endif

        mov     lll,#0

        mov     llh,#0

#ifdef IEEE_FULL        

        LFPState sreg03,sreg02

        SetInexact sreg03

        StFPState sreg03,sreg02 

#endif

        Return



ftoll_overflow

	#ifdef IEEE_FULL_OR_FIXED

        cmp     fx,#0xdf000000          ; don't signal invalid for a true MININT

        beq     ftoll_minint

        LFPState sreg03,sreg02

        SetInvalidE sreg03

        StFPState sreg03,sreg02 

ftoll_minint

#endif

        mvn     lll,fx,ASR #31          ; fx = -1 if positive, 0 if negative

        add     llh,lll,#0x80000000     ; MININT if -, MAXINT if +

        Return



	#ifdef IEEE_FULL_OR_FIXED

	IMPORT __fp_handler_32_to_64



; invalid exception handler

__wrap_invalid_handler

        ldr     r1, =(FE_EX_INVALID :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_LONGLONG :OR: FE_EX_FN_CVT :OR: FE_EX_ROUND_ZERO)

        stmfd   sp!,{lr}

        bl      __fp_handler_32_to_64

        ldmfd   sp!,{lr}

        Return

#endif



#ifdef IEEE_FULL

	IMPORT __fp_handler_64_to_64



; inexact exception wrapper

__wrap_inexact_handler

        ldr     r2, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_LONGLONG :OR: FE_EX_FN_CVT :OR: FE_EX_ROUND_ZERO)

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
