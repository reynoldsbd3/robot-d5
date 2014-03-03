/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:46:51 $
 * $Revision: 1.1 $
 */

#include <ansi_parms.h>

#if _EWL_PTHREADS && _EWL_LOCALDATA_AVAILABLE

#include <stdlib.h>
#include <locale.h>
#include <critical_regions.h>
#include <ewl_thread_local_data.h>
#include <lconv.h>
#include <string.h>



static pthread_key_t _ewl_local_data_key = 0;

/* This function must be called before _InitializeThreadData. */
/* Returns false on error, true on success. */
static int _InitializeThreadDataIndex(void)
{
	if (_ewl_local_data_key != 0)
	{
		/* _ewl_local_data_key is already initialized */
		return 1;
	}

	/* Call free() on the thread data when a thread terminates and it has local data associated with it */
	if (pthread_key_create(&_ewl_local_data_key, &free) != 0)
	{
		/* The allocation failed. */
		return 0;
	}

	return 1;
}

/* This function sets up a structure of thread local data needed by various EWL functions. */
/* Returns false on error, true on success. */
static int _InitializeThreadData(void)
{
	_ThreadLocalData *tld;

	tld = (_ThreadLocalData *) pthread_getspecific(_ewl_local_data_key);

	if (tld == NULL)
	{
		tld = (_ThreadLocalData *) malloc(sizeof(_ThreadLocalData));

		if (tld == NULL)
			return 0;

		/* All initialization of items in the _ThreadLocalData structure goes here... */

		tld->_errno = 0;
		tld->random_next = 1;
		tld->strtok_dn = (unsigned char *) "";
		tld->strtok_ds = (unsigned char *) "";
		tld->locale_name = "C";
		tld->localtime_tm.tm_sec   =  0;
		tld->localtime_tm.tm_min   =  0;
		tld->localtime_tm.tm_hour  =  0;
		tld->localtime_tm.tm_mday  =  1;
		tld->localtime_tm.tm_mon   =  0;
		tld->localtime_tm.tm_year  =  0;
		tld->localtime_tm.tm_wday  =  1;
		tld->localtime_tm.tm_yday  =  0;
		tld->localtime_tm.tm_isdst = -1;

		tld->gmtime_tm.tm_sec   =  0;
		tld->gmtime_tm.tm_min   =  0;
		tld->gmtime_tm.tm_hour  =  0;
		tld->gmtime_tm.tm_mday  =  1;
		tld->gmtime_tm.tm_mon   =  0;
		tld->gmtime_tm.tm_year  =  0;
		tld->gmtime_tm.tm_wday  =  1;
		tld->gmtime_tm.tm_yday  =  0;
		tld->gmtime_tm.tm_isdst = -1;

		tld->_current_locale.next_locale = NULL;
		strcpy(tld->_current_locale.locale_name, _current_locale.locale_name);
		tld->_current_locale.coll_cmpt_ptr = _current_locale.coll_cmpt_ptr;
		tld->_current_locale.ctype_cmpt_ptr = _current_locale.ctype_cmpt_ptr;
		tld->_current_locale.mon_cmpt_ptr = _current_locale.mon_cmpt_ptr;
		tld->_current_locale.num_cmpt_ptr = _current_locale.num_cmpt_ptr;
		tld->_current_locale.time_cmpt_ptr = _current_locale.time_cmpt_ptr;
		memcpy((void*) &tld->_current_locale.next_locale, (void*) &_current_locale.next_locale,
			sizeof(struct __locale));

		tld->__lconv = __lconv;

		#if ((__dest_os == __mac_os) || (__dest_os == __mac_os_x))
			tld->MacOSErrNo = 0;
		#endif

		return (pthread_setspecific(_ewl_local_data_key, tld) == 0);
	}

	return 1;	/* data is already setup, just return success */
}

int _EWL_CDECL __ewl_InitializeMainThreadData(void)
{
    /* Check to see if data was already initialized */
    if (_InitializeThreadDataIndex())
    {
        /* If it wasn't initialized, setup the data for the main thread */
        if (_InitializeThreadData())
        {
            return 1;
        }
    }

    return 0;
}

void _EWL_CDECL __ewl_DisposeAllThreadData(void)
{
	if (_ewl_local_data_key != 0)
	{
		pthread_key_delete(_ewl_local_data_key);
		_ewl_local_data_key = 0;
	}
}

_ThreadLocalData * _EWL_CDECL __ewl_GetThreadLocalData(void)
{
	_ThreadLocalData *tld;

	tld = (_ThreadLocalData *) pthread_getspecific(_ewl_local_data_key);

	#if (__dest_os == __mac_os_x)

	/* initialization of locale items in the _ThreadLocalData structure */

		tld->locale_name = "C";

		tld->_current_locale.next_locale = NULL;
		strcpy(tld->_current_locale.locale_name, _current_locale.locale_name);
		tld->_current_locale.coll_cmpt_ptr = _current_locale.coll_cmpt_ptr;
		tld->_current_locale.ctype_cmpt_ptr = _current_locale.ctype_cmpt_ptr;
		tld->_current_locale.mon_cmpt_ptr = _current_locale.mon_cmpt_ptr;
		tld->_current_locale.num_cmpt_ptr = _current_locale.num_cmpt_ptr;
		tld->_current_locale.time_cmpt_ptr = _current_locale.time_cmpt_ptr;
		memcpy((void*) &tld->_current_locale.next_locale, (void*) &_current_locale.next_locale,
			sizeof(struct __locale));

		tld->__lconv = __lconv;

	#endif /* mas_os */

	if (tld == NULL)
	{
		if (!__ewl_InitializeMainThreadData())
			exit(127);
		else
		{
			tld = (_ThreadLocalData *) pthread_getspecific(_ewl_local_data_key);

			if (tld == NULL)
				exit(127);
		}
	}

	return tld;
}

#endif /* _EWL_PTHREADS && _EWL_LOCALDATA_AVAILABLE */

