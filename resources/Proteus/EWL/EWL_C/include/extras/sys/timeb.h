/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:43:25 $
 * $Revision: 1.1 $
 */

#ifndef    _EWL_TIMEB_H
#define    _EWL_TIMEB_H

#include <ansi_parms.h>
#include <sys/stat.h>

_EWL_BEGIN_EXTERN_C

#pragma options align=native

struct _timeb
{
    __std(time_t)    time;      /* Seconds in the current time                    */
    unsigned short   millitm;   /* Milliseconds in the current time               */
    short            timezone;  /* The difference,in minutes, between local time 
                                           and Greenwich Mean time                */
    short            dstflag;   /* 1 if Daylight Savings time is in effect        */
};

#define timeb _timeb
        
#pragma options align=reset

_EWL_END_EXTERN_C

#endif /* _EWL_TIMEB_H */
