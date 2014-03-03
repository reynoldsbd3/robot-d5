	#include "support.s"
	
#ifdef _AEABI
;==============================================================        
		EXPORT __aeabi_lcmp
		
		EXPORT __aeabi_ulcmp
		
;============================================================

	AREA |.text|,CODE,READONLY

__aeabi_lcmp
			subs	ip, xxl, yyl
			sbcs	ip, xxh, yyh
			subeqs  ip, xxl, yyl
			mov		r0, ip
			mov		pc, lr			; RET
			
;============================================================

__aeabi_ulcmp
			cmp	xxh, yyh
			movlo	r0, #-1
			movhi	r0, #1
			movne	pc, lr			; RETc(ne)
			subs 	r0, xxl, yyl	; cmp	xxl, yyl
			mov	r0, r0, asr #31
			movhi	r0, #1
			mov	pc, lr			; RET
			
;============================================================

#endif
	END
/
* April 07, 2006 Added with reference to 4.2 The long long helper functions,
* in RUN-TIME ABI FOR THE ARM ARCHITECTURE document
*/
