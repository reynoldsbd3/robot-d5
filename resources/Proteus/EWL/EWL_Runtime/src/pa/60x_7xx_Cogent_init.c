/* CodeWarrior PowerPC EABI Runtime Support Library 
 * Copyright © 1995-2006 Freescale Semiconductor, Inc.  All rights reserved.
 *
 * $Date: 2012/06/01 16:06:51 $
 * $Revision: 1.1 $
 */
//////////////////////////////////////////////////////////////////////////////
//
// Cogent PPC board-specific initialization code. 
//
//
//////////////////////////////////////////////////////////////////////////////

#pragma section all_types ".init" ".init"	

//////////////////////////////////////////////////////////////////////
//
//	init_board
//
//	Performs board-specific initializations.
//
//	For the Cogent PPC board, none are required, so this is just a
//	stub.
//
//////////////////////////////////////////////////////////////////////

asm void init_board()
{
	nofralloc
	
	blr
}