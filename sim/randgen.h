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

/*		M-N: Period parameters		*/
#define RANDGEN_NN		312
#define RANDGEN_M0		63
#define RANDGEN_M1		151
#define RANDGEN_M2		224

/*	   M-N: Constant vector a		*/
#define RANDGEN_MATRIX_A	0xB3815B624FC82E2FULL

/*	M-N: Most significant 33 bits		*/
#define RANDGEN_UMASK		0xFFFFFFFF80000000ULL

/*	M-N: Least significant 31 bits		*/
#define RANDGEN_LMASK 0x7FFFFFFFULL

/*	M-N: Tempering parameters		*/
#define RANDGEN_MASK_B		0x599CFCBFCA660000ULL
#define RANDGEN_MASK_C		0xFFFAAFFE00000000ULL
#define RANDGEN_UU		26
#define RANDGEN_SS		17
#define RANDGEN_TT		33
#define RANDGEN_LL		39

/*										*/
/*	NOTE: these are defined in main.h due to dependencies. They are		*/
/*	mirrored here for informational purposes.				*/
/*										*/
/*
typedef struct
{
	double	*table;
	int	size;
} RandTable;

enum
{
	MAX_RANDTABLEENTRIES = 8192,
	MAX_RVARENTRIES = 128,
};

typedef struct
{
	int	id;
	int	idx;
	int	valdistfnid;
	int	valdisttabid;
	int	durdistfnid;
	int	durdisttabid;
	int	type;
	int	mode;

	char	*name;
	void	*addr;

	Picosec	nextupdate;
} Rvar;
*/

typedef double (*RandgenPrfptr)(Engine*, double, double, double, double, double);
typedef double (*RandgenPfunfptr)(Engine*, double, double, double, double, double, double);




typedef struct
{
	double	p1;
	double	p2;
	double	p3;
	double	p4;
} PFUNparams;

/*	We warn if a created randtable will have more than this many support values	*/
#define	SF_HUGE_RANDTABLE 1000

enum
{
	SF_RVAR_MODE_REPL,
	SF_RVAR_MODE_ADD,
};

enum
{
	SF_PFUN_UNIFORM,
	SF_PFUN_EXP,
	SF_PFUN_GAUSS,
	SF_PFUN_BPARETO,
	SF_PFUN_WEIBULL,
	SF_PFUN_XI2,
	SF_PFUN_CAUCHY,
	SF_PFUN_XI,
	SF_PFUN_BETA,
	SF_PFUN_EXTREMEVAL,
	SF_PFUN_F,
	SF_PFUN_FERMIDIRAC,
	SF_PFUN_FISHERZ,
	SF_PFUN_GUMBEL,
	SF_PFUN_GAMMA,
	SF_PFUN_HALFNORMAL,
	SF_PFUN_LAPLACE,
	SF_PFUN_LOGSERIES,
	SF_PFUN_LOGISTIC,
	SF_PFUN_MAXWELL,
	SF_PFUN_NEGBINOMIAL,
	SF_PFUN_PEARSONTYPE3,
	SF_PFUN_POISSON,
	SF_PFUN_RAYLEIGH,
	SF_PFUN_STUDENTST,
	SF_PFUN_STUDENTSZ,
	SF_PFUN_BETAPRIME,
	SF_PFUN_ERLANG,
	SF_PFUN_LOGNORMAL,
	SF_PFUN_GIBRAT,
	SF_PFUN_BATHTUB,

	SF_PFUN_MAXPFUN
};

enum
{
	SF_RVAR_flash_r_latency,
	SF_RVAR_flash_w_latency,
	SF_RVAR_cache_activated,
	SF_RVAR_ENABLE_CLK_INTR,
	SF_RVAR_rxok_intrenable_flag,
	SF_RVAR_txok_intrenable_flag,
	SF_RVAR_addrerr_intrenable_flag,
	SF_RVAR_frameerr_intrenable_flag,
	SF_RVAR_collserr_intrenable_flag,
	SF_RVAR_csenseerr_intrenable_flag,
	SF_RVAR_rxovrrunerr_intrenable_flag,
	SF_RVAR_txovrrunerr_intrenable_flag,
	SF_RVAR_rxundrrunerr_intrenable_flag,
	SF_RVAR_txundrrunerr_intrenable_flag,
	SF_RVAR_csumerr_intrenable_flag,
	SF_RVAR_TIMER_INTR_DELAY,
	SF_RVAR_TRA,
	SF_RVAR_EXPEVT,
	SF_RVAR_INTEVT,
	SF_RVAR_PTEH,
	SF_RVAR_PTEL,
	SF_RVAR_TTB,
	SF_RVAR_TEA,
	SF_RVAR_MMUCR,
	SF_RVAR_ICR,
	SF_RVAR_ICRA,
	SF_RVAR_ICRB,
	SF_RVAR_R0_BANK0,
	SF_RVAR_R1_BANK0,
	SF_RVAR_R2_BANK0,
	SF_RVAR_R3_BANK0,
	SF_RVAR_R4_BANK0,
	SF_RVAR_R5_BANK0,
	SF_RVAR_R6_BANK0,
	SF_RVAR_R7_BANK0,
	SF_RVAR_R0_BANK1,
	SF_RVAR_R1_BANK1,
	SF_RVAR_R2_BANK1,
	SF_RVAR_R3_BANK1,
	SF_RVAR_R4_BANK1,
	SF_RVAR_R5_BANK1,
	SF_RVAR_R6_BANK1,
	SF_RVAR_R7_BANK1,
	SF_RVAR_R8,
	SF_RVAR_R9,
	SF_RVAR_R10,
	SF_RVAR_R11,
	SF_RVAR_R12,
	SF_RVAR_R13,
	SF_RVAR_R14,
	SF_RVAR_R15,
	SF_RVAR_MACH,
	SF_RVAR_MACL,
	SF_RVAR_SSR,
	SF_RVAR_SR,
	SF_RVAR_PR,
	SF_RVAR_SPC,
	SF_RVAR_GBR,
	SF_RVAR_VBR,
	SF_RVAR_mem_r_latency,
	SF_RVAR_mem_w_latency,
	SF_RVAR_MEMSIZE,
	SF_RVAR_MEMBASE,
	SF_RVAR_MEMEND,
	SF_RVAR_MEMbyte,
	SF_RVAR_MEMword,
	SF_RVAR_MEMlongword,
	SF_RVAR_VDD,
	SF_RVAR_LOWVDD,
	SF_RVAR_SVDD,
	SF_RVAR_temperature,
	SF_RVAR_voltscale_alpha,
	SF_RVAR_voltscale_Vt,
	SF_RVAR_voltscale_K,
	SF_RVAR_Sensor_reading,
	SF_RVAR_force_avgpwr,
	SF_RVAR_force_sleeppwr,
	SF_RVAR_CYCLETIME,
	SF_RVAR_PC,
	SF_RVAR_STARTPC,
	SF_RVAR_NODE_ID,
	SF_RVAR_fail_clocks_left,
	SF_RVAR_got_correlated_failure,
	SF_RVAR_faultthreshold,
	SF_RVAR_ENABLE_TOO_MANY_FAULTS,
	SF_RVAR_clock_modulus,
	SF_RVAR_ENABLE_BATT_LOW_INTR,
	SF_RVAR_battery_alert_frac,
	SF_RVAR_runnable,
	SF_RVAR_pipelined,
	SF_RVAR_pipeshow,
	SF_RVAR_sleep,
	SF_RVAR_orbit,
	SF_RVAR_velocity,
	SF_RVAR_xloc,
	SF_RVAR_yloc,
	SF_RVAR_zloc,
	SF_RVAR_SIM_THROTTLE_NANOSEC,
	SF_RVAR_SIM_THROTTLE_WINDOW,
	SF_RVAR_SIM_QUANTUM,
	SF_RVAR_SIM_BASENODEID,
	SF_RVAR_CUR_NODE_ID,
	SF_RVAR_SIM_ON,
	SF_RVAR_SIM_BATT_LEAK_CURRENT,
	SF_RVAR_SIM_FAULT_PERIOD,
	SF_RVAR_SIM_PHYSICS_PERIOD,
	SF_RVAR_SIM_BATTFEED_PERIOD,
	SF_RVAR_SIM_DUMPPWR_PERIOD,
	SF_RVAR_Signalsrc_valid,
	SF_RVAR_Signalsrc_ID,
	SF_RVAR_Signalsrc_type,
	SF_RVAR_Signalsrc_updates_tau,
	SF_RVAR_Signalsrc_propagation_speed,
	SF_RVAR_Signalsrc_const_coeff_A,
	SF_RVAR_Signalsrc_const_coeff_B,
	SF_RVAR_Signalsrc_const_coeff_C,
	SF_RVAR_Signalsrc_const_coeff_D,
	SF_RVAR_Signalsrc_const_coeff_E,
	SF_RVAR_Signalsrc_const_coeff_F,
	SF_RVAR_Signalsrc_const_coeff_G,
	SF_RVAR_Signalsrc_const_coeff_H,
	SF_RVAR_Signalsrc_const_coeff_I,
	SF_RVAR_Signalsrc_const_base_K,
	SF_RVAR_Signalsrc_const_pow_m,
	SF_RVAR_Signalsrc_const_pow_n,
	SF_RVAR_Signalsrc_const_pow_o,
	SF_RVAR_Signalsrc_const_pow_p,
	SF_RVAR_Signalsrc_const_pow_q,
	SF_RVAR_Signalsrc_const_pow_r,
	SF_RVAR_Signalsrc_const_pow_s,
	SF_RVAR_Signalsrc_const_pow_t,
	SF_RVAR_Signalsrc_xloc,
	SF_RVAR_Signalsrc_yloc,
	SF_RVAR_Signalsrc_zloc,
	SF_RVAR_Signalsrc_trajectory_rate,
	SF_RVAR_Signalsrc_looptrajectory,
	SF_RVAR_Signalsrc_sample,
	SF_RVAR_Signalsrc_sample_rate,
	SF_RVAR_Signalsrc_loopsamples,

	SF_RVAR_MAXNAME,
};

enum
{
	SF_RVAR_TYPE_char,
	SF_RVAR_TYPE_short,
	SF_RVAR_TYPE_int,
	SF_RVAR_TYPE_ulong,
	SF_RVAR_TYPE_uvlong,
	SF_RVAR_TYPE_float,
	SF_RVAR_TYPE_double,
};

typedef struct
{
	int	type;
	char	*name;
}SimRvar;

