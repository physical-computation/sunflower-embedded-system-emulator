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
enum
{
	/*	Maximum number of battery/DC-DC converters	*/
	MAX_BATTERIES = 128,

	/*	Max num. of nodes you can attach to a batt.	*/
	MAX_BATTNODES = 4, 

	/*		Default # of LUT entries		*/
	DEFLT_NLUTENTRIES = 100,
};

typedef struct
{
	int		ID;
	int		dead;
	int		num_attached;
	double		battery_capacity;
	double		battery_remaining;
	double		maxIload;
	double		nsamplesIload;
	double		avgIload;
	double		sumIload;
	void*		node_ptrs[MAX_BATTNODES];

	double		C;
	double		Cf;
	double		Ibatt;
	double		Ibatt_old;
	double		Inominal;
	double		Rf;
	double		Vbatt;
	double		Vbatt_expended;
	double		Vc;
	double		Vc_0;
	double		Vlost;
	double		Vr;
	double		Vrate;
	double		Vrate_0;
	double		Vrateold;
	double		capacity_Ah;
	double		tinit;
	double		tnow;
	double		told;
	double		cap_act;

	double		*VbattLUT;
	double		*VlostLUT;
	double		*etaLUT;

	int		VbattLUTnentries;
	int		VlostLUTnentries;
	int		etaLUTnentries;
} Batt;
