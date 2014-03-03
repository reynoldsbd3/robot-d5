/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:30 $
 * $Revision: 1.1 $
 */

/* Include your OS dependent include files */
#include "file_io.h"

#if !_EWL_OS_DISK_FILE_SUPPORT
#error file_io.stub.c can't be in your project if _EWL_OS_DISK_FILE_SUPPORT is defined to 0.
#endif

typedef struct {
	/* change these members to whatever is appropriate */
	short	refnum;
	void *	spec;
} temp_file_info;

static temp_file_info	temp_info[FOPEN_MAX];

static temp_file_info * find_temp_info(short refnum)
{
	int								i;
	temp_file_info *	p;

	for (i = 0, p = temp_info; i < FOPEN_MAX; i++, p++)
		if (p->refnum == refnum)
			return(p);

	return(0);
}


int	__open_file(const char * name, __file_modes mode, __file_handle * handle)
{
#warning "replace return with an OS call that opens the file"
	return(__no_io_error);
}

int __open_temp_file(__file_handle * handle)
{
	char					temp_name[L_tmpnam];
	temp_file_info *		info;
	void *					spec;
	int						ioresult;

	__temp_file_name(temp_name, &spec);

	if (!(info = find_temp_info(0)))
		return(__io_error);

	ioresult = __open_file(temp_name, __temp_file_mode, handle);

	if (ioresult == __no_io_error)
	{
		info->refnum = *handle;
		info->spec   = spec;
	}

	return(ioresult);
}

#if __dest_os == __starcore
int __read_file(__file_handle handle, unsigned char * buffer, size_t * count, __ref_con ref_con)
#else
int __read_file(__file_handle handle, unsigned char * buffer, size_t * count)
#endif
{
#warning "replace return with an OS call that reads from the file"
	return(__no_io_error);
}

#if __dest_os == __starcore
int __write_file(__file_handle handle, unsigned char * buffer, size_t * count, __ref_con ref_con)
#else
int __write_file(__file_handle handle, unsigned char * buffer, size_t * count)
#endif
{
#warning "replace return with an OS call that writes to the file"
	return(__no_io_error);
}

#if __dest_os == __starcore
int __position_file(__file_handle handle, fpos_t * position, int mode, __ref_con ref_con)
#else
int __position_file(__file_handle handle, fpos_t * position, int mode)
#endif
{
#warning "replace return with an OS call that sets the current position in the file"
	return(__no_io_error);
}

int __close_file(__file_handle handle)
{
#warning "replace return with an OS call that closes the file"
	return(__no_io_error);
}

void __temp_file_name(char * name_str, void * filespec)
{
#warning "copy the temp file's name into name_str and the file specification structure into filespec"
}

int __delete_file(const char * name)
{
#warning "replace return with an OS call that deletes the file"
	return(__no_io_error);
}

int __rename_file(const char * old_name, const char * new_name)
{
#warning "replace return with an OS call that renames the file"
	return(__no_io_error);
}
