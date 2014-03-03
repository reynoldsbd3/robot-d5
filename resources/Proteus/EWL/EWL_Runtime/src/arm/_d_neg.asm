;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _d_neg.s

;===============================================================================

;;;	.option no_section_resume on

	#include "ARMsfpe.pre.asm"



;===============================================================================


	EXPORT _d_neg


;===============================================================================

; FUNCTION: _d_neg

; DESCRIPTION: Negate value (i.e. flip its sign bit).

;

; INPUT: dx

; OUTPUT: dx

	AREA |.text|,CODE,READONLY
_d_neg

        eor     dxh,dxh,#0x80000000

#ifdef THUMB

        bx      lr

#else

        mov     pc,lr

#endif
	END



/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */


