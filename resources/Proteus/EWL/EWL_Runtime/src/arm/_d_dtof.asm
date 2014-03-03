;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:23 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _d_dtof.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



;===============================================================================



	EXPORT _f_qtof

	EXPORT _d_dtof

	EXPORT _d2f                    ; FPLIB for _d_dtof



	EXPORT __f_result_zero

	EXPORT __f_underflow

	EXPORT __f_very_tiny_result



	#ifdef VFP_AND_NOT_IEEE_STD

	EXPORT _vfp_support_d2f

#endif



#ifdef THUMB    

	MACRO
	Return

        bx      lr

	MEND

	MACRO
	Reteq

        bxeq    lr

	MEND

	MACRO
	Retne

        bxne    lr

	MEND

	MACRO
	Retcc

        bxcc    lr

	MEND

#else

	MACRO
	Return

        mov     pc,lr

	MEND

	MACRO
	Reteq

        moveq   pc,lr

	MEND

	MACRO
	Retne

        movne   pc,lr

	MEND

	MACRO
	Retcc

        movcc   pc,lr

	MEND

#endif



;===============================================================================

; FUNCTION: _f_qtof

; DESCRIPTION: Convert a long double to float.

; INPUT:

;       dxh/dxl = long double

; OUTPUT:

;       fx = float

;

;===============================================================================

; FUNCTION: _d_dtof

; DESCRIPTION: Convert a double to float.

; INPUT:

;       dxh/dxl = double

; OUTPUT:

;       fx = float

	AREA |.text|,CODE,READONLY
	
#ifdef VFP

_d2f
_d_dtof
_f_qtof

        fmdhr   d0, dxh

        fmdlr   d0, dxl

        fcvtsd  s0, d0

        fmrs    r0, s0

        Return


	END
	AREA |.text|,CODE,READONLY

_vfp_support_d2f

#else

_d2f
_d_dtof
_f_qtof

#endif



	#ifdef NOT_VFP_OR_NOT_IEEE_STD



        and     sreg02, dxh, #0x80000000  ; sign in sreg02  

        mov     sreg12, dxh, LSR #20    ; exponent in sreg12

; sign is in sreg02

; exponent and sign are in sreg12

        bics    sreg12, sreg12, #0x0800  ; isolate exp

        beq     dtof_x_tiny

        mov     sreg03, sreg12, LSL #21  ; load exponent in high part of fx

        cmn     sreg03, #0x00200000     ; compute fx + 0x00200000

        bcs     dtof_x_huge             ; if carry is set, x is huge

; normal case 

; unbiased exponent is in sreg12

; number is in dxh/dxl

; sign is in sreg02

        subs    sreg12, sreg12, #(1023-127)  ; bias exponent

        bls     dtof_negative_or_zero_exp  ; special case

        cmp     sreg12, #0xff

        bge.w     __f_overflow            ; overflow 

        mov     dxh, dxh, LSL #12       ; keep only high part of significand

        orr     sreg03, sreg02, dxh, LSR #9  ; sreg03 now has sign and a part of significand

        orr     sreg03, sreg03, dxl, LSR #29  ; other bits of significand

        movs    sreg01, dxl, LSL #3     ; round bits  

        orr     fx, sreg03, sreg12, LSL #23  ; stuff exponent

        Reteq                           ; exit if round bits are zero

; rounding must be done here

; here we have the inexact result packed in fx

; sreg01 contains all the rounding bits

; sreg02 has the sign

; must round and set the inexact bit in IEEE_FULL mode

; cases for rounding :

; ZERO - do nothing

; DOWN - check sign and substract 1 if negative

; UP - check sign and add 1 if positive

; NEAREST - the normal case

  #ifdef IEEE_FULL

; @@@ Modified: added LFPState  

        LFPState sreg03, sreg12

        SetInexact sreg03               ; set inexact

        StFPState sreg03,sreg12         ; store FPSCR  

        tst     sreg03, CHOP_OR_DOWN_BIT_MASK

        bne     chop_or_down_round

        tst     sreg03, UP_OR_NEAR_BIT_MASK 

        bne     up_round                ; rounding is up

  #endif

; round to nearest

        tst     sreg01, #0x80000000     ; done if round bit is clear

        Reteq

        movs    sreg01, sreg01, LSL #1  ; round up if sticky bits are non-0

        andeqs  sreg01, fx, #0x1        ; in half-way case, round up if not already even

  #if !defined(IEEE_FULL) && !defined(IEEE_FIXED)

; mode is NO_FENV or STD; just round and return

        addne   fx, fx, #1

        Return

  #else   

; IEEE_FULL or IEEE_FIXED; must check overflow

        Reteq                           ; done if nothing must be added

dtof_add

        add     fx, fx, #1

        mov     sreg01, fx, LSL #1      ; look for overflow

        cmn     sreg01, #0x01000000     ; compute sreg01 + 0x01000000

        Retcc                           ; exit if overflow did not occur  

; in IEEE_FULL mode arrive here with r3 = __SoftFPSCR__ and r12 = &__SoftFPSCR_

    #ifdef IEEE_FIXED

; @@@ Modified: added LFPState    

;ldr       sreg12, =__SoftFPSCR__                ; load FPSCR

;ldr    sreg03, [sreg12]      

        LFPState sreg03, sreg12

    #endif

        SetOverflowL sreg03, __wrap_overflow_handler_special  ; signal overflow

        StFPState sreg03,sreg12         ; store FPSCR

        Return

    #ifdef IEEE_FULL

up_round

        cmp     sreg02, #0              ; sreg02 holds the sign

        beq     dtof_add                ; add and check overflow only if positive

        Return



chop_or_down_round

        tst     sreg03, ZERO_OR_DOWN_BIT_MASK 

        Retne                           ; done if rounding is to zero

        cmp     sreg02, #0

        bne     dtof_add                ; add and check overflow only if negative

        Return

    #endif                                 
; #ifdef IEEE_FULL

  #endif                                   
; #if #defined(IEEE_FULL) && !defined(IEEE_FIXED)



; arrive here if the exponent is huge

; sreg02 has the sign

; dxh/dxl has the number

dtof_x_huge

        orrs    sreg03, dxl, dxh, LSL #12  ; check for non-zero bits in the significand

        bne     dtof_nan 

; x is INF, so return INF with the same sign

        mov     fx, #0xff000000         ; prepare code for INF

        orr     fx, sreg02, fx, LSR #1  ; make signed INF

        Return

dtof_nan

	#ifdef IEEE_FULL_OR_FIXED

        tst     dxh, #0x00080000        ; check signal

        bne     __f_make_NaN

; @@@ Modified: added LFPState    

        LFPState sreg03, sreg02

        SetInvalidE sreg03              ; set invalid

        StFPState sreg03,sreg02         ; store FPSCR  

  #endif

__f_make_NaN

        MakeFNaN

        Return



; arrive here if the exponent is tiny

; sreg02 has the sign

; dxh/dxl has the numbenr

dtof_x_tiny

  #ifndef IEEE_STD

        orrs    sreg03, dxl, dxh, LSL #12  ; look for any non-zero bits

	#ifdef IEEE_FULL_OR_FIXED

        beq     __f_result_zero

        LFPState sreg03, sreg03

        tst     sreg03, FE_IEEE_MASK_UNDERFLOW

        bne.w     __wrap_underflow_handler

        beq     __f_underflow

    #else

        bne     __f_underflow

    #endif

  #endif  

; the number is 0, so return a signed 0

__f_result_zero

        mov     fx, sreg02              ; signed zero

        Return



; arrive here if the exponent is zero or less

; sreg02 contains the sign

; sreg12 contains the exponent

; dxh/dxl has the number

dtof_negative_or_zero_exp

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg03, sreg03

        tst     sreg03, FE_IEEE_MASK_UNDERFLOW

        bne.w     __wrap_underflow_handler

  #endif

        cmn     sreg12, #23

        beq     dtof_very_tiny_result   ; case: very tiny

        bmi.w   __f_underflow           ; case: underflow

; exponent is -22...0

  #ifdef IEEE_STD

        mov     fx, sreg02              ; return signed 0 in STD mode

        Return

  #else      

        mov     dxh, dxh, LSL #11       ; keep only high part of significand

        orr     dxh, dxh, #0x80000000   ; set lead bit

        mov     sreg03, dxh, LSR #8     ; sreg03 now has a part of significand

        orr     sreg03, sreg03, dxl, LSR #29  ; other bits of significand

        rsb     sreg12, sreg12, #1      ; compute 1-exp

        movs    sreg01, dxl, LSL #3     ; round bits from dxl in sreg01    

;;;        orr     fx, sreg02, sreg03, LSR sreg12  ; shift right and stuff sign

	mov	fx, sreg03, LSR sreg12
	orr	fx, sreg02, fx

        rsb     sreg12, sreg12, #32     ; compute shift for round bits

        mov     sreg03, sreg03, LSL sreg12  ; round and sticky bits from shift

        orrne   sreg03, sreg03, #1      ; sticky bits from dxl

        movs    sreg01, sreg03          ; move in sreg01 and set flags

        Reteq                           ; done 

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

;ldr       sreg12, =__SoftFPSCR__                ; load FPSCR

;ldr    sreg03, [sreg12]

        LFPState sreg03, sreg12

        SetUnderflow sreg03             ; set underflow

        StFPState sreg03,sreg12         ; store FPSCR  

    #endif

    #ifdef IEEE_FULL

        tst     sreg03, CHOP_OR_DOWN_BIT_MASK

        bne     __f_chop_or_down_round_denorm

        tst     sreg03, UP_OR_NEAR_BIT_MASK 

        bne     __f_up_round_denorm     ; rounding is up

    #endif

; round to nearest

        tst     sreg01, #0x80000000     ; done if round bit is clear

        Reteq

        movs    sreg01, sreg01, LSL #1  ; round up if sticky bits are non-0

        andeqs  sreg01, fx, #0x1        ; in half-way case, round up if not already even

        addne   fx, fx, #1

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

  #endif                                   
; #ifdef IEEE_STD



; special case: exponent is -23

; sreg02 holds the sign

; dxh/dxl holds the number

dtof_very_tiny_result

        orr     fx, dxl, dxh, LSL #12   ; stickies

; fall through __f_very_riny_result



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

;ldr       sreg12, =__SoftFPSCR__                  ; load FPSCR

;ldr    sreg03, [sreg12]     

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



; an overflow occured, INF must be returned             

; arrive here with sreg02 = sign         

__f_overflow

	#ifdef IEEE_FULL_OR_FIXED

; @@@ Modified: added LFPState  

;ldr       sreg12, =__SoftFPSCR__                  ; load FPSCR

;ldr    sreg03, [sreg12]     

        LFPState sreg03, sreg12

        SetOverflow sreg03

        StFPState sreg03,sreg12         ; set overflow 

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



	#ifdef IEEE_FULL_OR_FIXED

	IMPORT __fp_handler_64_to_32



; overflow exception handler

__wrap_overflow_handler

        sub     dxh, dxh, #(192 << 20)  ; coerce exponent

        and     dxl, dxl, #0xE0000000   ; keep only 3 bits from dxl

        stmfd   sp!, {lr} 

        ldr     r2, =(FE_EX_OVERFLOW :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_CVT)

        bl      __fp_handler_64_to_32

        Return_lr



; overflow exception handler special (called for overflow after rounding)

__wrap_overflow_handler_special

        ldr     dxh, =0x3bf00000

        mov     dxl, #0x0

        stmfd   sp!, {lr} 

        ldr     r2, =(FE_EX_OVERFLOW :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_CVT)

        bl      __fp_handler_64_to_32

        Return_lr



; underflow exception handler       

__wrap_underflow_handler

        add     dxh, dxh, #(192 << 20)  ; coerce exponent

        and     dxl, dxl, #0xE0000000   ; keep only 3 bits from dxl

        stmfd   sp!, {lr} 

        ldr     r2, =(FE_EX_UNDERFLOW :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_CVT)

        bl      __fp_handler_64_to_32

        Return_lr



; invalid exception handler

__wrap_invalid_handler

        ldr     r2, =(FE_EX_INVALID :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_CVT) 

        stmfd   sp!, {lr}

        bl      __fp_handler_64_to_32

        Return_lr



  #endif



  #ifdef IEEE_FULL

	IMPORT __fp_handler_32_to_32



; inexact exception wrapper

__wrap_inexact_handler

        stmfd   sp!, {lr}

        ldr     r1, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_CVT)

        bl      __fp_handler_32_to_32

        Return_lr

  #endif

  #ifdef USING_ARMASM
	END
  #endif

#endif

/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */

