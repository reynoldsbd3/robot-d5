/* EWL
 * Copyright © 1995-2010 Freescale Corporation.  All rights reserved.
 *
 *  $Date: 2012/06/28 06:41:24 $ 
 *  $Revision: 1.2 $ 
 */

/*
 *	Prefix file for EWL C++ projects which pulls in the most common precompiled
 *	EWL Header files.
 *
 *	This prefix file is ONLY for C++ projects, it will not work for C projects unless
 *	C++ is activated.
 */

#if __POWERPC__ && !__PPC_EABI__
	#include <EWLHeaders.PPC.mch>
#elif __INTEL__
	#include <EWLHeaders.x86.mch>
#elif defined(__arm)
	#if __option(little_endian)
		#ifndef __thumb
			#include <EWLHeaders_ARM_LE.mch>
		#else
			#include <EWLHeaders_Thumb_LE.mch>
		#endif
	#else
		#ifndef __thumb
			#include <EWLHeaders_ARM_BE.mch>
		#else
			#include <EWLHeaders_Thumb_BE.mch>
		#endif
	#endif
#else
	#error Unknown platform!
#endif

