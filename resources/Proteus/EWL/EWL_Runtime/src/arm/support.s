#define _AEABI 1
#ifdef _AEABI

#ifdef BIG_ENDIAN
#define xxh r0
#define xxl r1
#define yyh r2
#define yyl r3
#define al	r1
#define ah	r0
#else
#define xxh r1
#define xxl r0
#define yyh r3
#define yyl r2
#define al	r0
#define ah	r1
#endif

#endif	
