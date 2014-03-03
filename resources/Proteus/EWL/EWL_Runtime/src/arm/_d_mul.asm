;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _d_mul.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



	EXPORT _d_mul

	EXPORT _dmul                   ; FPLIB for _d_mul



	#ifdef VFP_AND_NOT_IEEE_STD

	EXPORT _vfp_support_dmul

#endif




	#ifdef IEEE_FULL_OR_FIXED

#ifdef USING_ARMASM
#define Clrstk(x) CLRSTK #x 
#else
#define Clrstk(x) CLRSTK ##x
#endif

	MACRO
	Prolog

        stmfd   sp!, {r4-r7, sreg08, dxh, dxl, dyh, dyl}

	MEND

#else

#define Clrstk(x)  

	MACRO
	Prolog

        stmfd   sp!, {r4-r7, sreg08}

	MEND

#endif                



#if defined(THUMB)
; && !defined(ARM5)

	MACRO
	Retcc

        Clrstk(cc)

        ldmccfd sp!, {r4-r7, sreg08}

        bxcc    lr

	MEND

	MACRO
	Reteq

        Clrstk(eq)

        ldmeqfd sp!, {r4-r7, sreg08}

        bxeq    lr

	MEND

	MACRO
	Retne

        Clrstk(ne)

        ldmnefd sp!, {r4-r7, sreg08}

        bxne    lr

	MEND

	MACRO
	Return

        Clrstk(al)

        ldmfd   sp!, {r4-r7, sreg08}

        bx      lr

	MEND

	MACRO
	Ret_restore

        ldmfd   sp!, {r4-r7, sreg08}

        bx      lr

	MEND

#else

	MACRO
	Retcc

        Clrstk(cc)

        ldmccfd sp!, {r4-r7, pc}

	MEND

	MACRO
	Reteq

        Clrstk(eq)

        ldmeqfd sp!, {r4-r7, pc}

	MEND

	MACRO
	Retne

        Clrstk(ne)

        ldmnefd sp!, {r4-r7, pc}

	MEND

	MACRO
	Return

        Clrstk(al)

        ldmfd   sp!, {r4-r7, pc}

	MEND

	MACRO
	Ret_restore

        ldmfd   sp!, {r4-r7, pc}

	MEND

#endif 



#ifdef ARM_xM

	MACRO
	UMULL_M $destlo, $desthi, $src1, $src2

        stmfd   sp!, {$src1, $src2}

        mov     sreg08, $src1, LSR #16 

        sub     $src1, $src1, sreg08, LSL #16 

        mov     sreg10, $src2, LSR #16 

        sub     $src2, $src2, sreg10, LSL #16 

        mul     $desthi, sreg08, sreg10 

        mul     sreg11, sreg08, $src2 

        mul     sreg08, $src1, sreg10 

        adds    sreg08, sreg08, sreg11

        addcs   $desthi, $desthi, #0x10000 

        mul     $destlo, $src1, $src2 

        adds    $destlo, $destlo, sreg08, LSL #16

        adc     $desthi, $desthi, sreg08, LSR #16  ; result is in desthi - $destlo

        ldmfd   sp!, {$src1, $src2}

	MEND

#endif 



;===============================================================================

; FUNCTION: double _d_mul (double, double)

; DESCRIPTION: Double multiply.

; INPUT: 

; x=dxh/dxl,

; y=dyh/dyl, doubles

; OUTPUT: 

; dxh/dxl = result

	AREA |.text|,CODE,READONLY


#ifdef VFP

_dmul

_d_mul


        fmdhr   d0, dxh

        fmdlr   d0, dxl

        fmdhr   d1, dyh

        fmdlr   d1, dyl

        fmuld   d0, d0, d1

        fmrdh   dxh, d0

        fmrdl   dxl, d0

  #ifdef THUMB

        bx      lr

  #else

        mov     pc,lr

  #endif        



_vfp_support_dmul

#else

_dmul									; FPLIB entry point

_d_mul

#endif



	#ifdef NOT_VFP_OR_NOT_IEEE_STD

        Prolog

; figure out sign in sreg08

        eor     sreg08, dxh, dyh

        and     sreg08, sreg08, #0x80000000     ; result sign in MSB of sreg08

; unpack x in in sreg12(exp) and dxh/dxl(significand)  

        mov     sreg12, dxh, LSR #20    ; sreg12 has x exp and sign

        mov     dxh, dxh, LSL #11       ; left-align 53 sig bits

        orr     dxh, dxh, dxl, LSR #21  ; all lead bits but msb is low exp

        mov     dxl, dxl, LSL #11       ; low 21 bits

        movs    sreg06, sreg12, LSL #21  ; check exponent

        cmnne   sreg06, #0x00200000     ; test for huge exponent

        beq.w     dmul_x_special_case     ; special case: tiny or huge

        orr     dxh, dxh, #0x80000000   ; set lead bit    

        bic     sreg12, sreg12, #0x0800  ; clear sign from exp  

; unpack fy in sreg04(exp) and dyh/dyl(significand)  

        mov     sreg04, dyh, LSR #20    ; sreg04 has x exp and sign

        mov     dyh, dyh, LSL #11       ; left-align 53 sig bits

        orr     dyh, dyh, dyl, LSR #21  ; all lead bits but msb is low exp

        mov     dyl, dyl, LSL #11       ; low 21 bits

        movs    sreg05, sreg04, LSL #21  ; check exponent

        cmnne   sreg05, #0x00200000     ; test for huge exponent

        beq.w     dmul_y_special_case     ; special case: tiny or huge

        orr     dyh, dyh, #0x80000000   ; set lead bit    

        bic     sreg04, sreg04, #0x0800  ; clear sign from exp 

; at this point: x is in dxh/dxl(significand) and sreg12(exp)

; y is in dyh/dyl(significand) and sreg04(exp)

; sreg08 has sign

dmul_start

        add     sreg12, sreg04, sreg12  ; sum of exponents

; free regs: sreg04, sreg05, sreg06, sreg07

; compute 128-bit product

  #ifndef ARM_xM

        umull   sreg05, sreg04, dxl, dyl

        umull   sreg07, sreg06, dxl, dyh

        adds    sreg04, sreg07, sreg04

        adc     sreg06, sreg06, #0

        umull   sreg07, dxl, dxh, dyl

        adds    sreg04, sreg07, sreg04

        adcs    dxl, dxl, sreg06

        umull   sreg07, dyl, dxh, dyh

        adc     dxh, dyl, #0

        adds    dxl, dxl, sreg07

        adc     dxh, dxh, #0 

  #else

        stmfd   sp!, {sreg08, sreg10, sreg11}



        UMULL_M sreg05, sreg04, dxl, dyl

        UMULL_M sreg07, sreg06, dxl, dyh

        adds    sreg04, sreg07, sreg04

        adc     sreg06, sreg06, #0

        UMULL_M sreg07, dxl, dxh, dyl

        adds    sreg04, sreg07, sreg04

        adcs    dxl, dxl, sreg06

        movcs   sreg06, #1

        movcc   sreg06, #0

        UMULL_M sreg07, dyl, dxh, dyh

        add     dxh, dyl, sreg06

        adds    dxl, dxl, sreg07

        adc     dxh, dxh, #0 



        ldmfd   sp!, {sreg08, sreg10, sreg11}

  #endif  

; result is in dxh - dxl - sreg04 - sreg05, sign and exp is in sreg12

; take into account stickies in sreg04 - sreg05

        orrs    sreg04, sreg04, sreg05

        orrne   dxl, dxl, #1            ; stickies  

        cmp     dxh,#0

        blt     dmul_cont               ; normalize ?

        sub     sreg12, sreg12, #1

        adds    dxl, dxl, dxl

        adc     dxh, dxh, dxh

dmul_cont

        add     sreg12, sreg12, #2      ; coerce exponent

        subs    sreg12, sreg12, #1024 

        bmi.w     dmul_negative_or_zero_exp  ; special case  

        beq.w    dmul_negative_or_zero_exp  ; special case    

        mov     sreg06, sreg12, LSL #20

        cmn     sreg06, #0x00100000     ; check overflow

        bmi.w     __d_overflow            ; overflow case

dmul_common

; normal case: pack 

; sreg12 has the exponent

; dxl/dxh has the significand

; sreg08 has the sign

        movs    sreg02, dxl, LSL #21    ; keep round bits in high part of sreg02  

        mov     dxl, dxl, LSR #11       ; align lowest 21 bits

        orr     dxl, dxl, dxh, LSL #21  ; lowest 32 bits

        add     dxh, dxh, dxh           ; shift off msb (LSL #1)

        orr     dxh, sreg08, dxh, LSR #12   ; stuff sign

        orr     dxh, dxh, sreg12, LSL #20  ; stuff exponent

        Reteq                           ; no round bits: just return

; rounding must be done here

; here we have the inexact result packed in dxh/dxl

; sreg02 contains all the rounding bits

; sreg08 has the sign

; must round and set the inexact bit in IEEE_FULL mode

; cases for rounding :

; ZERO - do nothing

; DOWN - check sign and substract 1 if negative

; UP - check sign and add 1 if positive

; NEAREST - the normal case

  #ifdef IEEE_FULL

; @@@ Modified: added LFPState

;ldr       sreg12, =__SoftFPSCR__                  ; load FPSCR

;ldr    sreg03, [sreg12]

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

  #if !defined(IEEE_FULL) && !defined(IEEE_FIXED)

        adds    dxl, dxl, #1

        adc     dxh, dxh, #0

        Return

  #else 

dmul_add

        adds    dxl, dxl, #1

        adc     dxh, dxh, #0 

        mov     sreg02, dxh, LSL #1     ; look for overflow

        cmn     sreg02, #0x00200000 

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

        cmp     sreg08, #0                  ; sreg08 holds the sign

        beq     dmul_add                ; add and check overflow only if positive

        Return



chop_or_down_round

        tst     sreg03, ZERO_OR_DOWN_BIT_MASK 

        Retne                           ; done if rounding is to zero

        cmp     sreg08, #0

        bne     dmul_add                ; add and check overflow only if negative

        Return

    #endif                               
; #ifdef IEEE_FULL

  #endif                                 
; #if #defined(IEEE_FULL) && !defined(IEEE_FIXED) 



; ============================================================================

; The code for special cases begins here

; ============================================================================



; arrive here if x is either tiny or huge

; dxh/dxl has x significand and lower bit of exp, sreg12 has x exponent and sign

dmul_x_special_case

        bics    sreg12, sreg12, #0x0800  ; clear sign bit from exp

  #ifdef IEEE_STD

        beq     __d_result_zero         ; zero if denorm

  #else

        beq     dmul_x_tiny             ; x is tiny

  #endif        

; otherwise fall through dmul_x_huge



; arrive here if x exp is 0x7ff

; sreg08 has the sign

; dxh/dxl has significand of x and lower bit of exp

dmul_x_huge

        orrs    sreg06, dxl, dxh, LSL #1

        bne.w     __d_result_x_NaN        ; check x and return a QNaN

; x is INF 

; must unpack y

; unpack fy in dyh/dyl(significand) and sreg04(exponent)  

        mov     sreg04, dyh, LSR #20    ; sreg04 has x exp and sign

        mov     dyh, dyh, LSL #11       ; left-align 53 sig bits

        orr     dyh, dyh, dyl, LSR #21  ; all lead bits 

        mov     dyl, dyl, LSL #11       ; low 21 bits

        movs    sreg05, sreg04, LSL #21  ; check exponent

        beq     dmul_y_checkzero        ; 0 or denorm

        cmn     sreg05, #0x00200000

        bne.w     __d_result_INF          ; INF * norm = INF

; if we arrive here x is INF, y is either INF, NaN

        orrs    sreg05, dyl, dyh, LSL #1

        beq.w     __d_result_INF          ; INF * INF = INF

        b       dmul_y_NaN              ; INF * NaN = QNaN

dmul_y_checkzero

; y is either zero or denorm

        orrs    sreg05, dyh, dyl

        beq.w     __d_result_invalid      ; INF * zero = QNaN and set invalid

        b       __d_result_INF          ; INF * denorm = INF



; arrive here if y is either tiny or huge

; dxh/dxl has x significand and lower bit of exp, sreg12 has x exponent 

; dyh/dyl has y significand and lower bit of exp, sreg04 has y exponent

dmul_y_special_case

        bics    sreg04, sreg04, #0x0800  ; clear sign bit

  #ifdef IEEE_STD

        beq     __d_result_zero         ; zero if denorm

  #else

        beq     dmul_y_tiny             ; y is tiny

  #endif

; otherwise fall through dmul_y_huge



; arrive here if y exp is 0xff AND x is a normal number

; sreg08 has the sign

; sreg12 has x exp, dxh/dxl has x significand and lead bit

; sreg04 has y exp, dyh/dyl has y significand and lower bit of exp

dmul_y_huge

        orrs    sreg06, dyl, dyh, LSL #1

        bne.w     dmul_y_NaN              ; check y and return a QNaN

; y is INF, x is a normal number, so return INF

        b       __d_result_INF



  #ifndef IEEE_STD

; arrive here if x exp is 0x00 

; sreg08 has the sign

; sreg12 has x exp, dxh/dxl has x significand and lower bit of exp

dmul_x_tiny

        orrs    sreg06, dxl, dxh, LSL #1  ; check significand

        beq     dmul_x_zero             ; x is zero

; x is a denorm

        mov     sreg12, #1              ; true exp for denorms

        cmp     dxh, #0

        bne     dmul_unpack_x_short

        sub     sreg12, sreg12, #32

        movs    dxh, dxl

        mov     dxl, #0

        bmi     dmul_unpack_x_done

dmul_unpack_x_short

        Normalize_num dxh, sreg06, #32  ; normalize number

;;;        orr     dxh, dxh, dxl, LSR sreg06
	mov	sreg07, dxl, LSR sreg06
	orr	dxh, dxh, sreg07

        rsb     sreg06, sreg06, #32

        mov     dxl, dxl, LSL sreg06

        sub     sreg12, sreg12, sreg06 

; unpack fy in sreg12(exp) and fy(significand)  

dmul_unpack_x_done

; unpack fy in sreg04(exp) and dyh/dyl(significand)  

        mov     sreg04, dyh, LSR #20    ; sreg04 has x exp and sign

        mov     dyh, dyh, LSL #11       ; left-align 53 sig bits

        orr     dyh, dyh, dyl, LSR #21  ; all lead bits but msb is low exp

        mov     dyl, dyl, LSL #11       ; low 21 bits

        movs    sreg05, sreg04, LSL #21  ; check exponent

        cmnne   sreg05, #0x00200000     ; test for huge exponent

        beq     dmul_y_special_case     ; special case: tiny or huge

        orr     dyh, dyh, #0x80000000   ; set lead bit 

        bic     sreg04, sreg04, #0x0800  ; clear sign bit from exp

        b       dmul_start              ; multiply numbers

  #endif



dmul_x_zero

; unpack y

        mov     sreg04, dyh, LSR #20    ; sreg04 has x exp and sign

        mov     dyh, dyh, LSL #11       ; left-align 53 sig bits

        orr     dyh, dyh, dyl, LSR #21  ; all lead bits 

        mov     dyl, dyl, LSL #11       ; low 21 bits

        movs    sreg05, sreg04, LSL #21  ; check exponent

        beq.w     __d_result_zero         ; 0 * denorm = 0 * norm = 0

        cmn     sreg05, #0x00200000

        bne.w     __d_result_zero         ; 0 * norm = 0

        orrs    sreg06, dyl, dyh, LSL #1

        beq.w     __d_result_invalid      ; O * INF = 0 and set invalid

        b       dmul_y_NaN              ; y is a NaN



  #ifndef IEEE_STD

; arrive here if y exp is 0x00 AND x is a normal number or a denorm

; sreg08 has the sign

; sreg12 has x exp, dxh/dxl has x significand and lead bit

; sreg04 has y exp, dyl/dyl has y significand and lower bit of exp

dmul_y_tiny

        orrs    sreg05, dyl, dyh, LSL #1  ; check significand

        beq.w     __d_result_zero         ; 0 * normal = 0

; y is a denorm; unpack and compute result

        mov     sreg04, #1              ; true exp for denorms

        cmp     dyh, #0

        bne     dmul_unpack_y_short

        sub     sreg04, sreg04, #32

        movs    dyh, dyl

        mov     dyl, #0

        bmi     dmul_start

dmul_unpack_y_short

        Normalize_num dyh, sreg06, #32  ; normalize number

;;;        orr     dyh, dyh, dyl, LSR sreg06
	mov	sreg07, dyl, LSR sreg06
	orr	dyh, dyh, sreg07

        rsb     sreg06, sreg06, #32

        mov     dyl, dyl, LSL sreg06

        sub     sreg04, sreg04, sreg06 

        b       dmul_start

  #endif



; arrive here if the exponent is zero or less after operations

; must check for special cases  

dmul_negative_or_zero_exp

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg02, sreg02

        tst     sreg02, FE_IEEE_MASK_UNDERFLOW

        bne.w     __wrap_underflow_handler

  #endif

        cmn     sreg12, #52

        beq     dmul_very_tiny_result   ; case: very tiny

        bmi.w     __d_underflow           ; case: underflow 

; exponent is -51...0

  #ifdef IEEE_STD

        mov     dxh, sreg08                 ; return signed 0 in STD mode 

        mov     dxl, #0

        Return

  #else      

; dxh/dxl has result 

; sreg12 has exponent

; sreg08 has sign   

; first compute stickies

        mov     sreg02, dxh

        mov     sreg03, dxl             ; save number in sreg02/sreg03 

        add     sreg04, sreg12, #52     ; compute shift for stickies

        cmp     sreg04, #32 

        movge   sreg02, sreg03

        movge   sreg03, #0

        subge   sreg04, sreg04, #32     ; case for huge shift

        rsb     sreg05, sreg04, #32     ; compute 32 - sreg04

        mov     sreg02, sreg02, LSL sreg04 

;;;        orr     sreg02, sreg02, sreg03, LSR sreg05

	mov	sreg07, sreg03, LSR sreg05
	orr	sreg02, sreg02, sreg07

        movs    sreg03, sreg03, LSL sreg04

        orrne   sreg02, sreg02, #1      ; sticky

; round/sticky in high part of sreg02

; shift number 

        rsb     sreg12, sreg12, #12     ; shift a total of 12-sreg12 positions

        cmp     sreg12, #32

        movge   dxl, dxh

        movge   dxh, #0

        subge   sreg12, sreg12, #32     ; hige shift

        rsb     sreg04, sreg12, #32     ; compute 32 - shift ammount

        mov     dxl, dxl, LSR sreg12

;;;        orr     dxl, dxl, dxh, LSL sreg04

	mov	sreg07, dxh, LSL sreg04
	orr	dxl, dxl, sreg07

;;;        orr     dxh, sreg08, dxh, LSR sreg12  ; final shift & stuff sign

	mov 	dxh, dxh, LSR sreg12
	orr	dxh, sreg08, dxh


        cmp     sreg02, #0

        Reteq

; rounding must be done here WITHOUT overflow check

; here we have the inexact result packed in dxh/dxl

; sreg02 contains all the rounding bits

; sreg08 has the sign

; cases for rounding :

; ZERO - do nothing

; DOWN - check sign and substract 1 if negative

; UP - check sign and add 1 if positive

; NEAREST - the normal case

	#ifdef IEEE_FULL_OR_FIXED

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

        tst     sreg02, #0x80000000     ; round bit is now the MSB of r12

        Reteq                           ; done if round bit is clear

        movs    sreg02, sreg02, LSL #1  ; round up if sticky bits are non-0

        andeqs  sreg02, dxl, #0x1       ; in half-way case, round up if not already even

        Reteq

        adds    dxl, dxl, #1

        adc     dxh, dxh, #0

        Return

    #ifdef IEEE_FULL

up_round_denorm

        cmp     sreg08, #0                  ; sreg02 holds the sign

        Retne

        adds    dxl, dxl, #1

        adc     dxh, dxh, #0

        Return



chop_or_down_round_denorm

        tst     sreg03, ZERO_OR_DOWN_BIT_MASK

        Retne

        adds    dxl, dxl, dxh, LSR #31  ; just add sign for rounding down

        adc     dxh, dxh, #0

        Return

    #endif                               
; #ifdef IEEE_FULL            

  #endif                                 
; #ifdef IEEE_STD   



; the result is very tiny

; prepare stickies and branch to __f_very_tiny_result  

dmul_very_tiny_result

        orr     fx, dxl, dxh, LSL #1

        b       __d_very_tiny_result



; ============================================================================

; The next functions will also be used in other parts of the code

; ============================================================================



; arrive here when result is INF

; return signed INF

; sreg08 holds the sign

__d_result_INF

        ldr     dxh, =0x7ff00000        ; prepare code for INF

        orr     dxh, sreg08, dxh            ; make signed INF

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

        movs    sreg05, sreg04, LSL #21  ; check exponent

        beq     __d_result_make_NaN     ; make NaN for 0

        cmn     sreg05, #0x00200000

        bne     __d_result_make_NaN     ; make NaN for normals

        orrs    sreg05, dyl, dyh, LSL #1

        beq     __d_result_make_NaN     ; make NaN for inf

; y is a NaN    

; fall through dmul_y_NaN

  #endif  



; arrive here if y is a NaN

dmul_y_NaN

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



; special case: exponent is -52

; sreg08 holds the sign

; fx has the stickies  

__d_very_tiny_result

  #ifdef IEEE_STD

        mov     dxh, sreg08                 ; load signed zero

        mov     dxl, #0

        Return                          ; done for IEEE_STD

  #else

        movs    sreg02, fx              ; stickies in sreg01

        mov     dxh, sreg08                 ; load signed zero

        mov     dxl, #0

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg03, sreg12

        SetUnderflow sreg03             ; set underflow

        StFPState sreg03,sreg12         ; store FPSCR      

    #endif

    #ifdef IEEE_FULL

        tst     sreg03, CHOP_OR_DOWN_BIT_MASK

        bne     chop_or_down_round_denorm

        tst     sreg03, UP_OR_NEAR_BIT_MASK 

        bne     up_round_denorm         ; rounding is up

        cmp     sreg02, #0

    #endif   

        addne   dxl, dxl, #1            ; add 1 if stickies are present

        Return

  #endif                                 
; #ifdef IEEE_STD



; an underflow occured; must round in IEEE_FULL mode

; sreg08 already contains signed zero

__d_underflow

        mov     dxh, sreg08                 ; load signed zero

        mov     dxl, #0

	#ifdef IEEE_FULL_OR_FIXED

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

        bne     chop_or_down_round_denorm

        tst     sreg03, UP_OR_NEAR_BIT_MASK

        Reteq                           ; round no nearest: do nothing

        cmp     dxh, #0

        addeq   dxl, dxl, #1            ; up: add 1 if positive

        Return

  #endif                                 
; #ifndef IEEE_FULL



; an overflow occured, INF must be returned             

; arrive here with sreg08 = sign         

__d_overflow

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg03, sreg02

        SetOverflowL sreg03, __wrap_overflow_handler_pack

        StFPState sreg03,sreg02         ; set overflow 

  #endif

        ldr     dxh, =0x7ff00000        ; prepare code for INF

        orr     dxh, sreg08, dxh            ; make signed INF

        mov     dxl, #0

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

        Reteq

        subs    dxl, dxl, sreg08, LSR #31

        sbc     dxh, dxh, #0

        Return



oflow_chopdown

        subs    dxl, dxl, #1

        sbc     dxh, dxh, #0            ; presume that 1 is always substracted

        tst     sreg03, ZERO_OR_DOWN_BIT_MASK

        Retne

        adds    dxl, dxl, sreg08, LSR #31   ; if rounding down, add the sign 

        adc     dxh, dxh, #0

        Return

  #endif                                 
; #ifndef IEEE_FULL



; arrive here when the result is 0

__d_result_zero

        mov     dxh, sreg08

        mov     dxl, #0

        Return



	#ifdef IEEE_FULL_OR_FIXED

	IMPORT __fp_handler_64_to_64

	IMPORT __fp_handler_binary_64_to_64



; overflow exception handler

__wrap_overflow_handler

        Clrstk(al)

        sub     dxh, dxh, #(1536 << 20)  ; coerce exponent 

        ldr     r2, =(FE_EX_OVERFLOW :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_MUL)

        bl      __fp_handler_64_to_64

        Ret_restore



; overflow exception handler with pack

; dxh/dxl has significand, exp is in sreg12, sreg08 has sign

__wrap_overflow_handler_pack

        Clrstk(al)

        mov     dxl, dxl, LSR #11       ; align lowest 21 bits

        orr     dxl, dxl, dxh, LSL #21  ; lowest 32 bits

        add     dxh, dxh, dxh           ; shift off msb (LSL #1)

        orr     dxh, sreg08 , dxh, LSR #12  ; pack high and stuff sign

        sub     sreg12, sreg12, #1536   ; coerce exponent

        orr     dxh, dxh, sreg12, LSL #20  ; and stuff it

        ldr     r2, =(FE_EX_OVERFLOW :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_MUL)

        bl      __fp_handler_64_to_64

        Ret_restore



; underflow exception handler with pack

; dxh/dxl has significand, exp is in sreg12, sreg08 has sign

__wrap_underflow_handler

        Clrstk(al)

        mov     dxl, dxl, LSR #11       ; align lowest 21 bits

        orr     dxl, dxl, dxh, LSL #21  ; lowest 32 bits

        add     dxh, dxh, dxh           ; shift off msb (LSL #1)

        orr     dxh, sreg08, dxh, LSR #12   ; pack high and stuff sign

        add     sreg12, sreg12, #1536   ; coerce exponent

        orr     dxh, dxh, sreg12, LSL #20  ; and stuff it

        ldr     r2, =(FE_EX_UNDERFLOW :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_MUL)

        bl      __fp_handler_64_to_64

        Ret_restore



; invalid exception handler

__wrap_invalid_handler

        ldmfd   sp!, {dxh, dxl, dyh, dyl}

        ldr     r12, =(FE_EX_INVALID :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_MUL)

        stmfd   sp!, {sreg12}

        bl      __fp_handler_binary_64_to_64

        add     sp, sp, #4

        Ret_restore



  #endif



  #ifdef IEEE_FULL    

; inexact exception wrapper

__wrap_inexact_handler

        Clrstk(al)

        ldr     r2, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_DOUBLE :OR: FE_EX_FN_MUL)

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
