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

#include "instr-ti-msp430.h"
#include "sf.h"

static tuck int		ilen(Engine *, State *S, int type, int amode_d, int amode_s);
static tuck int		ilat(Engine *, State *S, int type, int op, int amode_d, int amode_s, ushort instr);
static tuck int		smode(Engine *, State *S, int type, ushort instr);
static tuck int		dmode(Engine *, State *S, int type, ushort instr);



/*									*/
/*		Based on pages 3-72 and 3-73 of slau049d.pdf.		*/
/*									*/
tuck int
ilen(Engine *E, State *S, int type, int amode_d, int amode_s)
{
	/*								*/
	/*	Returned length is length in bytes. Manual lists	*/
	/*	length in words.					*/
	/*								*/

	if (type == MSP430_INSTR_I)
	{
		int	amode = (amode_s << 3)|amode_d;

		switch (amode)
		{
			case (MSP430_AMODE_REG<<3)|MSP430_AMODE_REG:
			case (MSP430_AMODE_IND<<3)|MSP430_AMODE_REG:
			case (MSP430_AMODE_INC<<3)|MSP430_AMODE_REG:
			{
				return 1*2;
			}

			case (MSP430_AMODE_REG<<3)|MSP430_AMODE_IDX:
			case (MSP430_AMODE_REG<<3)|MSP430_AMODE_SYM:
			case (MSP430_AMODE_REG<<3)|MSP430_AMODE_ABS:
			case (MSP430_AMODE_IND<<3)|MSP430_AMODE_IDX:
			case (MSP430_AMODE_IND<<3)|MSP430_AMODE_SYM:
			case (MSP430_AMODE_IND<<3)|MSP430_AMODE_ABS:
			case (MSP430_AMODE_INC<<3)|MSP430_AMODE_IDX:
			case (MSP430_AMODE_INC<<3)|MSP430_AMODE_SYM:
			case (MSP430_AMODE_INC<<3)|MSP430_AMODE_ABS:
			case (MSP430_AMODE_IMM<<3)|MSP430_AMODE_REG:
			case (MSP430_AMODE_IDX<<3)|MSP430_AMODE_REG:
			case (MSP430_AMODE_SYM<<3)|MSP430_AMODE_REG:
			case (MSP430_AMODE_ABS<<3)|MSP430_AMODE_REG:
			{
				return 2*2;
			}

			case (MSP430_AMODE_IMM<<3)|MSP430_AMODE_IDX:
			case (MSP430_AMODE_IMM<<3)|MSP430_AMODE_SYM:
			case (MSP430_AMODE_IMM<<3)|MSP430_AMODE_ABS:
			case (MSP430_AMODE_IDX<<3)|MSP430_AMODE_SYM:
			case (MSP430_AMODE_IDX<<3)|MSP430_AMODE_IDX:
			case (MSP430_AMODE_IDX<<3)|MSP430_AMODE_ABS:
			case (MSP430_AMODE_SYM<<3)|MSP430_AMODE_SYM:
			case (MSP430_AMODE_SYM<<3)|MSP430_AMODE_IDX:
			case (MSP430_AMODE_SYM<<3)|MSP430_AMODE_ABS:
			case (MSP430_AMODE_ABS<<3)|MSP430_AMODE_SYM:
			case (MSP430_AMODE_ABS<<3)|MSP430_AMODE_IDX:
			case (MSP430_AMODE_ABS<<3)|MSP430_AMODE_ABS:
			{
				return 3*2;
			}

			default:
			{
				sfatal(E, S, "Unknown instruction src/dst modes supplied to ilen");
			}
		}
	}
	else if (type == MSP430_INSTR_II)
	{
		switch (amode_d)
		{
			case MSP430_AMODE_REG:
			case MSP430_AMODE_IND:
			case MSP430_AMODE_INC:
			{
				return 1*2;
			}

			case MSP430_AMODE_IMM:
			case MSP430_AMODE_IDX:
			case MSP430_AMODE_SYM:
			case MSP430_AMODE_ABS:
			{
				return 2*2;
			}

			default:
			{
				sfatal(E, S, "Unknown instruction mode supplied to ilen");
			}
		}
	}
	else if (type == MSP430_INSTR_III)
	{
		/*								*/
		/*	All format III (jump) instruction are one word long	*/
		/*								*/
		return 1*2;
	}
	else
	{
		sfatal(E, S, "Unknown instruction format supplied to ilen");
	}
	
	/* Not reached */

	return -1;
}

tuck int
ilat(Engine *E, State *S, int type, int op, int amode_d, int amode_s, ushort instr)
{
	if (type == MSP430_INSTR_I)
	{
		int	amode = (amode_s << 3)|amode_d;

		switch (amode)
		{
			case (MSP430_AMODE_REG<<3)|MSP430_AMODE_REG:
			{
				if (msp430_instrI_dreg(instr) == 0)
				{
					return 2;
				}
				
				return 1;
			}

			case (MSP430_AMODE_REG<<3)|MSP430_AMODE_IDX:
			case (MSP430_AMODE_REG<<3)|MSP430_AMODE_SYM:
			case (MSP430_AMODE_REG<<3)|MSP430_AMODE_ABS:
			{
				return 4;
			}

			case (MSP430_AMODE_IND<<3)|MSP430_AMODE_REG:
			{
				/*						*/
				/*	@Rn, PC was listed as 3 cycles in	*/
				/*	slau049d.pdf, but in the new		*/
				/*	slau049e.pdf, it is listed same as	*/
				/*	@Rn, Rn as 2 cycles			*/
				/*						*/
				//if (msp430_instrI_dreg(instr) == 0)
				//{
				//	return 3;
				//}

				return 2;
			}

			case (MSP430_AMODE_IND<<3)|MSP430_AMODE_IDX:
			case (MSP430_AMODE_IND<<3)|MSP430_AMODE_SYM:
			case (MSP430_AMODE_IND<<3)|MSP430_AMODE_ABS:
			{
				return 5;
			}

			case (MSP430_AMODE_INC<<3)|MSP430_AMODE_REG:
			{
				if (msp430_instrI_dreg(instr) == 0)
				{
					return 3;
				}

				return 2;
			}
	
			case (MSP430_AMODE_INC<<3)|MSP430_AMODE_IDX:
			case (MSP430_AMODE_INC<<3)|MSP430_AMODE_SYM:
			case (MSP430_AMODE_INC<<3)|MSP430_AMODE_ABS:
			{
				return 5;
			}

			case (MSP430_AMODE_IMM<<3)|MSP430_AMODE_REG:
			{
				if (msp430_instrI_dreg(instr) == 0)
				{
					return 3;
				}

				return 2;
			}

			case (MSP430_AMODE_IMM<<3)|MSP430_AMODE_IDX:
			case (MSP430_AMODE_IMM<<3)|MSP430_AMODE_SYM:
			case (MSP430_AMODE_IMM<<3)|MSP430_AMODE_ABS:
			{
				return 5;
			}

			case (MSP430_AMODE_IDX<<3)|MSP430_AMODE_REG:
			{
				return 3;
			}

			case (MSP430_AMODE_IDX<<3)|MSP430_AMODE_SYM:
			case (MSP430_AMODE_IDX<<3)|MSP430_AMODE_IDX:
			case (MSP430_AMODE_IDX<<3)|MSP430_AMODE_ABS:
			{
				return 6;
			}

			case (MSP430_AMODE_SYM<<3)|MSP430_AMODE_REG:
			{
				return 3;
			}

			case (MSP430_AMODE_SYM<<3)|MSP430_AMODE_SYM:
			case (MSP430_AMODE_SYM<<3)|MSP430_AMODE_IDX:
			case (MSP430_AMODE_SYM<<3)|MSP430_AMODE_ABS:
			{
				return 6;
			}

			case (MSP430_AMODE_ABS<<3)|MSP430_AMODE_REG:
			{
				return 3;
			}

			case (MSP430_AMODE_ABS<<3)|MSP430_AMODE_SYM:
			case (MSP430_AMODE_ABS<<3)|MSP430_AMODE_IDX:
			case (MSP430_AMODE_ABS<<3)|MSP430_AMODE_ABS:
			{
				return 6;
			}

			default:
			{
				sfatal(E, S, "Unknown instruction src/dst modes supplied to ilat");
			}
		}
	}
	else if (type == MSP430_INSTR_II)
	{
		switch (amode_d)
		{
			case MSP430_AMODE_REG:
			{
				switch (op)
				{
					case MSP430_OP_RRA:
					case MSP430_OP_RRC:
					case MSP430_OP_SWPB:
					case MSP430_OP_SXT:
					{
						return 1;
					}
					case MSP430_OP_PUSH:
					{
						return 3;
					}
					case MSP430_OP_CALL:
					{
						return 4;
					}
					default:
					{
						sfatal(E, S, "Illegal instruction type supplied to ilat()");
					}
				}
				break;
			}

			case MSP430_AMODE_IND:
			{
				switch (op)
				{
					case MSP430_OP_RRA:
					case MSP430_OP_RRC:
					case MSP430_OP_SWPB:
					case MSP430_OP_SXT:
					{
						return 3;
					}
					case MSP430_OP_PUSH:
					{
						return 4;
					}
					case MSP430_OP_CALL:
					{
						return 4;
					}
					default:
					{
						sfatal(E, S, "Illegal instruction type supplied to ilat()");
					}
				}
				break;
			}

			case MSP430_AMODE_INC:
			{
				switch (op)
				{
					case MSP430_OP_RRA:
					case MSP430_OP_RRC:
					case MSP430_OP_SWPB:
					case MSP430_OP_SXT:
					{
						return 3;
					}
					case MSP430_OP_PUSH:
					{
						/*					*/
						/*	Listed as 4 in slau049d.pdf, 	*/
						/*	updated to 5 in slau049e.pdf	*/
						/*					*/
						return 5;
					}
					case MSP430_OP_CALL:
					{
						return 5;
					}
					default:
					{
						sfatal(E, S, "Illegal instruction type supplied to ilat()");
					}
				}
				break;
			}

			case MSP430_AMODE_IMM:
			{
				switch (op)
				{
					case MSP430_OP_RRA:
					case MSP430_OP_RRC:
					case MSP430_OP_SWPB:
					case MSP430_OP_SXT:
					{
						sfatal(E, S, "instrlatency(): bad instr: "
							"RRA/RRC/SWPB/SXT not permitted "
							"in format II immediate mode");
					}
					case MSP430_OP_PUSH:
					{
						return 4;
					}
					case MSP430_OP_CALL:
					{
						return 5;
					}
					default:
					{
						sfatal(E, S, "Illegal instruction type supplied to ilat()");
					}
				}
				break;
			}

			case MSP430_AMODE_IDX:
			case MSP430_AMODE_SYM:
			case MSP430_AMODE_ABS:
			{
				switch (op)
				{
					case MSP430_OP_RRA:
					case MSP430_OP_RRC:
					case MSP430_OP_SWPB:
					case MSP430_OP_SXT:
					{
						return 4;
					}
					case MSP430_OP_PUSH:
					{
						return 5;
					}
					case MSP430_OP_CALL:
					{
						return 5;
					}
					default:
					{
						sfatal(E, S, "Illegal instruction type supplied to ilat()");
					}
				}
				break;
			}		

			default:
			{
				sfatal(E, S, "Unknown instruction mode supplied to ilat");
			}
		}
	}
	else if (type == MSP430_INSTR_III)
	{
		/*								*/
		/*	All format III (jump) instruction take 2 cycles 	*/
		/*	regardless of taken/not-taken resolution.		*/
		/*								*/
		return 2;
	}
	else
	{
		sfatal(E, S, "Unknown instruction format supplied to ilat");
	}

	/* Not reached */

	return -1;
}

/*									*/
/*	Source register addressing mode: the As field in an instr	*/
/*	is 00 for REG, 01 for IDX, 10 for IND, and 11 for INC.		*/
/*	However, if source registers (or source/dest for type II) is	*/
/*	r0 (i.e.,  PC). the 01 => SYM and 11 => IMM. If dsreg is r2 	*/
/*	(i.e. SR), then 01 => ABS. See Table 3-3 in slau049e.pdf.	*/
/*									*/
tuck int
smode(Engine *E, State *S, int type, ushort instr)
{
	int	asd = -1, dsreg = -1;

	switch (type)
	{
		case MSP430_INSTR_I:
		{
			asd = msp430_instrI_as(instr);
			dsreg = msp430_instrI_sreg(instr);

			break;
		}
		case MSP430_INSTR_II:
		{
			asd = msp430_instrII_ad(instr);
			dsreg = msp430_instrII_dsreg(instr);

			break;
		}
		case MSP430_INSTR_III:
		{
			sfatal(E, S, "Internal error: "
				"smode() should not have been called "
				"for a type III instruction");
			break;
		}
		default:
		{
			sfatal(E, S, "Unknown instruction format supplied to smode");
		}
	}

	switch (asd)
	{
		case B0000:
		{
			return MSP430_AMODE_REG;
		}
		case B0001:
		{
			if (dsreg == 0)
				return MSP430_AMODE_SYM;
			else if (dsreg == 2)
				return MSP430_AMODE_ABS;
			else
				return MSP430_AMODE_IDX;
		}
		case B0010:
		{
			return MSP430_AMODE_IND;
		}
		case B0011:
		{
			if (dsreg == 0)
				return MSP430_AMODE_IMM;
			else
				return MSP430_AMODE_INC;
		}
		default:
		{
			sfatal(E, S, "Invalid As in smode()");
		}
	}

	return -1;
}

tuck int
dmode(Engine *E, State *S, int type, ushort instr)
{
	int	ad = -1, dreg = -1;

	switch (type)
	{
		case MSP430_INSTR_I:
		{
			ad = msp430_instrI_ad(instr);
			dreg = msp430_instrI_dreg(instr);

			break;
		}
		case MSP430_INSTR_II:
		{
			sfatal(E, S, "Internal error: "
				"dmode() should not have been called for a"
				"type II instruction (use smode() instead)");
			break;
		}
		case MSP430_INSTR_III:
		{
			sfatal(E, S, "Internal error: "
				"dmode() should not have been called "
				"for a type III instruction");
			break;
		}
		default:
		{
			sfatal(E, S, "Unknown instruction format supplied to smode");
		}
	}

	switch (ad)
	{
		case 0:
		{
			return MSP430_AMODE_REG;
		}
		case 1:
		{
			if (dreg == 0)
				return MSP430_AMODE_SYM;
			else if (dreg == 2)
				return MSP430_AMODE_ABS;
			else
				return MSP430_AMODE_IDX;
		}
		default:
		{
			sfatal(E, S, "Invalid Ad in dmode()");
		}
	}

	return -1;
}

void
msp430decode(Engine *E, State *S, ushort instr, MSP430Pipestage *p)
{
	p->instr = instr;

	switch ((instr & 0xF000) >> 12)
	{
		case 0x1:
		{
			p->format = MSP430_INSTR_II;
			p->amode_s = p->amode_d = smode(E, S, p->format, p->instr);
			p->ilen = ilen(E, S, p->format, p->amode_d, p->amode_s);

			switch (instr & 0xFFF)
			{
				case 0x000:
				{
					p->op = MSP430_OP_RRC;
					p->fptr = (void *)msp430_rrc;

					if (p->amode_s == MSP430_AMODE_IMM)
					{
						sfatal(E, S,
							"Architecture has undefined behavior for "
							"immediate mode in destination field. "
							"See slau049d.pdf, Table 3-15");
					}

					break;
				}
				case 0x040:
				{
					p->op = MSP430_OP_RRCB;
					p->fptr = (void *)msp430_rrcb;

					break;
				}
				case 0x080:
				{
					p->op = MSP430_OP_SWPB;
					p->fptr = (void *)msp430_swpb;

					if (p->amode_s == MSP430_AMODE_IMM)
					{
						sfatal(E, S,
							"Architecture has undefined behavior for "
							"immediate mode in destination field. "
							"See slau049d.pdf, Table 3-15");
					}

					break;
				}
				case 0x100:
				{
					p->op = MSP430_OP_RRA;
					p->fptr = (void *)msp430_rra;

					if (p->amode_s == MSP430_AMODE_IMM)
					{
						sfatal(E, S,
							"Architecture has undefined behavior for "
							"immediate mode in destination field. "
							"See slau049d.pdf, Table 3-15");
					}

					break;
				}
				case 0x140:
				{
					p->op = MSP430_OP_RRAB;
					p->fptr = (void *)msp430_rrab;

					break;
				}
				case 0x180:
				{
					p->op = MSP430_OP_SXT;
					p->fptr = (void *)msp430_sxt;

					if (p->amode_s == MSP430_AMODE_IMM)
					{
						sfatal(E, S,
							"Architecture has undefined behavior for "
							"immediate mode in destination field. "
							"See slau049d.pdf, Table 3-15");
					}

					break;
				}
				case 0x200:
				{
					p->op = MSP430_OP_PUSH;
					p->fptr = (void *)msp430_push;

					break;
				}
				case 0x240:
				{
					p->op = MSP430_OP_PUSHB;
					p->fptr = (void *)msp430_pushb;

					break;
				}
				case 0x280:
				{
					p->op = MSP430_OP_CALL;
					p->fptr = (void *)msp430_call;

					break;
				}
				case 0x300:
				{
					p->op = MSP430_OP_RETI;
					p->fptr = (void *)msp430_reti;

					break;
				}
				default:
				{
					/*	No instructions mapped to this opcode	*/
					mprint(E, NULL, siminfo,
						"Illegal instruction encoding in msp430decode(), [%4X]\n",
						instr);
					sfatal(E, S, "See above messages.");
				}
			}
			p->cycles = ilat(E, S, p->format, p->op, p->amode_d, p->amode_s, instr);

			break;
		}

		case 0x2:
		{
			p->format = MSP430_INSTR_III;
			p->ilen = ilen(E, S, p->format, -1, -1);

			switch ((instr & 0x0F00) >> 8)
			{
				case 0x0:
				{
					p->op = MSP430_OP_JNEJZ;
					p->fptr = (void *)msp430_jnejnz;

					break;
				}
				case 0x4:
				{
					p->op = MSP430_OP_JEQJZ;
					p->fptr = (void *)msp430_jeqjz;

					break;
				}
				case 0x8:
				{
					p->op = MSP430_OP_JNC;
					p->fptr = (void *)msp430_jnc;

					break;
				}
				case 0xC:
				{
					p->op = MSP430_OP_JC;
					p->fptr = (void *)msp430_jc;

					break;
				}
				default:
				{
					/*	No instructions mapped to this opcode	*/
					mprint(E, NULL, siminfo,
						"Illegal instruction encoding in msp430decode(), [%4X]\n",
						instr);
					sfatal(E, S, "See above messages.");
				}
			}

			p->cycles = ilat(E, S, p->format, p->op, -1, -1, instr);

			break;
		}

		case 0x3:
		{
			p->format = MSP430_INSTR_III;
			p->ilen = ilen(E, S, p->format, -1, -1);

			switch ((instr & 0x0F00) >> 8)
			{
				case 0x0:
				{
					p->op = MSP430_OP_JN;
					p->fptr = (void *)msp430_jn;

					break;
				}
				case 0x4:
				{
					p->op = MSP430_OP_JGE;
					p->fptr = (void *)msp430_jge;

					break;
				}
				case 0x8:
				{
					p->op = MSP430_OP_JL;
					p->fptr = (void *)msp430_jl;

					break;
				}
				case 0xC:
				{
					p->op = MSP430_OP_JMP;
					p->fptr = (void *)msp430_jmp;

					break;
				}
				default:
				{
					/*	No instructions mapped to this opcode	*/
					mprint(E, NULL, siminfo,
						"Illegal instruction encoding in msp430decode(), [%4X]\n",
						instr);
					sfatal(E, S, "See above messages.");
				}
			}

			p->cycles = ilat(E, S, p->format, p->op, -1, -1, instr);

			break;
		}
		case 0x4:
		{
			p->format = MSP430_INSTR_I;
			p->amode_s = smode(E, S, p->format, p->instr);
			p->amode_d = dmode(E, S, p->format, p->instr);
			p->ilen = ilen(E, S, p->format, p->amode_d, p->amode_s);

			if ((instr >> 6) & B0001)
			{
				p->op = MSP430_OP_MOVB;
				p->fptr = (void *)msp430_movb;
			}
			else
			{
				p->op = MSP430_OP_MOV;
				p->fptr = (void *)msp430_mov;
			}
			p->cycles = ilat(E, S, p->format, p->op, p->amode_d, p->amode_s, instr);

			break;
		}
		case 0x5:
		{
			p->format = MSP430_INSTR_I;
			p->amode_s = smode(E, S, p->format, p->instr);
			p->amode_d = dmode(E, S, p->format, p->instr);
			p->ilen = ilen(E, S, p->format, p->amode_d, p->amode_s);

			if ((instr >> 6) & B0001)
			{
				p->op = MSP430_OP_ADDB;
				p->fptr = (void *)msp430_addb;
			}
			else
			{
				p->op = MSP430_OP_ADD;
				p->fptr = (void *)msp430_add;
			}
			p->cycles = ilat(E, S, p->format, p->op, p->amode_d, p->amode_s, instr);

			break;
		}
		case 0x6:
		{
			p->format = MSP430_INSTR_I;
			p->amode_s = smode(E, S, p->format, p->instr);
			p->amode_d = dmode(E, S, p->format, p->instr);
			p->ilen = ilen(E, S, p->format, p->amode_d, p->amode_s);

			if ((instr >> 6) & B0001)
			{
				p->op = MSP430_OP_ADDCB;
				p->fptr = (void *)msp430_addcb;
			}
			else
			{
				p->op = MSP430_OP_ADDC;
				p->fptr = (void *)msp430_addc;
			}
			p->cycles = ilat(E, S, p->format, p->op, p->amode_d, p->amode_s, instr);

			break;
		}
		case 0x7:
		{
			p->format = MSP430_INSTR_I;
			p->amode_s = smode(E, S, p->format, p->instr);
			p->amode_d = dmode(E, S, p->format, p->instr);
			p->ilen = ilen(E, S, p->format, p->amode_d, p->amode_s);

			if ((instr >> 6) & B0001)
			{
				p->op = MSP430_OP_SUBCB;
				p->fptr = (void *)msp430_subcb;
			}
			else
			{
				p->op = MSP430_OP_SUBC;
				p->fptr = (void *)msp430_subc;
			}
			p->cycles = ilat(E, S, p->format, p->op, p->amode_d, p->amode_s, instr);

			break;
		}
		case 0x8:
		{
			p->format = MSP430_INSTR_I;
			p->amode_s = smode(E, S, p->format, p->instr);
			p->amode_d = dmode(E, S, p->format, p->instr);
			p->ilen = ilen(E, S, p->format, p->amode_d, p->amode_s);

			if ((instr >> 6) & B0001)
			{
				p->op = MSP430_OP_SUBB;
				p->fptr = (void *)msp430_subb;
			}
			else
			{
				p->op = MSP430_OP_SUB;
				p->fptr = (void *)msp430_sub;
			}
			p->cycles = ilat(E, S, p->format, p->op, p->amode_d, p->amode_s, instr);

			break;
		}
		case 0x9:
		{
			p->format = MSP430_INSTR_I;
			p->amode_s = smode(E, S, p->format, p->instr);
			p->amode_d = dmode(E, S, p->format, p->instr);
			p->ilen = ilen(E, S, p->format, p->amode_d, p->amode_s);

			if ((instr >> 6) & B0001)
			{
				p->op = MSP430_OP_CMPB;
				p->fptr = (void *)msp430_cmpb;
			}
			else
			{
				p->op = MSP430_OP_CMP;
				p->fptr = (void *)msp430_cmp;
			}
			p->cycles = ilat(E, S, p->format, p->op, p->amode_d, p->amode_s, instr);

			break;
		}
		case 0xA:
		{
			p->format = MSP430_INSTR_I;
			p->amode_s = smode(E, S, p->format, p->instr);
			p->amode_d = dmode(E, S, p->format, p->instr);
			p->ilen = ilen(E, S, p->format, p->amode_d, p->amode_s);

			if ((instr >> 6) & B0001)
			{
				p->op = MSP430_OP_DADDB;
				p->fptr = (void *)msp430_daddb;
			}
			else
			{
				p->op = MSP430_OP_DADD;
				p->fptr = (void *)msp430_dadd;
			}
			p->cycles = ilat(E, S, p->format, p->op, p->amode_d, p->amode_s, instr);

			break;
		}
		case 0xB:
		{
			p->format = MSP430_INSTR_I;
			p->amode_s = smode(E, S, p->format, p->instr);
			p->amode_d = dmode(E, S, p->format, p->instr);
			p->ilen = ilen(E, S, p->format, p->amode_d, p->amode_s);

			if ((instr >> 6) & B0001)
			{
				p->op = MSP430_OP_BITB;
				p->fptr = (void *)msp430_bitb;
			}
			else
			{
				p->op = MSP430_OP_BIT;
				p->fptr = (void *)msp430_bit;
			}
			p->cycles = ilat(E, S, p->format, p->op, p->amode_d, p->amode_s, instr);

			break;
		}

		case 0xC:
		{
			p->format = MSP430_INSTR_I;
			p->amode_s = smode(E, S, p->format, p->instr);
			p->amode_d = dmode(E, S, p->format, p->instr);
			p->ilen = ilen(E, S, p->format, p->amode_d, p->amode_s);

			if ((instr >> 6) & B0001)
			{
				p->op = MSP430_OP_BICB;
				p->fptr = (void *)msp430_bicb;
			}
			else
			{
				p->op = MSP430_OP_BIC;
				p->fptr = (void *)msp430_bic;
			}
			p->cycles = ilat(E, S, p->format, p->op, p->amode_d, p->amode_s, instr);

			break;
		}
		case 0xD:
		{
			p->format = MSP430_INSTR_I;
			p->amode_s = smode(E, S, p->format, p->instr);
			p->amode_d = dmode(E, S, p->format, p->instr);
			p->ilen = ilen(E, S, p->format, p->amode_d, p->amode_s);

			if ((instr >> 6) & B0001)
			{
				p->op = MSP430_OP_BISB;
				p->fptr = (void *)msp430_bisb;
			}
			else
			{
				p->op = MSP430_OP_BIS;
				p->fptr = (void *)msp430_bis;
			}
			p->cycles = ilat(E, S, p->format, p->op, p->amode_d, p->amode_s, instr);

			break;
		}
		case 0xE:
		{
			p->format = MSP430_INSTR_I;
			p->amode_s = smode(E, S, p->format, p->instr);
			p->amode_d = dmode(E, S, p->format, p->instr);
			p->ilen = ilen(E, S, p->format, p->amode_d, p->amode_s);

			if ((instr >> 6) & B0001)
			{
				p->op = MSP430_OP_XORB;
				p->fptr = (void *)msp430_xorb;
			}
			else
			{
				p->op = MSP430_OP_XOR;
				p->fptr = (void *)msp430_xor;
			}
			p->cycles = ilat(E, S, p->format, p->op, p->amode_d, p->amode_s, instr);

			break;
		}
		case 0xF:
		{
			p->format = MSP430_INSTR_I;
			p->amode_s = smode(E, S, p->format, p->instr);
			p->amode_d = dmode(E, S, p->format, p->instr);
			p->ilen = ilen(E, S, p->format, p->amode_d, p->amode_s);

			if ((instr >> 6) & B0001)
			{
				p->op = MSP430_OP_ANDB;
				p->fptr = (void *)msp430_andb;
			}
			else
			{
				p->op = MSP430_OP_AND;
				p->fptr = (void *)msp430_and;
			}
			p->cycles = ilat(E, S, p->format, p->op, p->amode_d, p->amode_s, instr);

			break;
		}
		default:
		{
			/*	No instructions mapped to this opcode	*/
			mprint(E, NULL, siminfo,
				"Illegal instruction encoding in msp430decode(), [%4X]\n", instr);
			sfatal(E, S, "See above messages.");
		}
	}

	return;
}
