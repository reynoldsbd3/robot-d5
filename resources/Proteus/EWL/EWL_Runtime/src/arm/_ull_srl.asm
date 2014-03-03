;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:25 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _ull_srl.s

;===============================================================================



; @@@ All the code in this file was modified to use llh/lll instead of dxh/dxl

; @@@ in order to insure compatibility with the SoftFPA mode



	#include "ARMsfpe.pre.asm"



;===============================================================================



	EXPORT _ull_srl

	EXPORT _ll_ushr                ; internal compiler for _ull_srl



; @@@ Modified here (external update)

	EXPORT __srl_big



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

; FUNCTION: _ull_srl  -- unsigned long long shift right

; INPUT:    llh/lll = long long value,  r2 = long long count

; OUTPUT:   llh/lll = shifted value

; COMMENTS: Shifts by r2 mod 64, without checking for neg shift

;
	AREA |.text|,CODE,READONLY
	
_ll_ushr

_ull_srl


; Just use low six bits of shift

        ands    r2,r2,#0x03f

        Return  eq                      ; result is input

;~~~~~~~

        subs    r3,r2,#32               ; if shifting >= 32 bits, just use bits of llh

        bge     __srl_big

;~~~~~~~

        rsb     r3,r2,#32

        mov     lll,lll,LSR r2          ; shift lll and OR in low llh bits

;;;        orr     lll,lll,llh,LSL r3
	mov	r3,llh,LSL r3
	orr	lll,lll,r3

        mov     llh,llh,LSR r2          ; shift llh

        Return

;~~~~~~~

__srl_big

; result is some bits that used to be in llh

        mov     lll,llh,LSR r3

        mov     llh,#0

        Return

	END

/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */
 

