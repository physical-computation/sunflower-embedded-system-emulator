/*
	Copyright (c) 1999-2008, Phillip Stanley-Marbell (author)
 
	All rights reserved.

	Redistribution and use in source and binary forms, with or without 
	modification, are permitted provided that the following conditions
	are met:

	*	Redistributions of source code must retain the above
		copyright notice, this list of conditions and the following
		disclaimer.

	*	Redistributions in binary form must reproduce the above
		copyright notice, this list of conditions and the following
		disclaimer in the documentation and/or other materials
		provided with the distribution.

	*	Neither the name of the author nor the names of its
		contributors may be used to endorse or promote products
		derived from this software without specific prior written 
		permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
	FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
	COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
	ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
	POSSIBILITY OF SUCH DAMAGE.
*/






	/*								*/
	/*	!!!!!! NOTE:  This implementation is out of sync 	*/
	/*	with changes made over the last few years to the	*/
	/*	simulator.						*/
	/*								*/








/*									*/
/*			The Power Adaptation Unit			*/
/*									*/
/*	This is a small structure in the CPU that monitors program	*/
/*	behavior and adjusts the CPU operating frequency and voltage	*/
/*	to minimize average power, hopefully without degrading 		*/
/*	performance.							*/
/*									*/
typedef struct
{
	int	valid;
	int	transient;
	int	active;
	int	init;
	int	device;

	ulong	trigger_pc;
	ulong	stride;

	int	Q;

	/*	dynamic instr cnt for period of stride		*/
	ulong	instrs;
	ulong	dyntmp;

	/*							*/
	/*	Only nclks needs to be uvlong. Others are 	*/
	/*	limited to PAU_STRIDE_MAX			*/
	/*							*/
	uvlong	nclks;
} PAUentry;

enum
{
	PAU_INIT_ADDR_MASK	= B0000001111111111,	/*	Lower 10 bits of address for indexing PAU	*/
	PAU_INIT_NENTRIES	= PAU_INIT_ADDR_MASK+1,	/*	Mask is (2^N)-1, and we are 2^N entries		*/

	PAU_Q_LOW_H2O	= 1,
	PAU_Q_HIGH_H2O	= 2,
	PAU_STRIDE_MAX	= 1 << 24,
	PAU_RFG_CYCLES	= 1024,
	PAU_STRIDE_MIN	= (2)*(1)*(PAU_RFG_CYCLES),
};

enum
{
	PAU_LOG_STEP = (1 << (sizeof(uchar)))/2,
};
