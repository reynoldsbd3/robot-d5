;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:23 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _d_dtoq.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



;===============================================================================



	EXPORT _d_qtod

	EXPORT _d_dtoq




;===============================================================================

; FUNCTION: _d_dtoq, _d_qtod

; DESCRIPTION: Convert between double and long double, which is trivial here.

; INPUT:

;       dxh/dxl = double or long double

; OUTPUT:

;       dxh/dxl = long double or long double

; TRICK: just return input

;
	AREA |.text|,CODE,READONLY

_d_qtod


#ifdef THUMB

        bx      lr

#else

        mov     pc,lr

#endif

;===============================================================================

; FUNCTION: _d_dtoq, _d_qtod

; DESCRIPTION: Convert between double and long double, which is trivial here.

; INPUT:

;       dxh/dxl = double or long double

; OUTPUT:

;       dxh/dxl = long double or long double

; TRICK: just return input

;

	AREA |.text|,CODE,READONLY
	
_d_dtoq


#ifdef THUMB

		bx		lr
		
#else

		mov pc, lr
		
#endif
	END



/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */
