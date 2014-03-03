;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:23 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _d_dtoull.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



;===============================================================================

	EXPORT _d_dtoull

	EXPORT _ll_ufrom_d             ; FPLIB for _d_dtoull



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

; FUNCTION: _d_dtoull

; DESCRIPTION: Convert double to unsigned long long integer.

; INPUT:

;       dxh/dxl = double

; OUTPUT:

;       llh/lll = unsigned long long word


	AREA |.text|,CODE,READONLY
	
_ll_ufrom_d							; FPLIB entry point

_d_dtoull


        tst     dxh,#0x80000000         ; NaN or <= 0

        bne     dtoull_negative



        ldr     sreg02,=0x43e           ; sreg02 = # of shifts ( 63 + bias - exp)

        subs    sreg02,sreg02,dxh,LSR #20

        blt     dtoull_overflow         ; negative # of shifts? overflow



        cmp     sreg02,#64              ; >=32 ? zero

        bge     dtoull_zero



        mov     sreg12,dxh,LSL #11      ; sreg12 = left aligned mantissa - high

        orr     sreg12,sreg12,#0x80000000

        orr     sreg12,sreg12,dxl,LSR #21



        cmp     sreg02,#32              ; 32 / 64 bit result

        ble     dtoull_64bit



        sub     sreg02,sreg02,#32

#ifdef IEEE_FULL

        mov     sreg03,dxl,LSL #11      ; sreg03 = left aligned mantissa - low

#endif  

        mov     llh,#0

        mov     lll,sreg12,LSR sreg02   ; result = mantissa >> #shifts (32 bit)

#ifdef IEEE_FULL

        rsb     sreg02,sreg02,#32

;;;        orrs    sreg12,sreg03,sreg12,LSL sreg02  ; sreg12 = round bits

	mov	sreg12,sreg12,LSL sreg02
	orrs	sreg12,sreg03,sreg12

        Return  eq

        LFPState sreg03,sreg02

        SetInexact sreg03

        StFPState sreg03,sreg02 

#endif

        Return



dtoull_64bit

        mov     sreg03,dxl,LSL #11      ; sreg03 = left aligned mantissa - low

        mov     llh,sreg12,LSR sreg02   ; result = mantissa >> #shifts

        mov     lll,sreg03,LSR sreg02

        rsb     sreg02,sreg02,#32

;;;        orr     lll,lll,sreg12,LSL sreg02
	mov	sreg12,sreg12,LSL sreg02
	orr	lll,lll,sreg12

#ifdef IEEE_FULL

        movs    sreg12,sreg03,LSL sreg02  ; sreg12 = round bits 

        Return  eq

        LFPState sreg03,sreg02

        SetInexact sreg03

        StFPState sreg03,sreg02 

#endif

        Return



dtoull_zero

#ifdef IEEE_FULL        

        orrs    sreg01,dxh,dxl          ; is it a true zero?

        Return  eq

#endif

        mov     llh,#0                  ; return 0 

        mov     lll,#0

#ifdef IEEE_FULL        

        LFPState sreg03,sreg02

        SetInexact sreg03

        StFPState sreg03,sreg02 

#endif

        Return





dtoull_negative

        cmn     dxh,#0x00100000         ; check for NaN

        cmpeq   dxl,#0

        bhi     dtoull_overflow

        mov     llh,#0                  ; return 0 if negative and not NaN

        mov     lll,#0

        Return



dtoull_overflow

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg03,sreg02          ; set invalid, not overflow

        SetInvalidE sreg03

        StFPState sreg03,sreg02 

#endif

        mvn     llh,#0                  ; return MAXUINT if overflow

        mvn     lll,#0

        Return



	#ifdef IEEE_FULL_OR_FIXED

	IMPORT __fp_handler_64_to_64



; invalid exception handler

__wrap_invalid_handler

        ldr     r2, =(FE_EX_INVALID :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_ULONGLONG :OR: FE_EX_FN_CVT :OR: FE_EX_ROUND_ZERO)

        stmfd   sp!,{lr}

        bl      __fp_handler_64_to_64

        ldmfd   sp!,{lr}

        Return

#endif



#ifdef IEEE_FULL



; inexact exception wrapper

__wrap_inexact_handler

        ldr     r2, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_ULONGLONG :OR: FE_EX_FN_CVT :OR: FE_EX_ROUND_ZERO)

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
