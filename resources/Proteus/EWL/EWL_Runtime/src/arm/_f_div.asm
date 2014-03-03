;===============================================================================
;===
;===	Metrowerks ARM Floating Point Emulation Library 
;===	Copyright © 1995-2004 Metrowerks Corporation.  All rights reserved.
;===
;===	$Date: 2012/06/01 16:05:24 $
;===	$Revision: 1.1 $
;===
;===============================================================================

;=== FILE: _f_div.s

;===============================================================================



	#include "ARMsfpe.pre.asm"



	EXPORT _f_div

	EXPORT _fdiv                   ; FPLIB for _f_div

	EXPORT _frdiv



	#ifdef VFP_AND_NOT_IEEE_STD

	EXPORT _vfp_support_fdiv

#endif        

	MACRO
	CLRSTK2 $cond
	add$cond	sp,sp,#8
	MEND



	#ifdef IEEE_FULL_OR_FIXED

#ifdef USING_ARMASM
#define Clrstk(x) CLRSTK2 #x 
#else
#define Clrstk(x) CLRSTK2 ##x 
#endif


	MACRO
	Prolog

        stmfd   sp!, {lr, fx, fy}

	MEND

#else

#define Clrstk(x)  

	MACRO
	Prolog

#ifdef USING_ARMASM
        stmfd   sp!, {lr}
#else
	str lr,[sp,#-4]!
#endif

	MEND

#endif  



#if defined(THUMB)
; && !defined(ARM5)

	MACRO
	Retcc

        Clrstk(cc)

#ifdef USING_ARMASM
        ldmccfd sp!, {lr}
#else
	ldrcc lr,[sp],#4
#endif

        bxcc    lr

	MEND

	MACRO
	Reteq

        Clrstk(eq)

#ifdef USING_ARMASM
        ldmeqfd sp!, {lr}
#else
	ldreq lr,[sp],#4
#endif
		

        bxeq    lr

	MEND

	MACRO
	Retne

        Clrstk(ne)

#ifdef USING_ARMASM
        ldmnefd sp!, {lr}
#else
	ldrne lr,[sp],#4
#endif

        bxne    lr

	MEND

	MACRO
	Retgt

        Clrstk(gt)

#ifdef USING_ARMASM
        ldmgtfd sp!, {lr}
#else
	ldrgt lr,[sp],#4
#endif

        bxgt    lr

	MEND

	MACRO
	Retpl

        Clrstk(pl)

#ifdef USING_ARMASM
        ldmplfd sp!, {lr}
#else
	ldrpl lr,[sp],#4
#endif

        bxpl    lr

	MEND

	MACRO
	Return

        Clrstk(al)

#ifdef USING_ARMASM
        ldmfd   sp!, {lr}
#else
	ldr lr,[sp],#4
#endif

        bx      lr

	MEND

#else

	MACRO
	Retcc

        Clrstk(cc)

        ldmccfd sp!, {pc}

	MEND

	MACRO
	Reteq

        Clrstk(eq)

        ldmeqfd sp!, {pc}

	MEND

	MACRO
	Retne

        Clrstk(ne)

        ldmnefd sp!, {pc}

	MEND

	MACRO
	Retgt

        Clrsrk(gt)

        ldmgtfd sp!, {pc}

	MEND

	MACRO
	Retpl

        Clrstk(pl)

        ldmplfd sp!, {pc}

	MEND

	MACRO
	Return

        Clrstk(al)

        ldmfd   sp!, {pc}

	MEND

#endif  



;===============================================================================

; FUNCTION: float _frdiv(float, float)

; DESCRIPTION: Reversed float division.

; INPUT: 

; fx=x,

; fy=y, floats

; OUTPUT: 

; fx = result   



;===============================================================================

; FUNCTION: float _f_div (float, float)

; DESCRIPTION: Float division.

; INPUT: 

; fx=x,

; fy=y, floats

; OUTPUT: 

; fx = result

; Uses the Newton-Rhapson method with an approximation table of 256 bytes


	AREA |.text|,CODE,READONLY

#ifdef VFP

_fdiv

_f_div


        fmsr    s0, r0

        fmsr    s1, r1

        fdivs   s0, s0, s1

        fmrs    r0, s0

  #ifdef THUMB        

        bx      lr

  #else        

        mov     pc,lr

  #endif



_frsb

        fmsr    s0, r0

        fmsr    s1, r1

        fdivs   s0, s1, s0

        fmrs    r0, s0

  #ifdef THUMB        

        bx      lr

  #else        

        mov     pc,lr

  #endif





_vfp_support_fdiv

#else

_frdiv

; swap operands

        eor     fx, fx, fy

        eor     fy, fx, fy

        eor     fx, fx, fy

; and fall through _f_div

_fdiv									; FPLIB entry point

_f_div

#endif



	#ifdef NOT_VFP_OR_NOT_IEEE_STD



        Prolog

        mov     sreg12,#0xff

        ands    sreg03,sreg12,fx,LSR #23  ; unpack fx in sreg03(exp) and fx(significand)

        cmpne   sreg03, #0xff           ; test for huge exponent

        beq.w     fdiv_x_special_case     ; special case: tiny or huge



        ands    sreg12,sreg12,fy, LSR #23  ; unpack fy in sreg12(exp) and fy(significand)  

        cmpne   sreg12, #0xff           ; test for huge exponent

        beq.w     fdiv_y_special_case     ; special case: tiny or huge

        

        orr     fy,fy,#0x00800000       ; set lead bit  

        orr     fx,fx,#0x00800000       ; set lead bit          

        bic     sreg02,fx,#0xff000000   ; r2 = mantissa(x) - clear remaining exponent bits

        bic     lr,fy,#0xff000000       ; lr = mantissa(y)

;

; The Newton-Raphson division algorithm:

; --------------------------------------

;  x0 = approximately 1/y - computed using an 8-bit lookup table

;  x1 = x0 * (2 + (-y)*x0)

;  x2 = x1 + x1 * (1 + (-y)*x1)

;   q = x1 * x

; --------------------------------------

; C implementation:

;

;  if (x<y) 

;    {

;    x <<= 1;

;    exp --;

;    }

;  x0 = table[(y & 0x007fffff) >> (23 - TABLEBITS)]; 

;  minusY = -y; 

;  xtemp=(minusY >> 1) * x0 + 0x80000000

;  x1 = (xtemp >> 6) * x0 >> 14;

;  xtemp = minusY * x1; 

;  xtemp = (xtemp >> 12) * x1;

;  x2 = (x1<<14) + (xtemp >> 15);

;  sigQ = (bits64)x * x2 >> 24; 

;

; Registers @ this point

;       r2 = mantissa(x)

;       lr = mantissa(y)

;       r3 = exp(x)

;       r12 = exp(y)

;       sign(fx ^ fy) = sign(result)

;               

fdiv_start

        cmp     sreg02,lr               ; if (x<y)

        movlo   sreg02,sreg02,LSL #1    ;   x <<= 1;

        sublo   sreg03,sreg03,#1        ;   exp --;



        teq     fx,fy                   ; N flag = sign of the result



  #if defined(ARM_xM) && defined(IEEE_FULL)

#ifdef USING_ARMASM
        add     fx,pc,#(rtable - . - 4 - 256)
#else
	adr.w 	fx,rtable
	sub.w	fx,fx,#0xFE
#endif

  #else

#ifdef USING_ARMASM
	add.w	fx,pc,#(rtable - . - 4 - 256)
#else
	adr.w 	fx,rtable
	sub.w	fx,fx,#0xFE
#endif

  #endif  

;;;        ldrb    fy,[fx,lr,LSR #15]      ;  x0 = table[(y & 0x007fffff) >> (23 - 8)]; 
	mov	fy,lr,LSR #15
	ldrb	fy,[fx,fy]



        rsb     lr,lr,#0                ;  minusY = -y; 



        mov     fx,lr,ASR #1            ;  xtemp=(minusY >> 1) * x0 + 0x80000000

        mul     fx,fy,fx

        add     fx,fx,#0x80000000 

        mov     fx,fx,LSR #6            ;  x1 = (xtemp >> 6) * x0 >> 14

        mul     fx,fy,fx

        mov     fx,fx,LSR #14



        mul     fy,lr,fx                ;  xtemp = minusY * x1

        sub     sreg12,sreg03,sreg12    ; r12 = final exponent, unbiased

        mov     fy,fy,LSR #12           ;  xtemp = (xtemp >> 12) * x1

        mul     fy,fx,fy 

        mov     fx,fx,LSL #14           ;  x2 = (x1<<14) + (xtemp >> 15)

        add     fx,fx,fy,LSR #15 



  #ifndef ARM_xM

        umull   fy,fx,sreg02,fx         ;  sigQ = (bits64)x * x2

        mov     sreg03,fx               ; save result mantissa

        orrmi   fx,fx,#0x80000000       ; pack result in fx: put sign

  #else

;  sigQ = (bits64)x * x2

        stmfd   sp!, {sreg04, sreg05}   ; save registers

        mov     fy,sreg02,LSR #16       ; fy = hi(x)

        mov     sreg03,fx,LSR #16       ; r3 = hi(x2)

        mul     sreg04,fy,sreg03        ; r4 = hi(x2) * hi(x)

        sub     sreg05,sreg02,fy,LSL #16  ; r5 = lo(x)

        sub     fx,fx,sreg03,LSL #16    ; fx = lo(x2)

        mul     fy,fx,fy                ; fy = lo(x2) * hi(x)

        mul     sreg03,sreg05,sreg03    ; r3 = hi(x2) * lo(x)

        orrmi   sreg04,sreg04,#0x80000000  ; pack result in r4: put sign

        adds    sreg03,sreg03,fy        ; r3 = hi(x2) * lo(x) + lo(x2) * hi(x)

        mul     fy,fx,sreg05            ; fy = lo(x2) * lo(x)

        addcs   sreg04,sreg04,#0x10000  ; propagate carry

        adds    fy,fy,sreg03,LSL #16    ; fx:fy - final result

        adc     fx,sreg04,sreg03,LSR #16

        ldmfd   sp!, {sreg04, sreg05} 

        bic     sreg03,fx,#0x80000000   ; r3 = unsigned result

  #endif  

;

; Now:  r3 = mantissa(x/y), not rounded

;       fx = r3 | sign bit

;       fy = extra precision bits for fx

;

        adds    sreg12,sreg12,#0x7e     ; r12 = biased exponent - 1

        bmi.w     fdiv_negative_or_zero_exp

        cmp     sreg12,#0xfe

        bge.w     __f_overflow            ; overflow 

        add     fx,fx,sreg12,LSL #23    ; fx = packed result, not rounded

  #ifdef IEEE_FULL

; compute the remainder and check for an exact result

        rsb     lr,lr,#0

        mul     fy,lr,sreg03            ; fy = y * q

        cmp     fy,sreg02,LSL #23       ; fy == x ? exact

        Reteq

        add     fy,fy,lr                ; fy = y * (q + 1 ulp)

        cmp     fy,sreg02,LSL #23       ; fy <= x ? q++

        beq.w     fdiv_increment_return

        addmi   fx,fx,#1

        subpl   fy,fy,lr                ; now fy = y*q <= x <= y*(q + 1)



        LFPState sreg03,sreg12

        SetInexact sreg03

        StFPState sreg03,sreg12



        tst     sreg03, CHOP_OR_DOWN_BIT_MASK

        bne.w     fdiv_chop_or_down_round

        tst     sreg03, UP_OR_NEAR_BIT_MASK 

        bne.w     fdiv_up_round 



        add     fy,lr,fy,LSL #1         ; fy = b * (q + 0.5 ulp)

        cmp     fy,sreg02,LSL #24       ; test sign of remainder: x - y*(q+0.5)

        Retpl

fdiv_increment

        add     fx,fx,#1 

        mov     lr,fx,LSL #1            ; look for overflow

        cmn     lr,#0x01000000          ; compute sreg03 + 0x01000000

        Retcc                           ; exit if overflow did not occur  

        SetOverflow sreg03              ; signal overflow

        StFPState sreg03,sreg12         ; store FPSCR

  #else

        mov     sreg12,fy,LSR #28       ; if the chopped bits are 0111....

        cmp     sreg12,#7               ; the round bit may not be exact

        beq.w     fdiv_remainder_round    ; round according to the remainder

    #ifdef IEEE_FIXED

        tst     fy,#0x80000000

        Reteq



fdiv_increment

        add     fx,fx,#1 

        mov     lr,fx,LSL #1            ; look for overflow

        cmn     lr,#0x01000000          ; compute sreg03 + 0x01000000

        Retcc                           ; exit if overflow did not occur  



        LFPState lr,sreg12

        SetOverflow lr                  ; signal overflow

        StFPState lr,sreg12             ; store FPSCR

    #else

        add     fx,fx,fy,LSR #31        ; bump up result if the round bit is 1

    #endif

  #endif

        Return



; initial aproximation for reciprocal (256 entries)  

rtable

	DCB 255, 255, 254, 253, 252, 251, 250, 249

	DCB 248, 247, 246, 245, 244, 243, 242, 241

	DCB 240, 240, 239, 238, 237, 236, 235, 234

	DCB 234, 233, 232, 231, 230, 230, 229, 228

	DCB 227, 226, 226, 225, 224, 223, 223, 222

	DCB 221, 220, 220, 219, 218, 217, 217, 216

	DCB 215, 215, 214, 213, 212, 212, 211, 210

	DCB 210, 209, 208, 208, 207, 206, 206, 205

	DCB 204, 204, 203, 203, 202, 201, 201, 200

	DCB 200, 199, 198, 198, 197, 197, 196, 195

	DCB 195, 194, 194, 193, 192, 192, 191, 191

	DCB 190, 190, 189, 189, 188, 188, 187, 186

	DCB 186, 185, 185, 184, 184, 183, 183, 182

	DCB 182, 181, 181, 180, 180, 179, 179, 178

	DCB 178, 177, 177, 176, 176, 175, 175, 175

	DCB 174, 174, 173, 173, 172, 172, 171, 171

	DCB 170, 170, 170, 169, 169, 168, 168, 167

	DCB 167, 167, 166, 166, 165, 165, 164, 164

	DCB 164, 163, 163, 162, 162, 162, 161, 161

	DCB 160, 160, 160, 159, 159, 158, 158, 158

	DCB 157, 157, 157, 156, 156, 155, 155, 155

	DCB 154, 154, 154, 153, 153, 153, 152, 152

	DCB 152, 151, 151, 150, 150, 150, 149, 149

	DCB 149, 148, 148, 148, 147, 147, 147, 146

	DCB 146, 146, 145, 145, 145, 145, 144, 144

	DCB 144, 143, 143, 143, 142, 142, 142, 141

	DCB 141, 141, 140, 140, 140, 140, 139, 139

	DCB 139, 138, 138, 138, 138, 137, 137, 137

	DCB 136, 136, 136, 136, 135, 135, 135, 134

	DCB 134, 134, 134, 133, 133, 133, 133, 132

	DCB 132, 132, 131, 131, 131, 131, 130, 130

	DCB 130, 130, 129, 129, 129, 129, 128, 128



  #ifdef IEEE_FULL

fdiv_chop_or_down_round

        tst     sreg03, ZERO_OR_DOWN_BIT_MASK

        Retne                           ; chop 

        cmp     fx,#0

        bmi     fdiv_increment          ; round down

        Return



fdiv_up_round

        cmp     fx,#0

        bpl     fdiv_increment

        Return

  #else

;

; Round to nearest: 

;   q <= x/y < q+1. Return q+1 if x/y is closer to q+1

;   In the latter case, x >= y * (q + 0.5 ulp)

;

fdiv_remainder_round

        mov     fy,sreg03,LSL #1        ; compute quotient mantissa + 0.5 ulp

        add     fy,fy,#1

        rsb     lr,lr,#0                ; lr = mantissa(y)

        mul     fy,lr,fy

        cmp     fy,sreg02,LSL #24       ; test sign of remainder: x - y*(q+0.5)

    #ifdef IEEE_FIXED

        bmi     fdiv_increment

    #else

        addmi   fx,fx,#1                ; if (y*(q+0.5)<= x) q++;

    #endif

        Return

  #endif



; ============================================================================

; The code for special cases begins here

; ============================================================================ 



; arrive here if x is either tiny or huge

; fx has x significand and lower bit of exp, sreg03 has x exponent

fdiv_x_special_case

        eor     lr,fx,fy                ; put sign in lr

        and     lr,lr,#0x80000000

        cmp     sreg03, #0

        #ifdef IEEE_STD

          beq     fdiv_x_zero

        #else

          beq     fdiv_x_tiny             ; x is tiny

        #endif

; otherwise fall through fdiv_x_huge



; arrive here if x exp is 0xff

; lr has sign

; fx has significand of x and lower bit of exp

fdiv_x_huge

        movs    fx, fx, LSL #9          ; clear high bit

        bne.w     __f_result_x_NaN        ; check x and return a QNaN

; x is INF 

; must unpack y

; unpack fy in sreg12(exp) and fy(significand)  

        mov     sreg12, fy, LSR #23     ; sreg12 has exponent and sign

        mov     fy, fy, LSL #9          ; fy has significand  

        ands    sreg12, sreg12, #0xff   ; clear sign bit

        beq.w     __f_result_INF          ; INF / 0 = INF / denorm = INF

        cmp     sreg12, #0xff 

        blt.w     __f_result_INF          ; INF / norm = INF

; if we arrive here x is INF, y is either INF, NaN

        cmp     fy, #0

        beq.w     __f_result_invalid      ; INF / INF = Invalid

        b       fdiv_y_NaN              ; INF / NaN = QNaN



; arrive here if y is either tiny or huge

; lr has the sign

; fx has x significand and lower bit of exp, sreg03 has x exponent 

; fy has y significand and lower bit of exp, sreg12 has y exponent

fdiv_y_special_case

        eor     lr,fx,fy                ; put sign in lr

        and     lr,lr,#0x80000000

        cmp     sreg12, #0

        #ifdef IEEE_STD

          beq     __f_result_INF

        #else

          beq     fdiv_y_tiny             ; y is tiny

        #endif

; otherwise fall through fdiv_y_huge



; arrive here if y exp is 0xff AND x is a normal number or a denorm

; lr has the sign

; sreg03 has x exp, fx has x | lead bit

; sreg12 has y exp, fy has y 

fdiv_y_huge

        movs    fy, fy, LSL #9          ; clear high bit

        bne.w     fdiv_y_NaN              ; check y and return a QNaN

; y is INF, x is a normal number, so return 0

        b       __f_result_zero



  #ifndef IEEE_STD

; arrive here if x exp is 0x00 

; lr has the sign

; sreg03 has x exp, fx has x significand and lower bit of exp

fdiv_x_tiny

        movs    sreg02, fx, LSL #9      ; clear high bits

        beq     fdiv_x_zero             ; x is zero

; x is a denorm

        Normalize_num sreg02, sreg03, #0  ; normalize number

        mov     sreg02,sreg02,LSR #8

; unpack fy in sreg12(exp) and lr (significand)  

        ands    sreg12,sreg12,fy,LSR #23  ; sreg12 has exponent

        beq     fdiv_x_tiny_y_tiny

        cmp     sreg12, #0xff           ; huge exponent

        beq     fdiv_y_huge

        orr     fy,fy,#0x00800000       ; set lead bit  

        bic     lr,fy,#0xff000000       ; lr = mantissa(y)

        b       fdiv_start 

  #endif



fdiv_x_zero

; unpack fy in sreg12(exp) and fy(significand)  

        mov     sreg12, fy, LSR #23     ; sreg12 has exponent and sign

        mov     fy, fy, LSL #9          ; fy has significand 

        ands    sreg12, sreg12, #0xff   ; clear sign bit

  #ifdef IEEE_STD

        beq     __f_result_invalid

  #else

        beq     fdiv_xtiny_checky       ; look for norm/denorm

  #endif

        cmp     sreg12, #0xff

        blt.w     __f_result_zero         ; 0 / norm = 0

        cmp     fy, #0

        beq.w     __f_result_zero         ; O / INF = 0 

        b       fdiv_y_NaN              ; y is a NaN

        

  #ifndef IEEE_STD

fdiv_xtiny_checky

        cmp     fy, #0

        beq.w     __f_result_invalid      ; signal div by zero and invalid

        b       __f_result_zero         ; 0 / denorm = 0



; arrive here if y exp is 0x00 AND x is a normal number or a denorm

; lr has the sign

; sreg03 has x exp, fx has x significand and lead bit

; sreg12 has y exp, fy has y significand and lower bit of exp

fdiv_y_tiny

        movs    sreg12, fy, LSL #9      ; clear high bit

        beq.w     __f_result_INF_divzero  ; normal / 0 = INF, div by zero

; y is a denorm, so normalize and compute the result

        mov     lr,sreg12

        Normalize_num lr, sreg12, #0    ; normalize number

        mov     lr,lr,LSR #8            ; lr = mantissa(y)

        orr     fx,fx,#0x00800000 

        bic     sreg02,fx,#0xff000000   ; r2 = mantissa(x) - clear remaining exponent bits

        b       fdiv_start



fdiv_x_tiny_y_tiny

        movs    sreg12, fy, LSL #9      ; clear high bit

        beq.w     __f_result_INF_divzero  ; normal / 0 = INF, div by zero

; y is a denorm, so normalize and compute the result

        mov     lr,sreg12

        Normalize_num lr, sreg12, #0    ; normalize number

        mov     lr,lr,LSR #8            ; lr = mantissa(y)

        b       fdiv_start

  #endif



; arrive here if the exponent is zero or less after operations

; must check for special cases  

;

; fx = sign | quotient

; r2 = mantissa(x)

; r3 = quotient

; r12 = biased exponent - 1

; lr = - mantissa(y)

;

fdiv_negative_or_zero_exp

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg01, sreg01

        tst     sreg01, FE_IEEE_MASK_UNDERFLOW

        bne.w     __wrap_underflow_handler

  #endif

        and     fx,fx,#0x80000000       ; prepare signed 0 in fx        

  #ifdef IEEE_STD

        Return                          ; return signed 0 in STD mode 

  #else      

        cmn     sreg12, #24

        beq     fdiv_very_tiny_result   ; case: very tiny

        bmi.w     __f_underflow           ; case: underflow 



; exponent is -23...-1



        add     fy,sreg12,#23

        mov     sreg02,sreg02,LSL fy    ; reposition x



        rsb     sreg12, sreg12, #0      ; shift ammount

        mov     sreg03,sreg03,LSR sreg12  ; r3 = normalized result (q)

        orr     fx,fx,sreg03            ; fx = denormalized result, signed, not rounded



        rsb     lr,lr,#0

        mul     fy,lr,sreg03            ; fy = y * q

        cmp     fy,sreg02               ; fy == x ? exact

        Reteq

        add     fy,fy,lr                ; fy = y * (q + 1 ulp)

        cmp     fy,sreg02               ; fy <= x ? q++

        beq     fdiv_increment_return

        addmi   fx,fx,#1

        subpl   fy,fy,lr                ; now fy = y*q <= x <= y*(q + 1)



	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg03,sreg12

        SetUnderflow sreg03

        StFPState sreg03,sreg12

      #ifdef IEEE_FULL

        tst     sreg03, CHOP_OR_DOWN_BIT_MASK

        bne     fdiv_chop_or_down_round

        tst     sreg03, UP_OR_NEAR_BIT_MASK 

        bne     fdiv_up_round           ; rounding is up    

      #endif

    #endif

        add     fy,lr,fy,LSL #1         ; fy = b * (q + 0.5 ulp)

        cmp     fy,sreg02,LSL #1        ; test sign of remainder: x - y*(q+0.5)

        and     sreg03,fx,#1

        addmi   fx,fx,#1 

        addeq   fx,fx,sreg03            ; round to nearest even if equal

        Return



fdiv_increment_return

        add     fx,fx,#1

        Return



; the result is smaller than the smallest denorm

fdiv_very_tiny_result

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg03, sreg12

        SetUnderflow sreg03             ; set underflow

        StFPState sreg03,sreg12         ; store FPSCR    

        tst     sreg03, CHOP_OR_DOWN_BIT_MASK

        bne.w     __f_underflow

        tst     sreg03, UP_OR_NEAR_BIT_MASK

        bne.w     __f_underflow           ; rounding is up    

    #endif     

        cmn     sreg02,lr               ; check if round up to the smallest denorm

        addne   fx,fx,#1

        Return

  #endif



; arrive here if y is a NaN

fdiv_y_NaN

        mov     fx, fy                  ; load into fx

        b       __f_result_NaN 



; arrive here when result is INF and div-by-zero must be set

; return signed INF

; lr holds the sign

__f_result_INF_divzero

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg03, sreg02

        SetDivByZero sreg03

        StFPState sreg03,sreg02

  #endif

; fall through __f_result_INF



; arrive here when result is INF

; return signed INF

; lr holds the sign

__f_result_INF

        mov     fx, #0xff000000         ; prepare code for INF

        orr     fx, lr, fx, LSR #1      ; make signed INF

        Return



__f_result_x_NaN

; x is a NaN

; must check if y is also a NaN and set invalid if any of them is signaling  

; fx has significand

; only happens in IEEE_FULL and IEEE_FIXED modes

	#ifdef IEEE_FULL_OR_FIXED

        tst     fx, #0x80000000

        beq     __f_result_invalid      ; fx has a signaling NaN

; x is a quite NaN, but y could be a signaling NaN

; unpack fy in sreg12(exp) and fy(significand)  

        mov     sreg12, fy, LSR #23     ; sreg12 has exponent and sign

        mov     fy, fy, LSL #9          ; fy has significand 

        ands    sreg12, sreg12, #0xff   ; clear sign bit

        beq     __f_result_make_NaN     ; make NaN for 0

        cmp     sreg12, #0xff

        blt     __f_result_make_NaN     ; make NaN for normals

        cmp     fy, #0

        beq     __f_result_make_NaN     ; make NaN for inf

        b       fdiv_y_NaN              ; y is a NaN    

  #endif  



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

        SetInvalidE sreg03              ; set invalid

        StFPState sreg03,sreg02         ; store FPSCR  

        Return

  #else

        MakeFNaN

        Return

  #endif



; set the invalid flag  

__f_result_invalid

	#ifdef IEEE_FULL_OR_FIXED

; @@@ Modified: added LFPState

;ldr       sreg02, =__SoftFPSCR__            ; load FPSCR address

;ldr    sreg03, [sreg02]                               ; load FPSCR

        LFPState sreg03, sreg02

        SetInvalidE sreg03              ; set invalid

        StFPState sreg03,sreg02         ; store FPSCR  

  #endif

; fall through make NaN



; arrive here when a NaN must be returned without any checks    

__f_result_make_NaN

        MakeFNaN

        Return



; an overflow occured, INF must be returned             

; arrive here high bit of fx = sign         

__f_overflow

	#ifdef IEEE_FULL_OR_FIXED

        LFPState sreg03, sreg01

        SetOverflowL sreg03, __wrap_overflow_handler_pack

        StFPState sreg03,sreg01         ; set overflow 

  #endif

        tst     fx, #0x80000000

        mov     fx, #0xff000000         ; prepare code for INF

        movne   fx, fx, ASR #1

        moveq   fx, fx, LSR #1

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

        subne   fx, fx, fx, LSR #31     ; sub 1 for negative numbers if rounding up

        Return



oflow_chopdown

        sub     fx, fx, #1              ; presume that 1 is always substracted

        tst     sreg03, ZERO_OR_DOWN_BIT_MASK

        addeq   fx, fx, fx, LSR #31     ; if rounding down, add the sign 

        Return

  #endif                                   
; #ifndef IEEE_FULL



; an underflow occured; must round in IEEE_FULL mode

  #ifndef IEEE_STD

__f_underflow

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

        bne     underflow_chop_or_down_round

        tst     sreg03, UP_OR_NEAR_BIT_MASK

        Reteq                           ; round no nearest: do nothing

        cmp     fx, #0

        addeq   fx, fx, #1              ; up: add 1 if positive

        Return

underflow_chop_or_down_round

        tst     sreg03, ZERO_OR_DOWN_BIT_MASK

        addeq   fx, fx, fx, LSR #31     ; add sign for down

        Return

    #endif                                 
; #ifndef IEEE_FULL 

  #endif



; the number is 0, so return a signed 0

__f_result_zero

  #ifdef IEEE_STD

        mov     fx,#0

  #else

        mov     fx,lr                   ; signed zero

  #endif  

        Return



	#ifdef IEEE_FULL_OR_FIXED

	IMPORT __fp_handler_32_to_32

	IMPORT __fp_handler_binary_32_to_32



; overflow exception handler    

__wrap_overflow_handler

        Clrstk(al)

        sub     fx, fx, #(192 << 23)    ; coerce exponent

        ldr     r1, =(FE_EX_OVERFLOW :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_DIV)

        bl      __fp_handler_32_to_32

        Return_lr



; overflow exception handler    

__wrap_overflow_handler_pack

        Clrstk(al)

        sub     sreg12, sreg12, #192    ; coerce exponent

        add     fx, fx, sreg12, LSL #23  ; exp := exp - 1 + lead bit  

        ldr     r1, =(FE_EX_OVERFLOW :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_DIV)

        bl      __fp_handler_32_to_32

        Return_lr



; underflow handler

; fx holds the significand and sign, exp-1 is in sreg12

__wrap_underflow_handler

        Clrstk(al)

        add     sreg12, sreg12, #192    ; coerce exponent

        add     fx, fx, sreg12, LSL #23  ; exp := exp - 1 + lead bit

        ldr     r1, =(FE_EX_UNDERFLOW :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_DIV)

        bl      __fp_handler_32_to_32 

        Return_lr



; division by zero wrapper

; fx/fy have the original operands

__wrap_divbyzero_handler

        Clrstk(al)

        ldr     r2, =(FE_EX_DIVBYZERO :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_DIV)

        bl      __fp_handler_binary_32_to_32

        Return_lr



; invalid exception handler

__wrap_invalid_handler

        ldmfd   sp!, {fx, fy}

        ldr     r2, =(FE_EX_INVALID :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_DIV)

        bl      __fp_handler_binary_32_to_32 

        Return_lr



  #endif



  #ifdef IEEE_FULL



; inexact exception wrapper

__wrap_inexact_handler

        Clrstk(al)

        ldr     r1, =(FE_EX_INEXACT :OR: FE_EX_INTYPE_FLOAT :OR: FE_EX_OUTTYPE_FLOAT :OR: FE_EX_FN_DIV)

        bl      __fp_handler_32_to_32

        Return_lr

  #endif



#endif
	END

/* Change record:
 * clm  July 12, 2004 Add .text directives to each individual function to enable dead stripping
 *      		  Put the FPLIB entry point first for dead stripping
 */
