;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _d_sub.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



	EXPORT _d_sub

	EXPORT _dsub                   ; FPLIB for _d_sub

	EXPORT _drsb



	EXPORT __dsub_start

	IMPORT __dadd_start



	#ifdef VFP_AND_NOT_IEEE_STD

	EXPORT _vfp_support_dsub

#endif        



	#ifdef IEEE_FULL_OR_FIXED

#ifdef USING_ARMASM
#define Clrstk(x) CLRSTK #x 
#else
#define Clrstk(x) CLRSTK ##x
#endif

	MACRO
	Prolog

        stmfd   sp!, {r4, r5, r6, sreg07, dxh, dyh, dxl, dyl}

	MEND

#else

#define Clrstk(x)  

	MACRO
	Prolog

        stmfd   sp!, {r4, r5, r6, sreg07}

	MEND

#endif  



; && !defined(ARM5)
#if defined(THUMB)

	MACRO
	Retcc

        Clrsstk(cc)

        ldmccfd sp!, {r4, r5, r6, sreg07}

        bxcc    lr

	MEND

	MACRO
	Reteq

        Clrstk(eq)

        ldmeqfd sp!, {r4, r5, r6, sreg07}

        bxeq    lr

	MEND

	MACRO
	Retne

        Clrstk(ne)

        ldmnefd sp!, {r4, r5, r6, sreg07}

        bxne    lr

	MEND

	MACRO
	Return

        Clrstk(al)

        ldmfd   sp!, {r4, r5, r6, sreg07}

        bx      lr

	MEND

	MACRO
	Retmi

        Clrstk(mi)

        ldmmifd sp!, {r4, r5, r6, sreg07}

        bxmi    lr 

	MEND

	MACRO
	Ret_restore

        ldmfd   sp!, {r4, r5, r6, sreg07}

        bx      lr

	MEND

#else

	MACRO
	Retcc

        Clrstk(cc)

        ldmccfd sp!, {r4, r5, r6, pc}

	MEND

	MACRO
	Reteq

        Clrstk(eq)

        ldmeqfd sp!, {r4, r5, r6, pc}

	MEND

	MACRO
	Retne

        Clrstk(ne)

        ldmnefd sp!, {r4, r5, r6, pc}

	MEND

	MACRO
	Return

        Clrstk(al)

        ldmfd   sp!, {r4, r5, r6, pc}

	MEND

	MACRO
	Retmi

        Clrstk(mi)

        ldmmifd sp!, {r4, r5, r6, pc}

	MEND

	MACRO
	Ret_restore

        ldmfd   sp!, {r4, r5, r6, pc}

	MEND

#endif 


;===============================================================================

; FUNCTION: double _drsb (double, double)

; DESCRIPTION: Reversed double substraction.

; INPUT: 

; x=dxh/dxl,

; y=dyh/dyl, doubles

; OUTPUT: 

; dxh/dxl = result



;===============================================================================

; FUNCTION: double _d_sub (double, double)

; DESCRIPTION: Double substraction.

; INPUT: 

; x=dxh/dxl,

; y=dyh/dyl, doubles

; OUTPUT: 

; dxh/dxl = result

	AREA |.text|,CODE,READONLY

#ifdef VFP

_dsub

_d_sub


        fmdhr   d0, dxh

        fmdlr   d0, dxl

        fmdhr   d1, dyh

        fmdlr   d1, dyl

        fsubd   d0, d0, d1

        fmrdh   dxh, d0

        fmrdl   dxl, d0

  #ifdef THUMB        

        bx      lr

  #else        

        mov     pc,lr

  #endif        



_drsb

        fmdhr   d0, dxh

        fmdlr   d0, dxl

        fmdhr   d1, dyh

        fmdlr   d1, dyl

        fsubd   d0, d1, d0

        fmrdh   dxh, d0

        fmrdl   dxl, d0

  #ifdef THUMB        

        bx      lr

  #else        

        mov     pc,lr

  #endif        



_vfp_support_dsub

#else

_drsb

; swap operands

        eor     dxh, dxh, dyh

        eor     dyh, dxh, dyh

        eor     dxh, dxh, dyh

        eor     dxl, dxl, dyl

        eor     dyl, dxl, dyl

        eor     dxl, dxl, dyl

; and fall through _d_sub

_dsub									; FPLIB entry point

_d_sub

#endif



	#ifdef NOT_VFP_OR_NOT_IEEE_STD



        Prolog

        eors    sreg12, dxh, dyh

        eormi   dyh, dyh, #0x80000000

        bmi.w     __dadd_start            ; same sign, this is actually an addition

__dsub_start

; first make sure that the highest number is in dxh/dxl

        subs    sreg12, dxl, dyl

        sbcs    sreg07, dxh, dyh

        bcs     dsub_unpack

; must swap numbers

        eor     sreg07, sreg07, #0x80000000     ; change sign

        adds    dyl, dyl, sreg12

        adc     dyh, dyh, sreg07

        subs    dxl, dxl, sreg12

        sbc     dxh, dxh, sreg07 

dsub_unpack

        mov     sreg07, #0x80000000         ; will be used later

; unpack x in in sreg12(exp) and dxh/dxl(significand)  

        mov     sreg12, dxh, LSR #20    ; sreg12 has x exp and sign

        orr     dxh, sreg07, dxh, LSL #11   ; left-align 53 sig bits

        orr     dxh, dxh, dxl, LSR #21  ; all lead bits but msb is low exp

        mov     dxl, dxl, LSL #11       ; low 21 bits

        movs    sreg04, sreg12, LSL #21  ; check exponent

        cmnne   sreg04, #0x00200000     ; test for huge exponent

        beq.w    dsub_x_special_case     ; special case: tiny or huge

; unpack fy in sreg04(exp) and dyh/dyl(significand)  

        mov     sreg04, dyh, LSR #20    ; sreg04 has x exp and sign

        orr     dyh, sreg07, dyh, LSL #11   ; left-align 53 sig bits

        orr     dyh, dyh, dyl, LSR #21  ; all lead bits but msb is low exp

        mov     dyl, dyl, LSL #11       ; low 21 bits

        movs    sreg07, sreg04, LSL #21     ; check exponent

  #ifdef IEEE_STD

        beq.w     dsub_result_x_std       ; x if denorm

  #else

        beq.w     dsub_y_special_case     ; special case: tiny or huge

  #endif

; at this point: x is in dxh/dxl(significand) and sreg12(exp)

; y is in dyh/dyl(significand) and sreg04(exp)

; sreg07 has sign of x

dsub_start

        subs    sreg04, sreg12, sreg04

        beq.w     dsub_aligned            ; sub significands for equal exponents

        cmp     sreg04, #32

        ble     dsub_small_shift        ; small shift

        cmp     sreg04, #56

        movge   sreg04, #63

        sub     sreg04, sreg04, #32     ; exp = exp - 32

        rsb     sreg07, sreg04, #32         ; count for stickies

;;;        orrs    sreg07, dyl, dyh, LSL sreg07    ; collect stickies

	mov	sreg07, dyh, LSL sreg07
	orrs	sreg07, dyl, sreg07

        mov     dyl, dyh, LSR sreg04    ; the actual shift

        orrne   dyl, dyl, #1            ; sticky

        subs    dxl, dxl, dyl

        sbcs    dxh, dxh, #0

        bmi     dsub_coerce

        b       dsub_normalize

dsub_small_shift

        rsb     sreg07, sreg04, #32         ; need 32-shift for left shift  

        movs    sreg07, dyl, LSL sreg07         ; stickies in flags

        rsb     sreg07, sreg04, #32         ; need 32-shift for left shift  

        mov     dyl, dyl, LSR sreg04    ; shift

;;;        orr     dyl, dyl, dyh, LSL sreg07
	
	mov	sreg07, dyh, LSL sreg07
	orr	dyl, dyl, sreg07
	

        mov     dyh, dyh, LSR sreg04 

        orrne   dyl, dyl, #1            ; sticky        

        subs    dxl, dxl, dyl

        sbcs    dxh, dxh, dyh

        bpl.w     dsub_normalize

dsub_coerce

; normal case: pack 

; sreg12 has exponent and sign

; dxl/dxh has the significand

        movs    sreg02, dxl, LSL #21    ; keep round bits in high part of sreg02  

        mov     dxl, dxl, LSR #11       ; align lowest 21 bits

        orr     dxl, dxl, dxh, LSL #21  ; lowest 32 bits

        add     dxh, dxh, dxh           ; shift off msb (LSL #1)

        mov     dxh, dxh, LSR #12       ; high part of significand

        orr     dxh, dxh, sreg12, LSL #20  ; stuff exponent

  #ifdef IEEE_FULL  

        Reteq                           ; no round bits: just return

; rounding must be done here

; here we have the inexact result packed in dxh/dxl

; sreg02 contains all the rounding bits

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

        tst     sreg02, #0x80000000     ; round bit is now the MSB of r12

        Reteq                           ; done if round bit is clear

        movs    sreg02, sreg02, LSL #1  ; round up if sticky bits are non-0

        andeqs  sreg02, dxl, #0x1       ; in half-way case, round up if not already even

        Reteq

        adds    dxl, dxl, #1

        adc     dxh, dxh, #0

        Return

  #ifdef IEEE_FULL

up_round

        cmp     dxh, #0 

        Retmi

        adds    dxl, dxl, #1

        adc     dxh, dxh, #0

        Return



chop_or_down_round

        tst     sreg03, ZERO_OR_DOWN_BIT_MASK

        Retne

        adds    dxl, dxl, dxh, LSR #31  ; just add sign for rounding down

        adc     dxh, dxh, #0

        Return

  #endif                                   
; #ifdef IEEE_FULL



; arrive here when exponents are equal

; dxh/dxl has x significand, dyh/dyl has y significand

; sreg12 has final exponent and sign

dsub_aligned

        subs    dxl, dxl, dyl

        sbc     dxh, dxh, dyh

        orrs    sreg07, dxh, dxl            ; are both zero?

        beq.w    __d_result_signed_zero  ; return 0 if equal

; Renormalize exact result.  Go ahead and shift till normal,

; even if tiny results will require shift back to exp=1.

        mov     sreg07, sreg12, LSL #20

        and     sreg07, sreg07, #0x80000000     ; sign in sreg07

        bic     sreg12, sreg12, #0x800  ; clear sign from exp           

        cmp     dxh, #0

        bmi     dsub_aligned_cont_check  ; already normalized?  

        bne     dsub_renorm_cont

        sub     sreg12, sreg12, #32

        movs    dxh, dxl

        mov     dxl, #0

        bmi     dsub_renorm_check

dsub_renorm_cont

        Normalize_num dxh, sreg04, #32

;;;        orr     dxh, dxh, dxl, LSR sreg04

	mov	sreg05, dxl, LSR sreg04	
	orr	dxh, dxh, sreg05

        rsb     sreg04, sreg04, #32

        mov     dxl, dxl, LSL sreg04

        sub     sreg12, sreg12, sreg04

dsub_renorm_check

        cmp     sreg12, #0

        bgt     dsub_aligned_cont

; exponent is <= 0, so shift

; dxh/dxl (integer)

; sreg12 has exponent

; sreg07 has sign     

  #ifdef IEEE_STD

        mov     dxh, #0                 ; just return 0 in IEEE_STD mode

        mov     dxl, #0

        Return

  #else

        rsb     sreg12, sreg12, #12     ; shift a total of 12-sreg12 positions

        cmp     sreg12, #32

        movge   dxl, dxh

        movge   dxh, #0

        subge   sreg12, sreg12, #32     ; huge shift

        rsb     sreg04, sreg12, #32     ; compute 32 - shift ammount

        mov     dxl, dxl, LSR sreg12

;;;        orr     dxl, dxl, dxh, LSL sreg04
	mov	sreg05,dxh,LSL sreg04
	orr	dxl, dxl, sreg05

;;;        orr     dxh, sreg07, dxh, LSR sreg12  ; final shift & stuff sign
	mov	sreg05, dxh, LSR sreg12
	orr	dxh, sreg07, sreg05

        Return                          ; result is always exact

  #endif

dsub_aligned_cont_check

        cmp     dxh, #0                 ; test lead bit

        subges  sreg12, sreg12, #1      ; sub 1 for denormals

  #ifdef IEEE_STD

        moveq   dxh, #0                 ; return 0 in IEEE_STD mode

        moveq   dxl, #0

        Reteq

  #endif  

dsub_aligned_cont

        mov     dxl, dxl, LSR #11       ; align lowest 21 bits

        orr     dxl, dxl, dxh, LSL #21  ; lowest 32 bits

        add     dxh, dxh, dxh           ; shift off msb (LSL #1)

        orr     dxh, sreg07, dxh, LSR #12   ; stuff sign

        orr     dxh, dxh, sreg12, LSL #20  ; stuff exponent

        Return                          ; no round bits: just return



; arrive here when significand must be normalized

; dxh/dxl has x significand, dyh/dyl has y significand

; sreg12 has final exponent and sign

dsub_normalize

        mov     sreg07, sreg12, LSL #20

        and     sreg07, sreg07, #0x80000000     ; sign in sreg07

        bic     sreg12, sreg12, #0x800  ; clear sign from exp             

        cmp     dxh, #0

        bne     dsub_renorm_cont_1

        sub     sreg12, sreg12, #32

        movs    dxh, dxl

        mov     dxl, #0

        bmi     dsub_renorm_check_1

dsub_renorm_cont_1

        Normalize_num dxh, sreg04, #32

;;;        orr     dxh, dxh, dxl, LSR sreg04
	mov	sreg05, dxl, LSR sreg04
	orr	dxh, dxh, sreg05

        rsb     sreg04, sreg04, #32

        mov     dxl, dxl, LSL sreg04

        sub     sreg12, sreg12, sreg04

dsub_renorm_check_1

        cmp     sreg12, #0

        orrgt   sreg12, sreg12, sreg07, LSR #20  ; put sign back in sreg12

        bgt     dsub_coerce

  #ifdef IEEE_STD

        mov     dxh, #0                 ; just return 0 in IEEE_STD mode

        mov     dxl, #0

        Return

  #else

        rsb     sreg12, sreg12, #12     ; shift a total of 12-sreg12 positions

        cmp     sreg12, #32

        movge   dxl, dxh

        movge   dxh, #0

        subge   sreg12, sreg12, #32     ; huge shift

        rsb     sreg04, sreg12, #32     ; compute 32 - shift ammount

        mov     dxl, dxl, LSR sreg12

;;;        orr     dxl, dxl, dxh, LSL sreg04
	mov	sreg05, dxh, LSL sreg04
	orr	dxl, dxl, sreg05

;;;        orr     dxh, sreg07, dxh, LSR sreg12  ; final shift & stuff sign
	mov	sreg05, dxh, LSR sreg12
	orr	dxh, sreg07, sreg05

        Return                          ; result is exact

  #endif



; ============================================================================

; The code for special cases begins here

; ============================================================================



; arrive here if x is either tiny or huge

; dxh/dxl has x significand and lower bit of exp, sreg12 has x exponent and sign

dsub_x_special_case

        cmp     sreg12, #0x800

        movge   sreg07, #0x80000000

        movlt   sreg07, #0x0                ; sign in sreg07

        bics    sreg12, sreg12, #0x0800  ; clear sign bit from exp

  #ifdef IEEE_STD

        beq     dsub_result_y           ; result is -y

  #else

        beq     dsub_x_tiny             ; x is tiny

  #endif

; otherwise fall through dsub_x_huge



; arrive here if x exp is 0x7ff

; sreg07 has x sign

; dxh/dxl has significand of x and lower bit of exp

dsub_x_huge

        orrs    sreg04, dxl, dxh, LSL #1

        bne.w     __d_result_x_NaN        ; check x and return a QNaN

; x is INF 

; must unpack y

; unpack fy in dyh/dyl(significand) and sreg04(exponent)  

        mov     sreg04, dyh, LSR #20    ; sreg04 has x exp and sign

        mov     dyh, dyh, LSL #11       ; left-align 53 sig bits

        orr     dyh, dyh, dyl, LSR #21  ; all lead bits 

        mov     dyl, dyl, LSL #11       ; low 21 bits

        movs    sreg04, sreg04, LSL #21  ; check exponent

        beq     __d_result_INF          ; INF - 0 = INF - denorm = INF

        cmn     sreg04, #0x00200000

        bne     __d_result_INF          ; INF - norm = INF

; if we arrive here x is INF, y is either INF, NaN

        orrs    sreg04, dyl, dyh, LSL #1

        beq.w     __d_result_invalid      ; INF - INF = invalid

        b       dsub_y_NaN              ; INF - NaN = QNaN, must check y



  #ifndef IEEE_STD

; arrive here if y is either tiny or huge

; dxh/dxl has x significand and lower bit of exp, sreg12 has x exponent 

; dyh/dyl has y significand and lower bit of exp, sreg04 has y exponent

dsub_y_special_case

        cmp     sreg04, #0x800

        movge   sreg07, #0x80000000

        movlt   sreg07, #0x0                ; sign in sreg07

        bic     sreg12, sreg12, #0x800  ; clear sign bit  

        bics    sreg04, sreg04, #0x0800  ; clear sign bit

        beq     dsub_y_tiny             ; y is tiny

; otherwise fall through dsub_y_huge



; arrive here if y exp is 0xff AND x is a normal number

; sreg07 has the sign of x

; sreg12 has x exp, dxh/dxl has x significand and lead bit

; sreg04 has y exp, dyh/dyl has y significand and lower bit of exp

dsub_y_huge

        orrs    sreg04, dyl, dyh, LSL #1

        bne     dsub_y_NaN              ; check y and return a QNaN

; y is INF, x is a normal number, so return INF

        b       __d_result_INF



; arrive here if x exp is 0x00 

; sreg07 has the sign of x

; sreg12 has x exp, dxh/dxl has x significand and lower bit of exp

dsub_x_tiny

        orrs    sreg04, dxl, dxh, LSL #1  ; check significand

        beq     dsub_x_zero             ; x is zero

; x is a denorm; don't need to normalize, just set exp

        mov     sreg12, #1              ; true exp for denorms

        bic     dxh, dxh, #0x80000000   ; clear lead bit

; unpack fy in sreg04(exp) and dyh/dyl(significand)  

        mov     sreg04, dyh, LSR #20    ; sreg04 has x exp and sign

        mov     dyh, dyh, LSL #11       ; left-align 53 sig bits

        orr     dyh, dyh, dyl, LSR #21  ; all lead bits but msb is low exp

        mov     dyl, dyl, LSL #11       ; low 21 bits

        movs    sreg04, sreg04, LSL #21  ; check exponent

        cmnne   sreg04, #0x00200000     ; test for huge exponent

        mov     sreg04, sreg04, LSR #21

        orr     sreg04, sreg04, sreg07, LSR #20  ; put sign and exp back

        beq     dsub_y_special_case     ; special case: tiny or huge

        orr     dyh, dyh, #0x80000000   ; set lead bit 

        orr     sreg12, sreg12, sreg07, LSR #20  ; sign back in exp

        b       dsub_start              ; common code

  #endif 

        

dsub_x_zero

; unpack y in dxh/dxl(significand) and sreg12(exp)

        mov     sreg12, dyh, LSR #20    ; sreg04 has x exp and sign

        mov     dxh, dyh, LSL #11       ; left-align 53 sig bits

        orr     dxh, dxh, dyl, LSR #21  ; all lead bits 

        mov     dxl, dyl, LSL #11       ; low 21 bits

        movs    sreg04, sreg12, LSL #21  ; check exponent

        beq     dsub_y_denorm_zero      ; y is 0 or denorm

        cmn     sreg04, #0x00200000

        bne     dsub_result_y           ; 0 - norm = -y

        orrs    sreg04, dxl, dxh, LSL #1

        bne     __d_result_NaN          ; y is a NaN

        b       __d_result_INF          ; return INF

dsub_y_denorm_zero

        orrs    sreg04, dxl, dxh, LSL #1

        beq     __d_result_signed_zero  ; 0 - 0, must check for rounding mode

        b       dsub_result_y           ; result is -y  



  #ifndef IEEE_STD

; arrive here if y exp is 0x00 AND x is a normal number or a denorm

; sreg07 has the sign of x

; sreg12 has x exp, dxh/dxl has x significand and lead bit

; sreg04 has y exp, dyl/dyl has y significand and lower bit of exp

dsub_y_tiny

        orrs    sreg04, dyl, dyh, LSL #1  ; check significand

        beq     dsub_result_x           ; normal + 0 = x

; y is a denorm; don't normalize, just fix exponent

        mov     sreg04, #1              ; true exp for denorms

        bic     dyh, dyh, #0x80000000   ; clear lead bit

        orr     sreg12, sreg12, sreg07, LSR #20

        orr     sreg04, sreg04, sreg07, LSR #20 

        b       dsub_start              ; normal acse

  #endif



; arrive here if x is 0

; y was not unpacked yet, so just return it  

dsub_result_y

  #ifdef IEEE_STD

; must check if y is a denorm

        mov     sreg12, dyh, LSR #20    ; sreg12 has y exp and sign    

        bics    sreg12, sreg12, #0x0800  ; clear sign

        moveq   dyh, #0                 ; make denorms 0

        moveq   dyl, #0

  #endif

        mov     dxh, dyh

        mov     dxl, dyl

        Return



; arrive here if x must be returned in IEEE_STD mode

; sreg12 has x exponent

; sreg07 has sign of x

; dxh/dxl has x significand and lead bit

  #ifdef IEEE_STD

dsub_result_x_std

        mov     dxl, dxl, LSR #11       ; align lowest 21 bits

        orr     dxl, dxl, dxh, LSL #21  ; lowest 32 bits

        add     dxh, dxh, dxh           ; shift off msb (LSL #1)

        orr     dxh, sreg07, dxh, LSR #12   ; stuff sign

        orr     dxh, dxh, sreg12, LSL #20  ; stuff exponent

        Return                          ; return 

  #endif  



; arrive here if y is 0 and x must be returned

; x is a normal number, unpacked 

; sreg07 has the sign

; sreg12 has the exponent

; dxh/dxl has x significand and lead bit

; must repack 

dsub_result_x

        cmp     dxh, #0                 ; test lead bit

        subges  sreg12, sreg12, #1      ; sub 1 for denormals

  #ifdef IEEE_STD

        moveq   dxh, #0                 ; return 0 for denorms in IEEE_STD mode

        moveq   dxl, #0

        Reteq

  #endif   

        mov     dxl, dxl, LSR #11       ; align lowest 21 bits

        orr     dxl, dxl, dxh, LSL #21  ; lowest 32 bits

        add     dxh, dxh, dxh           ; shift off msb (LSL #1)

        orr     dxh, sreg07, dxh, LSR #12   ; stuff sign

        orr     dxh, dxh, sreg12, LSL #20  ; stuff exponent

        Return                          ; return 



; ============================================================================

; The next functions will also be used in other parts of the code

; ============================================================================



; arrive here when result is INF

; return signed INF

; sreg07 holds the sign 

__d_result_INF

        ldr     dxh, =0x7ff00000        ; prepare code for INF

        orr     dxh, sreg07, dxh            ; make signed INF

        mov     dxl, #0

        Return



__d_result_x_NaN

; x is a NaN

; must check if y is also a NaN and set invalid if any of them is signaling  

; dxh/dxl has significand and low bit of exp

; only happens in IEEE_FULL and IEEE_FIXED modes

	#ifdef IEEE_FULL_OR_FIXED

        tst     dxh, #0x40000000

        beq     __d_result_invalid      ; fx has a signaling NaN

; x is a quiet NaN, but y could be a signaling NaN

; unpack fy in sreg04(exp) and dyh/dyl(significand)  

        mov     sreg04, dyh, LSR #20    ; sreg04 has x exp and sign

        mov     dyh, dyh, LSL #11       ; left-align 53 sig bits

        orr     dyh, dyh, dyl, LSR #21  ; all lead bits but msb is low exp

        mov     dyl, dyl, LSL #11       ; low 21 bits

        movs    sreg04, sreg04, LSL #21  ; check exponent

        beq     __d_result_make_NaN     ; make NaN for 0

        cmn     sreg04, #0x00200000

        bne     __d_result_make_NaN     ; make NaN for normals

        orrs    sreg04, dyl, dyh, LSL #1

        beq     __d_result_make_NaN     ; make NaN for inf

; y is a NaN    

; fall through dsub_y_NaN

  #endif  



; arrive here if y is a NaN

dsub_y_NaN

        mov     dxh, dyh



__d_result_NaN

; arrive here when input is a NaN and invalid must be checked

; return a QNaN and check for invalid if required

; dxh/dxl contains the unpacked significand and lower bit of exp

	#ifdef IEEE_FULL_OR_FIXED

        mov     sreg03, dxh             ; save high part for tests

        MakeDNaN

        tst     sreg03, #0x40000000     ; check signal

        Retne                           ; exit if quiet

        LFPState sreg03, sreg02

        SetInvalidE sreg03              ; set invalid

        StFPState sreg03,sreg02         ; store FPSCR  

        Return

  #else

        MakeDNaN

        Return

  #endif



; set the invalid flag  

__d_result_invalid

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg03, sreg02

        SetInvalidE sreg03              ; set invalid

        StFPState sreg03,sreg02         ; store FPSCR  

  #endif

; fall through make NaN



; arrive here when a NaN must be returned without any checks    

__d_result_make_NaN

        MakeDNaN

        Return



; arrive here if a 0 must be returned

; in IEEE_FULL mode must check rounding mode                          

__d_result_signed_zero

        mov     dxh, #0

        mov     dxl, #0

  #ifdef IEEE_FULL

        LFPState sreg03, sreg03

        tst     sreg03, CHOP_OR_DOWN_BIT_MASK 

        Reteq

        tst     sreg03, ZERO_OR_DOWN_BIT_MASK

        Retne

        mov     dxh, #0x80000000        ; -0 for down

  #endif

        Return



	#ifdef IEEE_FULL_OR_FIXED

	IMPORT __fp_handler_binary_64_to_64



; invalid exception handler

__wrap_invalid_handler

        ldmfd   sp!, {dxh, dxl, dyh, dyl}

        ldr     r12, =(FE_EX_INVALID :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_SUB)

        stmfd   sp!, {sreg12}

        bl      __fp_handler_binary_64_to_64

        add     sp, sp, #4

        Ret_restore



  #endif     



  #ifdef IEEE_FULL

	IMPORT __fp_handler_64_to_64



; inexact exception wrapper

__wrap_inexact_handler

        Clrstk(al)

        ldr     r2, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_SUB)

        bl      __fp_handler_64_to_64

        Ret_restore

  #endif

  #ifdef USING_ARMASM
	END
  #endif

#endif

/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */
