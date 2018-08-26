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
#include <string.h>
#include "sf.h"


/*										*/
/*	mrandominit(), and mrandom() implement the Matsumoto-Nishimura		*/
/*	Mersenne-Twister RNG.  The implementation is taken from the 		*/
/*	following paper:							*/
/*										*/
/*		64-bit version:							*/
/*			@article{369540,					*/
/* 			author = {Takuji Nishimura},				*/
/* 			title = {Tables of 64-bit Mersenne twisters},		*/
/* 			journal = {ACM Trans. Model. Comput. Simul.},		*/
/* 			volume = {10},						*/
/* 			number = {4},						*/
/* 			year = {2000},						*/
/* 			issn = {1049-3301},					*/
/* 			pages = {348--357},					*/
/* 			doi = {http://doi.acm.org/10.1145/369534.369540},	*/
/* 			publisher = {ACM Press},				*/
/* 			address = {New York, NY, USA},				*/
/* 			}							*/
/*										*/
/*		Original version:						*/
/*			@article{272995,					*/
/* 			author = {Makoto Matsumoto and Takuji Nishimura},	*/
/* 			title = {Mersenne twister: a 623-dimensionally		*/
/*				equidistributed uniform pseudo-random number	*/
/*				generator},					*/
/* 			journal = {ACM Trans. Model. Comput. Simul.},		*/
/* 			volume = {8},						*/
/* 			number = {1},						*/
/* 			year = {1998},						*/
/* 			issn = {1049-3301},					*/
/* 			pages = {3--30},					*/
/* 			doi = {http://doi.acm.org/10.1145/272991.272995},	*/
/* 			publisher = {ACM Press},				*/
/*			address = {New York, NY, USA},				*/
/* 			}							*/
/*										*/
/*	Comments labeled "M-N" are from original paper, possibly with minor	*/
/*	edits to reflect our changes to the code (e.g., function names).	*/
/*										*/
/*										*/


static tuck double	beta(double p, double q);
static tuck double	factorial(double n);
static tuck double	binomial(double n, double k);
static tuck double	e(double x);
static tuck double	u(Engine *, double min, double max);

static tuck int		disttabname2id(Engine *, char *distname);
static tuck int		pfunname2id(char *pfunname);
static tuck int		simrvarname2id(char *rvarname);
static tuck void	allocrandtable(Engine *E, RandTable *rt, double (*pr)(Engine *, double, double, double, double, double),
				double basis_start, double basis_end, double granularity,
				double p1, double p2, double p3, double p4);


SimRvar SF_RVAR_NAMES[] =
{
	/*				*/
	/*	SuperH-specific		*/
	/*				*/
	[SF_RVAR_flash_r_latency]		{SF_RVAR_TYPE_int, "flash_r_latency"},
	[SF_RVAR_flash_w_latency]		{SF_RVAR_TYPE_int, "flash_w_latency"},
	[SF_RVAR_cache_activated]		{SF_RVAR_TYPE_int, "cache_activated"},
	[SF_RVAR_ENABLE_CLK_INTR]		{SF_RVAR_TYPE_int, "ENABLE_CLK_INTR"},
	[SF_RVAR_rxok_intrenable_flag]		{SF_RVAR_TYPE_int, "rxok_intrenable_flag"},
	[SF_RVAR_txok_intrenable_flag]		{SF_RVAR_TYPE_int, "txok_intrenable_flag"},
	[SF_RVAR_addrerr_intrenable_flag]	{SF_RVAR_TYPE_int, "addrerr_intrenable_flag"},
	[SF_RVAR_frameerr_intrenable_flag]	{SF_RVAR_TYPE_int, "frameerr_intrenable_flag"},
	[SF_RVAR_collserr_intrenable_flag]	{SF_RVAR_TYPE_int, "collserr_intrenable_flag"},
	[SF_RVAR_csenseerr_intrenable_flag]	{SF_RVAR_TYPE_int, "csenseerr_intrenable_flag"},
	[SF_RVAR_rxovrrunerr_intrenable_flag]	{SF_RVAR_TYPE_int, "rxovrrunerr_intrenable_flag"},
	[SF_RVAR_txovrrunerr_intrenable_flag]	{SF_RVAR_TYPE_int, "txovrrunerr_intrenable_flag"},
	[SF_RVAR_rxundrrunerr_intrenable_flag]	{SF_RVAR_TYPE_int, "rxundrrunerr_intrenable_flag"},
	[SF_RVAR_txundrrunerr_intrenable_flag]	{SF_RVAR_TYPE_int, "txundrrunerr_intrenable_flag"},
	[SF_RVAR_csumerr_intrenable_flag]	{SF_RVAR_TYPE_int, "csumerr_intrenable_flag"},
	[SF_RVAR_TIMER_INTR_DELAY]		{SF_RVAR_TYPE_double, "TIMER_INTR_DELAY"},
	[SF_RVAR_TRA]				{SF_RVAR_TYPE_ulong, "TRA"},
	[SF_RVAR_EXPEVT]			{SF_RVAR_TYPE_ulong, "EXPEVT"},
	[SF_RVAR_INTEVT]			{SF_RVAR_TYPE_ulong, "INTEVT"},
	[SF_RVAR_PTEH]				{SF_RVAR_TYPE_ulong, "PTEH"},
	[SF_RVAR_PTEL]				{SF_RVAR_TYPE_ulong, "PTEL"},
	[SF_RVAR_TTB]				{SF_RVAR_TYPE_ulong, "TTB"},
	[SF_RVAR_TEA]				{SF_RVAR_TYPE_ulong, "TEA"},
	[SF_RVAR_MMUCR]				{SF_RVAR_TYPE_ulong, "MMUCR"},
	[SF_RVAR_ICR]				{SF_RVAR_TYPE_ulong, "ICR"},
	[SF_RVAR_ICRA]				{SF_RVAR_TYPE_ulong, "ICRA"},
	[SF_RVAR_ICRB]				{SF_RVAR_TYPE_ulong, "ICRB"},
	[SF_RVAR_R0_BANK0]			{SF_RVAR_TYPE_ulong, "R0_BANK0"},
	[SF_RVAR_R1_BANK0]			{SF_RVAR_TYPE_ulong, "R1_BANK0"},
	[SF_RVAR_R2_BANK0]			{SF_RVAR_TYPE_ulong, "R2_BANK0"},
	[SF_RVAR_R3_BANK0]			{SF_RVAR_TYPE_ulong, "R3_BANK0"},
	[SF_RVAR_R4_BANK0]			{SF_RVAR_TYPE_ulong, "R4_BANK0"},
	[SF_RVAR_R5_BANK0]			{SF_RVAR_TYPE_ulong, "R5_BANK0"},
	[SF_RVAR_R6_BANK0]			{SF_RVAR_TYPE_ulong, "R6_BANK0"},
	[SF_RVAR_R7_BANK0]			{SF_RVAR_TYPE_ulong, "R7_BANK0"},
	[SF_RVAR_R0_BANK1]			{SF_RVAR_TYPE_ulong, "R0_BANK1"},
	[SF_RVAR_R1_BANK1]			{SF_RVAR_TYPE_ulong, "R1_BANK1"},
	[SF_RVAR_R2_BANK1]			{SF_RVAR_TYPE_ulong, "R2_BANK1"},
	[SF_RVAR_R3_BANK1]			{SF_RVAR_TYPE_ulong, "R3_BANK1"},
	[SF_RVAR_R4_BANK1]			{SF_RVAR_TYPE_ulong, "R4_BANK1"},
	[SF_RVAR_R5_BANK1]			{SF_RVAR_TYPE_ulong, "R5_BANK1"},
	[SF_RVAR_R6_BANK1]			{SF_RVAR_TYPE_ulong, "R6_BANK1"},
	[SF_RVAR_R7_BANK1]			{SF_RVAR_TYPE_ulong, "R7_BANK1"},
	[SF_RVAR_R8]				{SF_RVAR_TYPE_ulong, "R8"},
	[SF_RVAR_R9]				{SF_RVAR_TYPE_ulong, "R9"},
	[SF_RVAR_R10]				{SF_RVAR_TYPE_ulong, "R10"},
	[SF_RVAR_R11]				{SF_RVAR_TYPE_ulong, "R11"},
	[SF_RVAR_R12]				{SF_RVAR_TYPE_ulong, "R12"},
	[SF_RVAR_R13]				{SF_RVAR_TYPE_ulong, "R13"},
	[SF_RVAR_R14]				{SF_RVAR_TYPE_ulong, "R14"},
	[SF_RVAR_R15]				{SF_RVAR_TYPE_ulong, "R15"},
	[SF_RVAR_MACH]				{SF_RVAR_TYPE_ulong, "MACH"},
	[SF_RVAR_MACL]				{SF_RVAR_TYPE_ulong, "MACL"},
	[SF_RVAR_SSR]				{SF_RVAR_TYPE_ulong, "SSR"},
	[SF_RVAR_SR]				{SF_RVAR_TYPE_ulong, "SR"},
	[SF_RVAR_PR]				{SF_RVAR_TYPE_ulong, "PR"},
	[SF_RVAR_SPC]				{SF_RVAR_TYPE_ulong, "SPC"},
	[SF_RVAR_GBR]				{SF_RVAR_TYPE_ulong, "GBR"},
	[SF_RVAR_VBR]				{SF_RVAR_TYPE_ulong, "VBR"},




	/*				*/
	/*	MSP430-specific		*/
	/*				*/





	/*				*/
	/*	Node-specific		*/
	/*				*/
	[SF_RVAR_mem_r_latency]			{SF_RVAR_TYPE_int, "mem_r_latency"},
	[SF_RVAR_mem_w_latency]			{SF_RVAR_TYPE_int, "mem_w_latency"},
	[SF_RVAR_MEMSIZE]			{SF_RVAR_TYPE_int, "MEMSIZE"},
	[SF_RVAR_MEMBASE]			{SF_RVAR_TYPE_int, "MEMBASE"},
	[SF_RVAR_MEMEND]			{SF_RVAR_TYPE_int, "MEMEND"},
	[SF_RVAR_MEMbyte]			{SF_RVAR_TYPE_char, "MEMbyte"},
	[SF_RVAR_MEMword]			{SF_RVAR_TYPE_short, "MEMword"},
	[SF_RVAR_MEMlongword]			{SF_RVAR_TYPE_ulong, "MEMlongword"},
	[SF_RVAR_VDD]				{SF_RVAR_TYPE_double, "VDD"},
	[SF_RVAR_LOWVDD]			{SF_RVAR_TYPE_double, "LOWVDD"},		
	[SF_RVAR_SVDD]				{SF_RVAR_TYPE_double, "SVDD"},			
	[SF_RVAR_temperature]			{SF_RVAR_TYPE_double, "temperature"},
	[SF_RVAR_voltscale_alpha]		{SF_RVAR_TYPE_double, "voltscale_alpha"},
	[SF_RVAR_voltscale_Vt]			{SF_RVAR_TYPE_double, "voltscale_Vt"},
	[SF_RVAR_voltscale_K]			{SF_RVAR_TYPE_double, "voltscale_K"},
	[SF_RVAR_Sensor_reading]		{SF_RVAR_TYPE_float, "Sensor.reading"},
	[SF_RVAR_force_avgpwr]			{SF_RVAR_TYPE_double, "force_avgpwr"},
	[SF_RVAR_force_sleeppwr]		{SF_RVAR_TYPE_double, "force_sleeppwr"},
	[SF_RVAR_CYCLETIME]			{SF_RVAR_TYPE_double, "CYCLETIME"},
	[SF_RVAR_PC]				{SF_RVAR_TYPE_ulong, "PC"},
	[SF_RVAR_STARTPC]			{SF_RVAR_TYPE_ulong, "STARTPC"},
	[SF_RVAR_NODE_ID]			{SF_RVAR_TYPE_int, "NODE_ID"},
	[SF_RVAR_fail_clocks_left]		{SF_RVAR_TYPE_uvlong, "fail_clocks_left"},
	[SF_RVAR_got_correlated_failure]	{SF_RVAR_TYPE_int, "got_correlated_failure"},
	[SF_RVAR_faultthreshold]		{SF_RVAR_TYPE_uvlong, "faultthreshold"},
	[SF_RVAR_ENABLE_TOO_MANY_FAULTS]	{SF_RVAR_TYPE_int, "ENABLE_TOO_MANY_FAULTS"},
	[SF_RVAR_clock_modulus]			{SF_RVAR_TYPE_int, "clock_modulus"},
	[SF_RVAR_ENABLE_BATT_LOW_INTR]		{SF_RVAR_TYPE_int, "ENABLE_BATT_LOW_INTR"},
	[SF_RVAR_battery_alert_frac]		{SF_RVAR_TYPE_double, "battery_alert_frac"},
	[SF_RVAR_runnable]			{SF_RVAR_TYPE_int, "runnable"},
	[SF_RVAR_pipelined]			{SF_RVAR_TYPE_int, "pipelined"},
	[SF_RVAR_pipeshow]			{SF_RVAR_TYPE_int, "pipeshow"},
	[SF_RVAR_sleep]				{SF_RVAR_TYPE_int, "sleep"},
	[SF_RVAR_orbit]				{SF_RVAR_TYPE_int, "orbit"},
	[SF_RVAR_velocity]			{SF_RVAR_TYPE_double, "velocity"},
	[SF_RVAR_xloc]				{SF_RVAR_TYPE_double, "xloc"},
	[SF_RVAR_yloc]				{SF_RVAR_TYPE_double, "yloc"},
	[SF_RVAR_zloc]				{SF_RVAR_TYPE_double, "zloc"},



	/*				*/
	/*	Global state		*/
	/*				*/
	[SF_RVAR_SIM_THROTTLE_NANOSEC]		{SF_RVAR_TYPE_ulong, "SIM_THROTTLE_NANOSEC"},
	[SF_RVAR_SIM_THROTTLE_WINDOW]		{SF_RVAR_TYPE_ulong, "SIM_THROTTLE_WINDOW"},
	[SF_RVAR_SIM_QUANTUM]			{SF_RVAR_TYPE_int, "SIM_QUANTUM"},
	[SF_RVAR_SIM_BASENODEID]		{SF_RVAR_TYPE_int, "SIM_BASENODEID"},
	[SF_RVAR_CUR_NODE_ID]			{SF_RVAR_TYPE_int, "CUR_NODE_ID"},
	[SF_RVAR_SIM_ON]			{SF_RVAR_TYPE_int, "SIM_ON"},
	[SF_RVAR_SIM_BATT_LEAK_CURRENT]		{SF_RVAR_TYPE_double, "SIM_BATT_LEAK_CURRENT"},
	[SF_RVAR_SIM_FAULT_PERIOD]		{SF_RVAR_TYPE_double, "SIM_FAULT_PERIOD"},
	[SF_RVAR_SIM_PHYSICS_PERIOD]		{SF_RVAR_TYPE_double, "SIM_PHYSICS_PERIOD"},
	[SF_RVAR_SIM_BATTFEED_PERIOD]		{SF_RVAR_TYPE_double, "SIM_BATTFEED_PERIOD"},
	[SF_RVAR_SIM_DUMPPWR_PERIOD]		{SF_RVAR_TYPE_double, "SIM_DUMPPWR_PERIOD"},
	[SF_RVAR_Signalsrc_valid]		{SF_RVAR_TYPE_int, "Signalsrc.valid"},
	[SF_RVAR_Signalsrc_ID]			{SF_RVAR_TYPE_int, "Signalsrc.ID"},
	[SF_RVAR_Signalsrc_type]		{SF_RVAR_TYPE_int, "Signalsrc.type"},
	[SF_RVAR_Signalsrc_updates_tau]		{SF_RVAR_TYPE_double, "Signalsrc.updates_tau"},
	[SF_RVAR_Signalsrc_propagation_speed]	{SF_RVAR_TYPE_double, "Signalsrc.propagation_speed"},
	[SF_RVAR_Signalsrc_const_coeff_A]	{SF_RVAR_TYPE_double, "Signalsrc.const_coeff_A"},
	[SF_RVAR_Signalsrc_const_coeff_B]	{SF_RVAR_TYPE_double, "Signalsrc.const_coeff_B"},
	[SF_RVAR_Signalsrc_const_coeff_C]	{SF_RVAR_TYPE_double, "Signalsrc.const_coeff_C"},
	[SF_RVAR_Signalsrc_const_coeff_D]	{SF_RVAR_TYPE_double, "Signalsrc.const_coeff_D"},
	[SF_RVAR_Signalsrc_const_coeff_E]	{SF_RVAR_TYPE_double, "Signalsrc.const_coeff_E"},
	[SF_RVAR_Signalsrc_const_coeff_F]	{SF_RVAR_TYPE_double, "Signalsrc.const_coeff_F"},
	[SF_RVAR_Signalsrc_const_coeff_G]	{SF_RVAR_TYPE_double, "Signalsrc.const_coeff_G"},
	[SF_RVAR_Signalsrc_const_coeff_H]	{SF_RVAR_TYPE_double, "Signalsrc.const_coeff_H"},
	[SF_RVAR_Signalsrc_const_coeff_I]	{SF_RVAR_TYPE_double, "Signalsrc.const_coeff_I"},
	[SF_RVAR_Signalsrc_const_base_K]	{SF_RVAR_TYPE_double, "Signalsrc.const_base_K"},
	[SF_RVAR_Signalsrc_const_pow_m]		{SF_RVAR_TYPE_double, "Signalsrc.const_pow_m"},
	[SF_RVAR_Signalsrc_const_pow_n]		{SF_RVAR_TYPE_double, "Signalsrc.const_pow_n"},
	[SF_RVAR_Signalsrc_const_pow_o]		{SF_RVAR_TYPE_double, "Signalsrc.const_pow_o"},
	[SF_RVAR_Signalsrc_const_pow_p]		{SF_RVAR_TYPE_double, "Signalsrc.const_pow_p"},
	[SF_RVAR_Signalsrc_const_pow_q]		{SF_RVAR_TYPE_double, "Signalsrc.const_pow_q"},
	[SF_RVAR_Signalsrc_const_pow_r]		{SF_RVAR_TYPE_double, "Signalsrc.const_pow_r"},
	[SF_RVAR_Signalsrc_const_pow_s]		{SF_RVAR_TYPE_double, "Signalsrc.const_pow_s"},
	[SF_RVAR_Signalsrc_const_pow_t]		{SF_RVAR_TYPE_double, "Signalsrc.const_pow_t"},
	[SF_RVAR_Signalsrc_xloc]		{SF_RVAR_TYPE_double, "Signalsrc.xloc"},
	[SF_RVAR_Signalsrc_yloc]		{SF_RVAR_TYPE_double, "Signalsrc.yloc"},
	[SF_RVAR_Signalsrc_zloc]		{SF_RVAR_TYPE_double, "Signalsrc.zloc"},
	[SF_RVAR_Signalsrc_trajectory_rate]	{SF_RVAR_TYPE_int, "Signalsrc.trajectory_rate"},
	[SF_RVAR_Signalsrc_looptrajectory]	{SF_RVAR_TYPE_int, "Signalsrc.looptrajectory"},
	[SF_RVAR_Signalsrc_sample]		{SF_RVAR_TYPE_double, "Signalsrc.sample"},
	[SF_RVAR_Signalsrc_sample_rate]		{SF_RVAR_TYPE_int, "Signalsrc.sample_rate"},
	[SF_RVAR_Signalsrc_loopsamples]		{SF_RVAR_TYPE_int, "Signalsrc.loopsamples"},
};

/*												*/
/*			And now, a few constant array definitions / tables			*/
/*												*/
char *
SF_PFUN_NAMES[] =
{
	[SF_PFUN_UNIFORM]	"UNIFORM",
	[SF_PFUN_EXP]		"EXP",
	[SF_PFUN_GAUSS]		"GAUSS",
	[SF_PFUN_BPARETO]	"BPARETO",
	[SF_PFUN_WEIBULL]	"WEIBULL",
	[SF_PFUN_XI2]		"XI2",
	[SF_PFUN_CAUCHY]	"CAUCHY",
	[SF_PFUN_XI]		"CHI",
	[SF_PFUN_BETA]		"BETA",
	[SF_PFUN_EXTREMEVAL]	"EXTREMEVAL",
	[SF_PFUN_F]		"F",
	[SF_PFUN_FERMIDIRAC]	"FERMIDIRAC",
	[SF_PFUN_FISHERZ]	"FISHERZ",
	[SF_PFUN_GUMBEL]	"GUMBEL",
	[SF_PFUN_GAMMA]		"GAMMA",
	[SF_PFUN_HALFNORMAL]	"HALFNORMAL",
	[SF_PFUN_LAPLACE]	"LAPLACE",
	[SF_PFUN_LOGSERIES]	"LOGSERIES",
	[SF_PFUN_LOGISTIC]	"LOGISTIC",
	[SF_PFUN_MAXWELL]	"MAXWELL",
	[SF_PFUN_NEGBINOMIAL]	"NEGBINOMIAL",
	[SF_PFUN_PEARSONTYPE3]	"PEARSONTYPE3",
	[SF_PFUN_POISSON]	"POISSON",
	[SF_PFUN_RAYLEIGH]	"RAYLEIGH",
	[SF_PFUN_STUDENTST]	"STUDENTST",
	[SF_PFUN_STUDENTSZ]	"STUDENTSZ",
	[SF_PFUN_BETAPRIME]	"BETAPRIME",
	[SF_PFUN_ERLANG]	"ERLANG",
	[SF_PFUN_LOGNORMAL]	"LOGNORMAL",
	[SF_PFUN_GIBRAT]	"GIBRAT",
	[SF_PFUN_BATHTUB]	"BATHTUB",
};


RandgenPrfptr SF_PR_FPTRS[] =
{
	[SF_PFUN_UNIFORM]	m_pr_uniform,
	[SF_PFUN_EXP]		m_pr_exp,
	[SF_PFUN_GAUSS]		m_pr_gauss,
//	[SF_PFUN_BPARETO]	m_pr_bpareto,
	[SF_PFUN_WEIBULL]	m_pr_weibull,
	[SF_PFUN_XI2]		m_pr_xi2,
	[SF_PFUN_CAUCHY]	m_pr_cauchy,
	[SF_PFUN_XI]		m_pr_xi,
	[SF_PFUN_BETA]		m_pr_beta,
	[SF_PFUN_EXTREMEVAL]	m_pr_extremeval,
	[SF_PFUN_F]		m_pr_f,
	[SF_PFUN_FERMIDIRAC]	m_pr_fermidirac,
	[SF_PFUN_FISHERZ]	m_pr_fisherz,
	[SF_PFUN_GUMBEL]	m_pr_gumbel,
	[SF_PFUN_GAMMA]		m_pr_gamma,
	[SF_PFUN_HALFNORMAL]	m_pr_halfnormal,
	[SF_PFUN_LAPLACE]	m_pr_laplace,
	[SF_PFUN_LOGSERIES]	m_pr_logseries,
	[SF_PFUN_LOGISTIC]	m_pr_logistic,
	[SF_PFUN_MAXWELL]	m_pr_maxwell,
	[SF_PFUN_NEGBINOMIAL]	m_pr_negbinomial,
	[SF_PFUN_PEARSONTYPE3]	m_pr_pearsontype3,
	[SF_PFUN_POISSON]	m_pr_poisson,
	[SF_PFUN_RAYLEIGH]	m_pr_rayleigh,
	[SF_PFUN_STUDENTST]	m_pr_studentst,
	[SF_PFUN_STUDENTSZ]	m_pr_studentsz,
	[SF_PFUN_BETAPRIME]	m_pr_betaprime,
	[SF_PFUN_ERLANG]	m_pr_erlang,
//	[SF_PFUN_LOGNORMAL]	m_pr_lognormal,
	[SF_PFUN_GIBRAT]	m_pr_gibrat,
	[SF_PFUN_BATHTUB]	m_pr_bathtub,
};

RandgenPfunfptr SF_PFUN_FPTRS[] = 
{
	[SF_PFUN_UNIFORM]	m_pfun_uniform,
	[SF_PFUN_EXP]		m_pfun_exp,
	[SF_PFUN_GAUSS]		m_pfun_gauss,
//	[SF_PFUN_BPARETO]	m_pfun_bpareto,
	[SF_PFUN_WEIBULL]	m_pfun_weibull,
	[SF_PFUN_XI2]		m_pfun_xi2,
	[SF_PFUN_CAUCHY]	m_pfun_cauchy,
	[SF_PFUN_XI]		m_pfun_xi,
	[SF_PFUN_BETA]		m_pfun_beta,
	[SF_PFUN_EXTREMEVAL]	m_pfun_extremeval,
	[SF_PFUN_F]		m_pfun_f,
	[SF_PFUN_FERMIDIRAC]	m_pfun_fermidirac,
	[SF_PFUN_FISHERZ]	m_pfun_fisherz,
	[SF_PFUN_GUMBEL]	m_pfun_gumbel,
	[SF_PFUN_GAMMA]		m_pfun_gamma,
	[SF_PFUN_HALFNORMAL]	m_pfun_halfnormal,
	[SF_PFUN_LAPLACE]	m_pfun_laplace,
	[SF_PFUN_LOGSERIES]	m_pfun_logseries,
	[SF_PFUN_LOGISTIC]	m_pfun_logistic,
	[SF_PFUN_MAXWELL]	m_pfun_maxwell,
	[SF_PFUN_NEGBINOMIAL]	m_pfun_negbinomial,
	[SF_PFUN_PEARSONTYPE3]	m_pfun_pearsontype3,
	[SF_PFUN_POISSON]	m_pfun_poisson,
	[SF_PFUN_RAYLEIGH]	m_pfun_rayleigh,
	[SF_PFUN_STUDENTST]	m_pfun_studentst,
	[SF_PFUN_STUDENTSZ]	m_pfun_studentsz,
	[SF_PFUN_BETAPRIME]	m_pfun_betaprime,
	[SF_PFUN_ERLANG]	m_pfun_erlang,
//	[SF_PFUN_LOGNORMAL]	m_pfun_lognormal,
	[SF_PFUN_GIBRAT]	m_pfun_gibrat,
	[SF_PFUN_BATHTUB]	m_pfun_bathtub,
};

static tuck double
e(double x)
{
	return pow(M_E, x);
}

static tuck double
factorial(double n)
{
	return lgamma(n+1);
}

static tuck double
binomial(double n, double k)
{
	return factorial(n)/(factorial(n-k)*factorial(k));
}

static tuck double
beta(double p, double q)
{
	return lgamma(p)*lgamma(q)/lgamma(p+q);
}

static tuck double
u(Engine *E, double min, double max)
{
	/*							*/
	/*	Generate a uniformly distributed double		*/
	/*	over the double basis.	Note that values	*/
	/*	that are exactly representable in floating-	*/
	/*	point numbers are not evenly distributed on	*/
	/*	the real line (see, e.g., Ercegovac pg 406).	*/
	/*	We thus can't use a bit-level mapping b/n	*/
	/*	uvlong and double to get a true uniform dist.	*/
	/*							*/
	/*	What we do is, we treat the interval max-min	*/
	/*	as having |uvlong| = ~0ULL points, and use	*/
	/*	result of mrandom() which is in [0 ~0ULL],	*/
	/*	to index into those points which are 		*/
	/*	[min max].					*/
	/*							*/

	return min + ((double)mrandom(E)) * ((max - min)/((double)~0ULL));
}

uvlong
mrandominit(Engine *E, uvlong seed)
{
	uvlong	ux, lx;

	if (seed == -1)
	{
		seed = mwallclockusecs();
	}

	E->randgen_mt = (uvlong *)mcalloc(E, RANDGEN_NN, sizeof(uvlong), SF_FILE_MACRO);
	if (E->randgen_mt == NULL)
	{
		mexit(E, "mcalloc failed", -1);
	}

	for (E->randgen_mti = 0; E->randgen_mti < RANDGEN_NN; E->randgen_mti++)
	{
		ux	= seed & 0xFFFFFFFF00000000ULL;
		seed	= 2862933555777941757ULL * seed + 1ULL;
		lx	= seed >> 32;
		seed	= 2862933555777941757ULL * seed + 1ULL;
		E->randgen_mt[E->randgen_mti] = ux | lx;
	}

	return seed;
}

static int
disttabname2id(Engine *E, char *distname)
{
	int	i;


	for (i = 0; i < E->nrandtabs; i++)
	{
		if (!strcmp(E->randtabs[i].name, distname))
		{
			return i;
		}
	}

	/*	If no match, allocate a name entry string	*/
/*
	if (	(i ==  E->nrandtabs) &&
		(E->nrandtabs < MAX_RANDTABLEENTRIES))
	{
		E->randtabs[E->nrandtabs].name = mcalloc(E, 1, strlen(distname)+1, SF_FILE_MACRO);
		if (E->randtabs[E->nrandtabs].name == NULL)
		{
			mexit(E, "mcalloc failed", -1);
		}

		strcpy(E->randtabs[E->nrandtabs++].name, distname);
		return i;
	}
*/
	return -1;
}

static int
pfunname2id(char *pfunname)
{
	int	i;

	for (i = 0; i < SF_PFUN_MAXPFUN; i++)
	{
		if (!strcmp(pfunname, SF_PFUN_NAMES[i]))
		{
			return i;
		}
	}

	return -1;
}

static tuck int
simrvarname2id(char *rvarname)
{
	int	i;

	for (i = 0; i < SF_RVAR_MAXNAME; i++)
	{
		if (!strcmp(rvarname, SF_RVAR_NAMES[i].name))
		{
			return i;
		}
	}

	return -1;
}


static void
allocrandtable(Engine *E, RandTable *rt, double (*pr)(Engine *, double, double, double, double, double),
	double basis_start, double basis_end, double granularity,
	double p1, double p2, double p3, double p4)
{
	double	minprob, entry;
	int	i, n, nprobs, count, size;

	/*									*/
	/*	Ensure that number with smallest prob. has an occurrence	*/
	/*	count of at least 1.  Since we have no a priori knowledge	*/
	/*	of the shape of the distribution, we have to scan through it	*/
	/*									*/
	minprob = 1.0;
	entry = basis_start;
	while (entry < basis_end)
	{
		minprob = min(minprob, pr(E, entry, p1, p2, p3, p4));
		entry += granularity;
	}

	nprobs	= (int)ceil(1/minprob);

	size = 0;
	entry = basis_start;
	while (entry < basis_end)
	{
		size += nprobs * pr(E, entry, p1, p2, p3, p4);
		entry += granularity;
	}

	//	TODO: restrict to a maximum size, abort if too big

	rt->table = (double *) mrealloc(E, rt->table, size*sizeof(double), SF_FILE_MACRO);
	if (rt->table == NULL)
	{
		mexit(E, "mrealloc failed", -1);
	}
	rt->size = size;

	count	= 0;
	entry	= basis_start;
	while (entry < basis_end)
	{
		/*	C truncation is OK here		*/
		n = nprobs * pr(E, entry, p1, p2, p3, p4);

		for (i = 0; i < n; i++)
		{
			rt->table[count++] = entry;
		}

		entry += granularity;
	}

	return;
}

void
m_initrandtable(Engine *E, char *distname,
	char *pfunname, double basis_start, double basis_end, double granularity,
	double p1, double p2, double p3, double p4)
{
	int	pfun_id, dist_id;


	/*									*/
	/*	Randtables are only generated at the request of the user, 	*/
	/*	to create a randtable-based distribution, specifying the 	*/
	/*	PMF/PDF and support						*/
	/*									*/
	/*	Register a new randtable distribution (PMF/PDF and basis)	*/
	/*	for use in simulator.						*/
	/*									*/
	/*	For discrete distributions, we can calculate needed basis	*/
	/*	size automatically (need one member per basis point).  For 	*/
	/*	continuous distributions however, the granularity needs to	*/
	/*	be explicitly specified.					*/
	/*									*/


	if (granularity <= 0)
	{
		merror(E, "Invalid granularity for creation of distribution");
		return;
	}

	if (basis_start >= basis_end)
	{
		merror(E, "Invalid basis in creation of distribution");
		return;
	}

	/*	Do a sanity check on minimum size of basis (end-start)/gran	*/
	if ((basis_end - basis_start)/granularity > SF_HUGE_RANDTABLE)
	{
		merror(E, "Warning: size of randtable will be very large,"
			"particularly for distributions with large Skewness");
	}

	pfun_id = pfunname2id(pfunname);
	if (pfun_id < 0)
	{
		merror(E, "Invalid PMF/PDF name supplied.  See online help for those implemented.");
		return;
	}

	dist_id = disttabname2id(E, distname);
	if (dist_id < 0)
	{
		merror(E, "Maximum number of registrable dists reached. Sorry.");
		return;
	}

	allocrandtable(E, &E->randtabs[dist_id], SF_PR_FPTRS[pfun_id], 
		basis_start, basis_end, granularity, p1, p2, p3, p4);


	return;
}

void
m_defndist(Engine *E, char *distname, DoubleList *support, DoubleList *prob)
{
	DoubleListItem	*p, *s;
	double		minprob = DBL_MAX;
	int		dist_id, nprobs, size, count, n, i;
	RandTable	*rt;


	/*									*/
	/*	Define a custom distribution by specifying its support		*/
	/*	and corresponding probabilities.  Leads to the generation	*/
	/*	of a randtable.							*/
	/*									*/

	//	TODO: we currently only permit one instance/match for a dist + parameters
	//	permit defn of multiple instances of a given dist with different params
	//	in the future...  Also, need to check the supplied probs to see
	//	whether it is a valid probability measure.

	/*	Sanity checks	*/
	if ((support->len != prob->len) || (prob->len == 0))
	{
		merror(E, "Badly specified probability measure");
		return;
	}

	dist_id = disttabname2id(E, distname);
	if (dist_id < 0)
	{
		merror(E, "Maximum number of registrable dists reached. Sorry.");
		return;
	}
	
	rt = &E->randtabs[dist_id];

	p = prob->hd;
	while (p != NULL)
	{
		minprob = min(minprob, p->value);
		p = p->next;
	}
	nprobs	= (int)ceil(1/minprob);

	p = prob->hd;
	size = 0;
	while (p != NULL)
	{
		/*	C truncation is OK here		*/
		size += p->value * nprobs;
		p = p->next;
	}
	
	rt->size = size;
	rt->table = (double *) mcalloc(E, size, sizeof(double), SF_FILE_MACRO);
	if (rt->table == NULL)
	{
		mexit(E, "mcalloc failed", -1);
	}

	count	= 0;
	s = support->hd;
	while (s != NULL)
	{
		n = nprobs * p->value;

		for (i = 0; i < n; i++)
		{
			rt->table[count++] = s->value;
		}

		s = s->next;
		p = p->next;
	}

	return;
}

double
m_randgen(Engine *E, char *distname, double min, double max, double p1, double p2, double p3, double p4)
{
	int	id, idx;


	/*	First, check if its a builtin dist	*/
	id = pfunname2id(distname);
	if (id >= 0)
	{
		return SF_PFUN_FPTRS[id](E, min, max, p1, p2, p3, p4);
	}

	/*	Check if its a randtable dist		*/
	id = disttabname2id(E, distname);
	if (id < 0)
	{
		merror(E, "No such distribution");
		return 0;
	}

	idx = mrandom(E) % E->randtabs[id].size;
	return E->randtabs[id].table[idx];
}

void
m_randprint(Engine *E, char *distname, double min, double max, double p1, double p2, double p3, double p4)
{
	/*	Print a random value from the said distribution to console	*/
	mprint(E, NULL, siminfo, "%f\n", m_randgen(E, distname, min, max, p1, p2, p3, p4));

	return;
}

uvlong
mrandom(Engine *E)
{
	int		i;
	uvlong		x;
	static uvlong	mag01[2] = {0ULL, RANDGEN_MATRIX_A};


	/*	M-N: generate NN words at one time	*/
	if (E->randgen_mti >= RANDGEN_NN) 
	{
		/*							*/
		/* 	M-N: 	if mrandominit() has not been called, 	*/
		/*		a default initial seed is used		*/
		/*							*/

		if (E->randgen_mti == RANDGEN_NN+1)
		{
			mrandominit(E, 1ULL);
		}

		for (i = 0; i < RANDGEN_NN - RANDGEN_M2; i++) 
		{
			x = 	(E->randgen_mt[i]&RANDGEN_UMASK) |
				(E->randgen_mt[i+1]&RANDGEN_LMASK);
			E->randgen_mt[i] = 	(x >> 1) ^ mag01[(int)(x&1ULL)];
			E->randgen_mt[i] ^= 	E->randgen_mt[i + RANDGEN_M0] ^
						E->randgen_mt[i + RANDGEN_M1] ^
						E->randgen_mt[i + RANDGEN_M2];
		}
		for (; i < RANDGEN_NN-RANDGEN_M1; i++)
		{
			x =	(E->randgen_mt[i]&RANDGEN_UMASK) |
				(E->randgen_mt[i+1]&RANDGEN_LMASK);
			E->randgen_mt[i] = 	(x>>1) ^ mag01[(int)(x&1ULL)];
			E->randgen_mt[i] ^=	E->randgen_mt[i + RANDGEN_M0] ^
						E->randgen_mt[i + RANDGEN_M1] ^
						E->randgen_mt[i + RANDGEN_M2 - RANDGEN_NN];
		}
		for (; i < RANDGEN_NN - RANDGEN_M0; i++)
		{
			x =	(E->randgen_mt[i]&RANDGEN_UMASK) |
				(E->randgen_mt[i+1]&RANDGEN_LMASK);
			E->randgen_mt[i] = 	(x>>1) ^ mag01[(int)(x&1ULL)];
			E->randgen_mt[i] ^= 	E->randgen_mt[i + RANDGEN_M0] ^
						E->randgen_mt[i + RANDGEN_M1 - RANDGEN_NN] ^
						E->randgen_mt[i + RANDGEN_M2 - RANDGEN_NN];
		}
		for (; i < RANDGEN_NN - 1; i++)
		{
			x =	(E->randgen_mt[i]&RANDGEN_UMASK) |
				(E->randgen_mt[i+1]&RANDGEN_LMASK);
			E->randgen_mt[i] = 	(x>>1) ^ mag01[(int)(x&1ULL)];
			E->randgen_mt[i] ^= 	E->randgen_mt[i + RANDGEN_M0 - RANDGEN_NN] ^
						E->randgen_mt[i + RANDGEN_M1 - RANDGEN_NN] ^
						E->randgen_mt[i + RANDGEN_M2 - RANDGEN_NN];
		}
		x = 	(E->randgen_mt[RANDGEN_NN-1]&RANDGEN_UMASK)|
			(E->randgen_mt[0]&RANDGEN_LMASK);
		E->randgen_mt[RANDGEN_NN-1] = 	(x>>1) ^ mag01[(int)(x&1ULL)];
		E->randgen_mt[RANDGEN_NN-1] ^= 	E->randgen_mt[RANDGEN_M0 - 1] ^
						E->randgen_mt[RANDGEN_M1 - 1] ^
						E->randgen_mt[RANDGEN_M2 - 1];
		E->randgen_mti = 0;
	}

	x = E->randgen_mt[E->randgen_mti++];
	x ^= (x >> RANDGEN_UU);
	x ^= (x << RANDGEN_SS) & RANDGEN_MASK_B;
	x ^= (x << RANDGEN_TT) & RANDGEN_MASK_C;
	x ^= (x >> RANDGEN_LL);

	return x;
}

/*										*/
/*	For the m_pfun_*() functions, we include the 'min' and 'max' params	*/
/*	as a convenience, since we will often want to restrict the range of	*/
/*	value we get from the generator (in which case the integral over	*/
/*	probabilities of the restricted set will be < 1.   For m_pr_*() on	*/
/*	the other hand, we do use 'min'/'max', since we want to return the	*/
/*	probability of 'basis_val' in the distribution.  If you really want	*/
/*	the distribution to have zero density outside some range (if even	*/
/*	possible), then you have to setup the distirbution accordingly.		*/
/*										*/
/*	Discrete, [-Inf, Inf]	*/
double
m_pr_uniform(Engine *E, double basis_val, double p1, double p2, double p3, double p4)
{
	/*	min = p1, max = p2;	*/

	USED(p3);
	USED(p4);

	if (p2 <= p1)
	{
		merror(E, "Sanity check failed in randgen.c:m_pr_uniform()");
		return 0;
	}

	if (basis_val >= p1 && basis_val <= p2)
	{
		return 1.0/(p2 - p1);
	}

	return 0;
}
double
m_pfun_uniform(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	USED(p1);
	USED(p2);
	USED(p3);
	USED(p4);

	if (max <= min)
	{
		merror(E, "Sanity check failed in randgen.c:m_pfun_uniform()");
		return 0;
	}

	return u(E, min, max);
}




/*	Continuous, (0, Inf]	*/
double
m_pr_exp(Engine *E, double basis_val, double p1, double p2, double p3, double p4)
{
	/*	lambda = p1	*/
	USED(p2);
	USED(p3);
	USED(p4);

	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfununiform()");
	//	return 0;
	//}

	return p1 * e(-p1 * basis_val);
}
double
m_pfun_exp(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	/*	lambda = p1	*/
	USED(p2);
	USED(p3);
	USED(p4);

	/*								*/
	/*		    The Inverse Transform Method		*/
	/*								*/
	/*	(1)	Pr(X <= x) = F(X)				*/
	/*								*/
	/*	(2)	Given a rvar U, uniform on [0, 1], we want	*/
	/*		to generate an rvar X, on [0, Inf]		*/
	/*								*/
	/*	(3)	For uniform, Pr(U <= u) = u			*/
	/*								*/
	/*	(4)	P(U <= u) = Pr(X <= x) = F(x), and we want	*/
	/*		to find this F(x) that makes the above true.	*/
	/*								*/
	/*	(5)	F(x) = P(U <= u) = u	thus x = Finv(u)	*/
	/*								*/
	/*	(6)	If X ~ Exponential r.v., F(x) = 1 - Exp[-Lx]	*/
	/*		x = Finv(u) => F(x) = u => 1-Exp[-Lx] = u	*/
	/*			So, given u on [0,1] 			*/
	/*			x = -1/L ln(1-u) ~ Exponentially dist	*/
	/*								*/
	/*	1-DBL_EPSILON to elide the 0 case, since (0, not [0	*/
	/*								*/
	return -(1.0/p1)*log(1.0 - u(E, 0.0, 1.0 - DBL_EPSILON));
}




/*										*/
/*		In many cases, we can't use the inverse tranform method		*/
/*		--- e.g., there is no closed form expression for F(X).		*/
/*										*/
/*			The "Accept/Reject" method				*/
/*										*/
/*		(1)	You need to have a pdf, say fy(t), that is		*/
/*			"similar" to the pdf fx(t) you want, and		*/
/*			that you know how to generate _easily_.			*/
/*										*/
/*		(2)	You need to know, for fy(t) and fx(t), the		*/
/*			value of 'c' s.t. fx(t)/fy(t) <= c			*/
/*			for all t s.t. fx(t) > 0				*/
/*										*/
/*		(*)	Given that you want fx(t) for some t:			*/
/*										*/
/*		(3)	Generate an instance of fy().  You have this		*/	
/*			instance in your hand, call it instance t.		*/
/*			a posteriori, find out what the probability		*/
/*			of getting 't' were, both for fy(t) and fx(t)		*/
/*										*/
/*		(4)	With probability fx(t)/(c*fy(t)), accept t		*/
/*			else go to step (3)					*/
/*										*/

double
accept_reject(Engine *E, double min, double max,
	double (*pr)(Engine *, double, double, double, double, double),
	double p1, double p2, double p3, double p4)
{
	double		j, pj, qj, test;
	const double	c = 1.0;

	/*					*/
	/*	Let fy(t) = 1, c = 1		*/
	/*	fx(t)/fy(t) <= 1  for all t	*/
	/*					*/
	do
	{
		j = u(E, min, max);
		pj = pr(E, j, p1, p2, p3, p4);
		qj = 1;
		test = pj/(c*qj);
	} while (u(E, 0.0, 1.0/test) != 1.0);

	return j;	
}





/*	Continuous, [-Inf, Inf]		*/
double
m_pr_gauss(Engine *E, double basis_val, double p1, double p2, double p3, double p4)
{
	/*								*/
	/*	Gaussian Distribution					*/
	/*								*/
	/*	  P(X=x) = 1/s*sqrt(2*pi) exp(-(x-u)^2/(2 s^2))		*/
	/*								*/
	/*	p1 and p2 are used as u and s resp.			*/
	/*								*/

	USED(p3);
	USED(p4);

	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_gauss()");
	//	return 0;
	//}

	return (1.0/(p2*sqrt(2.0*M_PI)))*pow(
		M_E, (-(basis_val*basis_val - 2.0*basis_val*p1 + p1*p1))/(2.0*p2*p2));
}
double
m_pfun_gauss(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_gauss()");
	//	return 0;
	//}

	return accept_reject(E, DBL_MIN, DBL_MAX, m_pr_gauss, p1, p2, p3, p4);	
}



/*	Continuous, [k, Inf]	*/
double
m_pr_pareto(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_pareto()");
	//	return 0;
	//}

	/*								*/
	/*	Pareto Distribution					*/
	/*								*/
	/*		      P(X=x) = (a*k^a)/(x^(a+1))		*/
	/*								*/
	/*	p1 and p2 are used as a and k resp.			*/
	/*								*/

	return (p1*pow(p2,p1))/pow(basis_value, p1+1);
}
double
m_pfun_pareto(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_pareto()");
	//	return 0;
	//}

	return accept_reject(E, DBL_MIN, DBL_MAX, m_pr_pareto, p1, p2, p3, p4);	
}





/*	Continuous, [0, Inf]	*/
double
m_pr_weibull(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_weibull()");
	//	return 0;
	//}

	/*								*/
	/*	Weibull Distribution					*/
	/*								*/
	/*		P(X=x) = a*b^(-a)x^(a-1)e^(-(x/b)^a)		*/
	/*								*/
	/*	p1 and p2 are used as a and b resp.			*/
	/*								*/

	USED(p3);
	USED(p4);

	return p1*pow(p2, -1.0)*pow(basis_value, p1-1.0)*e(-pow(basis_value/p2, p1));
}
double
m_pfun_weibull(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_weibull()");
	//	return 0;
	//}

	return accept_reject(E, 0, DBL_MAX, m_pr_weibull, p1, p2, p3, p4);	
}






/*	Continuous, [0, Inf]	*/
double
m_pr_xi2(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_xi2()");
	//	return 0;
	//}

	USED(p2);
	USED(p3);
	USED(p4);

	/*								*/
	/*	Chi squared distribution.				*/
	/*								*/
	/*	P(X=x) = (x^(r/2 - 1) e^(-x/2))/lgamma(r/2)2^(r/2)	*/
	/*								*/
	/*	p1 is used as r						*/
	/*								*/

	return pow(basis_value, (p1/2.0)-1.0)*e(-basis_value/2.0)/(lgamma(p1/2.0)*exp2(p1/2.0));
}
double
m_pfun_xi2(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_xi2()");
	//	return 0;
	//}

	return accept_reject(E, 0, DBL_MAX, m_pr_xi2, p1, p2, p3, p4);	
}




/*	Coninuous, [-Inf, Inf]		*/
double
m_pr_cauchy(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_cauchy()");
	//	return 0;
	//}

	/*								*/
	/*	Cauchy Distribution					*/
	/*								*/
	/*		P(X=x) = (1.0/pi)(b/((x-m)^2.0 + b^2.0))	*/
	/*								*/
	/*	p1 and p2 are used as b and m resp.			*/
	/*								*/

	USED(p3);
	USED(p4);

	return (1.0/M_PI)*(p1/(basis_value*basis_value - 2.0*basis_value*p2 + p2*p2 + p1*p1));
}
double
m_pfun_cauchy(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_cauchy()");
	//	return 0;
	//}

	return	accept_reject(E, DBL_MIN, DBL_MAX, m_pr_cauchy, p1, p2, p3, p4);
}





/*	Continuous, [0, Inf]	*/
double
m_pr_xi(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_xi()");
	//	return 0;
	//}

	USED(p2);
	USED(p3);
	USED(p4);

	/*								*/
	/*	Chi Distribution.					*/
	/*								*/
	/*	P(X=x) = (2^(1-n/2)*x^(n-1)*e^(-x^2/2))/(lgamma(n/2))	*/
	/*								*/
	/*	p1 is used as n						*/
	/*								*/

	return (pow(2, 1-(p1/2))*pow(basis_value, p1-1)*e(-(basis_value*basis_value)/2))/lgamma(p1/2);
}
double
m_pfun_xi(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_xi()");
	//	return 0;
	//}

	return accept_reject(E, 0, DBL_MAX, m_pr_xi, p1, p2, p3, p4);
}






/*	Continuous, [-Inf, Inf]		*/
double
m_pr_beta(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	USED(p3);
	USED(p4);

	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_beta()");
	//	return 0;
	//}

	/*								*/
	/*	Beta distribution.					*/
	/*								*/
	/*	P(X=x) = ((1-x)^(b-1) * x^(a-1))/B(a, b)		*/
	/*								*/
	/*	p1 and p2 are used as a and b resp.			*/
	/*								*/

	return pow(1.0-basis_value, p2-1.0)*pow(basis_value, p1-1.0)/beta(p1,p2);
}
double
m_pfun_beta(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_beta()");
	//	return 0;
	//}

	return accept_reject(E, DBL_MIN, DBL_MAX, m_pr_beta, p1, p2, p3, p4);
}





/*	Continuous, [-Inf, Inf]		*/
double
m_pr_extremeval(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_extremeval()");
	//	return 0;
	//}

	USED(p3);
	USED(p4);

	/*								*/
	/*	Extreme value distribution.				*/
	/*								*/
	/*	P(X=x) = e^((a-x)/b - e^((a-x)/b))/b			*/
	/*								*/
	/*	p1 and p2 are used as a and b resp.			*/
	/*								*/

	return e((p1-basis_value)/p2 - e((p1-basis_value)/p2))/p2;
}
double
m_pfun_extremeval(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_extremeval()");
	//	return 0;
	//}

	return accept_reject(E, DBL_MIN, DBL_MAX, m_pr_extremeval, p1, p2, p3, p4);
}





/*	Continuous, [0, Inf]	*/
double
m_pr_f(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_f()");
	//	return 0;
	//}

	USED(p3);
	USED(p4);

	/*								*/
	/*	F-distribution.						*/
	/*								*/
	/*	P(X=x) =						*/
	/* (m^(m/2)n^(n/2)x^(n/2 - 1))/((m + n*x)^(n+m)/2 * B(n/2,m/2))	*/
	/*								*/
	/*	p1 and p2 are used as m and n resp.			*/
	/*								*/

	return pow(p1, p1/2)*pow(p2, p2/2)*pow(basis_value, p2/2 -1)/
			(pow(p1+p2*basis_value, (p2+p1)/2)*beta(p2/2, p1/2));
}
double
m_pfun_f(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_f()");
	//	return 0;
	//}

	return accept_reject(E, 0, DBL_MAX, m_pr_f, p1, p2, p3, p4);
}






/*	Continuous, [0, Inf] ??	*/
double
m_pr_fermidirac(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_fermidirac()");
	//	return 0;
	//}

	USED(p3);
	USED(p4);

	/*								*/
	/*		Fermi-Dirac distribution.			*/
	/*								*/
	/*		P(k) = (k^s)/(e^(k - u) + 1)			*/
	/*								*/
	/*		p1 and p2 are used as s and u resp		*/
	/*								*/

	return pow(basis_value, p1)/(e(basis_value-p2) + 1);
}
double
m_pfun_fermidirac(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_fermidirac()");
	//	return 0;
	//}

	return accept_reject(E, 0, DBL_MAX, m_pr_fermidirac, p1, p2, p3, p4);
}






/*	Continuous, [-Inf, Inf]??	*/
double
m_pr_fisherz(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_fisherz()");
	//	return 0;
	//}

	USED(p3);
	USED(p4);

	/*								*/
	/*	Fisher's z-distribution.				*/
	/*								*/
	/*	P(Z=z) = 						*/
	/*		(2*n1^(n1/2) * n2^(n2/2))/(B(n1/2, n2/2)) *	*/
	/*		(e^(n1 * z))/(n1*e^(2z) + n2)^(n1+n2)/2		*/
	/*								*/
	/*	p1 and p2 used as n1 and n2 resp.			*/
	/*								*/

	return 2 * pow(p1, p1/2)*pow(p2, p2/2)*e(p1*basis_value)/
			(beta(p1/2, p2/2)*pow(p1*e(2*basis_value)+p2, (p1+p2)/2));
}
double
m_pfun_fisherz(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_fisherz()");
	//	return 0;
	//}

	return accept_reject(E, DBL_MIN, DBL_MAX, m_pr_fisherz, p1, p2, p3, p4);
}






/*	Continuous, [-Inf, Inf]		*/
double
m_pr_gumbel(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_gumbel()");
	//	return 0;
	//}

	USED(p3);
	USED(p4);

	/*								*/
	/*	Gumbel distribution.					*/
	/*								*/
	/*	P(X=x) = 1/b exp((x-a)/b - exp((x-a)/b))		*/
	/*								*/
	/*	p1 and p2 used as a and b resp.				*/
	/*								*/

	return (1/p2)*e(((basis_value-p1)/p2) - e((basis_value-p1)/p2));
}
double
m_pfun_gumbel(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_gumbel()");
	//	return 0;
	//}

	return accept_reject(E, DBL_MIN, DBL_MAX, m_pr_gumbel, p1, p2, p3, p4);
}






/*	Continuous, [0, Inf]	*/
double
m_pr_gamma(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_gamma()");
	//	return 0;
	//}

	USED(p3);
	USED(p4);

	/*								*/
	/*	Gamma distribution.					*/
	/*								*/
	/*	P(X=x) = (x^(a - 1)e^(-x/t))/(lgamma(a)*t^a)		*/
	/*								*/
	/*	p1 and p2 are used as a and t resp.			*/
	/*								*/

	return pow(basis_value, p1-1)*e(-basis_value/p2)/(lgamma(p1)*pow(p2, p1));
}
double
m_pfun_gamma(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_gamma()");
	//	return 0;
	//}

	return accept_reject(E, 0, DBL_MAX, m_pr_gamma, p1, p2, p3, p4);
}






/*	Continuous, [0, Inf]	*/
double
m_pr_halfnormal(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_halfnormal()");
	//	return 0;
	//}

	USED(p2);
	USED(p3);
	USED(p4);

	/*								*/
	/*	Half Normal distribution.				*/
	/*								*/
	/*	P(X=x) = 2t/pi e^(-x^2 t^2/pi)				*/
	/*								*/
	/*	p1 is used as t						*/
	/*								*/

	return (2*p1/M_PI)*e(-basis_value*basis_value*p1*p1/M_PI);
}
double
m_pfun_halfnormal(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_halfnormal()");
	//	return 0;
	//}

	return accept_reject(E, 0, DBL_MAX, m_pr_halfnormal, p1, p2, p3, p4);
}






/*	Continuous, [-Inf, Inf]		*/
double
m_pr_laplace(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_laplace()");
	//	return 0;
	//}

	USED(p3);
	USED(p4);

	/*								*/
	/*	Laplace distribution.					*/
	/*								*/
	/*	P(X=x) = 1/(2b) e^(-|x-u|/b)				*/
	/*								*/
	/*	p1 and p2 are used as u and b resp.			*/
	/*								*/

	return (1/(2*p2))*e(-fabs(basis_value-p1)/p2);
}
double
m_pfun_laplace(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_laplace()");
	//	return 0;
	//}

	return accept_reject(E, DBL_MIN, DBL_MAX, m_pr_laplace, p1, p2, p3, p4);
}






/*	Discrete, (0, Inf]	*/
double
m_pr_logseries(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_logseries()");
	//	return 0;
	//}

	USED(p2);
	USED(p3);
	USED(p4);

	/*								*/
	/*	Log series distribution.				*/
	/*								*/
	/*	P(N=n) = (t^n)/(n*ln(1-t))				*/
	/*								*/
	/*	p1 used as t						*/
	/*								*/

	return pow(p1, basis_value)/(basis_value*log(1-p1));
}
double
m_pfun_logseries(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_logseries()");
	//	return 0;
	//}

	return accept_reject(E, DBL_EPSILON, DBL_MAX, m_pr_logseries, p1, p2, p3, p4);
}






/*	Continuous, [-Inf, Inf]		*/
double
m_pr_logistic(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_logistic()");
	//	return 0;
	//}

	USED(p3);
	USED(p4);

	/*								*/
	/*	Logistic distribution.					*/
	/*								*/
	/*	P(X=x) = e^(-(x-m)/b)/b(1+e^(-x-m)/b)^2			*/
	/*								*/
	/*	p1 and p2 are used as b and m resp.			*/
	/*								*/

	return e(-(basis_value-p2)/p1)/(p1*pow(1+e(-(basis_value-p2)/p1), 2));
}
double
m_pfun_logistic(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_logistic()");
	//	return 0;
	//}

	return accept_reject(E, DBL_MIN, DBL_MAX, m_pr_laplace, p1, p2, p3, p4);
}







/*	Continuous, [0, Inf]	*/
double
m_pr_maxwell(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_maxwell()");
	//	return 0;
	//}

	USED(p2);
	USED(p3);
	USED(p4);

	/*								*/
	/*	Maxwell-Boltzmann distribution.				*/
	/*								*/
	/*	P(X=x) = sqrt(2/pi) (x^2 e^(-x^2 / (2a^2)))/a^3		*/
	/*								*/
	/*	p1 used as a						*/
	/*								*/

	return sqrt(2/M_PI)*
		(basis_value*basis_value*e(-(basis_value*basis_value)/(2*p1*p1)))/(p1*p1*p1);
}
double
m_pfun_maxwell(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_maxwell()");
	//	return 0;
	//}

	return accept_reject(E, 0, DBL_MAX, m_pr_maxwell, p1, p2, p3, p4);
}







/*	Discrete, [0, Inf]	*/
double
m_pr_negbinomial(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_negbinomial()");
	//	return 0;
	//}

	USED(p3);
	USED(p4);

	/*								*/
	/*	Negative Binomial distribution.				*/
	/*								*/
	/*	P(X=x) = binomial(x+r-1, r-1)p^r (1-p)^x		*/
	/*								*/
	/*	p1 and p2 are used as r and p resp.			*/
	/*								*/

	return binomial(basis_value+p1-1, p1-1)*pow(p2, p1)*pow(1-p2, basis_value);
}
double
m_pfun_negbinomial(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_negbinomial()");
	//	return 0;
	//}

	return accept_reject(E, 0, DBL_MAX, m_pr_negbinomial, p1, p2, p3, p4);
}







/*	Continuous, [-Inf, Inf] ??	*/
double
m_pr_pearsontype3(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_pearsontype3()");
	//	return 0;
	//}

	USED(p4);

	/*								*/
	/*	Pearson Type III distribution.				*/
	/*								*/
	/*	P(X=x) = (1/b*lgamma(p))((x-a)/b)^(p-1)	* exp(-(x-a)/b)	*/
	/*								*/
	/*	p1, p2 and p3 are used as a, b and p			*/
	/*								*/

	return (1/p2*lgamma(p3))*pow((basis_value-p1)/p2, p3-1)*e(-(basis_value-p1)/p2);
}
double
m_pfun_pearsontype3(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_pearsontype3()");
	//	return 0;
	//}

	return accept_reject(E, DBL_MIN, DBL_MAX, m_pr_pearsontype3, p1, p2, p3, p4);
}







/*	Discrete, [0, Inf]	*/
double
m_pr_poisson(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_poisson()");
	//	return 0;
	//}

	USED(p2);
	USED(p3);
	USED(p4);

	/*								*/
	/*	Poisson distribution.					*/
	/*								*/
	/*	P(n) = (e^(-l) l^n)/n!					*/
	/*								*/
	/*	p1 used as l						*/
	/*								*/

	return e(-p1)*pow(p1, basis_value)/factorial(basis_value);
}
double
m_pfun_poisson(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_poisson()");
	//	return 0;
	//}

	return accept_reject(E, 0, DBL_MAX, m_pr_poisson, p1, p2, p3, p4);
}







/*	Continuous, [0, Inf]	*/
double
m_pr_rayleigh(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_rayleigh()");
	//	return 0;
	//}

	USED(p3);
	USED(p4);

	/*								*/
	/*	Rayleigh distribution.					*/
	/*								*/
	/*	P(R=r) = r e^((-r^2)/(2 s^2)) / s^2			*/
	/*								*/
	/*	p1 used as s.						*/
	/*								*/

	return basis_value*e(-basis_value*basis_value/(2*p1*p1))/(p1*p1);
}
double
m_pfun_rayleigh(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_rayleigh()");
	//	return 0;
	//}

	return accept_reject(E, 0, DBL_MAX, m_pr_rayleigh, p1, p2, p3, p4);
}







/*	Continuous, [-Inf, Inf]		*/
double
m_pr_studentst(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_studentst()");
	//	return 0;
	//}

	USED(p2);
	USED(p3);
	USED(p4);

	/*								*/
	/*	"Student"'s t-distribution.				*/
	/*								*/
	/*	P(T=t) = ((r/(r+t^2))^(1+r)/2)/(sqrt(r)B(r/2, 1/2))	*/
	/*								*/
	/*	p1 used as r						*/
	/*								*/

	return pow(p1/(p1+basis_value*basis_value), (1+p1)/2.0)/(sqrt(p1)*beta(p1/2.0, 0.5));
}
double
m_pfun_studentst(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_studentst()");
	//	return 0;
	//}

	return accept_reject(E, DBL_MIN, DBL_MAX, m_pr_studentst, p1, p2, p3, p4);
}







/*	Continuous, [-Inf, Inf]??	*/
double
m_pr_studentsz(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_studentsz()");
	//	return 0;
	//}

	USED(p2);
	USED(p3);
	USED(p4);

	/*								*/
	/*	"Student"'s z-distribution.				*/
	/*								*/
	/* P(Z=z) = (lgamma(n/2)/sqrt(pi)lgamma((n-1)/2))(1+z^2)^(-n/2)	*/
	/*								*/
	/*	p1 used as n						*/
	/*								*/

	return lgamma(p1/2)*pow(1+basis_value*basis_value, -p1/2)/(sqrt(M_PI)*lgamma((p1-1)/2));
}
double
m_pfun_studentsz(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_studentsz()");
	//	return 0;
	//}

	return accept_reject(E, DBL_MIN, DBL_MAX, m_pr_studentsz, p1, p2, p3, p4);
}







/*	Continuous, [-Inf, Inf]		*/
double
m_pr_betaprime(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_betaprime()");
	//	return 0;
	//}

	USED(p3);
	USED(p4);

	/*								*/
	/*	Beta Prime distribution.				*/
	/*								*/
	/*	P(X=x) = (x^(a-1) (1+x)^(-a-b))/(B(a,b)			*/
	/*								*/
	/*	p1 and p2 are used as a and b resp.			*/
	/*								*/

	return pow(basis_value, p1-1)*pow(1+basis_value, -p1-p2)/beta(p1,p2);
}
double
m_pfun_betaprime(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_betaprime()");
	//	return 0;
	//}

	return accept_reject(E, DBL_MIN, DBL_MAX, m_pr_betaprime, p1, p2, p3, p4);
}







/*	Continuous, [0, Inf]	*/
double
m_pr_erlang(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_erlang()");
	//	return 0;
	//}

	USED(p3);
	USED(p4);

	/*								*/
	/*	Erlang distribution.					*/
	/*								*/
	/*	P(X=x) = (l(l x)^(h-1))/(h-1)!	* e^(-l x)		*/
	/*								*/
	/*	p1 and p2 are used as l and h resp.			*/
	/*								*/

	return p1*pow(p1*basis_value, p2-1)*e(-p1*basis_value)/factorial(p2-1);
}
double
m_pfun_erlang(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_erlang()");
	//	return 0;
	//}

	return accept_reject(E, 0, DBL_MAX, m_pr_erlang, p1, p2, p3, p4);
}







/*	Continuous, (0, Inf] ??	(check)		*/
double
m_pr_lognorm(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_lognorm()");
	//	return 0;
	//}

	USED(p3);
	USED(p4);

	/*								*/
	/*	Log Normal distribution.				*/
	/*								*/
	/*	P(X=x) = 1/(S sqrt(2*pi) x) e^(-(ln(x) - M)^2/(2 S^2))	*/
	/*								*/
	/*	p1 and p2 are  used as S and M resp.			*/
	/*								*/

	return e(-pow(log(basis_value)-p2, 2)/(2*p1*p1))/(p1*sqrt(2*M_PI)*basis_value);
}
double
m_pfun_lognorm(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_lognorm()");
	//	return 0;
	//}

	return accept_reject(E, DBL_EPSILON, DBL_MAX, m_pr_lognorm, p1, p2, p3, p4);
}







/*	Continuous, (0, Inf] ??		*/
double
m_pr_gibrat(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_gibrat()");
	//	return 0;
	//}

	USED(p1);
	USED(p2);
	USED(p3);
	USED(p4);

	/*								*/
	/*	Gibrat's distribution.					*/
	/*								*/
	/*	P(X=x) = 1/(x sqrt(2*pi)) e^(-(ln(x))^2/2)		*/
	/*								*/
	return e(-pow(log(basis_value), 2)/2)/(basis_value*sqrt(2*M_PI));
}
double
m_pfun_gibrat(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_gibrat()");
	//	return 0;
	//}

	return accept_reject(E, DBL_EPSILON, DBL_MAX, m_pr_gibrat, p1, p2, p3, p4);
}







/*	Continuous, (0, Inf]?? (check)	*/
double
m_pr_bathtub(Engine *E, double basis_value, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pr_bathtub()");
	//	return 0;
	//}

	USED(p3);
	USED(p4);

	/*								*/
	/*	A pdf with a "Bathtub"-shaped hazard function. 		*/
	/*	(from Glen, Evans and Leemis: "APPL: A Probability	*/
	/*	Programming Language", The American Statistician, 	*/
	/*	May 2001, Vol. 55, No. 2, pp 156 -- 166)		*/
	/*								*/
	/*	P(t) = a(t^2 - 2tb + b^2)e^(-ta(t^2 - 3tb + 3b^2)/3)	*/
	/*		where a > 0, b > 0 and t > 0			*/
	/*								*/

	return p1*(basis_value*basis_value - 2*basis_value*p2 + p2*p2)*
		e(-basis_value*p1*(basis_value*basis_value - 3*basis_value*p2 + 3*p2*p2)/3.0);
}
double
m_pfun_bathtub(Engine *E, double min, double max, double p1, double p2, double p3, double p4)
{
	//if (0)
	//{
	//	merror(E, "Sanity check failed in randgen.c:m_pfun_bathtub()");
	//	return 0;
	//}

	return accept_reject(E, DBL_EPSILON, DBL_MAX, m_pr_bathtub, p1, p2, p3, p4);
}










void
m_registerrvar(Engine *E, State *S, char *name, int idx,
	char *valdistname, double v_p1, double v_p2, double v_p3, double v_p4,
	char *durdistname, double d_p1, double d_p2, double d_p3, double d_p4,
	char *mode)
{
	int	id;


	/*									*/
	/*	Rvars are stored in global E->rvars				*/
	/*	depending on whether they pertain to per-node or global		*/
	/*	variables.							*/
	/*									*/
	/*	For items which are a member of a collection, the idx field	*/
	/*	says which member of the collection.				*/
	/*									*/
	/*								*/
	/*	A nice stress / regression test would be to sweep	*/
	/*	various simulator parameters with distirbutions of	*/
	/*	values...						*/
	/*								*/

	/*	Register name of simulator variable that may be 
		treated as a random variable

		durdist:	determines probability the variable will be
				updated each clock tick

		valuedist:	determines the value taken on by the rvar
	
		Things which can be controlled: simulator implementation parameters
				AND
		hadware structure values.  E.g., let contents of r5 taken some
		distribution, or let pc take on some distirbution.

	*/

	Rvar		*r;



	id = simrvarname2id(name);
	if (id < 0)
	{
		merror(E, "Supplied name is not a simulator r.v. name");
		return;
	}


	r = (Rvar *) mcalloc(E, 1, sizeof(Rvar), SF_FILE_MACRO);
	if (r == NULL)
	{
		merror(E, "mcalloc failed");
		return;
	}

	if (!strncmp(mode, "add", strlen("add")))
	{
		r->mode = SF_RVAR_MODE_ADD;
	}
	else if (!strncmp(mode, "repl", strlen("repl")))
	{
		r->mode = SF_RVAR_MODE_REPL;
	}
	else
	{
		merror(E, "Supplied rvar mode is neither \"add\" nor \"repl\".");
		mfree(E, r, SF_FILE_MACRO);

		return;
	}

	r->valdistfnid  = pfunname2id(valdistname);
	if (r->valdistfnid  < 0)
	{
		r->valdisttabid = disttabname2id(E, valdistname);
		if (r->valdisttabid < 0)
		{
			mfree(E, r, SF_FILE_MACRO);
			merror(E, "Invalid value distribution name");
			return;
		}
	}

	r->durdistfnid  = pfunname2id(durdistname);
	if (r->durdistfnid  < 0)
	{
		r->durdisttabid = disttabname2id(E, durdistname);
		if (r->durdisttabid < 0)
		{
			mfree(E, r, SF_FILE_MACRO);
			merror(E, "Invalid duration distribution name");
			return;
		}
	}

	r->id = id;
	r->idx = idx;
	r->type = SF_RVAR_NAMES[id].type;

	if (S->machinetype == MACHINE_SUPERH) switch (id)
	{
		case SF_RVAR_flash_r_latency:
		{
			r->addr = &S->superH->flash_r_latency;
			break;
		}
		case SF_RVAR_flash_w_latency:
		{
			r->addr = &S->superH->flash_w_latency;
			break;
		}

		case SF_RVAR_cache_activated:
		{
			r->addr = &S->superH->cache_activated;
			break;
		}
		case SF_RVAR_ENABLE_CLK_INTR:
		{
			r->addr = &S->superH->ENABLE_CLK_INTR;
			break;
		}
		case SF_RVAR_rxok_intrenable_flag:
		{
			r->addr = &S->superH->rxok_intrenable_flag;
			break;
		}
		case SF_RVAR_txok_intrenable_flag:
		{
			r->addr = &S->superH->txok_intrenable_flag;
			break;
		}
		case SF_RVAR_addrerr_intrenable_flag:
		{
			r->addr = &S->superH->addrerr_intrenable_flag;
			break;
		}
		case SF_RVAR_frameerr_intrenable_flag:
		{
			r->addr = &S->superH->frameerr_intrenable_flag;
			break;
		}
		case SF_RVAR_collserr_intrenable_flag:
		{
			r->addr = &S->superH->collserr_intrenable_flag;
			break;
		}
		case SF_RVAR_csenseerr_intrenable_flag:
		{
			r->addr = &S->superH->csenseerr_intrenable_flag;
			break;
		}
		case SF_RVAR_rxovrrunerr_intrenable_flag:
		{
			r->addr = &S->superH->rxovrrunerr_intrenable_flag;
			break;
		}
		case SF_RVAR_txovrrunerr_intrenable_flag:
		{
			r->addr = &S->superH->txovrrunerr_intrenable_flag;
			break;
		}
		case SF_RVAR_rxundrrunerr_intrenable_flag:
		{
			r->addr = &S->superH->rxundrrunerr_intrenable_flag;
			break;
		}
		case SF_RVAR_txundrrunerr_intrenable_flag:
		{
			r->addr = &S->superH->txundrrunerr_intrenable_flag;
			break;
		}
		case SF_RVAR_csumerr_intrenable_flag:
		{
			r->addr = &S->superH->csumerr_intrenable_flag;
			break;
		}
		case SF_RVAR_TIMER_INTR_DELAY:
		{
			r->addr = &S->superH->TIMER_INTR_DELAY;
			break;
		}
		case SF_RVAR_TRA:
		{
			r->addr = &S->superH->TRA;
			break;
		}
		case SF_RVAR_EXPEVT:
		{
			r->addr = &S->superH->EXPEVT;
			break;
		}
		case SF_RVAR_INTEVT:
		{
			r->addr = &S->superH->INTEVT;
			break;
		}
		case SF_RVAR_PTEH:
		{
			r->addr = &S->superH->PTEH;
			break;
		}
		case SF_RVAR_PTEL:
		{
			r->addr = &S->superH->PTEL;
			break;
		}
		case SF_RVAR_TTB:
		{
			r->addr = &S->superH->TTB;
			break;
		}

		case SF_RVAR_TEA:
		{
			r->addr = &S->superH->TEA;
			break;
		}
		case SF_RVAR_MMUCR:
		{
			r->addr = &S->superH->MMUCR;
			break;
		}
		case SF_RVAR_ICR:
		{
			r->addr = &S->superH->ICR;
			break;
		}
		case SF_RVAR_ICRA:
		{
			r->addr = &S->superH->ICRA;
			break;
		}
		case SF_RVAR_ICRB:
		{
			r->addr = &S->superH->ICRB;
			break;
		}
		case SF_RVAR_R0_BANK0:
		case SF_RVAR_R1_BANK0:
		case SF_RVAR_R2_BANK0:
		case SF_RVAR_R4_BANK0:
		case SF_RVAR_R5_BANK0:
		case SF_RVAR_R6_BANK0:
		case SF_RVAR_R7_BANK0:
		{
			r->addr = &S->superH->R[id - SF_RVAR_R0_BANK0];
			break;
		}
		case SF_RVAR_R0_BANK1:
		case SF_RVAR_R1_BANK1:
		case SF_RVAR_R2_BANK1:
		case SF_RVAR_R3_BANK1:
		case SF_RVAR_R4_BANK1:
		case SF_RVAR_R5_BANK1:
		case SF_RVAR_R6_BANK1:
		case SF_RVAR_R7_BANK1:
		{
			r->addr = &S->superH->R_BANK[id - SF_RVAR_R0_BANK1];
			break;
		}
		case SF_RVAR_R8:
		case SF_RVAR_R9:
		case SF_RVAR_R10:
		case SF_RVAR_R11:
		case SF_RVAR_R12:
		case SF_RVAR_R13:
		case SF_RVAR_R14:
		case SF_RVAR_R15:
		{
			r->addr = &S->superH->R[id - SF_RVAR_R8];
			break;
		}
		case SF_RVAR_MACH:
		{
			r->addr = &S->superH->MACH;
			break;
		}
		case SF_RVAR_MACL:
		{
			r->addr = &S->superH->MACL;
			break;
		}
		case SF_RVAR_SSR:
		{
			r->addr = &S->superH->SSR;
			break;
		}
		case SF_RVAR_SR:
		{
			r->addr = &S->superH->SR;
			break;
		}
		case SF_RVAR_PR:
		{
			r->addr = &S->superH->PR;
			break;
		}
		case SF_RVAR_SPC:
		{
			r->addr = &S->superH->SPC;
			break;
		}
		case SF_RVAR_GBR:
		{
			r->addr = &S->superH->GBR;
			break;
		}
		case SF_RVAR_VBR:
		{
			r->addr = &S->superH->VBR;
			break;
		}

		default:
		{
			merror(E, "Sanity check failed: Invalid rvar id for machinetype superH");
			mfree(E, r, SF_FILE_MACRO);
			return;
		}
	}
	//else if (S->machinetype == MACHINE_MSP430) switch(id) {}

	switch(id)
	{
	/*	Node-specific	*/
		case SF_RVAR_mem_r_latency:
		{
			r->addr = &S->mem_r_latency;
			break;
		}
		case SF_RVAR_mem_w_latency:
		{
			r->addr = &S->mem_w_latency;
			break;
		}
		case SF_RVAR_MEMSIZE:
		{
			r->addr = &S->MEMSIZE;
			break;
		}
		case SF_RVAR_MEMBASE:
		{
			r->addr = &S->MEMBASE;
			break;
		}
		case SF_RVAR_MEMEND:
		{
			r->addr = &S->MEMEND;
			break;
		}
		case SF_RVAR_VDD:
		{
			r->addr = &S->VDD;
			break;
		}
		case SF_RVAR_LOWVDD:
		{
			r->addr = &S->LOWVDD;
			break;
		}
		case SF_RVAR_SVDD:
		{
			r->addr = &S->SVDD;
			break;
		}
		case SF_RVAR_temperature:
		{
			r->addr = &S->temperature;
			break;
		}
		case SF_RVAR_voltscale_alpha:
		{
			r->addr = &S->voltscale_alpha;
			break;
		}
		case SF_RVAR_voltscale_Vt:
		{
			r->addr = &S->voltscale_Vt;
			break;
		}
		case SF_RVAR_voltscale_K:
		{
			r->addr = &S->voltscale_K;
			break;
		}
		case SF_RVAR_Sensor_reading:
		{
			if (idx >= MAX_NODE_SENSORS)
			{
				merror(E, "idx spplied in rvar registration for sensor.reading is out of range");
				mfree(E, r, SF_FILE_MACRO);

				return;
			}

			r->addr = &S->sensors[idx].reading;
			break;
		}
		case SF_RVAR_force_avgpwr:
		{
			r->addr = &S->force_avgpwr;
			break;
		}
		case SF_RVAR_force_sleeppwr:
		{
			r->addr = &S->force_sleeppwr;
			break;
		}
		case SF_RVAR_CYCLETIME:
		{
			r->addr = &S->CYCLETIME;
			break;
		}
		case SF_RVAR_PC:
		{
			r->addr = &S->PC;
			break;
		}
		case SF_RVAR_STARTPC:
		{
			r->addr = &S->STARTPC;
			break;
		}
		case SF_RVAR_NODE_ID:
		{
			r->addr = &S->NODE_ID;
			break;
		}
		case SF_RVAR_fail_clocks_left:
		{
			r->addr = &S->fail_clocks_left;
			break;
		}
		case SF_RVAR_got_correlated_failure:
		{
			r->addr = &S->got_correlated_failure;
			break;
		}
		case SF_RVAR_faultthreshold:
		{
			r->addr = &S->faultthreshold;
			break;
		}
		case SF_RVAR_ENABLE_TOO_MANY_FAULTS:
		{
			r->addr = &S->ENABLE_TOO_MANY_FAULTS;
			break;
		}
		case SF_RVAR_clock_modulus:
		{
			r->addr = &S->clock_modulus;
			break;
		}
		case SF_RVAR_ENABLE_BATT_LOW_INTR:
		{
			r->addr = &S->ENABLE_BATT_LOW_INTR;
			break;
		}
		case SF_RVAR_battery_alert_frac:
		{
			r->addr = &S->battery_alert_frac;
			break;
		}
		case SF_RVAR_runnable:
		{
			r->addr = &S->runnable;
			break;
		}
		case SF_RVAR_pipelined:
		{
			r->addr = &S->pipelined;
			break;
		}
		case SF_RVAR_pipeshow:
		{
			r->addr = &S->pipeshow;
			break;
		}
		case SF_RVAR_sleep:
		{
			r->addr = &S->sleep;
			break;
		}
		case SF_RVAR_xloc:
		{
			r->addr = &S->xloc;
			break;
		}
		case SF_RVAR_yloc:
		{
			r->addr = &S->yloc;
			break;
		}

		case SF_RVAR_zloc:
		{
			r->addr = &S->zloc;
			break;
		}


	/*	Global state	*/
		case SF_RVAR_SIM_THROTTLE_NANOSEC:
		{
			r->addr = &E->throttlensec;
			break;
		}
		case SF_RVAR_SIM_THROTTLE_WINDOW:
		{
			r->addr = &E->throttlewin;
			break;
		}
		case SF_RVAR_SIM_QUANTUM:
		{
			r->addr = &E->quantum;
			break;
		}
		case SF_RVAR_SIM_BASENODEID:
		{
			r->addr = &E->baseid;
			break;
		}
		case SF_RVAR_CUR_NODE_ID:
		{
			r->addr = &E->cn;
			break;
		}
		case SF_RVAR_SIM_ON:
		{
			r->addr = &E->on;
			break;
		}
		case SF_RVAR_SIM_FAULT_PERIOD:
		{
			r->addr = &E->fperiodpsec;
			break;
		}
		case SF_RVAR_SIM_PHYSICS_PERIOD:
		{
			r->addr = &E->phyperiodpsec;
			break;
		}
		case SF_RVAR_SIM_BATTFEED_PERIOD:
		{
			r->addr = &E->battperiodpsec;
			break;
		}
		case SF_RVAR_SIM_DUMPPWR_PERIOD:
		{
			r->addr = &E->dumpperiodpsec;
			break;
		}

		case SF_RVAR_Signalsrc_valid:
		case SF_RVAR_Signalsrc_ID:
		case SF_RVAR_Signalsrc_type:
		case SF_RVAR_Signalsrc_updates_tau:
		case SF_RVAR_Signalsrc_propagation_speed:
		case SF_RVAR_Signalsrc_const_coeff_A:
		case SF_RVAR_Signalsrc_const_coeff_B:
		case SF_RVAR_Signalsrc_const_coeff_C:
		case SF_RVAR_Signalsrc_const_coeff_D:
		case SF_RVAR_Signalsrc_const_coeff_E:
		case SF_RVAR_Signalsrc_const_coeff_F:
		case SF_RVAR_Signalsrc_const_coeff_G:
		case SF_RVAR_Signalsrc_const_coeff_H:
		case SF_RVAR_Signalsrc_const_coeff_I:
		case SF_RVAR_Signalsrc_const_base_K:
		case SF_RVAR_Signalsrc_const_pow_m:
		case SF_RVAR_Signalsrc_const_pow_n:
		case SF_RVAR_Signalsrc_const_pow_o:
		case SF_RVAR_Signalsrc_const_pow_p:
		case SF_RVAR_Signalsrc_const_pow_q:
		case SF_RVAR_Signalsrc_const_pow_r:
		case SF_RVAR_Signalsrc_const_pow_s:
		case SF_RVAR_Signalsrc_const_pow_t:
		case SF_RVAR_Signalsrc_xloc:
		case SF_RVAR_Signalsrc_yloc:
		case SF_RVAR_Signalsrc_zloc:
		case SF_RVAR_Signalsrc_trajectory_rate:
		case SF_RVAR_Signalsrc_looptrajectory:
		case SF_RVAR_Signalsrc_sample:
		case SF_RVAR_Signalsrc_sample_rate:
		case SF_RVAR_Signalsrc_loopsamples:	
		{
			if (idx >= MAX_SIGNAL_SRCS)
			{
				merror(E, "idx spplied in rvar registration for Signalsrc.valid is out of range");
				mfree(E, r, SF_FILE_MACRO);

				return;
			}

			switch (id)
			{
				case SF_RVAR_Signalsrc_valid:
				{
					r->addr = &E->sigsrcs[idx].valid;
					break;
				}
				case SF_RVAR_Signalsrc_ID:
				{
					// BUG: should the ID be allowed to be a rvar ?
					r->addr = &E->sigsrcs[idx].ID;
					break;
				}
				case SF_RVAR_Signalsrc_type:
				{
					r->addr = &E->sigsrcs[idx].type;
					break;
				}
				case SF_RVAR_Signalsrc_updates_tau:
				{
					r->addr = &E->sigsrcs[idx].updates_tau;
					break;
				}
				case SF_RVAR_Signalsrc_propagation_speed:
				{
					r->addr = &E->sigsrcs[idx].propagation_speed;
					break;
				}
				case SF_RVAR_Signalsrc_const_coeff_A:
				{
					r->addr = &E->sigsrcs[idx].const_coeff_A;
					break;
				}
				case SF_RVAR_Signalsrc_const_coeff_B:
				{
					r->addr = &E->sigsrcs[idx].const_coeff_B;
					break;
				}
				case SF_RVAR_Signalsrc_const_coeff_C:
				{
					r->addr = &E->sigsrcs[idx].const_coeff_C;
					break;
				}
				case SF_RVAR_Signalsrc_const_coeff_D:
				{
					r->addr = &E->sigsrcs[idx].const_coeff_D;
					break;
				}
				case SF_RVAR_Signalsrc_const_coeff_E:
				{
					r->addr = &E->sigsrcs[idx].const_coeff_E;
					break;
				}
				case SF_RVAR_Signalsrc_const_coeff_F:
				{
					r->addr = &E->sigsrcs[idx].const_coeff_F;
					break;
				}
				case SF_RVAR_Signalsrc_const_coeff_G:
				{
					r->addr = &E->sigsrcs[idx].const_coeff_G;
					break;
				}
				case SF_RVAR_Signalsrc_const_coeff_H:
				{
					r->addr = &E->sigsrcs[idx].const_coeff_H;
					break;
				}
				case SF_RVAR_Signalsrc_const_coeff_I:
				{
					r->addr = &E->sigsrcs[idx].const_coeff_I;
					break;
				}
				case SF_RVAR_Signalsrc_const_base_K:
				{
					r->addr = &E->sigsrcs[idx].const_base_K;
					break;
				}
				case SF_RVAR_Signalsrc_const_pow_m:
				{
					r->addr = &E->sigsrcs[idx].const_pow_m;
					break;
				}
				case SF_RVAR_Signalsrc_const_pow_n:
				{
					r->addr = &E->sigsrcs[idx].const_pow_n;
					break;
				}
				case SF_RVAR_Signalsrc_const_pow_o:
				{
					r->addr = &E->sigsrcs[idx].const_pow_o;
					break;
				}
				case SF_RVAR_Signalsrc_const_pow_p:
				{
					r->addr = &E->sigsrcs[idx].const_pow_p;
					break;
				}
				case SF_RVAR_Signalsrc_const_pow_q:
				{
					r->addr = &E->sigsrcs[idx].const_pow_q;
					break;
				}
				case SF_RVAR_Signalsrc_const_pow_r:
				{
					r->addr = &E->sigsrcs[idx].const_pow_r;
					break;
				}
				case SF_RVAR_Signalsrc_const_pow_s:
				{
					r->addr = &E->sigsrcs[idx].const_pow_s;
					break;
				}
				case SF_RVAR_Signalsrc_const_pow_t:
				{
					r->addr = &E->sigsrcs[idx].const_pow_t;
					break;
				}
				case SF_RVAR_Signalsrc_xloc:
				{
					r->addr = &E->sigsrcs[idx].xloc;
					break;
				}
				case SF_RVAR_Signalsrc_yloc:
				{
					r->addr = &E->sigsrcs[idx].yloc;
					break;
				}
				case SF_RVAR_Signalsrc_zloc:
				{
					r->addr = &E->sigsrcs[idx].zloc;
					break;
				}
				case SF_RVAR_Signalsrc_trajectory_rate:
				{
					r->addr = &E->sigsrcs[idx].trajectory_rate;
					break;
				}
				case SF_RVAR_Signalsrc_looptrajectory:
				{
					r->addr = &E->sigsrcs[idx].looptrajectory;
					break;
				}
				case SF_RVAR_Signalsrc_sample:
				{
					r->addr = &E->sigsrcs[idx].sample;
					break;
				}
				case SF_RVAR_Signalsrc_sample_rate:
				{
					r->addr = &E->sigsrcs[idx].sample_rate;
					break;
				}
				case SF_RVAR_Signalsrc_loopsamples:
				{
					r->addr = &E->sigsrcs[idx].loopsamples;
					break;
				}
				default:
				{
					merror(E, "Sanity check failed: Invalid rvar id");
					mfree(E, r, SF_FILE_MACRO);
					return;
				}
			}
		}
		default:
		{
			merror(E, "Sanity check failed: Invalid rvar id");
			mfree(E, r, SF_FILE_MACRO);
			return;
		}
	}

	E->rvars[id] = r;
	E->validrvars[E->nvalidrvars++] = id;


	return;
}

void
m_listrvars(Engine *E)
{
}

void
m_updatervars(Engine *E)
{
	/*								*/
	/*	Called from the main loop to update items which are	*/
	/*	defined from rvars.					*/
	/*								*/
	int	i;
	Rvar	*p;


	if (E->globaltimepsec < E->rvarsnextpsec)
	{
		return;
	}

	E->rvarsnextpsec = PICOSEC_MAX;
	for (i = 0; i < E->nvalidrvars; i++)
	{
		p = E->rvars[E->validrvars[i]];

		if (E->globaltimepsec >= p->nextupdate)
		{
			double val = 0, dur = 0;

	
			if (p->valdistfnid >= 0)
			{
				val = SF_PFUN_FPTRS[p->valdistfnid](E,
						p->min,
						p->max,
						p->p1,
						p->p2,
						p->p3,
						p->p4);
			}
			else if (p->valdisttabid >= 0)
			{
				val = E->randtabs[p->valdisttabid].table[mrandom(E) % E->randtabs[p->valdisttabid].size];
			}
			else
			{
				merror(E, "Sanity check failed, aborting updatervars...");
				return;
			}

			if (p->durdistfnid >= 0)
			{
				dur = SF_PFUN_FPTRS[p->durdistfnid](E,
						p->min,
						p->max,
						p->p1,
						p->p2,
						p->p3,
						p->p4);
			}
			else if (p->durdisttabid >= 0)
			{
				dur = E->randtabs[p->durdisttabid].table[
						mrandom(E) % E->randtabs[p->durdisttabid].size];
			}
			else
			{
				merror(E, "Sanity check failed, aborting updatervars...");
				return;
			}

			p->nextupdate = E->globaltimepsec + dur;

			switch (p->type)
			{
				case SF_RVAR_TYPE_char:
				{
					if (p->mode == SF_RVAR_MODE_REPL)
					{
						*((char *)p->addr) = (char)ceil(val);
					}
					else if (p->mode == SF_RVAR_MODE_ADD)
					{
						*((char *)p->addr) += (char)ceil(val);
					}
					break;
				}

				case SF_RVAR_TYPE_short:
				{
					if (p->mode == SF_RVAR_MODE_REPL)
					{
						*((short *)p->addr) = (short)ceil(val);
					}
					else if (p->mode == SF_RVAR_MODE_ADD)
					{
						*((short *)p->addr) += (short)ceil(val);
					}
					break;
				}

				case SF_RVAR_TYPE_int:
				{
					if (p->mode == SF_RVAR_MODE_REPL)
					{
						*((int *)p->addr) = (int)ceil(val);
					}
					else if (p->mode == SF_RVAR_MODE_ADD)
					{
						*((int *)p->addr) += (int)ceil(val);
					}
					break;
				}
	
				case SF_RVAR_TYPE_ulong:
				{
					if (p->mode == SF_RVAR_MODE_REPL)
					{
						*((ulong *)p->addr) = (ulong)ceil(val);
					}
					else if (p->mode == SF_RVAR_MODE_ADD)
					{
						*((ulong *)p->addr) += (ulong)ceil(val);
					}
					break;
				}
	
				case SF_RVAR_TYPE_uvlong:
				{
					if (p->mode == SF_RVAR_MODE_REPL)
					{
						*((uvlong *)p->addr) = (uvlong)ceil(val);
					}
					else if (p->mode == SF_RVAR_MODE_ADD)
					{
						*((uvlong *)p->addr) += (uvlong)ceil(val);
					}
					break;
				}
	
				case SF_RVAR_TYPE_float:
				{
					if (p->mode == SF_RVAR_MODE_REPL)
					{
						*((float *)p->addr) = (float)val;
					}
					else if (p->mode == SF_RVAR_MODE_ADD)
					{
						*((float *)p->addr) += (float)val;
					}

					break;
				}
				
				case SF_RVAR_TYPE_double:
				{
					if (p->mode == SF_RVAR_MODE_REPL)
					{
						*((double *)p->addr) = val;
					}
					else if (p->mode == SF_RVAR_MODE_ADD)
					{
						*((double *)p->addr) += val;
					}
					
					break;
				}

				default:
				{
					merror(E, "Sanity check failed");
				}
			}
		}

		E->rvarsnextpsec = min(E->rvarsnextpsec, p->nextupdate);
	}
}
