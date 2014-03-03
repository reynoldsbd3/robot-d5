;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _f_add.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



	EXPORT _f_add

	EXPORT _fadd                   ; FPLIB for _f_add



	EXPORT __fadd_start

	IMPORT __fsub_start



	#ifdef VFP_AND_NOT_IEEE_STD

	EXPORT _vfp_support_fadd

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

        add     sp, sp, #12

        Retm    al

	MEND

	MACRO
	Reteq

        addeq   sp, sp, #12

        Retm    eq

	MEND

	MACRO
	Retne

        addne   sp, sp, #12

        Retm    ne

	MEND

	MACRO
	Retcc

        addcc   sp, sp, #12

        Retm    cc

	MEND

	MACRO
	Prolog

        mov     sreg02, #FE_EX_FN_ADD

        stmfd   sp!, {fx, fy, sreg02}

	MEND

	MACRO
	Clrstk

        add     sp, sp, #12

	MEND

#else

	MACRO
	Return

        Retm    al

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

; FUNCTION: float _f_add (float, float)

; DESCRIPTION: Float addition.

; INPUT: 

; fx=x,

; fy=y, floats

; OUTPUT: 

; fr = result

	AREA |.text|,CODE,READONLY

#ifdef VFP

_fadd

_f_add

        fmsr    s0, r0

        fmsr    s1, r1

        fadds   s0, s0, s1

        fmrs    r0, s0

        Retm



_vfp_support_fadd

#else

_fadd									; FPLIB entry point

_f_add


#endif



	#ifdef NOT_VFP_OR_NOT_IEEE_STD



        Prolog

; figure out sign

        eors    sreg02, fx, fy

        eormi   fy, fy, #0x80000000 

        bmi.w     __fsub_start            ; change sign & branch for substraction

__fadd_start

; make sure that the higher number is in fx and the lower number in fy

        subs    sreg12, fx, fy

        sublo   fx, fx, sreg12

        addlo   fy, fy, sreg12

        mov     sreg02, #0x80000000     ; will need this later

; unpack fx in sreg03(exp) and fx(significand)

        mov     sreg03, fx, LSR #23     ; sreg03 has exponent and sign   

        orr     fx, sreg02, fx, LSL #8  ; fx has significand & lead bit

        ands    sreg12, sreg03, #0xff   ; clear sign bit

        cmpne   sreg12, #0xff           ; test for huge exponent

        beq     fadd_x_special_case     ; special case: tiny or huge



; unpack fy in sreg12(exp) and fy(significand)  

        mov     sreg12, fy, LSR #23     ; sreg12 has exponent and sign

        orr     fy, sreg02, fy, LSL #8  ; fy has significand & lead bit

        ands    sreg02, sreg12, #0xff   ; clear sign bit

        beq.w     fadd_y_special_case     ; special case: tiny or huge



fadd_start

; sreg03 has x exp and sign, fx has x significand

; sreg12 has y exp and sign, fy has y significand

; check exponents

        subs    sreg12, sreg03, sreg12

        beq     fadd_common             ; add significands for equal exponents

; fx has the number with the highest exponent; must shift fy

; sreg03 has the final exponent

; sreg12 has shift ammount

; case: shift less that 9 bits

  #ifdef IEEE_FULL

        cmp     sreg12, #28

        movge   sreg12, #31

  #endif

        rsb     sreg02, sreg12, #32     ; sreg02 = left shift

        movs    sreg02, fy, LSL sreg02  ; bits off the right

        mov     fy, fy, LSR sreg12

        orrne   fy, fy, #1              ; sticky

fadd_common

; sreg03 has exponent AND sign

; fx/fy have the significands

; add significands and watch for carry

        adds    fx, fx, fy

        bcc     fadd_next

        and     sreg01, fx, #1          ; sticky

        orr     fx, sreg01, fx, RRX     ; clear high bit and stuff lead bit (carry)

        add     sreg03, sreg03, #1

        and     sreg02, sreg03, #0xff   ; strip sign from exp

        cmp     sreg02, #0xff 

        beq.w     fadd_overflow 

fadd_next

; normal case: pack 

; sreg03 has the exponent AND sign

; fx has the significand

fadd_pack

        ands    sreg01, fx, #0xff       ; round bits

        add     fx, fx, fx              ; clear msb (LSL #1)

        mov     fx, fx, LSR #9          ; align 23 sig bits 

        orr     fx, fx, sreg03, LSL #23  ; stuff exp and sign

  #ifdef IEEE_FULL        

        Reteq                           ; no rounding: done

; rounding must be done here

; here we have the inexact result packed in fx

; sreg01 contains all the rounding bits

; must round and set the inexact bit in IEEE_FULL mode

; cases for rounding :

; ZERO - do nothing

; DOWN - check sign and substract 1 if negative

; UP - check sign and add 1 if positive

; NEAREST - the normal case  

        LFPState sreg03, sreg12

        SetInexact sreg03               ; set inexact

        StFPState sreg03,sreg12         ; store FPSCR  

        tst     sreg03, CHOP_OR_DOWN_BIT_MASK

        bne     chop_or_down_round

        tst     sreg03, UP_OR_NEAR_BIT_MASK 

        bne     up_round                ; rounding is up

  #endif

; round to nearest  

        tst     sreg01, #0x0080         ; done if round bit is clear

        Reteq

        ands    sreg01, sreg01, #0x7f   ; round up if sticky bits are non-0

        andeqs  sreg01, fx, #0x1        ; in half-way case, round up if not already even

  #if !defined(IEEE_FULL) && !defined(IEEE_FIXED)

; mode is NO_FENV or STD; just round and return

        addne   fx, fx, #1

        Return

  #else   

; IEEE_FULL or IEEE_FIXED; must check overflow

        Reteq                           ; done if nothing must be added

fadd_add

        add     fx, fx, #1

        mov     sreg01, fx, LSL #1      ; look for overflow

        cmn     sreg01, #0x01000000     ; compute sreg01 + 0x01000000

        Retcc                           ; exit if overflow did not occur  

; in IEEE_FULL mode arrive here with r3 = __SoftFPSCR__ and r12 = &__SoftFPSCR_

    #ifdef IEEE_FIXED

; @@@ Modified: added LFPState 

        LFPState sreg03, sreg12

    #endif

        SetOverflow sreg03              ; signal overflow

        StFPState sreg03,sreg12         ; store FPSCR

        Return

    #ifdef IEEE_FULL

up_round

        cmp     fx, #0                  ; check sign of fx

        bpl     fadd_add                ; add and check overflow only if positive

        Return



chop_or_down_round

        tst     sreg03, ZERO_OR_DOWN_BIT_MASK 

        Retne                           ; done if rounding is to zero

        cmp     fx, #0

        bmi     fadd_add                ; add and check overflow only if negative

        Return

    #endif                                 
; #ifdef IEEE_FULL

  #endif                                   
; #if #defined(IEEE_FULL) && !defined(IEEE_FIXED) 



; ============================================================================

; The code for special cases begins here

; ============================================================================



; arrive here if x is either tiny or huge

; fx has x significand and lead bit, sreg03 has x exponent and sign

fadd_x_special_case

        cmp     sreg03, #0x100          ; figure out sign

        movge   sreg02, #0x80000000     ; negative sign

        movlt   sreg02, #0x00000000     ; positive sign

        ands    sreg03, sreg03, #0xff   ; strip sign from exp register

        #ifdef IEEE_STD

          beq     fadd_result_y

        #else        

          beq     fadd_x_tiny             ; x is tiny

        #endif

; otherwise fall through fadd_x_huge



; arrive here if x exp is 0xff

; sreg02 has the sign

; fx has significand of x and lead bit

fadd_x_huge

        movs    fx, fx, LSL #1          ; clear high bit

        bne.w     __f_result_x_NaN        ; check x and return a QNaN

; x is INF 

; must unpack y

; unpack fy in sreg12(exp) and fy(significand)  

        mov     sreg12, fy, LSR #23     ; sreg12 has exponent and sign

        mov     fy, fy, LSL #9          ; fy has significand  

        ands    sreg12, sreg12, #0xff   ; clear sign bit

        beq.w     __f_result_INF          ; INF + 0 = INF + denorm = INF

        cmp     sreg12, #0xff 

        blt.w     __f_result_INF          ; INF + norm = INF

; if we arrive here x is INF, y is either INF, NaN

        cmp     fy, #0

        beq.w     __f_result_INF          ; INF + INF = INF

        b       fadd_y_NaN              ; INF + NaN = QNaN, must check y



; arrive here if y is either tiny or huge

; fx has x significand and lead bit, sreg03 has x exponent and x sign

; fy has y significand and lead bit, sreg12 has y exponent and y sign

fadd_y_special_case

        cmp     sreg03, #0x100          ; figure out sign

        movge   sreg02, #0x80000000     ; negative sign

        movlt   sreg02, #0x00000000     ; positive sign

        and     sreg03, sreg03, #0xff   ; strip sign from exp register

        ands    sreg12, sreg12, #0xff   ; same for y 

        #ifdef IEEE_STD

          beq     fadd_result_x_std

        #else

          beq     fadd_y_tiny             ; y is tiny

        #endif

; otherwise fall through fadd_y_huge



; arrive here if y exp is 0xff AND x is a normal number

; sreg02 has the sign

; sreg03 has x exp, fx has x significand and lead bit

; sreg12 has y exp, fy has y significand and lower bit of exp

fadd_y_huge

        movs    fy, fy, LSL #1          ; clear high bit

        bne.w     fadd_y_NaN              ; check y and return a QNaN

; y is INF, x is a normal number, so return INF

        b       __f_result_INF



; arrive here if x exp is 0x00 

; sreg02 has the sign

; sreg03 has x exp, fx has x significand and lead bit

fadd_x_tiny

        movs    fx, fx, LSL #1          ; clear high bit

        beq     fadd_x_zero             ; x is 0

; x is a denorm

; don't need to normalize, just set exponent

        mov     sreg03, #1              ; true exp for denorms  

        mov     fx, fx, LSR #1          ; place for lead bit  

; unpack fy in sreg12(exp) and fy(significand)  

        mov     sreg12, fy, LSR #23     ; sreg12 has exponent and sign

        mov     fy, fy, LSL #8          ; fy has significand & low bit of exp

        ands    sreg12, sreg12, #0xff   ; clear sign bit

        beq     fadd_y_tiny

        cmp     sreg12, #0xff           ; huge exponent

        beq     fadd_y_huge

        orr     fy, fy, #0x80000000     ; set lead bit  

        orr     sreg03, sreg03, sreg02, LSR #23  ; put sign back

        orr     sreg12, sreg12, sreg02, LSR #23  ; put sign back

        b       fadd_start              ; branch to common code

fadd_x_zero

; unpack fy in sreg01(exp) and fx(significand)  

        mov     sreg03, fy, LSR #23     ; sreg03 has exponent and sign

        mov     fx, fy, LSL #9          ; fx has significand 

        ands    sreg03, sreg03, #0xff   ; clear sign bit

        beq     fadd_result_y           ; 0 + denorm = 0 + 0 = y

        cmp     sreg03, #0xff

        blt     fadd_result_y           ; 0 + norm = y

        cmp     fx, #0

        beq.w     __f_result_INF          ; O + INF = INF

        b       __f_result_NaN



; arrive here if y exp is 0x00 AND x is a denorm or a norm

; sreg02 has the sign

; sreg03 has x exp, fx has x significand and lead bit

; sreg12 has y exp, fy has y significand and lead bit

fadd_y_tiny

        movs    fy, fy, LSL #1          ; clear high bit

        beq     fadd_result_x           ; x + 0 = x

; y is a denorm, so compute the result

        mov     fy, fy, LSR #1          ; place for lead bit

        mov     sreg12, #1              ; true exp of denormals

        orr     sreg03, sreg03, sreg02, LSR #23  ; put sign back

        orr     sreg12, sreg12, sreg02, LSR #23  ; put sign back

        cmp     fx, #0

        bmi     fadd_start              ; denorm + norm case  

; @@@ Modified here

;  b      fadd_start                    ; normal case

; both numbers are denorms; special case

; sreg12 has exponent AND sign

; fx/fy have the significands

; add significands and watch for carry

        adds    fx, fx, fy

        bcc     fadd_next_2

        and     sreg01, fx, #1          ; sticky

        orr     fx, sreg01, fx, RRX     ; clear high bit and stuff lead bit (carry)

        add     sreg12, sreg12, #1

fadd_next_2

        cmp     fx, #0

        subge   sreg12, sreg12, #1

  #ifdef IEEE_STD

        bics    sreg02, sreg12, #0x100  ; strip sign

        moveq   fx, #0

        Reteq                           ; return 0 for denorms in IEEE_STD mode

  #endif

        ands    sreg01, fx, #0xff       ; round bits

        add     fx, fx, fx              ; clear msb (LSL #1)

        mov     fx, fx, LSR #9          ; align 23 sig bits 

        orr     fx, fx, sreg12, LSL #23  ; stuff exp and sign

        Reteq                           ; no rounding: done

; rounding must be done here

; here we have the inexact result packed in fx

; sreg01 contains all the rounding bits

; must round and set the inexact bit in IEEE_FULL mode

; cases for rounding :

; ZERO - do nothing

; DOWN - check sign and substract 1 if negative

; UP - check sign and add 1 if positive

; NEAREST - the normal case  

  #ifdef IEEE_FULL

; @@@ Modified: added LFPState

;ldr       sreg12, =__SoftFPSCR__                ; load FPSCR

;ldr    sreg03, [sreg12]

        LFPState sreg03, sreg12

        SetInexact sreg03               ; set inexact

        StFPState sreg03,sreg12         ; store FPSCR  

        tst     sreg03, CHOP_OR_DOWN_BIT_MASK

        bne     chop_or_down_round

        tst     sreg03, UP_OR_NEAR_BIT_MASK 

        bne     up_round                ; rounding is up

  #endif

; round to nearest  

        tst     sreg01, #0x0080         ; done if round bit is clear

        Reteq

        ands    sreg01, sreg01, #0x7f   ; round up if sticky bits are non-0

        andeqs  sreg01, fx, #0x1        ; in half-way case, round up if not already even

        addne   fx, fx, #1

        Return



; arrive here if x is 0

; y was not unpacked yet, so just return it  

fadd_result_y

  #ifdef IEEE_STD

; must check if y is a denorm

        mov     sreg03, fy, LSR #23     ; sreg12 has exponent and sign    

        ands    sreg03, sreg03, #0xff   ; clear sign bit    

        moveq   fy, #0                  ; make denorms 0

  #endif

        mov     fx, fy

        Return



; arrive here if x must be returned in IEEE_STD mode

; sreg03 has x exponent

; fx has x significand and lead bit

; sreg02 has sign

  #ifdef IEEE_STD

fadd_result_x_std

        add     fx, fx, fx              ; strip MSB    

        mov     fx, fx, LSR #9          ; keep 23 bits 

        orr     fx, fx, sreg03, LSL #23  ; stuff exponent and sign

        orr     fx, fx, sreg02

        Return

  #endif  



; arrive here if y is 0 and x must be returned

; x is a normal number, unpacked 

; sreg02 has the sign

; sreg03 has the exponent

; fx has x significand and lead bit

; must repack 

fadd_result_x

        cmp     fx, #0                  ; test lead bit

        subges  sreg03, sreg03, #1      ; sub 1 for denormals

  #ifdef IEEE_STD

        moveq   fx, #0                  ; return 0 for denorms in IEEE_STD mode

        Reteq

  #endif   

        add     fx, fx, fx              ; clear msb (LSL #1)

        orr     fx, sreg02, fx, LSR #9  ; align 23 sig bits and stuff sign

        orr     fx, fx, sreg03, LSL #23  ; stuff exp

        Return                          ; done



; arrive here when an overflow occurs after adding the numbers

; must get sign in sreg02 

fadd_overflow

        cmp     sreg03, #0x100

        movge   sreg02, #0x80000000     ; negative sign

        movlt   sreg02, #0x00000000     ; positive sign

	#ifdef IEEE_FULL_OR_FIXED

; @@@ Modified: added LFPState     

        LFPState sreg03, sreg12

        SetOverflowL sreg03, __wrap_overflow_handler_pack

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



__f_result_NaN

; arrive here when input is a NaN and invalid must be checked

; return a QNaN and check for invalid if required

; fx contains the unpacked significand, so check MSB

	#ifdef IEEE_FULL_OR_FIXED

        mov     sreg03, fx              ; save high part for tests

        MakeFNaN

        tst     sreg03, #0x80000000     ; check signal

        Retne                           ; exit if quiet

; @@@ Modified: added LFPState

;ldr       sreg02, =__SoftFPSCR__            ; load FPSCR address

;ldr    sreg03, [sreg02]                               ; load FPSCR

        LFPState sreg03, sreg02

        SetInvalid sreg03               ; set invalid

        StFPState sreg03,sreg02         ; store FPSCR  

        Return

  #else

        MakeFNaN

        Return

  #endif



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

fadd_y_NaN

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



	#ifdef IEEE_FULL_OR_FIXED

	IMPORT __fp_handler_32_to_32

	IMPORT __fp_handler_binary_32_to_32



; overflow exception handler

__wrap_overflow_handler

        Clrstk

        sub     fx, fx, #(192 << 23)    ; coerce exponent

        stmfd   sp!, {lr} 

        ldr     r1, =(FE_EX_OVERFLOW :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_ADD)

        bl      __fp_handler_32_to_32

        Return_lr



; overflow exception handler with pack

; fx has significand, exp is always 63 (255-192), sreg02 has sign 

__wrap_overflow_handler_pack

        Clrstk

        orr     fx, sreg02, fx, LSR #8  ; align 23 sig bits and stuff sign

        add     fx, fx, #(62 << 23)     ; exp := exp - 1 + lead bit  

        stmfd   sp!, {lr} 

        ldr     r1, =(FE_EX_OVERFLOW :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_ADD)

        bl      __fp_handler_32_to_32

        Return_lr



; invalid exception handler

__wrap_invalid_handler

        ldmfd   sp!, {fx, fy, sreg02}

        stmfd   sp!, {lr}

        ldr     r12, =(FE_EX_INVALID :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_FLOAT)

        orr     sreg02, sreg02, sreg12

        bl      __fp_handler_binary_32_to_32 

        Return_lr



  #endif



  #ifdef IEEE_FULL



; inexact exception wrapper

__wrap_inexact_handler

        Clrstk

        stmfd   sp!, {lr}

        ldr     r1, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_ADD)

        bl      __fp_handler_32_to_32

        Return_lr

  #endif

#endif
	END

/* Change record:
 * 
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */
