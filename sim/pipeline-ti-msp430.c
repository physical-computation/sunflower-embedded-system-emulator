/*										*/
/*		Copyright (C) 1999-2006 Phillip Stanley-Marbell.		*/
/*										*/
/*	You may redistribute and/or copy the source and binary			*/
/*	versions of this software provided:					*/
/*										*/
/*	1.	The source retains the copyright notice above, 			*/
/*		this list of conditions, and the following 			*/
/*		disclaimer.							*/
/*										*/
/*	2.	Redistributed binaries must reproduce the above			*/
/*		copyright notice, this list of conditions, and			*/
/*		the following disclaimer.					*/
/*										*/
/*	3.	The source and binaries are used for non-commercial 		*/
/*		purposes only.							*/
/*										*/
/*	4.	Permission is granted to all others, to redistribute		*/
/*		or make copies of the source or binaries, either		*/
/*		as it was received or with modifications, provided		*/
/*		the above three conditions are enforced and 			*/
/*		preserved, and that permission is granted for 			*/
/*		further redistribution, under the conditions of this		*/
/*		document.							*/
/*										*/
/*	The source is provided "as is", and no warranty is either		*/
/*	implied or explicitly granted. The authors will not be liable 		*/
/*	for any damages arising in any way out of the use or misuse 		*/
/*	of this software, even if advised of the possibility of such 		*/
/*	damage.									*/
/*										*/
/*	Contact: phillip Stanley-Marbell <pstanley@ece.cmu.edu>			*/
/*										*/	

#include "sf.h"
#include "mextern.h"


int
msp430step(State *S)
{
	/*								*/
	/*	If ~RST/NMI pin is configured for reset, CPU stays	*/
	/*	in reset state as long as pin stays low. When pin	*/
	/*	goes high, PC goes to 0xFFFE				*/
	/*								*/	

	return -1;
}

int
msp430faststep(State *S)
{
	return -1;
}

void
msp430pipeflush(State *S)
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
msp430dumppipe(State *S)
{
	mprint(S, nodeinfo, "\nnode ID=%d, PC=0x" UHLONGFMT ", ICLK=" UVLONGFMT ", sleep?=%d\n",
		S->NODE_ID, S->PC, S->ICLK, S->sleep);


	if (S->msp430->P.EX.valid)
	{
		mprint(S, nodeinfo, "EX: [%s],%d\n",
			opstrs[S->msp430->P.EX.op], S->msp430->P.EX.cycles);
	}
	else
	{
		mprint(S, nodeinfo, "EX: []\n");
	}

	if (S->msp430->P.ID.valid)
	{
		mprint(S, nodeinfo, "ID: [0x%x],%d\n",
			S->msp430->P.ID.instr, S->msp430->P.ID.cycles);
	}
	else
	{
		mprint(S, nodeinfo, "ID: []\n");
	}

	if (S->msp430->P.IF.valid)
	{
		mprint(S, nodeinfo, "IF: [0x%x],%d\n\n",
			S->msp430->P.IF.instr, S->msp430->P.IF.cycles);
	}
	else
	{
		mprint(S, nodeinfo, "IF: []\n\n");
	}

	return;
}
