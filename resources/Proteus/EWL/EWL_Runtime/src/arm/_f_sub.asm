;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _f_sub.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



	EXPORT _f_sub

	EXPORT _fsub                   ; FPLIB for _f_sub

	EXPORT _frsb



	IMPORT __fadd_start

	EXPORT __fsub_start



	#ifdef VFP_AND_NOT_IEEE_STD

	EXPORT _vfp_support_fsub

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

        Retm

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

        Retm 	cc   

	MEND

	MACRO
	Prolog

        mov     sreg02, #FE_EX_FN_SUB

        stmfd   sp!, {fx, fy, sreg02}

	MEND

	MACRO
	Clrstk

        add     sp, sp, #12

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

; FUNCTION: float _frsb (float, float)

; DESCRIPTION: Reverse float substraction.

; INPUT: 

; fx=x,

; fy=y, floats

; OUTPUT: 

; fx = result



;===============================================================================

; FUNCTION: float _f_sub (float, float)

; DESCRIPTION: Float substraction.

; INPUT: 

; fx=x,

; fy=y, floats

; OUTPUT: 

; fx = result


	AREA |.text|,CODE,READONLY
	
#ifdef VFP

_fsub
	
_f_sub


        fmsr    s0, r0

        fmsr    s1, r1

        fsubs   s0, s0, s1

        fmrs    r0, s0

        Retm


_frsb

        fmsr    s0, r0

        fmsr    s1, r1

        fsubs   s0, s1, s0

        fmrs    r0, s0

        Retm


_vfp_support_fsub

#else


_frsb

; swap operands

        eor     fx, fx, fy

        eor     fy, fx, fy

        eor     fx, fx, fy

; and fall through _fsub


_f_sub

_fsub                                  ; FPLIB entry point

#endif



	#ifdef NOT_VFP_OR_NOT_IEEE_STD



        Prolog

; sub is add with y's sign flipped

        eors    sreg02, fx, fy

        eormi   fy, fy, #0x80000000     ; flip sign of y

        bmi.w     __fadd_start          ; this is an addition (different signs)

__fsub_start

; keep the highest operand in fx and adjust sign

        subs    sreg12, fx, fy

        eorlo   sreg12, sreg12, #0x80000000  ; change sign

        sublo   fx, fx, sreg12

        addlo   fy, fy, sreg12

        mov     sreg02, #0x80000000     ; will be used later

; unpack fx in sreg03(exp) and fx(significand)

        mov     sreg03, fx, LSR #23     ; sreg03 has exponent and sign   

        orr     fx, sreg02, fx, LSL #8  ; fx has significand & lead bit

        ands    sreg12, sreg03, #0xff   ; clear sign bit

        cmpne   sreg12, #0xff           ; test for huge exponent

        beq.w     fsub_x_special_case     ; special case: tiny or huge



; unpack fy in sreg12(exp) and fy(significand)  

        mov     sreg12, fy, LSR #23     ; sreg12 has exponent and sign

        orr     fy, sreg02, fy, LSL #8  ; fy has significand & lead bit

        ands    sreg02, sreg12, #0xff   ; clear sign bit

        beq.w     fsub_y_special_case     ; special case: tiny



fsub_start

; sreg03 has x exp and sign, fx has x significand

; sreg12 has y exp and sign, fy has y significand

; compare exponents

        subs    sreg12, sreg03, sreg12

        beq     fsub_aligned            ; sub significands for equal exponents

fsub_swap_done

; fx has the number with the highest exponent; must shift fy

; sreg03 has the final exponent and sign

; sreg12 has shift ammount

  #ifdef IEEE_FULL

        cmp     sreg12, #28

        movge   sreg12, #31

  #endif

        rsb     sreg02, sreg12, #32     ; sreg02 = left shift

        movs    sreg02, fy, LSL sreg02  ; bits off the right

        mov     fy, fy, LSR sreg12

        orrne   fy, fy, #1              ; 1 if first sticky is 1  

fsub_common

; sreg03 has exponent AND sign

; fx/fy have the significands

; sub significands

        subs    fx, fx, fy

        bpl     fsub_normalize

; normal case: pack 

; sreg03 has the exponent AND sign

; fx has the significand

; must coerce exponent for denorms 

fsub_pack_round

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

        tst     sreg01, #0x0080         ; done if round bit is clear

        Reteq

        ands    sreg01, sreg01, #0x7f   ; round up if sticky bits are non-0

        andeqs  sreg01, fx, #0x1        ; in half-way case, round up if not already even

        addne   fx, fx, #1

        Return

  #ifdef IEEE_FULL

up_round

        cmp     fx, #0                  ; check sign of fx

        addpl   fx, fx, #1              ; add and check overflow only if positive

        Return



chop_or_down_round

        tst     sreg03, ZERO_OR_DOWN_BIT_MASK 

        Retne                           ; done if rounding is to zero

        cmp     fx, #0

        addmi   fx, fx, #1              ; add and check overflow only if negative

        Return

  #endif



; arrive here when exponents are equal

; fx has x significand, fy has y significand

; sreg03 has final exponent and sign in position 8

fsub_aligned

        subs    fx, fx, fy

        beq.w     __f_result_signed_zero  ; result is signed 0 

        mov     sreg02, sreg03, LSL #23

        and     sreg02, sreg02, #0x80000000  ; sign in sreg02

        bic     sreg03, sreg03, #0x100  ; clear sign from exp register

        Normalize_a5 fx, sreg03, sreg12  ; otherwise normalize

        cmp     sreg03, #0

        bgt     fsub_aligned_cont

; exponent is <= 0, so shift

; fx has result (integer)

; sreg03 has exponent

; sreg02 has sign    

  #ifdef IEEE_STD

        mov     fx, #0                  ; just return 0 in IEEE_STD mode

        Return

  #else

        rsb     sreg03, sreg03, #9      ; compute 1-exp 

;;;        orr     fx, sreg02, fx, LSR sreg03  ; shift right and stuff sign
	mov	sreg03, fx, LSR sreg03
	orr	fx, sreg02, sreg03

        Return

  #endif

fsub_aligned_cont

        add     fx, fx, fx              ; clear msb (LSL #1)

        orr     fx, sreg02, fx, LSR #9  ; align 23 sig bits and stuff sign

        orr     fx, fx, sreg03, LSL #23  ; stuff exp 

        Return                          ; result is always exact        



; arrive here when exponent result must be normalized

; fx has x significand, fy has y significand

; sreg03 has final exponent and sign in position 8

fsub_normalize

        mov     sreg02, sreg03, LSL #23

        and     sreg02, sreg02, #0x80000000  ; sign in sreg02

        bic     sreg03, sreg03, #0x100  ; clear sign from exp register

        Normalize_a5 fx, sreg03, sreg12

        cmp     sreg03, #0

        bgt     fsub_pack_round2

; exponent is <= 0, so shift

; fx has result (integer)

; sreg03 has exponent

; sreg02 has sign    

  #ifdef IEEE_STD

        mov     fx, #0

        Return                          ; return 0 in IEEE_STD mode

  #else

        rsb     sreg03, sreg03, #9      ; compute shift ammount

;;;        orr     fx, sreg02, fx, LSR sreg03  ; shift right and stuff sign
	mov	sreg03, fx, LSR sreg03
	orr 	fx, sreg02, sreg03

        Return

  #endif

fsub_pack_round2

        ands    sreg01, fx, #0xff       ; round bits

        add     fx, fx, fx              ; clear msb (LSL #1)

        orr     fx, sreg02, fx, LSR #9  ; align 23 sig bits and stuff sign

        orr     fx, fx, sreg03, LSL #23  ; stuff exp 

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



; ============================================================================

; The code for special cases begins here

; ============================================================================



; arrive here if x is either tiny or huge

; fx has x significand and lower bit of exp, sreg03 has x exponent and sign

fsub_x_special_case

        cmp     sreg03, #0x100          ; figure out sign

        movge   sreg02, #0x80000000     ; negative sign

        movlt   sreg02, #0x00000000     ; positive sign

        ands    sreg03, sreg03, #0xff   ; strip sign from exp register

        #ifdef IEEE_STD

          beq     fsub_result_minusy

        #else

          beq     fsub_x_tiny             ; x is tiny

        #endif

; otherwise fall through fsub_x_huge



; arrive here if x exp is 0xff

; sreg03 has x sign

; fx has significand of x and lower bit of exp

fsub_x_huge

        movs    fx, fx, LSL #1          ; clear high bit

        bne.w     __f_result_x_NaN        ; check x and return a QNaN

; x is INF 

; must unpack y

; unpack fy in sreg12(exp) and fy(significand)  

        mov     sreg12, fy, LSR #23     ; sreg12 has exponent and sign

        mov     fy, fy, LSL #9          ; fy has significand  

        ands    sreg12, sreg12, #0xff   ; clear sign bit

        beq.w     __f_result_INF          ; INF - 0 = INF - denorm = INF

        cmp     sreg12, #0xff 

        blt.w     __f_result_INF          ; INF - norm = INF

; if we arrive here x is INF, y is either INF, NaN

        cmp     fy, #0

        beq.w     __f_result_invalid      ; INF - INF = invalid

        b       fsub_y_NaN              ; INF - NaN = QNaN, must check y



; arrive here if y is either tiny or huge

; fx has x significand and lead bit, sreg03 has x exponent and x sign

; fy has y significand and lower bit of exp, sreg12 has y exponent and y sign

fsub_y_special_case

        cmp     sreg12, #0x100          ; figure out sign

        movge   sreg02, #0x80000000     ; negative sign

        movlt   sreg02, #0x00000000     ; positive sign

        and     sreg03, sreg03, #0xff   ; strip sign from exp register

        ands    sreg12, sreg12, #0xff   ; same for y 

        #ifdef IEEE_STD

          beq     fsub_result_x_std

        #else

          beq     fsub_y_tiny             ; y is tiny

        #endif

; otherwise fall through fsub_y_huge



fsub_y_huge_flipsign

        eor     sreg02, sreg02, #0x80000000  ; y sign    

; arrive here if y exp is 0xff AND x is a normal number

; sreg02 has y sign

; sreg03 has x exp, fx has x significand and lead bit

; sreg12 has y exp, fy has y significand and lower bit of exp

fsub_y_huge

        movs    fy, fy, LSL #1          ; clear high bit

        bne.w     fsub_y_NaN              ; check y and return a QNaN

; y is INF, x is a normal number, so return INF WITH THE SIGN FLIPPED

        b       __f_result_INF



; arrive here if x exp is 0x00 

; sreg02 has x sign

; sreg03 has x exp, fx has x significand and lower bit of exp

fsub_x_tiny

        movs    fx, fx, LSL #1          ; clear high bit

        beq     fsub_x_zero             ; x is 0

; x is a denorm

; don't need to normalize, just set exponent

        mov     fx, fx, LSR #1          ; place for lead bit  

        mov     sreg03, #1              ; true exp for denorms

; unpack fy in sreg12(exp) and fy(significand)  

        mov     sreg12, fy, LSR #23     ; sreg12 has exponent and sign

        mov     fy, fy, LSL #8          ; fy has significand & low bit of exp

        ands    sreg12, sreg12, #0xff   ; clear sign bit

        beq     fsub_y_tiny_flipsign

        cmp     sreg12, #0xff           ; huge exponent

        beq     fsub_y_huge_flipsign

        orr     fy, fy, #0x80000000     ; set lead bit  

        orr     sreg03, sreg03, sreg02, LSR #23  ; put sign back

        orr     sreg12, sreg12, sreg02, LSR #23  ; put sign back

        b       fsub_start              ; branch to common code

fsub_x_zero

; unpack fy in sreg01(exp) and fx(significand)  

        mov     sreg03, fy, LSR #23     ; sreg03 has exponent and sign

        mov     fx, fy, LSL #9          ; fx has significand 

        ands    sreg02, sreg03, #0xff   ; clear sign bit

        beq     fsub_y_denorm_zero 

        cmp     sreg02, #0xff

        blt     fsub_result_minusy      ; 0 - norm = -y

        cmp     fx, #0

        bne     __f_result_NaN          ; NaN

; y is INF : return INF with y's sign flipped

;  eor    sreg03, sreg03, #0x100        ; sign bit

;  mov    sreg03, sreg03, LSR #8        ; sign in LSB of sreg03

;  mov    sreg02, sreg03, LSL #31       ; sign in MSB of sreg03

        b       __f_result_INF          ; O - INF = INF with sign flipped

fsub_y_denorm_zero

        cmp     fx, #0

        beq     __f_result_signed_zero  ; 0 - 0, must check for rounding mode

        b       fsub_result_minusy      ; result is -y  



fsub_y_tiny_flipsign

;  eor    sreg02, sreg02, #0x80000000   ; sign of y    

; arrive here if y exp is 0x00 AND x is a denorm or a norm

; sreg02 has y sign

; sreg03 has x exp, fx has x significand and lead bit

; sreg12 has y exp, fy has y significand and lower bit of exp

fsub_y_tiny

        movs    fy, fy, LSL #1          ; clear high bit

        beq     fsub_result_x           ; x - 0 = x

; y is a denorm, so normalize and compute the result

        mov     fy, fy, LSR #1          ; place for lead bit

        mov     sreg12, #1              ; true exp of denormals

        orr     sreg12, sreg12, sreg02, LSR #23  ; put sign back  

;  eor    sreg02, sreg02, #0x80000000   ; compute sign of x  

        orr     sreg03, sreg03, sreg02, LSR #23  ; put sign back

        b       fsub_start              ; normal case



; arrive here if x is 0

; y was not unpacked yet, so just return it  

fsub_result_minusy

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

fsub_result_x_std

        add     fx, fx, fx              ; strip MSB    

        mov     fx, fx, LSR #9          ; keep 23 bits 

        orr     fx, fx, sreg03, LSL #23  ; stuff exponent and sign

        orr     fx, fx, sreg02

        Return

  #endif  



; arrive here if y is 0 and x must be returned

; x is a normal number, unpacked 

; sreg03 has the exponent

; fx has x significand and lead bit

; must repack 

fsub_result_x

;  eor    sreg02, sreg02, #0x80000000   ; find out x sign first

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



; arrive here if a signed 0 must be return

; return +0 if rounding down, -0 otherwise

__f_result_signed_zero

        mov     fx, #0

  #ifdef IEEE_FULL

        LFPState sreg03, sreg03

        tst     sreg03, CHOP_OR_DOWN_BIT_MASK 

        Reteq                           ; up or nearest: +0

        tst     sreg03, ZERO_OR_DOWN_BIT_MASK

        Retne                           ; chop: +0

        mov     fx, #0x80000000         ; -0 for down

  #endif

        Return



; pack the result and exit

; sreg03 has exponent and sign

; fx has significand

fsub_pack

        cmp     fx, #0                  ; test lead bit

        subges  sreg03, sreg03, #1      ; sub 1 for denormals

  #ifdef IEEE_STD

        moveq   fx, #0                  ; return 0 for denorms in IEEE_STD mode

        Return

  #endif   

        add     fx, fx, fx              ; clear msb (LSL #1)

        mov     fx, fx, LSR #9          ; align 23 sig bits and stuff sign

        orr     fx, fx, sreg03, LSL #23  ; stuff exp and sign

        Return                          ; done  



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

fsub_y_NaN

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



; inexact exception wrapper

__wrap_inexact_handler

        Clrstk

        stmfd   sp!, {lr}

        ldr     r1, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_SUB)

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

#endif
	END

/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */
