;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _f_itof.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



;===============================================================================



	EXPORT _f_itof

	EXPORT _fflt                   ; FPLIB for _f_itof

	EXPORT _f_utof

	EXPORT _ffltu                  ; FPLIB for _f_utof



	EXPORT __f_utof_common

	EXPORT __f_itof_common



	#ifdef VFP_AND_NOT_IEEE_STD

	EXPORT _vfp_support_fflt

	EXPORT _vfp_support_ffltu

#endif



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

; @@@ Modified: optimized functions

;===============================================================================



;===============================================================================

; STRATEGY: Converting integral types to floating types requires alignment and

; then coercion.  Use common alignment strategies for the 32-bit types and

; the 64-bit types, then before coercing check for the required rounding

; width and exponent bias.

; These flags are set in the bottom of yexp for dispatching.



;===============================================================================

; STRATEGY: The conversions mix 32- and 64-bit operands freely, so care must

; be taken with respect to 32- vs. 64-bit operations.  In 64-bit mode, wide

; operands enter and exit in a single register and narrow operands must be

; widened within a single register.  In 32-bit mode, wide operands enter and

; exit in a pair of regs.



;===============================================================================

; FUNCTION: _f_utof

; DESCRIPTION: Convert unsigned word to float.

; INPUT:

;       fx = unsigned word

; OUTPUT:

;       fx = float

; OPTIMIZATION: Uses signed conversion, with faked positive sign.

;

	AREA |.text|,CODE,READONLY
	
#ifdef VFP

_ffltu

_f_utof


        fmsr    s0, r0

        fuitos  s0, s0

        fmrs    r0, s0

        Return


	AREA |.text|,CODE,READONLY
	
_vfp_support_ffltu

#else

_ffltu

_f_utof


#endif



	#ifdef NOT_VFP_OR_NOT_IEEE_STD



        cmp     fx, #0                  ; return 0 if int is 0

__f_utof_common

        Return  eq

        mov     sreg03, #(31+127)       ; normalized exp if it is a float       

        bmi     f_coerce_int32_utof     ; already normalized

        Normalize_a5 fx, sreg03, sreg12

f_coerce_int32_utof

        ands    sreg02, fx, #0xff       ; keep rounding bits in sreg02

        add     fx, fx, fx              ; clear msb of significand

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

        SetInexactL sreg03, __wrap_inexact_handler_utof  ; set inexact

        StFPState sreg03,sreg01         ; store FPSCR  

        tst     sreg03, CHOP_OR_DOWN_BIT_MASK

        Return  ne

        tst     sreg03, UP_OR_NEAR_BIT_MASK 

        bne     f_round_add2            ; rounding is up: always add 1

  #endif

; round to nearest

        tst     sreg02, #0x0080         ; done if round bit is clear

        Return  eq

        ands    sreg01, sreg02, #0x7f   ; round up if sticky bits are non-0

        andeqs  sreg01, fx, #0x1        ; in half-way case, round up if not already even

        addne   fx, fx, #1

        Return

#endif



;===============================================================================

; FUNCTION: _f_itof

; DESCRIPTION: Convert signed word to float.

; INPUT:

;       fx = signed word

; OUTPUT:

;       fx = float

; CASES: Zero maps to +0. Large values are rounded according to the current mode.

	AREA |.text|,CODE,READONLY
	
#ifdef VFP

_fflt

_f_itof


        fmsr    s0, r0

        fsitos  s0, s0

        fmrs    r0, s0

        Return


	
_vfp_support_fflt

#else

_fflt
	
_f_itof

#endif



	#ifdef NOT_VFP_OR_NOT_IEEE_STD



        ands    sreg02, fx, #0x80000000  ; test sign bit                                  

        rsbmi   fx, fx, #0              ; neg value if negative

        cmp     fx, #0 

__f_itof_common

        Return  eq 

        Normalize_num fx, sreg03, #(31+127)

        ands    sreg01, fx, #0xff       ; keep rounding bits in sreg01

        add     fx, fx, fx              ; clear msb of significand

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

;ldr       sreg12,=__SoftFPSCR__                ; load FPSCR

;ldr    sreg03,[sreg12]

        LFPState sreg03, sreg12

        SetInexactL sreg03, __wrap_inexact_handler_itof  ; set inexact

        StFPState sreg03,sreg12         ; store FPSCR  

        tst     sreg03, CHOP_OR_DOWN_BIT_MASK

        bne     chop_or_down_round

        tst     sreg03, UP_OR_NEAR_BIT_MASK 

        bne     up_round                ; rounding is up

  #endif

; round to nearest

        tst     sreg01, #0x0080         ; done if round bit is clear

        Return  eq

        ands    sreg03, sreg01, #0x7f   ; round up if sticky bits are non-0

        andeqs  sreg03, fx, #0x1        ; in half-way case, round up if not already even

        addne   fx, fx, #1

        Return



  #ifdef IEEE_FULL

f_round_add2

        add     fx, fx, #1

        Return



chop_or_down_round

        tst     sreg03, ZERO_OR_DOWN_BIT_MASK

        addeq   fx, fx, sreg02, LSR #31  ; downward: substract sign

        Return



up_round

        cmp     sreg02, #0

        addpl   fx, fx, #1              ; add 1 only if sign is positive

        Return



  #endif  



  #ifdef IEEE_FULL

	IMPORT __fp_handler_32_to_32



; inexact exception wrappers

__wrap_inexact_handler_utof

        stmfd   sp!, {lr}

        ldr     r1, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_UINT :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_CVT)

        bl      __fp_handler_32_to_32

        Return_lr



; inexact exception wrappers

__wrap_inexact_handler_itof

        stmfd   sp!, {lr}

        ldr     r1, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_INT :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_CVT)

        bl      __fp_handler_32_to_32

        Return_lr

  #endif

#endif
	END

/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */
