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
