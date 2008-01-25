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

/*									*/
/*	We store Power info as opposed to Energy info, since a  	*/
/*	given instr may stay in the combinational logic of, say, 	*/
/*	the multiply unit for longer than it should, due to some 	*/
/*	pipeline hazard.						*/
/*									*/
enum
{
	MAXINSTRNAME = 64,
	NREADINGS = 256,
};

#define	SUPERH_READINGS_VDD	(3.3)
#define	SUPERH_ORIG_VDD		(3.3)
#define	SUPERH_ORIG_CYCLE	(1/60E6)

typedef struct
{
	double		reading1;
	double		reading2;
	int		ncycles;
} Currents;

typedef struct
{
	double		CPUEtot;
	uvlong		ntrans;
	
	/*	Per-Component Transitions	*/
	uvlong	 	ntrans_phy_addrbus;
	uvlong	 	ntrans_virt_addrbus;
	uvlong	 	ntrans_databus;
	uvlong	 	ntrans_prph_addrbus;
	uvlong	 	ntrans_prph_databus;
	uvlong	 	ntrans_clock;
	uvlong	 	ntrans_pc;
	uvlong	 	ntrans_cache;
	uvlong	 	ntrans_pipeline;

	/*	    Per-nic Energy		*/
	double		Enetwork;

	/*	Current drawn in last cycle	*/
	double	 	current_draw;
} EnergyInfo;


/*									*/
/*	TX/RX Power consumption is updated by updating E.current_draw	*/
/*	in network.c E.current_draw is reset to zero at the start of 	*/
/*	each simulation cycle and sums up the op and NIC power (and 	*/
/*	any other future) currents.					*/
/*									*/
#define	update_energy(op, Rm, Rn)\
	if ((S->force_sleeppwr != 0.0) && S->sleep)\
	{\
		S->energyinfo.CPUEtot += S->force_sleeppwr*S->CYCLETIME;\
		S->energyinfo.current_draw += S->force_sleeppwr/S->VDD;\
	}\
	else if (S->force_avgpwr != 0.0)\
	{\
		S->energyinfo.CPUEtot += S->force_avgpwr*S->CYCLETIME;\
		S->energyinfo.current_draw += S->force_avgpwr/S->VDD;\
	}\
	else\
	{\
		S->energyinfo.CPUEtot += S->scaledcurrents[op]*S->VDD*S->CYCLETIME;\
		S->energyinfo.current_draw += S->scaledcurrents[op];\
	}
