;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _ll_div.s

;===============================================================================



; @@@ All the code in this file was modified to use llh/lll instead of dxh/dxl

; @@@ and lyh/lyl instead of dyh/dyl in order to insure compatibility 

; @@@ with the SoftFPA mode



	#include "ARMsfpe.pre.asm"



;===============================================================================



	EXPORT _ull_div

	EXPORT _ll_udiv                ; internal compiler for _ull_div

	EXPORT _ull_mod

	EXPORT _ll_mod

	EXPORT _ll_div

	EXPORT _ll_sdiv                ; internal compiler for _ll_div



	IMPORT _u32_div_not_0_f

	IMPORT _s32_div_f



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

; FUNCTION: _ull_div, _ll_div, _ull_mod, _ll_mod  -- (un)signed long long

;       divide, modulo

; INPUT:        llh/lll = long long x,  lyh/lyl = long long y

; OUTPUT:       llh/lll = quotient or mod of x/y

; COMMENTS: The quotient q is rounded toward zero and the mod m satisfies

;       x = y*q + m   so m always has the sign of x (unless it's zero).

;       Optimize by looking for easy case of small positive or negative

;       values.

; REGISTERS: r4.0 = 0 for div, 1 for mod.  r4.31 = sign of result, sign of

;       x in case of mod and eor of signs in case of div.  (Everything's

;       positive in unsigned case, of course.)

;

	AREA |.text|,CODE,READONLY
_ull_div

_ll_udiv

        stmfd   sp!,{r4-r7,r11-r12,lr}

        mov     r4,#0                   ; r4.31 = 0 for positive quotient, r4.0 = div

        b       ull_divmod_common

;~~~~~~~

_ull_mod

        stmfd   sp!,{r4-r7,r11-r12,lr}

        mov     r4,#1                   ; r4.31 = 0 for positive remainder, r4.0 = mod

;~~~~~~~

ull_divmod_common

        orrs    r5,lyh,lyl              ; if divisor is 0, return unchanged

        bne     ull_div_notzero

        ldmfd   sp!,{r4-r7,r11-r12,lr}

        Return

;~~~~~~~

ull_div_notzero

        orrs    r5,llh,lyh              ; if both high halves are 0, it is easy (32*32 div)

        bne     ll_div_hard_common

;~~~~~~~

#ifdef BIG_ENDIAN

        mov     r0,lll                  ; NOTE: in little endian, lll is already in r0

#endif

        mov     r1,lyl

;~~~~~~~

        bl      _u32_div_not_0_f        ; Know that divisor is not zero

;~~~~~~~

        cmp     r4,#0                   ; low result is quotient (in r0) or remainder (in r1)

#ifdef BIG_ENDIAN

        moveq   lll,r0                  ; NOTE: in little endian, lll is already in r0

#else

        movne   lll,r1                  ; NOTE: in big endian, lll is already in r1

#endif

;~~~~~~~

        mov     llh,#0                  ; high result is 0 (zero extend of low result)

        ldmfd   sp!,{r4-r7,r11-r12,lr}

        Return

;~~~~~~~



	AREA |.text|,CODE,READONLY

_ll_mod

        stmfd   sp!,{r4-r7,r11-r12,lr}

        mov     r4,llh                  ; r4.31 = sign of remainder (0 for positive)

        orr     r4,r4,#1                ; r4.0 = 1 means mod

        b       ll_divmod_common

;~~~~~~~

_ll_div

_ll_sdiv

        stmfd   sp!,{r4-r7,r11-r12,lr}

        eor     r4,llh,lyh              ; r4.31 = sign of quotient (0 for positive)

        mov     r4,r4,ASR #1

        mov     r4,r4,LSL #1            ; r4.0 = 0 means div

;~~~~~~~

ll_divmod_common

; Input: dx, dy, r4 = sign / (mod vs. quo)

;~~~~~~~

        orrs    r5,lyh,lyl              ; if divisor is 0, return unchanged

        bne     ll_div_notzero

        ldmfd   sp!,{r4-r7,r11-r12,lr}

        Return

;~~~~~~~

; Easy case if x and y are small signed values, i.e. their high 33 bits

; match.  Trick: add sign bit from low half into high half and demand a

; zero result from both x and y.

; This picks up case of 0/0, but does not get all cases of 0/N and N/0 for

; nonzero N.

;~~~~~~~

ll_div_notzero

        mov     r5,lll,LSR #31          ; r5 = sign bit of low half of dx

        add     r5,r5,llh               ; r5 = 0 if 33 bits of dx match

        mov     r6,lyl,LSR #31

        add     r6,r6,lyh

        orrs    r6,r5,r6                ; this is 0 if both are 32-bit operands

        bne     ll_div_hard

;~~~~~~~

; Easy case: call signed 32*32 divide

;~~~~~~~

#ifdef BIG_ENDIAN

        mov     r0,lll                  ; NOTE: in little endian, lll is already in r0

#endif

        mov     r1,lyl

;~~~~~~~

        bl      _s32_div_f

;~~~~~~~

        ands    r4,r4,#1                ; low result is quotient (in r0) or remainder (in r1)

#ifdef BIG_ENDIAN

        moveq   lll,r0                  ; NOTE: in little endian, lll is already in r0

#else

        movne   lll,r1                  ; NOTE: in big endian, lll is already in r1

#endif

;~~~~~~~

        mov     llh,lll,ASR #31         ; high result is sign extend of low result

        ldmfd   sp!,{r4-r7,r11-r12,lr}

        Return

;~~~~~~~



ll_div_hard

; INPUT: dx, dy, r4 = sign / function indicator.

; Scrub the signs from x and y and fall through to the big

; divide code.

        cmp     llh,#0                  ; check for negative dividend

        bge     ll_check_divisor

;~~~~~~~

        rsbs    lll,lll,#0              ; negate

;;;        rsc     llh,llh,#0
	rsb	llh,llh,#0
	subcc	llh,llh,#1

ll_check_divisor

        cmp     lyh,#0                  ; check for negative divisor

        bge     ll_div_hard_common

;~~~~~~~

        rsbs    lyl,lyl,#0              ; negate

;;;        rsc     lyh,lyh,#0
	rsb	lyh,lyh,#0
	subcc	lyh,lyh,#1

;~~~~~~~



ll_div_hard_common

; First check for x = 0.  Have already filtered y = 0.

        orrs    r5,llh,lll

        beq     ll_div_zero



;~~~~~~~

; Enter with r4.31 = result sign, r4.0 = 0 for div & 1 for mod.

; INPUT:

; llh/lll = abs(x)

; lyh/lyl = abs(y)

; USES:

; drh/drl = result slots, used for quotient

; remhh = high word of partial remainder

; yshift = left shift count of y, to post-align remainder

; qcount = quotient bit count (and later) = flags in r4

; OUTPUT:

; drh/drl = result

;

; First pre-condition the operands.

; Normalize y, increasing the quo bit count from 1 by 1 for each

; shift.  This leaves the remainder binary point leftward by the

; number of shifts.

; Then normalize x, decreasing the quo bit count, but not past 1.

; Shifting x does not affect the placement of the remainder binary

; point, but rather just the number of leading zero quo bits

; bypassed.

; Number of integer quotient bits is 1+D-N, where D is the number of

; denom left shifts and N is the number of numerator left shifts.



	GBLS	yshift
	GBLS	qcount
	GBLS	remhh
	GBLS	t1

yshift SETS "r5"                     ; left shift count of y

qcount SETS "r6"                     ; quotient bit count

remhh SETS "r7"                      ; high word of partial remainder

t1 SETS "r7"                         ; scratch (same register as remhh, so be cautious)



        mov     $yshift,#0               ; init count of left shifts of y

        mov     $qcount,#1               ; init quotient bit counter

;~~~~~~~

        cmp     lyh,#0

        bmi     ll_norm_x

;~~~~~~~

ll_y_loop

        add     $yshift,$yshift,#1        ; record left shifts

        adds    lyl,lyl,lyl

        adcs    lyh,lyh,lyh

        bpl     ll_y_loop

;~~~~~~~

        add     $qcount,$qcount,$yshift

ll_norm_x

; Normalize x until shift count is 1.

        cmp     llh,#0

        blt     ll_divide

;~~~~~~~

ll_x_loop

        cmp     $qcount,#1

        beq     ll_divide               ; exit if count down to one

;~~~~~~~

; @@@ Modified here

; add   $qcount,$qcount,#-1

        sub     $qcount,$qcount,#1

        adds    lll,lll,lll

        adcs    llh,llh,llh

        bpl     ll_x_loop

;~~~~~~~

ll_divide

; remhh/llh/lll = partially normalized numerator with sign-extension

; lyh/lyl = fully normalized denominator

; qcount = number of quo bits (possibly nonpositive)

; yshift = number of left shifts of y, to post-align remainder, if necessary

; drh/drl = 0 = initial quotient

        mov     $remhh,#0

        mov     drl,#0

        mov     drh,#0

        b       ll_div_start

;~~~~~~~



ll_div_surplus

; Insert quotient bit, check for done

        orr     drl,drl,#1              ; stuff last quo bit

; @@@ Modified here

; adds $qcount,$qcount,#-1

        subs    $qcount,$qcount,#1        ; decrement quo bit counter

        beq     ll_surplus_fini

; Shift left partial remainder

        adds    lll,lll,lll

        adcs    llh,llh,llh

        adcs    $remhh,$remhh,$remhh

;~~~~~~~

ll_div_start

; Subtract divisor from numerator

        subs    lll,lll,lyl

        sbcs    llh,llh,lyh

        sbcs    $remhh,$remhh,#0

; Shift quotient

        adds    drl,drl,drl

        adc     drh,drh,drh

; If subtract result is a nonnegative high part, go subtract again.

        cmp     $remhh,#0

        bge     ll_div_surplus

;~~~~~~~

; Fall through to deficit when there is a borrow out (negative numerator)



ll_div_deficit

; Quotient bit is 0, so check for done

; @@@ Modified here

; adds $qcount,$qcount,#-1

        subs    $qcount,$qcount,#1        ; decrement quo bit counter

        beq     ll_deficit_fini

; Shift left partial remainder

        adds    lll,lll,lll

        adcs    llh,llh,llh

        adc     $remhh,$remhh,$remhh

; mirror entry of loop entry point in surplus side

; Add back the divisor and watch for carries.  Go to surplus if remhh is fixed.

        adds    lll,lll,lyl

        adcs    llh,llh,lyh

        adc     $remhh,$remhh,#0

; Shift quotient

        adds    drl,drl,drl

        adc     drh,drh,drh

; If add result is a nonnegative high part, go subtract again.

        cmp     $remhh,#0

        bge     ll_div_surplus

;~~~~~~~

        b       ll_div_deficit

;~~~~~~~



ll_deficit_fini

; When ending on a deficit, add divisor in once more to correct remainder.

        adds    lll,lll,lyl

        adc     llh,llh,lyh

ll_surplus_fini

; If div or divu, ignore remainder - move quotient to result and fix sign.

; Otherwise, right shift remainder by yshift and give fix the result sign.

; yshift may be 0 to 63.

        ands    $t1,r4,#1                ; r4.0 nonzero if mod

        moveq   lll,drl                 ; move quotient into return registers

        moveq   llh,drh

        beq     ll_sign_fix             ; t1 is 0 for div, nonzero for mod

;~~~~~~~

; Fix shift and sign.

        subs    $t1,$yshift,#32           ; if shifting >= 32 bits, just use bits of llh

; @@@ Modified 2 lines here (external update)

        movge   lll,llh,LSR $t1

        bge     ll_div_high_zero 

;~~~~~~~

        rsb     $t1,$yshift,#32

        mov     lll,lll,LSR $yshift      ; shift lll and OR in low llh bits

;;;        orr     lll,lll,llh,LSL $t1
	mov	sreg07,llh,LSL $t1
	orr	lll,lll,sreg07

        mov     llh,llh,LSR $yshift      ; shift llh

        b       ll_sign_fix

;~~~~~~~

ll_big_fix_shift

; result is some bits that used to be in llh

        mov     lll,llh,LSR $t1

        mov     llh,#0

ll_sign_fix

        cmp     r4,#0

        blt     ll_sign_lt

        ldmfd   sp!,{r4-r7,r11-r12,lr}

        Return

;~~~~~~~

ll_sign_lt

        rsbs    lll,lll,#0              ; negate result

;;;        rsc     llh,llh,#0
	rsb	llh,llh,#0
	subcc	llh,llh,#1		; uses Carry from rsbs 

        ldmfd   sp!,{r4-r7,r11-r12,lr}

        Return



ll_div_zero

        mov     lll,#0

; @@@ Modified here (External update)

ll_div_high_zero

        mov     llh,#0 

; @@@ Inlined ll_sign_fix here; added for correct result sign 

        cmp     r4,#0

        blt     ll_sign_lt

        ldmfd   sp!,{r4-r7,r11-r12,lr}

        Return

	END

/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */

