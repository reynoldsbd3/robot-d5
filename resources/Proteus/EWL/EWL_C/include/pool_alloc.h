/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

#include <ansi_parms.h>

#if _EWL_CLASSIC_MALLOC

	#ifndef _EWL_POOL_ALLOC_H
	#define _EWL_POOL_ALLOC_H

	#include <ansi_parms.h>

	/* sanity check */
	#if _EWL_BAREBOARD_MALLOC || _EMBEDDED_WARRIOR_MALLOC
	#error Wrong memory configuration
	#endif

	#pragma options align=native

	typedef signed long tag_word;

	typedef struct block_header {
		tag_word				tag;
		struct block_header *	prev;
		struct block_header *	next;
	} block_header;

	typedef struct list_header {
		block_header *		rover;
		block_header		header;
	} list_header;

	typedef struct heap_header {
		struct heap_header* 	prev;
		struct heap_header*		next;
	} heap_header;

	typedef unsigned long mem_size;

	struct mem_pool_obj;
	typedef void *	(*sys_alloc_ptr)(mem_size  , struct mem_pool_obj*);
	typedef void	(*sys_free_ptr)(void *   , struct mem_pool_obj*);

	typedef struct pool_options{
		sys_alloc_ptr	sys_alloc_func;
		sys_free_ptr	sys_free_func;
		mem_size		min_heap_size;
	#ifndef __ALTIVEC__
		int				always_search_first;
	#else
		char			always_search_first;			/* for ALTIVEC */
		char			block_alignment;
		char			reserved1;
		char			reserved2;
	#endif /* __ALTIVEC__ */
	} pool_options;

	typedef struct mem_pool_obj {
		list_header		free_list;
	#if _EWL_OS_ALLOC_HEAP_SUPPORT
		pool_options	options;
		heap_header*	heap_list;
		void*			userData;
	#endif /* _EWL_OS_ALLOC_HEAP_SUPPORT */
	} mem_pool_obj;

	#define __heap_version	1

	_EWL_BEGIN_EXTERN_C

	_EWL_IMP_EXP_C extern mem_pool_obj __malloc_pool;

	#ifdef __ALTIVEC__
		void _EWL_CDECL	__init_align_pool_obj(mem_pool_obj * pool_obj, char align) _EWL_CANT_THROW;
	#endif /* __ALTIVEC__ */

	void   _EWL_CDECL __init_pool_obj(mem_pool_obj * pool_obj) _EWL_CANT_THROW;
	int	   _EWL_CDECL __pool_preallocate(mem_pool_obj * pool_obj, mem_size  ) _EWL_CANT_THROW;
	void   _EWL_CDECL __pool_preassign(mem_pool_obj * pool_obj, void *   , mem_size  ) _EWL_CANT_THROW;
	void * _EWL_CDECL __pool_alloc(mem_pool_obj * pool_obj, mem_size  ) _EWL_CANT_THROW;
	void * _EWL_CDECL __pool_alloc_clear(mem_pool_obj * pool_obj, mem_size  ) _EWL_CANT_THROW;
	void * _EWL_CDECL __pool_realloc(mem_pool_obj * pool_obj, void *   ,	mem_size  ) _EWL_CANT_THROW;
	void   _EWL_CDECL __pool_free(mem_pool_obj * pool_obj, void *   ) _EWL_CANT_THROW;

	_EWL_IMP_EXP_C void	_EWL_CDECL __pool_free_all(mem_pool_obj * pool_obj) _EWL_CANT_THROW;

	void * _EWL_CDECL __sys_alloc(mem_size  , mem_pool_obj*) _EWL_CANT_THROW;
	void   _EWL_CDECL __sys_free(void *  , mem_pool_obj*) _EWL_CANT_THROW;

	#if !_EWL_OS_ALLOC_HEAP_SUPPORT
		_EWL_IMP_EXP_C int _EWL_CDECL init_alloc(void * , mem_size ) _EWL_CANT_THROW;
	#endif /* !_EWL_OS_ALLOC_HEAP_SUPPORT */

	_EWL_END_EXTERN_C

	#pragma options align=reset

	#endif /* _EWL_POOL_ALLOC_H */

#else  /* _EWL_CLASSIC_MALLOC */

	#ifndef _EWL_POOL_ALLOC_H
	#define _EWL_POOL_ALLOC_H

	#include <ansi_parms.h>
	#include <size_t.h>

	/* sanity check */
	#if _EWL_BAREBOARD_MALLOC || _EMBEDDED_WARRIOR_MALLOC
	#error Wrong memory configuration
	#endif

	#pragma options align=native

	#define __heap_version	2

	_EWL_BEGIN_EXTERN_C

	void * _EWL_CDECL			__sys_alloc(__std(size_t) sz);
	void   _EWL_CDECL			__sys_free(void *ptr);

	__std(size_t) _EWL_CDECL	__sys_pointer_size(void *ptr);

	typedef struct __mem_pool
	{
		void* reserved[14];
	} __mem_pool;

	_EWL_IMP_EXP_C void  _EWL_CDECL __init_pool_obj(__mem_pool* pool_obj) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C void* _EWL_CDECL __pool_alloc(__mem_pool* pool_obj, __std(size_t) size) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C void  _EWL_CDECL __pool_free(__mem_pool* pool_obj, void* ptr) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C void* _EWL_CDECL __pool_realloc(__mem_pool* pool_obj, void* ptr, __std(size_t) size) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C void* _EWL_CDECL __pool_alloc_clear(__mem_pool* pool_obj, __std(size_t) size) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C void	 _EWL_CDECL __pool_free_all(__mem_pool* pool) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C void	 _EWL_CDECL __malloc_free_all(void) _EWL_CANT_THROW;

	#if !_EWL_OS_ALLOC_HEAP_SUPPORT
		_EWL_IMP_EXP_C int _EWL_CDECL init_alloc(void * , __std(size_t) heap_size) _EWL_CANT_THROW;
	#endif /* !_EWL_OS_ALLOC_HEAP_SUPPORT */

	_EWL_END_EXTERN_C

	#pragma options align=reset

	#endif /* _EWL_POOL_ALLOC_H */

#endif /* _EWL_CLASSIC_MALLOC */
