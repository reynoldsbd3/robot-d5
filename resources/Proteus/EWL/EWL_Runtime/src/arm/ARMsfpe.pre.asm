;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:25 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: ARMsfpe.pre.s

;===============================================================================

;;; No equivalent ADS directive, (yet)
;;; .option no_section_resume on

#ifndef ARMSFPE_PRE_H
#define ARMSFPE_PRE_H


#define _AEABI


#ifdef _AEABI

#define _dadd     __aeabi_dadd
#define _ddiv     __aeabi_ddiv
#define _dmul     __aeabi_dmul
#define _drsb     __aeabi_drsub
#define _dsub     __aeabi_dsub
#define _d_neg    __aeabi_dneg
#define _deq      __aeabi_dcmpeq
#define _dls      __aeabi_dcmplt
#define _dleq     __aeabi_dcmple
#define _dgeq     __aeabi_dcmpge
#define _dgr      __aeabi_dcmpgt
#define _fadd     __aeabi_fadd 
#define _fdiv     __aeabi_fdiv
#define _fmul     __aeabi_fmul
#define _frsb     __aeabi_frsub
#define _fsub     __aeabi_fsub
#define _f_neg    __aeabi_fneg
#define _feq      __aeabi_fcmpeq
#define _fls      __aeabi_fcmplt 
#define _fleq     __aeabi_fcmple
#define _fgeq     __aeabi_fcmpge
#define _fgr      __aeabi_fcmpgt
#define _d_dtoi   __aeabi_d2iz
#define _d_dtou   __aeabi_d2uiz
#define _d_dtoll  __aeabi_d2lz
#define _d_dtoull __aeabi_d2ulz
#define _f_ftoi   __aeabi_f2iz
#define _f_ftou   __aeabi_f2uiz
#define _f_ftoll  __aeabi_f2lz
#define _f_ftoull __aeabi_f2ulz
#define _d2f      __aeabi_d2f
#define _f2d      __aeabi_f2d
#define _d_itod   __aeabi_i2d
#define _d_utod   __aeabi_ui2d
#define _d_lltod  __aeabi_l2d
#define _d_ulltod __aeabi_ul2d
#define _f_itof   __aeabi_i2f
#define _f_utof   __aeabi_ui2f
#define _f_lltof  __aeabi_l2f
#define _f_ulltof __aeabi_ul2f
#define _f_ftoh   __aeabi_f2h
#define _f_htof   __aeabi_h2f
#define _f_ftoh_alt   __aeabi_f2h_alt
#define _f_htof_alt   __aeabi_h2f_alt

#define _ll_mul   __aeabi_lmul
;#define _ll_mod   __aeabi_ldivmod
;#define _ull_mod  __aeabi_uldivmod
#define _ll_sll   __aeabi_llsl
#define _ll_sra   __aeabi_llsr

#include "FSL_defines.s"


#endif

;;; Conditional is configured this way due to bug in mwasmarm with -ads flag
;;; IEEE_FULL || IEEE_FIXED
#ifdef IEEE_FULL
	#define IEEE_FULL_OR_FIXED
#endif
#ifdef IEEE_FIXED
	#define IEEE_FULL_OR_FIXED
#endif

;;; VFP || !IEEE_STD
#ifdef VFP
	#define VFP_OR_NOT_IEEE_STD
#endif
#ifndef IEEE_STD
	#define VFP_OR_NOT_IEEE_STD
#endif

; no VFP support needed in RunFast mode
;;; !VFP || !IEEE_STD
#ifndef VFP
	#define NOT_VFP_OR_NOT_IEEE_STD
#endif
#ifndef IEEE_STD
	#define NOT_VFP_OR_NOT_IEEE_STD
#endif

;;; VFP && !IEEE_STD
#ifdef VFP
	#ifndef IEEE_STD
		#define VFP_AND_NOT_IEEE_STD
	#endif
#endif

;;; CLRSTK macro
	MACRO
	CLRSTK $cond
	add$cond	sp,sp,#16
	MEND



#ifdef IEEE_FULL_OR_FIXED

        IMPORT __rt_fp_status_addr
	
#endif



;=============================================================================== 
; REGISTER NAMES
_R0	RN 0
_R1	RN 1
_R2	RN 2
_R3	RN 3
_R4	RN 4
_R5	RN 5
_R6	RN 6
_R7	RN 7
_R8	RN 8
_R9	RN 9
_R10	RN 10
_R11	RN 11
_R12	RN 12
_SP	RN 13
_LR	RN 14
_PC	RN 15

;
; SCRATCH REGISTERS

sreg01 	RN 1                      ; volatile        same as fy, dxl or dxh
sreg02  RN 2                      ; volatile        same as dyl or dyh
sreg03 	RN 3                      ; volatile    same as dyl or dyh; whether to add or sub
sreg04	RN 4                      ; nonvolatile same as fpstate
sreg05	RN 5                      ; nonvolatile
rscratch RN 6                     ; nonvolatile same as rsign
sreg06	RN	6 

; @@@ Added next

sreg07	RN 7
sreg08	RN 8                      ; nonvolatile same as yexp
sreg09	RN 9                      ; nonvolatile same as fyb
sreg10	RN 10                     ; nonvolatile same as fr
sreg11	RN 11                     ; nonvolatile same as drh or dyl
sreg12	RN 12                     ; nonvolatile same as drh or dyl

;
; FLOATING-POINT NUMBERS
fx 	RN 0	                      ; volatile        fp argument x and x mantissa
fy	RN 1                          ; volatile        fp argument y
fr 	RN 10                        ; nonvolatile fp argument result

  #ifdef BIG_ENDIAN

dxh	RN 0                         ; volatile        double argument x, hi 32 bits
dxl	RN 1                         ; volatile        double argument x, lo 32 bits

; @@@ Added next four definitions for long long registers

llh	RN 0
lll	RN 1                         ; long long registers for first argument
lyh	RN 2
lyl	RN 3                         ; long long registers for second argument
dyh	RN 2                         ; volatile        double argument y, hi 32 bits
dyl	RN 3                         ; volatile        double argument y, lo 32 bits

drh	RN 12			; nonvolatile double argument result, hi 32 bits
drl	RN 11			 ; nonvolatile double argument result, lo 32 bits

  #else

; @@@ Modified here to take into account the SoftFPA mode

    #ifdef SOFTFPA

; @@@ In SoftFPA mode double arguments are passed like in the the Big Endian mode

dxh	RN 0			 ; volatile        double argument x, hi 32 bits
dxl	RN 1			 ; volatile        double argument x, lo 32 bits
dyh	RN 2
dyl	RN 3

    #else

dxh	RN 1			 ; volatile        double argument x, hi 32 bits
dxl	RN 0			 ; volatile        double argument x, lo 32 bits
dyh	RN 3
dyl	RN 2

    #endif

; @@@ Added next four definitions for long long registers   

llh	RN 1
lll	RN 0			 ; long long registers for first argument
lyh	RN 3
lyl	RN 2			 ; long long registers for second argument
drh	RN 11			 ; nonvolatile double argument result, hi 32 bits
drl	RN 12			 ; nonvolatile double argument result, lo 32 bits

  #endif

;
; SIGN

rsign	RN 6			 ; nonvolatile sign of result

;
; MANTISSA

fyb	RN 9			 ; nonvolatile y mantissa (x mantissa is unpacked into fr)

;
; EXPONENT

rexp	RN 7			 ; nonvolatile exponent of result
yexp	RN 8			 ; nonvolatile same as sreg08; exponent of y

;
; FLOATING-POINT STATUS/ENVIRONMENT

vfpstate	RN 2		 ; volatile    state 0-based code: number - 0, zero - 2, INF - 4, NaN -6
				;  on converts, may indicate signed/unsigned and length

fpstate		RN 4		 ; nonvolatile state 0-based code: number - 0, zero - 2, INF - 4, NaN -6
				;  on converts, may indicate signed/unsigned and length

;
; FLAGS
;
; These flags indicate the type to which a value will be coverted


DST_DBL 	EQU 1
DST_FLT 	EQU 0

;
;===============================================================================
;CONSTANTS 
;

#define EXCEPTION_ENABLE_SHIFT 8
#define EXCEPTION_ENABLE_MASK (0x1F << EXCEPTION_ENABLE_SHIFT)

#ifdef IEEE_FULL_OR_FIXED



#define INVALID_BIT #0x00000001
#define DIVBYZERO_BIT  #0x00000002
#define OVERFLOW_BIT #0x00000004
#define UNDERFLOW_BIT #0x00000008
#define INEXACT_BIT #0x00000010
#define STICKY_FLAGS_SHIFT 0
#define STICKY_FLAGS_MASK (0x1F << STICKY_FLAGS_SHIFT)
#define ROUND_MODE_MASK #0x00C00000
#define ROUND_MODE_BIT_SHIFT #22
           ;/* FPSCR bits[23:22] = 0b00 */
#define FE_TONEAREST #0x00000000
          ;/* FPSCR bits[23:22] = 0b11 */
#define FE_TOWARDZERO #0x00C00000
             ;/* FPSCR bits[23:22] = 0b01 */
#define FE_UPWARD #0x00400000
           ;/* FPSCR bits[23:22] = 0b10 */
#define FE_DOWNWARD #0x00800000



; @@@ Added next lines

#define FE_IEEE_MASK_INVALID #0x00000100
#define FE_IEEE_MASK_DIVBYZERO #0x00000200
#define FE_IEEE_MASK_OVERFLOW #0x00000400
#define FE_IEEE_MASK_UNDERFLOW #0x00000800
#define FE_IEEE_MASK_INEXACT #0x00001000

#define FE_EX_RDIR              (-1 << 31)
#define FE_EX_EXCEPT_SHIFT      (26)
#define FE_EX_EXCEPT_MASK       (0x1F << FE_EX_EXCEPT_SHIFT)
#define FE_EX_INEXACT           (0x10 << FE_EX_EXCEPT_SHIFT)
#define FE_EX_UNDERFLOW         (0x08 << FE_EX_EXCEPT_SHIFT)
#define FE_EX_OVERFLOW          (0x04 << FE_EX_EXCEPT_SHIFT)
#define FE_EX_DIVBYZERO         (0x02 << FE_EX_EXCEPT_SHIFT)
#define FE_EX_INVALID           (0x01 << FE_EX_EXCEPT_SHIFT)
#define FE_EX_FLUSHZERO_SHIFT   (24)
#define FE_EX_FLUSHZERO         (1 << FE_EX_FLUSHZERO_SHIFT)
#define FE_EX_ROUND_SHIFT       (22)
#define FE_EX_ROUND_MASK        (3 << FE_EX_ROUND_SHIFT)
#define FE_EX_INTYPE_SHIFT      (7)
#define FE_EX_INTYPE_MASK       (7 << FE_EX_INTYPE_SHIFT)
#define FE_EX_OUTTYPE_SHIFT     (4)
#define FE_EX_OUTTYPE_MASK      (7 << FE_EX_OUTTYPE_SHIFT)
#define FE_EX_TYPE_SHIFT        (4)
#define FE_EX_TYPE_MASK         (7 << FE_EX_TYPE_SHIFT)
#define FE_EX_FN_SHIFT          (0)
#define FE_EX_FN_MASK           (15 << FE_EX_FN_SHIFT)
#define FE_EX_ROUND_NEAREST     (0 << FE_EX_ROUND_SHIFT)
#define FE_EX_ROUND_PLUSINF     (1 << FE_EX_ROUND_SHIFT)
#define FE_EX_ROUND_MINUSINF    (2 << FE_EX_ROUND_SHIFT)
#define FE_EX_ROUND_ZERO        (3 << FE_EX_ROUND_SHIFT)
#define FE_EX_BASETYPE_FLOAT    (0)
#define FE_EX_BASETYPE_DOUBLE   (1)
#define FE_EX_BASETYPE_INT      (4)
#define FE_EX_BASETYPE_LONGLONG (5)
#define FE_EX_BASETYPE_UINT     (6)
#define FE_EX_BASETYPE_ULONGLONG (7)
#define FE_EX_TYPE_FLOAT        (FE_EX_BASETYPE_FLOAT << FE_EX_TYPE_SHIFT)
#define FE_EX_TYPE_DOUBLE       (FE_EX_BASETYPE_DOUBLE << FE_EX_TYPE_SHIFT)
#define FE_EX_TYPE_INT          (FE_EX_BASETYPE_INT << FE_EX_TYPE_SHIFT)
#define FE_EX_TYPE_LONGLONG     (FE_EX_BASETYPE_LONGLONG << FE_EX_TYPE_SHIFT)
#define FE_EX_TYPE_UINT         (FE_EX_BASETYPE_UINT << FE_EX_TYPE_SHIFT)
#define FE_EX_TYPE_ULONGLONG    (FE_EX_BASETYPE_ULONGLONG << FE_EX_TYPE_SHIFT)
#define FE_EX_INTYPE_FLOAT      (FE_EX_BASETYPE_FLOAT << FE_EX_INTYPE_SHIFT)
#define FE_EX_INTYPE_DOUBLE     (FE_EX_BASETYPE_DOUBLE << FE_EX_INTYPE_SHIFT)
#define FE_EX_INTYPE_INT        (FE_EX_BASETYPE_INT << FE_EX_INTYPE_SHIFT)
#define FE_EX_INTYPE_LONGLONG   (FE_EX_BASETYPE_LONGLONG << FE_EX_INTYPE_SHIFT)
#define FE_EX_INTYPE_UINT       (FE_EX_BASETYPE_UINT << FE_EX_INTYPE_SHIFT)
#define FE_EX_INTYPE_ULONGLONG  (FE_EX_BASETYPE_ULONGLONG<<FE_EX_INTYPE_SHIFT)
#define FE_EX_OUTTYPE_FLOAT     (FE_EX_BASETYPE_FLOAT << FE_EX_OUTTYPE_SHIFT)
#define FE_EX_OUTTYPE_DOUBLE    (FE_EX_BASETYPE_DOUBLE << FE_EX_OUTTYPE_SHIFT)
#define FE_EX_OUTTYPE_INT       (FE_EX_BASETYPE_INT << FE_EX_OUTTYPE_SHIFT)
#define FE_EX_OUTTYPE_LONGLONG  (FE_EX_BASETYPE_LONGLONG<<FE_EX_OUTTYPE_SHIFT)
#define FE_EX_OUTTYPE_UINT      (FE_EX_BASETYPE_UINT << FE_EX_OUTTYPE_SHIFT)
#define FE_EX_OUTTYPE_ULONGLONG (FE_EX_BASETYPE_ULONGLONG<<FE_EX_OUTTYPE_SHIFT)
#define FE_EX_FN_ADD            (1 << FE_EX_FN_SHIFT)
#define FE_EX_FN_SUB            (2 << FE_EX_FN_SHIFT)
#define FE_EX_FN_MUL            (3 << FE_EX_FN_SHIFT)
#define FE_EX_FN_DIV            (4 << FE_EX_FN_SHIFT)
#define FE_EX_FN_REM            (5 << FE_EX_FN_SHIFT)
#define FE_EX_FN_RND            (6 << FE_EX_FN_SHIFT)
#define FE_EX_FN_SQRT           (7 << FE_EX_FN_SHIFT)
#define FE_EX_FN_CVT            (8 << FE_EX_FN_SHIFT)
#define FE_EX_FN_CMP            (9 << FE_EX_FN_SHIFT)
#define FE_EX_FN_RAISE          (15 << FE_EX_FN_SHIFT)
#define FE_EX_CMPRET_UNORDERED  (8)
#define FE_EX_CMPRET_LESS       (4)
#define FE_EX_CMPRET_EQUAL      (2)
#define FE_EX_CMPRET_GREATER    (1)


; Bits separating rounding mode cases

#define CHOP_OR_DOWN_BIT_OFFSET 23
#define CHOP_OR_UP_BIT_OFFSET 22
#define CHOP_OR_DOWN_BIT_MASK #(1 << CHOP_OR_DOWN_BIT_OFFSET)
#define CHOP_OR_UP_BIT_MASK #(1 << CHOP_OR_UP_BIT_OFFSET)
;#define DIRECTED_BIT_SHIFT #30

; Bits selecting rounding mode cases
#define UP_OR_NEAR_BIT_MASK #(1 << CHOP_OR_UP_BIT_OFFSET)
#define DOWN_OR_NEAR_BIT_MASK #(1 << CHOP_OR_DOWN_BIT_OFFSET)
#define ZERO_OR_UP_BIT_MASK #(1 << CHOP_OR_DOWN_BIT_OFFSET)
#define ZERO_OR_DOWN_BIT_MASK #(1 << CHOP_OR_UP_BIT_OFFSET)
#define CHOP_OR_DOWN_BIT_SHIFT #31


; @@@ Added next definitions

#ifndef IEEE_FIXED

#define OVERFLOW_MASK #0x00000014
#define UNDERFLOW_MASK #0x00000018

; never set inexact in IEEE_FIXED mode
#else

#define OVERFLOW_MASK #0x00000004
#define UNDERFLOW_MASK #0x00000008

#endif



#ifdef THUMB

	MACRO
	Return_lr

        ldmfd   sp!, {lr}
        bx      lr

       	MEND 

#else

	MACRO
	Return_lr

        ldmfd   sp!, {pc}

	MEND

#endif

; defined(IEEE_FULL_OR_FIXED)
#endif

;===============================================================================
; Macro definitions
;



; MACRO: LFPState       (Load floating point state)
; DESCRIPTION: Loads the floating point state word from software/hardware FPSCR.
; PARAMETERS: fpstate_reg - value of FPSCR 
;             addr_reg    - software fp: address of the software FPSCR
;                         - hardware fp: not used
; INPUT:      none
; OUTPUT:     fpstate_reg, addr_reg (soft fp only)
;             (if addr_reg==fpstate_reg, the macro returns the value of FPSCR 
;             and its address is lost)



#ifdef VFP

	MACRO
	LFPState $fpstate_reg, $addr_reg

        fmrx    $fpstate_reg, FPSCR
	MEND

#else  

	#ifdef __VFPV4__
	MACRO
	LFPState $fpstate_reg, $addr_reg
	
	   vmrs    $fpstate_reg, FPSCR
	MEND
	
	#else
	
	#ifdef IEEE_FULL_OR_FIXED

	MACRO
	LFPState $fpstate_reg, $addr_reg

	IF $addr_reg = _R0

		stmfd   sp!, {r1-r3, r12, lr}
		bl      __rt_fp_status_addr
		ldmfd   sp!, {r1-r3, r12, lr}
	ENDIF

      	IF $addr_reg = _R1 

        	stmfd   sp!, {r0, r2-r3, r12, lr}
	        bl      __rt_fp_status_addr
	        mov     r1, r0
       		ldmfd   sp!, {r0, r2-r3, r12, lr}
	ENDIF

       IF $addr_reg = _R2 

	        stmfd   sp!, {r0-r1, r3, r12, lr}
        	bl      __rt_fp_status_addr
        	mov     r2, r0
        	ldmfd   sp!, {r0-r1, r3, r12, lr}
	ENDIF

	IF $addr_reg = _R3 

        	stmfd   sp!, {r0-r2, r12, lr}
        	bl      __rt_fp_status_addr
        	mov     r3, r0
        	ldmfd   sp!, {r0-r2, r12, lr}
	ENDIF

       IF $addr_reg = _R12 

        	stmfd   sp!, {r0-r3, lr}
        	bl      __rt_fp_status_addr
        	mov     r12, r0
        	ldmfd   sp!, {r0-r3, lr}
	ENDIF

      IF $addr_reg = LR 

        	stmfd   sp!, {r0-r3,r12}
        	bl      __rt_fp_status_addr
        	mov     lr, r0
        	ldmfd   sp!, {r0-r3,r12}
	ENDIF

      IF $addr_reg = R4 :LOR: $addr_reg = r5 :LOR: \
              $addr_reg = r6 :LOR: $addr_reg = r7 :LOR: \
              $addr_reg = r8 :LOR: $addr_reg = r9 :LOR: \
              $addr_reg = r10 :LOR: $addr_reg = r11

	        stmfd   sp!, {r0-r3, r12, lr}
	        bl      __rt_fp_status_addr
	        mov     $addr_reg, r0
	        ldmfd   sp!, {r0-r3, r12, lr}              

     	ENDIF 

       ldr     $fpstate_reg, [$addr_reg]

     	MEND 
	; ifdef IEEE_FULL_OR_FIXED
	#endif

; ifdef VFPV4
#endif
;#ifdef VFP
#endif



  

; MACRO: StFPState      (Store floating point state)
; DESCRIPTION: Store state word back to software/hardware FPSCR.
; PARAMETERS: fpstate_reg - value of FPSCR 
;             addr_reg    - software fp: address of the software FPSCR
;                         - hardware fp: not used
; INPUT:      fpstate_reg, addr_reg (soft fp only)
; OUTPUT:     none

#ifdef VFP

	MACRO
	StFPState $fpstate_reg, $addr_reg

        fmxr    FPSCR,$fpstate_reg

	MEND

#else

	#ifdef __VFPV4__
	
	MACRO
	StFPState $fpstate_reg, $addr_reg

        vmsr    FPSCR,$fpstate_reg

	MEND
	#else	
	#ifdef IEEE_FULL_OR_FIXED

	MACRO
	StFPState $fpstate_reg, $addr_reg

        str     $fpstate_reg, [$addr_reg]

	MEND
	; #ifdef IEEE_FULL_OR_FIXED
	#endif

	;ifdef VFPV4
	#endif
#endif



#ifdef IEEE_FULL_OR_FIXED

; MACRO: SetInvalid
; DESCRIPTION: Set the invalid flag.

	MACRO
	SetInvalid	$fpstate2

        orr     $fpstate2,$fpstate2,INVALID_BIT

	MEND


; MACRO: SetInexact
; DESCRIPTION: Set the inexact flag or jump to __wrap_inexact_handler
; Modified for user defined handlers

	MACRO
	SetInexact $fpstate2

        tst     $fpstate2, FE_IEEE_MASK_INEXACT
        bne.w     __wrap_inexact_handler
        orr     $fpstate2, $fpstate2, INEXACT_BIT

	MEND


; MACRO: SetInexactL
; DESCRIPTION: Set the inexact flag or jump to the specified label
; Modified for user defined handlers



	MACRO
	SetInexactL $fpstate2, $where

        tst     $fpstate2, FE_IEEE_MASK_INEXACT
        bne.w     $where
        orr     $fpstate2, $fpstate2, INEXACT_BIT

	MEND


; MACRO: SetOverflow
; DESCRIPTION: Set the overflow and inexact flags or jump to __wrap_overflow_handler
; Modified for user defined handlers

	MACRO
	SetOverflow $fpstate2, $where

        tst     $fpstate2, FE_IEEE_MASK_OVERFLOW
        bne.w     __wrap_overflow_handler
        orr     $fpstate2, $fpstate2, OVERFLOW_MASK

	MEND


; MACRO: SetOverflowL
; DESCRIPTION: Set the overflow and inexact flags or jump to the specified label
; Modified for user defined handlers

	MACRO
	SetOverflowL $fpstate2, $where

        tst     $fpstate2, FE_IEEE_MASK_OVERFLOW
        bne.w     $where
        orr     $fpstate2, $fpstate2, OVERFLOW_MASK

	MEND


; MACRO: SetUnderflow
; DESCRIPTION: Set the underflow and inexact flags or jump to __wrap_inexact_handler
; Modified for user defined handlers



#ifdef IEEE_FULL  

	MACRO
	SetUnderflow $fpstate2

        tst     $fpstate2, FE_IEEE_MASK_INEXACT
        bne.w     __wrap_inexact_handler
        orr     $fpstate2, $fpstate2, UNDERFLOW_MASK

	MEND

#else

	MACRO
	SetUnderflow $fpstate2

        orr     $fpstate2, $fpstate2, UNDERFLOW_MASK

	MEND

#endif



; MACRO: SetDivByZero
; DESCRIPTION: Set the divide by zero flag.

	MACRO
	SetDivByZero $fpstate2

        tst     $fpstate2, FE_IEEE_MASK_DIVBYZERO
        bne.w     __wrap_divbyzero_handler
        orr     $fpstate2, $fpstate2, DIVBYZERO_BIT

	MEND


; MACRO: SetInvalidE
; DESCRIPTION: Set the invalid flag or jump to __wrap_invalid_handler
; Modified for user defined handlers

	MACRO
	SetInvalidE $fpstate2

        tst     $fpstate2, FE_IEEE_MASK_INVALID
        bne.w     __wrap_invalid_handler
        orr     $fpstate2,$fpstate2,INVALID_BIT

	MEND


; MACRO: SetInvalidEL
; DESCRIPTION: Set the invalid flag or jump to the specified label
; Modified for user defined handlers



	MACRO
	SetInvalidEL $fpstate2, $label

        tst     $fpstate2, FE_IEEE_MASK_INVALID
        bne.w     $label
        orr     $fpstate2,$fpstate2,INVALID_BIT

	MEND

#endif
; IEEE_FULL || IEEE_FIXED


; MACRO: MakeFNaN (Make Floating NaN)
; DESCRIPTION: Create default float NaN in v0
; COMPATIBILITY: Sign is always positive in new NaNs, as with PowerPC.
;

	MACRO
	MakeFNaN


        ldr     fx, =0x7fffffff
	MEND


; MACRO: MakeDNaN (Make Double NaN)
; DESCRIPTION: Create default double NaN in dxh/dxl
; COMPATIBILITY: Sign is always positive in new NaNs, as with PowerPC.
; TRICK: Force the leading fraction bit to be zero, as in MIPS ISA.
;

	MACRO
	MakeDNaN

        mvn     dxl, #0
        bic     dxh, dxl, #0x80000000

	MEND


	MACRO
	Normalize $number, $shift

        cmp     $number, #0x00010000
        sublo   $shift, $shift, #16
        movlo   $number, $number, LSL #16
        cmp     $number, #0x01000000
        sublo   $shift, $shift, #8
        movlo   $number, $number, LSL #8
        cmp     $number, #0x10000000
        sublo   $shift, $shift, #4
        movlo   $number, $number, LSL #4
        cmp     $number, #0x40000000
        sublo   $shift, $shift, #2
        movlo   $number, $number, LSL #2
        cmp     $number,#0
        subpl   $shift, $shift, #1
        addpl   $number, $number, $number  ; LSL #1  


	MEND

;@@@ Added: nomalize macro
;@@@ Behaves differently for ARM4T and ARM5

	MACRO
	Normalize_a5 $number, $shift, $temp

#ifndef ARM5

        cmp     $number, #0x00010000
        sublo   $shift, $shift, #16
        movlo   $number, $number, LSL #16
        cmp     $number, #0x01000000
        sublo   $shift, $shift, #8
        movlo   $number, $number, LSL #8
        cmp     $number, #0x10000000
        sublo   $shift, $shift, #4
        movlo   $number, $number, LSL #4
        cmp     $number, #0x40000000
        sublo   $shift, $shift, #2
        movlo   $number, $number, LSL #2
        cmp     $number,#0
        subpl   $shift, $shift, #1
        addpl   $number, $number, $number  ; LSL #1  

#else

;@@@ Add code for ARM5 here

        clz     $temp, $number
        movs    $number, $number, LSL $temp
        sub     $shift, $shift, $temp 

#endif

	MEND


;@@@ Added: nomalize macro for special case (shift is a number)
;@@@ Behaves differently for ARM4T and ARM5

	MACRO
	Normalize_num $number, $shift, $init

#ifndef ARM5

        mov     $shift, $init
        cmp     $number, #0x00010000
        sublo   $shift, $shift, #16
        movlo   $number, $number, LSL #16
        cmp     $number, #0x01000000
        sublo   $shift, $shift, #8
        movlo   $number, $number, LSL #8
        cmp     $number, #0x10000000
        sublo   $shift, $shift, #4
        movlo   $number, $number, LSL #4
        cmp     $number, #0x40000000
        sublo   $shift, $shift, #2
        movlo   $number, $number, LSL #2
        cmp     $number,#0
        subpl   $shift, $shift, #1
        addpl   $number, $number, $number  ; LSL #1  

#else

;@@@ Add code for ARM5 here

        clz     $shift, $number
        movs    $number, $number, LSL $shift
        rsb     $shift, $shift, $init

#endif

	MEND
; _H
#endif
