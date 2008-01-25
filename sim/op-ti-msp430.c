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

#include <stdlib.h>
#include "instr-ti-msp430.h"
#include "sf.h"
#include "mextern.h"

#include "regaccess-ti-msp430.c"
#include "mem-ti-msp430.c"


enum
{
	FIRST_OP	= 1,
	SECOND_OP	= 2,
	WORDINSTR	= 1,
	BYTEINSTR	= 0
};

/*
		Decode should set the As and Ad felds of the pipe,
		in addition to setting the amode_s and amode_d.
		the former are needed in regaccess, and the latter
		are implicitly contain information about whether
		the register being used is r0 or r2, and is
		generally easier to reson about
*/	



/*
	TODO: check all setting of overflow flag with rules defined 
	in table 3-1 of slau049e.pdf
*/


static tuck ushort
getval(Engine *E, State *S, int mode, int regnum, int whichop, int iswordinstr, MSP430Pipestage *p)
{
	switch (mode)
	{
		case MSP430_AMODE_REG:
		{
			return msp430regread(S, regnum, p);
		}

		case MSP430_AMODE_IDX:
		case MSP430_AMODE_SYM:
		case MSP430_AMODE_ABS:
		{
			int	X = msp430readword(S,
					msp430regread(S, MSP430_PC, p) + (1 << whichop));

		/*
			BUG ?: If you look at example in slau049e.pdf, 3.3.3, it looks like 
			the value should be X + PC + (1 << whichop) in the case of IDX mode
			i.e., we don't use the PC as the base address for the register indirect,
			but rather add the address at which the offset resides (the memory 
			address of X) to X (at least in the case of IDX mode). I think the 
			manual is wrong, since it contradicts itself right there in in 3.3.3 
			saying the addresses are X + PC and Y+PC, and not of X+&X, Y+&Y... ???  
			(similar thing is done in 3.3.7)...
		*/

			if (iswordinstr)
			{
				return msp430readword(S, msp430regread(S, regnum, p) + X);
			}
			else
			{
				return msp430readbyte(S, msp430regread(S, regnum, p) + X);
			}
		}

		case MSP430_AMODE_IND:
		{
			if (iswordinstr)
			{
				return msp430readword(S, msp430regread(S, regnum, p));
			}
			else
			{
				return msp430readbyte(S, msp430regread(S, regnum, p));
			}
		}

		case MSP430_AMODE_INC:
		{
			ushort	addr = msp430regread(S, regnum, p);

			msp430regset(S, regnum, msp430regread(S, regnum, p) + (1 << iswordinstr));

			if (iswordinstr)
			{
				return msp430readword(S, addr);
			}
			else
			{
				return msp430readbyte(S, addr);
			}
		}

		case MSP430_AMODE_IMM:
		{
			if (iswordinstr)
			{
				return msp430readword(S,
					msp430regread(S, MSP430_PC, p)+ (1 << whichop));
			}
			else
			{
				return msp430readbyte(S,
					msp430regread(S, MSP430_PC, p)+ (1 << whichop));
			}
		}

		default:
		{
			mprint(E, S, nodeinfo, "Illegal addressing mode [%4X] in getval()\n", mode);
			sfatal(E, S, "See above messages.");
		}
	}

	/*	Not Reached	*/
	return -1;
}

static tuck void
setval(Engine *E, State *S, int mode, int regnum, int whichop, int iswordinstr, ushort data, MSP430Pipestage *p)
{
	switch (mode)
	{
		case MSP430_AMODE_REG:
		{
			msp430regset(S, regnum, data);
		}

		case MSP430_AMODE_IDX:
		case MSP430_AMODE_SYM:
		case MSP430_AMODE_ABS:
		{
			int X = msp430readword(S, msp430regread(S, MSP430_PC, p) + (1 << whichop));
			if (iswordinstr)
			{
				msp430writeword(S, msp430regread(S, regnum, p) + X, data);
			}
			else
			{
				msp430writebyte(S, msp430regread(S, regnum, p) + X, data);
			}
		}

		default:
		{
			mprint(E, S, nodeinfo, "Illegal addressing mode [%4X] in setval()\n", mode);
			sfatal(E, S, "See above messages.");
		}
	}

	return;
}




/*												*/
/*		This implementation and many of the notes herein are based			*/
/*		on the descriptions in slau049d.pdf from TI, the MSP430x1xx			*/
/*		family user guide.								*/
/*												*/

/*												*/
/*				Double-Operand (FORMAT I) instructions				*/
/*	15		      12 11		       8    7     6   5    4   3  2  1  0	*/
/*	|	op-code		|	    S-reg	|  Ad  | B/W |  As   |    D-reg		*/
/*												*/
/*												*/
/*	Mnemonic	S-Reg		Operation 			Status Bits		*/
/*			D-Reg					V	N	Z	C	*/
/*	----------------------------------------------------------------------------------	*/
/*	MOV(.B)		src,dst 	src -> dst						*/
/*	ADD(.B)		src,dst		src + dst -> dst 	*	*	*	*	*/
/*	ADDC(.B)	src,dst		src + dst + C -> dst	*	*	*	*	*/
/*	SUB(.B)		src,dst		dst + !src + 1 -> dst	*	*	*	*	*/
/*	SUBC(.B)	src,dst		dst + !src + C -> dst	*	*	*	*	*/
/*	CMP(.B)		src,dst		dst  src		*	*	*	*	*/
/*	DADD(.B)	src,dst	src + dst + C -> dst (decimally) *	*	*	*	*/
/*	BIT(.B)		src,dst		src .and. dst		0	*	*	*	*/
/*	BIC(.B)		src,dst	!src .and. dst -> dst						*/
/*	BIS(.B)		src,dst		src .or. dst -> dst					*/
/*	XOR(.B)		src,dst		src .xor. dst -> dst	*	*	*	*	*/
/*	AND(.B)		src,dst		src .and. dst -> dst	0	*	*	*	*/
/*												*/
/*	Legend:											*/
/*	*	The status bit is affected							*/
/*		The status bit is not affected							*/
/*	0	The status bit is cleared							*/
/*	1	The status bit is set								*/
/*												*/

/*												*/
/*	NOTE: ilen is the instruction length in _bytes_. We recorded the length in bytes	*/
/*	when we decoded (manual specifies the length in _words_).				*/
/*												*/

tuck void
msp430_mov(State *S, ushort m, ushort n, MSP430Pipestage *p)
{
	setval(S, p->amode_d, n, SECOND_OP, WORDINSTR,
		getval(S, p->amode_s, m, FIRST_OP, WORDINSTR, p), p);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);

	return;
}

tuck void
msp430_movb(State *S, ushort m, ushort n, MSP430Pipestage *p)
{
	setval(S, p->amode_d, n, SECOND_OP, BYTEINSTR,
		getval(S, p->amode_s, m, FIRST_OP, BYTEINSTR, p) & 0xFF, p);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);

	return;
}

tuck void
msp430_add(State *S, ushort m, ushort n, MSP430Pipestage *p)
{
	ushort	tmpSR, orig_dst, orig_src, result;


	tmpSR = msp430regread(S, MSP430_SR, p);
	msp430_sreg_clr_C(tmpSR);
	msp430_sreg_clr_Z(tmpSR);
	msp430_sreg_clr_N(tmpSR);
	msp430_sreg_clr_V(tmpSR);
	msp430regset(S, MSP430_SR, tmpSR);

	orig_src = getval(S, p->amode_s, m, FIRST_OP, WORDINSTR, p);
	orig_dst = getval(S, p->amode_d, n, SECOND_OP, WORDINSTR, p);
	result = orig_src + orig_dst;
	setval(S, p->amode_d, n, SECOND_OP, WORDINSTR, result, p);

	tmpSR = msp430regread(S, MSP430_SR, p);
	if ((result >> 15) & B0001)
	{
		msp430_sreg_set_N(tmpSR);
	}
	if (result == 0)
	{
		msp430_sreg_set_Z(tmpSR);
	}
	if ((short)orig_dst > (short)result)
	{
		msp430_sreg_set_C(tmpSR);
	}
	if (((orig_src >> 15) && (orig_dst >> 15) && !(result >> 15)) ||
		(!(orig_src >> 15) && !(orig_dst >> 15) && (result >> 15))
	)
	{
		msp430_sreg_set_V(tmpSR);
	}
	msp430regset(S, MSP430_SR, tmpSR);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}

tuck void
msp430_addb(State *S, int m, int n, MSP430Pipestage *p)
{
	uchar	tmpSR, orig_dst, orig_src, result;


	tmpSR = msp430regread(S, MSP430_SR, p);
	msp430_sreg_clr_C(tmpSR);
	msp430_sreg_clr_Z(tmpSR);
	msp430_sreg_clr_N(tmpSR);
	msp430_sreg_clr_V(tmpSR);
	msp430regset(S, MSP430_SR, tmpSR);

	orig_src = getval(S, p->amode_s, m, FIRST_OP, BYTEINSTR, p) & 0xFF;
	orig_dst = getval(S, p->amode_d, n, SECOND_OP, BYTEINSTR, p) & 0xFF;
	result = orig_src + orig_dst;
	setval(S, p->amode_d, n, SECOND_OP, BYTEINSTR, result, p);

	tmpSR = msp430regread(S, MSP430_SR, p);
	if ((result >> 7) & B0001)
	{
		msp430_sreg_set_N(tmpSR);
	}
	if (result == 0)
	{
		msp430_sreg_set_Z(tmpSR);
	}
	if ((char)orig_dst > (char)result)
	{
		msp430_sreg_set_C(tmpSR);
	}
	if (((orig_src >> 7) && (orig_dst >> 7) && !(result >> 7)) ||
		(!(orig_src >> 7) && !(orig_dst >> 7) && (result >> 7))
	)
	{
		msp430_sreg_set_V(tmpSR);
	}
	msp430regset(S, MSP430_SR, tmpSR);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}

tuck void
msp430_addc(State *S, int m, int n, MSP430Pipestage *p)
{
	ushort	tmpSR, orig_dst, orig_src, result, C;


	tmpSR = msp430regread(S, MSP430_SR, p);
	C = msp430_sreg_get_C(tmpSR);
	msp430_sreg_clr_C(tmpSR);
	msp430_sreg_clr_Z(tmpSR);
	msp430_sreg_clr_N(tmpSR);
	msp430_sreg_clr_V(tmpSR);
	msp430regset(S, MSP430_SR, tmpSR);

	orig_src = getval(S, p->amode_s, m, FIRST_OP, WORDINSTR, p);
	orig_dst = getval(S, p->amode_d, n, SECOND_OP, WORDINSTR, p);
	result = orig_src + orig_dst + C;
	setval(S, p->amode_d, n, SECOND_OP, WORDINSTR, result, p);

	tmpSR = msp430regread(S, MSP430_SR, p);
	if ((result >> 15) & B0001)
	{
		msp430_sreg_set_N(tmpSR);
	}
	if (result == 0)
	{
		msp430_sreg_set_Z(tmpSR);
	}
	if ((short)orig_dst > (short)result)
	{
		msp430_sreg_set_C(tmpSR);
	}
	if (((orig_src >> 15) && (orig_dst >> 15) && !(result >> 15)) ||
		(!(orig_src >> 15) && !(orig_dst >> 15) && (result >> 15))
	)
	{
		msp430_sreg_set_V(tmpSR);
	}
	msp430regset(S, MSP430_SR, tmpSR);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}

tuck void
msp430_addcb(State *S, int m, int n, MSP430Pipestage *p)
{
	ushort	tmpSR;
	uchar	orig_dst, orig_src, result, C;


	tmpSR = msp430regread(S, MSP430_SR, p);
	C = msp430_sreg_get_C(tmpSR);
	msp430_sreg_clr_C(tmpSR);
	msp430_sreg_clr_Z(tmpSR);
	msp430_sreg_clr_N(tmpSR);
	msp430_sreg_clr_V(tmpSR);
	msp430regset(S, MSP430_SR, tmpSR);

	orig_src = getval(S, p->amode_s, m, FIRST_OP, BYTEINSTR, p) & 0xFF;
	orig_dst = getval(S, p->amode_d, n, SECOND_OP, BYTEINSTR, p) & 0xFF;
	result = orig_src + orig_dst + C;
	setval(S, p->amode_d, n, SECOND_OP, BYTEINSTR, result, p);

	tmpSR = msp430regread(S, MSP430_SR, p);
	if ((result >> 7) & B0001)
	{
		msp430_sreg_set_N(tmpSR);
	}
	if (result == 0)
	{
		msp430_sreg_set_Z(tmpSR);
	}
	if ((char)orig_dst > (char)result)
	{
		msp430_sreg_set_C(tmpSR);
	}
	if (((orig_src >> 7) && (orig_dst >> 7) && !(result >> 7)) ||
		(!(orig_src >> 7) && !(orig_dst >> 7) && (result >> 7))
	)
	{
		msp430_sreg_set_V(tmpSR);
	}
	msp430regset(S, MSP430_SR, tmpSR);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}

tuck void
msp430_sub(State *S, int m, int n, MSP430Pipestage *p)
{
	ushort	tmpSR, orig_dst, orig_src, result;


	tmpSR = msp430regread(S, MSP430_SR, p);
	msp430_sreg_clr_C(tmpSR);
	msp430_sreg_clr_Z(tmpSR);
	msp430_sreg_clr_N(tmpSR);
	msp430_sreg_clr_V(tmpSR);
	msp430regset(S, MSP430_SR, tmpSR);

	orig_src = getval(S, p->amode_s, m, FIRST_OP, WORDINSTR, p);
	orig_dst = getval(S, p->amode_d, n, SECOND_OP, WORDINSTR, p);
	result = orig_dst + !orig_src + 1;
	setval(S, p->amode_d, n, SECOND_OP, WORDINSTR, result, p);

	tmpSR = msp430regread(S, MSP430_SR, p);
	if ((result >> 15) & B0001)
	{
		msp430_sreg_set_N(tmpSR);
	}
	if (result == 0)
	{
		msp430_sreg_set_Z(tmpSR);
	}
	if ((short)orig_dst < (short)result)
	{
		msp430_sreg_set_C(tmpSR);
	}
	if ((!(orig_dst >> 15) && (orig_src >> 15) && (result >> 15)) ||
		((orig_dst >> 15) && !(orig_src >> 15) && !(result >> 15))
	)
	{
		msp430_sreg_set_V(tmpSR);
	}
	msp430regset(S, MSP430_SR, tmpSR);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}

tuck void
msp430_subb(State *S, int m, int n, MSP430Pipestage *p)
{
	ushort	tmpSR;
	uchar	orig_dst, orig_src, result;


	tmpSR = msp430regread(S, MSP430_SR, p);
	msp430_sreg_clr_C(tmpSR);
	msp430_sreg_clr_Z(tmpSR);
	msp430_sreg_clr_N(tmpSR);
	msp430_sreg_clr_V(tmpSR);
	msp430regset(S, MSP430_SR, tmpSR);

	orig_src = getval(S, p->amode_s, m, FIRST_OP, BYTEINSTR, p) & 0xFF;
	orig_dst = getval(S, p->amode_d, n, SECOND_OP, BYTEINSTR, p) & 0xFF;
	result = orig_dst + !orig_src + 1;
	setval(S, p->amode_d, n, SECOND_OP, BYTEINSTR, result, p);

	tmpSR = msp430regread(S, MSP430_SR, p);
	if ((result >> 7) & B0001)
	{
		msp430_sreg_set_N(tmpSR);
	}
	if (result == 0)
	{
		msp430_sreg_set_Z(tmpSR);
	}
	if ((char)orig_dst < (char)result)
	{
		msp430_sreg_set_C(tmpSR);
	}
	if ((!(orig_src >> 7) && (orig_dst >> 7) && (result >> 7)) ||
		((orig_src >> 7) && !(orig_dst >> 7) && !(result >> 7))
	)
	{
		msp430_sreg_set_V(tmpSR);
	}
	msp430regset(S, MSP430_SR, tmpSR);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}

tuck void
msp430_subc(State *S, int m, int n, MSP430Pipestage *p)
{
	ushort	tmpSR, orig_dst, orig_src, result, C;


	tmpSR = msp430regread(S, MSP430_SR, p);
	C = msp430_sreg_get_C(tmpSR);
	msp430_sreg_clr_C(tmpSR);
	msp430_sreg_clr_Z(tmpSR);
	msp430_sreg_clr_N(tmpSR);
	msp430_sreg_clr_V(tmpSR);
	msp430regset(S, MSP430_SR, tmpSR);

	orig_src = getval(S, p->amode_s, m, FIRST_OP, WORDINSTR, p);
	orig_dst = getval(S, p->amode_d, n, SECOND_OP, WORDINSTR, p);
	result = orig_dst + !orig_src + 1 + C;
	setval(S, p->amode_d, n, SECOND_OP, WORDINSTR, result, p);

	tmpSR = msp430regread(S, MSP430_SR, p);
	if ((result >> 15) & B0001)
	{
		msp430_sreg_set_N(tmpSR);
	}
	if (result == 0)
	{
		msp430_sreg_set_Z(tmpSR);
	}
	if ((short)orig_dst < (short)result)
	{
		msp430_sreg_set_C(tmpSR);
	}
	if ((!(orig_dst >> 15) && (orig_src >> 15) && (result >> 15)) ||
		((orig_dst >> 15) && !(orig_src >> 15) && !(result >> 15))
	)
	{
		msp430_sreg_set_V(tmpSR);
	}
	msp430regset(S, MSP430_SR, tmpSR);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}

tuck void
msp430_subcb(State *S, int m, int n, MSP430Pipestage *p)
{
	ushort	tmpSR;
	uchar	orig_dst, orig_src, result, C;


	tmpSR = msp430regread(S, MSP430_SR, p);
	C = msp430_sreg_get_C(tmpSR);
	msp430_sreg_clr_C(tmpSR);
	msp430_sreg_clr_Z(tmpSR);
	msp430_sreg_clr_N(tmpSR);
	msp430_sreg_clr_V(tmpSR);
	msp430regset(S, MSP430_SR, tmpSR);

	orig_src = getval(S, p->amode_s, m, FIRST_OP, BYTEINSTR, p) & 0xFF;
	orig_dst = getval(S, p->amode_d, n, SECOND_OP, BYTEINSTR, p) & 0xFF;
	result = orig_dst + !orig_src + 1;
	setval(S, p->amode_d, n, SECOND_OP, BYTEINSTR, result, p);

	tmpSR = msp430regread(S, MSP430_SR, p);
	if ((result >> 7) & B0001)
	{
		msp430_sreg_set_N(tmpSR);
	}
	if (result == 0)
	{
		msp430_sreg_set_Z(tmpSR);
	}
	if ((char)orig_dst < (char)result)
	{
		msp430_sreg_set_C(tmpSR);
	}
	if ((!(orig_src >> 7) && (orig_dst >> 7) && (result >> 7)) ||
		((orig_src >> 7) && !(orig_dst >> 7) && !(result >> 7))
	)
	{
		msp430_sreg_set_V(tmpSR);
	}
	msp430regset(S, MSP430_SR, tmpSR);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}

tuck void
msp430_cmp(State *S, int m, int n, MSP430Pipestage *p)
{
	ushort	tmpSR, orig_dst, orig_src, result;
	int	dsign = 1, ssign = 1, asign = 1;


	tmpSR = msp430regread(S, MSP430_SR, p);
	msp430_sreg_clr_C(tmpSR);
	msp430_sreg_clr_Z(tmpSR);
	msp430_sreg_clr_N(tmpSR);
	msp430_sreg_clr_V(tmpSR);
	msp430regset(S, MSP430_SR, tmpSR);

	orig_src = getval(S, p->amode_s, m, FIRST_OP, WORDINSTR, p);
	orig_dst = getval(S, p->amode_d, n, SECOND_OP, WORDINSTR, p);
	result = !orig_src + 1 + orig_dst;
	setval(S, p->amode_d, n, SECOND_OP, WORDINSTR, result, p);

	tmpSR = msp430regread(S, MSP430_SR, p);
	if ((result >> 15) & B0001)
	{
		msp430_sreg_set_N(tmpSR);
	}
	if (result == 0)
	{
		msp430_sreg_set_Z(tmpSR);
	}
	if ((short)orig_dst > (short)result)
	{
		msp430_sreg_set_C(tmpSR);
	}
	if ((short)orig_src >= 0)
	{
		ssign = 0;
	}
	if ((short)orig_dst >= 0)
	{
		dsign = 0;
	}
	if ((short)result >= 0)
	{
		asign = 0;
	}
	if ((ssign == dsign) && (asign != ssign))
	{
		msp430_sreg_set_V(tmpSR);
	}
	msp430regset(S, MSP430_SR, tmpSR);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}

tuck void
msp430_cmpb(State *S, int m, int n, MSP430Pipestage *p)
{
	uchar	orig_dst, orig_src, result;
	int	dsign = 1, ssign = 1, asign = 1;
	ushort	tmpSR;


	tmpSR = msp430regread(S, MSP430_SR, p);
	msp430_sreg_clr_C(tmpSR);
	msp430_sreg_clr_Z(tmpSR);
	msp430_sreg_clr_N(tmpSR);
	msp430_sreg_clr_V(tmpSR);
	msp430regset(S, MSP430_SR, tmpSR);

	orig_src = getval(S, p->amode_s, m, FIRST_OP, BYTEINSTR, p) & 0xFF;
	orig_dst = getval(S, p->amode_d, n, SECOND_OP, BYTEINSTR, p) & 0xFF;
	result = !orig_src + 1 + orig_dst;
	setval(S, p->amode_d, n, SECOND_OP, BYTEINSTR, result, p);

	tmpSR = msp430regread(S, MSP430_SR, p);
	if ((result >> 7) & B0001)
	{
		msp430_sreg_set_N(tmpSR);
	}
	if (result == 0)
	{
		msp430_sreg_set_Z(tmpSR);
	}
	if ((char)orig_dst > (char)result)
	{
		msp430_sreg_set_C(tmpSR);
	}
	if ((char)orig_src >= 0)
	{
		ssign = 0;
	}
	if ((char)orig_dst >= 0)
	{
		dsign = 0;
	}
	if ((char)result >= 0)
	{
		asign = 0;
	}
	if ((ssign == dsign) && (asign != ssign))
	{
		msp430_sreg_set_V(tmpSR);
	}
	msp430regset(S, MSP430_SR, tmpSR);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}

tuck void
msp430_dadd(State *S, int m, int n, MSP430Pipestage *p)
{
	ushort	tmpSR, result, orig_dst, orig_src, C;
	int	src_dec, dst_dec, result_dec, d1, d2, d3, d4;


	tmpSR = msp430regread(S, MSP430_SR, p);
	C = msp430_sreg_get_C(tmpSR);
	msp430_sreg_clr_C(tmpSR);
	msp430_sreg_clr_Z(tmpSR);
	msp430_sreg_clr_N(tmpSR);
	msp430regset(S, MSP430_SR, tmpSR);

	orig_src = getval(S, p->amode_s, m, FIRST_OP, WORDINSTR, p);
	orig_dst = getval(S, p->amode_d, n, SECOND_OP, WORDINSTR, p);

	/*	 B -> BCD	*/
	src_dec = (orig_src & 0xF) +
		((orig_src >> 4) & 0xF)*10 +
		((orig_src >> 8) & 0xF)*100 +
		((orig_src >> 12) & 0xF)*1000;
	dst_dec = (orig_dst & 0xF) +
		((orig_dst >> 4) & 0xF)*10 +
		((orig_dst >> 8) & 0xF)*100 +
		((orig_dst >> 12) & 0xF)*1000;
	result_dec = src_dec + dst_dec + C;

	/*	  BCD -> B 	*/
	d4 = (result_dec/1000);	if (d4 > 0) result_dec -= d4*1000;
	d3 = (result_dec/100);	if (d3 > 0) result_dec -= d3*100;
	d2 = (result_dec/10);	if (d2 > 0) result_dec -= d2*10;
	d1 = result_dec;
	result = (d4 << 12) | (d3 << 8) | (d2 << 4) | d1;
	setval(S, p->amode_d, n, SECOND_OP, WORDINSTR, result, p);

	tmpSR = msp430regread(S, MSP430_SR, p);
	if ((result >> 15) & B0001)
	{
		msp430_sreg_set_N(tmpSR);
	}
	if (result == 0)
	{
		msp430_sreg_set_Z(tmpSR);
	}
	if (result_dec > 9999)
	{
		msp430_sreg_set_C(tmpSR);
	}
	msp430regset(S, MSP430_SR, tmpSR);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}

tuck void
msp430_daddb(State *S, int m, int n, MSP430Pipestage *p)
{
	uchar	result, orig_dst, orig_src;
	int	src_dec, dst_dec, result_dec, d1, d2;
	ushort	C, tmpSR;


	tmpSR = msp430regread(S, MSP430_SR, p);
	C = msp430_sreg_get_C(tmpSR);
	msp430_sreg_clr_C(tmpSR);
	msp430_sreg_clr_Z(tmpSR);
	msp430_sreg_clr_N(tmpSR);
	msp430regset(S, MSP430_SR, tmpSR);

	orig_src = getval(S, p->amode_s, m, FIRST_OP, BYTEINSTR, p) & 0xFF;
	orig_dst = getval(S, p->amode_d, n, SECOND_OP, BYTEINSTR, p) & 0xFF;

	/*	 B -> BCD	*/
	src_dec = (orig_src & 0xF) +
		((orig_src >> 4) & 0xF)*10;
	dst_dec = (orig_dst & 0xF) +
		((orig_dst >> 4) & 0xF)*10;
	result_dec = src_dec + dst_dec + C;

	/*	  BCD -> B 	*/
	d2 = (result_dec/10);	if (d2 > 0) result_dec -= d2*10;
	d1 = result_dec;
	result = (d2 << 4) | d1;
	setval(S, p->amode_d, n, SECOND_OP, BYTEINSTR, result, p);

	tmpSR = msp430regread(S, MSP430_SR, p);
	if ((result >> 15) & B0001)
	{
		msp430_sreg_set_N(tmpSR);
	}
	if (result == 0)
	{
		msp430_sreg_set_Z(tmpSR);
	}
	if (result_dec > 99)
	{
		msp430_sreg_set_C(tmpSR);
	}
	msp430regset(S, MSP430_SR, tmpSR);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}

tuck void
msp430_bit(State *S, int m, int n, MSP430Pipestage *p)
{
	ushort	tmpSR, result;


	tmpSR = msp430regread(S, MSP430_SR, p);
	msp430_sreg_clr_C(tmpSR);
	msp430_sreg_clr_Z(tmpSR);
	msp430_sreg_clr_N(tmpSR);
	msp430_sreg_clr_V(tmpSR);
	msp430regset(S, MSP430_SR, tmpSR);

	result = getval(S, p->amode_s, m, FIRST_OP, WORDINSTR, p) &
		getval(S, p->amode_d, n, SECOND_OP, WORDINSTR, p);

	tmpSR = msp430regread(S, MSP430_SR, p);
	if ((result >> 15) & B0001)
	{
		msp430_sreg_set_N(tmpSR);
	}
	if (result == 0)
	{
		msp430_sreg_set_Z(tmpSR);
	}
	if (!msp430_sreg_get_Z(tmpSR))
	{
		msp430_sreg_set_C(tmpSR);
	}
	msp430regset(S, MSP430_SR, tmpSR);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}

tuck void
msp430_bitb(State *S, int m, int n, MSP430Pipestage *p)
{
	uchar	result;
	ushort	tmpSR;


	tmpSR = msp430regread(S, MSP430_SR, p);
	msp430_sreg_clr_C(tmpSR);
	msp430_sreg_clr_Z(tmpSR);
	msp430_sreg_clr_N(tmpSR);
	msp430_sreg_clr_V(tmpSR);
	msp430regset(S, MSP430_SR, tmpSR);

	result = (getval(S, p->amode_s, m, FIRST_OP, BYTEINSTR, p) & 0xFF) &
		(getval(S, p->amode_d, n, SECOND_OP, BYTEINSTR, p) & 0xFF);

	tmpSR = msp430regread(S, MSP430_SR, p);
	if ((result >> 7) & B0001)
	{
		msp430_sreg_set_N(tmpSR);
	}
	if (result == 0)
	{
		msp430_sreg_set_Z(tmpSR);
	}
	if (!msp430_sreg_get_Z(tmpSR))
	{
		msp430_sreg_set_C(tmpSR);
	}
	msp430regset(S, MSP430_SR, tmpSR);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}

tuck void
msp430_bic(State *S, int m, int n, MSP430Pipestage *p)
{
	ushort	result;

	result = !getval(S, p->amode_s, m, FIRST_OP, WORDINSTR, p) &
			getval(S, p->amode_d, n, SECOND_OP, WORDINSTR, p);
	setval(S, p->amode_d, n, SECOND_OP, WORDINSTR, result, p);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);

	return;
}

tuck void
msp430_bicb(State *S, int m, int n, MSP430Pipestage *p)
{
	uchar	result;

	result = !(getval(S, p->amode_s, m, FIRST_OP, BYTEINSTR, p) & 0xFF) &
		(getval(S, p->amode_d, n, SECOND_OP, BYTEINSTR, p) & 0xFF);
	setval(S, p->amode_d, n, SECOND_OP, BYTEINSTR, result, p);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}

tuck void
msp430_bis(State *S, int m, int n, MSP430Pipestage *p)
{
	ushort	result;

	result = !getval(S, p->amode_s, m, FIRST_OP, WORDINSTR, p) | 
		getval(S, p->amode_d, n, SECOND_OP, WORDINSTR, p);
	setval(S, p->amode_d, n, SECOND_OP, WORDINSTR, result, p);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);

	return;
}

tuck void
msp430_bisb(State *S, int m, int n, MSP430Pipestage *p)
{
	uchar	result;

	result = !(getval(S, p->amode_s, m, FIRST_OP, BYTEINSTR, p) & 0xFF) |
		(getval(S, p->amode_d, n, SECOND_OP, BYTEINSTR, p) & 0xFF);
	setval(S, p->amode_d, n, SECOND_OP, BYTEINSTR, result, p);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}

tuck void
msp430_xor(State *S, int m, int n, MSP430Pipestage *p)
{
	ushort	tmpSR, orig_src, orig_dst, result;


	tmpSR = msp430regread(S, MSP430_SR, p);
	msp430_sreg_clr_C(tmpSR);
	msp430_sreg_clr_Z(tmpSR);
	msp430_sreg_clr_N(tmpSR);
	msp430_sreg_clr_V(tmpSR);
	msp430regset(S, MSP430_SR, tmpSR);

	orig_src = getval(S, p->amode_s, m, FIRST_OP, WORDINSTR, p);
	orig_dst = getval(S, p->amode_d, n, SECOND_OP, WORDINSTR, p);
	result = orig_src ^ orig_dst;
	setval(S, p->amode_d, n, SECOND_OP, WORDINSTR, result, p);

	tmpSR = msp430regread(S, MSP430_SR, p);
	if ((short)orig_src < 0 && (short)orig_dst < 0)
	{
		msp430_sreg_set_V(tmpSR);
	}
	if ((result >> 15) & B0001)
	{
		msp430_sreg_set_N(tmpSR);
	}
	if (result == 0)
	{
		msp430_sreg_set_Z(tmpSR);
	}
	if (!msp430_sreg_get_Z(tmpSR))
	{
		msp430_sreg_set_C(tmpSR);
	}
	msp430regset(S, MSP430_SR, tmpSR);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}


tuck void
msp430_xorb(State *S, int m, int n, MSP430Pipestage *p)
{
	uchar	tmpSR, orig_src, orig_dst, result;


	tmpSR = msp430regread(S, MSP430_SR, p);
	msp430_sreg_clr_C(tmpSR);
	msp430_sreg_clr_Z(tmpSR);
	msp430_sreg_clr_N(tmpSR);
	msp430_sreg_clr_V(tmpSR);
	msp430regset(S, MSP430_SR, tmpSR);

	orig_src = getval(S, p->amode_s, m, FIRST_OP, BYTEINSTR, p) & 0xFF;
	orig_dst = getval(S, p->amode_d, n, SECOND_OP, BYTEINSTR, p) & 0xFF;
	result = orig_src ^ orig_dst;
	setval(S, p->amode_d, n, SECOND_OP, BYTEINSTR, result, p);

	tmpSR = msp430regread(S, MSP430_SR, p);
	if ((char)orig_src < 0 && (char)orig_dst < 0)
	{
		msp430_sreg_set_V(tmpSR);
	}
	if ((result >> 7) & B0001)
	{
		msp430_sreg_set_N(tmpSR);
	}
	if (result == 0)
	{
		msp430_sreg_set_Z(tmpSR);
	}
	if (!msp430_sreg_get_Z(tmpSR))
	{
		msp430_sreg_set_C(tmpSR);
	}
	msp430regset(S, MSP430_SR, tmpSR);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}

tuck void
msp430_and(State *S, int m, int n, MSP430Pipestage *p)
{
	ushort	tmpSR, result;


	tmpSR = msp430regread(S, MSP430_SR, p);
	msp430_sreg_clr_C(tmpSR);
	msp430_sreg_clr_Z(tmpSR);
	msp430_sreg_clr_N(tmpSR);
	msp430_sreg_clr_V(tmpSR);
	msp430regset(S, MSP430_SR, tmpSR);

	result = getval(S, p->amode_s, m, FIRST_OP, WORDINSTR, p) &
			getval(S, p->amode_d, n, SECOND_OP, WORDINSTR, p);
	setval(S, p->amode_d, n, SECOND_OP, WORDINSTR, result, p);

	tmpSR = msp430regread(S, MSP430_SR, p);
	if ((result >> 15) & B0001)
	{
		msp430_sreg_set_N(tmpSR);
	}
	if (result == 0)
	{
		msp430_sreg_set_Z(tmpSR);
	}
	if (!msp430_sreg_get_Z(tmpSR))
	{
		msp430_sreg_set_C(tmpSR);
	}
	msp430regset(S, MSP430_SR, tmpSR);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}

tuck void
msp430_andb(State *S, int m, int n, MSP430Pipestage *p)
{
	uchar	result;
	ushort	tmpSR;


	tmpSR = msp430regread(S, MSP430_SR, p);
	msp430_sreg_clr_C(tmpSR);
	msp430_sreg_clr_Z(tmpSR);
	msp430_sreg_clr_N(tmpSR);
	msp430_sreg_clr_V(tmpSR);
	msp430regset(S, MSP430_SR, tmpSR);

	result = (getval(S, p->amode_s, m, FIRST_OP, BYTEINSTR, p) & 0xFF) &
			(getval(S, p->amode_d, n, SECOND_OP, BYTEINSTR, p) & 0xFF);
	setval(S, p->amode_d, n, SECOND_OP, BYTEINSTR, result, p);

	tmpSR = msp430regread(S, MSP430_SR, p);
	if ((result >> 7) & B0001)
	{
		msp430_sreg_set_N(tmpSR);
	}
	if (result == 0)
	{
		msp430_sreg_set_Z(tmpSR);
	}
	if (!msp430_sreg_get_Z(tmpSR))
	{
		msp430_sreg_set_C(tmpSR);
	}
	msp430regset(S, MSP430_SR, tmpSR);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}





/*												*/
/*				Single-Operand (FORMAT II) instructions				*/
/*	15				       7     6     5      4   	3  2  1  0		*/
/*	|		op-code			|   B/W  |    Ad    |	D/S-reg			*/
/*												*/
/*												*/
/*	Mnemonic	S-Reg		Operation 			Status Bits		*/
/*			D-Reg					V	N	Z	C	*/
/*	----------------------------------------------------------------------------------	*/
/*	RRC(.B)		dst	C -> MSB ->.......LSB -> C	*	*	*	*	*/
/*	RRA(.B)		dst	MSB -> MSB ->....LSB -> C	0	*	*	*	*/
/*	PUSH(.B)	src	SP  2 -> SP, src -> @SP					*/
/*	SWPB		dst	Swap bytes							*/
/*	CALL		dst	SP  2 -> SP, PC+2 -> @SP					*/
/*				dst -> PC							*/
/*	RETI			TOS -> SR, SP + 2 -> SP		*	*	*	*	*/
/*				TOS -> PC, SP + 2 -> SP						*/
/*	SXT		dst	Bit 7 -> Bit 8........Bit 15	0	*	*	*	*/
/*												*/
/*	Legend:											*/
/*	*	The status bit is affected							*/
/*		The status bit is not affected							*/
/*	0	The status bit is cleared							*/
/*	1	The status bit is set								*/
/*												*/

tuck void
msp430_rrc(State *S, ushort n, MSP430Pipestage *p)
{
	ushort	tmpSR, orig_dst, orig_C, tmp, result;


	tmpSR = msp430regread(S, MSP430_SR, p);

	orig_dst = getval(S, p->amode_s, n, FIRST_OP, WORDINSTR, p);
	orig_C = msp430_sreg_get_C(tmpSR);
	tmp = orig_C << 15;
	msp430_sreg_setval_C(tmpSR, (orig_dst & 0x1));
	result = (orig_dst >> 1) | tmp;
	setval(S, p->amode_s, n, FIRST_OP, WORDINSTR, result, p);

	if (((short)orig_dst > 0) && orig_C)
	{
		msp430_sreg_set_V(tmpSR);
	}
	else
	{
		msp430_sreg_clr_V(tmpSR);
	}
	if ((result >> 15) & B0001)
	{
		msp430_sreg_set_N(tmpSR);
	}
	if (result == 0)
	{
		msp430_sreg_set_Z(tmpSR);
	}
	msp430regset(S, MSP430_SR, tmpSR);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}

tuck void
msp430_rrcb(State *S, ushort n, MSP430Pipestage *p)
{
	uchar	orig_dst, tmp, result;
	ushort	tmpSR;


	tmpSR = msp430regread(S, MSP430_SR, p);
	orig_dst = getval(S, p->amode_s, n, FIRST_OP, BYTEINSTR, p) & 0xFF;
	tmp = msp430_sreg_get_C(tmpSR) << 7;
	msp430_sreg_setval_C(tmpSR, (orig_dst & 0x1));
	result = (orig_dst >> 1) | tmp;
	setval(S, p->amode_d, n, FIRST_OP, BYTEINSTR, result, p);

	if (((char)orig_dst > 0) && msp430_sreg_get_C(tmpSR))
	{
		msp430_sreg_set_V(tmpSR);

	}
	else
	{
		msp430_sreg_clr_V(tmpSR);
	}
	if ((result >> 7) & B0001)
	{
		msp430_sreg_set_N(tmpSR);
	}
	if (result == 0)
	{
		msp430_sreg_set_Z(tmpSR);
	}
	msp430regset(S, MSP430_SR, tmpSR);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}

tuck void
msp430_rra(State *S, ushort n, MSP430Pipestage *p)
{
	ushort	tmpSR, orig_dst, tmp, result;


	tmpSR = msp430regread(S, MSP430_SR, p);
	orig_dst = getval(S, p->amode_s, n, FIRST_OP, WORDINSTR, p);
	msp430_sreg_setval_C(tmpSR, (orig_dst & 0x1));
	tmp = orig_dst & (1 << 15);
	result = (orig_dst >> 1) | tmp;
	setval(S, p->amode_d, n, FIRST_OP, WORDINSTR, result, p);

	if ((result >> 15) & B0001)
	{
		msp430_sreg_set_N(tmpSR);
	}
	if (result == 0)
	{
		msp430_sreg_set_Z(tmpSR);
	}
	msp430_sreg_clr_V(tmpSR);
	msp430regset(S, MSP430_SR, tmpSR);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}

tuck void
msp430_rrab(State *S, ushort n, MSP430Pipestage *p)
{
	uchar	tmp, result, orig_dst;
	ushort	tmpSR;

	tmpSR = msp430regread(S, MSP430_SR, p);
	orig_dst = getval(S, p->amode_s, n, FIRST_OP, BYTEINSTR, p) & 0xFF;
	tmp = orig_dst & (1 << 7);
	msp430_sreg_setval_C(tmpSR, (orig_dst & 0x1));
	result = (orig_dst >> 1) | tmp;
	setval(S, p->amode_d, n, FIRST_OP, BYTEINSTR, result, p);

	tmpSR = msp430regread(S, MSP430_SR, p);
	if ((result >> 15) & B0001)
	{
		msp430_sreg_set_N(tmpSR);
	}
	if (result == 0)
	{
		msp430_sreg_set_Z(tmpSR);
	}
	msp430_sreg_clr_V(tmpSR);
	msp430regset(S, MSP430_SR, tmpSR);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}

tuck void
msp430_push(State *S, ushort m, MSP430Pipestage *p)
{
	msp430regset(S, MSP430_SP, msp430regread(S, MSP430_SP, p) - 2);
	msp430writeword(S, msp430regread(S, MSP430_SP, p),
		getval(S, p->amode_s, m, FIRST_OP, WORDINSTR, p));
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);

	return;
}

tuck void
msp430_pushb(State *S, ushort m, MSP430Pipestage *p)
{
	msp430regset(S, MSP430_SP, msp430regread(S, MSP430_SP, p) - 2);
	msp430writebyte(S, msp430regread(S, MSP430_SP, p),
		(getval(S, p->amode_s, m, FIRST_OP, BYTEINSTR, p) & 0xFF));
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);

	return;
}

tuck void
msp430_swpb(State *S, ushort n, MSP430Pipestage *p)
{
	ushort	tmp;

	tmp = getval(S, p->amode_s, n, FIRST_OP, WORDINSTR, p);
	setval(S, p->amode_d, n, FIRST_OP, BYTEINSTR, (tmp >> 8) | ((tmp & 0xFF) << 8), p);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);

	return;
}

tuck void
msp430_call(State *S, ushort n, MSP430Pipestage *p)
{
	ushort tmp;

	tmp = getval(S, p->amode_s, n, FIRST_OP, WORDINSTR, p);
	msp430regset(S, MSP430_SP, msp430regread(S, MSP430_SP, p) - 2);
	msp430writeword(S, msp430regread(S, MSP430_SP, p), msp430regread(S, MSP430_PC, p));
	msp430regset(S, MSP430_PC, tmp);

	return;
}

tuck void
msp430_reti(State *S, MSP430Pipestage *p)
{
	msp430regset(S, MSP430_SR,
		msp430readword(S, msp430regread(S, MSP430_SP, p)));
	msp430regset(S, MSP430_SP, msp430regread(S, MSP430_SP, p) + 2);
	msp430regset(S, MSP430_PC,
		msp430readword(S, msp430regread(S, MSP430_SP, p)));
	msp430regset(S, MSP430_SP, msp430regread(S, MSP430_SP, p) + 2);

	return;
}

tuck void
msp430_sxt(State *S, ushort n, MSP430Pipestage *p)
{
	ushort	tmpSR, result, orig_dst;

	orig_dst = getval(S, p->amode_s, n, FIRST_OP, WORDINSTR, p);
	if (orig_dst & (1 << 7))
	{
		result = orig_dst | 0xFF00;
	}
	else
	{
		result = orig_dst & 0x00FF;
	}
	setval(S, p->amode_d, n, FIRST_OP, WORDINSTR, result, p);

	tmpSR = msp430regread(S, MSP430_SR, p);
	if ((result >> 15) & B0001)
	{
		msp430_sreg_set_N(tmpSR);
	}
	if (result == 0)
	{
		msp430_sreg_set_Z(tmpSR);
	}
	if (!msp430_sreg_get_Z(tmpSR))
	{
		msp430_sreg_set_C(tmpSR);
	}
	msp430_sreg_clr_V(tmpSR);
	msp430regset(S, MSP430_SR, tmpSR);
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);


	return;
}






/*												*/
/*				Single-Operand (FORMAT III) instructions			*/
/*	15		      13 12           10  9   				0		*/
/*	|	op-code		|	C 	|    	10-bit PC offset			*/
/*												*/
/*												*/
/*	Mnemonic	S-Reg		Operation 						*/
/*			D-Reg									*/
/*	----------------------------------------------------------------------------------	*/
/*												*/
/*	JEQ/JZ		Label		Jump to label if zero bit is set			*/
/*	JNE/JNZ		Label		Jump to label if zero bit is reset			*/
/*	JC		Label		Jump to label if carry bit is set			*/
/*	JNC		Label		Jump to label if carry bit is reset			*/
/*	JN		Label		Jump to label if negative bit is set			*/
/*	JGE		Label		Jump to label if (N .XOR. V) = 0			*/
/*	JL		Label		Jump to label if (N .XOR. V) = 1			*/
/*	JMP		Label		Jump to label unconditionally				*/
/*												*/

tuck void
msp430_jeqjz(State *S, short offset, MSP430Pipestage *p)
{
	if (msp430_sreg_get_Z(msp430regread(S, MSP430_SR, p)))
	{
		msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p) + 2*offset);
	}
	else
	{
		msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);
	}

	return;
}


tuck void
msp430_jnejnz(State *S, short offset, MSP430Pipestage *p)
{
	if (!msp430_sreg_get_Z(msp430regread(S, MSP430_SR, p)))
	{
		msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p) + 2*offset);
	}
	else
	{
		msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);
	}

	return;
}

tuck void
msp430_jc(State *S, short offset, MSP430Pipestage *p)
{
	if (msp430_sreg_get_C(msp430regread(S, MSP430_SR, p)))
	{
		msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p) + 2*offset);
	}
	else
	{
		msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);
	}

	return;
}


tuck void
msp430_jnc(State *S, short offset, MSP430Pipestage *p)
{
	if (!msp430_sreg_get_C(msp430regread(S, MSP430_SR, p)))
	{
		msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p) + 2*offset);
	}
	else
	{
		msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);
	}

	return;
}


tuck void
msp430_jn(State *S, short offset, MSP430Pipestage *p)
{
	if (msp430_sreg_get_N(msp430regread(S, MSP430_SR, p)))
	{
		msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p) + 2*offset);
	}
	else
	{
		msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);
	}

	return;
}


tuck void
msp430_jge(State *S, short offset, MSP430Pipestage *p)
{
	ushort	tmpSR = msp430regread(S, MSP430_SR, p);
	if (!(msp430_sreg_get_N(tmpSR) ^ msp430_sreg_get_V(tmpSR)))
	{
		msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p) + 2*offset);
	}
	else
	{
		msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);
	}

	return;
}


tuck void
msp430_jl(State *S, short offset, MSP430Pipestage *p)
{
	ushort	tmpSR = msp430regread(S, MSP430_SR, p);
	if (msp430_sreg_get_N(tmpSR) ^ msp430_sreg_get_V(tmpSR))
	{
		msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p) + 2*offset);
	}
	else
	{
		msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p)+p->ilen);
	}

	return;
}


tuck void
msp430_jmp(State *S, short offset, MSP430Pipestage *p)
{
	msp430regset(S, MSP430_PC, msp430regread(S, MSP430_PC, p) + 2*offset);

	return;
}
