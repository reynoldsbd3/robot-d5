#define _AEABI 1

#ifdef _AEABI
		EXPORT __aeabi_memset
		EXPORT __aeabi_memset4
		EXPORT __aeabi_memset8
		
		IMPORT __FSL_memset
		IMPORT __FSL_memset_w
		
		AREA |.text|,CODE,READONLY 

__aeabi_memset
         and         r3,r2,#255
         orr         r2,r3,r3,lsl #8
         orr         r2,r2,r2,lsl #16
         b           __FSL_memset           
		
__aeabi_memset8
__aeabi_memset4
         and         r3,r2,#255
         orr         r2,r3,r3,lsl #8
         orr         r2,r2,r2,lsl #16
         b           __FSL_memset_w         

#endif		
	END

/* Chipwerks change record:
* April 07, 2006 Added with reference to 4.3.4 Memory copying, clearing, and setting,
* in RTABIRTABIRUN-TIME ABI FOR THE ARM ARCHITECTURE document
*/
