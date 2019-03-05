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

	double		ileak;
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
