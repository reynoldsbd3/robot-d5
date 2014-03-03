/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:48:40 $
 * $Revision: 1.1 $
 */

/*
 *	Modification History
 *	--------------------
 *  rewritten by matt fassiotto on 7/30/97
 *  definitions were based on which architecture when
 *  they should have been based only on "endian-ness"
 *  which is now the case.
 *  This assumes both the "long" and "float" type are
 *  32 bits whenever  compiled
 */

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT

#include <ewl_misra_types.h>
#include <float.h>
#include <math.h>

int32_t __float_nan[]    = {0x7FFFFFFFL};
int32_t __float_huge[]   = {0x7f800000L};

#if (_EWL_DOUBLE_SIZE == 80) || (_EWL_DOUBLE_SIZE == 96)
	int32_t __double_min	[]	= {0x00018000L,0L,0L};
	int32_t __double_max	[]	= {0x7FFEFFFFL,(int32_t)0xFFFFFFFFL,(int32_t)0xFFFF0000L};
	int32_t __double_epsilon[]	= {0x3FC08000L,0L,0L};
	int32_t __double_tiny	[]	= {0L,0L,0x00010000L};
	int32_t __double_huge	[]	= {0x7FFF0000L,0L,0L};
	int32_t __double_nan	[]	= {0x7FFFFFFFL,(int32_t)0xFFFFFFFFL,(int32_t)0xFFFF0000L};

#elif (_EWL_DOUBLE_SIZE == 64) && _EWL_LITTLE_ENDIAN

	int32_t __double_min	[]	= {0L,0x00100000L};
	int32_t __double_max	[]	= {(int32_t)0xffffffffL,0x7fefffffL};
	int32_t __double_epsilon[]	= {0L,0x3cb00000L};
	int32_t __double_tiny	[]	= {1L,0L};
	int32_t __double_huge	[]	= {0L,0x7FF00000L} ;
	int32_t __double_nan	[]	= {(int32_t)0xFFFFFFFFL,0x7FFFFFFFL};

#elif (_EWL_DOUBLE_SIZE == 64) && !_EWL_LITTLE_ENDIAN

	int32_t __double_min	 [] = {0x00100000L ,0L};
	int32_t __double_max	 [] = {0x7fefffffL,(int32_t)0xffffffffL};
	int32_t __double_epsilon [] = {0x3cb00000L,0L};
	int32_t __double_tiny	 [] = {0L,1L};
	int32_t __double_huge	 [] = {0x7FF00000L,0L};
	int32_t __double_nan	 [] = {0x7FFFFFFFL,(int32_t)0xFFFFFFFFL};

#elif (_EWL_DOUBLE_SIZE == 32)

	int32_t __double_min	[]	= {0x00800000L};
	int32_t __double_max	[]	= {0x7F7FFFFFL};
	int32_t __double_epsilon[]	= {0x34000000L};
	int32_t __double_tiny	[]	= {1L};
	int32_t __double_huge	[]	= {0x7f800000L};
	int32_t __double_nan	[]	= {0x7FFFFFFFL};

	int32_t *__huge_val = __double_huge;
	int32_t *__nan_val  = __double_nan;

#endif /* _EWL_DOUBLE_SIZE  */

#if (_EWL_LONG_DOUBLE_SIZE == 96)

  int32_t __extended_min	[]	= {0x00010000L,(int32_t)0x80000000L, 0L};
  int32_t __extended_max	[]	= {0x7FFE0000L,(int32_t)0xFFFFFFFFL,(int32_t)0xFFFFFFFFL};
  int32_t __extended_epsilon[]	= {0x3FC00000L,(int32_t)0x80000000L,0x00000000L};
  int32_t __extended_tiny   []	= {0L,0L,1L};
  int32_t __extended_huge   []	= {0x7FFF0000L,0L,0L};
  int32_t __extended_nan	[]	= {0x7FFF0000L,(int32_t)0xFFFFFFFFL,(int32_t)0xFFFFFFFFL};

#elif (_EWL_LONG_DOUBLE_SIZE == 80)

  int32_t __extended_min	[]	= {0x00018000L,0L,0L};
  int32_t __extended_max	[]	= {0x7FFEFFFFL,(int32_t)0xFFFFFFFFL,(int32_t)0xFFFF0000L};
  int32_t __extended_epsilon[]	= {0x3FC08000L,0L,0L};
  int32_t __extended_tiny   []	= {0L,0L,0x00010000L};
  int32_t __extended_huge   []	= {0x7FFF0000L,0L,0L};
  int32_t __extended_nan	[]	= {0x7FFFFFFFL,(int32_t)0xFFFFFFFFL,(int32_t)0xFFFF0000L};

#elif (_EWL_LONG_DOUBLE_SIZE == 64) && _EWL_LITTLE_ENDIAN

   int32_t __extended_min    []	= {0L,0x00100000L};
   int32_t __extended_max    []	= {(int32_t)0xffffffffL,0x7fefffffL};
   int32_t __extended_epsilon[]	= {0L,0x3cb00000L};
   int32_t __extended_tiny   []	= {1L,0L};
   int32_t __extended_huge   []	= {0L,0x7FF00000L};
   int32_t __extended_nan    []	= {(int32_t)0xFFFFFFFFL,0x7FFFFFFFL};

#elif (_EWL_LONG_DOUBLE_SIZE == 64) && !_EWL_LITTLE_ENDIAN

   int32_t __extended_min    []	= {0x00100000L,0L};
   int32_t __extended_max    []	= {0x7fefffffL,(int32_t)0xffffffffL};
   int32_t __extended_epsilon[]	= {0x3cb00000L,0L};
   int32_t __extended_tiny   []	= {0L,1L};
   int32_t __extended_huge   []	= {0x7FF00000L,0L};
   int32_t __extended_nan    []	= {0x7FFFFFFFL,(int32_t)0xFFFFFFFFL};

#elif (_EWL_LONG_DOUBLE_SIZE == 32)

	int32_t __extended_min	  []= {0x00800000L};
	int32_t __extended_max	  []= {0x7F7FFFFFL};
	int32_t __extended_epsilon[]= {0x34000000L};
	int32_t __extended_tiny   []= {1L};
	int32_t __extended_huge	  []= {0x7F800000L};
	int32_t __extended_nan    []= {0x7FFFFFFFL};

#endif /* _EWL_LONG_DOUBLE_SIZE  */

#if (_EWL_FLOAT_SIZE == 32)

    int32_t __float_min    []	= {0x00800000L};
    int32_t __float_max    []	= {0x7F7FFFFFL};
    int32_t __float_epsilon[]	= {0x34000000L};

#endif /* _EWL_FLOAT_SIZE  */
#endif /* _EWL_FLOATING_POINT */
