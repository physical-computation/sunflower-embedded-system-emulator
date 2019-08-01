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

#include "sf.h"
#include "mextern.h"


int
msp430step(Engine *E, State *S, int drain_pipe)
{
	/*								*/
	/*	If ~RST/NMI pin is configured for reset, CPU stays	*/
	/*	in reset state as long as pin stays low. When pin	*/
	/*	goes high, PC goes to 0xFFFE				*/
	/*								*/	

	return -1;
}

int
msp430faststep(Engine *E, State *S, int drain_pipe)
{
	return -1;
}

void
msp430flushpipe(State *S)
{
	/*								*/
	/*	Flush pipeline, count # bits we clear in pipe regs	*/
	/*								*/
	S->msp430->P.IF.cycles = 0;
	S->msp430->P.IF.valid = 0;

	S->msp430->P.ID.cycles = 0;
	S->msp430->P.ID.valid = 0;

	S->msp430->P.EX.cycles = 0;
	S->msp430->P.EX.valid = 0;

	if (SF_BITFLIP_ANALYSIS)
	{
		S->Cycletrans += bit_flips_32(S->superH->P.IF.instr, 0);
		S->Cycletrans += bit_flips_32(S->superH->P.ID.instr, 0);
		S->Cycletrans += bit_flips_32(S->superH->P.EX.instr, 0);
	}

	return;
}

void
msp430dumppipe(Engine *E, State *S)
{
	mprint(E, S, nodeinfo, "\nnode ID=%d, PC=0x" UHLONGFMT ", ICLK=" UVLONGFMT ", sleep?=%d\n",
		S->NODE_ID, S->PC, S->ICLK, S->sleep);


	if (S->msp430->P.EX.valid)
	{
		mprint(E, S, nodeinfo, "EX: [%s],%d\n",
			msp430_opstrs[S->msp430->P.EX.op], S->msp430->P.EX.cycles);
	}
	else
	{
		mprint(E, S, nodeinfo, "EX: []\n");
	}

	if (S->msp430->P.ID.valid)
	{
		mprint(E, S, nodeinfo, "ID: [0x%x],%d\n",
			S->msp430->P.ID.instr, S->msp430->P.ID.cycles);
	}
	else
	{
		mprint(E, S, nodeinfo, "ID: []\n");
	}

	if (S->msp430->P.IF.valid)
	{
		mprint(E, S, nodeinfo, "IF: [0x%x],%d\n\n",
			S->msp430->P.IF.instr, S->msp430->P.IF.cycles);
	}
	else
	{
		mprint(E, S, nodeinfo, "IF: []\n\n");
	}

	return;
}
