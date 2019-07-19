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

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "sf.h"
#include "mextern.h"







	/*								*/
	/*	!!!!!! NOTE:  This implementation is out of sync 	*/
	/*	with changes made over the last few years to the	*/
	/*	simulator.						*/
	/*								*/








void
pau_init(Engine *E, State *S, int nentries)
{
	if (S->superH->PAUs != NULL)
	{
		mfree(E, S->superH->PAUs, "S->superH->PAUs");
	}

	S->superH->PAUs = (PAUentry *)mcalloc(E, nentries, sizeof(PAUentry),
		"(PAUentry *)S->superH->PAUs");
	if (S->superH->PAUs == NULL)
	{
		mexit(E, "Could not allocate memory for PAUentry *X", -1);
	}

	S->superH->PAUvalids = (int *)mcalloc(E, nentries, sizeof(int),
		"(int *)S->superH->PAUvalids");
	if (S->superH->PAUvalids == NULL)
	{
		mexit(E, "Could not allocate memory for PAUvalids array", -1);
	}

	S->superH->npau = nentries;
	S->superH->controlling_pau = -1;
	S->superH->numpauvalids	= 0;
	S->superH->influenced = 0;
	S->superH->pauaddrmask = (ulong)pow(2, ceil(log10(S->superH->npau)/log10(2))) - 1;

	mprint(E, S, nodeinfo, "\nPAU size to [%d] entries\n\n", S->superH->npau);


	return;
}

void
pau_clk(Engine *E, State *S)
{
	int	n, i;


	/*									*/
	/*	All memory accesses (and hence PC values) are word aligned,	*/
	/*	so LSB of address is always 0, so odd entries can be skipt.	*/
	/*									*/
	for (n = 0; n < S->superH->numpauvalids; n++)
	{
		i = S->superH->PAUvalids[n];

		/*	All valid states increment NCLKS	*/
		S->superH->PAUs[i].nclks++;

		if (S->superH->PAUs[i].active || S->superH->PAUs[i].transient)
		{
			if (S->superH->PAUs[i].stride == 0)
			{
				mprint(E, S, nodeinfo, "time between two stalls (stride) can only be ");
				mprint(E, S, nodeinfo, "> MEMSTALLCYCLES !!!\n");
				mprint(E, S, nodeinfo, "stride = " UVLONGFMT "\nQ=%d\ntrigger PC = 0x" UHLONGFMT "\n",
					S->superH->PAUs[i].stride, S->superH->PAUs[i].Q, S->superH->PAUs[i].trigger_pc);

				sfatal(E, S, "active/transient, and stride == 0");
			}

			/*								*/
			/*	Withering - if we go for _more than_ one 'period', 	*/
			/*	and have not been reset by an 'event', then maybe 	*/
			/*	events have stopped happening. Decrement Q ('wither' 	*/
			/*	this entry)						*/
			/*								*/
			if (S->superH->PAUs[i].nclks > S->superH->PAUs[i].stride)
			{
		/*	
			mprint(E, NULL, siminfo,
			"Withering pau entry [%d] (stride=%ld trigger PC=0x" UHLONGFMT ") CLK = %E...\n",
			i, S->superH->PAUs[i].stride, S->superH->PAUs[i].trigger_pc, S->CLK);
		*/

				S->superH->PAUs[i].stride = S->superH->PAUs[i].nclks;
				S->superH->PAUs[i].Q--;
			}

			if ((S->superH->PAUs[i].active) && (S->superH->PAUs[i].Q <= PAU_Q_LOW_H2O))
			{
		/*		
				mprint(E, NULL, siminfo, "PAU entry [%d] active->transient...\n", i);
		*/
				S->superH->PAUs[i].active = 0;
				S->superH->PAUs[i].transient = 1;

				/*	Were we the current 'influencer' ?	*/
				if (S->superH->controlling_pau == i)
				{
			mprint(E, S, nodeinfo, 
			"PAU entry [%d] ending influence (influencer PC = 0x" UHLONGFMT ", CLK = " UVLONGFMT ")...\n",
			i, S->superH->PAUs[i].trigger_pc, S->CLK);

					/*	Rfg. penalty = PAU_RFG_CYCLES*(NOP Energy)     */
					S->energyinfo.CPUEtot +=\
						S->scaledcurrents[SUPERH_OP_NOP]*\
						S->VDD*\
						S->CYCLETIME*\
						PAU_RFG_CYCLES;

					S->TIME += PAU_RFG_CYCLES*S->CYCLETIME;

					S->superH->influenced = 0;
					S->superH->controlling_pau = -1;
					S->VDD = S->SVDD;
					power_scaledelay(E, S, S->VDD);
				}
			}

			/*	Delete dying entries w/ Q == 0	*/
			if (S->superH->PAUs[i].Q == 0)
			{
		/*
			mprint(E, NULL, siminfo,
				"PAU entry [%d] transient->invalid (trigger PC=0x" UHLONGFMT ")...\n",
				i, S->superH->PAUs[i].trigger_pc);
		*/

					memset(&S->superH->PAUs[i], 0, sizeof(PAUentry));

					S->superH->PAUvalids[n] = 
						S->superH->PAUvalids[S->superH->numpauvalids-1];
					S->superH->numpauvalids--;
			}
		}
		else if ((S->superH->PAUs[i].init) && (S->superH->PAUs[i].nclks == PAU_STRIDE_MAX))
		{
		/*
			mprint(E, NULL, siminfo, "discarding dormant INIT entry\n");
		*/
			memset(&S->superH->PAUs[i], 0, sizeof(PAUentry));

			S->superH->PAUvalids[n] = S->superH->PAUvalids[S->superH->numpauvalids-1];
			S->superH->numpauvalids--;
		}
	}

	return;
}

void
pau_lvdd(Engine *E, State *S, int pauentry)
{
	int		i;
	float		mf, cf, delta, calc_vdd;


	/*	Rcfg. penalty = PAU_RFG_CYCLES*(NOP Energy cost)	*/
	S->energyinfo.CPUEtot +=\
		S->scaledcurrents[SUPERH_OP_NOP]*\
		S->VDD*\
		S->CYCLETIME*\
		PAU_RFG_CYCLES;

	S->TIME += PAU_RFG_CYCLES*S->CYCLETIME;

	mprint(E, S, nodeinfo, "stride=[" UVLONGFMT "], instrs=[" UVLONGFMT "]\n",
		S->superH->PAUs[pauentry].stride,
		S->superH->PAUs[pauentry].instrs);
	mf = ((float)(S->superH->PAUs[pauentry].stride - S->superH->PAUs[pauentry].instrs))/
		((float)S->superH->PAUs[pauentry].stride);
	cf = ((float)S->superH->PAUs[pauentry].instrs)/((float)S->superH->PAUs[pauentry].stride);
	delta = ((0.01)*(mf+cf) + cf)/cf;
	if (delta < 1.0)
	{
		delta = 1.0;
	}
	calc_vdd = S->VDD/delta;
	mprint(E, S, nodeinfo, "mf=[%.4f], cf=[%.4f], delta=[%.4f], calc_vdd=[%.4f]\n",
		mf, cf, delta, calc_vdd);

	S->SVDD = S->VDD;
	S->VDD = calc_vdd;
	power_scaledelay(E, S, S->VDD);

	for (i = SUPERH_OP_ADD; i <= SUPERH_OP_XTRCT; i++)
	{
		double reading = (R0000[i].reading1 + R0000[i].reading2)/2;

		/*							*/
		/*	Scaled current, I2 = (I1*V2*t1)/(V1*t2);	*/
		/*							*/
		S->scaledcurrents[i] =
			((reading*S->VDD*SUPERH_ORIG_CYCLE)/(SUPERH_READINGS_VDD*S->CYCLETIME))*1E-3;
	}


	return;
}

void
pau_feed(Engine *E, State *S, int feed_type, ulong addr)
{
	if (S->superH->PAUs == NULL)
	{
		return;
	}

	switch (feed_type)
	{
		/*	  Entry Use : We consider stalls due to IF	*/
		case MEM_READ_BYTE:
		case MEM_READ_WORD:
		case MEM_READ_LONG:
		case MEM_WRITE_BYTE:
		case MEM_WRITE_WORD:
		case MEM_WRITE_LONG:
		{
			ulong	i = (S->superH->P.EX.fetchedpc & S->superH->pauaddrmask);


			if (!S->superH->PAUs[i].valid)
			{
				return;
			}

			/*							*/
			/*	! We dont need to store whole trigger_pc, 	*/
			/*	! just the upper 32-PAU_MASK_BITS bits. 	*/
			/*			Hit in PAU			*/
			/*							*/
			if ((S->superH->PAUs[i].active) && (S->superH->PAUs[i].trigger_pc == S->superH->P.EX.fetchedpc))
			{
				/*	  If !(influenced), can influence	*/
				if (!S->superH->influenced && (S->superH->PAUs[i].stride > PAU_STRIDE_MIN))
				{
					/*	Lower VDD, calc rfg penalty	*/
					mprint(E, S, nodeinfo, "Lowering vdd (influencer PC = 0x" UHLONGFMT ", CLK = " UVLONGFMT ", STRIDE=" UVLONGFMT ")\n",\
						S->superH->PAUs[i].trigger_pc, S->CLK, S->superH->PAUs[i].stride);
					pau_lvdd(E, S, i);
					S->superH->influenced = 1;
					S->superH->controlling_pau = i;
				}
			}

			return;
		}


		/*	Entry Update : Only use stalls due to EX	*/
		case MEM_READ_STALL:
		case MEM_WRITE_STALL:
		{
			int	i = (S->superH->P.EX.fetchedpc & S->superH->pauaddrmask);

			if (S->superH->mem_access_type == MEM_ACCESS_IFETCH)
			{
				return;
			}

			if (!S->superH->PAUs[i].valid)
			{
			/*
				mprint(E, NULL, siminfo,
				"Allocating new PAU entry, CLK = " UVLONGFMT "\n", S->CLK);
			*/

				/*					*/
				/*	Initialise the new entry	*/
				/*	It doesnt become 'transient'	*/
				/*	till first stall and record	*/
				/*	stride.				*/
				/*					*/
				memset(&S->superH->PAUs[i], 0, sizeof(PAUentry));
				S->superH->PAUs[i].valid = 1;
				S->superH->PAUs[i].init = 1;
				S->superH->PAUs[i].trigger_pc = S->superH->P.EX.fetchedpc;

				S->superH->PAUvalids[S->superH->numpauvalids] = i;
				S->superH->numpauvalids++;

				return;
			}

			/*	Hit in PAU	*/
			if (S->superH->PAUs[i].trigger_pc == S->superH->P.EX.fetchedpc)
			{
			/*
				mprint(E, NULL, siminfo, "Hit in PAU, CLK = " UVLONGFMT "\n", S->CLK);
			*/

				/*					*/
				/*	As soon as nclks > stride, it 	*/
				/*	will get reset by pau_clk, so	*/
				/*	we could use the infromation	*/
				/*	as to whether stride has dec'd	*/
				/*	however, we do not.		*/
				/*					*/
				S->superH->PAUs[i].stride = S->superH->PAUs[i].nclks;
				S->superH->PAUs[i].nclks = 0;
				S->superH->PAUs[i].instrs = S->dyncnt - S->superH->PAUs[i].dyntmp;
				S->superH->PAUs[i].dyntmp = S->dyncnt;

				/*	Saturating Q	*/
				if (S->superH->PAUs[i].Q < PAU_Q_HIGH_H2O)
				{
					S->superH->PAUs[i].Q++;

				/*
					mprint(E, NULL, siminfo, "trigger PC 0x" UHLONGFMT ", Q=%ld\n",
						S->superH->PAUs[i].trigger_pc,
						S->superH->PAUs[i].Q);
				*/
				}

				/*					*/
				/*	If first time hit, we now	*/
				/*	have a valid stride. become 	*/
				/*	transient			*/
				/*					*/
				if (S->superH->PAUs[i].init)
				{
			/*
				mprint(E, NULL, siminfo,
					"init->transient for PAU entry for trigger PC 0x" UHLONGFMT ",
					CLK=%E, stride=%ld\n",
					S->superH->PAUs[i].trigger_pc,
					S->CLK, S->superH->PAUs[i].stride);
			*/

					S->superH->PAUs[i].transient = 1;
					S->superH->PAUs[i].init = 0;
				}
				else if ((S->superH->PAUs[i].transient) &&
						(S->superH->PAUs[i].Q == PAU_Q_HIGH_H2O))
				{
			/*
				mprint(E, NULL, siminfo,
				"transient->active  for trigger PC 0x" UHLONGFMT ", Q=%ld, stride=%ld\n",
				S->superH->PAUs[i].trigger_pc, 	S->superH->PAUs[i].Q, 
				S->superH->PAUs[i].stride);
			*/

					S->superH->PAUs[i].active = 1;
					S->superH->PAUs[i].transient = 0;
				}
			}

			return;
		}

		default:
		{	
			sfatal(E, S, "invalid food for PAU :(");
			return; //ha ha!
		}
	}

	return;
}


void
pau_printstats(Engine *E, State *S)
{
	int	i;


	if (S->machinetype != MACHINE_SUPERH)
	{
		merror(E, "This machine does not know how to \"pauinfo\".");
		return;
	}

	mprint(E, S, nodeinfo, "\n");
	for (i = 0; i < S->superH->npau; i++)
	{
		if (!S->superH->PAUs[i].valid)
		{
			continue;
		}
		
		mprint(E, S, nodeinfo,
			"PAU[%d].trigger_pc=0x" UHLONGFMT ", stride=" UVLONGFMT ", nclks=" UVLONGFMT ", instrs=" UVLONGFMT ", Q=%d,I=%d,T=%d,A=%d\n",
			i,
			S->superH->PAUs[i].trigger_pc,
			S->superH->PAUs[i].stride,
			S->superH->PAUs[i].nclks,
			S->superH->PAUs[i].instrs,
			S->superH->PAUs[i].Q,
			S->superH->PAUs[i].init,
			S->superH->PAUs[i].transient,
			S->superH->PAUs[i].active);
	}
	
	mprint(E, S, nodeinfo, "Number of active PAU entries = [%d]: ",
					S->superH->numpauvalids);
				
	for (i = 0; i < S->superH->numpauvalids; i++)
	{
		mprint(E, S, nodeinfo, "%d, ", S->superH->PAUvalids[i]);
	}		
	mprint(E, S, nodeinfo, "\n\n");
			

	return;
}
