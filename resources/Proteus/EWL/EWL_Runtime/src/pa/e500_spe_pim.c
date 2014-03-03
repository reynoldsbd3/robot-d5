/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:06:51 $
 * $Revision: 1.1 $
 */

#if __SPE__

#include <spe.h>
#include <stdlib.h>

/*	Additional Library Routines	*/

#define __fix16factor	0x8000
#define __fix32factor	0x80000000
#define __fix64factor	0x8000000000000000ULL

#define __d2fs(d,s) ((long double)__fix##s##factor * (d))
#define __d2fu(d,s) ((long double)__fix##s##factor * ((d) + 0.5))

#define __double2sfix16(d) 	(int16_t)__d2fs(d,16)
#define __double2ufix16(d) 	(uint16_t)__d2fu(d,16)
#define __double2sfix32(d) 	(int32_t)__d2fs(d,32)
#define __double2ufix32(d) 	(uint32_t)__d2fu(d,32)
#define __double2sfix64(d)	(int64_t)__d2fs(d,64)
#define __double2ufix64(d)	(uint64_t)__d2fu(d,64)

#define UFIXED_MIN 0.0
#define UFIXED_MAX +1.0
#define SFIXED_MIN -1.0
#define SFIXED_MAX +1.0


int16_t _EWL_CDECL strtosfix16(const char *str, char **endptr) 
{
	double d = strtod(str, endptr);
	if (d >= SFIXED_MAX) {
		errno = ERANGE;
		return 0x7fff;
	} else if (d < SFIXED_MIN) {
		errno = ERANGE;
		return (int16_t)0x8000;
	} else {
		return __double2sfix16(d);
	}
}

int32_t _EWL_CDECL strtosfix32(const char *str, char **endptr)
{
	long double d = strtold(str, endptr);
	if (d >= SFIXED_MAX) {
		errno = ERANGE;
		return 0x7fffffff;
	} else if (d < SFIXED_MIN) {
		errno = ERANGE;
		return (int32_t)0x80000000;
	} else {
		return __double2sfix32(d);
	}
}

int64_t _EWL_CDECL strtosfix64(const char *str, char **endptr)
{
	long double d = strtold(str, endptr);
	if (d >= SFIXED_MAX) {
		errno = ERANGE;
		return 0x7fffffffffffffffLL;
	} else if (d < SFIXED_MIN) {
		errno = ERANGE;
		return (int64_t)0x8000000000000000LL;
	}
	return __double2sfix64(d);
}

uint16_t _EWL_CDECL strtoufix16(const char *str, char **endptr)
{
	double d = strtod(str, endptr);
	if (d >= UFIXED_MAX) {
		errno = ERANGE;
		return 0xffff;
	} else if (d < UFIXED_MIN) {
		errno = ERANGE;
		return 0x0000;
	}
	return __double2ufix16(d);
}

uint32_t _EWL_CDECL strtoufix32(const char *str, char **endptr)
{
	long double d = strtold(str, endptr);
	if (d >= UFIXED_MAX) {
		errno = ERANGE;
		return 0xffffffff;
	} else if (d < UFIXED_MIN) {
		errno = ERANGE;
		return 0x00000000;
	}
	else {
		return __double2ufix32(d);
	}
}

uint64_t _EWL_CDECL strtoufix64(const char *str, char **endptr)
{
	long double d = strtold(str, endptr);
	if (d >= UFIXED_MAX) {
		errno = ERANGE;
		return 0xffffffffffffffffULL;
	} else if (d < UFIXED_MIN) {
		errno = ERANGE;
		return 0x0000000000000000ULL;
	} else {
		return __double2ufix64(d);
	}
}

int16_t _EWL_CDECL atosfix16(const char *str)
{
	return (strtosfix16(str, 0L));
}

int32_t _EWL_CDECL atosfix32(const char *str)
{
	return (strtosfix32(str, 0L));
}

int64_t _EWL_CDECL atosfix64(const char *str)
{
	return (strtosfix64(str, 0L));
}

uint16_t _EWL_CDECL atoufix16(const char *str)
{
	return (strtoufix16(str, 0L));
}

uint32_t _EWL_CDECL atoufix32(const char *str)
{
	return (strtoufix32(str, 0L));
}

uint64_t _EWL_CDECL atoufix64(const char *str)
{
	return (strtoufix64(str, 0L));
}


#endif /*__SPE__*/

/*  Change Record
 *	ejs	030113	Moved functions from spe.h into source
 */
 