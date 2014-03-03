/*
 *	File:		CF_runtime.c
 *	Copyright © 2005-2006 Freescale Semiconductor, Inc. All Rights Reserved.
 *				©1993-1995 metrowerks Inc. All rights reserved
 *
 *	Content:	runtime support
 *
 */


#ifdef __cplusplus
extern "C" {
#endif

extern void *__clear(char *mem, __typeof__(sizeof(0)) size);
extern __declspec(register_abi) long __ldivu__(unsigned long left,unsigned long right);
extern __declspec(register_abi) long __ldiv__(long left,long right);
extern __declspec(register_abi) long __lmodu__(unsigned long left,unsigned long right);
extern __declspec(register_abi) long __lmod__(long left,long right);

extern int __longlongswitchconvertor__  (long long * arr, long range, long long value);

extern __declspec(register_abi) long __ldivu__(unsigned long left, unsigned long right)
{
unsigned long bit = 1;
unsigned long res = 0;

    if (right== 0) asm { illegal }
    if (left == 0) return 0;

    while (right < left && bit && !(right & 0x80000000))
    {
        right <<= 1;
        bit <<= 1;
    }
    while (bit != 0)
    {
        if (left >= right)
        {
            left -= right;
            res |= bit;
        }
        bit >>= 1;
        right >>= 1;
    }
    return res;
}

extern __declspec(register_abi) long __ldiv__(long left,long right)
{
    if (right== 0) asm { illegal }
    if (left == 0) return 0;
    if (left < 0)
        if (right < 0)
            return __ldivu__(-left, -right);
        else
            return -__ldivu__(-left, right);
    else
        if (right < 0)
            return -__ldivu__(left, -right);
        else
            return __ldivu__(left, right);
}

extern __declspec(register_abi) long __lmodu__(unsigned long left,unsigned long right)
{
unsigned long bit = 1;
unsigned long res = 0;

    if (right== 0) asm { illegal }
    if (left == 0) return 0;

    while (right < left && bit && !(right & 0x80000000))
    {
        right <<= 1;
        bit <<= 1;
    }
    while (bit != 0)
    {
        if (left >= right)
        {
            left -= right;
            res |= bit;
        }
        bit >>= 1;
        right >>= 1;
    }
    return left;
}

extern __declspec(register_abi) long __lmod__(long left, long right)
{
    if (right== 0) asm { illegal }
    if (left == 0) return 0;
    if (left < 0)
        if (right < 0)
            return __lmodu__(-left, -right);
        else
            return -__lmodu__(-left, right);
    else
        if (right < 0)
            return -__lmodu__(left, -right);
        else
            return __lmodu__(left, right);
}

extern int __longlongswitchconvertor__  (long long * arr, long range, long long value)
{
	long long * p=arr; 
	while ((p-arr) < range) 
	{
	  if (*p== value) return p-arr;
	  p++;
	} 
	return -1;
}  

/*
 *	__copy		-	copy 'size' bytes data from 'from' to 'to'
 *
 *	We return the pointer to the destination. If it is 0, no copy is performed.
 *
 */

void *__copy(char *to, char *from, __typeof__(sizeof(0)) size)
{
	char *p;
	
	if (to && size) {
		p = to;
		do {
			*p = *from;
			++p;
			++from;
			--size;
		} while (size);
	}
	
	return(to);
}

/*
 *	__clear		-	clear 'size' bytes of data at 'mem'
 *
 *	We return the pointer to the destination. If it is 0, no clear is performed.
 *
 */
void *__clear(char *mem, __typeof__(sizeof(0)) size)
{
	char *ptr;

	if((ptr = mem) != 0 && size != 0) {
		for(;;) {
			*ptr++ = 0;
			if(--size == 0) break;
		}
	}
	return mem;
}

#ifdef __cplusplus
} 
#endif


