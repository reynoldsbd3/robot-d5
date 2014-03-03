/*
 *	Runtime configuration file for ColdFire ...
 */

#ifndef __RUNTIMECONFIG_H__
#define __RUNTIMECONFIG_H__

/* copy ROM to RAM locations.  Set to 0 for more aggressive dead stripping ... */
#ifndef SUPPORT_ROM_TO_RAM
#define SUPPORT_ROM_TO_RAM			1
#endif

#endif
