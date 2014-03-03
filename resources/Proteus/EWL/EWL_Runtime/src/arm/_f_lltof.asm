;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _f_lltof.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



; @@@ All the code in this file was modified to use llh/lll instead of dxh/dxl

; @@@ in order to insure compatibility with the SoftFPA mode    



;===============================================================================



	EXPORT _f_ulltof

	EXPORT _ll_uto_f               ; FPLIB for _f_ulltof

	EXPORT _f_lltof

	EXPORT _ll_sto_f               ; FPLIB for _f_lltof

	EXPORT _d_ulltod

	EXPORT _ll_uto_d               ; FPLIB for _d_ulltod

	EXPORT _d_lltod

	EXPORT _ll_sto_d               ; FPLIB for _d_lltod



	IMPORT __f_utof_common

	IMPORT __f_itof_common

	IMPORT __d_utod_common

	IMPORT __d_itod_common



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

; FUNCTION: _f_ulltof

; DESCRIPTION: Convert unsigned long long to float.

; INPUT:

;       llh/lll = unsigned long long word

; OUTPUT:

;       fx = float

; USES:

;       sreg03 = type of convert

; OPTIMIZATION: Uses signed conversion, with faked positive sign.

;
	AREA |.text|,CODE,READONLY
	
_ll_uto_f                              ; FPLIB entry point

_f_ulltof



; if high bits are 0, we just have a 32-bit integer to convert

        cmp     llh, #0

        bne     dwtoX_hard_ulltof       ; not easy 32-bit case

        movs    fx, lll

        b       __f_utof_common

dwtoX_hard_ulltof

        mov     sreg03, #32

; at this point, condition codes set based on comparing llh to 0

        bmi     dwtoX_hard_normalized_ulltof

        Normalize_a5 llh, sreg03, sreg12

;;;        orr     llh, llh, lll, LSR sreg03

	mov 	sreg02, lll, LSR sreg03
	orr	llh, llh, sreg02

        rsb     sreg02, sreg03, #32

        mov     lll, lll, LSL sreg02

dwtoX_hard_normalized_ulltof

        cmp     lll, #0

        orrne   llh, llh, #1

        add     sreg03, sreg03, #158    ; bias exponent by 127+31

        ands    sreg02, llh, #0xff      ; keep rounding bits in sreg02

        add     fx, llh, llh            ; clear msb of significand

        mov     fx, fx, LSR #9          ; align

        orr     fx, fx, sreg03, LSL #23  ; stuff exponent

        Return  eq                      ; done (no sticky bits)

; here we have the inexact result packed in fx

; must round and set the inexact bit in IEEE_FULL mode

; cases for rounding :

; ZERO - do nothing

; DOWN - do nothing, the number is never negative

; UP - always round, the number is always positive

; NEAREST - the normal case

#ifdef IEEE_FULL

; @@@ Modified: added LFPState

;ldr       sreg01,=__SoftFPSCR__                ; load FPSCR

;ldr    sreg03,[sreg01]

        LFPState sreg03, sreg01

        SetInexactL sreg03, __wrap_inexact_handler_ulltof  ; set inexact

        StFPState sreg03,sreg01         ; store FPSCR  

        tst     sreg03, CHOP_OR_DOWN_BIT_MASK

        Return  ne

        tst     sreg03, UP_OR_NEAR_BIT_MASK 

        bne.w     f_round_add2            ; rounding is up: always add 1

#endif

; round to nearest

        tst     sreg02, #0x0080         ; done if round bit is clear

        Return  eq

        ands    sreg01, sreg02, #0x7f   ; round up if sticky bits are non-0

        andeqs  sreg01, fx, #0x1        ; in half-way case, round up if not already even

        addne   fx, fx, #1

        Return



;===============================================================================

; FUNCTION: _f_lltof

; DESCRIPTION: Convert signed long long to float.

; INPUT:

;       llh/lll = signed long long word

; OUTPUT:

;       fx = float

; USES:

;       sreg03 = type of convert

; CASES: Zero maps to +0. Large values are rounded according to the current mode.

; OPTIMIZATION: If leading word is 0 (after sign strip), go right to 32-bit

; case. Otherwise, accumulate low sticky bits in normalized high word and finish

; in 32-bit case.

;
	AREA |.text|,CODE,READONLY
	
_ll_sto_f                              ; FPLIB entry point
	
_f_lltof



        ands    sreg02, llh, #0x80000000  ; sign in sreg02

        beq     f_lltof_pos

; Negate.

        rsbs    lll, lll, #0

;;;        rsc     llh, llh, #0
	mov	sreg12, #0
	sbc	llh, sreg12, llh

f_lltof_pos

; if high bits are 0, we just have a 32-bit integer to convert

        cmp     llh, #0

        bne     dwtoX_hard_lltof        ; not easy 32-bit case

        movs    fx, lll

        b       __f_itof_common

dwtoX_hard_lltof

        Normalize_num llh, sreg03, #32

;;;        orr     llh, llh, lll, LSR sreg03

	mov	sreg12, lll, LSR sreg03
	orr	llh, llh, sreg12

        rsb     sreg12, sreg03, #32

        movs    lll, lll, LSL sreg12 

        orrne   llh, llh, #1

        add     sreg03, sreg03, #158    ; bias exponent by 127+31

        ands    sreg12, llh, #0xff      ; keep rounding bits in sreg12

        add     fx, llh, llh            ; clear msb of significand

        orr     fx, sreg02, fx, LSR #9  ; align and stuff sign bit

        orr     fx, fx, sreg03, LSL #23  ; stuff exponent

        Return  eq                      ; done (no sticky bits)

; here we have the inexact result packed in fx

; must round and set the inexact bit in IEEE_FULL mode

; cases for rounding :

; ZERO - do nothing

; DOWN - check sign and substract 1 if negative

; UP - check sign and add 1 if positive

; NEAREST - the normal case

#ifdef IEEE_FULL

; @@@ Modified: added LFPState

;ldr       sreg02,=__SoftFPSCR__                ; load FPSCR

;ldr    sreg03,[sreg02]

        LFPState sreg03, sreg02

        SetInexactL sreg03, __wrap_inexact_handler_lltof  ; set inexact

        StFPState sreg03,sreg02         ; store FPSCR  

        tst     sreg03, CHOP_OR_DOWN_BIT_MASK

        bne.w     chop_or_down_round

        tst     sreg03, UP_OR_NEAR_BIT_MASK 

        bne.w     up_round                ; rounding is up

#endif

; round to nearest

        tst     sreg12, #0x0080         ; done if round bit is clear

        Return  eq

        ands    sreg03, sreg12, #0x7f   ; round up if sticky bits are non-0

        andeqs  sreg03, fx, #0x1        ; in half-way case, round up if not already even

        addne   fx, fx, #1

        Return



;===============================================================================

; FUNCTION: _d_ulltod

; DESCRIPTION: Convert unsigned double-word to double.

; INPUT:

;       llh/lll = unsigned long long word

; OUTPUT:

;       dxh/dxl = double

; USES:

;   sreg02 = sign

;       sreg03 = type of convert

; OPTIMIZATION: Uses signed conversion, with faked positive sign.

;

	AREA |.text|,CODE,READONLY
	
_ll_uto_d                              ; FPLIB entry point

_d_ulltod


; if high bits are 0, we just have a 32-bit integer to convert

        cmp     llh, #0

        bne     dwtoX_hard_ulltod       ; not easy 32-bit case

        movs    fx, lll

        b       __d_utod_common

dwtoX_hard_ulltod

        mov     sreg03, #32

; at this point, condition codes set based on comparing llh to 0

        bmi     dwtoX_hard_normalized_ulltod

        Normalize_a5 llh, sreg03, sreg12 

;;;        orr     llh, llh, lll, LSR sreg03

	mov	sreg02, lll, LSR sreg03
	orr	llh, llh, sreg02

        rsb     sreg02, sreg03, #32

        mov     lll, lll, LSL sreg02

dwtoX_hard_normalized_ulltod

        add     sreg03, sreg03, #1024   ; bias exponent by 1023+31

        add     sreg03, sreg03, #30

        movs    sreg12, lll, LSL #21    ; keep roundind bits IN THE HIGH PART of sreg12

#if defined(SOFTFPA) && !defined(BIG_ENDIAN)

; swap the values of llh/lll in SOFTFPA, little endian mode

        eor     llh, llh, lll

        eor     lll, llh, lll

        eor     llh, llh, lll

; and then swap between llh/lll in pack to preserve registers

        mov     dxl, llh, LSR #11       ; align lowest 21 bits

        orr     dxl, dxl, lll, LSL #21  ; lowest 32 bits

        add     dxh, lll, lll           ; shift off msb (LSL #1)

        mov     dxh, dxh, LSR #12

        orr     dxh, dxh, sreg03, LSL #20  ; stuff exponent

        Return  eq                      ; done (no sticky bits)

#else

        mov     dxl, lll, LSR #11       ; align lowest 21 bits

        orr     dxl, dxl, llh, LSL #21  ; lowest 32 bits

        add     dxh, llh, llh           ; shift off msb (LSL #1)

        mov     dxh, dxh, LSR #12

        orr     dxh, dxh, sreg03, LSL #20  ; stuff exponent

        Return  eq                      ; done (no sticky bits)

#endif  

; here we have the inexact result packed in dxh/dxl

; must round and set the inexact bit in IEEE_FULL mode

; cases for rounding :

; ZERO - do nothing

; DOWN - do nothing, the number is never negative

; UP - always round, the number is always positive

; NEAREST - the normal case

#ifdef IEEE_FULL

; @@@ Modified: added LFPState

;ldr       sreg02,=__SoftFPSCR__                ; load FPSCR

;ldr    sreg03,[sreg02]

        LFPState sreg03, sreg02

        SetInexactL sreg03, __wrap_inexact_handler_ulltod  ; set inexact

        StFPState sreg03,sreg02         ; store FPSCR  

        tst     sreg03, CHOP_OR_DOWN_BIT_MASK

        Return  ne

        tst     sreg03, UP_OR_NEAR_BIT_MASK 

        bne     d_round_add2            ; rounding is up: always add 1

#endif

; round to nearest

        tst     sreg12, #0x80000000     ; round bit is now the MSB of r12

        Return  eq                      ; done if round bit is clear

        movs    sreg12, sreg12, LSL #1  ; round up if sticky bits are non-0

        andeqs  sreg12, dxl, #0x1       ; in half-way case, round up if not already even

        Return  eq

        adds    dxl, dxl, #1

        adc     dxh, dxh, #0

        Return



;===============================================================================

; FUNCTION: _d_lltod

; DESCRIPTION: Convert signed double-word to double.

; INPUT:

;       llh/lll = signed long long word

; OUTPUT:

;       dxh/dxl = double

; USES:

;   sreg02 = sign

;       sreg03 = type of convert

;

	AREA |.text|,CODE,READONLY
	
_ll_sto_d                              ; FPLIB entry point

_d_lltod


        ands    sreg02, llh, #0x80000000  ; sign in sreg02

        beq     d_lltod_pos

; Negate.

        rsbs    lll, lll, #0

;;;        rsc     llh, llh, #0

	mov	sreg12, #0
	sbc	llh, sreg12, llh

d_lltod_pos

        cmp     llh, #0

        bne     dwtoX_hard_lltod        ; not easy 32-bit case

        movs    fx, lll

        b       __d_itod_common

dwtoX_hard_lltod

; at this point, condition codes set based on comparing llh to 0

        Normalize_num llh, sreg03, #32

;;;        orr     llh, llh, lll, LSR sreg03

	mov	sreg12, lll, LSR sreg03
	orr	llh, llh, sreg12

        rsb     sreg12, sreg03, #32

        mov     lll, lll, LSL sreg12

        add     sreg03, sreg03, #1024   ; bias exponent by 1023+31

        add     sreg03, sreg03, #30

        movs    sreg12, lll, LSL #21    ; keep roundind bits IN THE HIGH PART of sreg12

#if defined(SOFTFPA) && !defined(BIG_ENDIAN)

; swap the values of llh/lll in SOFTFPA, little endian mode

        eor     llh, llh, lll

        eor     lll, llh, lll

        eor     llh, llh, lll

; and then swap between llh/lll in pack to preserve registers

        mov     dxl, llh, LSR #11       ; align lowest 21 bits

        orr     dxl, dxl, lll, LSL #21  ; lowest 32 bits

        add     dxh, lll, lll           ; shift off msb (LSL #1)

        orr     dxh, sreg02, dxh, LSR #12  ; stuff sign

        orr     dxh, dxh, sreg03, LSL #20  ; stuff exponent

        Return  eq                      ; done (no sticky bits)

#else

        mov     dxl, lll, LSR #11       ; align lowest 21 bits

        orr     dxl, dxl, llh, LSL #21  ; lowest 32 bits

        add     dxh, llh, llh           ; shift off msb (LSL #1)

        orr     dxh, sreg02, dxh, LSR #12

        orr     dxh, dxh, sreg03, LSL #20  ; stuff exponent

        Return  eq                      ; done (no sticky bits)

#endif  

; here we have the inexact result packed in dxh/dxl

; must round and set the inexact bit in IEEE_FULL mode

; cases for rounding :

; ZERO - do nothing

; DOWN - substract 1 if number is negative

; UP - add 1 if number is positive

; NEAREST - the normal case

#ifdef IEEE_FULL

; @@@ Modified: added LFPState

;ldr       sreg02,=__SoftFPSCR__                ; load FPSCR

;ldr    sreg03,[sreg02]

        LFPState sreg03, sreg02

        SetInexactL sreg03, __wrap_inexact_handler_lltod  ; set inexact

        StFPState sreg03,sreg02         ; store FPSCR  

        tst     sreg03, CHOP_OR_DOWN_BIT_MASK

        bne     d_chop_or_down_round

        tst     sreg03, UP_OR_NEAR_BIT_MASK 

        bne     d_up_round

#endif

; round to nearest

        tst     sreg12, #0x80000000     ; round bit is now the MSB of r12

        Return  eq                      ; done if round bit is clear

        movs    sreg12, sreg12, LSL #1  ; round up if sticky bits are non-0

        andeqs  sreg12, dxl, #0x1       ; in half-way case, round up if not already even

        Return  eq

        adds    dxl, dxl, #1

        adc     dxh, dxh, #0

        Return



#ifdef IEEE_FULL

f_round_add2

        add     fx, fx, #1

        Return



chop_or_down_round

        tst     sreg03, ZERO_OR_DOWN_BIT_MASK

        addeq   fx, fx, fx, LSR #31     ; downward: add sign

        Return



up_round

        cmp     fx, #0

        addpl   fx, fx, #1              ; add 1 only if number is positive

        Return



d_round_add2

        adds    dxl, dxl, #1

        adc     dxh, dxh, #0

        Return



d_chop_or_down_round

        tst     sreg03, ZERO_OR_DOWN_BIT_MASK

        Return  ne

        adds    dxl, dxl, dxh, LSR #31  ; downward: add sign

        adc     dxh, dxh, #0

        Return



d_up_round

        cmp     dxh, #0

        Return  mi

        adds    dxl, dxl, #1

        adc     dxh, dxh, #0

        Return



#endif



#ifdef IEEE_FULL

	IMPORT __fp_handler_32_to_32

	IMPORT __fp_handler_64_to_64



; inexact exception wrappers

__wrap_inexact_handler_ulltof

        stmfd   sp!, {lr}

        ldr     r1, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_ULONGLONG :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_CVT)

        bl      __fp_handler_32_to_32

        Return_lr



__wrap_inexact_handler_lltof

        stmfd   sp!, {lr}

        ldr     r1, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_LONGLONG :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_CVT)

        bl      __fp_handler_32_to_32

        Return_lr



__wrap_inexact_handler_ulltod

        stmfd   sp!, {lr}

        ldr     r2, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_ULONGLONG :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_CVT)

        bl      __fp_handler_64_to_64

        Return_lr



__wrap_inexact_handler_lltod

        stmfd   sp!, {lr}

        ldr     r2, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_LONGLONG :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_CVT)

        bl      __fp_handler_64_to_64

        Return_lr

#endif
	END

/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */
