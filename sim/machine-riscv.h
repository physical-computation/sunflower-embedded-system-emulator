/*
	Copyright (c) 2017-2018, Zhengyang Gu (author)

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

typedef struct
{
	uint32_t		R[RISCV_XMAX];
	uint64_t		fR[RF32FD_fMAX];
	uint32_t		fCSR;
	RiscvPipe		P;
	UncertainState *	uncertain;
	uint32_t		instruction_distribution[RISCV_OP_MAX];

	/*
	 *	Taint analysis for registers (need last entry for PC's taint entry 32)
	 */
	ShadowMem		instruction_taintDistribution[RISCV_OP_MAX];
	ShadowMem		taintR[RISCV_XMAX+1];
	ShadowMem		taintfR[RF32FD_fMAX+1];

	/*
	 *	Histograms
	 */
	Histogram		histograms[RISCV_XMAX];

	/*
	 *	Features borrowed from superH
	 */
	int		cache_activated;
	
	SuperHBuses	*B;

	/*	Time of last generated timer intr	*/
	Picosec		TIMER_LASTACTIVATE;

	/*		Delay b/n timer interrupts	*/
	Picosec		TIMER_INTR_DELAY;

	/*		Power Adaptation Unit		*/
	PAUentry	*PAUs;
	int		npau;
	int		influenced;
	int		controlling_pau;
	ulong		pauaddrmask;

	Cache		*TLB;

	int		*PAUvalids;
	int		numpauvalids;

	/*      Whether mem access is in IF or EX       */
        int             mem_access_type;

	/*		MMU stuff			*/
	ulong		PTEH;
	ulong		PTEL;
	ulong		TTB;
	ulong		TEA;
	ulong		MMUCR;

} RiscvState;
