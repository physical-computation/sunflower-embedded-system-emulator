#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "sf.h"
#include "mextern.h"


/*	For ease of flexibility (to avoid different functions for
*	different combinations of registers and memory in propagation,
*	taintprop has ShadowMems (rather than addresses to ShadowMems)
*	as arguments
*/

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
		mprint(E,S,nodeinfo,"%llu,%u,%llu\n",ptr->taintAddress,ptr->taintPC,ptr->taintCol);
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

TaintOriginNode *
findByAll (Engine *E, State *S, uint64_t searchAddr, uint32_t taintPC, SunflowerTaintMemType memType)
{
	TaintOriginNode * current = taintOriginHead;
	
	if (taintOriginHead == NULL)
	{
		mprint(E,S,nodeinfo,"Couldn't find taintOriginHead in findByAll function");
		return NULL;
	}

	while (current->taintAddress != searchAddr
		|| current->taintPC != taintPC
		|| current->memType != memType)
	{
		if (current->next == NULL)
		{
			mprint(E,S,nodeinfo,"Couldn't find taintOriginHead in findByAll function");
			return NULL;
		}
		else
		{
			current = current -> next;
		}
	}
	return current;
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
		|| current->taintPC != taintPC
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

void
insertFirst (uint64_t addr, uint32_t PCval, uint64_t taintCol, SunflowerTaintMemType memType)
{
	TaintOriginNode * link = (TaintOriginNode *) calloc(1,sizeof(TaintOriginNode));
	
	link->taintAddress	= addr;
	link->taintPC		= PCval;
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
delete(Engine *E, State *S, uint64_t addr, uint32_t taintPC, SunflowerTaintMemType memType)
{
	TaintOriginNode * current 	= taintOriginHead;
	TaintOriginNode * prev		= NULL;

	if (taintOriginHead == NULL)
	{
		mprint(E,S,nodeinfo,"Tried to delete from an empty list in delete()");
		return NULL;
	}

	while  (current->taintAddress != addr
		|| current->taintPC != taintPC
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
	uint64_t Addr1, ShadowMem SM1,
	uint64_t Addr2, ShadowMem SM2,
			ShadowMem SMO)
{
	uint64_t tmpCol1;
	uint64_t tmpCol2;

	if (contains(Addr1,S->PC,SM1.memType))
	{
		tmpCol1 = findByAll(E,S,Addr1,S->PC,SM1.memType)->taintCol;
	}
	else
	{
		tmpCol1 = SM1.taintCol;
	}

	if (contains(Addr2,S->PC,SM2.memType))
	{
		tmpCol2 = findByAll(E,S,Addr2,S->PC,SM2.memType)->taintCol;
	}
	else
	{
		tmpCol2 = SM2.taintCol;
	}


	SMO.taintCol = tmpCol1 | tmpCol2 | S->riscv->taintR[32].taintCol;

	/*
	*	Last OR represents PC taint which should be propagated on every step
	*/

	return;
}

void
taintpropi(Engine *E, State *S,
	uint64_t Addr1, ShadowMem SM1,
	uint64_t immtaint, ShadowMem SMO)
{
	uint64_t tmpCol1;

	if (contains(Addr1,S->PC,SM1.memType))
	{
		tmpCol1 = findByAll(E,S,Addr1,S->PC,SM1.memType)->taintCol;
	}
	else
	{
		tmpCol1 = SM1.taintCol;
	}

	SMO.taintCol = tmpCol1 | immtaint | S->riscv->taintR[32].taintCol;
	return;
}

void
taintpropPC(Engine *E, State *S,
	uint64_t Addr1, ShadowMem SM1,
	uint64_t Addr2, ShadowMem SM2)
{
	/*
	*	This procedure is used specifically to update the taint of the PC
	*	Assumption: PC taint can be overwritten
	*/
	uint64_t tmpCol1;
	uint64_t tmpCol2;

	if (contains(Addr1,S->PC,SM1.memType))
	{
		tmpCol1 = findByAll(E,S,Addr1,S->PC,SM1.memType)->taintCol;
	}
	else
	{
		tmpCol1 = SM1.taintCol;
	}

	if (contains(Addr2,S->PC,SM2.memType))
	{
		tmpCol2 = findByAll(E,S,Addr2,S->PC,SM2.memType)->taintCol;
	}
	else
	{
		tmpCol2 = SM2.taintCol;
	}

	S->riscv->taintR[32].taintCol = tmpCol1 | tmpCol2;
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
			tmpCol1 = findByAll(E,S,tmpAddr,S->PC,kSunflowerTaintMemTypeMemory)->taintCol;
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
		outCol = findByAll(E,S,rs1,S->PC,kSunflowerTaintMemTypeRegister)->taintCol;
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
		outCol = findByAll(E,S,rs1,S->PC,kSunflowerTaintMemTypefltRegister)->taintCol;
	}
	else
	{
		outCol = S->riscv->taintfR[rs1].taintCol;
	}

	return outCol;
}


void
taintclear(Engine *E, State *S,ShadowMem SM1)
{
	SM1.taintCol = 0;
	return;
}

void
m_taintmem(Engine *E, State *S, uint64_t addr, uint32_t taintPC, uint64_t taintCol, uint64_t taintLength)
{
	/*	
	*	iterate over all addresses within taintLength (assumption:
	*	taintLength in bytes)
	*/
	for (int i = 0; i < taintLength; i++)
	{
		insertFirst(addr, taintPC, taintCol, kSunflowerTaintMemTypeMemory);
	}
	return;
}

void
m_taintreg(Engine *E, State *S, uint64_t addr, uint32_t taintPC, uint64_t taintCol)
{
	insertFirst(addr, taintPC, taintCol, kSunflowerTaintMemTypeRegister);

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
m_ftaintreg(Engine *E, State *S, uint64_t addr, uint32_t taintPC, uint64_t taintCol)
{
	insertFirst(addr, taintPC, taintCol, kSunflowerTaintMemTypefltRegister);

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
		mprint(E, S, nodeinfo, "%-8s {%llu}\n", riscv_opstrs[i], S->riscv->instruction_taintDistribution[i]);
	}
}