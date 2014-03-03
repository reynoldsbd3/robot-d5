;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _div32_common_f.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



;===============================================================================
#ifdef USING_ARMASM
	#define PAD_2BYTES mov r0,r0
#else
	#define PAD_2BYTES
#endif


	EXPORT _div32_common_f

        



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

; FUNCTION: _div32_common_f

; DESCRIPTION: common function between _u32_div_f and _s32_div_f

; INPUT: 

;       r0,r1,r2,r3


; OUTPUT: 

;       r0 = quotient

;       r1 = remainder

; REGISTERS: r0, r1, r2, r3, lr

; MEMORY: 0 bytes

	AREA |.text|,CODE,READONLY

_div32_common_f




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
	add	r2,pc,r2
	mov	pc,r2			; branch into unrolled loop



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

        Return

	END

/* Change record:
 *  jrusso : added new file to abtract common div 32 code into a function.
 */

