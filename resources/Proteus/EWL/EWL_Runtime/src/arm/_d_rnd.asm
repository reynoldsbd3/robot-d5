;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _d_rnd.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



;===============================================================================


	EXPORT _drnd


;===============================================================================

; FUNCTION: _drnd

; DESCRIPTION: IEEE754 round-to-integer operation. The function takes a number 

;              and rounds it to an integer (in accordance with the current 

;              rounding mode), but returns that integer in the floating-point 

;              number format rather than as a C int variable.

; INPUT:

;       dxh/dxl = double

; OUTPUT:

;       dxh/dxl = double



	#ifdef IEEE_FULL_OR_FIXED

	MACRO
	DRND_SAVE

        stmfd   sp!,{lr}

	MEND



	MACRO
	DRND_RESTORE

        ldmfd   sp!,{lr}

	MEND

#endif



#ifdef THUMB

	MACRO
	DRND_RETURN $cc

        bx$cc  lr

	MEND

#else

	MACRO
	DRND_RETURN $cc

        mov$cc pc,lr

	MEND

#endif


	AREA |.text|,CODE,READONLY
_drnd

        ldr     sreg03,=0x3ff00000

        bic     sreg02,dxh,#0x80000000  ; sreg02 = abs. value of dxh

        subs    sreg03,sreg02,sreg03

        blt.w     drnd_zero

        mov     sreg03,sreg03,LSR #20   ; sreg03 = real.exp

        cmp     sreg03,#52              ; exp >= 52 ? already an integer (or inf/nan)

        bge.w     drnd_large

        cmp     sreg03,#20

        blt     drnd_clear_hi           ; have to clear bits in both words

        beq.w     drnd_clear_lo           ; clear lower word, round higher word



        sub     sreg03,sreg03,#20       ; sreg03 = number of shifts

        mvn     sreg02,#0               ; sreg02 = mask the bits to be cleared

        mov     sreg02,sreg02,LSR sreg03

        ands    sreg03,dxl,sreg02       ; sreg03 = rounding bits

        bic     dxl,dxl,sreg02

        DRND_RETURN eq                  ; no rounding? return



#ifdef IEEE_FULL

        DRND_SAVE

        LFPState lr, sreg12

        SetInexact lr

        StFPState lr,sreg12 

#endif  

        add     sreg12,sreg02,#1        ; r12 = mask + 1 - use it in rounding

#ifdef IEEE_FULL

        tst     lr,CHOP_OR_DOWN_BIT_MASK

        bne     drnd_down

        tst     lr,UP_OR_NEAR_BIT_MASK

        bne     drnd_up

        DRND_RESTORE

#endif

        tst     sreg03,sreg12,LSR #1    ; check rounding bit

        DRND_RETURN eq                  ; 0 ? quit

        tst     sreg03,sreg02,LSR #1    ; check sticky bits

        tsteq   dxl,sreg12              ; check LSB of the integer part

        moveq   sreg12,#0

        adds    dxl,dxl,sreg12          ; return dxh/dxl + sign

        adc     dxh,dxh,#0 

        DRND_RETURN



#ifdef IEEE_FULL

drnd_up

        tst     dxh,#0x80000000

        movne   sreg12,#0

        adds    dxl,dxl,sreg12          ; return dxh/dxl + sign

        adc     dxh,dxh,#0

        DRND_RESTORE

        DRND_RETURN



drnd_down

        tst     lr,ZERO_OR_DOWN_BIT_MASK

        bne     drnd_return             ; chop? return dxh/dxl

        tst     dxh,#0x80000000

        moveq   sreg12,#0

        adds    dxl,dxl,sreg12          ; return dxh/dxl + sign

        adc     dxh,dxh,#0

drnd_return

        DRND_RESTORE

        DRND_RETURN

#endif  





; clear low word and masked bits in high word



drnd_clear_hi

        ldr     sreg02,=0x000fffff      ; sreg02 = mask of bits to clear

        mov     sreg02,sreg02,LSR sreg03

#ifdef IEEE_FULL

        tst     dxh,sreg02              ; test rounding bits from hi word

        cmpeq   dxl,#0                  ; test also low word

        beq     drnd_clear_hi_return    ; no rounding? return

#endif



        add     sreg03,sreg02,#1        ; sreg03 = mask + 1 - use it in rounding



#ifdef IEEE_FULL

        DRND_SAVE

        LFPState lr, sreg12

        SetInexactL lr,__wrap_inexact_handler_clear_high

        StFPState lr,sreg12 



        tst     lr,CHOP_OR_DOWN_BIT_MASK

        bne     drnd_clear_hi_down

        tst     lr,UP_OR_NEAR_BIT_MASK

        DRND_RESTORE

        bne     drnd_clear_hi_up

#endif

        tst     dxh,sreg03,LSR #1       ; check rounding bit

        beq     drnd_clear_hi_return    ; 0 ? quit

        tst     dxh,sreg02,LSR #1       ; check sticky bits

        cmpeq   dxl,#0

        tsteq   dxh,sreg03              ; check LSB of the integer part

        addnes  dxh,dxh,sreg03          ; return dxh/dxl + sign

; TRICK: 1.xxx is represented as 0x3ffxxxxx. The LSB check tests bit #20, 

; which is out of the significant, but it is 1, so the code 

; works for this case too



drnd_clear_hi_return

        bic     dxh,dxh,sreg02

        mov     dxl,#0

        DRND_RETURN



#ifdef IEEE_FULL

drnd_clear_hi_up

        tst     dxh,#0x80000000

        addeq   dxh,dxh,sreg03          ; return dxh/dxl + !sign

        bic     dxh,dxh,sreg02

        mov     dxl,#0

        DRND_RETURN



drnd_clear_hi_down

        tst     lr,ZERO_OR_DOWN_BIT_MASK

        DRND_RESTORE

        bne     drnd_clear_hi_return    ; chop? return dxh/dxl

        tst     dxh,#0x80000000

        addne   dxh,dxh,sreg03          ; return dxh/dxl + sign

        bic     dxh,dxh,sreg02

        mov     dxl,#0

        DRND_RETURN

#endif



; clear low word, keep high word



drnd_clear_lo

        movs    sreg02,dxl              ; save rounding bits

        mov     dxl,#0                  ; clear low word

#ifdef IEEE_FULL

        DRND_RETURN eq                  ; no rounding? return



        DRND_SAVE

        LFPState lr, sreg12

        SetInexact lr

        StFPState lr,sreg12 



        tst     lr,CHOP_OR_DOWN_BIT_MASK

        bne     drnd_clear_lo_down

        tst     lr,UP_OR_NEAR_BIT_MASK

        DRND_RESTORE

        bne     drnd_clear_lo_up

#endif

        tst     sreg02,#0x80000000      ; check rounding bit

        DRND_RETURN eq                  ; 0 ? quit

        movs    sreg02,sreg02,LSL #1    ; check sticky bits

        tsteq   dxh,#1                  ; check LSB of the integer part

        addnes  dxh,dxh,#1              ; return dxh/dxl + sign

        DRND_RETURN



#ifdef IEEE_FULL

drnd_clear_lo_up

        tst     dxh,#0x80000000

        addeq   dxh,dxh,#1              ; return dxh/dxl + !sign

        DRND_RETURN



drnd_clear_lo_down

        tst     lr,ZERO_OR_DOWN_BIT_MASK

        DRND_RESTORE

        DRND_RETURN ne                  ; chop? return dxh/dxl

        tst     dxh,#0x80000000

        addne   dxh,dxh,#1              ; return dxh/dxl + sign

        DRND_RETURN

#endif



; real exponent is <= 0 ; sreg02 = abs. value of dxh; sreg03 = sreg02 - 0x3ff00000



drnd_zero

#ifdef IEEE_FULL

        orrs    sreg02,sreg02,dxl       ; is dxh/dxl a true zero? return

        DRND_RETURN eq

#endif

        mov     sreg12,dxl

        and     dxh,dxh,#0x80000000

        mov     dxl,#0

#ifdef IEEE_FULL

        DRND_SAVE

        LFPState sreg02, lr

        SetInexact sreg02

        StFPState sreg02,lr

        DRND_RESTORE



        tst     sreg02,CHOP_OR_DOWN_BIT_MASK

        bne     drnd_zero_down

        tst     sreg02,UP_OR_NEAR_BIT_MASK

        bne     drnd_zero_up

#endif  

        cmn     sreg03,#0x00100000

        cmpeq   sreg12,#0               ; if abs(dxh/dxl) > 0x3fe00000....

        ldrhi   sreg02,=0x3ff00000 

        orrhi   dxh,dxh,sreg02          ; a number between 0.5 and 1 rounds to 1

        DRND_RETURN



#ifdef IEEE_FULL

drnd_zero_down

        tst     sreg02,ZERO_OR_DOWN_BIT_MASK

        DRND_RETURN ne                  ; chop? return 0.0

        tst     dxh,#0x80000000

        ldrne   sreg02,=0x3ff00000 

        orrne   dxh,dxh,sreg02          ; return sign ? -1.0 : 0.0

        DRND_RETURN



drnd_zero_up

        tst     dxh,#0x80000000         ; round to + inf

        ldreq   sreg02,=0x3ff00000 

        orreq   dxh,dxh,sreg02          ; return sign ? 1.0 : -0.0

        DRND_RETURN

#endif  



; real exponent is >=52 ; sreg02 = abs. value of dxh



drnd_large

        ldr     sreg03,=0x7FF00000

        cmp     sreg02,sreg03           ; is it a NaN?

        cmpeq   dxl,#0

	#ifdef IEEE_FULL_OR_FIXED

        DRND_RETURN ls                  ; no? return input



        tst     dxh,#0x00080000         ; is it a signaling NaN?

        DRND_RETURN ne                  ; no? return



        LFPState sreg03, sreg02

        SetInvalidE sreg03

        StFPState sreg03,sreg02 

        orr     dxh,dxh,#0x00080000     ; quiet the NaN

#else

        orrhi   dxh,dxh,#0x00080000     ; quiet the NaN

#endif

        DRND_RETURN



	#ifdef IEEE_FULL_OR_FIXED

	IMPORT __fp_handler_64_to_64



; invalid exception handler

__wrap_invalid_handler

        ldr     r2, =(FE_EX_INVALID :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_RND)

        DRND_SAVE

        bl      __fp_handler_64_to_64

        DRND_RESTORE

        DRND_RETURN

#endif



#ifdef IEEE_FULL

; inexact exception wrappers

__wrap_inexact_handler_clear_high

        bic     dxh,dxh,sreg02          ; r2 - mask of bits to be cleared

        mov     dxl,#0

__wrap_inexact_handler

        ldr     r2, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_RND)

        bl      __fp_handler_64_to_64

        DRND_RESTORE

        DRND_RETURN

#endif
	END


/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */

