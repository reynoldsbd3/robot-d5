;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _f_ftoll_r.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



;===============================================================================

	EXPORT _ll_sfrom_f_r           ; Rounded _f_ftoll



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

; FUNCTION: _ll_sfrom_f_r

; DESCRIPTION: Convert float to long long integer.

; INPUT:

;       fx = float

; OUTPUT:

;       llh/lll = signed long long word

; OPERATION:

;       Negate the operand if necessary, then shift it to the right position

	AREA |.text|,CODE,READONLY

_ll_sfrom_f_r

        bic     sreg03,fx,#0x80000000   ; sreg03 = absolute value

        mov     sreg02,#0xbd            ; sreg02 = # of shifts ( 62 + bias - exp)

        subs    sreg02,sreg02,sreg03,LSR #23

        blt     ftoll_r_overflow        ; negative? overflow    



        cmp     sreg02,#63

        bgt     ftoll_r_zero



        mov     sreg12,fx,ASR #31       ; sreg12 = 0 if positive, -1 if negative



        mov     fx,fx,LSL #8            ; left align mantissa

        orr     fx,fx,#0x80000000       ; add implicit 1 bit

        eor     sreg03,sreg12,fx,LSR #1  ; sreg03 = result (shifted, signed) 

        add     sreg03,sreg03,sreg12,LSR #31  ; r3 = ~r3 + 1 if sreg12 == -1



        cmp     sreg02,#32

        ble     ftoll_r_64bit           ; 32 / 64 bit result



        sub     sreg02,sreg02,#32

        mov     llh,sreg12              ; sign-extend result

        mov     lll,sreg03,ASR sreg02   ; lll = sreg03 >> #shifts

        rsb     sreg02,sreg02,#32

        movs    sreg03,sreg03,LSL sreg02  ; sreg03 = round bits

#ifdef IEEE_FULL        

        Return  eq

        LFPState sreg12,sreg02          ; if any, signal inexact

        SetInexact sreg12

        StFPState sreg12,sreg02 

        tst     sreg12,CHOP_OR_DOWN_BIT_MASK

        bne     ftoll_r_down

        tst     sreg12,UP_OR_NEAR_BIT_MASK

        bne     ftoll_r_up

#endif

        tst     sreg03,#0x80000000      ; check rounding bit

        Return  eq                      ; 0 ? quit

        bics    sreg03,sreg03,#0x80000000  ; check sticky bits

        tsteq   lll,#1                  ; check LSB of the integer (same even if negated)

        Return  eq



ftoll_r_up

        adds    lll,lll,#1              ; round up: increment 

        adc     llh,llh,#0

        Return



#ifdef IEEE_FULL        

ftoll_r_down

        tst     sreg12,ZERO_OR_DOWN_BIT_MASK

        Return  eq

        adds    lll,lll,llh,LSR #31     ; round-to=zero: increment if negative

        adc     llh,llh,#0

        Return

#endif  



ftoll_r_64bit

        mov     llh,sreg03,ASR sreg02   ; llh = sreg03 >> #shifts

        rsb     sreg02,sreg02,#32

        mov     lll,sreg03,LSL sreg02   ; lll = sreg03 << (32 - #shifts)

        Return



ftoll_r_zero

#ifdef IEEE_FULL        

        mov     sreg12,fx

        bics    sreg01,fx,#0x80000000   ; is it a true +/- zero?

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

        beq     ftoll_r_zero_down

        cmp     sreg03,FE_UPWARD

        beq     ftoll_r_zero_up 

#endif

        Return



#ifdef IEEE_FULL        

ftoll_r_zero_up

        mov     lll,sreg12,LSR #31      ; 0 if positive, 1 if negative

        rsb     lll,lll,#1

        mov     llh,#0

        Return

ftoll_r_zero_down

        mov     lll,sreg12,ASR #31      ; 0 if positive, -1 if negative

        mov     llh,lll 

        Return

#endif



ftoll_r_overflow

	#ifdef IEEE_FULL_OR_FIXED

        cmp     fx,#0xdf000000          ; don't signal invalid for a true MININT

        beq     ftoll_r_minint

        LFPState sreg03,sreg02

        SetInvalidE sreg03

        StFPState sreg03,sreg02 

ftoll_r_minint

#endif

        mvn     lll,fx,ASR #31          ; fx = -1 if positive, 0 if negative

        add     llh,lll,#0x80000000     ; MININT if -, MAXINT if +

        Return



	#ifdef IEEE_FULL_OR_FIXED

	IMPORT __fp_handler_32_to_64



; invalid exception handler

__wrap_invalid_handler

        ldr     r1, =(FE_EX_INVALID :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_LONGLONG :OR: FE_EX_FN_CVT)

        stmfd   sp!,{lr}

        bl      __fp_handler_32_to_64

        ldmfd   sp!,{lr}

        Return

#endif



#ifdef IEEE_FULL

	IMPORT __fp_handler_64_to_64



; inexact exception wrapper

__wrap_inexact_handler

        ldr     r2, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_LONGLONG :OR: FE_EX_FN_CVT)

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
