;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:25 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _ll_sll.s

;===============================================================================



; @@@ All the code in this file was modified to use llh/lll instead of dxh/dxl

; @@@ in order to insure compatibility with the SoftFPA mode



	#include "ARMsfpe.pre.asm"



;===============================================================================



	EXPORT _ull_sll

	EXPORT _ll_sll

	EXPORT _ll_shl                 ; internal compiler for _ull_sll & _ll_sll



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

; FUNCTION: _ull_sll, _ll_sll  -- (un)signed long long shift left

; INPUT:    llh/lll = long long value,  r2 = long long count

; OUTPUT:   llh/lll = shifted value

; COMMENTS: Shifts by r2 mod 64, without checking for neg shift

;
	AREA |.text|,CODE,READONLY
	
_ll_shl

_ull_sll

_ll_sll

; Just use low six bits of shift

        ands    r2,r2,#0x03f

        Return  eq                      ; result is input

;~~~~~~~

        subs    r3,r2,#32               ; if shifting >= 32 bits, just use bits of lll

        bge     sll_big

;~~~~~~~

        rsb     r3,r2,#32

        mov     llh,llh,LSL r2          ; shift llh and OR in high lll bits

;;;        orr     llh,llh,lll,LSR r3
	mov	r3,lll,LSR r3
	orr	llh,llh,r3

        mov     lll,lll,LSL r2          ; shift lll

        Return

;~~~~~~~

sll_big

; result is some bits that used to be in lll

        mov     llh,lll,LSL r3

        mov     lll,#0

        Return

	END

/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */
 

