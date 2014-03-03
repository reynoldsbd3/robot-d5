;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:23 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _d_dtoll_r.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



;===============================================================================

	EXPORT _ll_sfrom_d_r           ; Rounded _d_dtoll




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

; FUNCTION: _ll_sfrom_d_r

; DESCRIPTION: Convert double to long long integer. Rounded.

; INPUT:

;       dxh/dxl = double

; OUTPUT:

;       llh/lll = signed long long word


	AREA |.text|,CODE,READONLY

_ll_sfrom_d_r

        bic     sreg03,dxh,#0x80000000  ; sreg03 = absolute value

        ldr     sreg02,=0x43d           ; sreg02 = # of shifts ( 62 + bias - exp)

        subs    sreg02,sreg02,sreg03,LSR #20

        blt.w     dtoll_r_overflow        ; negative? overflow    



        cmp     sreg02,#63

        bgt     dtoll_r_zero



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

        ble     dtoll_r_64bit



        sub     sreg02,sreg02,#32

        mov     lll,sreg12,ASR sreg02   ; shift result into position

        mov     llh,sreg12,ASR #31      ; sign-extend the result

        rsb     sreg02,sreg02,#32

        mov     sreg03,sreg03,LSR #1    ; make room for the round bit

;;;        orrs    sreg12,sreg03,sreg12,LSL sreg02  ; sreg12 = round bits
	mov	sreg12,sreg12,LSL sreg02
	orrs	sreg12,sreg03,sreg12

        bne     dtoll_r_round

        Return



dtoll_r_64bit

        mov     llh,sreg12,ASR sreg02   ; shift result into position

        mov     lll,sreg03,LSR sreg02

        rsb     sreg02,sreg02,#32

;;;        orr     lll,lll,sreg12,LSL sreg02
	mov	sreg12,sreg12,LSL sreg02
	orr	lll,lll,sreg12

        movs    sreg12,sreg03,LSL sreg02  ; sreg12 = round bits

#ifdef IEEE_FULL        

        Return  eq

dtoll_r_round

        LFPState sreg03,sreg02          ; if any, signal inexact

        SetInexact sreg03

        StFPState sreg03,sreg02 

        tst     sreg03,CHOP_OR_DOWN_BIT_MASK

        bne     dtoll_r_down

        tst     sreg03,UP_OR_NEAR_BIT_MASK

        bne     dtoll_r_up

#else

dtoll_r_round

#endif

        tst     sreg12,#0x80000000      ; check rounding bit

        Return  eq                      ; 0 ? quit

        bics    sreg12,sreg12,#0x80000000  ; check sticky bits

        tsteq   lll,#1                  ; check LSB of the integer (same even if negated)

        Return  eq

dtoll_r_up

        adds    lll,lll,#1              ; round up: increment 

        adc     llh,llh,#0

        Return



#ifdef IEEE_FULL        

dtoll_r_down

        tst     sreg03,ZERO_OR_DOWN_BIT_MASK

        Return  eq

        adds    lll,lll,llh,LSR #31     ; round-to=zero: increment if negative

        adc     llh,llh,#0

        Return

#endif  



dtoll_r_zero

#ifdef IEEE_FULL        

        mov     sreg12,dxh

        orrs    sreg02,dxl,dxh,LSL #1   ; is it a true +/- zero?

#endif

        mov     lll,#0

        mov     llh,#0

#ifdef IEEE_FULL        

        Return  eq

        LFPState sreg03,sreg02

        SetInexact sreg03

        StFPState sreg03,sreg02 



        and     sreg03,sreg03,ROUND_MODE_MASK

        cmp     sreg03,FE_DOWNWARD

        beq     dtoll_r_zero_down

        cmp     sreg03,FE_UPWARD

        beq     dtoll_r_zero_up 

#endif

        Return



#ifdef IEEE_FULL        

dtoll_r_zero_up

        mov     lll,sreg12,LSR #31      ; 0 if positive, 1 if negative

        rsb     lll,lll,#1

        mov     llh,#0

        Return

dtoll_r_zero_down

        mov     lll,sreg12,ASR #31      ; 0 if positive, -1 if negative

        mov     llh,lll 

        Return

#endif     



dtoll_r_overflow

	#ifdef IEEE_FULL_OR_FIXED

        ldr     sreg02,=0xc3e00000      ; don't signal invalid for a true MININT

        cmp     dxh,sreg02

        cmpeq   dxl,#0

        beq     dtoll_r_minint

        LFPState sreg03,sreg02

        SetInvalidE sreg03

        StFPState sreg03,sreg02 

dtoll_r_minint

#endif

        mvn     lll,dxh,ASR #31         ; fx = -1 if positive, 0 if negative

        add     llh,lll,#0x80000000     ; MININT if -, MAXINT if +

        Return





	#ifdef IEEE_FULL_OR_FIXED

	IMPORT __fp_handler_64_to_64



; invalid exception handler

__wrap_invalid_handler

        ldr     r2, =(FE_EX_INVALID :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_LONGLONG :OR: FE_EX_FN_CVT)

        stmfd   sp!,{lr}

        bl      __fp_handler_64_to_64

        ldmfd   sp!,{lr}

        Return

#endif



#ifdef IEEE_FULL



; inexact exception wrapper

__wrap_inexact_handler

        ldr     r2, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_LONGLONG :OR: FE_EX_FN_CVT)

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
