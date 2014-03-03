#define _AEABI 1
	
#ifdef _AEABI

	EXPORT __aeabi_memclr
	EXPORT __aeabi_memclr4
	EXPORT __aeabi_memclr8
	
	EXPORT __FSL_memset
	EXPORT __FSL_memset_w
	
	AREA |.text|,CODE,READONLY 

__aeabi_memclr
         mov         r2,#0
__FSL_memset
         cmp         r1,#4
         bcc         _Bytes          
         ands        r12,r0,#3
         beq         __FSL_memset_w       
         rsb         r12,r12,#4
         strb        r2,[r0],#1
         cmp         r12,#2
         strgeb      r2,[r0],#1
         sub         r1,r1,r12
         strgtb      r2,[r0],#1
         b           __FSL_memset_w       
_Bytes
         movs        r12,r1,lsl #31
         strcsb      r2,[r0],#1
         strcsb      r2,[r0],#1
         strmib      r2,[r0],#1
         mov 	     pc,lr



__aeabi_memclr8
__aeabi_memclr4
         mov         r2,#0
__FSL_memset_w
         subs        r1,r1,#32
         stmfd       sp!,{lr}
         mov         lr,r2
         mov         r3,r2
         mov         r12,r2
_loop
         stmcsia     r0!,{r2-r3,r12,lr}
         stmcsia     r0!,{r2-r3,r12,lr}
         subcss      r1,r1,#32
         bcs         _loop                   
         movs        r1,r1,lsl #28
         stmcsia     r0!,{r2-r3,r12,lr}
         stmmiia     r0!,{r2-r3}
         movs        r1,r1,lsl #2
         ldmfd       sp!,{lr}
         strcs       r2,[r0],#4
         
         strmib      r2,[r0],#1
         strmib      r2,[r0],#1
         tst         r1,#1073741824
         strneb      r2,[r0],#1
         #ifdef THUMB
         	bx lr
         #else	
         	mov 		 pc,lr
         #endif

#endif
	END

/* Chipwerks change record:
* April 07, 2006 Added with reference to 4.3.4 Memory copying, clearing, and setting,
* in RUN-TIME ABI FOR THE ARM ARCHITECTURE document
*/
