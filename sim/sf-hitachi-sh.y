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

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <math.h>
#include "endian-hitachi-sh.h"
#include "sf.h"
#include "mextern.h"

/*
 *	Prevent generated parser from allocating memory on stack, useful
 *	for when running Sunflower on embedded targets.
 */
#define	YYSTACK_USE_ALLOCA	0
%}


/*									*/
/*	The 'value' field will always hold a value, likewise the	*/
/*	'type' field.  The 'rv' field will be populated	if the		*/
/*	type field indicates that this is a random variable.		*/
/*	Random constants look just like other immediate constants	*/
/*	and do not have rv field set.  Random variables will always	*/
/*	have the value field set to their initial value.		*/
/*									*/
/*	Grammar rules can get by by using only $X.value.  If the	*/
/*	semantics of random variables for arguments make sense, it	*/
/*	can additionally supply the $X.type and $X.rv to the m_...()	*/
/*	function it invokes, to enable registering the appropriate	*/
/*	variables as random variables.					*/
/*									*/
%union
{
	double		dval;
	ulong		uval;
	long		sval;
	char		*str;
	Rval		*rval;
	DoubleList	*dlist;
}


/*	Sunflower Commands	*/
%token	T_BATTALERTFRAC
%token	T_BATTLEAK
%token	T_BATTCF
%token	T_BATTILEAK
%token	T_BATTETALUT
%token	T_BATTETALUTNENTRIES
%token	T_BATTINOMINAL
%token	T_BATTNODEATTACH
%token	T_BATTRF
%token	T_BATTSTATS
%token	T_BATTVBATTLUT
%token	T_BATTVBATTLUTNENTRIES
%token	T_BATTVLOSTLUT
%token	T_BATTVLOSTLUTNENTRIES
%token	T_PCBT
%token	T_CACHEINIT
%token	T_CACHEOFF
%token	T_CACHESTATS
%token	T_CD
%token	T_CLOCKINTR
%token	T_COMMENT
%token	T_CONT
%token	T_DISABLEPIPELINE
%token	T_DUMPALL
%token	T_DUMPMEM
%token	T_DUMPPIPE
%token	T_DUMPDISTRIBUTION
%token	T_DUMPPWR
%token	T_DUMPREGS
%token	T_DUMPSYSREGS
%token	T_DUMPTIME
%token	T_DUMPTLB
%token	T_DYNINSTR
%token	T_EBATTINTR
%token	T_EFAULTS
%token	T_ENABLEPIPELINE
%token	T_FILE2NETSEG
%token	T_FLTTHRESH
%token	T_FORCEAVGPWR
%token	T_HELP
%token	T_IGNORENODEDEATHS
%token	T_INITSEESTATE
%token	T_HWSEEREG
%token	T_REGISTERRVAR
%token	T_INITRANDTABLE
%token	T_DEFNDIST
%token	T_RANDPRINT
%token	T_LOAD
%token	T_LOCSTATS
%token	T_LISTRVARS
%token	T_MALLOCDEBUG
%token	T_MMAP
%token	T_MODECA
%token	T_MODEFF
%token	T_NETCORREL
%token	T_NETDEBUG
%token	T_NETNEWSEG
%token	T_NETNODENEWIFC
%token	T_NETSEG2FILE
%token	T_NETSEGDELETE
%token	T_NETSEGFAILDURMAX
%token	T_NETSEGFAILPROB
%token	T_NETSEGFAILPROBFN
%token	T_NETSEGNICATTACH
%token	T_NETSEGPROPMODEL
%token	T_NEWBATT
%token	T_NEWNODE
%token	T_NODEFAILDURMAX
%token	T_NODEFAILPROB
%token	T_NODEFAILPROBFN
%token	T_NODETACH
%token	T_NUMAREGION
%token	T_NUMASTATS
%token	T_NUMASTATSALL
%token	T_NUMASETMAPID
%token	T_ADDVALUETRACE
%token	T_DELVALUETRACE
%token	T_REGISTERSTABS
%token	T_VALUESTATS
%token	T_OFF
%token	T_ON
%token	T_PAUINFO
%token	T_NANOPAUSE
%token	T_PAUSE
%token	T_PFUN
%token	T_PIPEFLUSH
%token	T_POWERSTATS
%token	T_POWERTOTAL
%token	T_PWD
%token	T_PARSEOBJDUMP
%token	T_QUIT
%token	T_GETRANDOMSEED
%token	T_SETRANDOMSEED
%token	T_RATIO
%token	T_RENUMBERNODES
%token	T_RESETNODECTRS
%token	T_RESETALLCTRS
%token	T_RESETCPU
%token	T_RETRYALG
%token	T_RUN
%token	T_SAVE
%token	T_SENSORSDEBUG
%token	T_SETBASENODEID
%token	T_SETBATT
%token	T_SETBATTFEEDPERIOD
%token	T_SETDUMPPWRPERIOD
%token	T_SETNETPERIOD
%token	T_SETFAULTPERIOD
%token	T_SETFREQ
%token	T_SETIFCOUI
%token	T_SETMEMBASE
%token	T_SETNODE
%token	T_SETPC
%token	T_SETPHYSICSPERIOD
%token	T_SETQUANTUM
%token	T_SETSCALEALPHA
%token	T_SETSCALEK
%token	T_SETSCALEVT
%token	T_SETSCHEDRANDOM
%token	T_SETSCHEDROUNDROBIN
%token	T_SETTAG
%token	T_SETTIMERDELAY
%token	T_SETVDD
%token	T_SETMEMRLATENCY
%token	T_SETMEMWLATENCY
%token	T_SETFLASHRLATENCY
%token	T_SETFLASHWLATENCY
%token	T_SETNODEMASS
%token	T_SETPROPULSIONCOEFFS
%token	T_SHAREBUS
%token	T_SHOWCLK
%token	T_SHOWMEMBASE
%token	T_SHOWPIPE
%token	T_SHOWTAGS
%token	T_SIGNALSRC
%token	T_SIGNALSUBSCRIBE
%token	T_SIZEMEM
%token	T_SIZEPAU
%token	T_SPLIT
%token	T_SRECL
%token	T_LOADMAPFILE
%token	T_STEP
%token	T_STOP
%token	T_SFATAL
%token	T_TRACE
%token	T_THROTTLE
%token	T_THROTTLEWIN
%token	T_VERBOSE
%token	T_VERSION
%token	T_SETLOC
%token	T_BPT
%token	T_BPTLS
%token	T_BPTDEL


/*	Breakpoint types	*/
%token	T_GLOBALTIME
%token	T_CYCLES
%token	T_INSTRS
%token	T_SENSORREADING

/*	Assembler control instructions 		*/
%token	T_DOTALIGN
%token	T_DOTORG
%token	T_DOTDATA
%token	T_DOTTEXT
%token	T_DOTGLOBAL
%token	T_DOTFILE
%token	T_DOTLONG
%token	T_DOTCOMM

/*	SuperH Registers	*/
%token	T_R0
%token	T_R1
%token	T_R2
%token	T_R3
%token	T_R4
%token	T_R5
%token	T_R6
%token	T_R7
%token	T_R8
%token	T_R9
%token	T_R10
%token	T_R11
%token	T_R12
%token	T_R13
%token	T_R14
%token	T_R15
%token	T_GBR
%token	T_VBR
%token	T_MACH
%token	T_MACL
%token	T_PC
%token	T_PR
%token	T_TRA
%token	T_SPC
%token	T_SR
%token	T_SSR

/*    	Misc		*/
%token	T_LABELDEFN

/*	SuperH Instructions	*/
%token	T_ADD
%token	T_ADDC
%token	T_ADDV
%token	T_AND
%token	T_ANDB
%token	T_BF
%token	T_BFS
%token	T_BRA
%token	T_BRAF	/*      TODO: This instr is not implemented in our interface assembler  	*/
%token	T_BSR
%token	T_BSRF
%token	T_BT
%token	T_BTS
%token	T_CLRMAC
%token	T_CLRS
%token	T_CLRT
%token	T_CMPEQ
%token	T_CMPGE
%token	T_CMPGT
%token	T_CMPHI
%token	T_CMPHS
%token	T_CMPPL
%token	T_CMPPZ
%token	T_CMPSTR
%token	T_DIV0S
%token	T_DIV0U	/*    TODO: This instr is not implemented in our interface assembler  	*/
%token	T_DIV1
%token	T_DMULSL
%token	T_DMULUL
%token	T_DT
%token	T_EXTSB
%token	T_EXTSW
%token	T_EXTUB
%token	T_EXTUW
%token	T_JMP
%token	T_JSR
%token	T_LDC
%token	T_LDCL
%token	T_LDS
%token	T_LDSL
%token	T_LDTLB
%token	T_MACL
%token	T_MACW
%token	T_MOV
%token	T_MOVB
%token	T_MOVL
%token	T_MOVW
%token	T_MOVA
%token	T_MOVT
%token	T_MULL
%token	T_MULS
%token	T_MULSW
%token	T_MULU
%token	T_MULUW
%token	T_NEG
%token	T_NEGC
%token	T_NOP
%token	T_NOT
%token	T_OR
%token	T_ORB
%token	T_PREF
%token	T_RFG 	/* 	New instr : Reconfigure (never used) 	*/
%token	T_ROTCL
%token	T_ROTCR
%token	T_ROTL
%token	T_ROTR
%token	T_RTE
%token	T_RTS
%token	T_SETS
%token	T_SETT
%token	T_SHAD
%token	T_SHAL
%token	T_SHAR
%token	T_SHLD
%token	T_SHLL
%token	T_SHLL2
%token	T_SHLL8
%token	T_SHLL16
%token	T_SHLR
%token	T_SHLR2
%token	T_SHLR8
%token	T_SHLR16
%token	T_SLEEP
%token	T_STC
%token	T_STCL
%token	T_STS
%token	T_STSL
%token	T_SUB
%token	T_SUBC
%token	T_SUBV
%token	T_SWAPB
%token	T_SWAPW
%token	T_TASB
%token	T_TRAPA
%token	T_TST
%token	T_TSTB
%token	T_XOR
%token	T_XORB
%token	T_XTRCT

%type	<uval>	reg
%type	<sval>	disp
%type	<uval>	uimm
%type	<sval>	simm
%type	<dval>	dimm
%type	<str>	optstring

/*									*/
/*	An rdimm immediate value indicates that either a dimm or a	*/
/*	rnd (random variable or random constant) can be used (i.e.,	*/
/*	rvars and rconsts are treated as though they were dimms, to	*/
/*	keep things simple (as opposed to differentiating between 	*/
/*	continuous r.v.'s as dimm, and discrete r.v.'s as uimms, etc.	*/
/*									*/
/*	TODO: update lex comments with designation of dimm/rdimm, so	*/
/*	that documentation reflects sites where we now permit an rdimm	*/
/*									*/
%type	<rval>	ruimm
%type	<rval>	rsimm
%type	<rval>	rdimm
%type	<rval>	rnd
%type	<rval>	rnd_const
%type	<rval>	rnd_var

/*
%type	<rval>	rnd_bathtub
%type	<rval>	rnd_beta
%type	<rval>	rnd_betaprime
%type	<rval>	rnd_bpareto
%type	<rval>	rnd_cauchy
%type	<rval>	rnd_chi
%type	<rval>	rnd_erlang
%type	<rval>	rnd_exp
%type	<rval>	rnd_extremeval
%type	<rval>	rnd_fdist
%type	<rval>	rnd_fermidirac
%type	<rval>	rnd_fisherz
%type	<rval>	rnd_gamma
%type	<rval>	rnd_gauss
%type	<rval>	rnd_gibrat
%type	<rval>	rnd_gumbel
%type	<rval>	rnd_halfnormal
%type	<rval>	rnd_laplace
%type	<rval>	rnd_logistic
%type	<rval>	rnd_lognorm
%type	<rval>	rnd_logseries
%type	<rval>	rnd_maxwell
%type	<rval>	rnd_negbinomial
%type	<rval>	rnd_pearsontype3
%type	<rval>	rnd_poisson
%type	<rval>	rnd_rayleigh
%type	<rval>	rnd_studentst
%type	<rval>	rnd_studentsz
%type	<rval>	rnd_uniform
%type	<rval>	rnd_weibull
%type	<rval>	rnd_xi2
*/

%type	<dlist>	dimmlist
%type	<dlist>	dimmlistbody

%token	<uval>	T_LABEL
%token	<str> 	T_STRING
%token	<str> 	T_MAN


%%
input		: /* empty */
		| input line
		;

line		: T_LABELDEFN
		| T_LABELDEFN expr
		| expr
		| asm_ctl
		| sf_cmd
		;

asm_ctl		: dotalign
		| dotdata
		| dottext
		| dotfile
		| dotorg
		| dotglobal
		| dotlong
		| dotfile
		| dotglobal
		| dotcomm
		;

expr		: add_instr
		| addi_instr
		| addc_instr
		| addv_instr
		| and_instr
		| andi_instr
		| andm_instr
		| bf_instr
		| bfs_instr
		| bra_instr
		| braf_instr
		| bsr_instr
		| bsrf_instr
		| bt_instr
		| bts_instr
		| clrmac_instr
		| clrs_instr
		| clrt_instr
		| cmpeq_instr
		| cmpge_instr
		| cmpgt_instr
		| cmphi_instr
		| cmphs_instr
		| cmppl_instr
		| cmppz_instr
		| cmpstr_instr
		| cmpim_instr
		| div0s_instr
		| div1_instr
		| dmuls_instr
		| dmulu_instr
		| dt_instr
		| extsb_instr
		| extsw_instr
		| extub_instr
		| extuw_instr
		| jmp_instr
		| jsr_instr
		| ldcr_bank_instr
		| ldcsr_instr
		| ldcgbr_instr
		| ldcvbr_instr
		| ldcssr_instr
		| ldcspc_instr
		| ldcmsr_instr
		| ldcmgbr_instr
		| ldcmvbr_instr
		| ldcmssr_instr
		| ldcmspc_instr
		| ldcmr_bank_instr
		| ldsmach_instr
		| ldsmacl_instr
		| ldspr_instr
		| ldsmmach_instr
		| ldsmmacl_instr
		| ldsmpr_instr
		| ldtlb_instr
		| macl_instr
		| macw_instr
		| mov_instr
		| movbs_instr
		| movws_instr
		| movls_instr
		| movbl_instr
		| movwl_instr
		| movll_instr
		| movbm_instr
		| movwm_instr
		| movlm_instr
		| movbp_instr
		| movwp_instr
		| movlp_instr
		| movbs0_instr
		| movws0_instr
		| movls0_instr
		| movbl0_instr
		| movwl0_instr
		| movll0_instr
		| movi_instr
		| movwi_instr
		| movli_instr
		| movblg_instr
		| movwlg_instr
		| movllg_instr
		| movbsg_instr
		| movwsg_instr
		| movlsg_instr
		| movbs4_instr
		| movws4_instr
		| movls4_instr
		| movbl4_instr
		| movwl4_instr
		| movll4_instr
		| mova_instr
		| movt_instr
		| mull_instr
		| muls_instr
		| mulsw_instr
		| mulu_instr
		| muluw_instr
		| neg_instr
		| negc_instr
		| nop_instr
		| not_instr
		| or_instr
		| ori_instr
		| orm_instr
		| pref_instr
		| rfg_instr	/*	New instr : Reconfigure	(unused)	*/
		| rotcl_instr
		| rotcr_instr
		| rotl_instr
		| rotr_instr
		| rte_instr
		| rts_instr
		| sets_instr
		| sett_instr
		| shad_instr
		| shal_instr
		| shar_instr
		| shld_instr
		| shll_instr
		| shll2_instr
		| shll8_instr
		| shll16_instr
		| shlr_instr
		| shlr2_instr
		| shlr8_instr
		| shlr16_instr
		| sleep_instr
		| stcr_bank_instr
		| stcsr_instr
		| stcgbr_instr
		| stcvbr_instr
		| stcssr_instr
		| stcspc_instr
		| stcmr_bank_instr
		| stcmsr_instr
		| stcmgbr_instr
		| stcmvbr_instr
		| stcmssr_instr
		| stcmspc_instr
		| stsmach_instr
		| stsmacl_instr
		| stspr_instr
		| stsmmach_instr
		| stsmmacl_instr
		| stsmpr_instr
		| sub_instr
		| subc_instr
		| subv_instr
		| swapb_instr
		| swapw_instr
		| tas_instr
		| trapa_instr
		| tst_instr
		| tsti_instr
		| tstb_instr
		| xor_instr
		| xori_instr
		| xorm_instr
		| xtrct_instr
		;


sf_cmd		: T_QUIT '\n'
		{
			if (!yyengine->scanning)
			{
				mexit(yyengine, "Exiting as per user's request.", 0);
			}
		}

		/*
		 *	NOTE: We no longer accept the old-style velocity and orbit syntax, though we allow you to omit either the x/y/z location, or the trajectory file, rate, and loop flag
		 */

		| T_NEWNODE optstring '\n'
		{
			/*
			 *	Only give the architecture type:
			 */
			if (!yyengine->scanning)
			{
				m_newnode(yyengine, $2, 0, 0, 0, NULL, 0, 0);
			}
		}
		| T_NEWNODE optstring rdimm rdimm rdimm '\n'
		{
			/*
			 *	Give the architecture type and the (fixed) x/y/z location
			 */
			if (!yyengine->scanning)
			{
				m_newnode(yyengine, $2, $3->dval, $3->dval, $3->dval, NULL, 0, 0);
			}
		}
		| T_NEWNODE optstring rdimm rdimm rdimm T_STRING uimm uimm '\n'
		{
			/*
			 *	Give the architecture type and the (initial) x/y/z location, and the
			 *	trajectory file, loop flag, and trajectory rate (the number of picoseconds
			 *	per sample in the trajectory file).
			 */
			if (!yyengine->scanning)
			{
				//	rdimm example: 	READ THE FOLLOWING!!!!
				//
				//	this is an example of using rdimm.  For rdimm,
				//	you can't get the value from, e.g., $4, but have to do $4.value
				//	The $X.rv field is the packaged rvar state needed to register
				//	the corresponding parameter using m_register_rvar(), if the
				//	rvar was declared as "{...}", or NULL if it was "<...>"
				//
				m_newnode(yyengine, $2, $3->dval, $4->dval, $5->dval, $6, $7, $8);

				//	The above should be changed to:
				//m_newnode($2, $3.value, $4.value, $5.value, $6.value, $7.value,
				//		$3.rv, $4.rv, $5.rv, $6.rv, $7.rv);
				//
				//	and then in m_newnode(), check each of the rv args to
				//	see if it is non-null, and if so, do approp. registration
				//	of state as an rvar
			}
		}		
		| T_BATTNODEATTACH uimm '\n'
		{
			if (!yyengine->scanning)
			{
				batt_nodeattach(yyengine, yyengine->cp, $2);
			}
		}
		| T_BATTSTATS uimm '\n'
		{
			if (!yyengine->scanning)
			{
				batt_printstats(yyengine, yyengine->cp, $2);
			}
		}
		| T_NEWBATT uimm dimm '\n'
		{
			if (!yyengine->scanning)
			{
				batt_newbatt(yyengine, $2, $3);
			}
		}
		| T_BATTILEAK dimm '\n'
		{
			if (!yyengine->scanning)
			{
				if (SF_DEBUG) mprint(yyengine, NULL, siminfo, 
						"Setting yyengine->batts[%d].ileak to %f\n",
						yyengine->curbatt, $2);
				yyengine->batts[yyengine->curbatt].ileak = $2;
			}
		}
		| T_BATTCF dimm '\n'
		{
			if (!yyengine->scanning)
			{
				if (SF_DEBUG) mprint(yyengine, NULL, siminfo, 
						"Setting yyengine->batts[%d].Cf to %f\n",
						yyengine->curbatt, $2);
				yyengine->batts[yyengine->curbatt].Cf = $2;
			}
		}
		| T_BATTINOMINAL dimm '\n'
		{
			if (!yyengine->scanning)
			{
				if (SF_DEBUG) mprint(yyengine, NULL, siminfo, 
					"Setting yyengine->batts[%d].Inominal to %f\n",
					yyengine->curbatt, $2);
				yyengine->batts[yyengine->curbatt].Inominal = $2;
			}
		}
		| T_BATTRF dimm '\n'
		{
			if (!yyengine->scanning)
			{
				if (SF_DEBUG) mprint(yyengine, NULL, siminfo, 
						"Setting yyengine->batts[%d].Rf to %f\n",
						yyengine->curbatt, $2);
				yyengine->batts[yyengine->curbatt].Rf = $2;
			}
		}
		| T_BATTETALUT uimm dimm '\n'
		{
			if (!yyengine->scanning)
			{
				if ($2 < yyengine->batts[yyengine->curbatt].etaLUTnentries)
				{
					yyengine->batts[yyengine->curbatt].etaLUT[$2] = $3;
				}
				else
				{
					merror(yyengine, "Invalid etaLUT index.");
				}
			}
		}
		| T_BATTETALUTNENTRIES uimm '\n'
		{
			if (!yyengine->scanning)
			{
				double *tmp = (double *)mrealloc(yyengine, yyengine->batts[yyengine->curbatt].etaLUT,
						$2*sizeof(double), 
						"shasm.y, (double *) for T_BATTETALUTNENTRIES");
				if (tmp == NULL)
				{
					mexit(yyengine, "Could not resize etaLUT: mcrealloc failed.", -1);
				}
				else
				{
					yyengine->batts[yyengine->curbatt].etaLUT = tmp;
					yyengine->batts[yyengine->curbatt].etaLUTnentries = $2;
				}
			}
		}
		| T_BATTVBATTLUT uimm dimm '\n'
		{
			if (!yyengine->scanning)
			{
				if ($2 < yyengine->batts[yyengine->curbatt].VbattLUTnentries)
				{
					yyengine->batts[yyengine->curbatt].VbattLUT[$2] = $3;
				}
				else
				{
					merror(yyengine, "Invalid VbattLUT index.");
				}
			}
		}
		| T_BATTVBATTLUTNENTRIES uimm '\n'
		{
			if (!yyengine->scanning)
			{
				double *tmp = (double *)mrealloc(yyengine, yyengine->batts[yyengine->curbatt].VbattLUT,
						$2*sizeof(double), 
						"shasm.y, (double *) for T_BATTVBATTLUTNENTRIES");
				if (tmp == NULL)
				{
					mexit(yyengine, "Could not resize VbattLUT: mrealloc() failed", -1);
				}
				else
				{
					yyengine->batts[yyengine->curbatt].VbattLUT = tmp;
					yyengine->batts[yyengine->curbatt].VbattLUTnentries = $2;
				}
			}
		}
		| T_BATTVLOSTLUT uimm dimm '\n'
		{
			if (!yyengine->scanning)
			{
				if ($2 < yyengine->batts[yyengine->curbatt].VlostLUTnentries)
				{
					yyengine->batts[yyengine->curbatt].VlostLUT[$2] = $3;
				}
				else
				{
					merror(yyengine, "Invalid VlostLUT index.");
				}
			}
		}
		| T_BATTVLOSTLUTNENTRIES uimm '\n'
		{
			if (!yyengine->scanning)
			{
				double *tmp = (double *)mrealloc(yyengine, yyengine->batts[yyengine->curbatt].VlostLUT,
						$2*sizeof(double), 
						"shasm.y, (double *) for T_BATTVLOSTLUTNENTRIES");
				if (tmp == NULL)
				{
					mexit(yyengine, "Could not resize Vlost: mrealloc failed.", -1);
				}
				else
				{
					yyengine->batts[yyengine->curbatt].VlostLUT = tmp;
					yyengine->batts[yyengine->curbatt].VlostLUTnentries = $2;
				}
			}
		}
		| T_SETBATT uimm '\n'
		{
			if (!yyengine->scanning)
			{
				if ($2 >= yyengine->nbatts)
				{
					merror(yyengine, "Battery ID out of range.");
				}
				else
				{
					yyengine->curbatt = $2;
				}
			}
		}
		| T_PCBT '\n'
		{
			if (!yyengine->scanning)
			{
				m_pcbacktrace(yyengine, yyengine->cp);
			}
		}
		| T_BPT T_GLOBALTIME uimm '\n'
		{
			if (!yyengine->scanning)
			{
				m_setbptglobaltime(yyengine, $3);
			}
		}
		| T_BPT T_CYCLES uimm '\n'
		{
			if (!yyengine->scanning)
			{
				m_setbptcycles(yyengine, yyengine->cp, $3);
			}
		}
		| T_BPT T_INSTRS uimm '\n'
		{
			if (!yyengine->scanning)
			{
				m_setbptinstrs(yyengine, yyengine->cp, $3);
			}
		}
		| T_BPT T_SENSORREADING uimm dimm '\n'
		{
			if (!yyengine->scanning)
			{
				m_setbptsensorreading(yyengine, yyengine->cp, $3, $4);
			}
		}
		| T_BPTLS '\n'
		{
			if (!yyengine->scanning)
			{
				m_bptls(yyengine);
			}
		}
		| T_BPTDEL uimm '\n'
		{
			if (!yyengine->scanning)
			{
				m_bptdel(yyengine, $2);
			}
		}
		| T_SETLOC dimm dimm dimm '\n'
		{
			if (!yyengine->scanning)
			{
				m_setloc(yyengine, yyengine->cp, $2, $3, $4);
			}
		}
		| T_RETRYALG uimm T_STRING '\n'
		{
			if (!yyengine->scanning)
			{
				network_setretryalg(yyengine, yyengine->cp, $2, $3);
			}
		}
		| T_RANDPRINT T_STRING dimm dimm dimm dimm dimm dimm '\n'
		{
			if (!yyengine->scanning)
			{
				m_randprint(yyengine, $2, $3, $4, $5, $6, $7, $8);
			}
		}
		| T_REGISTERRVAR T_STRING uimm T_STRING dimm dimm dimm dimm T_STRING dimm dimm dimm dimm T_STRING '\n'
		{
			if (!yyengine->scanning)
			{
				//m_registerrvar(yyengine->cp, $2, $3, $4, $5, $6, $7,
				//		$8, $9, $10, $11, $12, $13, $14);
			}
		}
		| T_INITRANDTABLE T_STRING T_STRING dimm dimm dimm dimm dimm dimm dimm '\n'
		{
			if (!yyengine->scanning)
			{
				m_initrandtable(yyengine, $2, $3, $4, $5, $6, $7, $8, $9, $10);
			}
		}
		| T_DEFNDIST T_STRING '{' dimmlist ',' dimmlist '}'
		{
			if (!yyengine->scanning)
			{
				//m_defndist($2, $4, $6);

			}
		}
		| T_HWSEEREG T_STRING uimm uimm uimm '\n'
		{
			if (!yyengine->scanning)
			{
				if (yyengine->cp->machinetype == MACHINE_SUPERH)
				{
					//superHhwSEEreg($2, $3, $4, $5);
				}
				else if (yyengine->cp->machinetype == MACHINE_MSP430)
				{
					//msp430hwSEEreg($2, $3, $4, $5);
				}
				else
				{
					sfatal(yyengine, yyengine->cp, "Unknown machine type");
				}
			}
		}
		| T_INITSEESTATE T_STRING dimm dimm dimm dimm T_STRING dimm dimm dimm dimm T_STRING dimm dimm dimm dimm T_STRING dimm dimm dimm dimm
		{
			if (!yyengine->scanning)
			{
				if (yyengine->cp->machinetype == MACHINE_SUPERH)
				{
					//superHinitSEEstate(yyengine->cp, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21);
				}
				else if (yyengine->cp->machinetype == MACHINE_MSP430)
				{
					//msp430initSEEstate(yyengine->cp, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21);
				}
				else
				{
					sfatal(yyengine, yyengine->cp, "Unknown machine type");
				}
			}
		}
		| T_IGNORENODEDEATHS uimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->ignoredeaths = $2;
			}
		}
		| T_SETSCALEALPHA dimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->voltscale_alpha = $2;
			}
		}
		| T_SETSCALEK dimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->voltscale_K = $2;
			}
		}
		| T_SETSCALEVT dimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->voltscale_Vt = $2;
			}
		}
		| T_SETSCHEDRANDOM '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->schedtype = SchedRandom;
			}
		}
		| T_SETSCHEDROUNDROBIN '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->schedtype = SchedRoundRobin;
			}
		}
		| T_SETQUANTUM uimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->quantum = $2;
			}
		}
		| T_SETBASENODEID uimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->baseid = $2;
			}
		}
		| T_RENUMBERNODES '\n'
		{
			if (!yyengine->scanning)
			{
				m_renumbernodes(yyengine);
			}
		}
		| T_RESETNODECTRS '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->trip_ustart = musercputimeusecs();
				yyengine->cp->trip_startclk = yyengine->cp->ICLK;
			}
		}
		| T_RESETALLCTRS '\n'
		{
			if (!yyengine->scanning)
			{
				int i;

				for (i = 0; i < yyengine->nnodes; i++)
				{
					yyengine->sp[i]->trip_ustart = musercputimeusecs();
					yyengine->sp[i]->trip_startclk = yyengine->sp[i]->ICLK;
				}
			}
		}
		| T_NETSEG2FILE uimm T_STRING '\n'
		{
			if (!yyengine->scanning)
			{
				network_netseg2file(yyengine, $2, $3);
			}
		}
		| T_FILE2NETSEG T_STRING uimm '\n'
		{
			if (!yyengine->scanning)
			{
				network_file2netseg(yyengine, $2, $3);
			}
		}
		| T_CD T_STRING '\n'
		{
			if (!yyengine->scanning)
			{
				int	n = mchdir($2);

				if (n < 0)
				{
					merror(yyengine, "Could not change directory to \"%s\".", $2);
				}
			}
		}
		| T_SETTIMERDELAY dimm
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->settimerintrdelay(yyengine, yyengine->cp, $2);
			}
		}
		| T_SETPHYSICSPERIOD dimm
		{
			if (!yyengine->scanning)
			{
				yyengine->phyperiodpsec = $2;
			}
		}
		| T_SETBATTFEEDPERIOD dimm
		{
			if (!yyengine->scanning)
			{
				yyengine->battperiodpsec = $2;
			}
		}
		| T_SETNETPERIOD dimm
		{
			if (!yyengine->scanning)
			{
				yyengine->netperiodpsec = $2;
			}
		}
		| T_SETFAULTPERIOD dimm
		{
			if (!yyengine->scanning)
			{
				yyengine->fperiodpsec = $2;
			}
		}

		| T_FORCEAVGPWR dimm dimm
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->force_avgpwr = $2;
				yyengine->cp->force_sleeppwr = $3;
			}
		}
		| T_NETSEGPROPMODEL uimm uimm dimm
		{
			if (!yyengine->scanning)
			{
				network_netsegpropmodel(yyengine, $2, $3, $4);
			}
		}
		| T_SETDUMPPWRPERIOD dimm
		{
			if (!yyengine->scanning)
			{
				yyengine->dumpperiodpsec = $2;
			}
		}
		| T_VERSION
		{
			if (!yyengine->scanning)
			{
				m_version(yyengine);
			}
		}
		| T_SENSORSDEBUG
		{
			if (!yyengine->scanning)
			{
				physics_sensorsdbg(yyengine);
			}
		}

		| T_SIGNALSUBSCRIBE uimm uimm
		{
			if (!yyengine->scanning)
			{
				physics_sigsubscr(yyengine, yyengine->cp, $2, $3);
			}
		}
		| T_SIGNALSRC uimm optstring 
			dimm dimm 	dimm dimm dimm dimm dimm dimm dimm dimm dimm 	
			dimm 	dimm dimm dimm dimm dimm dimm dimm dimm 	
			optstring uimm dimm dimm dimm uimm		
			optstring uimm dimm uimm '\n'
		{
			if (!yyengine->scanning)
			{
				physics_newsigsrc(yyengine, $2, $3, $4, $5, $6, $7, $8, $9, $10,
					$11, $12, $13, $14, $15, $16, $17, $18, $19, $20,
					$21, $22, $23, $24, $25, $26, $27, $28, $29, $30,
					$31, $32, $33);
			}
		}
		| T_PWD '\n'
		{
			if (!yyengine->scanning)
			{
				mprint(yyengine, NULL, siminfo,
					"Current directory: %s\n", mgetpwd());
			}
		}
		| T_PARSEOBJDUMP T_STRING '\n'
		{
			if (!yyengine->scanning)
			{
				m_parseobjdump(yyengine, yyengine->cp, $2);
			}
		}
		| T_DUMPALL T_STRING T_STRING T_STRING '\n'
		{
			if (!yyengine->scanning)
			{
				m_dumpall(yyengine, $2, M_OWRITE, $3, $4);
			}
		}
		| T_SETNODE uimm '\n'
		{
			if (!yyengine->scanning)
			{
				if ($2 >= yyengine->nnodes)
				{
					merror(yyengine, "Node ID out of range.");
				}
				else
				{
					m_setnode(yyengine, $2);
				}
			}
		}
		| T_MMAP uimm uimm '\n'
		{
			if (!yyengine->scanning)
			{
				if (($2 > yyengine->nnodes) || ($3 > yyengine->nnodes))
				{
					merror(yyengine, "Node indeces out of range in call to MMAP!");
				}
				else
				{
					yyengine->sp[$3]->MEM  = yyengine->sp[$2]->MEM;
					mprint(yyengine, NULL, siminfo,
						"Mapped mem of Node " ULONGFMT " into Node " ULONGFMT "\n", $2, $3);
				}
			}
		}
		| T_MODECA '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->step = yyengine->cp->cyclestep;
			}
		}
		| T_MODEFF '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->step = yyengine->cp->faststep;
			}
		}
		| T_CACHEINIT uimm uimm uimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->cache_init(yyengine, yyengine->cp, $2, $3, $4);
			}
		}
		| T_CACHEOFF '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->cache_deactivate(yyengine, yyengine->cp);
			}
		}
		| T_CACHESTATS '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->cache_printstats(yyengine, yyengine->cp);
			}
		}
		| T_DUMPREGS '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->dumpregs(yyengine, yyengine->cp);
			}
		}
		| T_DUMPSYSREGS '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->dumpsysregs(yyengine, yyengine->cp);
			}
		}
		| T_DUMPPIPE '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->dumppipe(yyengine, yyengine->cp);
			}
		}
		| T_DUMPDISTRIBUTION '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->dumpdistribution(yyengine, yyengine->cp);
			}
		}
		| T_RESETCPU '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->resetcpu(yyengine, yyengine->cp);
			}
		}
		| T_DYNINSTR '\n'
		{
			if (!yyengine->scanning)
			{
				mprint(yyengine, yyengine->cp, nodeinfo,
					"Dynamic instruction count = [" UVLONGFMT "], fetched instructions = [" UVLONGFMT "]\n",
					yyengine->cp->dyncnt, yyengine->cp->nfetched);	
			}
		}
		| T_NODETACH uimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->nodetach = $2;
			}
		}
		| T_ADDVALUETRACE T_STRING uimm uimm uimm uimm uimm uimm '\n'
		{
			if (!yyengine->scanning)
			{
				m_addvaluetrace(yyengine, yyengine->cp, $2, $3, $4, $5, $6, $7, $8);
			}
		}
		| T_DELVALUETRACE T_STRING uimm uimm uimm uimm uimm uimm '\n'
		{
			if (!yyengine->scanning)
			{
				m_delvaluetrace(yyengine, yyengine->cp, $2, $3, $4, $5, $6, $7, $8);
			}
		}
		| T_REGISTERSTABS T_STRING '\n'
		{
			if (!yyengine->scanning)
			{
				m_readstabs(yyengine, yyengine->cp, $2);
			}
		}
		| T_VALUESTATS '\n'
		{
			if (!yyengine->scanning)
			{
				m_valuestats(yyengine, yyengine->cp);
			}
		}
		| T_NUMAREGION T_STRING uimm uimm simm simm simm simm uimm uimm uimm '\n'
		{
			if (!yyengine->scanning)
			{
				m_numaregion(yyengine, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11);
			}
		}
		| T_NUMASTATS '\n'
		{
			if (!yyengine->scanning)
			{
				m_numastats(yyengine, yyengine->cp);
			}
		}
		| T_NUMASTATSALL '\n'
		{
			if (!yyengine->scanning)
			{
				m_numastatsall(yyengine);
			}
		}
		| T_NUMASETMAPID uimm uimm '\n'
		{
			if (!yyengine->scanning)
			{
				m_numasetmapid(yyengine, $2, $3);
			}
		}
		| T_DUMPTLB '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->dumptlb(yyengine, yyengine->cp);
			}
		}
		| T_RATIO '\n'
		{
			if (!yyengine->scanning)
			{
				if (yyengine->cp->ICLK > 0)
				{
					mprint(yyengine, yyengine->cp, nodeinfo,
						"Ratio of active/sleep cycles = %.6f\n",
						(float)yyengine->cp->CLK/(float)yyengine->cp->ICLK);
				}
				else
				{
					merror(yyengine, "We don't stand a chance.");
				}
			}
		}
		| T_GETRANDOMSEED '\n'
		{
			if (!yyengine->scanning)
			{
				mprint(yyengine, NULL, siminfo,
					"Simulation random seed = %ld\n", yyengine->randseed);
			}
		}
		| T_SETRANDOMSEED uimm '\n'
		{
			if (!yyengine->scanning)
			{
				mprint(yyengine, NULL, siminfo,
					"Resetting random number generation with seed [%ld]\n", $2);
				yyengine->randseed = mrandominit(yyengine, $2);
			}
		}
		| T_EBATTINTR uimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->ENABLE_BATT_LOW_INTR = $2;
			}
		}
		| T_BATTALERTFRAC dimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->battery_alert_frac = $2;
			}
		}
		| T_NODEFAILPROB dimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->fail_prob = $2;
			}
		}
		| T_NODEFAILDURMAX uimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->failure_duration_max = $2;
			}
		}
		| T_NETSEGFAILDURMAX uimm uimm '\n'
		{
			if (!yyengine->scanning)
			{
				if ($2 >= MAX_NETSEGMENTS)
				{
					merror(yyengine, "Segment # > max. number of network segments.");
				}
				else
				{
					yyengine->netsegs[$2].failure_duration_max = $3;
				}
			}
		}
		| T_NETNEWSEG uimm uimm uimm uimm uimm uimm dimm dimm dimm 
			uimm dimm dimm dimm '\n'
		{
			if (!yyengine->scanning)
			{
				network_netnewseg(yyengine, $2, $3, $4, $5, $6, $7,
					$8, $9, $10, $11, $12, $13, $14);
			}
		}
		| T_NETSEGFAILPROB uimm dimm '\n'
		{
			if (!yyengine->scanning)
			{
				if ($2 >= yyengine->nnetsegs)
				{
					merror(yyengine, "NETSEGMENT out of range.");
				}
				else
				{
					yyengine->netsegs[$2].fail_prob = $3;
				}
			}
		}
		| T_NETSEGNICATTACH uimm uimm '\n'
		{
			if (!yyengine->scanning)
			{
				network_netsegnicattach(yyengine, yyengine->cp, $2, $3);
			}
		}
		| T_NETCORREL uimm uimm dimm '\n'
		{
			if (!yyengine->scanning)
			{
				if ($2 >= MAX_NETSEGMENTS)
				{
					merror(yyengine, "Segment # > max. number of network segments.");
				}
				else if ($3 >= yyengine->nnodes)
				{
					merror(yyengine, "Invalid node number.");
				}
				else
				{
					yyengine->netsegs[$2].correl_coeffs[$3] = $4;
				}
			}
		}
		| T_NETNODENEWIFC uimm dimm dimm dimm dimm uimm dimm dimm dimm uimm uimm '\n'
		{
			if (!yyengine->scanning)
			{
				network_netnodenewifc(yyengine, yyengine->cp, $2, $3, $4, $5,
					$6, $7, $8, $9, $10, $11, $12);
			}
		}
		| T_NETSEGDELETE uimm '\n'
		{
			if (!yyengine->scanning)
			{
				network_netsegdelete(yyengine, $2);
			}
		}
		| T_NODEFAILPROBFN T_STRING '\n'
		{
			if (!yyengine->scanning)
			{
				merror(yyengine, "Command \"NODEFAILPROBFN\" unimplemented.");
			}
		}
		| T_NETSEGFAILPROBFN T_STRING '\n'
		{
			if (!yyengine->scanning)
			{
				merror(yyengine, "Command \"NETSEGFAILPROBFN\" unimplemented.");
			}
		}
		| T_SIZEMEM uimm '\n'
		{
			if (!yyengine->scanning)
			{
				m_sizemem(yyengine, yyengine->cp, $2);
			}
		}
		| T_SIZEPAU uimm '\n'
		{
			if (!yyengine->scanning)
			{
				pau_init(yyengine, yyengine->cp, $2);
			}
		}
		| T_SPLIT uimm  uimm uimm T_STRING '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->split(yyengine, yyengine->cp, $2, $3, $4, $5);
			}
		}
		| T_DUMPMEM uimm uimm '\n'
		{
			if (!yyengine->scanning)
			{
				m_dumpmem(yyengine, yyengine->cp, $2, $3);
			}
		}
		| T_DISABLEPIPELINE '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->pipelined = 0;
			}
		}
		| T_ENABLEPIPELINE '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->pipelined = 1;
			}
		}
		| T_CLOCKINTR uimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->superH->ENABLE_CLK_INTR = $2;
			}
		}
		| T_STOP '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->runnable = 0;
			}
		}
		| T_SFATAL T_STRING '\n'
		{
			if (!yyengine->scanning)
			{
				sfatal(yyengine, yyengine->cp, $2);
			}
		}
		| T_VERBOSE '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->verbose ^= 1;
			}
		}
		| T_RUN optstring '\n'
		{
			if (!yyengine->scanning)
			{
				m_run(yyengine, yyengine->cp, $2);
			}

			/*	The 'string' is dynamically allocated, in lex.c		*/
			free($2);
		}
		| T_STEP '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->step(yyengine, yyengine->cp, 0);
			}
		}
		| T_STEP uimm '\n'
		{
			if (!yyengine->scanning)
			{
				go(yyengine, yyengine->cp, $2);
			}
		}
		| T_LOAD T_STRING '\n'
		{
			if (!yyengine->scanning)
			{
				loadcmds(yyengine, $2);
			}
		}
		| T_SETPC uimm '\n'
		{	
			if (!yyengine->scanning)
			{
				yyengine->cp->PC = $2;
			}
		}
		| T_SETVDD dimm '\n'
		{
			if (!yyengine->scanning)
			{
				/*	Scale frequency accordingly for provided Vdd	*/
				power_scaledelay(yyengine, yyengine->cp, $2);
			}
		}
		| T_SETMEMRLATENCY uimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->mem_r_latency = $2;
			}
		}
		| T_SETMEMWLATENCY uimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->mem_w_latency = $2;
			}
		}
		| T_SETFLASHRLATENCY uimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->flash_r_latency = $2;
			}
		}
		| T_SETFLASHWLATENCY uimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->flash_w_latency = $2;
			}
		}
		| T_SETFREQ dimm '\n'
		{
			if (!yyengine->scanning)
			{
				/*	Scale Vdd accordingly for provided frequency	*/
				power_scalevdd(yyengine, yyengine->cp, $2);
			}
		}
		| T_SETNODEMASS dimm '\n'
		{
			if (!yyengine->scanning)
			{
				/*	Set node mass	*/
				massSetNodeMass(yyengine, yyengine->cp, $2);
			}
		}
		| T_SETPROPULSIONCOEFFS		dimm dimm dimm dimm dimm dimm		dimm dimm dimm dimm dimm dimm		dimm dimm dimm dimm dimm dimm
		{
			if (!yyengine->scanning)
			{
				/*	Scale Vdd accordingly for provided frequency	*/
				propulsionSetPropulsionCoeffs(yyengine, yyengine->cp,
											$2,	$3,	$4,	$5,	$6,	$7,
											$8,	$9,	$10,	$11,	$12,	$13,
											$14,	$15,	$16,	$17,	$18,	$19);
			}
		}
		| T_SETMEMBASE uimm
		{
			yyengine->cp->MEMBASE = $2;
		}
		| T_SHOWMEMBASE
		{
			mprint(yyengine, NULL, siminfo, "Memory base address is %d.",yyengine->cp->MEMBASE);
		}
		| T_HELP '\n'
		{
			if (!yyengine->scanning)
			{
				help(yyengine);
			}
		}
		| T_MAN '\n'
		{
			if (!yyengine->scanning)
			{
				man(yyengine, $1);
			}
		}
		| T_PIPEFLUSH '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->flushpipe(yyengine->cp);
			}
		}
		| T_SHOWPIPE '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->pipeshow = !yyengine->cp->pipeshow;
			}
		}
		| T_SAVE uimm uimm T_STRING '\n'
		{
			if (!yyengine->scanning)
			{
				savemem(yyengine, yyengine->cp, $2, $3, $4);
			}
		}
		| T_SRECL T_STRING '\n'
		{
			if (!yyengine->scanning)
			{
				load_srec(yyengine, yyengine->cp, $2);
			}
		}
		| T_LOADMAPFILE T_STRING '\n'
		{
			if (!yyengine->scanning)
			{
				load_mapfile(yyengine, yyengine->cp, $2);
			}
		}
		| T_CONT uimm '\n'
		{
			if (!yyengine->scanning)
			{
				cont(yyengine, yyengine->cp, $2);
			}
		}
		| T_SHAREBUS uimm '\n'
		{
			if (!yyengine->scanning)
			{
				m_sharebus(yyengine, yyengine->cp, $2);
			}
		}
		| T_SHOWCLK '\n'
		{
			if (!yyengine->scanning)
			{
				mprint(yyengine, NULL, siminfo,
					"CLK = " UVLONGFMT ", ICLK = " UVLONGFMT ", TIME = %E, CYCLETIME = %E\n",
					yyengine->cp->CLK, yyengine->cp->ICLK, yyengine->cp->TIME, 
					yyengine->cp->CYCLETIME);
			}
		}
		| T_POWERSTATS '\n'
		{
			if (!yyengine->scanning)
			{
				power_printstats(yyengine, yyengine->cp);
			}
		}
		| T_LOCSTATS '\n'
		{
			if (!yyengine->scanning)
			{
				m_locstats(yyengine, yyengine->cp);
			}
		}
		| T_LISTRVARS '\n'
		{
			if (!yyengine->scanning)
			{
				m_listrvars(yyengine);
			}
		}

		| T_ON '\n'
		{
			if (!yyengine->scanning)
			{
				m_on(yyengine, yyengine->cp);
			}
		}
		| T_OFF '\n'
		{
			if (!yyengine->scanning)
			{
				m_off(yyengine, yyengine->cp);
			}
		}
		| T_NETDEBUG '\n'
		{
			if (!yyengine->scanning)
			{
				network_netdebug(yyengine, yyengine->cp);
			}
		}
		| T_TRACE uimm '\n'
		{
			if (!yyengine->scanning)
			{
				mprint(yyengine, NULL, siminfo, 
					"WARNING: Trace only works if compiled w/ -MYRMIGKI_DEBUG\n\n");
				yyengine->cp->trace = $2;
			}
		}
		| T_THROTTLE uimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->throttlensec = $2;
			}
		}
		| T_THROTTLEWIN uimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->throttlewin = $2;
			}
		}
		| T_FLTTHRESH uimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->faultthreshold = $2;
			}
		}
		| T_EFAULTS uimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->ENABLE_TOO_MANY_FAULTS = $2;
			}
		}
		| T_PFUN T_STRING '\n'
		{
			if (!yyengine->scanning)
			{
				fault_setnodepfun(yyengine, yyengine->cp, $2);
			}
		}
		| T_SETIFCOUI uimm T_STRING '\n'
		{
			if (!yyengine->scanning)
			{
				if ($2 < yyengine->cp->superH->NIC_NUM_IFCS)
				{
					msnprint((char*)&yyengine->cp->superH->NIC_IFCS[$2].IFC_OUI[0],
						NIC_ADDR_LEN, "%s", $3);
				}
				else
				{
					merror(yyengine, "Network interface [%d] out of range.", $2);
				}
			}
		}
		| T_MALLOCDEBUG '\n'
		{
			if (!yyengine->scanning)
			{
				mmblocksdisplay(yyengine);
			}
		}
		| T_PAUINFO '\n'
		{
			if (!yyengine->scanning)
			{
				pau_printstats(yyengine, yyengine->cp);
			}
		}
		| T_NANOPAUSE uimm '\n'
		{
			if (!yyengine->scanning)
			{
				// TODO: we should account for the cost of the m_on and m_off

				m_off(yyengine, yyengine->cp);
				mnsleep($2);
				m_on(yyengine, yyengine->cp);
			}
		}
		| T_PAUSE uimm '\n'
		{
			if (!yyengine->scanning)
			{
				// TODO: we should account for the cost of the m_on and m_off

				m_off(yyengine, yyengine->cp);
				mnsleep($2 * 1000000000);
				m_on(yyengine, yyengine->cp);
			}
		}
		| T_COMMENT '\n'
		{
			if (!yyengine->scanning)
			{
				/*	For now, nothing fun is done with comments	*/
			}
		}
		| '\n'
		{
			if (!yyengine->scanning)
			{
				/*								*/
				/*	The case where the command is just a solitary newline 	*/
				/*	is actually short circuited and does not even get here 	*/
				/*	We only parse input if # chars read from input is > 1.	*/
				/*								*/
			}
		}
		;


dotalign	: T_DOTALIGN uimm
		{
			/*							*/
			/*	Whether yyengine->scanning or not, forcefully align PC 	*/
			/*							*/
			ulong align = $2;

			if (align == 2)
			{
				mprint(yyengine, NULL, siminfo, "adjusting PC from " ULONGFMT " to " ULONGFMT "\n",
					yyengine->cp->PC, (yyengine->cp->PC+16-(yyengine->cp->PC&0xF)));
				yyengine->cp->PC += 16 - (yyengine->cp->PC & 0xF);
			}
			else if (align == 4)
			{
				mprint(yyengine, NULL, siminfo, "adjusting PC from " ULONGFMT " to " ULONGFMT "\n",
					yyengine->cp->PC, (yyengine->cp->PC+16-(yyengine->cp->PC&0xF)));
				yyengine->cp->PC += 256 - (yyengine->cp->PC & 0xFF);
			}
			else
			{
				merror(yyengine, ".align for arbitrary alignment not implemented !!!");
			}
		}
		;


dotdata		: T_DOTDATA
		{
		}
		;


dottext		: T_DOTTEXT
		{
		}
		;


dotglobal	: T_DOTGLOBAL disp
		{
		}
		;


dotfile		: T_DOTFILE T_STRING
		{
		}
		;

dotorg		: T_DOTORG uimm
		{
		}
		;

dotlong		: T_DOTLONG disp
		{
			/*								*/
			/*	disp is _relative_ to the current pc, so convert it	*/
			/*	to absolute address before laying down the value, by	*/
			/*	just doing the opposite of what we did in rule for	*/
			/*	'disp', at the end of this shasm.y			*/
			/*								*/
			long absdisp = ($2 << 1) + yyengine->cp->PC + 4;

			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else
			{
				mprint(yyengine, NULL, siminfo,
					"Laying down raw data, val = [" ULONGFMT "]\n", absdisp);
				memmove(&yyengine->cp->MEM[yyengine->cp->PC], &absdisp, sizeof(absdisp));

				/*	.long : 32 bits == 4 bytes	*/
				yyengine->cp->PC += 4;
			}
		}
		;
dotcomm		: T_DOTCOMM disp ',' T_STRING
		{
			/*						*/
			/*	My understanding is that the .comm is	*/
			/*	a global var definition. The address 	*/
			/*	in memory of the global var is the	*/
			/*	PC of the .comm defn.			*/
			/*						*/
			if (yyengine->scanning)
			{
				char tmp;
				char *ep = &tmp;

				yyengine->cp->PC += strtol($4, &ep, 0);
				
				if (*ep != '\0')
				{
					mprint(yyengine, NULL, siminfo, "invalid size for .comm variable");
				}	
			}
		}
		;

add_instr	: T_ADD reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_add(yyengine, yyengine->cp, $2, $4);
				}
		
				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1100;
				tmp.code_hi = B0011;

				if (yyengine->cp->PC - yyengine->cp->MEMBASE < 0 ||
					yyengine->cp->PC - yyengine->cp->MEMBASE > yyengine->cp->MEMSIZE - 1)
				{
					sfatal(yyengine, yyengine->cp, "Invalid PC address. Must be within alocated memory.");
				}
				else
				{
					memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
						&tmp, sizeof(tmp));
				}

				yyengine->cp->PC += 2;
			}
		}
		;

addi_instr	: T_ADD '#' simm ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_ni tmp;

				if (!yyengine->cp->pipelined)	
				{
					superH_addi(yyengine, yyengine->cp, $3, $5);
				}
		
				tmp.imm = ($3&B11111111);
				tmp.dst = ($5&B1111);
				tmp.code = B0111;

				if (yyengine->cp->PC - yyengine->cp->MEMBASE < 0 ||
					yyengine->cp->PC - yyengine->cp->MEMBASE > yyengine->cp->MEMSIZE - 1)
				{
					sfatal(yyengine, yyengine->cp, "Invalid PC address. Must be within allocated memory.");
				}
				else
				{
					memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
						&tmp, sizeof(tmp));
				}
				yyengine->cp->PC += 2;
				
			}
		}
		;

addc_instr	: T_ADDC reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_addc(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1110;
				tmp.code_hi = B0011;

				if (yyengine->cp->PC - yyengine->cp->MEMBASE < 0 ||
					yyengine->cp->PC - yyengine->cp->MEMBASE > yyengine->cp->MEMSIZE - 1)
				{
					sfatal(yyengine, yyengine->cp, "Invalid PC address. Must be within allocated memory.");
				}
				else
				{
					memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
						&tmp, sizeof(tmp));
				}
				yyengine->cp->PC += 2;
			}
		}
		;

addv_instr	: T_ADDV reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_addv(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1111;
				tmp.code_hi = B0011;
			
				if (yyengine->cp->PC - yyengine->cp->MEMBASE < 0 ||
					yyengine->cp->PC - yyengine->cp->MEMBASE > yyengine->cp->MEMSIZE - 1)
				{
					sfatal(yyengine, yyengine->cp, "Invalid PC address. Must be within allocated memory.");
				}
				else
				{
					memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
						&tmp, sizeof(tmp));
				}
				yyengine->cp->PC += 2;
			}
		}
		;

and_instr	: T_AND reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_and(yyengine, yyengine->cp, $2, $4);
				}
	
				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1001;
				tmp.code_hi = B0010;

				if (yyengine->cp->PC - yyengine->cp->MEMBASE < 0 ||
					yyengine->cp->PC - yyengine->cp->MEMBASE > yyengine->cp->MEMSIZE - 1)
				{
					sfatal(yyengine, yyengine->cp, "Invalid PC address. Must be within allocated memory.");
				}
				else
				{
					memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
						&tmp, sizeof(tmp));
				}
				yyengine->cp->PC += 2;
			}
		}
		;

andi_instr	: T_AND '#' simm ',' T_R0
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_andi(yyengine, yyengine->cp, $3);
				}
	
				tmp.imm = ($3&B11111111);
				tmp.code = B11001001;

				if (yyengine->cp->PC - yyengine->cp->MEMBASE < 0 ||
					yyengine->cp->PC - yyengine->cp->MEMBASE > yyengine->cp->MEMSIZE - 1)
				{
					sfatal(yyengine, yyengine->cp, "Invalid PC address. Must be within allocated memory.");
				}
				else
				{
					memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
						&tmp, sizeof(tmp));
				}
				yyengine->cp->PC += 2;
			}
		}
		;

andm_instr	: T_ANDB '#' simm ',' '@''(' T_R0 ',' T_GBR ')'
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_andm(yyengine, yyengine->cp, $3);
				}
			
				tmp.imm = ($3&B11111111);
				tmp.code = B11001101;

				if (yyengine->cp->PC - yyengine->cp->MEMBASE < 0 ||
					yyengine->cp->PC - yyengine->cp->MEMBASE > yyengine->cp->MEMSIZE - 1)
				{
					sfatal(yyengine, yyengine->cp, "Invalid PC address. Must be within allocated memory.");
				}
				else
				{
					memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
						&tmp, sizeof(tmp));
				}
				yyengine->cp->PC += 2;
			}
		}
		;

bf_instr	: T_BF disp
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_bf(yyengine, yyengine->cp, $2 - yyengine->cp->PC);
				}

				tmp.disp = ($2&B11111111);
				tmp.code = B10001011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

bfs_instr	: T_BFS disp
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_bfs(yyengine, yyengine->cp, $2 - yyengine->cp->PC);
				}
	
				tmp.disp = ($2&B11111111);
				tmp.code = B10001111;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

bra_instr	: T_BRA disp
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_d12 tmp;

				if (!yyengine->cp->pipelined)
				{
					/*						*/
					/*	The disp is absolute, and we have to	*/
					/*	pass PC relative to bra(). No need to	*/
					/*	PC-2 as in main.c, since no pipelining	*/
					/*						*/
					superH_bra(yyengine, yyengine->cp, $2 - yyengine->cp->PC);
				}

				tmp.disp = $2;
				tmp.code = B1010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

braf_instr	: T_BRA reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;
	
				if (!yyengine->cp->pipelined)
				{
					superH_braf(yyengine, yyengine->cp, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00100011;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

bsr_instr	: T_BSR disp
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d12 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_bsr(yyengine, yyengine->cp, $2 - yyengine->cp->PC);
				}

				tmp.disp = ($2&B111111111111);
				tmp.code = B1011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

bsrf_instr	: T_BSRF reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_bsrf(yyengine, yyengine->cp, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00000011;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

bt_instr	: T_BT disp
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_bt(yyengine, yyengine->cp, $2 - yyengine->cp->PC);
				}

				tmp.disp = ($2&B11111111);
				tmp.code = B10001001;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

bts_instr:	T_BTS disp
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_bts(yyengine, yyengine->cp, $2 - yyengine->cp->PC);
				}

				tmp.disp = ($2&B11111111);
				tmp.code = B10001101;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

clrmac_instr	: T_CLRMAC
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_clrmac(yyengine, yyengine->cp);
				}

				tmp.code = B0000000000101000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

clrs_instr	: T_CLRS
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_clrs(yyengine, yyengine->cp);
				}

				tmp.code = B0000000001001000;
			
				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

clrt_instr	: T_CLRT
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_clrt(yyengine, yyengine->cp);
				}

				tmp.code = B0000000000001000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

cmpeq_instr	: T_CMPEQ reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_cmpeq(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B0000;
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

cmpge_instr	: T_CMPGE reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_cmpge(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B0011;
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

cmpgt_instr	: T_CMPGT reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_cmpgt(yyengine, yyengine->cp, $2, $4);
				}
	
				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B0111;
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

cmphi_instr	: T_CMPHI reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_cmphi(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B0110; 
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

cmphs_instr	: T_CMPHS reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_cmphs(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B0010;
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

cmppl_instr	: T_CMPPL reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_cmppl(yyengine, yyengine->cp, $2);
				}
				tmp.dst = ($2&B111);
				tmp.code_lo = B00010101;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

cmppz_instr	: T_CMPPZ reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_cmppz(yyengine, yyengine->cp, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00010001;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

cmpstr_instr	: T_CMPSTR reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_cmpstr(yyengine, yyengine->cp, $2, $4);
				}
		
				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1100;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

cmpim_instr	: T_CMPEQ '#' simm ',' T_R0
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_cmpim(yyengine, yyengine->cp, $3);
				}

				tmp.imm = ($3&B11111111);
				tmp.code = B10001000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

div0s_instr	: T_DIV0S reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_div0s(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B0111;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

div1_instr	: T_DIV1 reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_div1(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B0100;
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

dmuls_instr	: T_DMULSL reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_dmuls(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1101;
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

dmulu_instr	: T_DMULUL reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_dmulu(yyengine, yyengine->cp, $2, $4);
				}
			
				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B0101;
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

dt_instr	: T_DT reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_dt(yyengine, yyengine->cp, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00010000;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

extsb_instr	: T_EXTSB reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_extsb(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1110;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

extsw_instr	: T_EXTSW reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_extsw(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1111;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

extub_instr	: T_EXTUB reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_extub(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1100;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

extuw_instr	: T_EXTUW reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_extuw(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1101;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

jmp_instr	: T_JMP '@' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_jmp(yyengine, yyengine->cp, $3);
				}

				tmp.dst = ($3&B1111);
				tmp.code_lo = B00101011;
				tmp.code_hi = B0100;


				if (yyengine->cp->PC - yyengine->cp->MEMBASE < 0 ||
					yyengine->cp->PC - yyengine->cp->MEMBASE > yyengine->cp->MEMSIZE - 1)
				{
					sfatal(yyengine, yyengine->cp, "Invalid PC address. Must be within alocated memory.");
				}
				else
				{
					memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
						&tmp, sizeof(tmp));
				}

				yyengine->cp->PC += 2;
			}
		}
		;

jsr_instr	: T_JSR '@' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_jsr(yyengine, yyengine->cp, $3);
				}

				tmp.dst = ($3&B1111);
				tmp.code_lo = B00001011;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

ldcsr_instr	: T_LDC reg ',' T_SR
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldcsr(yyengine, yyengine->cp, $2);
				}

				tmp.src = ($2&B1111);
				tmp.code_lo = B00001110;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

ldcgbr_instr	: T_LDC reg ',' T_GBR
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldcgbr(yyengine, yyengine->cp, $2);
				}

				tmp.src = ($2&B1111);
				tmp.code_lo = B00011110;
				tmp.code_hi = B0100;
			
				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

ldcvbr_instr	: T_LDC reg ',' T_VBR
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldcvbr(yyengine, yyengine->cp, $2);
				}

				tmp.src = ($2&B1111);
				tmp.code_lo = B00101110;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

ldcssr_instr	: T_LDC reg ',' T_SSR
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldcssr(yyengine, yyengine->cp, $2);
				}

				tmp.src = ($2&B1111);
				tmp.code_lo = B00111110;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

ldcspc_instr	: T_LDC reg ',' T_SPC
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldcspc(yyengine, yyengine->cp, $2);
				}

				tmp.src = ($2&B1111);
				tmp.code_lo = B01001110;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

ldcr_bank_instr	: T_LDC reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_mbank tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldcr_bank(yyengine, yyengine->cp, $4, $2);
				}
		
				tmp.code_lo = B1110;
				tmp.reg = ($4&B111);
				tmp.fill = 1;
				tmp.src = ($2&B1111);
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

ldcmsr_instr	: T_LDCL '@'reg'+'',' T_SR
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldcmsr(yyengine, yyengine->cp, $3);
				}

				tmp.src = ($3&B1111);
				tmp.code_lo = B00000111;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

ldcmgbr_instr	: T_LDCL '@'reg'+'',' T_GBR
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else
			{
				instr_m tmp;
	
				if (!yyengine->cp->pipelined)
				{
					superH_ldcmgbr(yyengine, yyengine->cp, $3);
				}

				tmp.src = ($3&B1111);
				tmp.code_lo = B00010111;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

ldcmvbr_instr	: T_LDCL '@'reg'+'',' T_VBR
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldcmvbr(yyengine, yyengine->cp, $3);
				}

				tmp.src = ($3&B1111);
				tmp.code_lo = B00100111;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

ldcmssr_instr	: T_LDCL '@'reg'+'',' T_SSR
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m  tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldcmssr(yyengine, yyengine->cp, $3);
				}

				tmp.src = ($3&B1111);
				tmp.code_lo = B00110111;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

ldcmspc_instr	: T_LDCL '@'reg'+'',' T_SPC
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldcmspc(yyengine, yyengine->cp, $3);
				}

				tmp.src = ($3&B1111);
				tmp.code_lo = B01000111;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

ldcmr_bank_instr: T_LDCL '@' reg '+' ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_mbank tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldcmr_bank(yyengine, yyengine->cp, $6, $3);
				}

				tmp.code_lo = B0111;
				tmp.reg = ($6&B111);
				tmp.fill = 1;
				tmp.src = ($3&B1111);
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

ldsmach_instr	: T_LDS reg ',' T_MACH
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldsmach(yyengine, yyengine->cp, $2);
				}

				tmp.src = ($2&B1111);
				tmp.code_lo = B00001010;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

ldsmacl_instr	: T_LDS reg ',' T_MACL
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldsmacl(yyengine, yyengine->cp, $2);
				}
	
				tmp.src = ($2&B1111);
				tmp.code_lo = B00011010;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

ldspr_instr	: T_LDS reg ',' T_PR
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldspr(yyengine, yyengine->cp, $2);
				}

				tmp.src = ($2&B1111);
				tmp.code_lo = B00101010;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

ldsmmach_instr	: T_LDSL '@' reg '+' ',' T_MACH
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldsmmach(yyengine, yyengine->cp, $3);
				}

				tmp.src = ($3&B1111);
				tmp.code_lo = B00000110;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

ldsmmacl_instr	: T_LDSL '@' reg '+' ',' T_MACL
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldsmmacl(yyengine, yyengine->cp, $3);
				}

				tmp.src = ($3&B1111);
				tmp.code_lo = B00010110;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

ldsmpr_instr	: T_LDSL '@' reg '+' ',' T_PR
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldsmpr(yyengine, yyengine->cp, $3);
				}

				tmp.src = ($3&B1111);
				tmp.code_lo = B00100110;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

ldtlb_instr	: T_LDTLB
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldtlb(yyengine, yyengine->cp);
				}

				tmp.code = B0000000000111000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

macl_instr	: T_MACL '@' reg '+' ',' '@' reg '+'
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_macl(yyengine, yyengine->cp, $3, $7);
				}

				tmp.src = ($3&B1111);
				tmp.dst = ($7&B1111);
				tmp.code_lo = B1111;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

macw_instr	: T_MACW '@' reg '+' ',' '@' reg '+'
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_macw(yyengine, yyengine->cp, $3, $7);
				}

				tmp.src = ($3&B1111);
				tmp.dst = ($7&B1111);
				tmp.code_lo = B1111;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

mov_instr	: T_MOV reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_mov(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B0011;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movbs_instr	: T_MOVB reg ',' '@' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movbs(yyengine, yyengine->cp, $2, $5);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($5&B1111);
				tmp.code_lo = B0000;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movws_instr	: T_MOVW reg ',' '@' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movws(yyengine, yyengine->cp, $2, $5);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($5&B1111);
				tmp.code_lo = B0001;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movls_instr	: T_MOVL reg ',' '@' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movls(yyengine, yyengine->cp, $2, $5);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($5&B1111);
				tmp.code_lo = B0010;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movbl_instr	: T_MOVB '@' reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movbl(yyengine, yyengine->cp, $3, $5);
				}

				tmp.src = ($3&B1111);
				tmp.dst = ($5&B1111);
				tmp.code_lo = B0000;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movwl_instr	: T_MOVW '@' reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movwl(yyengine, yyengine->cp, $3, $5);
				}

				tmp.src = ($3&B1111);
				tmp.dst = ($5&B1111);
				tmp.code_lo = B0001;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movll_instr	: T_MOVL '@' reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movll(yyengine, yyengine->cp, $3, $5);
				}

				tmp.src = ($3&B1111);
				tmp.dst = ($5&B1111);
					tmp.code_lo = B0010;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movbm_instr	: T_MOVB reg ',' '@' '-' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movbm(yyengine, yyengine->cp, $2, $6);
				}
	
				tmp.src = ($2&B1111);
				tmp.dst = ($6&B1111);
				tmp.code_lo = B0100;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movwm_instr	: T_MOVW reg ',' '@' '-' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movwm(yyengine, yyengine->cp, $2, $6);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($6&B1111);
				tmp.code_lo = B0101;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movlm_instr	: T_MOVL reg ',' '@' '-' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movlm(yyengine, yyengine->cp, $2, $6);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($6&B1111);
				tmp.code_lo = B0110;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movbp_instr	: T_MOVB '@' reg '+' ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movbp(yyengine, yyengine->cp, $3, $6);
				}

				tmp.src = ($3&B1111);
				tmp.dst = ($6&B1111);
				tmp.code_lo = B0100;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movwp_instr	: T_MOVW '@' reg '+' ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movwp(yyengine, yyengine->cp, $3, $6);
				}

				tmp.src = ($3&B1111);
				tmp.dst = ($6&B1111);
				tmp.code_lo = B0101;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movlp_instr	: T_MOVL '@' reg '+' ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movlp(yyengine, yyengine->cp, $3, $6);
				}

				tmp.src = ($3&B1111);
				tmp.dst = ($6&B1111);
				tmp.code_lo = B0110;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movbs0_instr	: T_MOVB reg ',' '@' '(' T_R0 ',' reg ')'
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movbs0(yyengine, yyengine->cp, $2, $8);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($8&B1111);
				tmp.code_lo = B0100;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movws0_instr	: T_MOVW reg ',' '@' '(' T_R0 ',' reg ')'
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movws0(yyengine, yyengine->cp, $2, $8);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($8&B1111);
				tmp.code_lo = B0101;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movls0_instr	: T_MOVL reg ',' '@' '(' T_R0 ',' reg ')'
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movls0(yyengine, yyengine->cp, $2, $8);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($8&B1111);
				tmp.code_lo = B0110;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movbl0_instr	: T_MOVB '@' '(' T_R0 ',' reg ')' ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movbl0(yyengine, yyengine->cp, $6, $9);
				}
	
				tmp.src = ($6&B1111);
				tmp.dst = ($9&B1111);
				tmp.code_lo = B1100;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movwl0_instr	: T_MOVW '@' '(' T_R0 ',' reg ')' ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movwl0(yyengine, yyengine->cp, $6, $9);
				}

				tmp.src = ($6&B1111);
				tmp.dst = ($9&B1111);
				tmp.code_lo = B1101;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}

		}
		;

movll0_instr	: T_MOVL '@' '(' T_R0 ',' reg ')' ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movll0(yyengine, yyengine->cp, $6, $9);
				}

				tmp.src = ($6&B1111);
				tmp.dst = ($9&B1111);
				tmp.code_lo = B1110;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movi_instr	: T_MOV '#' simm ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_ni tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movi(yyengine, yyengine->cp, $3, $5);
				}

				tmp.imm = ($3&B11111111);
				tmp.dst = ($5&B1111);
				tmp.code = B1110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movwi_instr	: T_MOVW '@' '(' disp ',' T_PC ')' ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_nd8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movwi(yyengine, yyengine->cp, $4 - yyengine->cp->PC, $9);
				}

				tmp.dst = ($9&B11111111);
				tmp.disp = ($4&B11111111);
				tmp.code = B1001;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;


		/*	Original version from spec	*/
movli_instr	: T_MOVL '@' '(' disp ',' T_PC ')' ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_nd8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movli(yyengine, yyengine->cp, $4 - yyengine->cp->PC, $9);
				}

				tmp.dst = ($9&B11111111);
				tmp.disp = ($4&B11111111);
				tmp.code = B1101;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		/*	Added to handle "mov.l LABEL, reg" that gcc emits	*/
		| T_MOVL disp ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_nd8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movli(yyengine, yyengine->cp, $2 - yyengine->cp->PC, $4);
				}

				tmp.dst = ($4&B11111111);
				tmp.disp = ($2&B11111111);
				tmp.code = B1101;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movblg_instr	: T_MOVB '@' '(' disp ',' T_GBR ')' ',' T_R0
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movblg(yyengine, yyengine->cp, $4 - yyengine->cp->PC);
				}

				tmp.disp = ($4&B11111111);
				tmp.code = B11000100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movwlg_instr	: T_MOVW '@' '(' disp ',' T_GBR ')' ',' T_R0
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movwlg(yyengine, yyengine->cp, $4 - yyengine->cp->PC);
				}

				tmp.disp = ($4&B11111111);
				tmp.code = B11000101;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movllg_instr	: T_MOVL '@' '(' disp ',' T_GBR ')' ',' T_R0
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movllg(yyengine, yyengine->cp, $4 - yyengine->cp->PC);
				}

				tmp.disp = ($4&B11111111);
				tmp.code = B11000110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movbsg_instr	: T_MOVB T_R0 ',' '@' '(' disp ',' T_GBR ')'
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movbsg(yyengine, yyengine->cp, $6 - yyengine->cp->PC);
				}

				tmp.disp = ($6&B11111111);
				tmp.code = B11000000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movwsg_instr	: T_MOVW T_R0 ',' '@' '(' disp ',' T_GBR ')'
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movwsg(yyengine, yyengine->cp, $6 - yyengine->cp->PC);
				}

				tmp.disp = ($6&B11111111);
				tmp.code = B11000001;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movlsg_instr	: T_MOVL T_R0 ',' '@' '(' disp ',' T_GBR ')'
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movlsg(yyengine, yyengine->cp, $6 - yyengine->cp->PC);
				}

				tmp.disp = ($6&B11111111);
				tmp.code = B11000010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movbs4_instr	: T_MOVB T_R0 ',' '@' '(' disp ',' reg ')'
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_nd4 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movbs4(yyengine, yyengine->cp, $6 - yyengine->cp->PC, $8);
				}
	
				tmp.dst = ($8&B1111);
				tmp.disp = ($6&B1111);
				tmp.code = B10000000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movws4_instr	: T_MOVW T_R0 ',' '@' '(' disp ',' reg ')'
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_nd4 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movws4(yyengine, yyengine->cp, $6 - yyengine->cp->PC, $8);
				}

				tmp.dst = ($8&B1111);
				tmp.disp = ($6&B1111);
				tmp.code = B10000001;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movls4_instr	: T_MOVL reg ',' '@' '(' disp ',' reg ')'
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_nmd tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movls4(yyengine, yyengine->cp, $2, $6 - yyengine->cp->PC, $8);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($8&B1111);
				tmp.disp = ($6&B1111);
				tmp.code = B0001;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movbl4_instr	: T_MOVB '@' '(' disp ',' reg ')' ',' T_R0
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_md tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movbl4(yyengine, yyengine->cp, $4 - yyengine->cp->PC, $6);
				}

				tmp.src = ($6&B1111);
				tmp.disp = ($4&B1111);
				tmp.code = B10000100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movwl4_instr	: T_MOVW '@' '(' disp ',' reg ')' ',' T_R0
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_md tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movwl4(yyengine, yyengine->cp, $4 - yyengine->cp->PC, $6);
				}

				tmp.src = ($6&B1111);
				tmp.disp = ($4&B1111);
				tmp.code = B10000101;
	
				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movll4_instr	: T_MOVL '@' '(' disp ',' reg ')' ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_nmd tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movll4(yyengine, yyengine->cp, $6, $4 - yyengine->cp->PC, $9);
				}

				tmp.src = ($6&B1111);
				tmp.dst = ($9&B1111);
				tmp.disp = ($4&B1111);
				tmp.code = B0101;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

mova_instr	: T_MOVA '@' '(' disp ',' T_PC ')' ',' T_R0
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_mova(yyengine, yyengine->cp, $4 - yyengine->cp->PC);
				}

				tmp.disp = ($4&B11111111);
				tmp.code = B11000111;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

movt_instr	: T_MOVT reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movt(yyengine, yyengine->cp, $2);
				}
		
				tmp.dst = ($2&B1111);
				tmp.code_lo = B00101001;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

mull_instr	: T_MULL reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_mull(yyengine, yyengine->cp, $2, $4);
				}
		
				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B0111;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

muls_instr	: T_MULS reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_muls(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1111;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

mulsw_instr	: T_MULSW reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_muls(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1111;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

mulu_instr	: T_MULU reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_mulu(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1110;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

muluw_instr	: T_MULUW reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_mulu(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1110;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

neg_instr	: T_NEG reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_neg(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1011;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

negc_instr	: T_NEGC reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_negc(yyengine, yyengine->cp, $2, $4);
				}
			
				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1010;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

nop_instr	: T_NOP
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_nop(yyengine, yyengine->cp);
				}

				tmp.code = B0000000000001001;
			
				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

not_instr	: T_NOT reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_not(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B0111;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

or_instr	: T_OR reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_or(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1011;
				tmp.code_hi = B0010;
	
				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

ori_instr	: T_OR '#' simm ',' T_R0
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ori(yyengine, yyengine->cp, $3);
				}

				tmp.imm = ($3&B11111111);
				tmp.code = B11001011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

orm_instr	: T_ORB '#' simm ',' '@' '(' T_R0 ',' T_GBR ')'
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_orm(yyengine, yyengine->cp, $3);
				}

				tmp.imm = ($3&B11111111);
				tmp.code = B11001111;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

pref_instr	: T_PREF '@' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_pref(yyengine, yyengine->cp, $3);
				}

				tmp.dst = ($3&B1111);
				tmp.code_lo = B10000011;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

rfg_instr	: T_RFG '#' simm
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_rfg(yyengine, yyengine->cp, $3);
				}

				tmp.imm = ($3&B11111111);
				tmp.code = B11110000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;
rotcl_instr	: T_ROTCL reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_rotcl(yyengine, yyengine->cp, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00100100;
				tmp.code_hi =  B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

rotcr_instr	: T_ROTCR reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_rotcr(yyengine, yyengine->cp, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00100101;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

rotl_instr	: T_ROTL reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_rotl(yyengine, yyengine->cp, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00000100;
				tmp.code_hi = B0100;
	
				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

rotr_instr	: T_ROTR reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_rotr(yyengine, yyengine->cp, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00000101;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

rte_instr	: T_RTE
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_rte(yyengine, yyengine->cp);
				}

				tmp.code = B0000000000101011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

rts_instr	: T_RTS
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_rts(yyengine, yyengine->cp);
				}

				tmp.code = B0000000000001011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

sets_instr	: T_SETS
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_sets(yyengine, yyengine->cp);
				}

				tmp.code = B0000000001011000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

sett_instr	: T_SETT
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_sett(yyengine, yyengine->cp);
				}

				tmp.code = B0000000000011000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

shad_instr	: T_SHAD reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shad(yyengine, yyengine->cp, $2, $4);
				}
	
				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1100; 
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

shal_instr	: T_SHAL reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shal(yyengine, yyengine->cp, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00100000;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

shar_instr	: T_SHAR reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shar(yyengine, yyengine->cp, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00100001;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;	
			}
		}
		;

shld_instr	: T_SHLD reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shld(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1101;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

shll_instr	: T_SHLL reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shll(yyengine, yyengine->cp, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00000000;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

shll2_instr	: T_SHLL2 reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shll2(yyengine, yyengine->cp, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00001000;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

shll8_instr	: T_SHLL8 reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shll8(yyengine, yyengine->cp, $2);
				}
		
				tmp.dst = ($2&B1111);
				tmp.code_lo = B00011000;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

shll16_instr	: T_SHLL16 reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shll16(yyengine, yyengine->cp, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00101000;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

shlr_instr	: T_SHLR reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shlr(yyengine, yyengine->cp, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00000001;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

shlr2_instr	: T_SHLR2 reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shlr2(yyengine, yyengine->cp, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00001001;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

shlr8_instr	: T_SHLR8 reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shlr8(yyengine, yyengine->cp, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00011001;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

shlr16_instr	: T_SHLR16 reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shlr16(yyengine, yyengine->cp, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00101001;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

sleep_instr	: T_SLEEP
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_sleep(yyengine, yyengine->cp);
				}

				tmp.code = B0000000000011011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

stcsr_instr	: T_STC T_SR ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcsr(yyengine, yyengine->cp, $4);
				}

				tmp.dst = ($4&B1111);
				tmp.code_lo = B00000010;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

stcgbr_instr	: T_STC T_GBR ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcgbr(yyengine, yyengine->cp, $4);
				}
			
				tmp.dst = ($4&B1111);
				tmp.code_lo = B00010010;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

stcvbr_instr	: T_STC T_VBR ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcvbr(yyengine, yyengine->cp, $4);
				}

				tmp.dst = ($4&B1111);
				tmp.code_lo = B00100010;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

stcssr_instr	: T_STC T_SSR ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcssr(yyengine, yyengine->cp, $4);
				}

				tmp.dst = ($4&B1111);
				tmp.code_lo = B00110010;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

stcspc_instr	: T_STC T_SPC ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcspc(yyengine, yyengine->cp, $4);
				}

				tmp.dst = ($4&B1111);
				tmp.code_lo = B01000010;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

stcr_bank_instr	: T_STC reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nbank tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcr_bank(yyengine, yyengine->cp, $2, $4);
				}

				tmp.code_lo = B0010;
				tmp.reg = ($2&B111);
				tmp.fill = 1;
				tmp.dst = ($4&B1111);
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

stcmsr_instr	: T_STCL T_SR ',' '@' '-' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcmsr(yyengine, yyengine->cp, $6);
				}

				tmp.dst = ($6&B1111);
				tmp.code_lo = B00000011;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

stcmgbr_instr	: T_STCL T_GBR ',' '@' '-' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcmgbr(yyengine, yyengine->cp, $6);
				}

				tmp.dst = ($6&B1111);
				tmp.code_lo = B00010011;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

stcmvbr_instr	: T_STCL T_VBR ',' '@' '-' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcmvbr(yyengine, yyengine->cp, $6);
				}

				tmp.dst = ($6&B1111);
				tmp.code_lo = B00100011;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

stcmssr_instr	: T_STCL T_SSR ',' '@' '-' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcmssr(yyengine, yyengine->cp, $6);
				}

				tmp.dst = ($6&B1111);
				tmp.code_lo = B00110011;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

stcmspc_instr	: T_STCL T_SPC ',' '@' '-' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcmspc(yyengine, yyengine->cp, $6);
				}

				tmp.dst = ($6&B1111);
				tmp.code_lo = B01000011;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

stcmr_bank_instr: T_STCL reg ',' '@' '-' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nbank tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcmr_bank(yyengine, yyengine->cp, $2, $6);
				}

				tmp.code_lo = B0011;
				tmp.reg = ($2&B111);
				tmp.fill = 1;
				tmp.dst = ($6&B1111);
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

stsmach_instr	: T_STS T_MACH ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stsmach(yyengine, yyengine->cp, $4);
				}

				tmp.dst = ($4&B1111);
				tmp.code_lo = B00001010;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

stsmacl_instr	: T_STS T_MACL ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stsmacl(yyengine, yyengine->cp, $4);
				}

				tmp.dst = ($4&B1111);
				tmp.code_lo = B00011010;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

stspr_instr	: T_STS T_PR ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stspr(yyengine, yyengine->cp, $4);
				}

				tmp.dst = ($4&B1111);
				tmp.code_lo = B00101010;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

stsmmach_instr	: T_STSL T_MACH ',' '@' '-' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stsmmach(yyengine, yyengine->cp, $6);
				}

				tmp.dst = ($6&B1111);
				tmp.code_lo = B00000010;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

stsmmacl_instr	: T_STSL T_MACL ',' '@' '-' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stsmmacl(yyengine, yyengine->cp, $6);
				}

				tmp.dst = ($6&B1111);
				tmp.code_lo = B00010010;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

stsmpr_instr	: T_STSL T_PR ',' '@' '-' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stsmpr(yyengine, yyengine->cp, $6);
				}

				tmp.dst = ($6&B1111);
				tmp.code_lo = B00100010;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

sub_instr	: T_SUB reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_sub(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1000;
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

subc_instr	: T_SUBC reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_subc(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1010;
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

subv_instr	: T_SUBV reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_subv(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1011;
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

swapb_instr	: T_SWAPB reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_swapb(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1000;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

swapw_instr	: T_SWAPW reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_swapw(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1001;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

tas_instr	: T_TASB '@' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_tas(yyengine, yyengine->cp, $3);
				}

				tmp.dst = ($3&B1111);
				tmp.code_lo = B00011011;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

trapa_instr	: T_TRAPA '#' simm
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_trapa(yyengine, yyengine->cp, $3);
				}

				tmp.imm = ($3&B11111111);
				tmp.code = B11000011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

tst_instr	: T_TST reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_tst(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1000;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

tsti_instr	: T_TST '#' simm ',' T_R0
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_tsti(yyengine, yyengine->cp, $3);
				}

				tmp.imm = ($3&B11111111);
				tmp.code = B11001000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

tstb_instr	: T_TSTB '#' simm ',''@''('T_R0','T_GBR')'
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_tstm(yyengine, yyengine->cp, $3);
				}

				tmp.imm = ($3&B11111111);
				tmp.code = B11001100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

xor_instr	: T_XOR reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_xor(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1010;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

xori_instr	: T_XOR '#' simm ',' T_R0
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_xori(yyengine, yyengine->cp, $3);
				}

				tmp.imm = ($3&B11111111);
				tmp.code = B11001010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

xorm_instr	: T_XORB '#' simm ',''@''('T_R0','T_GBR')'
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_xorm(yyengine, yyengine->cp, $3);
				}

				tmp.imm = ($3&B11111111);
				tmp.code = B11001110;
	
				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

xtrct_instr	: T_XTRCT reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_xtrct(yyengine, yyengine->cp, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1101;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
		;

rnd		: rnd_const | rnd_var
		;
/*
rnd		: rnd_exp		| rnd_gauss		| rnd_bpareto
		| rnd_weibull		| rnd_xi2		| rnd_cauchy
		| rnd_chi		| rnd_beta		| rnd_extremeval
		| rnd_fdist		| rnd_fermidirac	| rnd_fisherz
		| rnd_gumbel		| rnd_gamma		| rnd_halfnormal
		| rnd_laplace		| rnd_logseries		| rnd_logistic
		| rnd_maxwell		| rnd_negbinomial	| rnd_pearsontype3
		| rnd_poisson		| rnd_rayleigh		| rnd_studentst
		| rnd_studentsz		| rnd_betaprime		| rnd_erlang
		| rnd_lognorm		| rnd_gibrat		| rnd_bathtub
		| rnd_uniform
		;
*/

rnd_const	: '<' "rnd" T_STRING dimm dimm dimm dimm '>'
		{
			if (!yyengine->scanning)
			{
				//$$->value = m_randgen(yyengine->cp, $3, $4, $5, $6, $7);
			}
		}
		;

rnd_var		: '{' "rnd" T_STRING dimm dimm dimm dimm '}'
		{
			if (!yyengine->scanning)
			{
				//$$->value = m_randgen(yyengine->cp, $3, $4, $5, $6, $7);
				//$$->rv.pfunid = m_pfunname2id(yyengine->cp, $3);
				//$$->rv.disttabid = m_disttabname2id(yyengine->cp, $3);
				$$->rv.p1 = $4;
				$$->rv.p2 = $5;
				$$->rv.p3 = $6;
				$$->rv.p4 = $7;
			}
		}
		;

/*
rnd_exp		: '<' "rnd" "exp" dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$->dval = m_pfun_exp(yyengine->cp, $4, 0, 0, 0);
			}
		}
		| '{' "rnd" "exp" dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$->dval = m_pfun_exp(yyengine->cp, $4, 0, 0, 0);
				$$->rv.pfun = m_pfun_exp;
				$$->rv.p1 = $4;
			}
		}
		;

rnd_gauss	: '<' "rnd" "gauss" dimm dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfun_gauss($4,$5);
			}
		}
		| '{' "rand" "gauss" dimm dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfun_gauss($4, $5);
				$$.rv.pfun = m_pfun_gauss;
				$$.rv.p1 = $4;
				$$.rv.p2 = $5;
			}
		}
		;

rnd_bathtub	: '<' "rand" "bathtub" dimm dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfun_bathtub($4, $5);
			}
		}
		| '{' "rand" "bathtub" dimm dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfun_bathtub($4, $5);
				$$.rv.pfun = m_pfun_bathtub;
				$$.rv.p1 = $4;
				$$.rv.p2 = $5;
			}
		}
		;

rnd_beta	: '<' "rand" "beta" dimm dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfun_beta($4, $5);
			}
		}
		| '{' "rand" "beta" dimm dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfun_beta($4, $5);
				$$.rv.pfun = m_pfun_beta;
				$$.rv.p1 = $4;
				$$.rv.p2 = $5;
			}
		}
		;

rnd_betaprime	: '<' "rand" "betaprime" dimm dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunbetaprime($4, $5);
			}
		}
		| '{' "rand" "betaprime" dimm dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunbetaprime($4, $5);
				$$.rv.pfun = m_pfunbetaprime;
				$$.rv.p1 = $4;
				$$.rv.p2 = $5;
			}
		}
		;

rnd_bpareto	: '<' "rand" "bpareto" dimm dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunbpareto($4, $5);
			}
		}
		| '{' "rand" "bpareto" dimm dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunbpareto($4, $5);
				$$.rv.pfun = m_pfunbpareto;
				$$.rv.p1 = $4;
				$$.rv.p2 = $5;
			}
		}
		;

rnd_cauchy	: '<' "rand" "cauchy" dimm dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfuncauchy($4, $5);
			}
		}
		| '{' "rand" "cauchy" dimm dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfuncauchy($4, $5);
				$$.rv.pfun = m_pfuncauchy;
				$$.rv.p1 = $4;
				$$.rv.p2 = $5;
			}
		}
		;

rnd_chi		: '<' "rand" "chi" dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunchi($4);
			}
		}
		| '{' "rand" "chi" dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunchi($4);
				$$.rv.pfun = m_pfunchi;
				$$.rv.p1 = $4;
			}
		}
		;

rnd_erlang	: '<' "rand" "erlang" dimm dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunerlang($4, $5);
			}
		}
		| '{' "rand" "erlang" dimm dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunerlang($4, $5);
				$$.rv.pfun = m_pfunerlang;
				$$.rv.p1 = $4;
				$$.rv.p2 = $5;
			}
		}
		;

rnd_extremeval	: '<' "rand" "extremeval" dimm dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunextremeval($4, $5);
			}
		}
		| '{' "rand" "extremeval" dimm dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunextremeval($4, $5);
				$$.rv.pfun = m_pfunextremeval;
				$$.rv.p1 = $4;
				$$.rv.p2 = $5;
			}
		}
		;

rnd_fdist	: '<' "rand" "f" dimm dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunfdist($4, $5);
			}
		}
		| '{' "rand" "f" dimm dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunfdist($4, $5);
				$$.rv.pfun = m_pfunfdist;
				$$.rv.p1 = $4;
				$$.rv.p2 = $5;
			}
		}
		;

rnd_fermidirac	: '<' "rand" "fermidirac" dimm dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunfermidirac($4, $5);
			}
		}
		| '{' "rand" "fermidirac" dimm dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunfermidirac($4, $5);
				$$.rv.pfun = m_pfunfermidirac;
				$$.rv.p1 = $4;
				$$.rv.p2 = $5;
			}
		}
		;

rnd_fisherz	: '<' "rand" "fisherz" dimm dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunfisherz($4, $5);
			}
		}
		| '{' "rand" "fisherz" dimm dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunfisherz($4, $5);
				$$.rv.pfun = m_pfunfisherz;
				$$.rv.p1 = $4;
				$$.rv.p2 = $5;
			}
		}
		;

rnd_gamma	: '<' "rand" "gamma" dimm dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfungamma($4, $5);
			}
		}
		| '{' "rand" "gamma" dimm dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfungamma($4, $5);
				$$.rv.pfun = m_pfungamma;
				$$.rv.p1 = $4;
				$$.rv.p2 = $5;
			}
		}
		;

rnd_gibrat	: '<' "rand" "gibrat" '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfungibrat();
			}
		}
		| '{' "rand" "gibrat" '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfungibrat();
				$$.rv.pfun = m_pfungibrat;
			}
		}
		;

rnd_gumbel	: '<' "rand" "gumbel" dimm dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfungumbel($4, $5);
			}
		}
		| '{' "rand" "gumbel" dimm dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfungumbel($4, $5);
				$$.rv.pfun = m_pfungumbel;
				$$.rv.p1 = $4;
				$$.rv.p2 = $5;
			}
		}
		;

rnd_halfnormal	: '<' "rand" "halfnormal" "," "t" "=" rdimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunhalfnormal($7);
			}
		}
		| '{' "rand" "halfnormal" "," "t" "="  rdimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunhalfnormal($7);
				$$.rv.pfun = m_pfungumbel;
				$$.rv.p1 = $7;
			}
		}
		;

rnd_laplace	: '<' "rand" "laplace" dimm dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunlaplace($4, $5);
			}
		}
		| '{' "rand" "laplace" dimm dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunlaplace($4, $5);
				$$.rv.pfun = m_pfunlaplace;
				$$.rv.p1 = $4;
				$$.rv.p2 = $5;
			}
		}
		;

rnd_logistic	: '<' "rand" "logistic" dimm dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunlogistic($4, $5);
			}
		}
		| '{' "rand" "logistic" dimm dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunlogistic($4, $5);
				$$.rv.pfun = m_pfunlogistic;
				$$.rv.p1 = $4;
				$$.rv.p2 = $5;
			}
		}
		;

rnd_lognorm	: '<' "rand" "lognormal" dimm dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunlognormal($4, $5);
			}
		}
		| '{' "rand" "lognormal" dimm dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunlognormal($4, $5);
				$$.rv.pfun = m_pfunlognormal;
				$$.rv.p1 = $4;
				$$.rv.p2 = $5;
			}
		}
		;

rnd_logseries	: '<' "rand" "logseries" dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunlogseries($4);
			}
		}
		| '{' "rand" "logseries" dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunlogseries($4);
				$$.rv.pfun = m_pfunlogseries;
				$$.rv.p1 = $4;
			}
		}
		;

rnd_maxwell	: '<' "rand" "maxwell" dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunmaxwell($4);
			}
		}
		| '{' "rand" "maxwell" dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunmaxwell($4);
				$$.rv.pfun = m_pfunmaxwell;
				$$.rv.p1 = $4;
			}
		}
		;

rnd_negbinomial	: '<' "rand" "negbinomial" dimm dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunnegbinomial($4, $5);
			}
		}
		| '{' "rand" "negbinomial" dimm dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunnegbinomial($4, $5);
				$$.rv.pfun = m_pfunnegbinomial;
				$$.rv.p1 = $4;
				$$.rv.p2 = $5;
			}
		}
		;

rnd_pearsontype3: '<' "rand" "pearsontype3" dimm dimm dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunpearsontype3($4, $5, $6);
			}
		}
		| '{' "rand" "negbinomial" dimm dimm dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunnegbinomial($4, $5);
				$$.rv.pfun = m_pfunnegbinomial;
				$$.rv.p1 = $4;
				$$.rv.p2 = $5;
			}
		}
		;

rnd_poisson	: '<' "rand" "poisson" dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunpoisson($4);
			}
		}
		| '{' "rand" "poisson" dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunpoisson($4);
				$$.rv.pfun = m_pfunpoisson;
				$$.rv.p1 = $4;
			}
		}
		;

rnd_rayleigh	: '<' "rand" "rayleigh" dimm dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunrayleigh($4, $5);
			}
		}
		| '{' "rand" "rayleigh" dimm dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunrayleigh($4, $5);
				$$.rv.pfun = m_pfunrayleighl;
				$$.rv.p1 = $4;
				$$.rv.p2 = $5;
			}
		}
		;

rnd_studentst	: '<' "rand" "studentst" dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunstudentst($4);
			}
		}
		| '{' "rand" "studentst" dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunstudentst($4);
				$$.rv.pfun = m_pfunstudentst;
				$$.rv.p1 = $4;
			}
		}
		;

rnd_studentsz	: '<' "rand" "studentsz" dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunstudentsz($4);
			}
		}
		| '{' "rand" "studentsz" dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunstudentsz($4);
				$$.rv.pfun = m_pfunstudentsz;
				$$.rv.p1 = $4;
			}
		}
		;

rnd_uniform	: '<' "rand" "uniform" dimm dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfununiform($4, $5);
			}
		}
		| '{' "rand" "uniform" dimm dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfununiform($4, $5);
				$$.rv.pfun = m_pfununiform;
				$$.rv.p1 = $4;
				$$.rv.p2 = $5;
			}
		}
		;

rnd_weibull	: '<' "rand" "weibull" dimm dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunweibull($4, $5);
			}
		}
		| '{' "rand" "weibull" dimm dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunweibull($4, $5);
				$$.rv.pfun = m_pfunweibull;
				$$.rv.p1 = $4;
				$$.rv.p2 = $5;
			}
		}
		;

rnd_xi2		: '<' "rand" "xi2" dimm '>'  
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunxi2($4);
			}
		}
		| '{' "rand" "xi2" dimm '}'
		{
			if (!yyengine->scanning)
			{
				$$.value = m_pfunxi2($4);
				$$.rv.pfun = m_pfunxi2;
				$$.rv.p1 = $4;
			}
		}
		;
*/

uimm		: T_STRING
		{
			char tmp;
			char *ep = &tmp;

			$$ = strtoul($1, &ep, 0);
			
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid unsigned immediate data %s.", $1);
			}
		}
		;

simm		: T_STRING
		{
			char tmp;
			char *ep = &tmp;

			$$ = strtol($1, &ep, 0);
			
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid signed immediate data %s.", $1);
			}
		}
		| '-' T_STRING
		{
			char tmp;
			char *ep = &tmp;

			$$ = -(strtol($2, &ep, 0));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid signed immediate data %s.", $2);
			}
		}
		| '+' T_STRING
		{
			char tmp;
			char *ep = &tmp;

			$$ = +(strtol($2, &ep, 0));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid signed immediate data %s.", $2);
			}
		}
		;

dimm		: T_STRING
		{
			char tmp;
			char *ep = &tmp;

			$$ = strtod($1, &ep);
			
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid double immediate data %s.", $1);
			}
		}
		| '-' T_STRING
		{
			char tmp;
			char *ep = &tmp;

			$$ = -(strtod($2, &ep));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid double immediate data %s.", $2);
			}
		}
		| '+' T_STRING
		{
			char tmp;
			char *ep = &tmp;

			$$ = +(strtod($2, &ep));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid double immediate data %s.", $2);
			}
		}
		;

rdimm		: dimm
		{
			$$ = (Rval *) mcalloc(yyengine, 1, sizeof(Rval), "sf.y:rdimm/$$");
			if ($$ == NULL)
			{
				sfatal(yyengine, yyengine->cp, "Mcalloc failed");
			}

			$$->dval = $1;
		}
		| rnd
		;

ruimm		: uimm
		{
			$$ = (Rval *) mcalloc(yyengine, 1, sizeof(Rval), "sf.y:ruimm/$$");
			if ($$ == NULL)
			{
				sfatal(yyengine, yyengine->cp, "Mcalloc failed");
			}

			$$->uval = $1;
		}
		| rnd
		{
			$$ = $1;
			$$->uval = max(0, (ulong)ceil($1->uval));
		}
		;

rsimm		: simm
		{
			$$ = (Rval *) mcalloc(yyengine, 1, sizeof(Rval), "sf.y:rsimm/$$");
			if ($$ == NULL)
			{
				sfatal(yyengine, yyengine->cp, "Mcalloc failed");
			}

			$$->sval = $1;
		}
		| rnd
		{
			$$ = $1;
			$$->sval = (int)ceil($1->sval);
		}
		;


dimmlist	: '{' dimmlistbody '}'
		{
			$$ = $2;
		}
		;

dimmlistbody	: dimm
		{
			DoubleListItem	*item;

			$$ = (DoubleList *) mcalloc(yyengine, 1, sizeof(DoubleList), "sf.y:dimmlistbody/$$");
			if ($$ == NULL)
			{
				sfatal(yyengine, yyengine->cp, "Mcalloc failed");
			}

			item = (DoubleListItem *)mcalloc(yyengine, 1, sizeof(DoubleListItem), "sf.y:dimmlistbody/item,1");
			if (item == NULL)
			{
				sfatal(yyengine, yyengine->cp, "Mcalloc failed");
			}
			item->value = $1;
			item->next = NULL;

			$$->hd = $$->tl = item;
			$$->len = 1;
		}
		| dimmlistbody ',' dimm
		{
			DoubleListItem	*item;

			item = (DoubleListItem *)mcalloc(yyengine, 1, sizeof(DoubleListItem), "sf.y:dimmlistbody/item,2");
			if (item == NULL)
			{
				sfatal(yyengine, yyengine->cp, "Mcalloc failed");
			}
			item->value = $3;
			item->next = NULL;

			$$->tl->next = item;
			$$->tl = item;
		}
		;


disp		: T_LABEL
		{
			/*								*/
			/*	According to manual, PC of instr two instrs away is	*/
			/*	used to calculate target addr:				*/
			/*			BF					*/
			/*			NOP					*/
			/*			NOP <- this PC is used to calc targ	*/
			/*	We divide by two b'cos the disp is in terms of number	*/
			/*	of instructions, whereas the PC arithmetic is in terms	*/
			/*	of mem loacation, and instructions are placed in mem	*/
			/*	at every other byte.					*/
			/*								*/
			$$ = ($1 - (yyengine->cp->PC+4)) >> 1;

			/*	Warn if the PC calculated displacement was not a multiple of 2 */
			if (($1 - (yyengine->cp->PC+4)) & 0x1)
			{
				merror(yyengine, "Disp in terms of mem addr was not on word boundary.");
			}
		}
		| T_STRING
		{
			char tmp;
			char *ep = &tmp;


			$$ = strtol($1, &ep, 0);
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid DISP ([%s]). Possibly due to a .comm.", $1);
			}
		}
		| '+' T_STRING
		{
			char tmp;
			char *ep = &tmp;


			$$ = +(strtol($2, &ep, 0));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid DISP ([%s]). Possibly due to a .comm.", $2);
			}
		}
		| '-' T_STRING
		{
			char tmp;
			char *ep = &tmp;


			$$ = -(strtol($2, &ep, 0));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid DISP ([%s]). Possibly due to a .comm.", $2);
			}
		}
		;


optstring	: {
			/*								*/
			/*	To make handling of empty string in T_RUN rule 		*/
			/*	uniform, we malloc a place holder.			*/
			/*								*/
			char *tmp = mmalloc(yyengine, sizeof(char), "opstring rule in shasm.y");
			tmp[0] = '\0';
			$$ = tmp;
		}
		| T_STRING
		{
			$$ = $1;
		}
		;


reg		: T_R0 {$$ = 0;}
		| T_R1 {$$ = 1;}
		| T_R2 {$$ = 2;}
		| T_R3 {$$ = 3;}
		| T_R4 {$$ = 4;}
		| T_R5 {$$ = 5;}
		| T_R6 {$$ = 6;}
		| T_R7 {$$ = 7;}
		| T_R8 {$$ = 8;}
		| T_R9 {$$ = 9;}
		| T_R10 {$$ = 10;}
		| T_R11 {$$ = 11;}
		| T_R12 {$$ = 12;}
		| T_R13 {$$ = 13;}
		| T_R14 {$$ = 14;}
		| T_R15 {$$ = 15;}
;
%%

#include "lex-hitachi-sh.c"

int
yyerror(char *err)
{
	merror(yyengine, "Invalid command! (for superH)");
	clearistream(yyengine);
	
	return 0;
}
