/* CodeWarrior ARM Runtime Support Library 
 * Copyright © 1995-2003 Metrowerks Corporation. All rights reserved. 
 * 
 * $Date: 2012/06/01 16:05:25 $ 
 * $Revision: 1.1 $ 
 */ 

/*
 *	semihosted_console_io.c	-	redirect console I/O through ARM semihosting interface
 *
 */

#include "console_io.h"
#include "semihosting.h"

/*
 *	sys_writec		-	write a character to the console via semihosting interface
 *
 */

void sys_writec(register void *cp) _EWL_WEAK _EWL_DO_NOT_INLINE;
void sys_writec(register void *cp)
{
#ifndef __thumb // ARM version

		// save return address on stack
		__asm ("str		lr,[sp,#-4]!");

		__asm ("mov		r1,%0"::"r"(cp));
		__asm ("mov		r0,%0"::"i"(SYS_WRITEC));
		__asm ("swi		%0"::"i"(SEMIHOSTING_SWI));

		// restore return address from stack and return
		__asm ("ldr		pc,[sp],#4");

#else // Thumb version

		// save return address on stack
		__asm ("push		{lr}");

		__asm ("mov		r1,%0"::"r"(cp));
		__asm ("mov		r0,%0"::"i"(SYS_WRITEC));

#if defined(__thumb2)
		__asm ("bkpt		%0"::"i"(SEMIHOSTING_SWI));
#else
		__asm ("swi		%0"::"i"(SEMIHOSTING_SWI));

#endif

		// restore return address from stack and return
		__asm ("pop		{pc}");

#endif 
}


/*
 *	sys_write0		-	write a nul-terminated string to the console via semihosting interface
 *
 */

void sys_write0(register void *str)  _EWL_WEAK _EWL_DO_NOT_INLINE;
void sys_write0(register void *str)
{
#ifndef __thumb // ARM version

		// save return address on stack
		__asm ("str		lr,[sp,#-4]!");

		__asm ("mov		r1,%0"::"r"(str));
		__asm ("mov		r0,%0"::"i"(SYS_WRITEC));
		__asm ("swi		%0"::"i"(SEMIHOSTING_SWI));

		// restore return address from stack and return
		__asm ("ldr		pc,[sp],#4");

#else // Thumb version

		// save return address on stack
		__asm ("push		{lr}");

		__asm ("mov		r1,%0"::"r"(str));
		__asm ("mov		r0,%0"::"i"(SYS_WRITEC));

#if defined(__thumb2)
		__asm ("bkpt		%0"::"i"(SEMIHOSTING_SWI));
#else
		__asm ("swi		%0"::"i"(SEMIHOSTING_SWI));
#endif

		// restore return address from stack and return
		__asm ("pop		{pc}");

#endif 
}


/*
 *	sys_readc		-	read a character from the console via semihosting interface
 *
 */

int sys_readc(void)  _EWL_WEAK _EWL_DO_NOT_INLINE;
int sys_readc(void)
{
#ifndef __thumb // ARM version

		// save return address on stack
		__asm ("str		r14,[r13,#-4]!");
		
		__asm ("mov		r1,#0");
		__asm ("mov		r0,%0"::"i"(SYS_READC));
		__asm ("swi		%0"::"i"(SEMIHOSTING_SWI));

		// restore return address from stack and return
		__asm ("ldr		pc,[sp],#4");

#else // Thumb version

		// save return address on stack
		__asm ("push		{lr}");

		__asm ("mov		r1,#0");
		__asm ("mov		r0,%0"::"i"(SYS_READC));

#if defined(__thumb2)
		__asm ("bkpt		%0"::"i"(SEMIHOSTING_SWI));
#else
		__asm ("swi		%0"::"i"(SEMIHOSTING_SWI));
#endif

		// restore return address from stack and return
		__asm ("pop		{pc}");

#endif 
}


/*
 *	sys_exit		-	terminate an application via semihosting interface
 *
 */

void sys_exit(int status)  _EWL_WEAK _EWL_DO_NOT_INLINE;
void sys_exit(int status)
{
	int addr = status == 0 ? ADP_Stopped_ApplicationExit : ADP_Stopped_RunTimeErrorUnknown;
	

		__asm ("mov		r1,%0"::"r"(addr));
		__asm ("mov		r0,#0x18");    /* same as gcc libs, angel_SWIreason_ReportException */
#if defined(__thumb2)
		__asm ("bkpt		%0"::"i"(SEMIHOSTING_SWI));
#else
		__asm ("swi		%0"::"i"(SEMIHOSTING_SWI));
#endif
		__asm ("mov		pc,lr\n");
}


/*
 *	__read_console	-	read from the console into 'buffer' until end-of-line or 'count' characters have been read
 *
 */

int __read_console(__std(__file_handle) handle, unsigned char *buffer, __std(size_t) *count)
{
	if ( *count <= 1)
	{
		buffer[0] = sys_readc();
		return(__no_io_error);
	}

	__read_file(handle,buffer,count);

	return(__no_io_error);
}


/*
 *	__write_console	-	write 'count' bytes from 'buffer' to the console
 *
 */


int	__write_console(__std(__file_handle) handle, unsigned char *buffer, __std(size_t) *count)
{
#if !defined(__SEMIHOSTING)
	#pragma unused(handle)
#endif
	__std(size_t) i, cnt;

#define _WC_BUFLEN 80
	static unsigned char tmpstring[_WC_BUFLEN+1];

	cnt = *count;

	if ( cnt <= 1)
	{
		sys_writec(buffer);
		return(__no_io_error);
	}

	i=0;

/* Use this for output to UART Terminal */

#if !defined(__SEMIHOSTING) 
	while ( cnt > _WC_BUFLEN )
	{

		strncpy(tmpstring,&buffer[i],_WC_BUFLEN);
		tmpstring[_WC_BUFLEN]=0;
		sys_write0(tmpstring);
		cnt -= _WC_BUFLEN;
		i += _WC_BUFLEN;
	}

	if ( cnt ) {
		strncpy(tmpstring,&buffer[i],cnt);
		tmpstring[cnt]=0;
		sys_write0(tmpstring);
	}

/* Use this for simulator output using semihosting interface (sys_write0 did not work for 
	gnu qemu simulator */
#else /* __SEMIHOSTING */
	__write_file(handle,buffer,count);
#endif

	return(__no_io_error);
}


/*
 *	__close_console	-	close the console
 *
 */

int __close_console(__std(__file_handle) handle)
{
	#pragma unused(handle)

	return(__no_io_error);
}
