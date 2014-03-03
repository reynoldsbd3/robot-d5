;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _d_sqrt.s

;===============================================================================



	#include "ARMsfpe.pre.asm"
		IMPORT errno       
#define EDOM  33	;if value for EDOM defined in cerrno is changed, this 
					;must also be changed.					/*- mm  040625 -*/

;===============================================================================


	EXPORT _dsqrt



	#ifdef VFP_AND_NOT_IEEE_STD

	EXPORT _vfp_support_dsqrt

#endif


;===============================================================================

; FUNCTION: _dsqrt

; DESCRIPTION: IEEE754 square root operation. 

; INPUT:

;       dxh/dxl = double

; OUTPUT:

;       dxh/dxl = double

; SPECIAL CASES:

;       sqrt(qNaN) = qNaN

;       sqrt(sNaN) = qNaN and signal invalid

;       sqrt(+/-0) = +/-0

;       sqrt(+inf) = +inf

;       sqrt(-inf) = qNaN and signal invalid

;       sqrt(-val) = qNaN and signal invalid



	MACRO
	DSQRT_SAVE

        stmfd   sp!,{r4-r6,sreg07}

	MEND



#if defined(THUMB)
; && !defined(ARM5)

	MACRO
	DSQRT_RETURN $cc

        ldm$cc.fd sp!,{r4-r6,sreg07}

        bx$cc  lr

	MEND

#else

	MACRO
	DSQRT_RETURN $cc

        ldm$cc.fd sp!,{r4-r6,pc}

	MEND

#endif  



	AREA |.text|,CODE,READONLY

#ifdef VFP

_d_sqrt

        fmdhr   d0, dxh

        fmdlr   d0, dxl

        fsqrtd  d0, d0

        fmrdh   dxh, d0

        fmrdl   dxl, d0

  #ifdef THUMB        

        bx      lr

  #else        

        mov     pc,lr

  #endif        



_vfp_support_dsqrt

#else

_dsqrt

#endif



	#ifdef NOT_VFP_OR_NOT_IEEE_STD



        DSQRT_SAVE

        ldr     sreg02,=0x7ff00000

        cmp     dxh,sreg02

        bhs.w     dsqrt_special           ; Check for Negative / NaN / Inf



        movs    sreg12,dxh,LSR #20      ; sreg12 = exponent

        beq     dsqrt_zero              ; 0 / denorm case?

        bic     dxh,dxh,sreg02          ; clear exponent bits in dxh

        orr     dxh,dxh,#0x00100000     ; add implicit 1 bit    

dsqrt_denorm_return



        movs    sreg12,sreg12,ASR #1    ; result exponent = input exponent / 2

        bcs     dsqrt_odd_exponent      ; biased input exponent should be odd

        sub     sreg12,sreg12,#1        ; adjust exponent if input exp is even

        movs    dxl,dxl,LSL #1          ; dxh:dxl <<= 1

        adc     dxh,dxh,dxh

dsqrt_odd_exponent

; Generate SQRT bit-by-bit



; Compute the high mantissa word:

;       q0 = HIGH(sqrt(i0:i1))

;

;       i0:i1 <<= 1;

;       q0 = 0;

;       s0 = 0;

;       for (r = 0x00200000; r; r >>= 1)

;         {

;         t0 = s0 + r;

;         if ( t0 <= i0 )

;           {

;           s0 = t0 + r;

;           i0 -= t0;

;           q0 += r;

;           }

;         i0:i1 <<= 1;

;         }

;         



        movs    sreg03,dxl,LSL #1       ; i0:i1 <<= 1 (compute and extra bit for rounding)

        adc     dxh,dxh,dxh

        mov     sreg02,#0               ; q0 = 0

        mov     sreg04,#0               ; s0 = 0

        mov     sreg07,#0x00200000          ; r = 0x00200000

dsqrt_loop1

        add     sreg06,sreg04,sreg07        ; t0 = s0 + r

        cmp     sreg06,dxh              ; if ( t0 <= i0 )

        addle   sreg04,sreg06,sreg07        ;   s0 = t0 + r

        suble   dxh,dxh,sreg06          ;   i0 -= t0;

        addle   sreg02,sreg02,sreg07        ;   q0 += r

        movs    sreg03,sreg03,LSL #1    ; i0:i1 <<= 1

        adc     dxh,dxh,dxh

        movs    sreg07,sreg07,LSR #1            ; r >>= 1

        bne     dsqrt_loop1



; Compute the low mantissa word:

;       q1 = LOW(sqrt(i0:i1))

;

;       q1 = 0;

;       s1 = 0;

;       for (r = 0x80000000; r; r >>= 1)

;         {

;         t0:t1 = s0:s1 + r; 

;         if ( t0:t1  <= i0:i1 )

;           {

;           s0:s1 = t0:t1 + r;

;           i0:i1 -= t0:t1;

;           q1 += r;

;           }

;         i0:i1 <<= 1;

;         }

        mov     dxl,#0                  ; q1 = 0

        mov     sreg05,#0               ; s1 = 0



; First iteration: r = 0x80000000, t = s0:8000 0000

        cmp     dxh,sreg04              ; if ( t0:t1  <= i0:i1)

        cmpeq   sreg03,#0x80000000

        blo     dsqrt_l1

        subs    sreg03,sreg03,#0x80000000  ;   i0:i1 -= t0:t1

        sbc     dxh,dxh,sreg04

        add     sreg04,sreg04,#1        ;   s0:s1 = t0:t1 + r   

        mov     dxl,#0x80000000         ;   q1 += r

dsqrt_l1

        movs    sreg03,sreg03,LSL #1    ; i0:i1 <<= 1

        adc     dxh,dxh,dxh



        mov     sreg07,#0x40000000          ; r = 0x40000000 and lower

dsqrt_loop2

        add     sreg06,sreg05,sreg07        ; t0:t1 = s0:s1 + r

        cmp     sreg04,dxh              ; if ( t0:t1  <= i0:i1 )

        cmpeq   sreg06,sreg03

        bhi     dsqrt_l2

        add     sreg05,sreg06,sreg07        ;   s0:s1 = t0:t1 + r

        subs    sreg03,sreg03,sreg06    ;   i0:i1 -= t0:t1

        sbc     dxh,dxh,sreg04

        add     dxl,dxl,sreg07              ;   q1 += r

dsqrt_l2

        movs    sreg03,sreg03,LSL #1    ; i0:i1 <<= 1

        adc     dxh,dxh,dxh

        movs    sreg07,sreg07,LSR #1            ; r >>= 1

        bne     dsqrt_loop2



; Rounding      

;   i0:i1 - remainder (treat as sticky for rounding)

;   q0:q1 = sqrt (fractional part, 54 bits including lead 1 bit)

;           round to nearest according to the last bit of q1



        orrs    dxh,dxh,sreg03          ; any remainder? round

        biceq   dxl,dxl,#1              ; clear sticky bit if no rounding

  #ifdef IEEE_FULL

        beq     dsqrt_round

        LFPState sreg06, sreg05

        SetInexact sreg06

        StFPState sreg06,sreg05 

        tst     sreg06,CHOP_OR_DOWN_BIT_MASK

        bicne   dxl,dxl,#1              ; clear sticky bit if rounding down

        bne     dsqrt_round

        tst     sreg06,UP_OR_NEAR_BIT_MASK

        orrne   dxl,dxl,#1              ; set sticky bit if rounding up

dsqrt_round

  #endif

        movs    dxh,sreg02,LSR #1       ; shift out the rounding bit

        movs    dxl,dxl,RRX

        adcs    dxl,dxl,#0              ; bump result if the bit shifted out is 1

        adc     dxh,dxh,#0



        add     dxh,dxh,#0x20000000     ; put exponent (r12 + 0x1ff)

        sub     dxh,dxh,#0x00100000

        add     dxh,dxh,sreg12,LSL #20

        DSQRT_RETURN



; normalize the denorm

dsqrt_zero

  #ifdef IEEE_STD

        mov     dxh,#0

        mov     dxl,#0

        DSQRT_RETURN

  #else

        cmp     dxh,#0 

        bne     dsqrt_tiny_high         ; case: high denorm word <> 0

        cmp     dxl,#0

        DSQRT_RETURN eq                 ; sqrt(0) = 0



        mvn     sreg12,#19              ; exponent = 1 - (21 + shift count)

        Normalize_a5 dxl,sreg12,sreg05

        mov     dxh,dxl,LSR #11 

        mov     dxl,dxl,LSL #21

        b       dsqrt_denorm_return



dsqrt_tiny_high

        Normalize_num dxh, sreg02, #43  ; normalize high word

        mov     dxh,dxh,LSR #11

;;;        orr     dxh,dxh,dxl,LSR sreg02  ; align number to 001f ffff...
	mov	sreg12,dxl,LSR sreg02
	orr	dxh,dxh,sreg12

        rsb     sreg02,sreg02,#32       ; sreg02 = shift count - 11

        mov     dxl,dxl,LSL sreg02



        rsb     sreg12,sreg02,#1        ; exponent = 1 - (shift count - 11)

        b       dsqrt_denorm_return

  #endif  



; negative, NaN, Inf    

dsqrt_special

        tst     dxh,#0x80000000         ; positive special case?

        beq     dsqrt_nan_inf



        bics    sreg03,dxh,#0x80000000  ; sreg03 = abs(dxh)

  #ifdef IEEE_STD

        movs    sreg12,sreg03,LSR #20   ; -denorm or -zero ?

        beq     dsqrt_zero              ; return zero

  #else

        cmpeq   dxl,#0

        DSQRT_RETURN eq                 ; sqrt(-0) = -0

  #endif  



	#ifdef IEEE_FULL_OR_FIXED

        cmp     sreg03,sreg02           ; abs(dxh) < 7FF0 0000 ?

        blo     dsqrt_invalid           ; sqrt(-val) = qNan -> invalid

        orrs    sreg02,dxl,dxh,LSL #12 

        bne     dsqrt_nan               ; sqrt(NaN) = qNaN

        b       dsqrt_invalid           ; sqrt(-inf) = qNaN -> invalid

  #else

        b       dsqrt_nan

  #endif



dsqrt_nan_inf

        orrs    sreg02,dxl,dxh,LSL #12  ; sqrt(+inf) = +inf

        DSQRT_RETURN eq

dsqrt_nan

	#ifdef IEEE_FULL_OR_FIXED

        tst     dxh,#0x00080000         ; sqrt(qNan) = qNaN

        DSQRT_RETURN ne

dsqrt_invalid

  #endif  

        ldr     sreg02,=0x7ff80000

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg03, sreg12

        SetInvalidE sreg03

        StFPState sreg03,sreg12 

  #endif

        orr     dxh,dxh,sreg02          ; create quiet NaN

        ldr     sreg03,=errno			; set errno to EDOM if     	 /*- mm 040625 -*/
        
        ldr     sreg04,=EDOM			; argument our of range		 /*- mm 040625 -*/
        
        str     sreg04,[sreg03]	

        DSQRT_RETURN



	#ifdef IEEE_FULL_OR_FIXED

	IMPORT __fp_handler_64_to_64



; invalid exception handler

__wrap_invalid_handler

        ldr     r2, =(FE_EX_INVALID :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_SQRT)

        bl      __fp_handler_64_to_64

        DSQRT_RETURN

  #endif



  #ifdef IEEE_FULL

; inexact exception wrapper

__wrap_inexact_handler

        movs    dxh,sreg02,LSR #1       ; shift out the rounding bit

        movs    dxl,dxl,RRX

        add     dxh,dxh,#0x20000000     ; put exponent (r12 + 0x1ff)

        sub     dxh,dxh,#0x00100000

        add     dxh,dxh,sreg12,LSL #20



        ldr     r2, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_SQRT)

        bl      __fp_handler_64_to_64

        DSQRT_RETURN

  #endif



#endif
	END

/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */
