;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _f_cmp.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



;===============================================================================

		 
	EXPORT _f_fne

	EXPORT _fneq                   ; FPLIB for _f_fne

	EXPORT _f_feq

	EXPORT _feq                    ; FPLIB for _f_feq

	EXPORT _f_cmp

	EXPORT _f_cmpe

	EXPORT _f_flt

	EXPORT _fls                    ; FPLIB for _f_flt

	EXPORT _f_fle

	EXPORT _fleq                   ; FPLIB for _f_fle

	EXPORT _f_fgt

	EXPORT _fgr                    ; FPLIB for _f_fgt

	EXPORT _f_fge

	EXPORT _fgeq                   ; FPLIB for _f_fge

	EXPORT _f_fun

	EXPORT _f_for

	EXPORT _d_cmp

	EXPORT _d_cmpe

	EXPORT _d_feq

	EXPORT _deq                    ; FPLIB for _d_feq

	EXPORT _d_fne

	EXPORT _dneq                   ; FPLIB for _d_fne

	EXPORT _d_flt

	EXPORT _dls                    ; FPLIB for _d_flt

	EXPORT _d_fle

	EXPORT _dleq                   ; FPLIB for _d_fle

	EXPORT _d_fgt

	EXPORT _dgr                    ; FPLIB for _d_fgt

	EXPORT _d_fge

	EXPORT _dgeq                   ; FPLIB for _d_fge

	EXPORT _d_fun

	EXPORT _d_for



	EXPORT _fcmpeq

	EXPORT _fcmpge

	EXPORT _fcmple

	EXPORT _dcmpeq

	EXPORT _dcmple

	EXPORT _dcmpge

#ifdef _AEABI	

		EXPORT	__aeabi_cdcmpeq
		EXPORT	__aeabi_cdcmple
		EXPORT __aeabi_cdrcmple	
		EXPORT __aeabi_dcmpun
		EXPORT __aeabi_cfcmpeq
		EXPORT __aeabi_cfcmple
		EXPORT __aeabi_cfrcmple
		EXPORT __aeabi_fcmpun
#endif


#if defined(VFP) && !defined(IEEE_FULL)

	EXPORT _vfp_support_fcmp

	EXPORT _vfp_support_fcmpe

	EXPORT _vfp_support_dcmp

	EXPORT _vfp_support_dcmpe 

#endif



	#ifdef IEEE_FULL_OR_FIXED

	IMPORT __fp_handler_binary_32_to_32

	IMPORT __fp_handler_binary_64_to_32

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



#ifdef VFP



	MACRO
	Prolog_fcmp $e

        fmsr    s0, r0

        fmsr    s1, r1

        fcmp$e s0,s1

        fmstat

	MEND



	MACRO
	Prolog_dcmp $e

        fmdhr   d0, dxh

        fmdlr   d0, dxl

        fmdhr   d1, dyh

        fmdlr   d1, dyl

        fcmp$e d0,d1

        fmstat

	MEND



;===============================================================================        

; FUNCTION: _f_cmp

; INPUT: fx, fy

; OUTPUT: fx = 0-eq, 1-lt, 2-gt, 3-un (invalid if SNaN)

;
	AREA |.text|,CODE,READONLY

_f_cmp

        Prolog_fcmp s

        mov     fx, #2                  ; presume result is GT

        moveq   fx, #0

        movmi   fx, #1

        movvs   fx, #3

        Return



;===============================================================================        

; FUNCTION: _f_cmpe

; INPUT: fx, fy

; OUTPUT: fx = 0-eq, 1-lt, 2-gt, 3-un (invalid if unordered)

;
	AREA |.text|,CODE,READONLY

_f_cmpe

        Prolog_fcmp es

        mov     fx, #2                  ; presume result is GT

        moveq   fx, #0

        movmi   fx, #1

        movvs   fx, #3

        Return



;===============================================================================        

; FUNCTION: _f_fne

; INPUT: fx, fy

; OUTPUT: fx = 0 (relation false), 1 (relation true) (invalid if SNaN)

;
	AREA |.text|,CODE,READONLY

_fneq

_f_fne


        Prolog_fcmp s
#ifdef EXPLICIT_FCMP        
        movne   fx, #1

        moveq   fx, #0
#endif
        Return



;===============================================================================        

; FUNCTION: _f_feq

; INPUT: fx, fy

; OUTPUT: fx = 0 (relation false), 1 (relation true) (invalid if SNaN)

;
	AREA |.text|,CODE,READONLY
	
_feq

_f_feq


        Prolog_fcmp s
#ifdef EXPLICIT_FCMP
        movne   fx, #0

        moveq   fx, #1 
#endif
        Return



;===============================================================================        

; FUNCTION: _f_flt

; INPUT: fx, fy

; OUTPUT: fx = 0 (relation false), 1 (relation true) (invalid if unordered)

;
	AREA |.text|,CODE,READONLY
_fls

_f_flt


        Prolog_fcmp es
#ifdef EXPLICIT_FCMP
        movlo   fx, #1

        movhs   fx, #0
#endif
        Return



;===============================================================================        

; FUNCTION: _f_fle

; INPUT: fx, fy

; OUTPUT: fx = 0 (relation false), 1 (relation true) (invalid if unordered)

;

	AREA |.text|,CODE,READONLY

_fleq

_f_fle


        Prolog_fcmp es
#ifdef EXPLICIT_FCMP
        movls   fx, #1

        movhi   fx, #0
#endif
        Return



;===============================================================================        

; FUNCTION: _f_fgt

; INPUT: fx, fy

; OUTPUT: fx = 0 (relation false), 1 (relation true) (invalid if unordered)

; 
	AREA |.text|,CODE,READONLY

_fgr

_f_fgt


        Prolog_fcmp es
#ifdef EXPLICIT_FCMP
        movgt   fx, #1

        movle   fx, #0 
#endif
        Return



;===============================================================================        

; FUNCTION: _f_fge

; INPUT: fx, fy

; OUTPUT: fx = 0 (relation false), 1 (relation true) (invalid if unordered)

;  

	AREA |.text|,CODE,READONLY

_fgeq

_f_fge


        Prolog_fcmp es
#ifdef EXPLICIT_FCMP
        movge   fx, #1

        movlt   fx, #0
#endif
        Return



;===============================================================================        

; FUNCTION: _f_fun

; INPUT: fx, fy

; OUTPUT: fx = 0 (relation false), 1 (relation true) (invalid on SNaN)

; 
	AREA |.text|,CODE,READONLY

_f_fun

        Prolog_fcmp s

        movvs   fx, #1

        movvc   fx, #0

        return



;===============================================================================        

; FUNCTION: _f_for

; INPUT: fx, fy

; OUTPUT: fx = 0 (relation false), 1 (relation true) (invalid on SNaN)

; 
	AREA |.text|,CODE,READONLY

_f_for

        Prolog_fcmp s

        movvs   fx, #0

        movvc   fx, #1

        return



;===============================================================================        

; FUNCTION: _fcmpeq

; INPUT: fx, fy

; OUTPUT: result in flags

; 
	AREA |.text|,CODE,READONLY

_fcmpeq

        Prolog_fcmp s

        Return



;===============================================================================        

; FUNCTION: _fcmple

; INPUT: fx, fy

; OUTPUT: result in flags

;

	AREA |.text|,CODE,READONLY
	
_fcmple

        Prolog_fcmp es

        Return




;===============================================================================        

; FUNCTION: _fcmpge

; INPUT: fx, fy

; OUTPUT: result in flags

; 
	AREA |.text|,CODE,READONLY

_fcmple

_fcmpge

        Prolog_fcmp es

        Return



; @@@ Double precision comparison functions



;===============================================================================        

; FUNCTION: _d_cmp

; INPUT: dxh/dxl, dyh/dyl

; OUTPUT: fx = 0-eq, 1-lt, 2-gt, 3-un (invalid if SNaN)

;

	AREA |.text|,CODE,READONLY

_d_cmp

        Prolog_dcmp d

        mov     fx, #2                  ; presume result is GT

        moveq   fx, #0

        movmi   fx, #1

        movvs   fx, #3 

        Return



;===============================================================================        

; FUNCTION: _d_cmpe

; INPUT: fxh/dxl, dyh/dyl

; OUTPUT: fx = 0-eq, 1-lt, 2-gt, 3-un (invalid if unordered)

;
	AREA |.text|,CODE,READONLY

_d_cmpe

        Prolog_dcmp ed

        mov     fx, #2                  ; presume result is GT

        moveq   fx, #0

        movmi   fx, #1

        movvs   fx, #3 

        Return



;===============================================================================        

; FUNCTION: _d_fne

; INPUT: dxh/dxl, dyh/dyl

; OUTPUT: fx = 0 (relation false), 1 (relation true) (invalid if SNaN)

;

	AREA |.text|,CODE,READONLY

_dneq

_d_fne


        Prolog_dcmp d
#ifdef EXPLICIT_FCMP
        movne   fx, #1

        moveq   fx, #0
#endif
        Return





;===============================================================================        

; FUNCTION: _d_feq

; INPUT: dxh/dxl, dyh/dyl

; OUTPUT: fx = 0 (relation false), 1 (relation true) (invalid if SNaN)

;
	AREA |.text|,CODE,READONLY

_deq

_d_feq


        Prolog_dcmp d
#ifdef EXPLICIT_FCMP
        movne   fx, #0

        moveq   fx, #1 
#endif
        Return





;===============================================================================        

; FUNCTION: _d_flt

; INPUT: dxh/dxl, dyh/dyl

; OUTPUT: fx = 0 (relation false), 1 (relation true) (invalid if unordered)

;
	AREA |.text|,CODE,READONLY

_dls

_d_flt


        Prolog_dcmp ed
#ifdef EXPLICIT_FCMP
        movlo   fx, #1

        movhs   fx, #0
#endif
        Return





;===============================================================================        

; FUNCTION: _d_fle

; INPUT: dxh/xhl, dyh/dyl

; OUTPUT: fx = 0 (relation false), 1 (relation true) (invalid if unordered)

;
	AREA |.text|,CODE,READONLY

_dleq

_d_fle


        Prolog_dcmp ed
#ifdef EXPLICIT_FCMP
        movls   fx, #1

        movhi   fx, #0
#endif
        Return



;===============================================================================        

; FUNCTION: _d_fgt

; INPUT: dxh/dxl, dyh/dyl

; OUTPUT: fx = 0 (relation false), 1 (relation true) (invalid if unordered)

;  

	AREA |.text|,CODE,READONLY

_dgr

_d_fgt


        Prolog_dcmp ed
#ifdef EXPLICIT_FCMP
        movgt   fx, #1

        movle   fx, #0 
#endif
        Return



;===============================================================================        

; FUNCTION: _d_fge

; INPUT: dxh/dxl, dyh/dyl

; OUTPUT: fx = 0 (relation false), 1 (relation true) (invalid if unordered)

;  
	AREA |.text|,CODE,READONLY

_dgeq

_d_fge


        Prolog_dcmp ed
#ifdef EXPLICIT_FCMP
        movge   fx, #1

        movlt   fx, #0
#endif
        Return



;===============================================================================        

; FUNCTION: _d_fun

; INPUT: dxh/dxl, dyh/dyl

; OUTPUT: fx = 0 (relation false), 1 (relation true) (invalid on SNaN)

;  
	AREA |.text|,CODE,READONLY

_d_fun

        Prolog_dcmp d

        movvs   fx, #1

        movvc   fx, #0 

        Return



;===============================================================================        

; FUNCTION: _d_for

; INPUT: dxh/dxl, dyh/dyl

; OUTPUT: fx = 0 (relation false), 1 (relation true) (invalid on SNaN)

;  

	AREA |.text|,CODE,READONLY

_d_for

        Prolog_dcmp d

        movvs   fx, #0

        movvc   fx, #1

        Return



;===============================================================================        

; FUNCTION: _dcmpeq

; INPUT: dxh/dxl, dyh/dyl

; OUTPUT: result in flags

;
	AREA |.text|,CODE,READONLY

_dcmpeq

        Prolog_dcmp d

        Return



;===============================================================================        

; FUNCTION: _dcmple

; INPUT: dxh/dxl, dyh/dyl

; OUTPUT: result in flags

	AREA |.text|,CODE,READONLY

_dcmple

        Prolog_dcmp ed

        Return


;===============================================================================        

; FUNCTION: _dcmpge

; INPUT: dxh/dxl, dyh/dyl

; OUTPUT: result in flags

; 
	AREA |.text|,CODE,READONLY

_dcmpge

        Prolog_dcmp ed

        Return



  #ifndef IEEE_STD



;===============================================================================

;

; VFP support functions return the VFP comparison result flags in r0, in the same

; bit position as in FPSCR

;



; Flags for '<':          Nzcv (1000)

#define VFP_LESS_FLAGS    0x80000000

; Flags for '==':         nZCv (0110)

#define VFP_GREATER_FLAGS 0x60000000

; Flags for '>':          nzCv (0010)

#define VFP_EQUAL_FLAGS   0x20000000

; Flags for 'unordered':  nzCV (0011)

#define VFP_UNORDER_FLAGS 0x30000000

	AREA |.text|,CODE,READONLY

_vfp_support_fcmp

	#ifdef IEEE_FULL_OR_FIXED

        mov     sreg03, #0xFF000000 

        cmp     sreg03, fx, LSL #1

        blo     f_compare_check_snan_x

        cmp     sreg03, fy, LSL #1

        blo     f_compare_check_snan_y

        b       f_compare_normal



f_compare_check_snan_x

        tst     fx,#0x00400000          ; check whether x is sNaN

        beq     f_compare_invalid

        cmp     sreg03, fy, LSL #1      ; x is qNaN; check whether y is NaN

        bhs     compare_return_unordered

f_compare_check_snan_y

        tst     fy,#0x00400000          ; y is NaN; check whether y is sNaN

        beq     f_compare_invalid

        b       compare_return_unordered



    #endif                          


; if not in full/fixed IEEE modes, fall through to _vfp_support_fcmpe

	AREA |.text|,CODE,READONLY

_vfp_support_fcmpe

        mov     sreg03, #0xFF000000     ; any input NaNs?

        cmp     sreg03, fx, LSL #1

        cmphs   sreg03, fy, LSL #1

	#ifdef IEEE_FULL_OR_FIXED

        blo     f_compare_invalid

    #else

        blo     compare_return_unordered

    #endif        



f_compare_normal

; check for negative inputs

        cmp     fx,#0

        bicmi   fx,fx,#0x80000000       ; strip sign and negate

        rsbmi   fx,fx,#0

        cmp     fy,#0

        bicmi   fy,fy,#0x80000000       ; strip sign and negate

        rsbmi   fy,fy,#0



; if we arrive here we are only concerned with eq, lt or gt

        cmp     fx, fy

        moveq   r0, #VFP_EQUAL_FLAGS

        movlt   r0, #VFP_LESS_FLAGS

        movgt   r0, #VFP_GREATER_FLAGS 

        Return



	#ifdef IEEE_FULL_OR_FIXED

f_compare_invalid

        LFPState sreg03, sreg12

        SetInvalidE sreg03              ; set invalid

        StFPState sreg03,sreg12         ; store FPSCR

        mov     r0, #VFP_UNORDER_FLAGS

        Return

    #endif



_vfp_support_dcmp

	#ifdef IEEE_FULL_OR_FIXED

        mov     sreg12, #0x00200000     ; check for NaNs

        cmn     sreg12, dxh, LSL #1     ; check whether x has max exponent

        bcc     d_compare_check_nan_y1

        bne     d_compare_check_snan_x

        cmp     dxl,#0                  ; check whether x is inf

        beq     d_compare_check_nan_y1



; x is NaN; check for sNaN

d_compare_check_snan_x

        tst     dxh,#0x00080000         ; check whether x is sNaN                 

        beq     d_compare_invalid



        cmn     sreg12, dyh, LSL #1     ; x is qNaN; check whether y is NaN

        bcc     compare_return_unordered

        bne     d_compare_check_snan_y

        cmp     dyl,#0

        beq     compare_return_unordered

        b       d_compare_check_snan_y 



; x is a normal number; check whether y is NaN

d_compare_check_nan_y1

        cmn     sreg12, dyh, LSL #1     ; check whether y has max exponent

        bcc     d_compare_normal

        bne     d_compare_check_snan_y

        cmp     dyl,#0                  ; check whether y is inf

        beq     d_compare_normal



; y is NaN; check for sNaN

d_compare_check_snan_y

        tst     dyh,#0x00080000         ; check whether y is sNaN 

        beq     d_compare_invalid

        b       compare_return_unordered

    #endif

; if not in full/fixed IEEE modes, fall through to _vfp_support_fcmpe


	AREA |.text|,CODE,READONLY

_vfp_support_dcmpe

        mov     sreg12, #0x00200000     ; check for NaNs

        cmn     sreg12, dxh, LSL #1

        bcc     d_compare_check_nan_y   ; x has not max exponent --> check y

	#ifdef IEEE_FULL_OR_FIXED

        bne     d_compare_invalid

    #else

        bne     compare_return_unordered

    #endif

        cmp     dxl,#0                  ; check whether x is inf

	#ifdef IEEE_FULL_OR_FIXED

        bne     d_compare_invalid

    #else

        bne     compare_return_unordered

    #endif



d_compare_check_nan_y

        cmn     sreg12, dyh, LSL #1

        bcc     d_compare_normal        ; y has not max exponent --> do compare

	#ifdef IEEE_FULL_OR_FIXED

        bne     d_compare_invalid

    #else

        bne     compare_return_unordered

    #endif

        cmp     dyl,#0                  ; check whether y is inf

	#ifdef IEEE_FULL_OR_FIXED

        bne     d_compare_invalid

    #else

        bne     compare_return_unordered

    #endif



d_compare_normal

        orrs    sreg12, dyh, dxh

        bmi     dcmp_checkminus 

        cmp     dxh, dyh

        cmpeq   dxl, dyl

        moveq   r0, #VFP_EQUAL_FLAGS

        movlo   r0, #VFP_LESS_FLAGS

        movhi   r0, #VFP_GREATER_FLAGS

        Return



dcmp_checkminus

        orr     sreg12, dxl, sreg12, LSL #1

        orrs    sreg12, sreg12, dyl

        mov     r0, #VFP_EQUAL_FLAGS

        Return  eq                      ; special case: signed 0



        cmp     dyh, dxh

        cmpeq   dyl, dxl

        movlo   r0, #VFP_LESS_FLAGS

        movhi   r0, #VFP_GREATER_FLAGS

        Return



d_compare_check_nan_x





	#ifdef IEEE_FULL_OR_FIXED

d_compare_invalid

        stmfd   sp!, {lr}

        LFPState lr, sreg12

        SetInvalidEL lr, __wrap_invalid_handler_double  ; set invalid

        StFPState lr,sreg12             ; store FPSCR   

        mov     r0, #VFP_UNORDER_FLAGS

        Return_lr

    #endif



compare_return_unordered

        mov     r0, #VFP_UNORDER_FLAGS

        Return

  #endif



#else



; @@@ The next macros were added for single precision comparison functions



; @@@ The prolog for every single precision comparison

	MACRO
	Prolog_fcmp

; @@@ Added: flush denorms to 0 for IEEE_STD

  #ifdef IEEE_STD

        mov     sreg03, #0x01000000

        cmp     sreg03, fx, LSL #1

        movhi   fx, #0

        cmp     sreg03, fy, LSL #1

        movhi   fy, #0

  #endif  

	MEND



; @@@ Optimised function that checks for any type of NaNs

; @@@ Used in flt, fle, fgt, fge, fcmpe


	MACRO
	FCheckAnyNaN   $label1, $label2

        mov     sreg03, #0xFF000000 

        cmp     sreg03, fx, LSL #1

        cmphs   sreg03, fy, LSL #1

  #ifdef IEEE_FULL_OR_FIXED


        blo.w     $label1

  #else

		blo		$label2

  #endif

	MEND



; @@@ Code for the functions that must check only SNaNs

; @@@ Used in fun, for, feq, fne, fcmp

	MACRO
	FCheckSNaN  $label1, $label2, $label3

        mov     sreg03, #0xFF000000 

        cmp     sreg03, fx, LSL #1

	#ifdef IEEE_FULL_OR_FIXED

        blo.w     $label1

  #else

		blo $label3

  #endif

        cmp     sreg03, fy, LSL #1

	#ifdef IEEE_FULL_OR_FIXED

        blo.w     $label2

  #else

		blo $label3

  #endif

	MEND




; @@@ Negate the operands if at least one of them is negative

; @@@ In IEEE_STD mode just swap the operands if at least one is negative

; @@@ This only works in IEEE_STD because it does not check +0/-0

; @@@ Also, if swapping, unsigned comparisons must be used 

	MACRO
	FCheckNeg

  #ifndef IEEE_STD        

        cmp     fx,#0

        bicmi   fx,fx,#0x80000000       ; strip sign and negate

        rsbmi   fx,fx,#0

        cmp     fy,#0

        bicmi   fy,fy,#0x80000000       ; strip sign and negate

        rsbmi   fy,fy,#0

  #else

        orrs    sreg03, fx, fy

        eormi   fx, fx, fy

        eormi   fy, fx, fy

        eormi   fx, fx, fy

  #endif

	MEND



; @@@ The next macros were added for double precision comparison functions 



; @@@ Added next macro for comparisons 

; @@@ The prolog for every double precision comparison

	MACRO
	Prolog_dcmp

; @@@ Added: flush denorms to 0 for IEEE_STD

  #ifdef IEEE_STD

        mov     sreg12, #0x00200000

        cmp     sreg12, dxh, LSL #1

        movhi   dxh, #0

        movhi   dxl, #0 

        cmp     sreg12, dyh, LSL #1 

        movhi   dyh, #0 

        movhi   dyl, #0 

  #endif

	MEND



; @@@ Check if the number in dxh/dxl or dyh/dyl is a NaN

; @@@ Jump to "label1" or "label2" if necesarry

	MACRO
	DCheckNaN_start  $label1, $label2

  #ifndef IEEE_STD

        mov     sreg12, #0x00200000

        cmn     sreg12, dxh, LSL #1

        bcs     $label1

        cmn     sreg12, dyh, LSL #1

        bcs     $label2

  #endif  

	MEND



; @@@ Check if x is a NaN  

	MACRO
        DCheckNaN_x  $label1, $label2, $label3 

	#ifdef IEEE_FULL_OR_FIXED

        bne.w     $label1

  #else
  
  		bne	$label3

  #endif

; we have an "equal" condition here; need to check dxl    

        cmp     dxl, #0x0

	#ifdef IEEE_FULL_OR_FIXED

        bhi.w     $label1

  #else

		bhi 	$label3

  #endif

; x is not a NaN; check y

        cmn     sreg12, dyh, LSL #1 

        bcc     $label2                  ; y is not a NaN

	MEND



; @@@ Check if y is a NaN  

	MACRO
	DCheckNaN_y  $label1, $label2, $label3

	#ifdef IEEE_FULL_OR_FIXED

        bne.w     $label1

  #else
  
  		bne		$label3

  #endif

        cmp     dyl, #0x0

; we have an "equal" condition here; need to check dxl

	#ifdef IEEE_FULL_OR_FIXED

        bhi.w     $label1

  #else

		bhi $label3

  #endif

        b       $label2

	MEND



;@@@ Define type of conditional moves for single precision functions

;@@@ Unsigned tests if IEEE_STD mode is used and signed othwerwise

	GBLS MMOVEQ
	GBLS MMOVNE
	GBLS MMOVLT
	GBLS MMOVLE
	GBLS MMOVGT
	GBLS MMOVGE

  #ifdef IEEE_STD

MMOVEQ SETS "moveq"

MMOVNE SETS "movne"

MMOVLT SETS "movlo"

MMOVLE SETS "movls"

MMOVGT SETS "movhi"

MMOVGE SETS "movhs"

  #else

MMOVEQ SETS "moveq"

MMOVNE SETS "movne"

MMOVLT SETS "movlt"

MMOVLE SETS "movle"

MMOVGT SETS "movgt"

MMOVGE SETS "movge"

  #endif  



;===============================================================================

; @@@ Modified: optimized functions

;===============================================================================



;===============================================================================        

; FUNCTION: _f_cmp

; INPUT: fx, fy

; OUTPUT: fx = 0-eq, 1-lt, 2-gt, 3-un (invalid if SNaN)

;
	AREA |.text|,CODE,READONLY

_f_cmp

        Prolog_fcmp

; we don't care about NaNs at all in IEEE_STD mode

  #ifndef IEEE_STD

        FCheckSNaN f_compare_un_inv_snan_x, f_compare_un_inv_snan_y, f_cmp_return_three

  #endif // IEEE_STD

; if we arrive here we are only concerned with eq, lt or gt

        FCheckNeg

        cmp     fx, fy

        $MMOVEQ  fx, #0

        $MMOVLT  fx, #1

        $MMOVGT  fx, #2

        Return

f_cmp_return_three

		mov		fx, #3
		
		Return


;===============================================================================        

; FUNCTION: _f_cmpe

; INPUT: fx, fy

; OUTPUT: fx = 0-eq, 1-lt, 2-gt, 3-un (invalid if unordered)

;
	AREA |.text|,CODE,READONLY
f_fcmpe_return_three

	mov		fx, #3
	
	Return
	
	AREA |.text|,CODE,READONLY

_f_cmpe

        Prolog_fcmp

; we don't care about NaNs at all in IEEE_STD mode

  #ifndef IEEE_STD

        FCheckAnyNaN f_compare_un_inv, f_fcmpe_return_three

  #endif // IEEE_STD

; if we arrive here we are only concerned with eq, lt or gt

        FCheckNeg

        cmp     fx, fy

        $MMOVEQ  fx, #0

        $MMOVLT  fx, #1

        $MMOVGT  fx, #2

        Return



;===============================================================================        

; FUNCTION: _f_fne

; INPUT: fx, fy

; OUTPUT: condition codes set in apsr

;
	AREA |.text|,CODE,READONLY
	
_fneq

_f_fne


        Prolog_fcmp

; set exceptions only in IEEE_FULL and IEEE_FIXED modes

; also change result to false in IEEE_NO_FENV mode

  #ifndef IEEE_STD

        FCheckSNaN  f_fne_un_inv_snan_x, f_fne_un_inv_snan_y, fne_return_true

  #endif 

  #ifndef IEEE_STD                          
;  don't check +0/-0 in IEEE_STD   

        orr     sreg03, fx, fy

        movs    sreg03, sreg03, LSL #1
#ifdef EXPLICIT_FCMP
		moveq fx, #0
#endif
        
        bne		fneq_exit
        
        mrs		r12, apsr
        
        orr 	r12, r12, #0x40000000
        
        msr	apsr, r12

        Return

  #endif

fneq_exit

        cmp     fx, fy
#ifdef EXPLICIT_FCMP
        $MMOVNE  fx, #1

        $MMOVEQ  fx, #0
#endif 
       
        mrs		r12, apsr
        
        bicne	r12, r12, #0x40000000
        
        orreq	r12, r12, #0x40000000
        
        msr		apsr, r12

        Return

fne_return_true
#ifdef EXPLICIT_FCMP
		mov fx,#1
#endif

		mrs		r12, apsr
		
		bic		r12, r12, #0x40000000
		
		msr		apsr, r12
		
		Return
		

;===============================================================================        

; FUNCTION: _f_feq

; INPUT: fx, fy

; OUTPUT: condition codes set in apsr

;
	AREA |.text|,CODE,READONLY
	
_feq

_f_feq


        Prolog_fcmp

; set exceptions only in IEEE_FULL and IEEE_FIXED modes

; also change result to 0 in IEEE_NO_FENV mode

  #ifndef IEEE_STD

        FCheckSNaN  f_feq_un_inv_snan_x, f_feq_un_inv_snan_y, feq_return_false

  #endif    

  #ifndef IEEE_STD                          
;  don't check +0/-0 in IEEE_STD

        orr     sreg03, fx, fy

        movs    sreg03, sreg03, LSL #1
#ifdef EXPLICIT_FCMP
		moveq fx, #1
#endif
        
        bne		feq_exit
        
        mrs		r12, apsr
        
        orr		r12, r12, #0x40000000
        
        msr		apsr, r12

        Return

  #endif

feq_exit

        cmp     fx, fy
#ifdef EXPLICIT_FCMP
        $MMOVEQ  fx, #1

        $MMOVNE  fx, #0 
#endif		
        
        mrs		r12, apsr
        
        orreq	r12, r12, #0x40000000
        
        bicne	r12, r12, #0x40000000
        
        msr		apsr, r12 

        Return
        
feq_return_false

#ifdef EXPLICIT_FCMP
		mov fx, #0
#endif
		mrs		r12, apsr
		
		bic		r12, r12, #0x40000000
		
		msr		apsr, r12
		
		Return


;===============================================================================        

; FUNCTION: _fcmpeq

; INPUT: fx, fy

; OUTPUT: result in flags

; 
	AREA |.text|,CODE,READONLY

_fcmpeq

        Prolog_fcmp

; set exceptions only in IEEE_FULL and IEEE_FIXED modes

; also change result to 0 in IEEE_NO_FENV mode

  #ifndef IEEE_STD

        mov     sreg03, #0xFF000000 

        cmp     sreg03, fx, LSL #1

	#ifdef IEEE_FULL_OR_FIXED

        blo.w     f_compare_un_inv_snan_x_f

    #else

        bhs     fcmpeq_check_y

        movs    sreg02, #1

        Return

	#endif

fcmpeq_check_y

        cmp     sreg03, fy, LSL #1

	#ifdef IEEE_FULL_OR_FIXED

        blo.w     f_compare_un_inv_snan_y_f

    #else

        bhs     fcmpeq_do_compare

        movs    sreg02, #1

        Return

	#endif

  #endif                                    
; #ifndef IEEE_STD

fcmpeq_do_compare

  #ifndef IEEE_STD                          
;  don't check +0/-0 in IEEE_STD

        orr     sreg03, fx, fy

        movs    sreg03, sreg03, LSL #1

        Return  eq

  #endif                                    
; #ifndef IEEE_STD

        cmp     fx, fy 

        Return

;===============================================================================        

; FUNCTION: _f_flt

; INPUT: fx, fy

; OUTPUT: condition codes set in apsr

;
	AREA |.text|,CODE,READONLY

_fls

_f_flt


        Prolog_fcmp

; set exceptions only in IEEE_FULL and IEEE_FIXED modes

; also change result to 0 in IEEE_NO_FENV mode

  #ifndef IEEE_STD

        FCheckAnyNaN f_fls_un_inv, f_fls_return_false

  #endif   

        FCheckNeg

        cmp     fx, fy
#ifdef EXPLICIT_FCMP
        $MMOVLT  fx, #1

        $MMOVGE  fx, #0 
#endif		

#ifndef IEEE_STD
      
        mrs		r12, apsr
        
        orrge	r12, r12, #0x20000000
        
        biclt	r12, r12, #0x20000000
        
        msr		apsr, r12
#else

		mrs		r12, apsr
		
		orrhs	r12, r12, #0x20000000
		
		biclo	r12, r12, #0x20000000
		
		msr		apsr, r12
#endif

        Return
        
f_fls_return_false
#ifdef EXPLICIT_FCMP
		mov fx, #0
#endif
		mrs		r12, apsr
		
        orr		r12, r12, #0x20000000
        
        msr		apsr, r12
        
        Return


;===============================================================================        

; FUNCTION: _f_fle

; INPUT: fx, fy

; OUTPUT: condition codes set in apsr

;
;===============================================================================
	
	AREA |.text|,CODE,READONLY

_fleq

_f_fle


        Prolog_fcmp

; set exceptions only in IEEE_FULL and IEEE_FIXED modes

; also change result to 0 in IEEE_NO_FENV mode

  #ifndef IEEE_STD

        FCheckAnyNaN  f_fle_un_inv, f_fle_return_false

  #endif   

        FCheckNeg

        cmp     fx, fy
#ifdef EXPLICIT_FCMP
        $MMOVLE  fx, #1

        $MMOVGT  fx, #0 
#endif
        
 #ifndef IEEE_STD
        
        mrs		r12, apsr
        
        orrgt	r12, r12, #0x20000000
        
        bicgt	r12, r12, #0x40000000
        
        bicle	r12, r12, #0x20000000
        
        orrle	r12, r12, #0x40000000
        
        msr		apsr, r12
#else

		mrs		r12, apsr
		
		orrhi	r12, r12, #0x20000000
		
		bichi	r12, r12, #0x40000000
		
		bicls	r12, r12, #0x20000000
		
		orrls	r12, r12, #0x40000000
		
		msr		apsr, r12
#endif
        
        Return

f_fle_return_false

#ifdef EXPLICIT_FCMP
		mov fx, #0
#endif

		mrs		r12, apsr
		
        bic 	r12, r12, #0x40000000
        
        orr		r12, r12, #0x20000000
        
        msr		apsr, r12
        
        Return

;===============================================================================        

; FUNCTION: _fcmple

; INPUT: fx, fy

; OUTPUT: result in flags

;
	AREA |.text|,CODE,READONLY

_fcmple

        Prolog_fcmp

; set exceptions only in IEEE_FULL and IEEE_FIXED modes

; also change result to 0 in IEEE_NO_FENV mode

  #ifndef IEEE_STD

        mov     sreg03, #0xFF000000 

        cmp     sreg03, fx, LSL #1

        cmphs   sreg03, fy, LSL #1

	#ifdef IEEE_FULL_OR_FIXED

        blo.w     f_compare_un_inv_f_nols

    #else

        bhs     _fcmple_compare

        mov     sreg02, #0xFF000000

        adds    sreg02, sreg02, #0x10000000 

        Return

	#endif

  #endif                                    
; #ifndef IEEE_STD

_fcmple_compare

  #ifndef IEEE_STD                          
;  don't check +0/-0 in IEEE_STD

        orr     sreg03, fx, fy

        movs    sreg03, sreg03, LSL #1

        cmpeq   fx, fx

        Return  eq

  #endif                                    
; #ifndef IEEE_STD

        orrs    sreg02, fx, fy

        bmi     _fcmple_compare_2

        cmp     fx, fy

        Return

_fcmple_compare_2

        cmp     fy, fx

        Return

;=============================================================

;===============================================================================        

; FUNCTION: _f_fgt

; INPUT: fx, fy

; OUTPUT: fx = 0 (relation false), 1 (relation true) (invalid if unordered)

;

	AREA |.text|,CODE,READONLY

_fgr

_f_fgt


        Prolog_fcmp

; set exceptions only in IEEE_FULL and IEEE_FIXED modes

; also change result to 0 in IEEE_NO_FENV mode

  #ifndef IEEE_STD

        FCheckAnyNaN  f_fgr_un_inv, f_fgr_return_false

  #endif   

        FCheckNeg

        cmp     fx, fy
#ifdef EXPLICIT_FCMP
        $MMOVGT  fx, #1

        $MMOVLE  fx, #0 
#endif		
        
 #ifndef IEEE_STD
        
        mrs		r12, apsr
        
        bicle	r12, r12, #0x20000000
        
        orrgt	r12, r12, #0x20000000
        
        msr		apsr, r12
 #else
 
 		mrs		r12, apsr
 		
 		bicls	r12, r12, #0x20000000
 		
 		orrhi	r12, r12, #0x20000000
 		
 		msr		apsr, r12
 		
 #endif


        Return

f_fgr_return_false
#ifdef EXPLICIT_FCMP
		mov fx, #0
#endif

		mrs		r12, apsr
		
		bic		r12, r12, #0x20000000
		
		msr		apsr, r12
		
		Return

;===============================================================================        

; FUNCTION: _f_fge

; INPUT: fx, fy

; OUTPUT: fx = 0 (relation false), 1 (relation true) (invalid if unordered)

;  
	AREA |.text|,CODE,READONLY

_fgeq

_f_fge


        Prolog_fcmp

; set exceptions only in IEEE_FULL and IEEE_FIXED modes

; also change result to 0 in IEEE_NO_FENV mode

  #ifndef IEEE_STD

        FCheckAnyNaN f_fge_un_inv, f_fge_return_false

  #endif   

        FCheckNeg

        cmp     fx, fy
        
 #ifdef EXPLICIT_FCMP
         $MMOVGE  fx, #1

         $MMOVLT  fx, #0 
 #endif
               
 #ifndef IEEE_STD
       
        mrs		r12, apsr
        
        biclt	r12, r12, #0x20000000
        
        orrge	r12, r12, #0x20000000
        
        msr		apsr, r12
#else
		mrs		r12, apsr
		
		biclo	r12, r12, #0x20000000
		
		orrhs	r12, r12, #0x20000000
		
		msr		apsr, r12
#endif

        Return

f_fge_return_false

#ifdef EXPLICIT_FCMP
		mov fx, #0
#endif

		mrs		r12, apsr
		
		bic		r12, r12, #0x20000000
		
		msr		apsr, r12
		
		Return


;===============================================================================        

; FUNCTION: _fcmpge

; INPUT: fx, fy

; OUTPUT: result in flags

; 
 
	AREA |.text|,CODE,READONLY

_fcmpge

        Prolog_fcmp

; set exceptions only in IEEE_FULL and IEEE_FIXED modes

; also change result to 0 in IEEE_NO_FENV mode

  #ifndef IEEE_STD

        mov     sreg03, #0xFF000000 

        cmp     sreg03, fx, LSL #1

        cmphs   sreg03, fy, LSL #1

	#ifdef IEEE_FULL_OR_FIXED

        blo.w     f_compare_un_inv_f_nohs

    #else

        bhs     _fcmpge_compare

        Return

	#endif

  #endif                                    
; #ifndef IEEE_STD

_fcmpge_compare

  #ifndef IEEE_STD                          
;  don't check +0/-0 in IEEE_STD

        orr     sreg03, fx, fy

        movs    sreg03, sreg03, LSL #1

        cmpeq   fx, fx

        Return  eq

  #endif                                    
; #ifndef IEEE_STD

        orrs    sreg02, fx, fy

        bmi     _fcmpge_compare_2

        cmp     fx, fy

        Return

_fcmpge_compare_2

        cmp     fy, fx

        Return



;===============================================================================        

; FUNCTION: _f_fun

; INPUT: fx, fy

; OUTPUT: fx = 0 (relation false), 1 (relation true) (invalid on SNaN)

;  
	AREA |.text|,CODE,READONLY

_f_fun

; don't care about special values on IEEE_STD; always return 0

  #ifdef IEEE_STD

        mov     fx, #0

        Return

  #else

        FCheckSNaN f_compare_un_inv_snan_x, f_compare_un_inv_snan_y, f_fun_return_true

        mov     fx, #0                  ; condition is false

        Return

  #endif

f_fun_return_true

		mov		fx, #1
		
		Return

;===============================================================================        

; FUNCTION: _f_for

; INPUT: fx, fy

; OUTPUT: fx = 0 (relation false), 1 (relation true) (invalid on SNaN)

; 
	AREA |.text|,CODE,READONLY

_f_for

; don't care about special values on IEEE_STD; always return 1

  #ifdef IEEE_STD

        mov     fx, #1

        Return

  #else

        FCheckSNaN f_compare_un_inv_snan_x, f_compare_un_inv_snan_y, f_for_return_false

        mov     fx, #1                  ; condition is true

        Return

  #endif

f_for_return_false

		mov		fx, #0
		
		Return

	#ifdef IEEE_FULL_OR_FIXED


f_compare_un_inv

; @@@ Modified: added LFPState

        LFPState sreg03, sreg12

        SetInvalidE sreg03              ; set invalid

        StFPState sreg03,sreg12         ; store FPSCR

        mov     fx, sreg02

        Return
        
f_feq_un_inv

; @@@ Modified: added LFPState

        LFPState sreg03, sreg12

        SetInvalidE sreg03              ; set invalid

        StFPState sreg03,sreg12         ; store FPSCR
        
        mrs	r0, apsr
        
        bic	 r0, r0, #0x40000000
        
        msr apsr, r0
        
        Return

		

f_fne_un_inv

; @@@ Modified: added LFPState


        LFPState sreg03, sreg12

        SetInvalidE sreg03              ; set invalid

        StFPState sreg03,sreg12         ; store FPSCR

        mrs	r12, apsr
        
        bic	 r12, r12, #0x40000000
        
        msr apsr, r12

		Return

        
f_fls_un_inv

; @@@ Modified: added LFPState

        LFPState sreg03, sreg12

        SetInvalidE sreg03              ; set invalid

        StFPState sreg03,sreg12         ; store FPSCR
        
        mrs	r12, apsr
        
        orr	 r12, r12, #0x20000000
        
        msr apsr, r12

        Return
        
f_fle_un_inv

; @@@ Modified: added LFPState

        LFPState sreg03, sreg12

        SetInvalidE sreg03              ; set invalid

        StFPState sreg03,sreg12         ; store FPSCR

        mrs	r12, apsr
        
        bic r12, r12, #0x40000000
        
        orr	r12, r12, #0x20000000
        
        msr apsr, r12       
        
        Return
        
f_fge_un_inv

; @@@ Modified: added LFPState

        LFPState sreg03, sreg12

        SetInvalidE sreg03              ; set invalid

        StFPState sreg03,sreg12         ; store FPSCR

        mrs	r12, apsr
        
        bic r12, r12, #0x20000000
        
        msr apsr, r12

        Return
        
f_fgr_un_inv

; @@@ Modified: added LFPState

        LFPState sreg03, sreg12

        SetInvalidE sreg03              ; set invalid

        StFPState sreg03,sreg12         ; store FPSCR

        mrs	r12, apsr
        
        bic r12, r12, #0x20000000
        
        msr apsr, r12       

        Return


	
f_compare_un_inv_f

; @@@ Modified: added LFPState

        LFPState sreg03, sreg12

        SetInvalidE sreg03              ; set invalid

        StFPState sreg03,sreg12         ; store FPSCR

        movs    sreg02, #1              ; clear all flags here

        Return


f_compare_un_inv_f_nols

; @@@ Modified: added LFPState

        LFPState sreg03, sreg12

        SetInvalidE sreg03              ; set invalid

        StFPState sreg03,sreg12         ; store FPSCR

        mov     sreg02, #0xFF000000

        adds    sreg02, sreg02, #0x10000000 

        Return



f_compare_un_inv_f_nohs

; @@@ Modified: added LFPState

        LFPState sreg03, sreg12

        SetInvalidE sreg03              ; set invalid

        StFPState sreg03,sreg12         ; store FPSCR

        mov     sreg02, #0x0

        adds    sreg02, sreg02, #0x0

        Return



f_compare_un_inv_snan_x

; when we arrive here we know that x is a NaN

; need to check if it is a SNaN

        tst     fx, #0x00400000         ; test SNaN bit 

        beq     f_compare_un_inv        ; set invalid on SNaN

; otherwise we need to check y

; arrive here with sreg03 = 0xFF000000

        cmp     sreg03, fy, LSL #1

        movhs   fx, sreg02              ; load result in fx

        Return  hs                      ; finish if y is not a NaN

; otherwise fall through the test for y

f_compare_un_inv_snan_y

; when we arrive here we know that y is a NaN

; need to check if it is a SNaN

        tst     fy, #0x00400000         ; test SNaN bit

        beq     f_compare_un_inv        ; set invalid on SNaN

        Return                          ; return

;=======================================================================================

f_feq_un_inv_snan_x

; when we arrive here we know that x is a NaN

; need to check if it is a SNaN

        tst     fx, #0x00400000         ; test SNaN bit 

        beq     f_feq_un_inv        ; set invalid on SNaN

; otherwise we need to check y

; arrive here with sreg03 = 0xFF000000

        cmp     sreg03, fy, LSL #1
        
        blo		f_feq_un_inv_snan_y
        
        mrs		r12, apsr
        
        bic		r12, r12, #0x40000000
        
        msr		apsr, r12
        
        Return


; otherwise fall through the test for y

f_feq_un_inv_snan_y

; when we arrive here we know that y is a NaN

; need to check if it is a SNaN

        tst     fy, #0x00400000         ; test SNaN bit

        beq     f_feq_un_inv        ; set invalid on SNaN

        mrs		r12, apsr
        
        bic		r12, r12, #0x40000000
        
        msr		apsr, r12

        Return                          ; return
;======================================================================================
;=======================================================================================

f_fne_un_inv_snan_x

; when we arrive here we know that x is a NaN

; need to check if it is a SNaN

        tst     fx, #0x00400000         ; test SNaN bit 

        beq     f_feq_un_inv        ; set invalid on SNaN

; otherwise we need to check y

; arrive here with sreg03 = 0xFF000000

        cmp     sreg03, fy, LSL #1
        
        blo		f_fne_un_inv_snan_y
        
        mrs		r12, apsr
        
        bic		r12, r12, #0x40000000
        
        msr		apsr, r12
        
        Return


; otherwise fall through the test for y

f_fne_un_inv_snan_y

; when we arrive here we know that y is a NaN

; need to check if it is a SNaN

        tst     fy, #0x00400000         ; test SNaN bit

        beq     f_feq_un_inv        ; set invalid on SNaN

        mrs		r12, apsr
        
        bic		r12, r12, #0x40000000
        
        msr		apsr, r12

        Return                          ; return
;======================================================================================

f_compare_un_inv_snan_x_f

; when we arrive here we know that x is a NaN

; need to check if it is a SNaN

        tst     fx, #0x00400000         ; test SNaN bit 

        beq     f_compare_un_inv_f      ; set invalid on SNaN

; otherwise we need to check y

; arrive here with sreg03 = 0xFF000000

        cmp     sreg03, fy, LSL #1

        blo     f_compare_un_inv_snan_y_f

        movs    sreg02, #1

        Return                          ; clear flags and return

f_compare_un_inv_snan_y_f

; when we arrive here we know that y is a NaN

; need to check if it is a SNaN

        tst     fy, #0x00400000         ; test SNaN bit

        beq     f_compare_un_inv_f      ; set invalid on SNaN

        movs    sreg02, #1

        Return



  #endif  



; @@@ Double precision comparison functions



;===============================================================================        

; FUNCTION: _d_cmp

; INPUT: dxh/dxl, dyh/dyl

; OUTPUT: fx = 0-eq, 1-lt, 2-gt, 3-un (invalid if SNaN)

;
	AREA |.text|,CODE,READONLY

_d_cmp

        Prolog_dcmp

        DCheckNaN_start x_maybe_NaN_cmp, y_maybe_NaN_cmp

dcmp_normal

; inline the normal processing here

        orrs    sreg12, dyh, dxh

        bmi     dcmp_checkminus 

        cmp     dxh, dyh

        cmpeq   dxl, dyl

        moveq   fx, #0

        movlo   fx, #1

        movhi   fx, #2

        Return
        
dcmp_return_three

		mov		fx, #3
		
		Return

dcmp_checkminus

  #ifndef IEEE_STD                          
; don't check +0/-0 in IEEE_STD mode

        orr     sreg12, dxl, sreg12, LSL #1

        orrs    sreg12, sreg12, dyl

        moveq   fx, #0

        Return  eq                      ; special case: signed 0

  #endif

        cmp     dyh, dxh

        cmpeq   dyl, dxl

        moveq   fx, #0

        movlo   fx, #1

        movhi   fx, #2

        Return

  #ifndef IEEE_STD

x_maybe_NaN_cmp

        DCheckNaN_x d_compare_un_inv_snan_x, dcmp_normal, dcmp_return_three 

y_maybe_NaN_cmp

        DCheckNaN_y d_compare_un_inv_snan_y, dcmp_normal, dcmp_return_three

  #endif



;===============================================================================        

; FUNCTION: _d_cmpe

; INPUT: fxh/dxl, dyh/dyl

; OUTPUT: fx = 0-eq, 1-lt, 2-gt, 3-un (invalid if unordered)

;
	AREA |.text|,CODE,READONLY

_d_cmpe

        Prolog_dcmp

        DCheckNaN_start x_maybe_NaN_cmpe, y_maybe_NaN_cmpe

dcmpe_normal

; inline the normal processing here

        orrs    sreg12, dyh, dxh

        bmi     dcmpe_checkminus 

        cmp     dxh, dyh

        cmpeq   dxl, dyl

        moveq   fx, #0

        movlo   fx, #1

        movhi   fx, #2

        Return
        
dcmpe_return_three

		mov		fx, #3
		
		Return

dcmpe_checkminus

  #ifndef IEEE_STD                          
; don't check +0/-0 in IEEE_STD mode

        orr     sreg12, dxl, sreg12, LSL #1

        orrs    sreg12, sreg12, dyl

        moveq   fx, #0

        Return  eq                      ; special case: signed 0

  #endif

        cmp     dyh, dxh

        cmpeq   dyl, dxl

        moveq   fx, #0

        movlo   fx, #1

        movhi   fx, #2

        Return

  #ifndef IEEE_STD

x_maybe_NaN_cmpe

        DCheckNaN_x d_compare_un_inv, dcmpe_normal, dcmpe_return_three 

y_maybe_NaN_cmpe

        DCheckNaN_y d_compare_un_inv, dcmpe_normal, dcmpe_return_three

  #endif



;===============================================================================        

; FUNCTION: _d_fne

; INPUT: dxh/dxl, dyh/dyl

; OUTPUT: condition codes in apsr

;
	AREA |.text|,CODE,READONLY

_dneq

_d_fne


        Prolog_dcmp

        DCheckNaN_start x_maybe_NaN_fne, y_maybe_NaN_fne

dfne_normal

; inline the normal processing here

        orrs    sreg12, dyh, dxh

        bmi     dfne_checkminus 

        cmp     dxh, dyh

        cmpeq   dxl, dyl
        
#ifdef EXPLICIT_FCMP
        movne   fx, #1

        moveq   fx, #0
#endif

        Return

dfne_return_true
#ifdef EXPLICIT_FCMP
		mov fx, #1
#endif

		mrs		r12, apsr
		
		bic		r12, r12, #0x40000000
		
		msr		apsr, r12
		
		Return
		
dfne_checkminus

  #ifndef IEEE_STD                          
; don't check +0/-0 in IEEE_STD mode

        orr     sreg12, dxl, sreg12, LSL #1

        orrs    sreg12, sreg12, dyl
#ifdef EXPLICIT_FCMP
		moveq fx, #0
#endif
        Return  eq                      ; special case: signed 0

  #endif

        cmp     dyh, dxh

        cmpeq   dyl, dxl
#ifdef EXPLICIT_FCMP
		movne fx, #1
		moveq fx, #0
#endif
        Return

  #ifndef IEEE_STD

x_maybe_NaN_fne

        DCheckNaN_x d_dne_un_inv_snan_x, dfne_normal, dfne_return_true

y_maybe_NaN_fne

        DCheckNaN_y d_dne_un_inv_snan_y, dfne_normal, dfne_return_true

  #endif



;===============================================================================        

; FUNCTION: _d_feq

; INPUT: dxh/dxl, dyh/dyl

; OUTPUT: fx = 0 (relation false), 1 (relation true) (invalid if SNaN)

;
	AREA |.text|,CODE,READONLY

_deq

_d_feq


        Prolog_dcmp

        DCheckNaN_start x_maybe_NaN_feq, y_maybe_NaN_feq

dfeq_normal

; inline the normal processing here

        orrs    sreg12, dyh, dxh

        bmi     dfeq_checkminus 

        cmp     dxh, dyh

        cmpeq   dxl, dyl
#ifdef EXPLICIT_FCMP
        moveq   fx, #1

        movne   fx, #0
#endif
        Return
        
dfeq_return_false
#ifdef EXPLICIT_FCMP
        mov   fx, #0
#endif
		mrs		r12, apsr
		
		bic		r12, r12, #0x40000000
		
		msr		apsr, r12
		
		Return

dfeq_checkminus

  #ifndef IEEE_STD                          
; don't check +0/-0 in IEEE_STD mode

        orr     sreg12, dxl, sreg12, LSL #1

        orrs    sreg12, sreg12, dyl
#ifdef EXPLICIT_FCMP
		moveq fx, #1
#endif
        Return  eq                      ; special case: signed 0

  #endif

        cmp     dyh, dxh

        cmpeq   dyl, dxl
#ifdef EXPLICIT_FCMP
        moveq   fx, #1

        movne   fx, #0
#endif
        Return

  #ifndef IEEE_STD

x_maybe_NaN_feq

        DCheckNaN_x d_deq_un_inv_snan_x, dfeq_normal, dfeq_return_false 

y_maybe_NaN_feq

        DCheckNaN_y d_deq_un_inv_snan_y, dfeq_normal, dfeq_return_false

  #endif



;===============================================================================        

; FUNCTION: _dcmpeq

; INPUT: dxh/dxl, dyh/dyl

; OUTPUT: result in flags

;
	AREA |.text|,CODE,READONLY

_dcmpeq

        Prolog_dcmp

        DCheckNaN_start x_maybe_NaN_cmpeq, y_maybe_NaN_cmpeq

dcmpeq_normal

; inline the normal processing here

        orrs    sreg12, dyh, dxh

        bmi     dcmpeq_checkminus 

        cmp     dxh, dyh

        cmpeq   dxl, dyl

        Return

dcmpeq_checkminus

  #ifndef IEEE_STD                          
; don't check +0/-0 in IEEE_STD mode

        orr     sreg12, dxl, sreg12, LSL #1

        orrs    sreg12, sreg12, dyl 

        Return  eq                      ; special case: signed 0

  #endif

        cmp     dyh, dxh

        cmpeq   dyl, dxl

        Return

  #ifndef IEEE_STD

x_maybe_NaN_cmpeq

	#ifdef IEEE_FULL_OR_FIXED

        bne.w     d_compare_un_inv_snan_x_f

    #else

        beq     _dcmpeq_2

        movs    sreg12, #1

        Return

	#endif

; we have an "equal" condition here; need to check dxl    

_dcmpeq_2

        cmp     dxl, #0x0

	#ifdef IEEE_FULL_OR_FIXED

        bhi.w     d_compare_un_inv_snan_x_f

    #else

        bls     _dcmpeq_3

        movs    sreg12, #1

        Return

	#endif

; x is not a NaN; check y

_dcmpeq_3

        cmn     sreg12, dyh, LSL #1 

        bcc     dcmpeq_normal

y_maybe_NaN_cmpeq

	#ifdef IEEE_FULL_OR_FIXED

        bne.w     d_compare_un_inv_snan_y_f

    #else

        beq     _dcmpeq_4

        movs    sreg12, #1

        Return

	#endif
_dcmpeq_4

        cmp     dyl, #0x0

; we have an "equal" condition here; need to check dxl

	#ifdef IEEE_FULL_OR_FIXED

        bhi.w     d_compare_un_inv_snan_y_f

    #else

        bls     dcmpeq_normal

        movs    sreg12, #1

        Return

	#endif

        b       dcmpeq_normal

  #endif                                    
; #ifndef IEEE_STD



;===============================================================================        

; FUNCTION: _d_flt

; INPUT: dxh/dxl, dyh/dyl

; OUTPUT: condition codes in apsr

;
	AREA |.text|,CODE,READONLY

_dls

_d_flt


        Prolog_dcmp

        DCheckNaN_start x_maybe_NaN_flt, y_maybe_NaN_flt

dflt_normal

; inline the normal processing here

        orrs    sreg12, dyh, dxh

        bmi     dflt_checkminus 

        cmp     dxh, dyh

        cmpeq   dxl, dyl
#ifdef EXPLICIT_FCMP
        movlo   fx, #1

        movhs   fx, #0
#endif
        Return
        
dflt_return_false
#ifdef EXPLICIT_FCMP
		mov fx, #0
#endif

		mrs		r12, apsr
		
		orr		r12, r12, #0x20000000
		
		msr		apsr, r12
		
		Return

dflt_checkminus

  #ifndef IEEE_STD                          
; don't check +0/-0 in IEEE_STD mode

        orr     sreg12, dxl, sreg12, LSL #1

        orrs    sreg12, sreg12, dyl
        
#ifdef EXPLICIT_FCMP
		moveq	fx, #0
#endif
        
        bne		dflt_not_signed_zero
		
		mrs		r12, apsr
		
		orr		r12, r12, #0x20000000
		
		msr		apsr, r12
		
        Return  eq                     ; special case: signed 0

dflt_not_signed_zero

  #endif

        cmp     dyh, dxh

        cmpeq   dyl, dxl
        
#ifdef EXPLICIT_FCMP
        movlo   fx, #1

        movhs   fx, #0
#endif
        Return

  #ifndef IEEE_STD

x_maybe_NaN_flt

        DCheckNaN_x d_dls_un_inv, dflt_normal, dflt_return_false

y_maybe_NaN_flt

		DCheckNaN_y d_dls_un_inv, dflt_normal, dflt_return_false

  #endif



;===============================================================================        

; FUNCTION: _d_fle

; INPUT: dxh/xhl, dyh/dyl

; OUTPUT: condition codes in apsr

;
	AREA |.text|,CODE,READONLY

_dleq

_d_fle


        Prolog_dcmp

        DCheckNaN_start x_maybe_NaN_fle, y_maybe_NaN_fle

dfle_normal

; inline the normal processing here

        orrs    sreg12, dyh, dxh

        bmi     dfle_checkminus 

        cmp     dxh, dyh

        cmpeq   dxl, dyl
#ifdef EXPLICIT_FCMP
        movls   fx, #1

        movhi   fx, #0
#endif
        Return
        
dfle_return_false
#ifdef EXPLICIT_FCMP
		mov fx, #0
#endif

		mrs		r12, apsr
		
		bic		r12, r12, #0x40000000
		
		orr		r12, r12, #0x20000000
		
		msr		apsr, r12
		
		Return

dfle_checkminus

  #ifndef IEEE_STD                          
; don't check +0/-0 in IEEE_STD mode

        orr     sreg12, dxl, sreg12, LSL #1

        orrs    sreg12, sreg12, dyl
        
#ifdef EXPLICIT_FCMP
		moveq fx, #1
#endif

        bne		dfle_not_signed_zero

		mrs		r12, apsr
		
		bic		r12, r12, #0x20000000
		
		orr		r12, r12, #0x40000000
		
		msr		apsr, r12
		
        Return  eq                      ; special case: signed 0
        
dfle_not_signed_zero
  #endif

        cmp     dyh, dxh

        cmpeq   dyl, dxl
        
#ifdef EXPLICIT_FCMP
        movls   fx, #1

        movhi   fx, #0
#endif 
        Return

  #ifndef IEEE_STD

x_maybe_NaN_fle

        DCheckNaN_x d_dle_un_inv, dfle_normal, dfle_return_false

y_maybe_NaN_fle

        DCheckNaN_y d_dle_un_inv, dfle_normal, dfle_return_false

  #endif



;===============================================================================        

; FUNCTION: _dcmple

; INPUT: dxh/dxl, dyh/dyl

; OUTPUT: result in flags

;  
	AREA |.text|,CODE,READONLY

_dcmple

        Prolog_dcmp

        DCheckNaN_start x_maybe_NaN_dcmple, y_maybe_NaN_dcmple

dcmple_normal

; inline the normal processing here

        orrs    sreg12, dyh, dxh

        bmi     dcmple_checkminus 

        cmp     dxh, dyh

        cmpeq   dxl, dyl

        Return

dcmple_checkminus

  #ifndef IEEE_STD                          
; don't check +0/-0 in IEEE_STD mode

        orr     sreg12, dxl, sreg12, LSL #1

        orrs    sreg12, sreg12, dyl

        cmpeq   dxh, dxh

        Return  eq                      ; special case: signed 0

  #endif

        cmp     dyh, dxh

        cmpeq   dyl, dxl

        Return

  #ifndef IEEE_STD  

x_maybe_NaN_dcmple

	#ifdef IEEE_FULL_OR_FIXED

        bne.w     d_compare_un_inv_f_nols

    #else

        beq     _dcmple_2

        mov     sreg12, #0xFF000000

        adds    sreg12, sreg12, #0x10000000 

        Return

	#endif

; we have an "equal" condition here; need to check dxl    

_dcmple_2

        cmp     dxl, #0x0

	#ifdef IEEE_FULL_OR_FIXED

        bhi.w     d_compare_un_inv_f_nols

    #else

        bls     _dcmple_3

        mov     sreg12, #0xFF000000

        adds    sreg12, sreg12, #0x10000000 

        Return

	#endif

; x is not a NaN; check y

_dcmple_3

        cmn     sreg12, dyh, LSL #1 

        bcc     dcmple_normal

y_maybe_NaN_dcmple

	#ifdef IEEE_FULL_OR_FIXED

        bne.w     d_compare_un_inv_f_nols

    #else

        beq     _dcmple_4

        mov     sreg12, #0xFF000000

        adds    sreg12, sreg12, #0x10000000 

        Return

	#endif

_dcmple_4

        cmp     dyl, #0x0

; we have an "equal" condition here; need to check dxl

	#ifdef IEEE_FULL_OR_FIXED

        bhi.w     d_compare_un_inv_f_nols

    #else

        bls     dcmple_normal

        mov     sreg12, #0xFF000000

        adds    sreg12, sreg12, #0x10000000 

        Return

	#endif

        b       dcmple_normal

  #endif    



;===============================================================================        

; FUNCTION: _d_fgt

; INPUT: dxh/dxl, dyh/dyl

; OUTPUT: condition codes in apsr 

;  
	AREA |.text|,CODE,READONLY

_dgr

_d_fgt


        Prolog_dcmp

        DCheckNaN_start x_maybe_NaN_fgt, y_maybe_NaN_fgt

dfgt_normal

; inline the normal processing here

        orrs    sreg12, dyh, dxh

        bmi     dfgt_checkminus 

        cmp     dxh, dyh

        cmpeq   dxl, dyl
#ifdef EXPLICIT_FCMP
        movhi   fx, #1

        movls   fx, #0
#endif
        Return
        
dfgt_return_false
#ifdef EXPLICIT_FCMP
		mov fx, #0
#endif

		mrs		r12, apsr
		
		bic		r12, r12, #0x20000000
		
		msr		apsr, r12
		
		Return

dfgt_checkminus

  #ifndef IEEE_STD                          
; don't check +0/-0 in IEEE_STD mode

        orr     sreg12, dxl, sreg12, LSL #1

        orrs    sreg12, sreg12, dyl
        
#ifdef EXPLICIT_FCMP
		moveq fx, #0
#endif
        
		mrs		r12, apsr
		
		bic		r12, r12, #0x20000000
		
		msr		apsr, r12        

        Return  eq                      ; special case: signed 0

  #endif

        cmp     dyh, dxh

        cmpeq   dyl, dxl
#ifdef EXPLICIT_FCMP
        movhi   fx, #1

        movls   fx, #0
#endif
        Return

  #ifndef IEEE_STD

x_maybe_NaN_fgt

        DCheckNaN_x d_dgr_un_inv, dfgt_normal, dfgt_return_false

y_maybe_NaN_fgt

        DCheckNaN_y d_dgr_un_inv, dfgt_normal, dfgt_return_false

  #endif



;===============================================================================        

; FUNCTION: _d_fge

; INPUT: dxh/dxl, dyh/dyl

; OUTPUT: apsr condition codes set
; 
	AREA |.text|,CODE,READONLY

_dgeq

_d_fge

        Prolog_dcmp

        DCheckNaN_start x_maybe_NaN_fge, y_maybe_NaN_fge

dfge_normal

; inline the normal processing here

        orrs    sreg12, dyh, dxh

        bmi     dfge_checkminus 

        cmp     dxh, dyh

        cmpeq   dxl, dyl
#ifdef EXPLICIT_FCMP
        movhs   fx, #1

        movlo   fx, #0
#endif
        Return
        
dfge_return_false
#ifdef EXPLICIT_FCMP
		mov fx, #0
#endif

		mrs		r12, apsr
		
		bic		r12, r12, #0x20000000
		
		msr		apsr, r12
		
		Return
		
dfge_checkminus

  #ifndef IEEE_STD                          
; don't check +0/-0 in IEEE_STD mode

        orr     sreg12, dxl, sreg12, LSL #1

        orrs    sreg12, sreg12, dyl
#ifdef EXPLICIT_FCMP
		moveq fx, #1
#endif
        
		mrs		r12, apsr
		
		orr		r12, r12, #0x20000000
		
		msr		apsr, r12        

        Return  eq                      ; special case: signed 0

  #endif

        cmp     dyh, dxh

        cmpeq   dyl, dxl

#ifdef EXPLICIT_FCMP
        movhs   fx, #1

        movlo   fx, #0
#endif
        Return

  #ifndef IEEE_STD

x_maybe_NaN_fge

        DCheckNaN_x d_dge_un_inv, dfge_normal, dfge_return_false

y_maybe_NaN_fge

        DCheckNaN_y d_dge_un_inv, dfge_normal, dfge_return_false

  #endif



;===============================================================================        

; FUNCTION: _dcmpge

; INPUT: dxh/dxl, dyh/dyl

; OUTPUT: result in flags

;  
	AREA |.text|,CODE,READONLY

_dcmpge

        Prolog_dcmp

        DCheckNaN_start x_maybe_NaN_dcmpge, y_maybe_NaN_dcmpge

dcmpge_normal

; inline the normal processing here

        orrs    sreg12, dyh, dxh

        bmi     dcmpge_checkminus 

        cmp     dxh, dyh

        cmpeq   dxl, dyl

        Return

dcmpge_checkminus

  #ifndef IEEE_STD                          
; don't check +0/-0 in IEEE_STD mode

        orr     sreg12, dxl, sreg12, LSL #1

        orrs    sreg12, sreg12, dyl

        cmpeq   dxh, dxh

        Return  eq                      ; special case: signed 0

  #endif

        cmp     dyh, dxh

        cmpeq   dyl, dxl

        Return

  #ifndef IEEE_STD  

x_maybe_NaN_dcmpge

	#ifdef IEEE_FULL_OR_FIXED

        bne.w     d_compare_un_inv_f_nohs

    #else

        beq     _dcmpge_2

        mov     sreg12, #0

        adds    sreg12, sreg12, #0

        Return

	#endif

; we have an "equal" condition here; need to check dxl    

_dcmpge_2

        cmp     dxl, #0x0

	#ifdef IEEE_FULL_OR_FIXED

        bhi.w     d_compare_un_inv_f_nohs

    #else

        bls     _dcmpge_3

        mov     sreg12, #0

        adds    sreg12, sreg12, #0

        Return

	#endif

; x is not a NaN; check y

_dcmpge_3

        cmn     sreg12, dyh, LSL #1 

        bcc     dcmpge_normal

y_maybe_NaN_dcmpge

	#ifdef IEEE_FULL_OR_FIXED

        bne.w     d_compare_un_inv_f_nohs

    #else

        beq     _dcmpge_4

        mov     sreg12, #0

        adds    sreg12, sreg12, #0

        Return

	#endif

_dcmpge_4

        cmp     dyl, #0x0

; we have an "equal" condition here; need to check dxl

	#ifdef IEEE_FULL_OR_FIXED

        bhi.w     d_compare_un_inv_f_nohs

    #else

        bls     dcmpge_normal

        mov     sreg12, #0

        adds    sreg12, sreg12, #0

        Return

	#endif

        b       dcmpge_normal

  #endif  



;===============================================================================        

; FUNCTION: _d_fun

; INPUT: dxh/dxl, dyh/dyl

; OUTPUT: fx = 0 (relation false), 1 (relation true) (invalid on SNaN)

;  
	AREA |.text|,CODE,READONLY

_d_fun

; don't care about special values on IEEE_STD; always return 0

  #ifdef IEEE_STD

        mov     fx, #0

        Return

  #else

        DCheckNaN_start x_maybe_NaN_fun, y_maybe_NaN_fun

dfun_normal

        mov     fx, #0

        Return
        
dfun_return_true

		mov		fx, #1
		
		Return

x_maybe_NaN_fun

        DCheckNaN_x d_compare_un_inv_snan_x, dfun_normal, dfun_return_true

y_maybe_NaN_fun

        DCheckNaN_y d_compare_un_inv_snan_y, dfun_normal, dfun_return_true

  #endif



;===============================================================================        

; FUNCTION: _d_for

; INPUT: dxh/dxl, dyh/dyl

; OUTPUT: fx = 0 (relation false), 1 (relation true) (invalid on SNaN)

;  
	AREA |.text|,CODE,READONLY

_d_for

; don't care about special values on IEEE_STD; always return 1

  #ifdef IEEE_STD

        mov     fx, #1

        Return

  #else

        DCheckNaN_start x_maybe_NaN_for, y_maybe_NaN_for

dfor_normal

        mov     fx, #1

        Return
        
dfor_return_false

		mov		fx, #0
		
		Return

x_maybe_NaN_for

        DCheckNaN_x d_compare_un_inv_snan_x, dfor_normal, dfor_return_false

y_maybe_NaN_for

        DCheckNaN_y d_compare_un_inv_snan_y, dfor_normal, dfor_return_false

  #endif



	#ifdef IEEE_FULL_OR_FIXED

d_compare_un_inv

; @@@ Modified: added LFPState

        stmfd   sp!, {lr, sreg12}

        LFPState lr, sreg12

        SetInvalidEL lr, __wrap_invalid_handler_double  ; set invalid

        StFPState lr,sreg12             ; store FPSCR   

        ldmfd   sp!, {fx}


        Return_lr
        

d_deq_un_inv

; @@@ Modified: added LFPState

        stmfd   sp!, {lr, sreg12}

        LFPState lr, sreg12

        SetInvalidEL lr, __wrap_invalid_handler_double  ; set invalid

        StFPState lr,sreg12             ; store FPSCR   

        ldmfd   sp!, {fx}
        
        mrs	r0, apsr
        
        bic	 r0, r0, #0x40000000
        
        msr apsr, r0

		Return_lr
		

d_dne_un_inv

; @@@ Modified: added LFPState

        stmfd   sp!, {lr, sreg12}

        LFPState lr, sreg12

        SetInvalidEL lr, __wrap_invalid_handler_double  ; set invalid

        StFPState lr,sreg12             ; store FPSCR   

        ldmfd   sp!, {fx}
        
        mrs	r0, apsr
        
        bic	 r0, r0, #0x40000000
        
        msr apsr, r0

		Return_lr
		

d_dls_un_inv

; @@@ Modified: added LFPState

        stmfd   sp!, {lr, sreg12}

        LFPState lr, sreg12

        SetInvalidEL lr, __wrap_invalid_handler_double  ; set invalid

        StFPState lr,sreg12             ; store FPSCR   

        ldmfd   sp!, {fx}
        
        mrs	r0, apsr
        
        orr	 r0, r0, #0x20000000
        
        msr apsr, r0

		Return_lr

	
d_dle_un_inv

; @@@ Modified: added LFPState

        stmfd   sp!, {lr, sreg12}

        LFPState lr, sreg12

        SetInvalidEL lr, __wrap_invalid_handler_double  ; set invalid

        StFPState lr,sreg12             ; store FPSCR   

        ldmfd   sp!, {fx}
        
        mrs	r0, apsr
        
        bic r0, r0, #0x40000000
        
        orr	r0, r0, #0x20000000
        
        msr apsr, r0

		Return_lr

	
d_dgr_un_inv

; @@@ Modified: added LFPState

        stmfd   sp!, {lr, sreg12}

        LFPState lr, sreg12

        SetInvalidEL lr, __wrap_invalid_handler_double  ; set invalid

        StFPState lr,sreg12             ; store FPSCR   

        ldmfd   sp!, {fx}
        
        mrs	r0, apsr
        
        bic r0,	r0, #0x20000000
        
        msr apsr, r0

		Return_lr
		

d_dge_un_inv

; @@@ Modified: added LFPState

        stmfd   sp!, {lr, sreg12}

        LFPState lr, sreg12

        SetInvalidEL lr, __wrap_invalid_handler_double  ; set invalid

        StFPState lr,sreg12             ; store FPSCR   

        ldmfd   sp!, {fx}
        
        mrs	r0, apsr
        
        bic r0,	r0, #0x20000000
        
        msr apsr, r0

		Return_lr
		

d_compare_un_inv_f

; @@@ Modified: added LFPState

        stmfd   sp!, {lr, sreg12}

        LFPState lr, sreg12

        SetInvalidEL lr, __wrap_invalid_handler_double  ; set invalid

        StFPState lr,sreg12             ; store FPSCR   

        add     sp, sp, #4

        movs    sreg12, #1              ; set  flags

        Return_lr



d_compare_un_inv_f_nols

; @@@ Modified: added LFPState

        stmfd   sp!, {lr, sreg12}

        LFPState lr, sreg12

        SetInvalidEL lr, __wrap_invalid_handler_double  ; set invalid

        StFPState lr,sreg12             ; store FPSCR   

        add     sp, sp, #4

        mov     sreg12, #0xFF000000

        adds    sreg12, sreg12, #0x10000000 

        Return_lr


d_compare_un_inv_f_nohs

; @@@ Modified: added LFPState

        stmfd   sp!, {lr, sreg12}

        LFPState lr, sreg12

        SetInvalidEL lr, __wrap_invalid_handler_double  ; set invalid

        StFPState lr,sreg12             ; store FPSCR   

        add     sp, sp, #4

        mov     sreg12, #0x0

        adds    sreg12, sreg12, #0x0

        Return_lr


d_compare_un_inv_snan_x

; when we arrive here we know that x is a NaN

; need to check if it is a SNaN

        tst     dxh, #0x00080000        ; test SNaN bit

        beq     d_compare_un_inv        ; set invalid on SNaN

; otherwise we need to check y

        stmfd   sp!, {sreg12}

        mov     sreg12, #0x00200000

        cmn     sreg12, dyh, LSL #1

        ldmfd   sp!, {sreg12}

        bcc     d_compare_exit          ; exit normally if y is not a NaN

        bne     d_compare_un_inv_snan_y  ; ne : y is a NaN

; arrive here only if "equal" is set; test lower part of y

        cmp     dyl, #0x0

        beq     d_compare_exit          ; exit if lower part is 0
        

d_compare_un_inv_snan_y

; when we arrive here we know that y is a NaN

; need to check if it is a SNaN

        tst     dyh, #0x00080000        ; test SNaN bit

        beq     d_compare_un_inv        ; set invalid on SNaN

d_compare_exit

        mov     fx, sreg12

        Return
        
;==============================================================


d_deq_un_inv_snan_x

; when we arrive here we know that x is a NaN

; need to check if it is a SNaN

        tst     dxh, #0x00080000        ; test SNaN bit

        beq     d_deq_un_inv        ; set invalid on SNaN

; otherwise we need to check y

        stmfd   sp!, {sreg12}

        mov     sreg12, #0x00200000

        cmn     sreg12, dyh, LSL #1

        ldmfd   sp!, {sreg12}

        bcc     deq_compare_exit          ; exit normally if y is not a NaN

        bne     d_deq_un_inv_snan_y  ; ne : y is a NaN

; arrive here only if "equal" is set; test lower part of y

        cmp     dyl, #0x0

        beq     deq_compare_exit          ; exit if lower part is 0
        

d_deq_un_inv_snan_y

; when we arrive here we know that y is a NaN

; need to check if it is a SNaN

        tst     dyh, #0x00080000        ; test SNaN bit

        beq     d_deq_un_inv        ; set invalid on SNaN

deq_compare_exit

		mrs		r0, apsr
		
		bic		r0, r0, #0x40000000
		
		msr		apsr, r0

        Return
;=====================================================================

d_dne_un_inv_snan_x

; when we arrive here we know that x is a NaN

; need to check if it is a SNaN

        tst     dxh, #0x00080000        ; test SNaN bit

        beq     d_dne_un_inv        ; set invalid on SNaN

; otherwise we need to check y

        stmfd   sp!, {sreg12}

        mov     sreg12, #0x00200000

        cmn     sreg12, dyh, LSL #1

        ldmfd   sp!, {sreg12}

        bcc     dne_compare_exit          ; exit normally if y is not a NaN

        bne     d_dne_un_inv_snan_y  ; ne : y is a NaN

; arrive here only if "equal" is set; test lower part of y

        cmp     dyl, #0x0

        beq     dne_compare_exit          ; exit if lower part is 0
        

d_dne_un_inv_snan_y

; when we arrive here we know that y is a NaN

; need to check if it is a SNaN

        tst     dyh, #0x00080000        ; test SNaN bit

        beq     d_deq_un_inv        ; set invalid on SNaN

dne_compare_exit
	
		mrs		r0, apsr
		
		bic		r0, r0,#0x40000000
		
		msr		apsr, r0

		Return
;=====================================================================

d_compare_un_inv_snan_x_f

; when we arrive here we know that x is a NaN

; need to check if it is a SNaN

        tst     dxh, #0x00080000        ; test SNaN bit

        beq     d_compare_un_inv_f      ; set invalid on SNaN

; otherwise we need to check y

        stmfd   sp!, {sreg12}

        mov     sreg12, #0x00200000

        cmn     sreg12, dyh, LSL #1

        ldmfd   sp!, {sreg12}

        bcc     d_compare_exit_f        ; exit normally if y is not a NaN

        bne     d_compare_un_inv_snan_y_f  ; ne : y is a NaN

; arrive here only if "equal" is set; test lower part of y

        cmp     dyl, #0x0

        beq     d_compare_exit_f        ; exit if lower part is 0

d_compare_un_inv_snan_y_f

; when we arrive here we know that y is a NaN

; need to check if it is a SNaN

        tst     dyh, #0x00080000        ; test SNaN bit

        beq     d_compare_un_inv_f      ; set invalid on SNaN

d_compare_exit_f

        Return

  #endif

#endif



	#ifdef IEEE_FULL_OR_FIXED

; invalid exception handlers

	EXPORT __wrap_invalid_handler

__wrap_invalid_handler

        stmfd   sp!, {lr}

        ldr     r2, =(FE_EX_INVALID :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_INT :OR: FE_EX_FN_CMP)

        bl      __fp_handler_binary_32_to_32

        Return_lr

	EXPORT __wrap_invalid_handler_double

__wrap_invalid_handler_double

        ldr     r12, =(FE_EX_INVALID :OR: FE_EX_INTYPE_DOUBLE :OR: FE_EX_OUTTYPE_INT :OR: FE_EX_FN_CMP)

        stmfd   sp!, {sreg12}

        bl      __fp_handler_binary_64_to_32

        add     sp, sp, #8

        Return_lr



#endif

#ifdef _AEABI	

__aeabi_cdcmpeq
__aeabi_cdcmple
__aeabi_cdrcmple

	; The status-returning routines are required to preserve all
	; registers except ip, lr, and cpsr.
	
	stmfd	sp!, {r0, lr}
	
	bl cmpdf2


	; Set the Z flag correctly, and the C flag unconditionally.

	cmp	 r0, #0

	; Clear the C flag if the return value was -1, indicating
	; that the first operand was smaller than the second.

	cmnmi	 r0, #0
	ldmfd	sp!,{r0,pc}			;RETLDM   "r0"
	
;=============================================================================

; For FPA, float words are always big-endian.
; For VFP, floats words follow the memory system mode.


#if defined(VFP) && !defined(BIG_ENDIAN)
#define xl r0
#define xh r1
#define yl r2
#define yh r3
#else
#define xh r0
#define xl r1
#define yh r2
#define yl r3
#endif


__aeabi_dcmpun
	mov		ip, xh, lsl #1
	mvns	ip, ip, asr #21
	bne		dcmpun1
	orrs	ip, xl, xh, lsl #12
	bne		dcmpun3					; x is NAN

dcmpun1
	mov		ip, yh, lsl #1
	mvns	ip, ip, asr #21
	bne		dcmpun2
	orrs	ip, yl, yh, lsl #12
	bne		dcmpun3					; y is NAN

dcmpun2
	mov		r0, #0					; arguments are ordered.
	mov		pc, lr

dcmpun3
	mov		r0, #1					; arguments are unordered.
	mov		pc, lr
;==============================================================================


__aeabi_cfcmpeq
__aeabi_cfcmple
	stmfd	sp!, {r0, r1, r2, r3, lr}
	
	bl cmpsf2									; Set the Z flag correctly, and the C flag unconditionally.
	
	cmp	 r0, #0
										; Clear the C flag if the return value was -1, indicating
										; that the first operand was smaller than the second.
	cmnmi	 r0, #0
	ldmfd	sp!, {r0, r1, r2, r3, pc}


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

cmpsf2

	mov		ip, #1							; how should we specify unordered here?

cmpsf2_1
	str		ip, [sp, #-4]
										; Trap any INF/NAN first.
	mov		r2, r0, lsl #1
	mov		r3, r1, lsl #1
	mvns	ip, r2, asr #24
	mvnnes	ip, r3, asr #24
	beq		cmpsf2_3

										; Compare values.
										; Note that 0.0 is equal to -0.0.
cmpsf2_2
	orrs	ip, r2, r3, lsr #1			; test if both are 0, clear C flag
	teqne	r0, r1						; if not 0 compare sign
	;subpls	r0, r2, r3					; if same sign compare values, set r0
	subspl	r0, r2, r3					; if same sign compare values, set r0

; Result:
	movhi	r0, r1, asr #31
	mvnlo	r0, r1, asr #31
	orrne	r0, r0, #1
	mov		pc, lr

; Look for a NAN. 
cmpsf2_3
	mvns	ip, r2, asr #24
	bne		cmpsf2_4
	movs	ip, r0, lsl #9
	bne		cmpsf2_5					; r0 is NAN

cmpsf2_4
	mvns	ip, r3, asr #24
	bne		cmpsf2_2
	movs	ip, r1, lsl #9
	beq		cmpsf2_2					; r1 is not NAN

cmpsf2_5
	ldr		r0, [sp, #-4]				; return unordered code.
	mov		pc, lr
	
	
	
;==============================================================================
cmpdf2

	mov	r12, xh		
	mov	xh, xl		
	mov	xl, r12		
	mov	r12, yh		
	mov	yh, yl		
	mov	yl, r12		
	mov	ip, #1		

cmpdf2_1	
	str	ip, [sp, #-4]
	mov	ip, xh, lsl #1		;Trap any INF/NAN first.
	mvns	ip, ip, asr #21
	mov	ip, yh, lsl #1
	mvnnes	ip, ip, asr #21
	beq	cmpdf2_3

	;@ Test for equality.
	;@ Note that 0.0 is equal to -0.0.
	
cmpdf2_2	
	orrs	ip, xl, xh, lsl #1			;@ if x == 0.0 or -0.0
	orreqs	ip, yl, yh, lsl #1				;@ and y == 0.0 or -0.0
	teqne	xh, yh				;@ or xh == yh
	teqeq	xl, yl				;@ and xl == yl
	moveq	r0, #0				;@ then equal.
	moveq pc,lr

    ;@ Clear C flag
	cmn	r0, #0

	;@ Compare sign, 
	teq	xh, yh

	;@ Compare values if same sign
	cmppl	xh, yh
	cmpeq	xl, yl

Result
	movcs	r0, yh, asr #31
	mvncc	r0, yh, asr #31
	orr	r0, r0, #1
	mov pc,lr

	;@ Look for a NAN.
cmpdf2_3	
	mov	ip, xh, lsl #1
	mvns	ip, ip, asr #21
	bne	cmpdf2_4
	orrs	ip, xl, xh, lsl #12
	bne	cmpdf2_5	                       		;@ x is NAN
	
cmpdf2_4	
	mov	ip, yh, lsl #1
	mvns	ip, ip, asr #21
	bne	cmpdf2_2
	orrs	ip, yl, yh, lsl #12
	beq	cmpdf2_2			                   ;@ y is not NAN
	
cmpdf2_5	
	ldr	r0, [sp, #-4]		          ;@ unordered return code
	mov pc,lr

;=============================================================================

__aeabi_cfrcmple

		mov	ip, r0
		mov	r0, r1
		mov	r1, ip
		b	_cfrcmple6

		; The status-returning routines are required to preserve all
		; registers except ip, lr, and cpsr.
		
_cfrcmple6
		stmfd	sp!, {r0, r1, r2, r3, lr}
		bl cmpsf2					;ARM_CALL cmpsf2
		
		; Set the Z flag correctly, and the C flag unconditionally.
		
		cmp	 r0, #0
		
		; Clear the C flag if the return value was -1, indicating
		; that the first operand was smaller than the second.
		
		cmnmi	 r0, #0
		ldmfd	sp!, {r0-r3,pc}				;RETLDM  "r0, r1, r2, r3"

;==============================================================================	

__aeabi_fcmpun

		mov	r2, r0, lsl #1
		mov	r3, r1, lsl #1
		mvns	ip, r2, asr #24
		bne	_fcmpun1
		movs	ip, r0, lsl #9
		bne	_fcmpun3			; r0 is NAN
_fcmpun1
		mvns	ip, r3, asr #24
		bne	_fcmpun2
		movs	ip, r1, lsl #9
		bne	_fcmpun3			; r1 is NAN
_fcmpun2
		mov	r0, #0			; arguments are ordered.
		mov	pc, lr			;RET
_fcmpun3
		mov	r0, #1			; arguments are unordered.
		mov	pc, lr			;RET
	
		
#endif

END

/* Change record:
 * clm  June 2004 Function result is in the condition codes for float/double compare routines.
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 * clm 	October 6, 2004 Full, Fast and Fixed support for condition code return values
 * jrusso Aug 7, 2006 - add back explicit return of float compares.
 */

