#define _AEABI 1
	
#ifdef _AEABI

	EXPORT __aeabi_uread4
	EXPORT __aeabi_uread8
	EXPORT __aeabi_uwrite4
	EXPORT __aeabi_uwrite8
	EXPORT __FSL__rt_uread4
	EXPORT __FSL__rt_uread8
	EXPORT __FSL__rt_uwrite4
	EXPORT __FSL__rt_uwrite8
	
	
	AREA |.text|,CODE,READONLY

__aeabi_uread4
__FSL__rt_uread4
         ldrb        r2,[r0,#1]
         ldrb        r1,[r0]
         ldrb        r3,[r0,#2]
         ldrb        r0,[r0,#3]
         orr         r1,r1,r2,lsl #8
         orr         r1,r1,r3,lsl #16
         orr         r0,r1,r0,lsl #24
         mov 		 pc,lr

__aeabi_uread8
__FSL__rt_uread8
         ldrb        r3,[r0,#5]
         ldrb        r2,[r0,#4]
         ldrb        r12,[r0,#6]
         ldrb        r1,[r0,#7]
         orr         r2,r2,r3,lsl #8
         ldrb        r3,[r0,#2]
         orr         r2,r2,r12,lsl #16
         orr         r1,r2,r1,lsl #24
         ldrb        r2,[r0,#1]
         ldrb        r12,[r0]
         ldrb        r0,[r0,#3]
         orr         r12,r12,r2,lsl #8
         orr         r12,r12,r3,lsl #16
         orr         r0,r12,r0,lsl #24
         mov 		 pc,lr         

__aeabi_uwrite4
__FSL__rt_uwrite4
          mov         r2,r0,lsr #8
          strb        r0,[r1]
          strb        r2,[r1,#1]
          mov         r2,r0,lsr #16
          strb        r2,[r1,#2]
          mov         r2,r0,lsr #24
          strb        r2,[r1,#3]
          mov 		  pc,lr

__aeabi_uwrite8
__FSL__rt_uwrite8
          mov         r3,r0,lsr #8
          
          strb        r0,[r1]
          strb        r3,[r1,#1]
          mov         r3,r0,lsr #16
          strb        r3,[r1,#2]
          mov         r3,r0,lsr #24
          strb        r3,[r1,#3]
          mov         r3,r2,lsr #8
          strb        r2,[r1,#4]
          strb        r3,[r2,#5]
          mov         r3,r2,lsr #16
          strb        r3,[r1,#6]
          mov         r3,r2,lsr #24
          strb        r3,[r1,#7]
          mov 		  pc,lr

#endif          
	END
