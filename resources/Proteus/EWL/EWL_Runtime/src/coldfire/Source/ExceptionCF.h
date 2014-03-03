/* Metrowerks Runtime Library
 * Copyright � 1993-2005 Metrowerks Corporation.  All rights reserved.
 *	Copyright � 2005 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * $Date: 2012/06/28 06:43:47 $
 * $Revision: 1.2 $
 */

#ifndef _EXCEPT68K_H
#define _EXCEPT68K_H

#include <ansi_parms.h>           /* hh 971207 Added <ansi_parms.h> header */

#pragma options align=mac68k

#define SEGMAPNAME			__segtable__
#define SEGMAPNAMESTR		"__segtable__"

//	non-volatile register restore bits

#define EX68K_DIRECTSTOREOFFSET	0x80000000L		//	direct store offset bit

#define EX68K_DIRECTSTORE		0x8000			//	record is stored in table
#define EX68K_RESERVED			0x4000			//	reserved for future extensions
#define EX68K_REGMASK			(EX68K_DATAREGMASK|EX68K_ADDRREGMASK|EX68K_FLOATREGMASK)

#define EX68K_DATAREGMASK		0x001F			//	data register mask
#define EX68K_DATAREGFIRSTBITM	0x0001			//	first bit in mask
#define EX68K_DATAREGFIRSTBIT	0				//	first bit in mask
#define EX68K_DATAREGFIRSTREG	3				//	first bit register D3
#define EX68K_DATAREGS			5				//	number of non-volatile registers D3-D7

#define EX68K_ADDRREGMASK		0x03E0			//	address register mask
#define EX68K_ADDRREGFIRSTBITM	0x0020			//	first bit in mask
#define EX68K_ADDRREGFIRSTBIT	5				//	first bit in mask
#define EX68K_ADDRREGFIRSTREG	2				//	first bit register A2
#define EX68K_ADDRREGS			5				//	number of non-volatile registers A2-A6

#if __COLDFIRE__

#define EX68K_FLOATREGMASK		0xFC00			//	address register mask
#define EX68K_FLOATREGFIRSTBITM	0x0400			//	first bit in mask
#define EX68K_FLOATREGFIRSTBIT	10				//	first bit in mask
#define EX68K_FLOATREGFIRSTREG	2				//	first bit register FP2
#define EX68K_FLOATREGS			6				//	number of non-volatile registers FP2-FP7

#else

#define EX68K_FLOATREGMASK		0x3C00			//	address register mask
#define EX68K_FLOATREGFIRSTBITM	0x0400			//	first bit in mask
#define EX68K_FLOATREGFIRSTBIT	10				//	first bit in mask
#define EX68K_FLOATREGFIRSTREG	4				//	first bit register FP4
#define EX68K_FLOATREGS			4				//	number of non-volatile registers FP4-FP7

#endif

//	This data structure will map a return address to an action list
//typedef struct RETURN_ACTION_OFFSET {
//	unsigned char|short|long	return_offset;	//	offset of return address in function (always > 0)
//	unsigned short				action_offset;	//	offset of action list in function exception record (shifted right by one bit)
//}	RETURN_ACTION_OFFSET;
//	Note: the return_offsets will be relative to the previous return_offset:
//	0XXX XXXX									: offsets 0...2^07-1 (127)
//	10XX XXXX XXXX XXXX							: offsets 0...2^14-1 (16383)
//	11XX XXXX XXXX XXXX	XXXX XXXX  XXXX XXXX	: offsets 0...2^30-1 (1073741823)

typedef struct ExceptionRecord {
	unsigned short			register_mask;		//	register restore mask
//	unsigned char|short|lng localvarsize;		//	local variable buffer size/2
//	RETURN_ACTION_OFFSET	raoffsets[]			//	variable length RETURN_ACTION_OFFSET list
//	char					0x00;				//	0x00 terminates the RETURN_ACTION_OFFSET list
//	char					pad[];				//	pad byte
//	long					actions[];			//	variable length list of actions
}	ExceptionRecord;

//	action table definitions:

typedef unsigned short exaction_type;

#define EXLREFREGMASK					0xFFFFFF00	//	local register ref mask
#define EXLREFREGBITS					0x80000000	//	local register bits
//	Note:	D0...D7 = EXLREFREGBITS+<x>
//			A0...A7 = EXLREFREGBITS+8+<x>

#define EXACTION_ENDBIT					0x8000	//	action table end bit
#define EXACTION_MASK					0x00FF	//	action mask

#define EXACTION_ENDOFLIST				0x0000	//	end of action list
#define EXACTION_ERROR					0x00FF	//	error

#define EXACTION_DESTROYLOCAL			0x0001	//	destroy local 
typedef struct ex_destroylocal {
	exaction_type	action;						//	EXACTION_DESTROYLOCAL
	long			local;						//	reference to local variable (frame offset)
	void			*dtor;						//	pointer to destructor function
}	ex_destroylocal;

#define EXACTION_DESTROYLOCALCOND		0x0002	//	destroy local 
typedef struct ex_destroylocalcond {
	exaction_type	action;						//	EXACTION_DESTROYLOCALCOND
	long			local;						//	reference to local variable (frame offset)
	long			cond;						//	reference to conditional variable (frame offset or register)
	void			*dtor;						//	pointer to destructor function
}	ex_destroylocalcond;

//#define EXACTION_DESTROYLOCALOFFSET	0x0003	//	destroy local 
//typedef struct ex_destroylocaloffset {
//	exaction_type	action;						//	EXACTION_DESTROYLOCALOFFSET
//	long			local;						//	reference to local variable
//	void			*dtor;						//	pointer to destructor function
//	long			offset;						//	offset
//}	ex_destroylocaloffset;

#define EXACTION_DESTROYLOCALPOINTER	0x0004	//	destroy local 
typedef struct ex_destroylocalpointer {
	exaction_type	action;						//	EXACTION_DESTROYLOCALPOINTER
	long			pointer;					//	reference to local pointer
	void			*dtor;						//	pointer to destructor function
}	ex_destroylocalpointer;

#define EXACTION_DESTROYLOCALARRAY		0x0005	//	destroy local 
typedef struct ex_destroylocalarray {
	exaction_type	action;						//	EXACTION_DESTROYLOCALPOINTER
	long			localarray;					//	reference to local pointer
	void			*dtor;						//	pointer to destructor function
	long			elements;					//	number of array elements
	long			element_size;				//	size of one array element
}	ex_destroylocalarray;

#define EXACTION_DESTROYPARTIALARRAY	0x0006	//	destroy local 
typedef struct ex_destroypartialarray {
	exaction_type	action;						//	EXACTION_DESTROYPARTIALARRAY
	long			arraypointer;				//	reference to array pointer
	long			arraycounter;				//	reference to array counter
	long			dtor;						//	reference to destructor function
	long			element_size;				//	reference to size object
}	ex_destroypartialarray;

#define EXACTION_DESTROYBASE			0x000C	//	destroy base
#define EXACTION_DESTROYMEMBER			0x0007	//	destroy member
typedef struct ex_destroymember {
	exaction_type	action;						//	EXACTION_DESTROYMEMBER |�EXACTION_DESTROYBASE
	long			objectptr;					//	pointer to complete object pointer
	void			*dtor;						//	pointer to destructor function
	long			offset;						//	offset of member in complete object
}	ex_destroymember;

#define EXACTION_DESTROYMEMBERCOND		0x0008	//	destroy member
typedef struct ex_destroymembercond {
	exaction_type	action;						//	EXACTION_DESTROYMEMBERCOND
	long			objectptr;					//	pointer to complete object pointer
	long			cond;						//	pointer to condition flag object
	void			*dtor;						//	pointer to destructor function
	long			offset;						//	offset of member in complete object
}	ex_destroymembercond;

#define EXACTION_DESTROYMEMBERARRAY		0x0009	//	destroy member array
typedef struct ex_destroymemberarray {
	exaction_type	action;						//	EXACTION_DESTROYMEMBERARRAY
	long			objectptr;					//	pointer to complete object pointer
	void			*dtor;						//	pointer to destructor function
	long			offset;						//	offset of member in complete object
	long			elements;					//	number of array elements
	long			element_size;				//	size of one array element
}	ex_destroymemberarray;

#define EXACTION_DELETEPOINTER			0x000A	//	delete pointer
typedef struct ex_deletepointer {
	exaction_type	action;						//	EXACTION_DELETEPOINTER
	long			pointerobject;				//	pointer object reference
	void			*deletefunc;				//	pointer to delete function
}	ex_deletepointer;

#define EXACTION_DELETEPOINTERCOND		0x000B	//	delete pointer
typedef struct ex_deletepointercond {
	exaction_type	action;						//	EXACTION_DELETEPOINTERCOND
	long			pointerobject;				//	pointer object reference
	void			*deletefunc;				//	pointer to delete function
	long			cond;						//	reference to conditional variable
}	ex_deletepointercond;

#define EXACTION_CATCHBLOCK				0x0010	//	catch block
typedef struct ex_catchblock {
	exaction_type	action;						//	EXACTION_CATCHBLOCK
	char			*catch_type;				//	pointer to catch type info (0L: catch(...))
	long			catch_pcoffset;				//	function relative catch label offset
	long			cinfo_ref;					//	reference to local CatchInfo struct
}	ex_catchblock;

#define EXACTION_ACTIVECATCHBLOCK		0x0011	//	active catch block
typedef struct ex_activecatchblock {
	exaction_type	action;						//	EXACTION_ACTIVECATCHBLOCK
	long			cinfo_ref;					//	reference to local CatchInfo struct
}	ex_activecatchblock;

#define EXACTION_TERMINATE				0x0012	//	active catch block
typedef struct ex_terminate {
	exaction_type	action;						//	EXACTION_TERMINATE
}	ex_terminate;

#define EXACTION_SPECIFICATION			0x0013	//	active catch block
typedef struct ex_specification {
	exaction_type	action;						//	EXACTION_SPECIFICATION
	unsigned short	specs;						//	number of specializations (0-n)
	long			pcoffset;					//	function relative offset
	long			cinfo_ref;					//	reference to local CatchInfo struct
	char			*spec[];					//	array of catch type pointers
}	ex_specification;

#define EXACTION_DESTROYVLA	            0x0014 	//	destroy local 
typedef struct ex_destroypvla {
	exaction_type	action;						//	EXACTION_DESTROYVLA
	long			arraypointer;				//	reference to array pointer
	long			arraysize;  				//	reference to array size
	long			dtor;						//	reference to destructor function
	long			element_size;				//	size object
}	ex_destroyvla;

//	code -> action mapping tables

typedef struct function_map {
	long	function_offset_start;			//	offset of function in segment
//	long	function_offset_end;			//	size of function
	long	actionoffset;					//	offset of action table entry (0L: no entry)
}	function_map;

typedef struct segment_map {
	char	*codeptr;						//	XVector of code.
	long	codesize;						//	size of code segment (0L: end of segment list)
	long	functionmapoffset;				//	pointer to function map
	long	functionmapsize;				//	size of function map 
}	segment_map;

#pragma options align=reset

#ifdef __CFM68K__
#ifdef __cplusplus
extern "C" {
#endif

#pragma import on
_EWL_IMP_EXP_RUNTIME extern long		__register_fragment(segment_map *fragExceptionTable, char *TOC);
_EWL_IMP_EXP_RUNTIME extern void		__unregister_fragment(long fragmentID);
#pragma import reset

#ifdef __cplusplus
}
#endif
#endif

#ifdef _EWL_USING_NAMESPACE       /* hh 971207 Added namespace support */

extern "C" void exit(int status);

#endif

#endif

/*
	Change Record
 * hh 971207 Added <ansi_parms.h> header
 * hh 971207 Added namespace support
*/
