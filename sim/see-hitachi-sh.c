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

void
superHhwSEEreg(Engine *E, State *S, char *hwstruct, int actual_bits, int logical_bits, int bit_offset)
{
	/*								*/
	/*	Current implementation can't handle registering hw	*/
	/*	structures that are more than 32 bits.  Such structs	*/
	/*	must be registered in chunks of 32 bits.		*/
	/*								*/
	if (actual_bits > 32)
	{
		mprint(E, NULL, siminfo,
			"Current implementation can't handle registering hw"
			"structures that are more than 32 bits.  Such structs"
			"must be registered in chunks of 32 bits.");
		return;
	}

	if (bit_offset > actual_bits)
	{
		mprint(E, NULL, siminfo,
			"Invalid bit_offset in HWSEEREG command;  "
			"actual_bits/bit_offset is \"%d\"/\"%d\"",
			actual_bits, bit_offset);
		return;
	}

	if (logical_bits <= 0)
	{
		mprint(E, NULL, siminfo,
			"Invalid logical_bits in HWSEEREG command.");
		return;
	}

	if (S->SEEstate == NULL)
	{
		mprint(E, S, nodeinfo,
			"Initalizing node's SEE generation state with defaults.\n"
			"It is unlikely that you wanted these defaults.\n"
			"Issuing a INITSEESTATE prior to HWSEEREG recommended. Beware!");
		/*						*/
		/*	Initialize SEE state with a default	*/
		/*	(loc min/max = 0, uniform bitstate)	*/
		/*						*/	
		superHinitSEEstate(E, S,	"pfunurnd", 0, 0, 0, 0,
					"pfunurnd", 0, 0, 0, 0,);
	}

	decoded_hwstruct = m_decodehwstructname(hwstruct);
	switch (decoded_hwstruct)
	{
		case SF_HWSTRUCT_R0_BANK0:
		{
			m_hwSEEreg(&S->superH->R[0],
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_R1_BANK0:
		{
			m_hwSEEreg(&S->superH->R[1],
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_R2_BANK0:
		{
			m_hwSEEreg(&S->superH->R[2],
				actual_bits, logical_bits, bit_offset);

			break;
		}

		case SF_HWSTRUCT_R3_BANK0:
		{
			m_hwSEEreg(&S->superH->R[3],
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_R4_BANK0:
		{
			m_hwSEEreg(&S->superH->R[4],
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_R5_BANK0:
		{
			m_hwSEEreg(&S->superH->R[5],
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_R6_BANK0:
		{
			m_hwSEEreg(&S->superH->R[6],
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_R7_BANK0:
		{
			m_hwSEEreg(&S->superH->R[7],
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_R8:
		{
			m_hwSEEreg(&S->superH->R[8],
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_R9:
		{
			m_hwSEEreg(&S->superH->R[9],
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_R10:
		{
			m_hwSEEreg(&S->superH->R[10],
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_R11:
		{
			m_hwSEEreg(&S->superH->R[11],
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_R12:
		{
			m_hwSEEreg(&S->superH->R[12],
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_R13:
		{
			m_hwSEEreg(&S->superH->R[13],
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_R14:
		{
			m_hwSEEreg(&S->superH->R[14],
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_R15:
		{
			m_hwSEEreg(&S->superH->R[15],
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_R0_BANK1:
		{
			m_hwSEEreg(&S->superH->R_BANK[0],
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_R1_BANK1:
		{
			m_hwSEEreg(&S->superH->R_BANK[1],
				actual_bits, logical_bits, bit_offset);
		}

		case SF_HWSTRUCT_R2_BANK1:
		{
			m_hwSEEreg(&S->superH->R_BANK[2],
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_R3_BANK1:
		{
			m_hwSEEreg(&S->superH->R_BANK[3],
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_R4_BANK1:
		{
			m_hwSEEreg(&S->superH->R_BANK[4],
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_R5_BANK1:
		{
			m_hwSEEreg(&S->superH->R_BANK[5],
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_R6_BANK1:
		{
			m_hwSEEreg(&S->superH->R_BANK[6],
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_R7_BANK1:
		{
			m_hwSEEreg(&S->superH->R_BANK[7],
				actual_bits, logical_bits, bit_offset);
			break;
		}


        	case SF_HWSTRUCT_GBR:
		{
			m_hwSEEreg(&S->superH->GBR,
				actual_bits, logical_bits, bit_offset);
			break;
		}

        	case SF_HWSTRUCT_VBR:
		{
			m_hwSEEreg(&S->superH->VBR,
				actual_bits, logical_bits, bit_offset);
			break;
		}

        	case SF_HWSTRUCT_MACH:
		{
			m_hwSEEreg(&S->superH->MACH,
				actual_bits, logical_bits, bit_offset);
			break;
		}

        	case SF_HWSTRUCT_MACL:
		{
			m_hwSEEreg(&S->superH->MACL,
				actual_bits, logical_bits, bit_offset);
			break;
		}

        	case SF_HWSTRUCT_PR:
		{
			m_hwSEEreg(&S->superH->PR,
				actual_bits, logical_bits, bit_offset);
			break;
		}

        	case SF_HWSTRUCT_TRA:
		{
			m_hwSEEreg(&S->superH->TRA,
				actual_bits, logical_bits, bit_offset);
			break;
		}

        	case SF_HWSTRUCT_SPC:
		{
			m_hwSEEreg(&S->superH->SPC,
				actual_bits, logical_bits, bit_offset);
			break;
		}

        	case SF_HWSTRUCT_SR:
		{
			m_hwSEEreg(&S->superH->SR,
				actual_bits, logical_bits, bit_offset);
			break;
		}

        	case SF_HWSTRUCT_SSR:
		{
			m_hwSEEreg(&S->superH->SSR,
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_DATA_BUS:
		{
			m_hwSEEreg(&S->superH->B->data_bus,
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_PADDR_BUS:
		{
			m_hwSEEreg(&S->superH->B->paddr_bus,
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_VADDR_BUS:
		{
			m_hwSEEreg(&S->superH->B->vaddr_bus,
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_PERADDR_BUS:
		{
			m_hwSEEreg(&S->superH->B->peraddr_bus,
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_PERDATA_BUS:
		{
			m_hwSEEreg(&S->superH->B->perdata_bus,
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_PC:
		{
			m_hwSEEreg(&S->PC,
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_P.EX.INSTR:
		{
			m_hwSEEreg(&S->superH->P.EX.instr,
				actual_bits, logical_bits, bit_offset;
			break;
		}

		case SF_HWSTRUCT_P.ID.INSTR:
		{
			m_hwSEEreg(&S->superH->P.ID.instr,
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_P.IF.INSTR:
		{
			m_hwSEEreg(&S->superH->P.IF.instr,
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_P.MA.INSTR:
		{
			m_hwSEEreg(&S->superH->P.MA.instr,
				actual_bits, logical_bits, bit_offset);
			break;
		}

		case SF_HWSTRUCT_P.WB.INSTR:
		{
			m_hwSEEreg(&S->superH->P.WB.instr,
				actual_bits, logical_bits, bit_offset);
			break;
		}

		default:
		{
			mprint(E, NULL, siminfo, "Invalid structure name in IDUCESEE command. Ignoring");

			return; 
		}
	}


	return;
}

void
superHinitSEEstate(Engine *E, State *S,
			char *occurpfun_name, double o_p1, double o_p2, double o_p3, double o_p4,
			char *locpfun_name, double l_p1, double l_p2, double l_p3, double l_p4,
			char *bitpfun_name, double b_p1, double b_p2, double b_p3, double b_p4,
			char *durpfun_name, double d_p1, double d_p2, double d_p3, double d_p4)
{
	int	loc_pfun, bit_pfun, dur_pfun;


	/*	Per-timestep probability of occurrence, e.g., w/ IFR, DFR, CFR etc.	*/
	occur_pfun = m_pfunname2id(occurpfun_name);
	if (occur_pfun < 0)
	{
		sfatal(S, "Invalid per-timestep occurrence probability distribution name");
		return;
	}

	loc_pfun = m_pfunname2id(locpfun_name);
	if (loc_pfun < 0)
	{
		sfatal(S, "Invalid bit-location probability distribution name");
		return;
	}

	bit_pfun = m_pfunname2id(bitpfun_name);
	if (bit_pfun < 0)
	{
		sfatal(S, "Invalid bit-state probability distribution name");
		return;
	}

	dur_pfun = m_pfunname2id(durpfun_name);
	if (dur_pfun < 0)
	{
		sfatal(S, "Invalid duration probability distribution name");
		return;
	}

	S->SEEstate = (SEEstate *)mcalloc(1, sizeof(SEEstate));
	if (S->SEEstate == NULL)
	{
		mexit("Failed to allocate memory for S->SEEstate.", -1);
	}

	S->SEEstate->loc_pfun		= E->randfns[loc_pfun];
	S->SEEstate->loc_pfun_params	= (PFUNparams *) mcalloc(1, sizeof(PFUNparams));
	if (S->SEEstate->loc_pfun_params == NULL)
	{
		sfatal(NULL, siminfo, Emalloc);
	}
	S->SEEstate->loc_pfun_params->p1	= l_p1;
	S->SEEstate->loc_pfun_params->p2	= l_p2;
	S->SEEstate->loc_pfun_params->p3	= l_p3;
	S->SEEstate->loc_pfun_params->p4	= l_p4;

	S->SEEstate->bit_pfun		= E->randfns[bit_pfun];
	S->SEEstate->bit_pfun_params	= (PFUNparams *) mcalloc(1, sizeof(PFUNparams));
	if (S->SEEstate->bit_pfun_params == NULL)
	{
		sfatal(NULL, siminfo, Emalloc);
	}
	S->SEEstate->bit_pfun_params->p1	= b_p1;
	S->SEEstate->bit_pfun_params->p2	= b_p2;
	S->SEEstate->bit_pfun_params->p3	= b_p3;
	S->SEEstate->bit_pfun_params->p4	= b_p4;

	S->SEEstate->dur_pfun		= E->randfns[dur_pfun];
	S->SEEstate->dur_pfun_params	= (PFUNparams *) mcalloc(1, sizeof(PFUNparams));
	if (S->SEEstate->dur_pfun_params == NULL)
	{
		sfatal(NULL, siminfo, Emalloc);
	}
	S->SEEstate->dur_pfun_params->p1	= d_p1;
	S->SEEstate->dur_pfun_params->p2	= d_p2;
	S->SEEstate->dur_pfun_params->p3	= d_p3;
	S->SEEstate->dur_pfun_params->p4	= d_p4;

//TODO: bitstate dist applied to all bits to enable modeling multi-bit upsets ?

	return;
}
