;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:25 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _ll_mul.s

;===============================================================================



; @@@ All the code in this file was modified to use llh/lll instead of dxh/dxl

; @@@ and lyh/lyl instead of dyh/dyl in order to insure compatibility 

; @@@ with the SoftFPA mode



	#include "ARMsfpe.pre.asm"



;===============================================================================



	EXPORT _ull_mul

	EXPORT _ll_mul



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



#ifdef ARM_xM

	MACRO
	UMULL_M  $destlo, $desthi, $src1, $src2

        stmfd   sp!, {$src1, $src2}

        mov     sreg08, $src1, LSR #16 

        sub     $src1, $src1, sreg08, LSL #16 

        mov     sreg10, $src2, LSR #16 

        sub     $src2, $src2, sreg10, LSL #16 

        mul     $desthi, sreg08, sreg10 

        mul     sreg11, sreg08, $src2 

        mul     sreg08, $src1, sreg10 

        adds    sreg08, sreg08, sreg11

        addcs   $desthi, $desthi, #0x10000 

        mul     $destlo, $src1, $src2 

        adds    $destlo, $destlo, sreg08, LSL #16

        adc     $desthi, $desthi, sreg08, LSR #16  ; result is in desthi - destlo

        ldmfd   sp!, {$src1, $src2}

	MEND

#endif 




;===============================================================================

; FUNCTION: _ull_mul, _ll_mul  -- (un)signed long long multiply

; INPUT:        llh/lll = long long x,  lyh/lyl = long long y

; OUTPUT:       llh/lll = product

; COMMENTS: The low 64 bits of the 128-bit product of signed and unsigned values

;       is the same, conveniently, so we just use unsigned 64x64-->64 multiply.

;       In the case of signed multiply, to see how this works, note that a

;       a negative (2's-complement) value -M is represented as (2^64 - M).

;       Forming the product N*(2^64 - M) with unsigned arithmetic yields the

;       128-bit value  N*2^64 - N*M.  When the top 64 bits are truncated, this

;       amounts to truncated(2^128 - N*M), just what's sought.  The same reasoning

;       applies when both operands are negative.

; NOTE: THIS USES THE ARM 3M INSTRUCTION UMULL

	AREA |.text|,CODE,READONLY
	
_ll_mul

_ull_mul


;~~~~~~~

; result of llh/lll * lyh/lyl is

;     high 32-bits              low 32-bits

;----------------------------------

;       high lll*lyl            low lll*lyl

; + low  lll*lyh             0

; + low  lyl*llh                 0

;----------------------------------

        stmfd   sp!,{r4-r5,lr}

#ifndef ARM_xM  

        umull   r5,r4,lll,lyl

#else

        stmfd   sp!, {sreg08, sreg10, sreg11}

        UMULL_M r5, r4, lll, lyl

        ldmfd   sp!, {sreg08, sreg10, sreg11}

#endif  

        mla     r4,lll,lyh,r4

        mla     r4,lyl,llh,r4

        mov     llh,r4

        mov     lll,r5

        ldmfd   sp!,{r4-r5,lr}

        Return

	END



/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */
 

