;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _f_rnd.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



;===============================================================================


	EXPORT _frnd


;===============================================================================

; FUNCTION: _frnd

; DESCRIPTION: IEEE754 round-to-integer operation. The function takes a number 

;              and rounds it to an integer (in accordance with the current 

;              rounding mode), but returns that integer in the floating-point 

;              number format rather than as a C int variable.

; INPUT:

;       fx = float

; OUTPUT:

;       fx = float



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



	#ifdef IEEE_FULL_OR_FIXED

	MACRO
	FRND_SAVE

        stmfd   sp!,{lr}

	MEND



	MACRO
	FRND_RESTORE

        ldmfd   sp!,{lr}

	MEND

#endif


	AREA |.text|,CODE,READONLY


_frnd

        mov     sreg02,fx,LSL #1        ; sreg02 = absolute value << 1

        cmp     sreg02,#0x96000000      ; >= 2^23 ? (0x96 = 23 + 0x7f) 

#ifdef IEEE_STD

        Return hs                      ; return input

#else

        bhs.w     frnd_check_nan          ; quiet NaNs and return input

#endif  

        cmp     sreg02,#0x7e000000      ; <= 0.5 ? return rounded 0.0

#ifdef IEEE_FULL

        bls     frnd_check_zero

#else

        eorls   fx,fx,sreg02,LSR #1     ; fx = signed 0.0       

        Return ls

#endif  

        mov     sreg01,sreg02,LSR #24 

        subs    sreg01,sreg01,#127      ; sreg01 = # of mask shifts

        blt     frnd_check_half         ; negative? input is < 1.0

        ldr     sreg03,=0x007fffff      ; sreg03 = mask of cleared bits

        mov     sreg03,sreg03,LSR sreg01

        bic     fx,fx,sreg03            ; fx = result (not rounded)

#ifdef IEEE_FULL

        tst     sreg02,sreg03,LSL #1    ; check if any rounding is needed

        Return eq                      ; quit if no rounding

#endif  

        add     sreg01,sreg03,#1        ; sreg01 = mask + 1 (rounding increment)

#ifdef IEEE_FULL

        FRND_SAVE

        LFPState sreg12, lr

        SetInexact sreg12

        StFPState sreg12,lr 

        FRND_RESTORE



        tst     sreg12,CHOP_OR_DOWN_BIT_MASK

        bne     frnd_down

        tst     sreg12,UP_OR_NEAR_BIT_MASK

        bne     frnd_up

#endif



        tst     sreg02,sreg01           ; check rounding bit

        Return eq                      ; 0 ? quit

        tst     sreg02,sreg03           ; check sticky bits

        tsteq   sreg02,sreg01,LSL #1    ; check LSB of the integer part

        addne   fx,fx,sreg01

        Return

; TRICK: if fx == 1.xxx, it is represented as 0x3f8xxxxx. The LSB check

; tests bit #23, which is out of the significant, but is 1, so the code 

; works for this case too



#ifdef IEEE_FULL

frnd_up

        tst     fx,#0x80000000

        addeq   fx,fx,sreg01            ; return fx + !sign

        Return



frnd_down

        tst     sreg12,ZERO_OR_DOWN_BIT_MASK

        Return ne                      ; chop? return fx

        tst     fx,#0x80000000

        addne   fx,fx,sreg01            ; return fx + sign

        Return

#endif



; result is rounded 0.5



frnd_check_half

        eor     fx,fx,sreg02,LSR #1     ; fx = signed 0.0       

#ifdef IEEE_FULL

        LFPState sreg03, sreg01

        SetInexactL sreg03,__wrap_inexact_handler_save

        StFPState sreg03,sreg01

        tst     sreg03,CHOP_OR_DOWN_BIT_MASK

        bne     frnd_zero_down

        tst     sreg03,UP_OR_NEAR_BIT_MASK

        tstne   fx,#0x80000000          ; round to + inf? check sign

        orreq   fx,fx,#0x3f800000       ; return sign ? 1.0 : -0.0

#else

        orr     fx,fx,#0x3f800000

#endif

        Return



; result is 0.0



#ifdef IEEE_FULL

frnd_check_zero

        eor     fx,fx,sreg02,LSR #1     ; fx = signed 0.0       

        cmp     sreg02,#0               ; return if there's a true 0

        Return eq

        LFPState sreg03, sreg01

        SetInexactL sreg03,__wrap_inexact_handler_save

        StFPState sreg03,sreg01

        tst     sreg03,CHOP_OR_DOWN_BIT_MASK

        bne     frnd_zero_down

        tst     sreg03,UP_OR_NEAR_BIT_MASK

        Return eq                      ; nearest? 0.0

        tst     fx,#0x80000000          ; round to + inf

        orreq   fx,fx,#0x3f800000       ; return sign ? 1.0 : -0.0

        Return



frnd_zero_down

        tst     sreg03,ZERO_OR_DOWN_BIT_MASK

        Return ne                      ; chop? return 0.0

        tst     fx,#0x80000000

        orrne   fx,fx,#0x3f800000       ; return sign ? -1.0 : 0.0

        Return

#endif



; result is >= 2^23



frnd_check_nan

        cmp     sreg02,#0xff000000      ; is it a NaN?

	#ifdef IEEE_FULL_OR_FIXED

        Return ls                      ; no? return input

        tst     sreg02,#0x00800000      ; is it a signaling NaN?

        Return ne                      ; no? return

        LFPState sreg03, sreg01

        SetInvalidE sreg03

        StFPState sreg03,sreg01 

        orr     fx,fx,#0x00400000       ; quiet the NaN

#else

        orrhi   fx,fx,#0x00400000       ; quiet the NaN

#endif

        Return





	#ifdef IEEE_FULL_OR_FIXED

	IMPORT __fp_handler_32_to_32



; invalid exception handler

__wrap_invalid_handler

        ldr     r1, =(FE_EX_INVALID :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_RND)

        FRND_SAVE

        bl      __fp_handler_32_to_32

        FRND_RESTORE

        Return

#endif



#ifdef IEEE_FULL

; inexact exception wrapper

__wrap_inexact_handler_save

        FRND_SAVE

__wrap_inexact_handler

        ldr     r1, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_RND)

        bl      __fp_handler_32_to_32

        FRND_RESTORE

        Return

#endif
	END

/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */
