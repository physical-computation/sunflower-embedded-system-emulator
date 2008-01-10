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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sf.h"
#include "mmalloc.h"
#include "mextern.h"


/*	The battery feed lags behind main simulation by one cycle.	*/
void
battery_feed(double force_current)
{
	int			i, j;
	Batt			*B;
	double	 		Iload;
	const double		Vload = 3.3;
	double 			tmpval;
	double 			tmpindex;
	double	 		tmplow;
	double 			tmphigh;
	double	 		tmpdelta;


	/*	First determine the total current drawn in this cycle	*/
	for (i = 0; i < SIM_NUM_ACTIVE_BATTERIES; i++)
	{
		B = SIM_ACTIVE_BATTERIES[i];

		if (B->dead)
		{
			continue;
		}

		/*							*/
		/*	Set min Iload to leak current, rather than 0 	*/
		/*	for calcs below					*/
		/*							*/
		Iload = SIM_BATT_LEAK_CURRENT;

		for (j = 0; j < B->num_attached; j++)
		{	
			if (((State *)B->node_ptrs[j])->last_stepclks == 0)
			{
				continue;
			}

			/*	Iload was sum over last_stepclks clks	*/
			Iload += ((State *)B->node_ptrs[j])->E.current_draw / 
					((State *)B->node_ptrs[j])->last_stepclks;

			/*						*/
			/*	Keep information about the maximum 	*/
			/*	load current seen			*/
			/*						*/
			if (Iload > B->maxIload)
			{
				B->maxIload = Iload;
			}
				
			B->nsamplesIload++;
			B->sumIload += Iload;
			B->avgIload = B->sumIload/B->nsamplesIload;
		}
		
	
		if (force_current > 0)
		{
			Iload = force_current;
			B->maxIload = Iload;
		}


		/*								*/
		/*	First, scale Iload due to DC-DC converter ineff		*/
		/*	where Ibatt (current at batt inputs):			*/
		/*								*/
		/*		Ibatt = Vload*Iload				*/
		/*		      -----------				*/
		/*		   eta(Vbatt, Iload)*Vbatt			*/
		/*								*/
		/*	LUT for eta is fxn of Iload only. Iload is a maximum	*/
		/*	of 10x the nominal current draw.			*/
		/*								*/
		if (Iload/B->Inominal > 10.0)
		{
			mprint(NULL, siminfo, 
				"\nIload is absurdly high (%E). Shutting down...\n",
				Iload);

			mexit("See above messages", -1);
		}

		/*	We do linear interpolation:	*/
		tmpval = (Iload/B->Inominal)/10.0;
		tmpindex = tmpval*(B->etaLUTnentries-1);
		tmplow = floor(tmpindex);
		tmphigh = ceil(tmpindex);
		tmpdelta = tmphigh-tmplow;

		B->Ibatt = (Vload*Iload)/
			((
			((tmphigh-tmpindex)/tmpdelta)*B->etaLUT[(int)tmplow] +
			((tmpindex-tmplow)/tmpdelta)*B->etaLUT[(int)tmphigh]
			)*B->Vbatt);

		B->tnow = SIM_GLOBAL_TIME;

		/*		Maximum Ibatt is 10x Inominal		*/
		B->Vr = (B->Ibatt/B->Inominal)/10;

		/*			Vrate				*/
		B->Vrate = (B->Vrateold - B->Vr)*exp((0 - (B->tnow-B->told))/(B->Rf*B->Cf)) + B->Vr;
		B->Vrateold = B->Vrate;

		/*	Vlost models dependency on discharge rate	*/
		/*	Entries 0 .. VlostLUTnentries-1 in LUT for 	*/
		/*	Vrate = 0 .. 1.0. We do linear interpolation	*/
		tmpindex = B->Vrate*(B->VlostLUTnentries-1);
		tmplow = floor(tmpindex);
		tmphigh = ceil(tmpindex);
		tmpdelta = tmphigh-tmplow;
		B->Vlost = ((tmphigh-tmpindex)/tmpdelta)*B->VlostLUT[(int)tmplow] +
				((tmpindex-tmplow)/tmpdelta)*B->VlostLUT[(int)tmphigh];


		/*		Next, compute Vc for battery		*/
		B->cap_act -= B->Ibatt_old*(B->tnow-B->told);
		B->Vc = (B->cap_act/B->C) - B->Vlost;


		/*							*/
		/*	Vc acts as a proxy for capacity remaining	*/
		/*	Entries 0 .. B->VbattLUTnentries-1 in LUT 	*/
		/*	for Vc = 0 .. 1.0				*/
		/*							*/
		/*	We do linear interpolation	*/
		tmpindex = B->Vc*(B->VbattLUTnentries-1);
		tmplow = floor(tmpindex);
		tmphigh = ceil(tmpindex);
		tmpdelta = tmphigh-tmplow;
		B->Vbatt = ((tmphigh-tmpindex)/tmpdelta)*B->VbattLUT[(int)tmplow] +
				((tmpindex-tmplow)/tmpdelta)*B->VbattLUT[(int)tmphigh];
		B->Ibatt_old = B->Ibatt;
		B->told = B->tnow;
		B->battery_remaining = B->battery_capacity*B->Vc;

		/*	Battery terminal voltage has fallen to 'dead' level 	*/
		if (B->Vbatt <= B->Vbatt_expended)
		{
			B->dead = 1;
			SIM_NUM_DEPLETED_BATTERIES++;

			if (SIM_NUM_DEPLETED_BATTERIES == SIM_NUM_ACTIVE_BATTERIES)
			{
				mexit("All batteries depleted; Exiting normally.\n", 0);
			}
		}
	}

	return;
}

void
battery_dumpall(State *S)
{
	int		i;


	for (i = 0; i < SIM_NUM_ACTIVE_BATTERIES; i++)
	{
		Batt *B = SIM_ACTIVE_BATTERIES[i];
	
		if (B == NULL)
		{
			mprint(NULL, siminfo,
				"No valid battery, but you're doing battdebug()!\n\n\n");
			mexit("See above messages.", -1);
		}

		mlog(S, "\n");
		mlog(S, "\tBattery ID %d\n", B->ID);
		mlog(S, "\tbattery_capacity	%E\n", B->battery_capacity);
		mlog(S, "\tbattery_remaining	%E\n", B->battery_remaining);
		mlog(S, "\tmaxIload		%E\n", B->maxIload);
		mlog(S, "\tnum_attached		%d\n", B->num_attached);
		mlog(S, "\tcapacity_Ah	%E\n", B->capacity_Ah);
		mlog(S, "\tC		%E\n", B->C);
		mlog(S, "\tCf		%E\n", B->Cf);
		mlog(S, "\tInominal	%E\n", B->Inominal);
		mlog(S, "\tIbatt		%E\n", B->Ibatt);
		mlog(S, "\tIbattold	%E\n", B->Ibatt_old);
		mlog(S, "\tRf		%E\n", B->Rf);
		mlog(S, "\tVc		%E\n", B->Vc);
		mlog(S, "\tVc_0		%E\n", B->Vc_0);
		mlog(S, "\tVbatt		%E\n", B->Vbatt);
		mlog(S, "\tVlost		%E\n", B->Vlost);
		mlog(S, "\tVr		%E\n", B->Vr);
		mlog(S, "\tVrate			%E\n", B->Vrate);
		mlog(S, "\tVrateold	%E\n", B->Vrateold);
		mlog(S, "\tVrate_0	%E\n", B->Vrate_0);
		mlog(S, "\ttnow		%E\n", B->tnow);
		mlog(S, "\ttold		%E\n", B->told);
		mlog(S, "\ttinit		%E\n", B->tinit);
		mlog(S, "\tavgIload		%E\n", B->avgIload);
		mlog(S, "\tnsamplesIload	%E\n", B->nsamplesIload);
	
		mlog(S, "\n");
	}

	return;
}

void
batt_newbatt(int ID, double capacity_mAh)
{
	if (SIM_NUM_BATTERIES >= MAX_BATTERIES)
	{
		mprint(NULL, siminfo, "Battery creation limit reached. Sorry.");

		return;
	}

	mprint(NULL, siminfo, "newbatt ID=%d, capacity_mAh=%E\n", ID, capacity_mAh);

	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT = (double *)mcalloc(DEFLT_NLUTENTRIES,
							sizeof(double), __FILE__);
	if (SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT == NULL)
	{
		mprint(NULL, siminfo, "Could not allocate memory for Vbatt LUT");
	}

	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT = (double *)mcalloc(DEFLT_NLUTENTRIES,
							sizeof(double), __FILE__);
	if (SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT == NULL)
	{
		mprint(NULL, siminfo, "Could not allocate memory for Vlost LUT");
	}

	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT = (double *)mcalloc(DEFLT_NLUTENTRIES,
							sizeof(double), __FILE__);
	if (SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT == NULL)
	{
		mprint(NULL, siminfo, "Could not allocate memory for eta LUT");
	}

	/*	Average Vbatt is 4.0V. Should eventually be in loaded parameter		*/
	SIM_BATTERIES[SIM_NUM_BATTERIES].battery_capacity = 
			(capacity_mAh/1000.0) * 4.0 * 3600.0;

	SIM_BATTERIES[SIM_NUM_BATTERIES].capacity_Ah = capacity_mAh/1000.0;
	SIM_BATTERIES[SIM_NUM_BATTERIES].battery_remaining = 
			SIM_BATTERIES[SIM_NUM_BATTERIES].battery_capacity;
	SIM_BATTERIES[SIM_NUM_BATTERIES].ID = ID;
	SIM_BATTERIES[SIM_NUM_BATTERIES].maxIload = 0.0;
	SIM_BATTERIES[SIM_NUM_BATTERIES].num_attached = 0;
	SIM_BATTERIES[SIM_NUM_BATTERIES].nsamplesIload = 0;
	SIM_BATTERIES[SIM_NUM_BATTERIES].avgIload = 0;
	SIM_BATTERIES[SIM_NUM_BATTERIES].sumIload = 0;

	/*											*/
	/*	Defaults for generic Li-Ion (Panasonic CGR-*) and generic DC-DC converter	*/
	/*											*/
	SIM_BATTERIES[SIM_NUM_BATTERIES].C = 3600.0*SIM_BATTERIES[SIM_NUM_BATTERIES].capacity_Ah;
	SIM_BATTERIES[SIM_NUM_BATTERIES].Cf = 0.0001;
	SIM_BATTERIES[SIM_NUM_BATTERIES].Ibatt = 0.0;
	SIM_BATTERIES[SIM_NUM_BATTERIES].Ibatt_old = 0.0;
	SIM_BATTERIES[SIM_NUM_BATTERIES].Inominal = 150E-3;
	SIM_BATTERIES[SIM_NUM_BATTERIES].Rf = 4000;
	SIM_BATTERIES[SIM_NUM_BATTERIES].Vc = 1.0;
	SIM_BATTERIES[SIM_NUM_BATTERIES].Vc_0 = 1.0;
	SIM_BATTERIES[SIM_NUM_BATTERIES].Vbatt = 4.2;
	SIM_BATTERIES[SIM_NUM_BATTERIES].Vbatt_expended = 2.0;
	SIM_BATTERIES[SIM_NUM_BATTERIES].Vlost = 0.0;
	SIM_BATTERIES[SIM_NUM_BATTERIES].Vr = SIM_BATTERIES[SIM_NUM_BATTERIES].Ibatt /
			SIM_BATTERIES[SIM_NUM_BATTERIES].capacity_Ah;
	SIM_BATTERIES[SIM_NUM_BATTERIES].Vrate = 0.0;
	SIM_BATTERIES[SIM_NUM_BATTERIES].Vrateold = 0.0;
	SIM_BATTERIES[SIM_NUM_BATTERIES].Vrate_0 = 0.0;
	SIM_BATTERIES[SIM_NUM_BATTERIES].tnow = SIM_GLOBAL_TIME;
	SIM_BATTERIES[SIM_NUM_BATTERIES].told = SIM_GLOBAL_TIME;
	SIM_BATTERIES[SIM_NUM_BATTERIES].tinit = SIM_GLOBAL_TIME;
	SIM_BATTERIES[SIM_NUM_BATTERIES].cap_act = SIM_BATTERIES[SIM_NUM_BATTERIES].Vc_0*
			SIM_BATTERIES[SIM_NUM_BATTERIES].C;

	/*			DC-DC converter efficiency. Indexed by Iout			*/
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[0] = 0.850;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[1] = 0.852;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[2] = 0.854;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[3] = 0.856;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[4] = 0.858;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[5] = 0.860;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[6] = 0.862;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[7] = 0.864;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[8] = 0.866;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[9] = 0.868;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[10] = 0.870;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[11] = 0.872;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[12] = 0.874;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[13] = 0.876;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[14] = 0.878;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[15] = 0.880;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[16] = 0.883;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[17] = 0.888;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[18] = 0.895;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[19] = 0.900;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[20] = 0.905;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[21] = 0.910;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[22] = 0.915;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[23] = 0.920;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[24] = 0.925;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[25] = 0.930;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[26] = 0.935;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[27] = 0.942;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[28] = 0.948;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[29] = 0.950;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[30] = 0.950;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[31] = 0.948;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[32] = 0.942;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[33] = 0.935;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[34] = 0.930;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[35] = 0.925;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[36] = 0.920;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[37] = 0.915;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[38] = 0.910;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[39] = 0.900;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[40] = 0.890;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[41] = 0.880;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[42] = 0.870;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[43] = 0.860;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[44] = 0.850;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[45] = 0.840;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[46] = 0.830;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[47] = 0.820;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[48] = 0.810;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUT[49] = 0.800;
	SIM_BATTERIES[SIM_NUM_BATTERIES].etaLUTnentries = 50;

	/*	Dependence of battery terminal voltage on state of charge. Indexed by Vc	*/
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[0] = 2.50;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[1] = 2.50;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[2] = 2.50;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[3] = 2.50;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[4] = 2.51;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[5] = 2.68;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[6] = 2.84;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[7] = 2.99;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[8] = 3.10;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[9] = 3.20;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[10] = 3.30;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[11] = 3.35;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[12] = 3.40;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[13] = 3.45;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[14] = 3.50;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[15] = 3.60;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[16] = 3.61;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[17] = 3.62;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[18] = 3.63;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[19] = 3.64;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[20] = 3.650;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[21] = 3.655;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[22] = 3.660;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[23] = 3.665;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[24] = 3.670;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[25] = 3.675;

	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[26] = 3.680;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[27] = 3.685;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[28] = 3.690;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[29] = 3.695;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[30] = 3.700;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[31] = 3.705;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[32] = 3.710;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[33] = 3.715;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[34] = 3.720;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[35] = 3.725;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[36] = 3.730;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[37] = 3.735;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[38] = 3.740;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[39] = 3.745;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[40] = 3.750;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[41] = 3.755;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[42] = 3.760;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[43] = 3.765;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[44] = 3.770;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[45] = 3.775;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[46] = 3.780;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[47] = 3.785;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[48] = 3.790;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[49] = 3.795;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[50] = 3.800;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[51] = 3.805;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[52] = 3.810;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[53] = 3.815;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[54] = 3.820;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[55] = 3.825;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[56] = 3.830;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[57] = 3.835;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[58] = 3.840;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[59] = 3.845;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[60] = 3.850;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[61] = 3.855;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[62] = 3.860;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[63] = 3.865;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[64] = 3.870;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[65] = 3.875;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[66] = 3.880;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[67] = 3.885;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[68] = 3.890;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[69] = 3.895;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[70] = 3.900;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[71] = 3.905;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[72] = 3.910;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[73] = 3.915;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[74] = 3.920;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[75] = 3.925;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[76] = 3.930;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[77] = 3.935;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[78] = 3.940;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[79] = 3.945;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[80] = 3.950;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[81] = 3.955;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[82] = 3.960;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[83] = 3.965;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[84] = 3.970;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[85] = 3.975;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[86] = 3.980;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[87] = 3.985;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[88] = 3.990;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[89] = 3.995;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[90] = 4.000;

	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[91] = 4.01;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[92] = 4.02;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[93] = 4.03;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[94] = 4.04;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[95] = 4.06;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[96] = 4.08;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[97] = 4.10;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[98] = 4.15;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUT[99] = 4.20;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VbattLUTnentries = 100;

	/*											*/
	/*	Dependence of battery terminal voltage on current draw. Indexed by Vrate	*/
	/*	Vrate is a time-sluggish version of Ibatt/(Nominal current (150mA))		*/
	/*											*/
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[0] = 0.005;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[1] = 0.010;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[2] = 0.015;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[3] = 0.020;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[4] = 0.025;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[5] = 0.030;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[6] = 0.035;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[7] = 0.040;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[8] = 0.045;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[9] = 0.050;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[10] = 0.055;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[11] = 0.060;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[12] = 0.065;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[13] = 0.070;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[14] = 0.075;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[15] = 0.080;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[16] = 0.085;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[17] = 0.090;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[18] = 0.095;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[19] = 0.100;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[20] = 0.105;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[21] = 0.110;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[22] = 0.115;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[23] = 0.120;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[24] = 0.125;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[25] = 0.130;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[26] = 0.135;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[27] = 0.140;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[28] = 0.145;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[29] = 0.150;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[30] = 0.155;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[31] = 0.160;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[32] = 0.165;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[33] = 0.170;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[34] = 0.175;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[35] = 0.180;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[36] = 0.185;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[37] = 0.190;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[38] = 0.195;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[39] = 0.200;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[40] = 0.205;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[41] = 0.210;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[42] = 0.215;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[43] = 0.220;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[44] = 0.225;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[45] = 0.230;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[46] = 0.235;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[47] = 0.240;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[48] = 0.245;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUT[49] = 0.250;
	SIM_BATTERIES[SIM_NUM_BATTERIES].VlostLUTnentries = 50;

	SIM_ACTIVE_BATTERIES[SIM_NUM_ACTIVE_BATTERIES] = &SIM_BATTERIES[SIM_NUM_BATTERIES];
	SIM_NUM_ACTIVE_BATTERIES++;
	SIM_NUM_BATTERIES++;


	return;
}

void
batt_nodeattach(State *S, int ID)
{
        int     i, which = 0;

        for (i = 0; i < MAX_BATTERIES; i++)
        {
                if (SIM_BATTERIES[i].ID == ID)
                {
                        which = i;
                        break;
                }
        }  
        if (i == MAX_BATTERIES)
        {
                merror("Battery with ID %d not found", ID);
                return;
        }

        if (SIM_BATTERIES[which].num_attached == MAX_BATTNODES)
        {
                merror("Too many nodes attached to battery with ID %d", ID);
                return;
        }
				
	SIM_BATTERIES[which].node_ptrs[SIM_BATTERIES[which].num_attached++] = S;
	S->BATT = &SIM_BATTERIES[which];
	
	return;
}

void
batt_printstats(State *S, int which)
{
	if (which >= MAX_BATTERIES)
	{
		mprint(NULL, siminfo, "Invalid battery index");
		return;
	}

	mprint(NULL, siminfo, "\n\n");
	mprint(NULL, siminfo, "ID = %d\n", SIM_BATTERIES[which].ID);
	mprint(NULL, siminfo, "Number of nodes attached = %d\n",
			SIM_BATTERIES[which].num_attached);
	mprint(NULL, siminfo, "Estimated Ideal Capacity = %E (Joules)\n",
			SIM_BATTERIES[which].battery_capacity);
	mprint(NULL, siminfo, "Estimated Ideal Battery remaining (Joules) = %E\n",
			SIM_BATTERIES[which].battery_remaining);
	mprint(NULL, siminfo, "Maximum Sampled Current Load (mA) = %E\n",
			SIM_BATTERIES[which].maxIload*1000);
	mprint(NULL, siminfo, "\n\n");


	return;			
}
