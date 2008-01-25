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

enum
{
	MSP430_DEFLT_MEMSIZE			= 256,
	MSP430_MEMBASE				= 0x0200,
	MSP430_FLASHBASE			= 0xFFFF - 8192,
	MSP430_DEFAULT_MEMREAD_LATENCY		= 1,
	MSP430_DEFAULT_MEMWRITE_LATENCY		= 1,
	MSP430_NPINS				= 256,
};

typedef struct
{
	ushort		mab16;
	ushort		mdb16;

	uchar		mab8;
	uchar		mdb8;

	/*					*/
	/*	When we use Numa facilities to	*/
	/*	translate addr, we count those	*/
	/*	separately.			*/
	/*					*/
	ulong		paddr_bus;
} MSP430Buses;

typedef struct MSP430State MSP430State;
struct MSP430State
{
	ushort 		R		[16];
	ushort		periph16	[(MSP430_RAM_BEGIN - MSP430_PERIPH16_BEGIN)/2];
	uchar		periph8		[MSP430_PERIPH16_BEGIN - MSP430_PERIPH8_BEGIN];
	uchar		sfr		[MSP430_PERIPH8_BEGIN - MSP430_SFR_BEGIN];		
	double		pins		[MSP430_NPINS];

	ushort		me1;
	ushort		me2;
	ushort		ie1;
	ushort		ie2;
	ushort		ifg1;
	ushort		ifg2;

	MSP430Pipe	P;
	MSP430Buses	*B;

	double		vdd;
};

/*		Entries in the Decode Cache		*/
typedef struct
{
	MSP430Pipestage	dc_p;
} MSP430DCEntry;

#define	MSP430_DEFAULT_VDD			(3.3)
#define	MSP430_DEFAULT_CYCLETIME		((double)1/8000000)
