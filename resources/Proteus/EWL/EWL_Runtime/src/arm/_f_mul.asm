;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _f_mul.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



	EXPORT _f_mul

	EXPORT _fmul                   ; FPLIB for _f_mul



	EXPORT __f_result_INF

	EXPORT __f_result_x_NaN

	EXPORT __f_result_invalid



	#ifdef VFP_AND_NOT_IEEE_STD

	EXPORT _vfp_support_fmul

#endif



#ifdef THUMB

	MACRO
	Retm $cc

        bx$cc  lr

	MEND

#else

	MACRO
	Retm $cc

        mov$cc pc,lr

	MEND

#endif          



	#ifdef IEEE_FULL_OR_FIXED

	MACRO
	Return

        add     sp, sp, #8

        Retm

	MEND

	MACRO
	Reteq

        addeq   sp, sp, #8

        Retm    eq

	MEND

	MACRO
	Retne

        addne   sp, sp, #8

        Retm    ne

	MEND

	MACRO
	Retcc

        addcc   sp, sp, #8

        Retm    cc

	MEND

	MACRO
	Prolog

        stmfd   sp!, {fx, fy}

	MEND

	MACRO
	Clrstk

        add     sp, sp, #8

	MEND

#else

	MACRO
	Return

        Retm

	MEND

	MACRO
	Reteq

        Retm    eq

	MEND

	MACRO
	Retne

        Retm    ne

	MEND

	MACRO
	Retcc

        Retm    cc

	MEND

	MACRO
	Prolog

	MEND

#endif                                   
; #if defined(IEEE_FULL) || defined(IEEE_FIXED)



;===============================================================================

; FUNCTION: float _f_mul (float, float)

; DESCRIPTION: Float multiply.

; INPUT: 

; fx=x,

; fy=y, floats

; OUTPUT: 

; fr = result

	AREA |.text|,CODE,READONLY

#ifdef VFP

_fmul

_f_mul


        fmsr    s0, r0

        fmsr    s1, r1

        fmuls   s0, s0, s1

        fmrs    r0, s0

        Retm


_vfp_support_fmul

#else

_fmul			;; FPLIB Entry Point

_f_mul

                                 
#endif



	#ifdef NOT_VFP_OR_NOT_IEEE_STD



; figure out sign

        Prolog

        eor     sreg02, fx, fy

        and     sreg02, sreg02, #0x80000000  ; result sign in MSB of sreg02

; unpack fx in sreg03(exp) and fx(significand)

        mov     sreg12,#0xff

        ands    sreg03,sreg12,fx,LSR #23  ; sreg03 has exponent 

        mov     fx, fx, LSL #8          ; fx has significand & low bit of exp

        cmpne   sreg03, #0xff           ; test for huge exponent

        beq     fmul_x_special_case     ; special case: tiny or huge



        orr     fx, fx, #0x80000000     ; set lead bit    

; unpack fy in sreg12(exp) and fy(significand)  

        ands    sreg12,sreg12,fy,LSR #23  ; sreg12 has exponent

        mov     fy, fy, LSL #8          ; fy has significand & low bit of exp

        cmpne   sreg12, #0xff           ; test for huge exponent

        beq     fmul_y_special_case     ; special case: tiny or huge



        orr     fy, fy, #0x80000000     ; set lead bit  

; now sreg03 and sreg12 contain the sign and exponents of fx and fy

; sreg02 has the sign of the operation

fmul_start

        add     sreg12, sreg03, sreg12  ; sum of exponents

; now sreg03 is free

  #ifndef ARM_xM  

        umull   fy, sreg03, fx, fy      ; compute product

  #else

        stmfd   sp!, {sreg04, sreg05, sreg06}  ; save registers

        mov     sreg04, fx, LSR #16     ; sreg04 = xh

        sub     fx, fx, sreg04, LSL #16  ; fx = xl

        mov     sreg05, fy, LSR #16     ; sreg05 = yh

        sub     fy, fy, sreg05, LSL #16  ; fy = yl  

        mul     sreg03, sreg04, sreg05  ; sreg03 = xh * yh

        mul     sreg06, sreg04, fy      ; sreg06 = xh * yl

        mul     sreg04, fx, sreg05      ; sreg04 = xl * yh

        adds    sreg04, sreg04, sreg06

        addcs   sreg03, sreg03, #0x10000  ; propagate carry

        mul     fy, fx, fy              ; fy = xl * yl  

        adds    fy, fy, sreg04, LSL #16

        adc     sreg03, sreg03, sreg04, LSR #16  ; result is in sreg03 - fy

        ldmfd   sp!, {sreg04, sreg05, sreg06} 

  #endif  

  #ifdef IEEE_FULL

        cmp     fy, #0

        orrne   sreg03, sreg03, #1      ; sticky bit from lower part

  #endif

        movs    fx, sreg03              ; result in fx

        addpl   fx, fx, fx

        subpl   sreg12, sreg12, #1      ; normalize if necessarry

; now fx has the significand

; sreg12 has the unbiased exponent 

        subs    sreg12, sreg12, #127    ; r12 = exponent - 1

        bmi.w     fmul_negative_or_zero_exp  ; special case  

        cmp     sreg12, #0xfe

        bge.w     __f_overflow            ; overflow case

fmul_common

; normal case: pack 

; sreg12 has the exponent

; fx has the significand

; sreg02 has the sign

        ands    sreg03, fx, #0xff       ; round bits

        orr     fx, sreg02, fx, LSR #8  ; align 23 sig bits and stuff sign

        add     fx, fx, sreg12, LSL #23  ; exp := exp - 1 + lead bit

  #ifdef IEEE_FULL

        Reteq                           ; no rounding: done

; rounding must be done here

; here we have the inexact result packed in fx

; sreg03 contains all the rounding bits

; sreg02 has the sign

; must round and set the inexact bit in IEEE_FULL mode

; cases for rounding :

; ZERO - do nothing

; DOWN - check sign and subtract 1 if negative

; UP - check sign and add 1 if positive

; NEAREST - the normal case

        LFPState sreg01, sreg12

        SetInexact sreg01               ; set inexact

        StFPState sreg01,sreg12         ; store FPSCR  

        tst     sreg01, CHOP_OR_DOWN_BIT_MASK

        bne     chop_or_down_round

        tst     sreg01, UP_OR_NEAR_BIT_MASK 

        bne     up_round                ; rounding is up

  #endif

; round to nearest  

        tst     sreg03, #0x0080         ; done if round bit is clear

        Reteq

  #ifdef IEEE_FULL        

        ands    sreg03, sreg03, #0x7f   ; round up if sticky bits are non-0

  #else

        orrs    sreg01, sreg01, sreg03, LSL #25

  #endif  

        andeqs  sreg03, fx, #0x1        ; in half-way case, round up if not already even

  #if !defined(IEEE_FULL) && !defined(IEEE_FIXED)

; mode is NO_FENV or STD; just round and return

        addne   fx, fx, #1

        Return

  #else   

; IEEE_FULL or IEEE_FIXED; must check overflow

        Reteq                           ; done if nothing must be added

fmul_add

        add     fx, fx, #1

        mov     sreg03, fx, LSL #1      ; look for overflow

        cmn     sreg03, #0x01000000     ; compute sreg03 + 0x01000000

        Retcc                           ; exit if overflow did not occur  

; in IEEE_FULL mode arrive here with r1 = __SoftFPSCR__ and r12 = &__SoftFPSCR_

    #ifdef IEEE_FIXED    

        LFPState sreg01, sreg12

    #endif

        SetOverflow sreg01              ; signal overflow

        StFPState sreg01,sreg12         ; store FPSCR

        Return

    #ifdef IEEE_FULL

up_round

        cmp     sreg02, #0              ; sreg02 holds the sign

        beq     fmul_add                ; add and check overflow only if positive

        Return



chop_or_down_round

        tst     sreg01, ZERO_OR_DOWN_BIT_MASK 

        Retne                           ; done if rounding is to zero

        cmp     sreg02, #0

        bne     fmul_add                ; add and check overflow only if negative

        Return

    #endif                                 
; #ifdef IEEE_FULL

  #endif                                   
; #if #defined(IEEE_FULL) && !defined(IEEE_FIXED) 



; ============================================================================

; The code for special cases begins here

; ============================================================================



; arrive here if x is either tiny or huge

; fx has x significand and lower bit of exp, sreg03 has x exponent

fmul_x_special_case

        cmp     sreg03, #0

        #ifdef IEEE_STD

          beq.w     __f_result_zero

        #else

          beq.w     fmul_x_tiny             ; x is tiny

        #endif

; otherwise fall through fmul_x_huge



; arrive here if x exp is 0xff

; sreg02 has the sign

; fx has significand of x and lower bit of exp

fmul_x_huge

        movs    fx, fx, LSL #1          ; clear high bit

        bne.w     __f_result_x_NaN        ; check x and return a QNaN

; x is INF 

; must unpack y

; unpack fy in sreg12(exp) and fy(significand)  

        mov     sreg12, fy, LSR #23     ; sreg12 has exponent and sign

        mov     fy, fy, LSL #9          ; fy has significand  

        ands    sreg12, sreg12, #0xff   ; clear sign bit

        beq     fmul_y_checkzero        ; 0 or denorm

        cmp     sreg12, #0xff 

        blt.w     __f_result_INF          ; INF * norm = INF

; if we arrive here x is INF, y is either INF, NaN

        cmp     fy, #0

        beq.w     __f_result_INF          ; INF * INF = INF

        b       fmul_y_NaN              ; INF * NaN = QNaN

fmul_y_checkzero

; y is either zero or denorm

        cmp     fy, #0

        beq.w     __f_result_invalid      ; INF * zero = QNaN and set invalid

        b       __f_result_INF          ; INF * denorm = INF



; arrive here if y is either tiny or huge

; fx has x significand and lower bit of exp, sreg03 has x exponent 

; fy has y significand and lower bit of exp, sreg12 has y exponent

fmul_y_special_case

        cmp     sreg12, #0

        #ifdef IEEE_STD

          beq     __f_result_zero

        #else

          beq     fmul_y_tiny             ; y is tiny

        #endif

; otherwise fall through fmul_y_huge



; arrive here if y exp is 0xff AND x is a normal number

; sreg02 has the sign

; sreg03 has x exp, fx has x significand and lead bit

; sreg12 has y exp, fy has y significand and lower bit of exp

fmul_y_huge

        movs    fy, fy, LSL #1          ; clear high bit

        bne.w     fmul_y_NaN              ; check y and return a QNaN

; y is INF, x is a normal number, so return INF

        b       __f_result_INF



; arrive here if x exp is 0x00 

; sreg02 has the sign

; sreg03 has x exp, fx has x significand and lower bit of exp

fmul_x_tiny

        movs    fx, fx, LSL #1          ; clear high bit

        beq     fmul_x_zero             ; x is zero

; x is a denorm

        mov     fx, fx, LSR #1          ; place for lead bit

        Normalize_num fx, sreg03, #1    ; normalize number

; unpack fy in sreg12(exp) and fy(significand)  

        mov     sreg12, fy, LSR #23     ; sreg12 has exponent and sign

        mov     fy, fy, LSL #8          ; fy has significand & low bit of exp

        ands    sreg12, sreg12, #0xff   ; clear sign bit

  #ifdef IEEE_STD

        beq     __f_result_zero         ; zero if denorm

  #else

        beq     fmul_y_tiny

        cmp     sreg12, #0xff           ; huge exponent

        beq     fmul_y_huge

  #endif

        orr     fy, fy, #0x80000000     ; set lead bit  

        b       fmul_start              ; multiply denormalized numbers

fmul_x_zero

; unpack fy in sreg12(exp) and fy(significand)  

        mov     sreg12, fy, LSR #23     ; sreg12 has exponent and sign

        mov     fy, fy, LSL #9          ; fy has significand 

        ands    sreg12, sreg12, #0xff   ; clear sign bit

        beq.w     __f_result_zero         ; 0 * denorm = 0 * norm = 0

        cmp     sreg12, #0xff

        blt.w     __f_result_zero         ; 0 * norm = 0

        cmp     fy, #0

        beq.w     __f_result_invalid      ; O * INF = 0 and set invalid

        b       fmul_y_NaN              ; y is a NaN



; arrive here if y exp is 0x00 AND x is a normal number or a denorm

; sreg02 has the sign

; sreg03 has x exp, fx has x significand and lead bit

; sreg12 has y exp, fy has y significand and lower bit of exp

fmul_y_tiny

        movs    fy, fy, LSL #1          ; clear high bit

        beq.w     __f_result_zero         ; 0 * normal = 0

; y is a denorm, so normalize and compute the result

        mov     fy, fy, LSR #1          ; place for lead bit

        Normalize_num fy, sreg12, #1    ; normalize number

        b       fmul_start



; arrive here if the exponent is zero or less after operations

; must check for special cases  

fmul_negative_or_zero_exp

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg03, sreg03

        tst     sreg03, FE_IEEE_MASK_UNDERFLOW

        bne.w     __wrap_underflow_handler

  #endif

        cmn     sreg12, #24

        beq     fmul_very_tiny_result   ; case: very tiny

        bmi.w     __f_underflow           ; case: underflow 

; exponent is -22...0

  #ifdef IEEE_STD

        mov     fx, sreg02              ; return signed 0 in STD mode 

        Return

  #else      

; fx has result (integer)

; sreg12 has exponent

; sreg02 has sign    

    #ifndef IEEE_FULL

        cmp     sreg01, #0

        orrne   fx, fx, #1

    #endif

        mov     sreg03, fx              ; keep significand for stickies

        mov     fx, fx, LSR #8          ; keep only high part of result

        rsb     sreg12, sreg12, #0      ; compute 1-exp in sreg03

;;;        orr     fx, sreg02, fx, LSR sreg12  ; shift right and stuff sign
	mov	sreg01, fx, LSR sreg12
	orr	fx, sreg02, sreg01
	

        rsb     sreg12, sreg12, #24     ; compute shift for round bits

        movs    sreg01, sreg03, LSL sreg12  ; round and sticky bits from shift

        Reteq

; rounding must be done here WITHOUT overflow check

; here we have the inexact result packed in fx

; sreg01 contains all the rounding bits

; sreg02 has the sign

; cases for rounding :

; ZERO - do nothing

; DOWN - check sign and substract 1 if negative

; UP - check sign and add 1 if positive

; NEAREST - the normal case

	#ifdef IEEE_FULL_OR_FIXED

; @@@ Modified: added LFPState

        LFPState sreg03, sreg12

        SetUnderflow sreg03             ; set underflow

        StFPState sreg03,sreg12         ; store FPSCR  

    #endif

    #ifdef IEEE_FULL

        tst     sreg03, CHOP_OR_DOWN_BIT_MASK

        bne     chop_or_down_round_denorm

        tst     sreg03, UP_OR_NEAR_BIT_MASK 

        bne     up_round_denorm         ; rounding is up

    #endif

; round to nearest

        tst     sreg01, #0x80000000     ; done if round bit is clear

        Reteq

        movs    sreg01, sreg01, LSL #1  ; round up if sticky bits are non-0

        andeqs  sreg01, fx, #0x1        ; in half-way case, round up if not already even

        addne   fx, fx, #1

        Return

    #ifdef IEEE_FULL

up_round_denorm

        cmp     sreg02, #0              ; sreg02 holds the sign

        addeq   fx, fx, #1              ; add 1 only if positive

        Return



chop_or_down_round_denorm

        tst     sreg03, ZERO_OR_DOWN_BIT_MASK 

        addeq   fx, fx, fx, LSR #31     ; just add sign for rounding down

        Return

    #endif                                 
; #ifdef IEEE_FULL            

  #endif                                   
; #ifdef IEEE_STD   



; the result is very tiny

; prepare stickies and fall through __f_very_tiny_result  

fmul_very_tiny_result

        mov     fx, fx, LSL #1          ; clear lead bit

        b       __f_very_tiny_result



; ============================================================================

; The next functions will also be used in other parts of the code

; ============================================================================



; arrive here when result is INF

; return signed INF

; sreg02 holds the sign

__f_result_INF

        mov     fx, #0xff000000         ; prepare code for INF

        orr     fx, sreg02, fx, LSR #1  ; make signed INF

        Return



__f_result_x_NaN

; x is a NaN

; must check if y is also a NaN and set invalid if any of them is signaling  

; fx has significand

; only happens in IEEE_FULL and IEEE_FIXED modes

	#ifdef IEEE_FULL_OR_FIXED

        tst     fx, #0x80000000

        beq     __f_result_invalid      ; fx has a signaling NaN

; x is a quiet NaN, but y could be a signaling NaN

; unpack fy in sreg12(exp) and fy(significand)  

        mov     sreg12, fy, LSR #23     ; sreg12 has exponent and sign

        mov     fy, fy, LSL #9          ; fy has significand 

        ands    sreg12, sreg12, #0xff   ; clear sign bit

        beq     __f_result_make_NaN     ; make NaN for 0

        cmp     sreg12, #0xff

        blt     __f_result_make_NaN     ; make NaN for normals

        cmp     fy, #0

        beq     __f_result_make_NaN     ; make NaN for inf

  #endif

fmul_y_NaN

	#ifdef IEEE_FULL_OR_FIXED

; y is a NaN; must check signalling

        tst     fy, #0x80000000

        beq     __f_result_invalid      ; set invalid

  #endif

        MakeFNaN

        Return



; set the invalid flag  

__f_result_invalid

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg03, sreg02

        SetInvalidE sreg03              ; set invalid

        StFPState sreg03,sreg02         ; store FPSCR  

  #endif

; fall through make NaN



; arrive here when a NaN must be returned without any checks    

__f_result_make_NaN

        MakeFNaN

        Return



; arrive here if an overflow occured

; must return INF; sign is in sreg02, exp-1 in sreg12

__f_overflow

	#ifdef IEEE_FULL_OR_FIXED

; @@@ Modified: added LFPState     

        LFPState sreg03, sreg01

        SetOverflowL sreg03, __wrap_overflow_handler_pack

        StFPState sreg03,sreg01         ; set overflow 

  #endif

        mov     fx, #0xff000000         ; prepare code for INF

        orr     fx, sreg02, fx, LSR #1  ; make signed INF

  #ifndef IEEE_FULL

        Return                          ; done

  #else

; cases for rounding:

; round to nearest: do nothing

; round to zero (chop) : always substract 1

; round up: substract 1 if the number is negative

; round down: substract 1 if the number is positive

        tst     sreg03, CHOP_OR_DOWN_BIT_MASK  ; take into account the rounding mode

        bne     oflow_chopdown

        tst     sreg03, UP_OR_NEAR_BIT_MASK 

        subne   fx, fx, sreg02, LSR #31  ; sub 1 for negative numbers if rounding up

        Return



oflow_chopdown

        sub     fx, fx, #1              ; presume that 1 is always substracted

        tst     sreg03, ZERO_OR_DOWN_BIT_MASK

        addeq   fx, fx, sreg02, LSR #31  ; if rounding down, add the sign 

        Return

  #endif                                   
; #ifndef IEEE_FULL    



; special case: exponent is -23

; sreg02 holds the sign

; fx has the stickies  

__f_very_tiny_result

  #ifdef IEEE_STD

        mov     fx, sreg02              ; load signed zero

        Return                          ; done for IEEE_STD

  #else

        movs    sreg01, fx              ; stickies in sreg01

        mov     fx, sreg02              ; load signed zero

	#ifdef IEEE_FULL_OR_FIXED

; @@@ Modified: added LFPState    

        LFPState sreg03, sreg12

        SetUnderflow sreg03             ; set underflow

        StFPState sreg03,sreg12         ; store FPSCR   

        cmp     sreg01, #0 

    #endif

    #ifdef IEEE_FULL

        tst     sreg03, CHOP_OR_DOWN_BIT_MASK

        bne     __f_chop_or_down_round_denorm

        tst     sreg03, UP_OR_NEAR_BIT_MASK 

        bne     __f_up_round_denorm     ; rounding is up

        cmp     sreg01, #0

    #endif

        addne   fx, fx, #1              ; add 1 if stickies are present

        Return

  #endif                                   
; #ifdef IEEE_STD



; an underflow occured; must round in IEEE_FULL mode

; sreg02 already contains signed zero

__f_underflow

        mov     fx, sreg02              ; load signed zero

	#ifdef IEEE_FULL_OR_FIXED

; @@@ Modified: added LFPState      

        LFPState sreg03, sreg12

        SetUnderflow sreg03

        StFPState sreg03,sreg12         ; set underflow    

  #endif

  #ifndef IEEE_FULL

        Return

  #else

; cases for rounding:

; round no nearest: do nothing

; round to zero (chop): do nothing

; round up: add 1 if number is positive

; round down: add 1 if number is negative

        tst     sreg03, CHOP_OR_DOWN_BIT_MASK

        bne     __f_chop_or_down_round_denorm

        tst     sreg03, UP_OR_NEAR_BIT_MASK

        Reteq                           ; round no nearest: do nothing

        cmp     fx, #0

        addeq   fx, fx, #1              ; up: add 1 if positive

        Return

  #endif                                   
; #ifndef IEEE_FULL  



__f_result_zero

        mov     fx, sreg02              ; signed zero

        Return



  #ifdef IEEE_FULL

__f_up_round_denorm

        cmp     sreg02, #0              ; sreg02 holds the sign

        addeq   fx, fx, #1              ; add 1 only if positive

        Return



__f_chop_or_down_round_denorm

        tst     sreg03, ZERO_OR_DOWN_BIT_MASK 

        addeq   fx, fx, fx, LSR #31     ; just add sign for rounding down

        Return

  #endif                                   
; #ifdef IEEE_FULL              



	#ifdef IEEE_FULL_OR_FIXED

	IMPORT __fp_handler_32_to_32

	IMPORT __fp_handler_binary_32_to_32



; overflow exception handler

__wrap_overflow_handler

        Clrstk

        sub     fx, fx, #(192 << 23)    ; coerce exponent

        stmfd   sp!, {lr} 

        ldr     r1, =(FE_EX_OVERFLOW :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_MUL)

        bl      __fp_handler_32_to_32

        Return_lr



; overflow exception handler with pack

__wrap_overflow_handler_pack

        Clrstk

        sub     sreg12, sreg12, #192    ; coerce exponent

        orr     fx, sreg02, fx, LSR #8  ; align 23 sig bits and stuff sign

        add     fx, fx, sreg12, LSL #23  ; exp := exp - 1 + lead bit

        stmfd   sp!, {lr} 

        ldr     r1, =(FE_EX_OVERFLOW :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_MUL)

        bl      __fp_handler_32_to_32

        Return_lr



; underflow handler

; fx holds the significand, exp is in sreg12, sign in sreg02

__wrap_underflow_handler

        Clrstk

        add     sreg12, sreg12, #192    ; coerce exponent

        orr     fx, sreg02, fx, LSR #8  ; align 23 sig bits and stuff sign

        add     fx, fx, sreg12, LSL #23  ; exp := exp - 1 + lead bit

        stmfd   sp!, {lr}

        ldr     r1, =(FE_EX_UNDERFLOW :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_MUL)

        bl      __fp_handler_32_to_32 

        Return_lr



;invalid exception handler

__wrap_invalid_handler

        ldmfd   sp!, {fx, fy}

        stmfd   sp!, {lr}

        ldr     r2, =(FE_EX_INVALID :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_MUL)

        bl      __fp_handler_binary_32_to_32 

        Return_lr



  #endif



  #ifdef IEEE_FULL



; inexact exception wrapper

__wrap_inexact_handler

        Clrstk

        stmfd   sp!, {lr}

        ldr     r1, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_MUL)

        bl      __fp_handler_32_to_32

        Return_lr

  #endif



#endif
	END

/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */
