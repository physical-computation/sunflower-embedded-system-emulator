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
#include <stdlib.h>
#include <math.h>
#include "sf.h"
#include "mextern.h"


/*	The battery feed lags behind main simulation by one cycle.	*/
void
battery_feed(Engine *E, double force_current)
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
	for (i = 0; i < E->nactivebatts; i++)
	{
		B = E->activebatts[i];

		if (B->dead)
		{
			continue;
		}

		/*							*/
		/*	Set min Iload to leak current, rather than 0 	*/
		/*	for calcs below					*/
		/*							*/
		Iload = B->ileak;

		for (j = 0; j < B->num_attached; j++)
		{	
			if (((State *)B->node_ptrs[j])->last_stepclks == 0)
			{
				continue;
			}

			/*	Iload was sum over last_stepclks clks	*/
			Iload += ((State *)B->node_ptrs[j])->energyinfo.current_draw / 
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
			mprint(E, NULL, siminfo, 
				"\nIload is absurdly high (%E). Shutting down...\n",
				Iload);

			mexit(E, "See above messages", -1);
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

		B->tnow = E->globaltimepsec;

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
			E->ndepletedbatts++;

			if (E->ndepletedbatts == E->nactivebatts)
			{
				mexit(E, "All batteries depleted; Exiting normally.\n", 0);
			}
		}
	}

	return;
}

void
battery_dumpall(Engine *E, State *S)
{
	int		i;


	for (i = 0; i < E->nactivebatts; i++)
	{
		Batt *B = E->activebatts[i];
	
		if (B == NULL)
		{
			mprint(E, NULL, siminfo,
				"No valid battery, but you're doing battdebug()!\n\n\n");
			mexit(E, "See above messages.", -1);
		}

		mlog(E, S, "\n");
		mlog(E, S, "\tBattery ID %d\n", B->ID);
		mlog(E, S, "\tbattery_capacity	%E\n", B->battery_capacity);
		mlog(E, S, "\tbattery_remaining	%E\n", B->battery_remaining);
		mlog(E, S, "\tmaxIload		%E\n", B->maxIload);
		mlog(E, S, "\tnum_attached		%d\n", B->num_attached);
		mlog(E, S, "\tcapacity_Ah	%E\n", B->capacity_Ah);
		mlog(E, S, "\tC		%E\n", B->C);
		mlog(E, S, "\tCf		%E\n", B->Cf);
		mlog(E, S, "\tInominal	%E\n", B->Inominal);
		mlog(E, S, "\tIbatt		%E\n", B->Ibatt);
		mlog(E, S, "\tIbattold	%E\n", B->Ibatt_old);
		mlog(E, S, "\tRf		%E\n", B->Rf);
		mlog(E, S, "\tVc		%E\n", B->Vc);
		mlog(E, S, "\tVc_0		%E\n", B->Vc_0);
		mlog(E, S, "\tVbatt		%E\n", B->Vbatt);
		mlog(E, S, "\tVlost		%E\n", B->Vlost);
		mlog(E, S, "\tVr		%E\n", B->Vr);
		mlog(E, S, "\tVrate			%E\n", B->Vrate);
		mlog(E, S, "\tVrateold	%E\n", B->Vrateold);
		mlog(E, S, "\tVrate_0	%E\n", B->Vrate_0);
		mlog(E, S, "\ttnow		%E\n", B->tnow);
		mlog(E, S, "\ttold		%E\n", B->told);
		mlog(E, S, "\ttinit		%E\n", B->tinit);
		mlog(E, S, "\tavgIload		%E\n", B->avgIload);
		mlog(E, S, "\tnsamplesIload	%E\n", B->nsamplesIload);
	
		mlog(E, S, "\n");
	}

	return;
}

void
batt_newbatt(Engine *E, int ID, double capacity_mAh)
{
	Batt	*b;


	if (E->nbatts >= MAX_BATTERIES)
	{
		mprint(E, NULL, siminfo, "Battery creation limit reached. Sorry.");

		return;
	}

	mprint(E, NULL, siminfo, "newbatt ID=%d, capacity_mAh=%E\n", ID, capacity_mAh);

	b 			= &E->batts[E->nbatts];
	b->VbattLUT		= (double *)mcalloc(E, DEFLT_NLUTENTRIES,
							sizeof(double), __FILE__);
	if (b->VbattLUT == NULL)
	{
		mprint(E, NULL, siminfo, "Could not allocate memory for Vbatt LUT");
	}

	b->VlostLUT		= (double *)mcalloc(E, DEFLT_NLUTENTRIES,
							sizeof(double), __FILE__);
	if (b->VlostLUT == NULL)
	{
		mprint(E, NULL, siminfo, "Could not allocate memory for Vlost LUT");
	}

	b->etaLUT		= (double *)mcalloc(E, DEFLT_NLUTENTRIES,
							sizeof(double), __FILE__);
	if (b->etaLUT == NULL)
	{
		mprint(E, NULL, siminfo, "Could not allocate memory for eta LUT");
	}

	/*	Default battery leak current is 1uA	*/
	b->ileak		= 1E-6;

	/*	Average Vbatt is 4.0V. Should eventually be in loaded parameter		*/
	b->battery_capacity	= (capacity_mAh/1000.0) * 4.0 * 3600.0;

	b->capacity_Ah		= capacity_mAh/1000.0;
	b->battery_remaining	= b->battery_capacity;
	b->ID			= ID;
	b->maxIload		= 0.0;
	b->num_attached		= 0;
	b->nsamplesIload	= 0;
	b->avgIload		= 0;
	b->sumIload		= 0;

	/*											*/
	/*	Defaults for generic Li-Ion (Panasonic CGR-*) and generic DC-DC converter	*/
	/*											*/
	b->C			= 3600.0*b->capacity_Ah;
	b->Cf			= 0.0001;
	b->Ibatt		= 0.0;
	b->Ibatt_old		= 0.0;
	b->Inominal		= 150E-3;
	b->Rf			= 4000;
	b->Vc			= 1.0;
	b->Vc_0			= 1.0;
	b->Vbatt		= 4.2;
	b->Vbatt_expended	= 2.0;
	b->Vlost		= 0.0;
	b->Vr			= b->Ibatt / b->capacity_Ah;
	b->Vrate		= 0.0;
	b->Vrateold		= 0.0;
	b->Vrate_0		= 0.0;
	b->tnow			= E->globaltimepsec;
	b->told			= E->globaltimepsec;
	b->tinit		= E->globaltimepsec;
	b->cap_act		= b->Vc_0*b->C;

	/*			DC-DC converter efficiency. Indexed by Iout			*/
	b->etaLUT[0]		= 0.850;
	b->etaLUT[1]		= 0.852;
	b->etaLUT[2]		= 0.854;
	b->etaLUT[3]		= 0.856;
	b->etaLUT[4]		= 0.858;
	b->etaLUT[5]		= 0.860;
	b->etaLUT[6]		= 0.862;
	b->etaLUT[7]		= 0.864;
	b->etaLUT[8]		= 0.866;
	b->etaLUT[9]		= 0.868;
	b->etaLUT[10]		= 0.870;
	b->etaLUT[11]		= 0.872;
	b->etaLUT[12]		= 0.874;
	b->etaLUT[13]		= 0.876;
	b->etaLUT[14]		= 0.878;
	b->etaLUT[15]		= 0.880;
	b->etaLUT[16]		= 0.883;
	b->etaLUT[17]		= 0.888;
	b->etaLUT[18]		= 0.895;
	b->etaLUT[19]		= 0.900;
	b->etaLUT[20]		= 0.905;
	b->etaLUT[21]		= 0.910;
	b->etaLUT[22]		= 0.915;
	b->etaLUT[23]		= 0.920;
	b->etaLUT[24]		= 0.925;
	b->etaLUT[25]		= 0.930;
	b->etaLUT[26]		= 0.935;
	b->etaLUT[27]		= 0.942;
	b->etaLUT[28]		= 0.948;
	b->etaLUT[29]		= 0.950;
	b->etaLUT[30]		= 0.950;
	b->etaLUT[31]		= 0.948;
	b->etaLUT[32]		= 0.942;
	b->etaLUT[33]		= 0.935;
	b->etaLUT[34]		= 0.930;
	b->etaLUT[35]		= 0.925;
	b->etaLUT[36]		= 0.920;
	b->etaLUT[37]		= 0.915;
	b->etaLUT[38]		= 0.910;
	b->etaLUT[39]		= 0.900;
	b->etaLUT[40]		= 0.890;
	b->etaLUT[41]		= 0.880;
	b->etaLUT[42]		= 0.870;
	b->etaLUT[43]		= 0.860;
	b->etaLUT[44]		= 0.850;
	b->etaLUT[45]		= 0.840;
	b->etaLUT[46]		= 0.830;
	b->etaLUT[47]		= 0.820;
	b->etaLUT[48]		= 0.810;
	b->etaLUT[49]		= 0.800;
	b->etaLUTnentries	= 50;

	/*	Dependence of battery terminal voltage on state of charge. Indexed by Vc	*/
	b->VbattLUT[0]		= 2.50;
	b->VbattLUT[1]		= 2.50;
	b->VbattLUT[2]		= 2.50;
	b->VbattLUT[3]		= 2.50;
	b->VbattLUT[4]		= 2.51;
	b->VbattLUT[5]		= 2.68;
	b->VbattLUT[6]		= 2.84;
	b->VbattLUT[7]		= 2.99;
	b->VbattLUT[8]		= 3.10;
	b->VbattLUT[9]		= 3.20;
	b->VbattLUT[10]		= 3.30;
	b->VbattLUT[11]		= 3.35;
	b->VbattLUT[12]		= 3.40;
	b->VbattLUT[13]		= 3.45;
	b->VbattLUT[14]		= 3.50;
	b->VbattLUT[15]		= 3.60;
	b->VbattLUT[16]		= 3.61;
	b->VbattLUT[17]		= 3.62;
	b->VbattLUT[18]		= 3.63;
	b->VbattLUT[19]		= 3.64;
	b->VbattLUT[20]		= 3.650;
	b->VbattLUT[21]		= 3.655;
	b->VbattLUT[22]		= 3.660;
	b->VbattLUT[23]		= 3.665;
	b->VbattLUT[24]		= 3.670;
	b->VbattLUT[25]		= 3.675;

	b->VbattLUT[26]		= 3.680;
	b->VbattLUT[27]		= 3.685;
	b->VbattLUT[28]		= 3.690;
	b->VbattLUT[29]		= 3.695;
	b->VbattLUT[30]		= 3.700;
	b->VbattLUT[31]		= 3.705;
	b->VbattLUT[32]		= 3.710;
	b->VbattLUT[33]		= 3.715;
	b->VbattLUT[34]		= 3.720;
	b->VbattLUT[35]		= 3.725;
	b->VbattLUT[36]		= 3.730;
	b->VbattLUT[37]		= 3.735;
	b->VbattLUT[38]		= 3.740;
	b->VbattLUT[39]		= 3.745;
	b->VbattLUT[40]		= 3.750;
	b->VbattLUT[41]		= 3.755;
	b->VbattLUT[42]		= 3.760;
	b->VbattLUT[43]		= 3.765;
	b->VbattLUT[44]		= 3.770;
	b->VbattLUT[45]		= 3.775;
	b->VbattLUT[46]		= 3.780;
	b->VbattLUT[47]		= 3.785;
	b->VbattLUT[48]		= 3.790;
	b->VbattLUT[49]		= 3.795;
	b->VbattLUT[50]		= 3.800;
	b->VbattLUT[51]		= 3.805;
	b->VbattLUT[52]		= 3.810;
	b->VbattLUT[53]		= 3.815;
	b->VbattLUT[54]		= 3.820;
	b->VbattLUT[55]		= 3.825;
	b->VbattLUT[56]		= 3.830;
	b->VbattLUT[57]		= 3.835;
	b->VbattLUT[58]		= 3.840;
	b->VbattLUT[59]		= 3.845;
	b->VbattLUT[60]		= 3.850;
	b->VbattLUT[61]		= 3.855;
	b->VbattLUT[62]		= 3.860;
	b->VbattLUT[63]		= 3.865;
	b->VbattLUT[64]		= 3.870;
	b->VbattLUT[65]		= 3.875;
	b->VbattLUT[66]		= 3.880;
	b->VbattLUT[67]		= 3.885;
	b->VbattLUT[68]		= 3.890;
	b->VbattLUT[69]		= 3.895;
	b->VbattLUT[70]		= 3.900;
	b->VbattLUT[71]		= 3.905;
	b->VbattLUT[72]		= 3.910;
	b->VbattLUT[73]		= 3.915;
	b->VbattLUT[74]		= 3.920;
	b->VbattLUT[75]		= 3.925;
	b->VbattLUT[76]		= 3.930;
	b->VbattLUT[77]		= 3.935;
	b->VbattLUT[78]		= 3.940;
	b->VbattLUT[79]		= 3.945;
	b->VbattLUT[80]		= 3.950;
	b->VbattLUT[81]		= 3.955;
	b->VbattLUT[82]		= 3.960;
	b->VbattLUT[83]		= 3.965;
	b->VbattLUT[84]		= 3.970;
	b->VbattLUT[85]		= 3.975;
	b->VbattLUT[86]		= 3.980;
	b->VbattLUT[87]		= 3.985;
	b->VbattLUT[88]		= 3.990;
	b->VbattLUT[89]		= 3.995;
	b->VbattLUT[90]		= 4.000;

	b->VbattLUT[91]		= 4.01;
	b->VbattLUT[92]		= 4.02;
	b->VbattLUT[93]		= 4.03;
	b->VbattLUT[94]		= 4.04;
	b->VbattLUT[95]		= 4.06;
	b->VbattLUT[96]		= 4.08;
	b->VbattLUT[97]		= 4.10;
	b->VbattLUT[98]		= 4.15;
	b->VbattLUT[99]		= 4.20;
	b->VbattLUTnentries	= 100;

	/*											*/
	/*	Dependence of battery terminal voltage on current draw. Indexed by Vrate	*/
	/*	Vrate is a time-sluggish version of Ibatt/(Nominal current (150mA))		*/
	/*											*/
	b->VlostLUT[0]		= 0.005;
	b->VlostLUT[1]		= 0.010;
	b->VlostLUT[2]		= 0.015;
	b->VlostLUT[3]		= 0.020;
	b->VlostLUT[4]		= 0.025;
	b->VlostLUT[5]		= 0.030;
	b->VlostLUT[6]		= 0.035;
	b->VlostLUT[7]		= 0.040;
	b->VlostLUT[8]		= 0.045;
	b->VlostLUT[9]		= 0.050;
	b->VlostLUT[10]		= 0.055;
	b->VlostLUT[11]		= 0.060;
	b->VlostLUT[12]		= 0.065;
	b->VlostLUT[13]		= 0.070;
	b->VlostLUT[14]		= 0.075;
	b->VlostLUT[15]		= 0.080;
	b->VlostLUT[16]		= 0.085;
	b->VlostLUT[17]		= 0.090;
	b->VlostLUT[18]		= 0.095;
	b->VlostLUT[19]		= 0.100;
	b->VlostLUT[20]		= 0.105;
	b->VlostLUT[21]		= 0.110;
	b->VlostLUT[22]		= 0.115;
	b->VlostLUT[23]		= 0.120;
	b->VlostLUT[24]		= 0.125;
	b->VlostLUT[25]		= 0.130;
	b->VlostLUT[26]		= 0.135;
	b->VlostLUT[27]		= 0.140;
	b->VlostLUT[28]		= 0.145;
	b->VlostLUT[29]		= 0.150;
	b->VlostLUT[30]		= 0.155;
	b->VlostLUT[31]		= 0.160;
	b->VlostLUT[32]		= 0.165;
	b->VlostLUT[33]		= 0.170;
	b->VlostLUT[34]		= 0.175;
	b->VlostLUT[35]		= 0.180;
	b->VlostLUT[36]		= 0.185;
	b->VlostLUT[37]		= 0.190;
	b->VlostLUT[38]		= 0.195;
	b->VlostLUT[39]		= 0.200;
	b->VlostLUT[40]		= 0.205;
	b->VlostLUT[41]		= 0.210;
	b->VlostLUT[42]		= 0.215;
	b->VlostLUT[43]		= 0.220;
	b->VlostLUT[44]		= 0.225;
	b->VlostLUT[45]		= 0.230;
	b->VlostLUT[46]		= 0.235;
	b->VlostLUT[47]		= 0.240;
	b->VlostLUT[48]		= 0.245;
	b->VlostLUT[49]		= 0.250;
	b->VlostLUTnentries	= 50;

	E->activebatts[E->nactivebatts] = &E->batts[E->nbatts];
	E->nactivebatts++;
	E->nbatts++;

	return;
}

void
batt_nodeattach(Engine *E, State *S, int ID)
{
        int     i, which = 0;

        for (i = 0; i < MAX_BATTERIES; i++)
        {
                if (E->batts[i].ID == ID)
                {
                        which = i;
                        break;
                }
        }  
        if (i == MAX_BATTERIES)
        {
                merror(E, "Battery with ID %d not found", ID);
                return;
        }

        if (E->batts[which].num_attached == MAX_BATTNODES)
        {
                merror(E, "Too many nodes attached to battery with ID %d", ID);
                return;
        }
				
	E->batts[which].node_ptrs[E->batts[which].num_attached++] = S;
	S->BATT = &E->batts[which];
	
	return;
}

void
batt_printstats(Engine *E, State *S, int which)
{
	if (which >= MAX_BATTERIES)
	{
		mprint(E, NULL, siminfo, "Invalid battery index");
		return;
	}

	mprint(E, NULL, siminfo, "\n\n");
	mprint(E, NULL, siminfo, "ID = %d\n", E->batts[which].ID);
	mprint(E, NULL, siminfo, "Number of nodes attached = %d\n",
			E->batts[which].num_attached);
	mprint(E, NULL, siminfo, "Estimated Ideal Capacity = %E (Joules)\n",
			E->batts[which].battery_capacity);
	mprint(E, NULL, siminfo, "Estimated Ideal Battery remaining (Joules) = %E\n",
			E->batts[which].battery_remaining);
	mprint(E, NULL, siminfo, "Maximum Sampled Current Load (mA) = %E\n",
			E->batts[which].maxIload*1000);
	mprint(E, NULL, siminfo, "\n\n");


	return;			
}
