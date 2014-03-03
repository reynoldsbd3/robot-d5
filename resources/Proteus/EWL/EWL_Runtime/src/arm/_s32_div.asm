;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:25 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _s32_div.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



;===============================================================================



	EXPORT _s32_div

	EXPORT __rt_sdiv               ; internal compiler for backwards _s32_div



	IMPORT _u32_div



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

; FUNCTION: _s32_div

; DESCRIPTION: 32-bit signed divide. Calls short, and maybe fast, unsigned divide.

; INPUT: 

;       r0 = dividend

;       r1 = divisor

;       lr = return address

; OUTPUT: 

;       r0 = quotient

;       r1 = remainder

; REGISTERS: r0, r1, r2, r3, r4, lr

; MEMORY: 0 bytes


	AREA |.text|,CODE,READONLY

__rt_sdiv

; conform to backwards ARM profile - BC



        mov     r3,r0

        mov     r0,r1

        mov     r1,r3



_s32_div

        stmfd   sp!,{r4,lr}

        eor     r4,r0,r1                ; r4.31 = sign of quotient (0 for positive)

        and     r4,r4,#0x80000000

        cmp     r0,#0                   ; if r0 is negative, negate it

        rsblt   r0,r0,#0

        addlt   r4,r4,#1                ; r4.0 = 1 indicates remainder will be negative

        cmp     r1,#0                   ; if r1 is negative, negate it

        rsblt   r1,r1,#0

;~~~~~~~

        bl      _u32_div

;~~~~~~~

        ands    r3,r4,#0x80000000       ; negate quotient if needed

        rsbne   r0,r0,#0

        ands    r3,r4,#0x00000001       ; negate remainder if needed

        rsbne   r1,r1,#0

        ldmfd   sp!,{r4,lr}

        Return
	
	END


/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */


