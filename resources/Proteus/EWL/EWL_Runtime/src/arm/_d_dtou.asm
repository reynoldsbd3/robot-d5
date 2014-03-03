;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright � 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:23 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _d_dtou.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



;===============================================================================

	EXPORT _d_dtou

	EXPORT _dfixu                  ; FPLIB for _d_dtou



	#ifdef VFP_AND_NOT_IEEE_STD

	EXPORT _vfp_support_dfixu

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

; FUNCTION: _d_dtou

; DESCRIPTION: Convert double to unsigned integer.

; INPUT:

;       dxh/dxl = double

; OUTPUT:

;       fx = unsigned word

;
	AREA |.text|,CODE,READONLY

#ifdef VFP

_dfixu

_d_dtou

        fmdhr   d0, dxh

        fmdlr   d0, dxl

        ftouizd s0, d0

        fmrs    r0, s0

        Return



_vfp_support_dfixu

#else

_dfixu									; FPLIB entry point

_d_dtou

#endif



	#ifdef NOT_VFP_OR_NOT_IEEE_STD



        tst     dxh,#0x80000000         ; NaN or <= 0

        bne     dtou_negative

        ldr     sreg02,=0x41e           ; sreg02 = # of shifts ( 31 + bias - exp)

        subs    sreg02,sreg02,dxh,LSR #20

        blt     dtou_overflow           ; negative? overflow



        cmp     sreg02,#32              ; >=32 ? zero

        bge     dtou_zero



        mov     sreg03,dxh,LSL #11      ; sreg03 = left-aligned mantissa

        orr     sreg03,sreg03,#0x80000000

        orr     sreg03,sreg03,dxl,LSR #21 

  #ifdef IEEE_FULL

        mov     sreg12,dxl,LSL #11      ; sreg12 = any sticky bits left in dxl

  #endif

        mov     fx,sreg03,LSR sreg02    ; compute result

  #ifdef IEEE_FULL

        rsb     sreg02,sreg02,#32 

;;;        orrs    sreg12,sreg12,sreg03,LSL sreg02  ; any round bits?

	mov	sreg02,sreg03,LSL sreg02
	orrs	sreg12,sreg12,sreg02

        Return  eq



        LFPState sreg03,sreg01          ; if any, signal inexact

        SetInexact sreg03

        StFPState sreg03,sreg01 

  #endif

        Return



dtou_zero

  #ifdef IEEE_FULL

        orrs    fx,dxh,dxl              ; check for a true zero

        Return  eq

  #endif

        mov     fx,#0

  #ifdef IEEE_FULL

        LFPState sreg03,sreg01          ; signal inexact

        SetInexact sreg03

        StFPState sreg03,sreg01 

  #endif

        Return



dtou_negative

        cmn     dxh,#0x00100000         ; check for NaN

        cmpeq   dxl,#0

        movls   fx,#0                   ; return 0 if it is not a NaN

	#ifdef IEEE_FULL_OR_FIXED

        Return  ls

  #else

        mvnhi   fx,#0

        Return

  #endif



dtou_overflow

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg03,sreg02          ; signal invalid        

        SetInvalidE sreg03

        StFPState sreg03,sreg02 

  #endif

        mvn     fx,#0

        Return



	#ifdef IEEE_FULL_OR_FIXED



	IMPORT __fp_handler_64_to_32

; invalid exception handler

__wrap_invalid_handler

        ldr     r2, =(FE_EX_INVALID :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_UINT :OR: FE_EX_FN_CVT :OR: FE_EX_ROUND_ZERO)

        stmfd   sp!,{lr}

        bl      __fp_handler_64_to_32

        ldmfd   sp!,{lr}

        Return

  #endif



  #ifdef IEEE_FULL

	IMPORT __fp_handler_32_to_32



; inexact exception wrapper

__wrap_inexact_handler

        ldr     r1, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_UINT :OR: FE_EX_FN_CVT :OR: FE_EX_ROUND_ZERO)

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
