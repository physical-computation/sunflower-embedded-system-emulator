/*
	Copyright (c) 2019, Hein Alexander Mante (author)

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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "sf.h"
#include "mextern.h"



TaintOriginNode * taintOriginHead = NULL;
bool RegMarked = false;
bool fRegMarked = false;



/*
*	Implementation of the linked list for storing TaintOriginNode objects:
*/


void
printList(Engine *E, State *S)
{
	TaintOriginNode * ptr = taintOriginHead;
	mprint(E,S,nodeinfo,"\n[ ");

	while (ptr != NULL)
	{
		mprint(E,S,nodeinfo,"%llu,%u,%u,%llu\n",ptr->taintAddress,ptr->taintstartPC,ptr->taintendPC,ptr->taintCol);
		ptr = ptr->next;
	}
	
	mprint(E,S,nodeinfo," ]");
}

bool
isEmpty(void)
{
	return taintOriginHead == NULL;
}

int
length(void)
{
	int len = 0;
	TaintOriginNode * current;
	for (current = taintOriginHead; current != NULL; current = current -> next)
	{
		len++;
	}
	return len;
}

bool
contains (uint64_t searchAddr, uint32_t taintPC, SunflowerTaintMemType memType)
{
	TaintOriginNode * current = taintOriginHead;
	
	if (taintOriginHead == NULL)
	{
		return false;
	}

	while (current->taintAddress != searchAddr
		|| current->taintstartPC > taintPC
		|| current->taintendPC < taintPC
		|| current->memType != memType)
	{
		if (current->next == NULL)
		{
			return false;
		}
		else
		{
			current = current -> next;
		}
	}
	return true;
}

bool
containsExactPC (uint64_t searchAddr, uint32_t taintstartPC, uint32_t taintendPC, SunflowerTaintMemType memType)
{
	TaintOriginNode * current = taintOriginHead;
	
	if (taintOriginHead == NULL)
	{
		return false;
	}

	while (current->taintAddress != searchAddr
		|| current->taintstartPC != taintstartPC
		|| current->taintendPC != taintendPC
		|| current->memType != memType)
	{
		if (current->next == NULL)
		{
			return false;
		}
		else
		{
			current = current -> next;
		}
	}
	return true;
}

/*

TaintOriginNode *
findByAddr (Engine *E, State *S, uint64_t searchAddr)
{
	TaintOriginNode * current = taintOriginHead;
	
	if (taintOriginHead == NULL)
	{
		mprint(E,S,nodeinfo,"Couldn't find taintOriginHead in findByAddr function");
		return NULL;
	}

	while (current->taintAddress != searchAddr)
	{
		if (current->next == NULL)
		{
			mprint(E,S,nodeinfo,"Couldn't find taintOriginHead in findByAddr function");
			return NULL;
		}
		else
		{
			current = current -> next;
		}
	}
	return current;
}

*/

uint64_t
findByAll (Engine *E, State *S, uint64_t searchAddr, uint32_t taintPC, SunflowerTaintMemType memType)
{
	TaintOriginNode * current = taintOriginHead;
	uint64_t outCol = 0;
	
	if (!contains(searchAddr,taintPC,memType))
	{
		mprint(E,S,nodeinfo,"findByAll function is looking for a taint origin which doesn't exist according to the contains function!");
		return 0;
	}

	while (current != NULL)
	{
		if (current->taintAddress == searchAddr
			&& current->taintstartPC <= taintPC
			&& current->taintendPC >= taintPC
			&& current->memType == memType)
		{
			outCol = outCol | current->taintCol;
		}

		current = current -> next;
	}
	if (outCol == 0)
	{
		mprint(E,S,nodeinfo,"Problem in findByAll function: the contains function claims a taint origin as below exists, but find cannot find it (or the taint colour thereof is 0 which would be silly):\nAddress: %llu\nPC range contains: %u\nmemType: %d\n",searchAddr,taintPC,(int)memType);
		return 0;
	}
	return outCol;
}

void
insertFirst (uint64_t addr, uint32_t taintstartPC, uint32_t taintendPC, uint64_t taintCol, SunflowerTaintMemType memType)
{
	TaintOriginNode * link = (TaintOriginNode *) calloc(1,sizeof(TaintOriginNode));
	
	link->taintAddress	= addr;
	link->taintstartPC	= taintstartPC;
	link->taintendPC	= taintendPC;
	link->taintCol		= taintCol;
	link->memType		= memType;
	link->next		= taintOriginHead;

	taintOriginHead		= link;
}

TaintOriginNode *
deleteFirst(void)
{
	TaintOriginNode * tmplink	= taintOriginHead;
	taintOriginHead			= taintOriginHead -> next;
	
	return tmplink;
}

TaintOriginNode *
delete(Engine *E, State *S, uint64_t addr, uint32_t taintstartPC, uint32_t taintendPC, SunflowerTaintMemType memType)
{
	TaintOriginNode * current 	= taintOriginHead;
	TaintOriginNode * prev		= NULL;

	if (taintOriginHead == NULL)
	{
		mprint(E,S,nodeinfo,"Tried to delete from an empty list in delete()");
		return NULL;
	}

	while  (current->taintAddress != addr
		|| current->taintstartPC != taintstartPC
		|| current->taintendPC != taintendPC
		|| current->memType != memType)
	{
		if (current->next == NULL)
		{
			mprint(E,S,nodeinfo,"Tried to delete non-existant entry from list in delete()");
			return NULL;
		}
		else
		{
			prev	= current;
			current = current->next;
		}
	}
	if (current == taintOriginHead)
	{
		taintOriginHead = taintOriginHead->next;
	}
	else
	{
		prev->next = current -> next;
	}
	return current;
}



/*
*	End of implementation of linked list of TaintOriginNode
*/


void
taintprop(Engine *E, State *S,
	uint64_t immtaint1, uint64_t immtaint2,
	uint64_t AddrOut, SunflowerTaintMemType memType)
{
	uint64_t outCol;

	outCol = immtaint1 | immtaint2 | S->riscv->taintR[32].taintCol;

	/*
	*	Last OR represents PC taint which should be propagated on every step
	*/

	if (S->riscv->P.EX.op != 39) 
	{
		/*
		*	Ecall is the 40th (indexing from 1, so 39 above) instruction
		*	which I don't know what to do with, hence its exclusion
		*/
		switch (S->riscv->P.ID.op)
		{
			case 2:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				S->riscv->instruction_taintDistribution[S->riscv->P.ID.op].taintCol =
						S->riscv->instruction_taintDistribution[S->riscv->P.ID.op].taintCol
						| immtaint1 | immtaint2 | S->riscv->taintR[32].taintCol;
				break;
			default:
				break;
		}
		switch (S->riscv->P.EX.op)
		{
			case 0:
			case 3:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
			case 24:
			case 25:
			case 26:
			case 27:
			case 28:
			case 29:
			case 30:
			case 31:
			case 32:
			case 33:
			case 34:
			case 35:
			case 36:
			case 37:
			case 38:
			case 40:
			case 41:
			case 42:
			case 43:
			case 44:
			case 45:
			case 46:
				S->riscv->instruction_taintDistribution[S->riscv->P.EX.op].taintCol =
						S->riscv->instruction_taintDistribution[S->riscv->P.EX.op].taintCol
						| immtaint1 | immtaint2 | S->riscv->taintR[32].taintCol;
				break;
			default:
				break;
		}
	}

	switch(memType)
	{
		case kSunflowerTaintMemTypeMemory:
			S->TAINTMEM[AddrOut-S->TAINTMEMBASE].taintCol = outCol;
			break;

		case kSunflowerTaintMemTypeRegister:

			S->riscv->taintR[AddrOut].taintCol = outCol;

			break;

		case kSunflowerTaintMemTypefltRegister:
			S->riscv->taintfR[AddrOut].taintCol = outCol;
			break;

		case kSunflowerTaintMemTypeInstruction:
			mprint(E,S,nodeinfo,"A ShadowMem of type kSunflowerTaintMemTypeInstruction was passed to taintprop this should never be the case!");
			break;

		default:
			mprint(E,S,nodeinfo,"In taintprop something went very wrong: illegal memType passed");
			break;
	}

	return;
}

uint64_t
taintretmems(Engine *E, State *S, uint64_t Addr1, int NumBytes)
{
	/*
	*	Function returns the ORed taintCol of the n consecutive memory addresses
	*	after Addr1
	*/
	uint64_t tmpCol1;
	uint64_t outCol = 0;
	uint64_t tmpAddr;

	for (int i = 0; i < NumBytes ; i++)
	{
		tmpAddr = Addr1 + i;
		if (contains(tmpAddr,S->PC,kSunflowerTaintMemTypeMemory))
		{
			tmpCol1 = findByAll(E,S,tmpAddr,S->PC,kSunflowerTaintMemTypeMemory);
		}
		else
		{
			tmpCol1 = S->TAINTMEM[tmpAddr-S->TAINTMEMBASE].taintCol;
		}
		outCol = tmpCol1 | outCol;
	}
	return outCol;
}

uint64_t
taintretreg(Engine *E, State *S, uint64_t rs1)
{
	uint64_t outCol;
	if (contains(rs1,S->PC,kSunflowerTaintMemTypeRegister))
	{
		outCol = findByAll(E,S,rs1,S->PC,kSunflowerTaintMemTypeRegister);
	}
	else
	{
		outCol =  S->riscv->taintR[rs1].taintCol;
	}
	return outCol;
}

uint64_t
ftaintretreg(Engine *E, State *S, uint64_t rs1)
{
	uint64_t outCol;

	if (contains(rs1,S->PC,kSunflowerTaintMemTypefltRegister))
	{
		outCol = findByAll(E,S,rs1,S->PC,kSunflowerTaintMemTypefltRegister);
	}
	else
	{
		outCol = S->riscv->taintfR[rs1].taintCol;
	}

	return outCol;
}


void
taintclear(Engine *E, State *S,uint64_t addr, SunflowerTaintMemType memType)
{
	switch(memType)
	{
		case kSunflowerTaintMemTypeMemory:
			S->TAINTMEM[addr-S->TAINTMEMBASE].taintCol = 0;
			break;

		case kSunflowerTaintMemTypeRegister:	
			S->riscv->taintR[addr].taintCol = 0;
			break;

		case kSunflowerTaintMemTypefltRegister:
			S->riscv->taintfR[addr].taintCol = 0;
			break;

		case kSunflowerTaintMemTypeInstruction:
			mprint(E,S,nodeinfo,"A ShadowMem of type kSunflowerTaintMemTypeInstruction was passed to taintclear this should never be the case!");
			break;

		default:
			mprint(E,S,nodeinfo,"In taintclear something went very wrong: illegal memType passed");
	}
	return;
}

void
m_taintmem(Engine *E, State *S, uint64_t addr, uint32_t taintstartPC, uint32_t taintendPC, uint64_t taintCol, uint64_t taintLength)
{
	/*	
	*	iterate over all addresses within taintLength (assumption:
	*	taintLength in bytes)
	*/

	uint64_t tmpAddr = addr;

	for (int i = 0; i < taintLength; i++)
	{
		tmpAddr = addr + i;

		if (containsExactPC(tmpAddr,taintstartPC,taintendPC,kSunflowerTaintMemTypeMemory))
		{
			mprint(E,S,nodeinfo,"You're adding a duplicate taint origin: exact same address, PC start and end values and memType, with only the taint colour being different. Perhaps you should reconsider this life choice.");
		}

		insertFirst(tmpAddr, taintstartPC, taintendPC, taintCol, kSunflowerTaintMemTypeMemory);
	}
	return;
}

void
m_taintreg(Engine *E, State *S, uint64_t addr, uint32_t taintstartPC, uint32_t taintendPC, uint64_t taintCol)
{
	if (containsExactPC(addr,taintstartPC,taintendPC,kSunflowerTaintMemTypeRegister))
	{
		mprint(E,S,nodeinfo,"You're adding a duplicate taint origin: exact same address, PC start and end values and memType, with only the taint colour being different. Perhaps you should reconsider this life choice.");
	}

	insertFirst(addr, taintstartPC, taintendPC, taintCol, kSunflowerTaintMemTypeRegister);

	if (!RegMarked)
	{
		for (int i = 0; i < (sizeof(S->riscv->taintR)/sizeof(ShadowMem)) ; i++)
		{
			S->riscv->taintR[i].memType = kSunflowerTaintMemTypeRegister;
		}
		RegMarked = true;
	}
	return;
}

void
m_ftaintreg(Engine *E, State *S, uint64_t addr, uint32_t taintstartPC, uint32_t taintendPC, uint64_t taintCol)
{
	if (containsExactPC(addr,taintstartPC,taintendPC,kSunflowerTaintMemTypefltRegister))
	{
		mprint(E,S,nodeinfo,"You're adding a duplicate taint origin: exact same address, PC start and end values and memType, with only the taint colour being different. Perhaps you should reconsider this life choice.");
	}

	insertFirst(addr, taintstartPC, taintendPC, taintCol, kSunflowerTaintMemTypefltRegister);

	if (!fRegMarked)
	{
		for (int i = 0; i < (sizeof(S->riscv->taintfR)/sizeof(ShadowMem)) ; i++)
		{
			S->riscv->taintfR[i].memType = kSunflowerTaintMemTypefltRegister;
		}
		fRegMarked = true;
	}

	return;
}

void
m_riscvdumptaintdistr(Engine *E, State *S)
{
	for(int i = 0; i < RISCV_OP_MAX; i++)
	{
		mprint(E, S, nodeinfo, "%-8s {", riscv_opstrs[i]);
			for(int j = 0 ; j < 8*sizeof(S->riscv->instruction_taintDistribution[0].taintCol) ; j++)
			{
				mprint(E,S,nodeinfo,"%d",(((S->riscv->instruction_taintDistribution[i].taintCol >> (8*sizeof(S->riscv->instruction_taintDistribution[0].taintCol)-j-1))  & 1) ? 1 : 0));
			}
		mprint(E, S, nodeinfo, "}\n");
	}
}