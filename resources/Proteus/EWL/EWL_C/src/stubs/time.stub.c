/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:31 $
 * $Revision: 1.1 $
 */


/* Include your OS dependent include files */
#include <time.h>

#if !_EWL_OS_TIME_SUPPORT
#error time.stub.c cannot be in your project if _EWL_OS_TIME_SUPPORT is defined to 0.
#endif

clock_t __get_clock(void)
{
#warning "replace 0 with an OS call"
	return 0; 
}

time_t __get_time(void)
{
#warning "replace 0 with an OS call"
	return 0; 
}

int __to_gm_time(time_t * time)
{
#warning "replace 0 with an OS call"
	return 0; 
}
