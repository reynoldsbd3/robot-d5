;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:25 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _u32_div.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



;===============================================================================



	EXPORT _u32_div

	EXPORT __rt_udiv               ; internal compiler for backwards _u32_div 

	EXPORT _u32_div_not_0



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

; FUNCTION: _u32_div

; DESCRIPTION: Short, and maybe fast, 32-bit unsigned divide.

; INPUT: 

;       r0 = dividend

;       r1 = divisor

;       lr = return address

; OUTPUT: 

;       r0 = quotient

;       r1 = remainder

; REGISTERS: r0, r1, r2, r3, lr

; MEMORY: 0 bytes

	AREA |.text|,CODE,READONLY

__rt_udiv



; conform to backwards ARM profile - BC



        mov     r3,r0

        mov     r0,r1

        mov     r1,r3

_u32_div

; if divisor = 0, result is undefined - just return

        cmp     r1,#0

        Return  eq

_u32_div_not_0                         ; NOTE: Entry point when the divisor is not 0

; calculate r2 = r0 / r1, with the remainder in r0

; based on 3/26/2002 comp.sys.arm post by Matthias Seifert, Acorn RiscPC

        mov     r3,#1

; ; multiply divisor till it is >= dividend, or its bit 31 is set

u32_div_loop1

        cmp     r1,#0x80000000

        cmplo   r1,r0

        movlo   r1,r1,LSL #1

        movlo   r3,r3,LSL #1            ; r3 = times divisor was multiplied (a power of 2)

        blo     u32_div_loop1



        mov     r2,#0

u32_div_loop2

        cmp     r0,r1                   ; if dividend >= divisor

        addhs   r2,r2,r3                ;    subtract divisor and add in r3

        subhss  r0,r0,r1

        movnes  r3,r3,LSR #1            ; half r3 as you half the divisor

        movne   r1,r1,LSR #1

        bne     u32_div_loop2



; NOTE: if we use this algorithm, we can save a mov by returning quotient

;               in r1 and remainder in r0 

        mov     r1,r0                   ; move remainder to r1

        mov     r0,r2                   ; move quotient to r0

        Return


	END


/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */
