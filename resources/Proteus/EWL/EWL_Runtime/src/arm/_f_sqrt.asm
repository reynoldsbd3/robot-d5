;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _f_sqrt.s

;===============================================================================



	#include "ARMsfpe.pre.asm"
	IMPORT errno
#define EDOM  33	;if value for EDOM defined in cerrno is changed, this 
					;must also be changed.					/*- mm  040625 -*/

;===============================================================================



	EXPORT _fsqrt



	#ifdef VFP_AND_NOT_IEEE_STD

	EXPORT _vfp_support_fsqrt

#endif



;===============================================================================

; FUNCTION: _fsqrt

; DESCRIPTION: IEEE754 square root.            

; INPUT:

;       fx = float

; OUTPUT:

;       fx = float

; SPECIAL CASES:

;       sqrt(qNaN) = qNaN

;       sqrt(sNaN) = qNaN and signal invalid

;       sqrt(+/-0) = +/-0

;       sqrt(+inf) = +inf

;       sqrt(-inf) = qNaN and signal invalid

;       sqrt(-val) = qNaN and signal invalid



	MACRO
	FSQRT_SAVE

        stmfd   sp!,{r4,sreg07}

	MEND



#if defined(THUMB)
; && !defined(ARM5)

	MACRO
	FSQRT_RETURN $cc

        ldm$cc.fd sp!,{r4,sreg07}

        bx$cc  lr

	MEND

#else

	MACRO
	FSQRT_RETURN $cc

        ldm$cc.fd sp!,{pc}

	MEND

#endif  

	AREA |.text|,CODE,READONLY

#ifdef VFP

_f_sqrt

        fmsr    s0, r0

        fsqrts  s0, s0

        fmrs    r0, s0

  #ifdef THUMB        

        bx      lr

  #else        

        mov     pc,lr

  #endif



_vfp_support_fsqrt

#else

_fsqrt

#endif



	#ifdef NOT_VFP_OR_NOT_IEEE_STD



        FSQRT_SAVE

        mov     sreg02,#0xff

        cmp     fx,sreg02,LSL #23

        bhs     fsqrt_special           ; Check for Negative / NaN / Inf



        movs    sreg12,fx,LSR #23       ; sreg12 = exponent

        beq     fsqrt_zero              ; 0 / denorm case?

        bic     fx,fx,sreg02,LSL #23    ; clear exponent bits in fx

        orr     fx,fx,#0x00800000       ; add implicit 1 bit    

fsqrt_denorm_return



        movs    sreg12,sreg12,ASR #1    ; result exponent = input exponent / 2

        bcs     fsqrt_odd_exponent      ; biased input exponent should be odd

        sub     sreg12,sreg12,#1        ; adjust exponent if input exp is even

        mov     fx,fx,LSL #1            ; fx <<= 1

fsqrt_odd_exponent



; Generate SQRT bit-by-bit



;       q = sqrt(i)

;

;       i <<= 1;

;       q = 0;

;       s = 0;

;       for (r = 0x01000000; r; r >>= 1)

;         {

;         t = s + r;

;         if ( t <= i )

;           {

;           s = t + r;

;           i -= t;

;           q += r;

;           }

;         i <<= 1;

;         }

;         



        movs    sreg01,fx,LSL #1        ; i <<= 1 (compute and extra bit for rounding)

        mov     fx,#0                   ; q = 0

        mov     sreg02,#0               ; s = 0

        mov     sreg07,#0x01000000          ; r = 0x01000000

fsqrt_loop

        add     sreg03,sreg02,sreg07        ; t = s + r

        cmp     sreg03,sreg01           ; if ( t <= i )

        addle   sreg02,sreg03,sreg07        ;   s = t + r

        suble   sreg01,sreg01,sreg03    ;   i -= t;

        addle   fx,fx,sreg07                ;   q0 += r

        mov     sreg01,sreg01,LSL #1    ; i0 <<= 1

        movs    sreg07,sreg07,LSR #1            ; r >>= 1

        bne     fsqrt_loop



; Rounding      

;   i - remainder (treat as sticky for rounding)

;   q = sqrt (fractional part, 25 bits including lead 1 bit)

;       round to nearest according to the last bit of q



        cmp     sreg01,#0               ; any remainder? round

        biceq   fx,fx,#1                ; clear sticky bit if no rounding

  #ifdef IEEE_FULL

        beq     fsqrt_round

        LFPState sreg03, sreg02

        SetInexact sreg03

        StFPState sreg03,sreg02 

        tst     sreg03,CHOP_OR_DOWN_BIT_MASK

        bicne   fx,fx,#1                ; clear sticky bit if rounding down

        bne     fsqrt_round

        tst     sreg03,UP_OR_NEAR_BIT_MASK

        orrne   fx,fx,#1                ; set sticky bit if rounding up

fsqrt_round

  #endif

        movs    fx,fx,LSR #1            ; shift out the rounding bit

        adc     fx,fx,#0                ; bump result if the bit shifted out is 1



        add     fx,fx,#0x20000000       ; put exponent (r12 + 0x1f8)

        sub     fx,fx,#0x00800000

        add     fx,fx,sreg12,LSL #23

        FSQRT_RETURN



; normalize the denorm

fsqrt_zero

  #ifdef IEEE_STD

        mov     fx,#0

        FSQRT_RETURN

  #else

        cmp     fx,#0 

        FSQRT_RETURN eq                 ; sqrt(0) = 0



fsqrt_tiny_high

        Normalize_num fx, sreg12, #9    ; normalize high word

        mov     fx,fx,LSR #8

        b       fsqrt_denorm_return

  #endif  



; negative, NaN, Inf    



fsqrt_special

        FSQRT_RETURN eq                 ; sqrt(+inf) = +inf

        tst     fx,#0x80000000          ; NaN is the remaining positive special case

        beq     fsqrt_nan



        bics    sreg03,fx,#0x80000000   ; sreg03 = abs(fx)

  #ifdef IEEE_STD

        movs    sreg12,sreg03,LSR #23   ; -denorm ? make it zero

        moveq   fx,#0

  #endif  

        FSQRT_RETURN eq                 ; sqrt(-0) = -0



	#ifdef IEEE_FULL_OR_FIXED

        cmp     sreg03,sreg02,LSL #23   ; exponent < 7F8 ?

        blo     fsqrt_invalid           ; sqrt(-val) = qNan -> invalid

        movs    sreg02,fx,LSL #9 

        beq     fsqrt_invalid           ; sqrt(-inf) = qNaN -> invalid

  #endif



fsqrt_nan

	#ifdef IEEE_FULL_OR_FIXED

        tst     fx,#0x00400000          ; sqrt(qNan) = qNaN

        FSQRT_RETURN ne

fsqrt_invalid

  #endif  

        ldr     sreg02,=0x7fc00000

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg03, sreg12

        SetInvalidE sreg03

        StFPState sreg03,sreg12 

  #endif

        orr     fx,fx,sreg02            ; create quiet NaN
        
        ldr     sreg03,=errno			; set errno to EDOM if     	 /*- mm 040625 -*/
        
        ldr     sreg04,=EDOM			; argument our of range		 /*- mm 040625 -*/
        
        str     sreg04,[sreg03]			;							 /*- mm 040625 -*/

        FSQRT_RETURN



	#ifdef IEEE_FULL_OR_FIXED

	IMPORT __fp_handler_32_to_32



; invalid exception handler

__wrap_invalid_handler

        ldr     r1, =(FE_EX_INVALID :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_SQRT)

        bl      __fp_handler_32_to_32

        FSQRT_RETURN

  #endif



  #ifdef IEEE_FULL

; inexact exception wrapper

__wrap_inexact_handler

        movs    fx,fx,LSR #1            ; shift out the rounding bit

        add     fx,fx,#0x20000000       ; put exponent (r12 + 0x1f8)

        sub     fx,fx,#0x00800000

        add     fx,fx,sreg12,LSL #23

        ldr     r1, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_SQRT)

        bl      __fp_handler_32_to_32

        FSQRT_RETURN

  #endif



#endif
	END

; CHANGE LOG
; mm  040625 Changes to set errno if argument our of range
; 
; clm 040712 Add .text directives to each individual function to enable dead stripping
;            Put the FPLIB entry point first for dead stripping

