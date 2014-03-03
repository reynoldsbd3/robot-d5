;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:25 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _u32_div_f.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



;===============================================================================
#ifdef USING_ARMASM
	#define PAD_2BYTES mov r0,r0
#else
	#define PAD_2BYTES
#endif

#ifdef _AEABI
	EXPORT __aeabi_uidivmod
        EXPORT __aeabi_uidiv
#endif 
	EXPORT __aeabi_ldiv0


	EXPORT _u32_div_f

	EXPORT _u32_div_not_0_f
        
#if defined(US32_DIV_SZ)
	EXPORT _div32_common_f
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

; FUNCTION: _u32_div_f

; DESCRIPTION: Fast, but lengthy, 32-bit unsigned divide.

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

_u32_div_f

#ifdef _AEABI
__aeabi_uidivmod
__aeabi_uidiv
#endif

; if divisor = 0, result is undefined - just return

        cmp     r1,#0

        Return  eq

_u32_div_not_0_f                       ; NOTE: Entry point when the divisor is not 0



; if dividend < divisor; quotient is 0, remainder is dividend

        cmp     r0,r1

        movlo   r1,r0

        movlo   r0,#0

        Return  lo


; MTWX17667
#if defined(US32_DIV_SZ)
		stmfd sp!,{r3,lr}					; save return if you got this far

		bl	_div32_common_f

#else
; determine how far left to shift the dividend (r0) before actually

; dividing. Shifts are calculated to the nearest 4-bit increment (0,4,8,etc.)

; [algorithm is from a 1991 post by Graeme Williams of University of Waterloo]



        mov     r2,#28

        mov     r3,r0,LSR #4

        cmp     r1,r3,LSR #12           ; will shift dividend by 16 or 0

        suble   r2,r2,#16

        movle   r3,r3,LSR #16

        cmp     r1,r3,LSR #4            ; will shift additional 8 or 0

        suble   r2,r2,#8

        movle   r3,r3,LSR #8

        cmp     r1,r3                   ; will shift additional 4 or 0

        suble   r2,r2,#4

        movle   r3,r3,LSR #4

;

        mov     r0,r0,LSL r2            ; shift dividend by multiple of 4 bits

        rsb     r1,r1,#0                ; reverse divisor sign for unrolled loop to work

        adds    r0,r0,r0                ; set that carry bit!



; skip over r2 copies of the 3 instruction loop.

        add     r2,r2,r2,LSL #1         ; multiply count by 3
;;;        add     pc,pc,r2,LSL #2
	mov	r2,r2,LSL #2
	add 	r2,pc,r2		; r2 = r2 + (pc+4)
        mov     pc,r2                   ; branch into unrolled loop

; for each bit: remainder = remainder * 2 + high bit of dividend + -divisor

;                               if there is no carry out, add back in the divisor

;                               dividend = dividend + dividend + carry

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        adcs    r3,r1,r3,LSL #1
	PAD_2BYTES

        subcc   r3,r3,r1

        adcs    r0,r0,r0

        mov     r1,r3
        
#endif

#if defined(US32_DIV_SZ)
	ldmfd sp!,{r3,lr}					; save return
#endif
        Return

__aeabi_ldiv0
        
        mov r1,r0
        mov r0,#0        


; US32_DIV_SZ

#if defined(US32_DIV_SZ)
	ldmfd sp!,{r3,lr}					; save return
#endif
        Return

	END

/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 *
 * jrusso Mar 06, abtract common part to subroutine for size libraries.
 */

