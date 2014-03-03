;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:25 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _ll_sra.s

;===============================================================================



; @@@ All the code in this file was modified to use llh/lll instead of dxh/dxl

; @@@ in order to insure compatibility with the SoftFPA mode



	#include "ARMsfpe.pre.asm"



;===============================================================================



	EXPORT _ll_sra

	EXPORT _ll_sshr                ; internal compiler for _ll_sra




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

; FUNCTION: _ll_sra  -- signed long long shift right

; INPUT:    llh/lll = long long value,  r2 = long long count

; OUTPUT:   llh/lll = shifted value

; COMMENTS: Shifts by r2 mod 64, without checking for neg shift

;
	AREA |.text|,CODE,READONLY
	
_ll_sshr

_ll_sra


; Just use low six bits of shift

        ands    r2,r2,#0x03f

        Return  eq                      ; result is input

;~~~~~~~

        subs    r3,r2,#32               ; if shifting >= 32 bits, just use bits of llh + sign of llh

        bge     sra_big

;

;~~~~~~~

        rsb     r3,r2,#32

        mov     lll,lll,LSR r2          ; shift lll and OR in low llh bits

;;;        orr     lll,lll,llh,LSL r3
	mov	r3,llh,LSL r3
	orr	lll,lll,r3

        mov     llh,llh,ASR r2          ; shift llh

        Return

;~~~~~~~

sra_big

; result is some bits that used to be in llh + llh sign extend

        mov     lll,llh,ASR r3

        mov     llh,llh,ASR #31

        Return

	END



/* Change record:
 * clm  July 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */
 

