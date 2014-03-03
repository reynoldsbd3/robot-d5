;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _f_neg.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



;===============================================================================



	EXPORT _f_neg



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

; FUNCTION: _f_neg

; DESCRIPTION: Negate value (i.e. flip its sign bit).

;

; INPUT: fx

; OUTPUT: fx

;

	AREA |.text|,CODE,READONLY
	
_f_neg

        eor     fx,fx,#0x80000000

        Return

;~~~~~~~
	END


/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */



