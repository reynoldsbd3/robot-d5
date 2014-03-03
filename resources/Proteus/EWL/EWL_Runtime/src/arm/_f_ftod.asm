;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _f_ftod.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



;===============================================================================



	EXPORT _f_ftoq

	EXPORT _f_ftod

	EXPORT _f2d                    ; FPLIB for _f_ftod



	#ifdef VFP_AND_NOT_IEEE_STD

	EXPORT _vfp_support_f2d

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

; FUNCTION: _f_ftod

; DESCRIPTION: Convert float to double

; FUNCTION: _f_ftoq

; DESCRIPTION: Convert float to double (== long double).

; INPUT:

;       fx = float

; OUTPUT:

;       dxh/dxl = double

;

	AREA |.text|,CODE,READONLY

#ifdef VFP

_f2d

_f_ftod

_f_ftoq


        fmsr    s0, r0

        fcvtds  d0, s0

        fmrdh   dxh, d0

        fmrdl   dxl, d0 

        Return



_vfp_support_f2d

#else

_f2d									; FPLIB entry point

_f_ftod

_f_ftoq


#endif



	#ifdef NOT_VFP_OR_NOT_IEEE_STD



        and     sreg02, fx, #0x80000000  ; keep sign in high bit of sreg02

        mov     sreg12, fx, LSR #23     ; GET X EXPONENT in sreg12

        mov     sreg03, fx, LSL #9      ; get X mantissa in sreg03

        ands    sreg12, sreg12, #0xff   ; strip sign and keep exp in sreg12

; @@@ Added next #ifdef for IEEE_STD (flush denormals to 0)

; @@@ and to ignore special cases (INFs/NANs)

  #ifdef IEEE_STD

        beq     ftod_zero_result

  #else     

        beq     ftod_x_tiny             ; X exponent is tiny 

  #endif          

        cmp     sreg12, #0xff           ; X exponent is huge 

        beq     ftod_x_huge 

ftod_pack

; normal case: just pack the number

; special case pack: drl = 0, drh = fx

        add     sreg12, sreg12, #(1023 - 127)  ; adjust exponent

        mov     dxl, sreg03, LSL #20    ; high part of dxl

        orr     dxh, sreg02, sreg03, LSR #12  ; stuff sign and rotate

        orr     dxh, dxh, sreg12, LSL #20  ; stuff exponent

        Return



; x is tiny: check for zero, normalize if denorm

ftod_x_tiny

        cmp     sreg03, #0

        bne     ftod_denorm

ftod_zero_result                       ; result is zero

        mov     dxh, sreg02             ; high part of +0 or -0

        mov     dxl, #0

        Return

ftod_denorm

        mov     sreg03, sreg03, LSR #1  ; align value for normalize

        Normalize_num sreg03, sreg12, #1  ; normalize the number in sreg03

        add     sreg03, sreg03, sreg03  ; LSL #1

        b       ftod_pack



; x is huge: check for INF/NaN  

ftod_x_huge

        cmp     sreg03, #0

        bhi     ftod_x_nan

; x is a INF

        ldr     dxh, =0x7ff00000

        orr     dxh, dxh, sreg02        ; stuff sign 

        mov     dxl, #0 

        Return

ftod_x_nan

	#ifdef IEEE_FULL_OR_FIXED

        tst     sreg03, #0x80000000     ; check signal

        bne     __f_make_NaN            ; exit if quiet

; @@@ Modified: added LFPState    

        LFPState sreg03, sreg02

        SetInvalidE sreg03              ; set invalid

        StFPState sreg03,sreg02         ; store FPSCR  

  #endif

__f_make_NaN

        MakeDNaN

        Return



	#ifdef IEEE_FULL_OR_FIXED

	IMPORT __fp_handler_32_to_64



;invalid exception handler

__wrap_invalid_handler

        ldr     r1, =(FE_EX_INVALID :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_CVT)

        stmfd   sp!, {lr}

        bl      __fp_handler_32_to_64

        Return_lr



  #endif  


#endif
	END

/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */
