;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:23 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _d_dtoi_r.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



;===============================================================================

	EXPORT _dfix_r                 ; Rounded for _d_dtoi



	#ifdef VFP_AND_NOT_IEEE_STD

	EXPORT _vfp_support_dfix_r

#endif



	AREA |.text|,CODE,READONLY



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

; FUNCTION: _dfix_r

; DESCRIPTION: Convert double to integer. Rounded.

; INPUT:

;       dxh/dxl = double

; OUTPUT:

;       fx = signed word

	AREA |.text|,CODE,READONLY

#ifdef VFP

_dfix_r

        fmdhr   d0, dxh

        fmdlr   d0, dxl

        ftosid  s0, d0

        fmrs    r0, s0

        Return



_vfp_support_dfix_r

#else

_dfix_r                                ; FPLIB entry point

#endif



	#ifdef NOT_VFP_OR_NOT_IEEE_STD



        bic     sreg03,dxh,#0x80000000  ; 

        ldr     sreg02,=0x41e           ; sreg02 = # of shifts ( 31 + bias - exp)

        subs    sreg02,sreg02,sreg03,LSR #20

        ble.w     dtoi_r_overflow         ; negative? overflow



        mov     sreg12,dxh,ASR #31      ; sreg12 = 0 if positive, -1 if negative



        cmp     sreg02,#32              ; > 32 ? zero

        bgt     dtoi_r_zero



        mov     sreg03,dxl,LSR #21      ; sreg03 = left-aligned mantissa

;;;        orr     sreg03,sreg03,dxh,LSL #11
	mov	dxh,dxh,LSL #11
	orr	sreg03,sreg03,dxh

        orr     sreg03,sreg03,#0x80000000



        mov     dxl,dxl,LSL #11         ; dxl = rounding bits left in dxl  

;;;        eor     dxh,sreg12,sreg03,LSR sreg02  ; dxh = result (not rounded)
	
	mov	dxh,sreg03,LSR sreg02
	eor	dxh,sreg12,dxh



        rsb     sreg02,sreg02,#32       ; add rounding bits left in sreg03

        mov     dxl,dxl,LSR #1          ; make room for round bit

;;;        orrs    sreg03,dxl,sreg03,LSL sreg02  ; sreg03 = rounding bits
	
	mov	sreg03,sreg03,LSL sreg02
	orrs	sreg03,dxl,sreg03



        add     fx,dxh,sreg12,LSR #31   ; fx = final result, not rounded



  #ifdef IEEE_FULL

        Return  eq                      ; return if no round bits



        LFPState sreg02,sreg01

        tst     sreg02,CHOP_OR_DOWN_BIT_MASK

        bne     dtoi_r_down             ; round to zero / -inf chops round bits

        tst     sreg02,UP_OR_NEAR_BIT_MASK

        bne     dtoi_r_up



        tst     sreg03,#0x80000000      ; check rounding bit

        beq     dtoi_r_inexact          ; 0 ? set inexact and quit

        bics    sreg03,sreg03,#0x80000000  ; check sticky bits

        tsteq   fx,#1                   ; check LSB of the integer (same even if negated)

        addne   fx,fx,#1                ; add 1 to round up

        addne   fx,fx,sreg12,LSL #1     ; add -2 if the input is negative

;;;        tst     fx,fx,lsl sreg12        ; test fx if positive, 0 if negative

	mov	sreg12,fx,lsl sreg12
	tst	fx,sreg12

        bmi     dtoi_r_invalid          ; if fx is positive & 0x80000000 - overflow

dtoi_r_inexact

        SetInexact sreg02

        StFPState sreg02,sreg01 

        Return

  #else

        tst     sreg03,#0x80000000      ; check rounding bit

        Return  eq                      ; 0 ? quit

        bics    sreg03,sreg03,#0x80000000  ; check sticky bits

        tsteq   fx,#1                   ; check LSB of the integer (same even if negated)

        addne   fx,fx,#1                ; add 1 to round up

        addne   fx,fx,sreg12,LSL #1     ; add -2 if the input is negative

;;;        tst     fx,fx,lsl sreg12        ; test fx if positive, 0 if negative

	mov	sreg12,fx,lsl sreg12
	tst	fx,sreg12

        Return  pl                      ; if fx is 0x80000000 & initial sign + => overflow

    #ifdef IEEE_FIXED

        LFPState sreg02,sreg01

        SetInvalidE sreg02              ; set invalid

        StFPState sreg02,sreg01 

    #endif        

        sub     fx,fx,#1                ; return MAXINT 

        Return

  #endif  



  #ifdef IEEE_FULL

dtoi_r_up

        add     fx,fx,#1                ; if positive add 1

        add     fx,fx,sreg12            ; if negative add 1 - 1 => keep #

        cmp     fx,#0x80000000

        beq     dtoi_r_invalid          ; if fx is 0x80000000 -> overflow

        SetInexact sreg02

        StFPState sreg02,sreg01 

        Return



dtoi_r_down                            ; round: towards zero or down

        SetInexact sreg02               ; set inexact

        StFPState sreg02,sreg01 

        tst     sreg02,ZERO_OR_DOWN_BIT_MASK

        addeq   fx,fx,sreg12            ; if round down and negative: add -1

        Return



dtoi_r_invalid

        SetInvalidE sreg02              ; set invalid

        StFPState sreg02,sreg01 

        sub     fx,fx,#1                ; return MAXINT 

        Return



dtoi_r_zero

        orrs    fx,dxl,dxh,LSL #1       ; check for a true zero

        Return  eq

        mov     fx,#0

        LFPState sreg02,sreg01

        SetInexact sreg02

        StFPState sreg02,sreg01 



        and     sreg02,sreg02,ROUND_MODE_MASK

        cmp     sreg02,FE_TONEAREST

        cmpne   sreg02,FE_TOWARDZERO

        Return  eq

        cmp     sreg02,FE_UPWARD

        addeq   fx,sreg12,#1

        movne   fx,sreg12

        Return



  #else

dtoi_r_zero

        mov     fx,#0

        Return

  #endif



dtoi_r_overflow

	#ifdef IEEE_FULL_OR_FIXED

        ldr     sreg02,=0xc1e00000

        cmp     dxh,sreg02

        beq     dtoi_r_check_minint

dtoi_r_not_minint

        LFPState sreg03,sreg02          ; signal invalid

dtoi_r_ovf_invalid

        SetInvalidE sreg03

        StFPState sreg03,sreg02 

  #endif  

        mvn     fx,dxh,ASR #31          ; fx = -1 if positive, 0 if negative

        add     fx,fx,#0x80000000       ; MININT if -, MAXINT if +      

        Return



	#ifdef IEEE_FULL_OR_FIXED

dtoi_r_check_minint

    #ifdef IEEE_FULL

        cmp     dxl,#0

        beq     dtoi_r_true_minint

        LFPState sreg03,sreg02          ; signal inexact

        tst     sreg03,CHOP_OR_UP_BIT_MASK  ; round up / zero? invalid!

        beq     dtoi_r_near_minint



        cmp     dxl,#0x00200000

        bhs     dtoi_r_ovf_invalid

        b       dtoi_r_inexact_minint



dtoi_r_near_minint

        tst     sreg03,DOWN_OR_NEAR_BIT_MASK

        bne     dtoi_r_ovf_invalid

    #endif

        cmp     dxl,#0x00100000

        bhi     dtoi_r_not_minint

    #ifdef IEEE_FULL

dtoi_r_inexact_minint

        mov     fx,#0x80000000

        SetInexact sreg03

        StFPState sreg03,sreg02 

        Return



dtoi_r_true_minint

    #endif

        mov     fx,#0x80000000

        Return

  #endif



	#ifdef IEEE_FULL_OR_FIXED



	IMPORT __fp_handler_64_to_32

; invalid exception handler

__wrap_invalid_handler

        ldr     r2, =(FE_EX_INVALID :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_INT :OR: FE_EX_FN_CVT)

        stmfd   sp!,{lr}

        bl      __fp_handler_64_to_32

        ldmfd   sp!,{lr}

        Return

  #endif



  #ifdef IEEE_FULL

	IMPORT __fp_handler_32_to_32

; inexact exception wrapper

__wrap_inexact_handler

        ldr     r1, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_INT :OR: FE_EX_FN_CVT)

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
 
