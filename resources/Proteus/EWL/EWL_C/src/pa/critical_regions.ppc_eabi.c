/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:08 $
 * $Revision: 1.1 $
 */

#include "critical_regions.h"

#if _EWL_THREADSAFE


void __init_critical_regions(void)
{
}

void __kill_critical_regions(void)
{
}

void __begin_critical_region(int region)
{
#pragma unused(region)
}

void __end_critical_region(int region)
{
#pragma unused(region)
}

#endif 
