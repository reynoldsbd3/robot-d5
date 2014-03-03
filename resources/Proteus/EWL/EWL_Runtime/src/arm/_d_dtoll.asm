;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:23 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _d_dtoll.s

;===============================================================================



; @@@ All the code in this file was modified to use llh/lll instead of dxh/dxl

; @@@ for long long conversions in order to insure compatibility with the 

; @@@ SoftFPA mode



	#include "ARMsfpe.pre.asm"



;===============================================================================



	EXPORT _d_dtoll

	EXPORT _ll_sfrom_d             ; FPLIB for _d_dtoll




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

; FUNCTION: _d_dtoll

; DESCRIPTION: Convert double to long long integer.

; INPUT:

;       dxh/dxl = double

; OUTPUT:

;       llh/lll = signed long long word

; USES:

;       vfpstate = type of convert

;
	AREA |.text|,CODE,READONLY
	
_ll_sfrom_d							; FPLIB entry point

_d_dtoll


        bic     sreg03,dxh,#0x80000000  ; sreg03 = absolute value

        ldr     sreg02,=0x43d           ; sreg02 = # of shifts ( 62 + bias - exp)

        subs    sreg02,sreg02,sreg03,LSR #20

        blt     dtoll_overflow          ; negative? overflow    



        cmp     sreg02,#63

        bge     dtoll_zero



        mov     sreg12,dxh,ASR #31      ; sreg12 = 0 if positive, -1 if negative



        orr     dxh,dxh,#0x00100000     ; add implicit 1 bit

        mov     dxh,dxh,LSL #10         ; align mantissa to 0x7fffffff...

        bic     dxh,dxh,#0x80000000 

        orr     dxh,dxh,dxl,LSR #22

        eor     dxh,dxh,sreg12          ; if negative, dxh = ~dxh

        eor     sreg03,sreg12,dxl,LSL #10  ; sreg03 = low mantissa word, aligned and negated

        adds    sreg03,sreg03,sreg12,LSR #31

        adc     sreg12,dxh,#0           ; sreg12 = hi mantissa word, aligned and negated



        cmp     sreg02,#32              ; 32/64 bit result

        ble     dtoll_64bit



        sub     sreg02,sreg02,#32

        mov     lll,sreg12,ASR sreg02   ; shift result into position

        mov     llh,sreg12,ASR #31      ; sign-extend the result

        rsb     sreg02,sreg02,#32

;;;        orrs    sreg12,sreg03,sreg12,LSL sreg02  ; sreg12 = round bits
	mov	sreg12,sreg12,LSL sreg02
	orrs	sreg12,sreg03,sreg12

#ifdef IEEE_FULL        

        Return  eq

        LFPState sreg03,sreg02

        SetInexact sreg03

        StFPState sreg03,sreg02 

        add     lll,lll,llh,LSR #31     ; if negative, round up

#else

        addne   lll,lll,llh,LSR #31

#endif

        Return



dtoll_64bit

        mov     llh,sreg12,ASR sreg02   ; shift result into position

        mov     lll,sreg03,LSR sreg02

        rsb     sreg02,sreg02,#32

;;;        orr     lll,lll,sreg12,LSL sreg02
	mov	sreg12,sreg12,LSL sreg02
	orr	lll,lll,sreg12

        movs    sreg12,sreg03,LSL sreg02  ; sreg12 = round bits



        Return  eq

#ifdef IEEE_FULL        

        LFPState sreg03,sreg02

        SetInexact sreg03

        StFPState sreg03,sreg02 

#endif

        adds    lll,lll,llh,LSR #31     ; if negative, round up

        adc     llh,llh,#0

        Return



dtoll_zero

#ifdef IEEE_FULL        

        orrs    sreg02,dxl,dxh,LSL #1   ; is it a true +/- zero?

#endif

        mov     lll,#0

        mov     llh,#0

#ifdef IEEE_FULL        

        Return  eq

        LFPState sreg03,sreg02

        SetInexact sreg03

        StFPState sreg03,sreg02 

#endif

        Return





dtoll_overflow

	#ifdef IEEE_FULL_OR_FIXED

        ldr     sreg02,=0xc3e00000      ; don't signal invalid for a true MININT

        cmp     dxh,sreg02

        cmpeq   dxl,#0

        beq     dtoll_minint

        LFPState sreg03,sreg02

        SetInvalidE sreg03

        StFPState sreg03,sreg02 

dtoll_minint

#endif

        mvn     lll,dxh,ASR #31         ; fx = -1 if positive, 0 if negative

        add     llh,lll,#0x80000000     ; MININT if -, MAXINT if +

        Return



	#ifdef IEEE_FULL_OR_FIXED

	IMPORT __fp_handler_64_to_64



; invalid exception handler

__wrap_invalid_handler

        ldr     r2, =(FE_EX_INVALID :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_LONGLONG :OR: FE_EX_FN_CVT :OR: FE_EX_ROUND_ZERO)

        stmfd   sp!,{lr}

        bl      __fp_handler_64_to_64

        ldmfd   sp!,{lr}

        Return

#endif



#ifdef IEEE_FULL



; inexact exception wrapper

__wrap_inexact_handler

        ldr     r2, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_LONGLONG :OR: FE_EX_FN_CVT :OR: FE_EX_ROUND_ZERO)

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
 
