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

#include <math.h>
#include "sf.h"
#include "mextern.h"

enum
{
	MAX_VDD_SOLUTIONS = 5,
};

static double	Power(double x, double y);
static double	Root(double x, double y);
static double	Sqrt(double x);
static double	scaledvdd05(Engine *, double delta, double K, double Vt);
static double	scaledvdd06(Engine *, double delta, double K, double Vt);
static double	scaledvdd07(Engine *, double delta, double K, double Vt);
static double	scaledvdd08(Engine *, double delta, double K, double Vt);
static double	scaledvdd09(Engine *, double delta, double K, double Vt);
static double	scaledvdd10(Engine *, double delta, double K, double Vt);
static double	scaledvdd11(Engine *, double delta, double K, double Vt);
static double	scaledvdd12(Engine *, double delta, double K, double Vt);
static double	scaledvdd13(Engine *, double delta, double K, double Vt);
static double	scaledvdd14(Engine *, double delta, double K, double Vt);
static double	scaledvdd15(Engine *, double delta, double K, double Vt);
static double	scaledvdd16(Engine *, double delta, double K, double Vt);
static double	scaledvdd17(Engine *, double delta, double K, double Vt);
static double	scaledvdd18(Engine *, double delta, double K, double Vt);
static double	scaledvdd19(Engine *, double delta, double K, double Vt);
static double	scaledvdd20(Engine *, double delta, double K, double Vt);



double
Power(double x, double y)
{
	return pow(x, y);
}

double
Root(double x, double y)
{
	return pow(x, 1.0/y);
}

double
Sqrt(double x)
{
	return pow(x, 0.5);
}

void
power_printstats(Engine *E, State *S)
{
	if (S->machinetype != MACHINE_SUPERH)
	{
		merror(E, "This machine does not know how to \"powerstats\"");
		return;
	}

	//fprintf(stderr, "Bus lock=%d, locker=%d\n", S->B->pbuslock, S->B->pbuslocker);

	mprint(E, NULL, siminfo, "\nVdd\t= %E\n", S->VDD);
	mprint(E, NULL, siminfo, "Vt\t= %E\n", S->voltscale_Vt);
	mprint(E, NULL, siminfo, "K\t= %E\n", S->voltscale_K);
	mprint(E, NULL, siminfo, "alpha\t= %E\n", S->voltscale_alpha);

	mprint(E, NULL, siminfo,
		"\nCumulative Signal Transitions to date\t= " UVLONGFMT "\n",
		S->energyinfo.ntrans);
	mprint(E, NULL, siminfo,
		"CPU-only Total Energy\t\t\t= %.6E Joules\n",
		S->energyinfo.CPUEtot);

	if (S->ICLK > 0 && S->CYCLETIME > 0)
	{
		mprint(E, NULL, siminfo,
			"CPU-only Average Power\t\t\t= %.6E Watts\n",
			S->energyinfo.CPUEtot / (S->ICLK*S->CYCLETIME));
	}
	mprint(E, NULL, siminfo, "\n");

	return;			
}

void
power_scaledelay(Engine *E, State *S, double Vdd)
{
	int	i;
	double	Vt = S->voltscale_Vt;
	double	K = S->voltscale_K;
	double	alpha = S->voltscale_alpha;
	double	oldcycle;


	/*		Sanity check on arguments			*/
	if (Vdd <= 0.0 || Vt < 0.0 || K < 0.0 || alpha < 0.0)
	{
		mprint(E, NULL, siminfo, "Invalid parameters passed to scale delay: %f %f %f %f\n",
			Vdd, Vt, K, alpha);

		return;
	}
	
	/*
	TODO: 
	*	still need to update latencies on flash_*_l;atency

	*	when we change and start using M[] array for memories, then cleanup 
		and basically update all latencies in M array

	*	need to stop using integer latencies, and define them as doubles
		in absolute time, which makes more sense essp for DVFS


	*	make sure those who depend on miss penalty are correctly updated,
		as well as local_r_latency (partly done the mem_r_latency/mem_w_)
	*/

	S->VDD = Vdd;
	oldcycle = S->CYCLETIME;
	if (S->force_avgpwr == 0.0)
	{
		S->CYCLETIME = K*Vdd/pow(Vdd - Vt, alpha);

		for (i = SUPERH_OP_ADD; i <= SUPERH_OP_XTRCT; i++)
		{
			double reading = (R0000[i].reading1 + R0000[i].reading2)/2;

			/*							*/
			/*	Scaled current, I2 = (I1*V2*t1)/(V1*t2);	*/
			/*							*/
			S->scaledcurrents[i] =
				((reading*S->VDD*SUPERH_ORIG_CYCLE)/(SUPERH_READINGS_VDD*S->CYCLETIME))*1E-3;
		}
	}
	S->mem_r_latency *= (int) ceil(oldcycle/S->CYCLETIME);
	S->mem_w_latency *= (int) ceil(oldcycle/S->CYCLETIME);

	E->mincycpsec = PICOSEC_MAX;
	E->maxcycpsec = 0;
	for (i = 0; i < E->nnodes; i++)
	{
		E->mincycpsec = min(E->mincycpsec, E->sp[i]->CYCLETIME);
		E->maxcycpsec = max(E->maxcycpsec, E->sp[i]->CYCLETIME);
	}

	return;
}

void
power_scalevdd(Engine *E, State *S, double freq)
{
	int	i, alphaX10;
	double	newvdd = -1.0;
	double	delta = 1.0/(freq*1E6);
	double	Vt = S->voltscale_Vt;
	double	K = S->voltscale_K;
	double	alpha = S->voltscale_alpha;


	/*		Sanity check on arguments			*/
	if (freq <= 0.0 || Vt < 0.0 || K < 0.0 || alpha < 0.0)
	{
		mprint(E, NULL, siminfo, "Invalid parameters passed to scale Vdd: %f %f %f %f\n",
			freq, Vt, K, alpha);

		return;
	}	

	/*								*/
	/*	Vdd and delta are related non-algebraically; use 	*/
	/*	Mathematica to solve for vdd for specific values	*/
	/*	of alpha=0.5 to 2.0 in increments of 0.1		*/
	/*		There are multiple solutions; the correct 	*/
	/*	solution is the one which gives Vdd > Vt for the 	*/
	/*	supplied alpha and K. If the scaledvddXX() function	*/
	/*	cannot find a solution it returns -1.0			*/
	/*								*/

	alphaX10 = (int)ceil(alpha*10.0);
	switch (alphaX10)
	{
		case 5:
			newvdd = scaledvdd05(E, delta, K, Vt);
			break;
		case 6:
			newvdd = scaledvdd06(E, delta, K, Vt);
			break;
		case 7:
			newvdd = scaledvdd07(E, delta, K, Vt);
			break;
		case 8:
			newvdd = scaledvdd08(E, delta, K, Vt);
			break;
		case 9:
			newvdd = scaledvdd09(E, delta, K, Vt);
			break;
		case 10:
			newvdd = scaledvdd10(E, delta, K, Vt);
			break;
		case 11:
			newvdd = scaledvdd11(E, delta, K, Vt);
			break;
		case 12:
			newvdd = scaledvdd12(E, delta, K, Vt);
			break;
		case 13:
			newvdd = scaledvdd13(E, delta, K, Vt);
			break;
		case 14:
			newvdd = scaledvdd14(E, delta, K, Vt);
			break;
		case 15:
			newvdd = scaledvdd15(E, delta, K, Vt);
			break;
		case 16:
			newvdd = scaledvdd16(E, delta, K, Vt);
			break;
		case 17:
			newvdd = scaledvdd17(E, delta, K, Vt);
			break;
		case 18:
			newvdd = scaledvdd18(E, delta, K, Vt);
			break;
		case 19:
			newvdd = scaledvdd19(E, delta, K, Vt);
			break;
		case 20:
			newvdd = scaledvdd20(E, delta, K, Vt);
			break;
		default:
			break;
	}

	if (newvdd < 0.0)
	{
		mprint(E, NULL, siminfo,
			"Could not solve for Vdd from given delta, K, alpha and Vt.\n");
		return;
	}

	S->mem_r_latency *= (int) ceil(S->CYCLETIME/delta);
	S->mem_w_latency *= (int) ceil(S->CYCLETIME/delta);
 	S->CYCLETIME = delta;
	if (S->force_avgpwr == 0.0)
	{
		S->VDD = newvdd;

		for (i = SUPERH_OP_ADD; i <= SUPERH_OP_XTRCT; i++)
		{
			double reading = (R0000[i].reading1 + R0000[i].reading2)/2;

			/*							*/
			/*	Scaled current, I2 = (I1*V2*t1)/(V1*t2);	*/
			/*							*/
			S->scaledcurrents[i] =
				((reading*S->VDD*SUPERH_ORIG_CYCLE)/(SUPERH_READINGS_VDD*S->CYCLETIME))*1E-3;
		}
	}

	E->mincycpsec = PICOSEC_MAX;
	E->maxcycpsec = 0;
	for (i = 0; i < E->nnodes; i++)
	{
		E->mincycpsec = min(E->mincycpsec, E->sp[i]->CYCLETIME);
		E->maxcycpsec = max(E->maxcycpsec, E->sp[i]->CYCLETIME);
	}

	return;
}

double
scaledvdd05(Engine *E, double delta, double K, double Vt)
{
	int		i, fsol = 0;
	const int	nsol = 2;
	double		xvdd[2], sol = -1.0;

	/*	alpha = 0.5	*/
	xvdd[0] = (0.5*(Power(delta,2.0) - delta*Sqrt(Power(delta,2.0) - 4.0*Power(K,2.0)*Vt)))/Power(K,2.0);
	xvdd[1] = (0.5*(Power(delta,2.0) + delta*Sqrt(Power(delta,2.0) - 4.0*Power(K,2.0)*Vt)))/Power(K,2.0);

	for (i = 0; i < nsol; i++)
	{
		if (xvdd[i] > Vt)
		{
			sol = xvdd[i];
			fsol++;
		}
	}
	if (SF_DEBUG)
	{
		mprint(E, NULL, siminfo,
			"Found %d solutions for Vdd from delay equation.\n", fsol);
	}

	return sol;
}

double
scaledvdd06(Engine *E, double delta, double K, double Vt)
{
	int		i, q, fsol = 0;
	const int	nsol = 5;
	double		xvdd[5], sol = -1.0;

	/*	alpha = 0.6	*/
	for (q = 1; q <= nsol; q++)
	{
		xvdd[q-1] = Root(Power(delta,5.0)*Power(Vt,3.0) - 
			3.0*Power(delta,5.0)*Power(Vt,2.0)*q + 
        		3.0*Power(delta,5.0)*Vt*Power(q,2.0) - 
			Power(delta,5.0)*Power(q,3.0) + 
			Power(K,5.0)*Power(q,5.0), q);
	}

	for (i = 0; i < nsol; i++)
	{
		if (xvdd[i] > Vt)
		{
			sol = xvdd[i];
			fsol++;
		}
	}
	if (SF_DEBUG)
	{
		mprint(E, NULL, siminfo,
			"Found %d solutions for Vdd from delay equation.\n", fsol);
	}

	return sol;
}

double
scaledvdd07(Engine *E, double delta, double K, double Vt)
{
	int		i, q, fsol = 0;
	const int	nsol = 10;
	double		xvdd[10], sol = -1.0;

	/*	alpha = 0.7	*/
	for (q = 1; q <= nsol; q++)
	{
		xvdd[q-1] = Root(Power(delta,10.0)*Power(Vt,7.0) - 
				7.0*Power(delta,10.0)*Power(Vt,6.0)*q + 
        			21.0*Power(delta,10.0)*Power(Vt,5.0)*Power(q,2.0) - 
				35.0*Power(delta,10.0)*Power(Vt,4.0)*Power(q,3.0) + 
        			35.0*Power(delta,10.0)*Power(Vt,3.0)*Power(q,4.0) - 
				21.0*Power(delta,10.0)*Power(Vt,2.0)*Power(q,5.0) + 
        			7.0*Power(delta,10.0)*Vt*Power(q,6.0) - 
				Power(delta,10.0)*Power(q,7.0) + 
				Power(K,10.0)*Power(q,10.0), q);
	}

	for (i = 0; i < nsol; i++)
	{
		if (xvdd[i] > Vt)
		{
			sol = xvdd[i];
			fsol++;
		}
	}
	if (SF_DEBUG)
	{
		mprint(E, NULL, siminfo,
			"Found %d solutions for Vdd from delay equation.\n", fsol);
	}

	return sol;
}

double
scaledvdd08(Engine *E, double delta, double K, double Vt)
{
	int		i, q, fsol = 0;
	const int	nsol = 5;
	double		xvdd[5], sol = -1.0;

	/*	alpha = 0.8	*/
	for (q = 1; q <= nsol; q++)
	{
		xvdd[q-1] = Root(0.0 - (Power(delta,5.0)*Power(Vt,4.0)) +
				4.0*Power(delta,5.0)*Power(Vt,3.0)*q - 
        			6.0*Power(delta,5.0)*Power(Vt,2.0)*Power(q,2.0) +
				4.0*Power(delta,5.0)*Vt*Power(q,3.0) - 
        			Power(delta,5.0)*Power(q,4.0) +
				Power(K,5.0)*Power(q,5.0), q);
	}

	for (i = 0; i < nsol; i++)
	{
		if (xvdd[i] > Vt)
		{
			sol = xvdd[i];
			fsol++;
		}
	}
	if (SF_DEBUG)
	{
		mprint(E, NULL, siminfo,
			"Found %d solutions for Vdd from delay equation.\n", fsol);
	}

	return sol;
}

double
scaledvdd09(Engine *E, double delta, double K, double Vt)
{
	int		i, q, fsol = 0;
	const int	nsol = 10;
	double		xvdd[10], sol = -1.0;

	/*	alpha = 0.9	*/
	for (q = 1; q <= nsol; q++)
	{
		xvdd[q-1] = Root(Power(delta,10.0)*Power(Vt,9.0) -
				9.0*Power(delta,10.0)*Power(Vt,8.0)*q + 
        			36.0*Power(delta,10.0)*Power(Vt,7.0)*Power(q,2.0) -
				84.0*Power(delta,10.0)*Power(Vt,6.0)*Power(q,3.0) + 
        			126.0*Power(delta,10.0)*Power(Vt,5.0)*Power(q,4.0) -
				126.0*Power(delta,10.0)*Power(Vt,4.0)*Power(q,5.0) + 
        			84.0*Power(delta,10.0)*Power(Vt,3.0)*Power(q,6.0) -
				36.0*Power(delta,10.0)*Power(Vt,2.0)*Power(q,7.0) + 
        			9.0*Power(delta,10.0)*Vt*Power(q,8.0) -
				Power(delta,10.0)*Power(q,9.0) +
				Power(K,10.0)*Power(q,10.0), q);
	}

	for (i = 0; i < nsol; i++)
	{
		if (xvdd[i] > Vt)
		{
			sol = xvdd[i];
			fsol++;
		}
	}
	if (SF_DEBUG)
	{
		mprint(E, NULL, siminfo,
			"Found %d solutions for Vdd from delay equation.\n", fsol);
	}

	return sol;
}

double
scaledvdd10(Engine *E, double delta, double K, double Vt)
{
	int		i, q, fsol = 0;
	const int	nsol = 1;
	double		xvdd[1], sol = -1.0;

	/*	alpha = 1.0	*/
	for (q = 1; q <= nsol; q++)
	{
		xvdd[q-1] = (delta*Vt)/(delta - K);
	}

	for (i = 0; i < nsol; i++)
	{
		if (xvdd[i] > Vt)
		{
			sol = xvdd[i];
			fsol++;
		}
	}
	if (SF_DEBUG)
	{
		mprint(E, NULL, siminfo,
			"Found %d solutions for Vdd from delay equation.\n", fsol);
	}

	return sol;
}

double
scaledvdd11(Engine *E, double delta, double K, double Vt)
{
	int		i, q, fsol = 0;
	const int	nsol = 11;
	double		xvdd[11], sol = -1.0;

	/*	alpha = 1.1	*/
	for (q = 1; q <= nsol; q++)
	{
		xvdd[q-1] = Root(0.0 - (Power(delta,10.0)*Power(Vt,11.0)) +
				11.0*Power(delta,10.0)*Power(Vt,10.0)*q - 
        			55.0*Power(delta,10.0)*Power(Vt,9.0)*Power(q,2.0) + 
				165.0*Power(delta,10.0)*Power(Vt,8.0)*Power(q,3.0) - 
        			330.0*Power(delta,10.0)*Power(Vt,7.0)*Power(q,4.0) + 
				462.0*Power(delta,10.0)*Power(Vt,6.0)*Power(q,5.0) - 
        			462.0*Power(delta,10.0)*Power(Vt,5.0)*Power(q,6.0) + 
				330.0*Power(delta,10.0)*Power(Vt,4.0)*Power(q,7.0) - 
        			165.0*Power(delta,10.0)*Power(Vt,3.0)*Power(q,8.0) + 
				55.0*Power(delta,10.0)*Power(Vt,2.0)*Power(q,9.0) - 
        			Power(K,10.0)*Power(q,10.0) - 
				11.0*Power(delta,10.0)*Vt*Power(q,10.0) + 
				Power(delta,10.0)*Power(q,11.0), q);
	}

	for (i = 0; i < nsol; i++)
	{
		if (xvdd[i] > Vt)
		{
			sol = xvdd[i];
			fsol++;
		}
	}
	if (SF_DEBUG)
	{
		mprint(E, NULL, siminfo,
			"Found %d solutions for Vdd from delay equation.\n", fsol);
	}

	return sol;
}

double
scaledvdd12(Engine *E, double delta, double K, double Vt)
{
	int		i, q, fsol = 0;
	const int	nsol = 6;
	double		xvdd[6], sol = -1.0;

	/*	alpha = 1.2	*/
	for (q = 1; q <= nsol; q++)
	{
		xvdd[q-1] = Root(Power(delta,5.0)*Power(Vt,6.0) -
				6.0*Power(delta,5.0)*Power(Vt,5.0)*q + 
        			15.0*Power(delta,5.0)*Power(Vt,4.0)*Power(q,2.0) -
				20.0*Power(delta,5.0)*Power(Vt,3.0)*Power(q,3.0) + 
        			15.0*Power(delta,5.0)*Power(Vt,2.0)*Power(q,4.0) -
				Power(K,5.0)*Power(q,5.0) - 
        			6.0*Power(delta,5.0)*Vt*Power(q,5.0) +
				Power(delta,5.0)*Power(q,6.0), q);
	}

	for (i = 0; i < nsol; i++)
	{
		if (xvdd[i] > Vt)
		{
			sol = xvdd[i];
			fsol++;
		}
	}
	if (SF_DEBUG)
	{
		mprint(E, NULL, siminfo,
			"Found %d solutions for Vdd from delay equation.\n", fsol);
	}

	return sol;
}

double
scaledvdd13(Engine *E, double delta, double K, double Vt)
{
	int		i, q, fsol = 0;
	const int	nsol = 13;
	double		xvdd[13], sol = -1.0;

	/*	alpha = 1.3	*/
	for (q = 1; q <= nsol; q++)
	{
		xvdd[q-1] = Root(0.0 - (Power(delta,10.0)*Power(Vt,13.0)) +
				13.0*Power(delta,10.0)*Power(Vt,12.0)*q - 
        			78.0*Power(delta,10.0)*Power(Vt,11.0)*Power(q,2.0) +
				286.0*Power(delta,10.0)*Power(Vt,10.0)*Power(q,3.0) - 
        			715.0*Power(delta,10.0)*Power(Vt,9.0)*Power(q,4.0) +
				1287.0*Power(delta,10.0)*Power(Vt,8.0)*Power(q,5.0) - 
        			1716.0*Power(delta,10.0)*Power(Vt,7.0)*Power(q,6.0) +
				1716.0*Power(delta,10.0)*Power(Vt,6.0)*Power(q,7.0) - 
        			1287.0*Power(delta,10.0)*Power(Vt,5.0)*Power(q,8.0) +
				715.0*Power(delta,10.0)*Power(Vt,4.0)*Power(q,9.0) - 
        			Power(K,10.0)*Power(q,10.0) -
				286.0*Power(delta,10.0)*Power(Vt,3.0)*Power(q,10.0) + 
        			78.0*Power(delta,10.0)*Power(Vt,2.0)*Power(q,11.0) -
				13.0*Power(delta,10.0)*Vt*Power(q,12.0) + 
        			Power(delta,10.0)*Power(q,13.0), q);
	}

	for (i = 0; i < nsol; i++)
	{
		if (xvdd[i] > Vt)
		{
			sol = xvdd[i];
			fsol++;
		}
	}
	if (SF_DEBUG)
	{
		mprint(E, NULL, siminfo,
			"Found %d solutions for Vdd from delay equation.\n", fsol);
	}

	return sol;
}

double
scaledvdd14(Engine *E, double delta, double K, double Vt)
{
	int		i, q, fsol = 0;
	const int	nsol = 7;
	double		xvdd[7], sol = -1.0;

	/*	alpha = 1.4	*/
	for (q = 1; q <= nsol; q++)
	{
		xvdd[q-1] = Root(0.0 - (Power(delta,5.0)*Power(Vt,7.0)) +
				7.0*Power(delta,5.0)*Power(Vt,6.0)*q - 
        			21.0*Power(delta,5.0)*Power(Vt,5.0)*Power(q,2.0) +
				35.0*Power(delta,5.0)*Power(Vt,4.0)*Power(q,3.0) - 
        			35.0*Power(delta,5.0)*Power(Vt,3.0)*Power(q,4.0) -
				Power(K,5.0)*Power(q,5.0) + 
        			21.0*Power(delta,5.0)*Power(Vt,2.0)*Power(q,5.0) -
				7.0*Power(delta,5.0)*Vt*Power(q,6.0) + 
        			Power(delta,5.0)*Power(q,7.0), q);
	}

	for (i = 0; i < nsol; i++)
	{
		if (xvdd[i] > Vt)
		{
			sol = xvdd[i];
			fsol++;
		}
	}
	if (SF_DEBUG)
	{
		mprint(E, NULL, siminfo,
			"Found %d solutions for Vdd from delay equation.\n", fsol);
	}

	return sol;
}

double
scaledvdd15(Engine *E, double delta, double K, double Vt)
{
	int		i, q, fsol = 0;
	const int	nsol = 1;
	double		xvdd[1], sol = -1.0;

	/*	alpha = 1.5	*/
	for (q = 1; q <= nsol; q++)
	{
		xvdd[q-1] = (0.0-0.3333333333333333*(0.0 - Power(K,2.0) - 3.0*Power(delta,2.0)*Vt))/Power(delta,2.0) -
				(0.41997368329829105*(0.0 - Power(K,4) -
					6.0*Power(delta,2.0)*Power(K,2.0)*Vt))/
       				(Power(delta,2.0)*Power(2.0*Power(K,6.0) + 
					18.0*Power(delta,2.0)*Power(K,4.0)*Vt + 
           				27.0*Power(delta,4.0)*Power(K,2.0)*Power(Vt,2.0) + 
           				10.392304845413264*Power(K,2.0)*Sqrt(Power(delta,6.0)*Power(K,2.0)*Power(Vt,3.0) +
					6.75*Power(delta,8.0)*Power(Vt,4.0)),
          			0.3333333333333333)) +
			(0.26456684199469993*Power(2.0*Power(K,6.0) + 
					18.0*Power(delta,2.0)*Power(K,4.0)*Vt +
					27.0*Power(delta,4.0)*Power(K,2.0)*Power(Vt,2.0) +
					10.392304845413264*Power(K,2.0)*
            				Sqrt(Power(delta,6.0)*Power(K,2.0)*Power(Vt,3.0) +
					6.75*Power(delta,8.0)*Power(Vt,4.0)),
				0.3333333333333333))/
       			Power(delta,2.0);
	}

	for (i = 0; i < nsol; i++)
	{
		if (xvdd[i] > Vt)
		{
			sol = xvdd[i];
			fsol++;
		}
	}
	if (SF_DEBUG)
	{
		mprint(E, NULL, siminfo,
			"Found %d solutions for Vdd from delay equation.\n", fsol);
	}

	return sol;
}

double
scaledvdd16(Engine *E, double delta, double K, double Vt)
{
	int		i, q, fsol = 0;
	const int	nsol = 8;
	double		xvdd[8], sol = -1.0;

	/*	alpha = 1.6	*/
	for (q = 1; q <= nsol; q++)
	{
		xvdd[q-1] = Root(Power(delta,5.0)*Power(Vt,8.0) -
			8.0*Power(delta,5.0)*Power(Vt,7.0)*q + 
        		28.0*Power(delta,5.0)*Power(Vt,6.0)*Power(q,2.0) -
			56.0*Power(delta,5.0)*Power(Vt,5.0)*Power(q,3.0) + 
        		70.0*Power(delta,5.0)*Power(Vt,4.0)*Power(q,4.0) -
			Power(K,5.0)*Power(q,5.0) -
			56.0*Power(delta,5.0)*Power(Vt,3.0)*Power(q,5.0) +
			28.0*Power(delta,5.0)*Power(Vt,2.0)*Power(q,6.0) - 
        		8.0*Power(delta,5.0)*Vt*Power(q,7.0) +
			Power(delta,5.0)*Power(q,8.0), q);
	}

	for (i = 0; i < nsol; i++)
	{
		if (xvdd[i] > Vt)
		{
			sol = xvdd[i];
			fsol++;
		}
	}
	if (SF_DEBUG)
	{
		mprint(E, NULL, siminfo,
			"Found %d solutions for Vdd from delay equation.\n", fsol);
	}

	return sol;
}

double
scaledvdd17(Engine *E, double delta, double K, double Vt)
{
	int		i, q, fsol = 0;
	const int	nsol = 17;
	double		xvdd[17], sol = -1.0;

	/*	alpha = 1.7	*/
	for (q = 1; q <= nsol; q++)
	{
		xvdd[q-1] = Root(0.0 - (Power(delta,10.0)*Power(Vt,17.0)) +
			17.0*Power(delta,10.0)*Power(Vt,16.0)*q - 
        		136.0*Power(delta,10.0)*Power(Vt,15.0)*Power(q,2.0) +
			680.0*Power(delta,10.0)*Power(Vt,14.0)*Power(q,3.0) - 
        		2380.0*Power(delta,10.0)*Power(Vt,13.0)*Power(q,4.0) +
			6188.0*Power(delta,10.0)*Power(Vt,12.0)*Power(q,5.0) - 
        		12376.0*Power(delta,10.0)*Power(Vt,11.0)*Power(q,6.0) +
			19448.0*Power(delta,10.0)*Power(Vt,10.0)*Power(q,7.0) - 
        		24310.0*Power(delta,10.0)*Power(Vt,9.0)*Power(q,8.0) +
			24310.0*Power(delta,10.0)*Power(Vt,8.0)*Power(q,9.0) - 
        		Power(K,10.0)*Power(q,10.0) -
			19448.0*Power(delta,10.0)*Power(Vt,7.0)*Power(q,10.0) + 
        		12376.0*Power(delta,10.0)*Power(Vt,6.0)*Power(q,11.0) -
			6188.0*Power(delta,10.0)*Power(Vt,5.0)*Power(q,12.0) + 
        		2380.0*Power(delta,10.0)*Power(Vt,4.0)*Power(q,13.0) -
			680.0*Power(delta,10.0)*Power(Vt,3.0)*Power(q,14.0) + 
        		136.0*Power(delta,10.0)*Power(Vt,2.0)*Power(q,15.0) -
			17.0*Power(delta,10.0)*Vt*Power(q,16.0) + 
        		Power(delta,10.0)*Power(q,17.0), q);
	}

	for (i = 0; i < nsol; i++)
	{
		if (xvdd[i] > Vt)
		{
			sol = xvdd[i];
			fsol++;
		}
	}
	if (SF_DEBUG)
	{
		mprint(E, NULL, siminfo,
			"Found %d solutions for Vdd from delay equation.\n", fsol);
	}

	return sol;
}

double
scaledvdd18(Engine *E, double delta, double K, double Vt)
{
	int		i, q, fsol = 0;
	const int	nsol = 9;
	double		xvdd[9], sol = -1.0;

	/*	alpha = 1.8	*/
	for (q = 1; q <= nsol; q++)
	{
		xvdd[q-1] = Root(0.0 - (Power(delta,5.0)*Power(Vt,9.0)) +
			9.0*Power(delta,5.0)*Power(Vt,8.0)*q - 
        		36.0*Power(delta,5.0)*Power(Vt,7.0)*Power(q,2.0) +
			84.0*Power(delta,5.0)*Power(Vt,6.0)*Power(q,3.0) - 
        		126.0*Power(delta,5.0)*Power(Vt,5.0)*Power(q,4.0) -
			Power(K,5.0)*Power(q,5.0) + 
        		126.0*Power(delta,5.0)*Power(Vt,4.0)*Power(q,5.0) -
			84.0*Power(delta,5.0)*Power(Vt,3.0)*Power(q,6.0) + 
        		36.0*Power(delta,5.0)*Power(Vt,2.0)*Power(q,7.0) -
			9.0*Power(delta,5.0)*Vt*Power(q,8.0) + 
        		Power(delta,5.0)*Power(q,9.0), q);
	}

	for (i = 0; i < nsol; i++)
	{
		if (xvdd[i] > Vt)
		{
			sol = xvdd[i];
			fsol++;
		}
	}
	if (SF_DEBUG)
	{
		mprint(E, NULL, siminfo,
			"Found %d solutions for Vdd from delay equation.\n", fsol);
	}

	return sol;
}

double
scaledvdd19(Engine *E, double delta, double K, double Vt)
{
	int		i, q, fsol = 0;
	const int	nsol = 19;
	double		xvdd[19], sol = -1.0;

	/*	alpha = 1.9	*/
	for (q = 1; q <= nsol; q++)
	{
		xvdd[q-1] = Root(0.0 - (Power(delta,10.0)*Power(Vt,19.0)) +
			19.0*Power(delta,10.0)*Power(Vt,18.0)*q - 
        		171.0*Power(delta,10.0)*Power(Vt,17.0)*Power(q,2.0) +
			969.0*Power(delta,10.0)*Power(Vt,16.0)*Power(q,3.0) - 
        		3876.0*Power(delta,10.0)*Power(Vt,15.0)*Power(q,4.0) +
			11628.0*Power(delta,10.0)*Power(Vt,14.0)*Power(q,5.0) - 
        		27132.0*Power(delta,10.0)*Power(Vt,13.0)*Power(q,6.0) +
			50388.0*Power(delta,10.0)*Power(Vt,12.0)*Power(q,7.0) - 
        		75582.0*Power(delta,10.0)*Power(Vt,11.0)*Power(q,8.0) +
			92378.0*Power(delta,10.0)*Power(Vt,10.0)*Power(q,9.0) - 
        		Power(K,10.0)*Power(q,10.0) -
			92378.0*Power(delta,10.0)*Power(Vt,9.0)*Power(q,10.0) + 
        		75582.0*Power(delta,10.0)*Power(Vt,8.0)*Power(q,11.0) -
			50388.0*Power(delta,10.0)*Power(Vt,7.0)*Power(q,12.0) + 
        		27132.0*Power(delta,10.0)*Power(Vt,6.0)*Power(q,13.0) -
			11628.0*Power(delta,10.0)*Power(Vt,5.0)*Power(q,14.0) + 
        		3876.0*Power(delta,10.0)*Power(Vt,4.0)*Power(q,15.0) -
			969.0*Power(delta,10.0)*Power(Vt,3.0)*Power(q,16.0) + 
        		171.0*Power(delta,10.0)*Power(Vt,2.0)*Power(q,17.0) -
			19.0*Power(delta,10.0)*Vt*Power(q,18.0) + 
        		Power(delta,10.0)*Power(q,19.0), q);
	}

	for (i = 0; i < nsol; i++)
	{
		if (xvdd[i] > Vt)
		{
			sol = xvdd[i];
			fsol++;
		}
	}
	if (SF_DEBUG)
	{
		mprint(E, NULL, siminfo,
			"Found %d solutions for Vdd from delay equation.\n", fsol);
	}

	return sol;
}

double
scaledvdd20(Engine *E, double delta, double K, double Vt)
{
	int		i, fsol = 0;
	const int	nsol = 2;
	double		xvdd[2], sol = -1.0;

	/*	alpha = 2.0	*/
	xvdd[0] = (0.5*(K + 2.0*delta*Vt - 2.0*Sqrt(0.25*Power(K,2.0) + delta*K*Vt)))/delta;
   	xvdd[1] = (0.5*(K + 2.0*delta*Vt + 2.0*Sqrt(0.25*Power(K,2.0) + delta*K*Vt)))/delta;

	for (i = 0; i < nsol; i++)
	{
		if (xvdd[i] > Vt)
		{
			sol = xvdd[i];
			fsol++;
		}
	}
	if (SF_DEBUG)
	{
		mprint(E, NULL, siminfo,
			"Found %d solutions for Vdd from delay equation.\n", fsol);
	}

	return sol;
}

double
check_delta(double Vdd, double Vt, double K, double alpha)
{
	return K*Vdd/pow(Vdd - Vt, alpha);
}
