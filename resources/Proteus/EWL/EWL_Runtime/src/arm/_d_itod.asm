;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _d_itod.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



;===============================================================================



	EXPORT _d_itod

	EXPORT _dflt                   ; FPLIB for _d_itod

	EXPORT _d_utod

	EXPORT _dfltu                  ; FPLIB for _dfltu



	EXPORT __d_utod_common

	EXPORT __d_itod_common



	#ifdef VFP_AND_NOT_IEEE_STD

	EXPORT _vfp_support_dflt

	EXPORT _vfp_support_dfltu

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

; @@@ Modified: optimized functions

;===============================================================================



;===============================================================================

; STRATEGY: Converting integral types to floating types requires alignment and

; then coercion.  Use common alignment strategies for the 32-bit types and

; the 64-bit types, then before coercing check for the required rounding

; width and exponent bias.





;===============================================================================

; FUNCTION: _d_utod

; DESCRIPTION: Convert unsigned word to double.

; INPUT:

;       fx = unsigned word

; OUTPUT:

;       dxh/dxl = double

;

	AREA |.text|,CODE,READONLY

#ifdef VFP

_dfltu

_d_utod


        fmsr    s0, r0

        fuitod  d0, s0

        fmrdh   dxh, d0

        fmrdl   dxl, d0

        Return



_vfp_support_dfltu

#else

_dfltu									; FPLIB entry point

_d_utod


#endif



	#ifdef NOT_VFP_OR_NOT_IEEE_STD



        cmp     fx, #0                  ; return 0 if int is 0

__d_utod_common

        mov     sreg01, #0

        Return  eq

        mov     sreg03, #1024

        add     sreg03, sreg03, #30

        bmi     d_coerce_int32_utod     ; already normalized

        Normalize_a5 fx, sreg03, sreg12

d_coerce_int32_utod

  #if !defined(BIG_ENDIAN) && !defined(SOFTFPA)

; dxh and fx only differ in this case

        mov     dxh,fx

  #endif

        mov     dxl, dxh, LSL #21       ; low 11 bits of dxl become dxl

        add     dxh, dxh, dxh           ; shift off msb (LSL #1)

        mov     dxh, dxh, LSR #12       ; align 20 mantissa bits

        orr     dxh, dxh, sreg03, LSL #20  ; stuff exponent 

        Return

#endif



;===============================================================================

; FUNCTION: _d_itod

; DESCRIPTION: Convert signed word to double.

; INPUT:

;       fx = signed word

; OUTPUT:

;       dxh/dxl = double

; USES:

;   sreg02 = sign

; CASES: Zero maps to +0. All conversions are exact.

; OPTIMIZATION: No exceptions, so no access to fpscr.

;
	AREA |.text|,CODE,READONLY

#ifdef VFP

_dflt

_d_itod


        fmsr    s0, r0

        fsitod  d0, s0

        fmrdh   dxh, d0

        fmrdl   dxl, d0

        Return



_vfp_support_dflt

#else

_dflt									; FPLIB entry point

_d_itod

#endif



	#ifdef NOT_VFP_OR_NOT_IEEE_STD



        ands    sreg02, fx, #0x80000000  ; copy sign bit to sreg02 bit 31

        rsbmi   fx, fx, #0              ; negate if fx is negative

        cmp     fx, #0 

__d_itod_common

        mov     sreg01, #0

        Return  eq

        mov     sreg03, #1024

        add     sreg03, sreg03, #30

        Normalize_a5 fx, sreg03, sreg12

  #if !defined(BIG_ENDIAN) && !defined(SOFTFPA)

; dxh and fx only differ in this case

        movs    dxh,fx

  #endif

        mov     dxl, dxh, LSL #21       ; low 11 bits of dxl become dxl

        add     dxh, dxh, dxh           ; shift off msb (LSL #1)

;;        orr     dxh, sreg02, dxh, LSR #12  ; align 20 mantissa bits

	mov 	dxh, dxh, LSR #12
	orr	dxh, sreg02, dxh

;;;        orr     dxh, dxh, sreg03, LSL #20  ; stuff exponent 

	mov	sreg03, sreg03, LSL #20
	orr	dxh, dxh, sreg03

        Return

#endif
	END

/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */
