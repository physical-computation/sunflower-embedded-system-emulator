/*
	Copyright (c) 1999-2008, Phillip Stanley-Marbell (author)
			2019, Samuel Man-Shun Wong (author)
 
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
%token	T_DUMPHIST
%token	T_DUMPHISTPRETTY
%token	T_LDHISTRND
%token	T_ADDHIST
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
%token	T_TAINTMEM
%token	T_TAINTREG
%token	T_FTAINTREG
%token	T_DUMPTAINTDISTR


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

/*	RISC-V Registers	*/
%token	T_X0 	//T_zero	hardwired to 0, ignores writes		
%token	T_X1 	//T_ra		return address for jumps		 
%token	T_X2 	//T_sp		stack pointer				 
%token	T_X3 	//T_gp		global pointer				
%token	T_X4 	//T_tp		thread pointer				
%token	T_X5 	//T_t0		temporary register 0			 
%token	T_X6 	//T_t1		temporary register 1			 
%token	T_X7 	//T_t2		temporary register 2			 
%token	T_X8 	//T_s0 or fp	saved register 0 or frame pointer 	 
%token	T_X9 	//T_s1 		saved register 1 			 
%token	T_X10 	//T_a0		return value or function argument 0 	 
%token	T_X11 	//T_a1		return value or function argument 1 	 
%token	T_X12 	//T_a2		function argument 2 			 
%token	T_X13 	//T_a3		function argument 3 			 
%token	T_X14 	//T_a4		function argument 4 			 
%token	T_X15 	//T_a5		function argument 5 			 
%token	T_X16 	//T_a6		function argument 6 			 
%token	T_X17 	//T_a7		function argument 7 			 
%token	T_X18 	//T_s2		saved register 2 			 
%token	T_X19 	//T_s3		saved register 3 			 
%token	T_X20 	//T_s4		saved register 4 			
%token	T_X21 	//T_s5		saved register 5 		
%token	T_X22 	//T_s6		saved register 6 			
%token	T_X23 	//T_s7		saved register 6 			
%token	T_X24 	//T_s8		saved register 8 		
%token	T_X25 	//T_s9		saved register 9 		
%token	T_X26 	//T_s10		saved register 10 			
%token	T_X27 	//T_s11		saved register 11 		
%token	T_X28 	//T_t3		temporary register 3 		
%token	T_X29 	//T_t4		temporary register 4 		
%token	T_X30 	//T_t5		temporary register 5 		
%token	T_X31 	//T_t6		temporary register 6 		
%token	T_PC 	//T_PC		program counter

/*	RISC-V Floating point registers	*/
%token	T_F0 	//T_FT0		fp temporaries
%token	T_F1 	//T_FT1		fp temporaries
%token	T_F2 	//T_FT2		fp temporaries
%token	T_F3 	//T_FT3		fp temporaries
%token	T_F4 	//T_FT4		fp temporaries
%token	T_F5 	//T_FT5		fp temporaries
%token	T_F6 	//T_FT6		fp temporaries
%token	T_F7 	//T_FT7		fp temporaries
%token	T_F8 	//T_FS0		fp saved registers
%token	T_F9 	//T_FS1		fp saved registers
%token	T_F10 	//T_FA0		fp arguments/return values
%token	T_F11 	//T_FA1		fp arguments/return values
%token	T_F12 	//T_FA2		fp arguments
%token	T_F13 	//T_FA3		fp arguments
%token	T_F14 	//T_FA4		fp arguments
%token	T_F15 	//T_FA5		fp arguments
%token	T_F16 	//T_FA6		fp arguments
%token	T_F17 	//T_FA7		fp arguments
%token	T_F18 	//T_FS2		fp saved registers
%token	T_F19 	//T_FS3		fp saved registers
%token	T_F20 	//T_FS4		fp saved registers
%token	T_F21 	//T_FS5		fp saved registers
%token	T_F22 	//T_FS6		fp saved registers
%token	T_F23 	//T_FS7		fp saved registers
%token	T_F24 	//T_FS8		fp saved registers
%token	T_F25 	//T_FS9		fp saved registers
%token	T_F26 	//T_FS10	fp saved registers
%token	T_F27 	//T_FS11	fp saved registers
%token	T_F28 	//T_FT8		fp temporaries
%token	T_F29 	//T_FT9		fp temporaries
%token	T_F30 	//T_FT10	fp temporaries
%token	T_F31 	//T_FT11	fp temporaries

/*    	Misc		*/
%token	T_LABELDEFN

/*	RISC-V Instructions	*/
%token T_ADD
%token T_ADDI
%token T_AND
%token T_ANDI
%token T_AUIPC
%token T_BEQ
%token T_BGE
%token T_BGEU
%token T_BLT
%token T_BLTU
%token T_BNE
%token T_FENCE		/*	Empty definition in file op-riscv.c	*/
%token T_FENCE_I	/*	Empty definition in file op-riscv.c	*/ 
%token T_JAL
%token T_JALR
%token T_LB
%token T_LBU
%token T_LH
%token T_LHU
%token T_LUI
%token T_LW
%token T_OR
%token T_ORI
%token T_SB
%token T_SH
%token T_SLL
%token T_SLLI
%token T_SLT
%token T_SLTI
%token T_SLTIU
%token T_SLTU
%token T_SRA
%token T_SRAI
%token T_SRL
%token T_SRLI
%token T_SUB
%token T_SW
%token T_XOR
%token T_XORI

%type	<uval>	reg
%type	<uval>	freg
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

expr		: add_riscv_instr
		| addi_riscv_instr
		| and_riscv_instr
		| andi_riscv_instr
		| auipc_riscv_instr
		| beq_riscv_instr
		| bge_riscv_instr
		| bgeu_riscv_instr
		| blt_riscv_instr
		| bltu_riscv_instr
		| bne_riscv_instr
		| fence_riscv_instr	/*	Not implemented in op-riscv.c */
		| fence_i_riscv_instr	/*	Not implemented in op-riscv.c */
		| jal_riscv_instr
		| jalr_riscv_instr
		| lb_riscv_instr
		| lbu_riscv_instr
		| lh_riscv_instr
		| lhu_riscv_instr
		| lui_riscv_instr
		| lw_riscv_instr
		| or_riscv_instr
		| ori_riscv_instr
		| sb_riscv_instr
		| sh_riscv_instr
		| sll_riscv_instr
		| slli_riscv_instr
		| slt_riscv_instr
		| slti_riscv_instr
		| sltiu_riscv_instr
		| sltu_riscv_instr
		| sra_riscv_instr
		| srai_riscv_instr
		| srl_riscv_instr
		| srli_riscv_instr
		| sub_riscv_instr
		| sw_riscv_instr
		| xor_riscv_instr
		| xori_riscv_instr
		;


sf_cmd		: T_QUIT '\n'
		{
			if (!yyengine->scanning)
			{
				mexit(yyengine, "Exiting as per user's request.", 0);
			}
		}

		/*
		 *	NOTE: We no longer accept the old-style velocity and orbit syntax,
		 *	though we allow you to omit either the x/y/z location,
		 *	or the trajectory file, rate, and loop flag.
		 */
		| T_TAINTMEM uimm uimm uimm uimm uimm '\n'
		/*
		*	Arguments: Address, start PC, end PC, Taint, size
		*/
		{	/*
			 *	Mark given memory as origin of taint
			 */
			if (!yyengine->scanning)
			{
				m_taintmem(yyengine,yyengine->cp, $2, $3, $4, $5, $6);

			}
		}
		| T_TAINTREG reg uimm uimm uimm '\n'
		/*
		*	Arguments: RegisterName, start PC, end PC, Taint
		*/
		{
			/*
			 *	Mark given register as an origin of taint
			 */
			if (!yyengine->scanning)
			{
				m_taintreg(yyengine,yyengine->cp, $2, $3, $4, $5);
			}
		}
		| T_FTAINTREG reg uimm uimm uimm '\n'
		/*
		*	Arguments: RegisterName, start PC, end PC, Taint
		*/
		{
			/*
			 *	Mark given register as an origin of taint
			 */
			if (!yyengine->scanning)
			{
				m_taintreg(yyengine,yyengine->cp, $2, $3, $4, $5);
			}
		}
		| T_DUMPTAINTDISTR '\n'
		/*
		*	Arguments: None
		*/
		{
			/*
			 *	Give taint of each RISC-V command
			 */
			if (!yyengine->scanning)
			{
				m_riscvdumptaintdistr(yyengine,yyengine->cp);
			}
		}
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
		| T_DUMPHIST uimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->dumphist(yyengine, yyengine->cp, $2);
			}
		}
		| T_DUMPHISTPRETTY uimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->dumphistpretty(yyengine, yyengine->cp, $2);
			}
		}
		| T_LDHISTRND uimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->ldhistrandom(yyengine, yyengine->cp, $2);
			}
		}
		| T_ADDHIST uimm uimm uimm '\n'
		{
			if (!yyengine->scanning)
			{
				yyengine->cp->addhist(yyengine, yyengine->cp, $2, $3, $4);
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
			yyengine->cp->TAINTMEMBASE = $2;
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

add_riscv_instr	: T_ADD reg ',' reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_add(yyengine, yyengine->cp, $4, $6, $2);
				}

				uint32_t tmp = (B00110011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0000 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	rs2,	bit 20-24		*/
						| (B00000000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
		;

addi_riscv_instr	: T_ADDI reg ',' reg ',' simm
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_addi(yyengine, yyengine->cp, $4, $2, $6);
				}

				uint32_t tmp = (B00010011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0000 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	imm,	bit 20-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
		;

and_riscv_instr	: T_AND reg ',' reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_and(yyengine, yyengine->cp, $4, $6, $2);
				}

				uint32_t tmp = (B00110011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0111 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	rs2,	bit 20-24		*/
						| (B00000000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
		;

andi_riscv_instr	: T_ANDI reg ',' reg ',' simm
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_andi(yyengine, yyengine->cp, $4, $2, $6);
				}

				uint32_t tmp = (B00010011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0111 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	imm,	bit 20-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
		;

auipc_riscv_instr	: T_AUIPC reg ',' simm
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_auipc(yyengine, yyengine->cp, $2, $4);
				}
				uint32_t tmp = B00010111	/*	opcode,	bit 0-6			*/
					| ($2 << 7)		/*	rd,	bit 7-11		*/
					| ($4 << 12)		/*	imm,	bit 12-31		*/
					;
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
				yyengine->cp->PC += 4;
			}
		}
		;

beq_riscv_instr	: T_BEQ reg ',' reg ',' simm	/*	the PC offset (of type simm) should be half of the desired jump	*/
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				uint8_t imm1 = ($6 >> 0) & B1111;
				uint8_t imm5 = ($6 >> 4) & B111111;	/*	Shift the fragements	*/
				uint8_t imm11 = ($6 >> 11) & 1;		/*	so that they are each	*/
				uint8_t imm12 = ($6 >> 12) & 1;		/*	an individual number	*/

				if (!yyengine->cp->pipelined)	
				{
					riscv_beq(yyengine, yyengine->cp, $2, $4, imm1, imm5, imm11, imm12);
				}

				uint32_t tmp = (B01100011 << 0)		/*	opcode,	bit 0-6			*/
						| (imm11 << 7)		/*	imm11,	bit 7			*/
						| (imm1 << 8)		/*	imm1-4,	bit 8-11		*/
						| (B0000 << 12)		/*	funct3,	bit 12-14		*/
						| ($2 << 15)		/*	rs1,	bit 15-19		*/
						| ($4 << 20)		/*	rs2,	bit 20-24		*/
						| (imm5 << 25)		/*	imm5-10,bit 25-30		*/
						| (imm12 << 31)		/*	imm12,	bit 31			*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
		;

bne_riscv_instr	: T_BNE reg ',' reg ',' simm	/*	the PC offset of type simm should be half of the desired jump	*/
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				uint8_t imm1 = ($6 >> 0) & B1111;
				uint8_t imm5 = ($6 >> 4) & B111111;	/*	Shift the fragements	*/
				uint8_t imm11 = ($6 >> 11) & 1;		/*	so that they are each	*/
				uint8_t imm12 = ($6 >> 12) & 1;		/*	an individual number	*/
				if (!yyengine->cp->pipelined)	
				{
					riscv_bne(yyengine, yyengine->cp, $2, $4, imm1, imm5, imm11, imm12);
				}
				uint32_t tmp = (B01100011 << 0)		/*	opcode,	bit 0-6			*/
						| (imm11 << 7)		/*	imm11,	bit 7			*/
						| (imm1 << 8)		/*	imm1-4,	bit 8-11		*/
						| (B0001 << 12)		/*	funct3,	bit 12-14		*/
						| ($2 << 15)		/*	rs1,	bit 15-19		*/
						| ($4 << 20)		/*	rs2,	bit 20-24		*/
						| (imm5 << 25)		/*	imm5-10,bit 25-30		*/
						| (imm12 << 31)		/*	imm12,	bit 31			*/
						;
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
				yyengine->cp->PC += 4;
			}
		}
		;

blt_riscv_instr	: T_BLT reg ',' reg ',' simm	/*	the PC offset of type simm should be half of the desired jump	*/
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{			
				uint8_t imm1 = ($6 >> 0) & B1111;
				uint8_t imm5 = ($6 >> 4) & B111111;	/*	Shift the fragements	*/
				uint8_t imm11 = ($6 >> 11) & 1;		/*	so that they are each	*/
				uint8_t imm12 = ($6 >> 12) & 1;		/*	an individual number	*/
				if (!yyengine->cp->pipelined)	
				{
					riscv_blt(yyengine, yyengine->cp, $2, $4, imm1, imm5, imm11, imm12);
				}
				uint32_t tmp = (B01100011 << 0)		/*	opcode,	bit 0-6			*/
						| (imm11 << 7)		/*	imm11,	bit 7			*/
						| (imm1 << 8)		/*	imm1-4,	bit 8-11		*/
						| (B0100 << 12)		/*	funct3,	bit 12-14		*/
						| ($2 << 15)		/*	rs1,	bit 15-19		*/
						| ($4 << 20)		/*	rs2,	bit 20-24		*/
						| (imm5 << 25)		/*	imm5-10,bit 25-30		*/
						| (imm12 << 31)		/*	imm12,	bit 31			*/
						;
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
				yyengine->cp->PC += 4;
			}
		}
		;

bge_riscv_instr	: T_BGE reg ',' reg ',' simm	/*	the PC offset of type simm should be half of the desired jump	*/
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				uint8_t imm1 = ($6 >> 0) & B1111;
				uint8_t imm5 = ($6 >> 4) & B111111;	/*	Shift the fragements	*/
				uint8_t imm11 = ($6 >> 11) & 1;		/*	so that they are each	*/
				uint8_t imm12 = ($6 >> 12) & 1;		/*	an individual number	*/
				if (!yyengine->cp->pipelined)	
				{
					riscv_bge(yyengine, yyengine->cp, $2, $4, imm1, imm5, imm11, imm12);
				}
				uint32_t tmp = (B01100011 << 0)		/*	opcode,	bit 0-6			*/
						| (imm11 << 7)		/*	imm11,	bit 7			*/
						| (imm1 << 8)		/*	imm1-4,	bit 8-11		*/
						| (B0101 << 12)		/*	funct3,	bit 12-14		*/
						| ($2 << 15)		/*	rs1,	bit 15-19		*/
						| ($4 << 20)		/*	rs2,	bit 20-24		*/
						| (imm5 << 25)		/*	imm5-10,bit 25-30		*/
						| (imm12 << 31)		/*	imm12,	bit 31			*/
						;
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
				yyengine->cp->PC += 4;
			}
		}
		;

bltu_riscv_instr	: T_BLTU reg ',' reg ',' uimm	/*	the PC offset of type simm should be half of the desired jump	*/
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				uint8_t imm1 = ($6 >> 0) & B1111;
				uint8_t imm5 = ($6 >> 4) & B111111;	/*	Shift the fragements	*/
				uint8_t imm11 = ($6 >> 11) & 1;		/*	so that they are each	*/
				uint8_t imm12 = ($6 >> 12) & 1;		/*	an individual number	*/
				if (!yyengine->cp->pipelined)	
				{
					riscv_bltu(yyengine, yyengine->cp, $2, $4, imm1, imm5, imm11, imm12);
				}
				uint32_t tmp = (B01100011 << 0)		/*	opcode,	bit 0-6			*/
						| (imm11 << 7)		/*	imm11,	bit 7			*/
						| (imm1 << 8)		/*	imm1-4,	bit 8-11		*/
						| (B0110 << 12)		/*	funct3,	bit 12-14		*/
						| ($2 << 15)		/*	rs1,	bit 15-19		*/
						| ($4 << 20)		/*	rs2,	bit 20-24		*/
						| (imm5 << 25)		/*	imm5-10,bit 25-30		*/
						| (imm12 << 31)		/*	imm12,	bit 31			*/
						;
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
				yyengine->cp->PC += 4;
			}
		}
		;
	
bgeu_riscv_instr	: T_BGEU reg ',' reg ',' uimm	/*	the PC offset of type simm should be half of the desired jump	*/
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				uint8_t imm1 = ($6 >> 0) & B1111;
				uint8_t imm5 = ($6 >> 4) & B111111;	/*	Shift the fragements	*/
				uint8_t imm11 = ($6 >> 11) & 1;		/*	so that they are each	*/
				uint8_t imm12 = ($6 >> 12) & 1;		/*	an individual number	*/
				if (!yyengine->cp->pipelined)	
				{
					riscv_bgeu(yyengine, yyengine->cp, $2, $4, imm1, imm5, imm11, imm12);
				}
				uint32_t tmp = (B01100011 << 0)		/*	opcode,	bit 0-6			*/
						| (imm11 << 7)		/*	imm11,	bit 7			*/
						| (imm1 << 8)		/*	imm1-4,	bit 8-11		*/
						| (B0111 << 12)		/*	funct3,	bit 12-14		*/
						| ($2 << 15)		/*	rs1,	bit 15-19		*/
						| ($4 << 20)		/*	rs2,	bit 20-24		*/
						| (imm5 << 25)		/*	imm5-10,bit 25-30		*/
						| (imm12 << 31)		/*	imm12,	bit 31			*/
						;
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
				yyengine->cp->PC += 4;
			}
		}
		;

fence_riscv_instr: T_FENCE
		{
			mprint(yyengine, NULL, siminfo, 
					"Not implemented.");
		}
		;

fence_i_riscv_instr: T_FENCE_I
		{
			mprint(yyengine, NULL, siminfo, 
					"Not implemented.");
		}
		;

jal_riscv_instr	: T_JAL reg ',' simm
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo,
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				uint8_t imm1 = ($4 >> 0) & B0000001111111111;
				uint8_t imm11 = ($4 >> 10) & 1;			/*	Shift the fragements	*/
				uint8_t imm12 = ($4 >> 11) & B11111111;		/*	so that they are each	*/
				uint8_t imm20 = ($4 >> 19) & 1;			/*	an individual number	*/
				if (!yyengine->cp->pipelined)	
				{
					riscv_jal(yyengine, yyengine->cp, $2, imm1, imm11, imm12, imm20);
				}
				uint32_t tmp = (B01101111 << 0)	/*	opcode,		bit 0-6			*/
						| ($2 << 7)	/*	rd,		bit 7-11		*/
						| (imm12 << 12)	/*	imm12-19,	bit 12-19		*/
						| (imm11 << 20)	/*	imm11,		bit 20			*/
						| (imm1 << 25)	/*	imm1-10,	bit 21-30		*/
						| (imm20 << 31)	/*	imm12,		bit 31			*/
						;
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
				yyengine->cp->PC += 4;
			}
		}
		;

jalr_riscv_instr	: T_JALR reg ',' reg ',' simm
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo,
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_jalr(yyengine, yyengine->cp, $4, $2, $6);
				}

				uint32_t tmp = (B01100111 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0000 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	imm,	bit 20-31		*/
						;
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
				yyengine->cp->PC += 4;
			}
		}
		;

lb_riscv_instr	: T_LB reg ',' reg ',' simm
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_lb(yyengine, yyengine->cp, $4, $2, $6);
				}

				uint32_t tmp = (B00000011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0000 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	imm,	bit 20-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
		;

lbu_riscv_instr	: T_LBU reg ',' reg ',' uimm
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_lbu(yyengine, yyengine->cp, $4, $2, $6);
				}

				uint32_t tmp = (B00000011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0100 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	imm,	bit 20-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
		;

lh_riscv_instr	: T_LH reg ',' reg ',' simm
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_lh(yyengine, yyengine->cp, $4, $2, $6);
				}

				uint32_t tmp = (B00000011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0001 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	imm,	bit 20-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
		;

lhu_riscv_instr	: T_LHU reg ',' reg ',' uimm
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_lhu(yyengine, yyengine->cp, $4, $2, $6);
				}

				uint32_t tmp = (B00000011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0101 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	imm,	bit 20-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
		;

lui_riscv_instr	: T_LUI reg ',' simm
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_lui(yyengine, yyengine->cp, $2, $4);
				}
				uint32_t tmp = B00110111	/*	opcode,	bit 0-6			*/
					| ($2 << 7)		/*	rd,	bit 7-11		*/
					| ($4 << 12)		/*	imm,	bit 12-31		*/
					;
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
				yyengine->cp->PC += 4;
			}
		}
		;

lw_riscv_instr	: T_LW reg ',' reg ',' simm
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_lw(yyengine, yyengine->cp, $4, $2, $6);
				}

				uint32_t tmp = (B00000011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0010 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	imm,	bit 20-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
		;

or_riscv_instr	: T_OR reg ',' reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_or(yyengine, yyengine->cp, $4, $6, $2);
				}

				uint32_t tmp = (B00110011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0110 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	rs2,	bit 20-24		*/
						| (B00000000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
		;

ori_riscv_instr	: T_ORI reg ',' reg ',' simm
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_ori(yyengine, yyengine->cp, $4, $2, $6);
				}

				uint32_t tmp = (B00010011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0110 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	imm,	bit 20-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
		;

sb_riscv_instr	: T_SB reg ',' reg ',' simm
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				uint8_t imm0 = ($6 >> 0) & B00011111;	/*	Shift the fragements	*/
				uint8_t imm5 = ($6 >> 4) & B01111111;	/*	so that they are each	*/
									/*	an individual number	*/

				if (!yyengine->cp->pipelined)	
				{
					riscv_sb(yyengine, yyengine->cp, $2, $4, imm0, imm5);
				}

				uint32_t tmp = (B00100011 << 0)		/*	opcode,	bit 0-6			*/
						| (imm0 << 7)		/*	imm11,	bit 7-11		*/
						| (B0000 << 12)		/*	funct3,	bit 12-14		*/
						| ($2 << 15)		/*	rs1,	bit 15-19		*/
						| ($4 << 20)		/*	rs2,	bit 20-24		*/
						| (imm5 << 25)		/*	imm5-10,bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
		;

sh_riscv_instr	: T_SH reg ',' reg ',' simm
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				uint8_t imm0 = ($6 >> 0) & B00011111;	/*	Shift the fragements	*/
				uint8_t imm5 = ($6 >> 4) & B01111111;	/*	so that they are each	*/
									/*	an individual number	*/

				if (!yyengine->cp->pipelined)	
				{
					riscv_sh(yyengine, yyengine->cp, $2, $4, imm0, imm5);
				}

				uint32_t tmp = (B00100011 << 0)		/*	opcode,	bit 0-6			*/
						| (imm0 << 7)		/*	imm11,	bit 7-11		*/
						| (B0001 << 12)		/*	funct3,	bit 12-14		*/
						| ($2 << 15)		/*	rs1,	bit 15-19		*/
						| ($4 << 20)		/*	rs2,	bit 20-24		*/
						| (imm5 << 25)		/*	imm5-10,bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
		;

sll_riscv_instr	: T_SLL reg ',' reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_sll(yyengine, yyengine->cp, $4, $6, $2);
				}

				uint32_t tmp = (B00110011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0001 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	rs2,	bit 20-24		*/
						| (B00000000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			 }
		}
		;

slli_riscv_instr	: T_SLLI reg ',' reg ',' simm/*shamt*/
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_slli(yyengine, yyengine->cp, $4, $2, $6);
				}

				uint32_t tmp = (B00010011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0001 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	rs2,	bit 20-24		*/
						| (B00000000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			 }
		}
		;

slt_riscv_instr	: T_SLT reg ',' reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_slt(yyengine, yyengine->cp, $4, $6, $2);
				}

				uint32_t tmp = (B00110011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0010 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	rs2,	bit 20-24		*/
						| (B00000000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			 }
		}
		;

slti_riscv_instr	: T_SLTI reg ',' reg ',' simm
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_slti(yyengine, yyengine->cp, $4, $2, $6);
				}

				uint32_t tmp = (B00010011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0010 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	imm,	bit 20-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
		;

sltiu_riscv_instr	: T_SLTIU reg ',' reg ',' uimm
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_sltiu(yyengine, yyengine->cp, $4, $2, $6);
				}

				uint32_t tmp = (B00010011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0011 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	imm,	bit 20-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
		;

sltu_riscv_instr	: T_SLTU reg ',' reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_sltu(yyengine, yyengine->cp, $4, $6, $2);
				}

				uint32_t tmp = (B00110011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0011 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	rs2,	bit 20-24		*/
						| (B00000000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			 }
		}
		;

sra_riscv_instr	: T_SRA reg ',' reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_sra(yyengine, yyengine->cp, $4, $6, $2);
				}

				uint32_t tmp = (B00110011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0101 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	rs2,	bit 20-24		*/
						| (B00100000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			 }
		}
		;

srai_riscv_instr	: T_SRAI reg ',' reg ',' simm/*shamt*/
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_srai(yyengine, yyengine->cp, $4, $2, $6);
				}

				uint32_t tmp = (B00010011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0101 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	rs2,	bit 20-24		*/
						| (B00100000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			 }
		}
		;

srl_riscv_instr	: T_SRL reg ',' reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_srl(yyengine, yyengine->cp, $4, $6, $2);
				}

				uint32_t tmp = (B00110011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0101 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	rs2,	bit 20-24		*/
						| (B00000000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			 }
		}
		;

srli_riscv_instr	: T_SRLI reg ',' reg ',' simm/*shamt*/
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_srli(yyengine, yyengine->cp, $4, $2, $6);
				}

				uint32_t tmp = (B00010011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0101 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	rs2,	bit 20-24		*/
						| (B00000000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			 }
		}
		;

sub_riscv_instr	: T_SUB reg ',' reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_sub(yyengine, yyengine->cp, $4, $6, $2);
				}

				uint32_t tmp = (B00110011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0000 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	rs2,	bit 20-24		*/
						| (B00100000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			 }
		}
		;

sw_riscv_instr	: T_SW reg ',' reg ',' simm
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				uint8_t imm0 = ($6 >> 0) & B00011111;	/*	Shift the fragements	*/
				uint8_t imm5 = ($6 >> 4) & B01111111;	/*	so that they are each	*/
									/*	an individual number	*/

				if (!yyengine->cp->pipelined)	
				{
					riscv_sw(yyengine, yyengine->cp, $2, $4, imm0, imm5);
				}

				uint32_t tmp = (B00100011 << 0)		/*	opcode,	bit 0-6			*/
						| (imm0 << 7)		/*	imm11,	bit 7-11		*/
						| (B0010 << 12)		/*	funct3,	bit 12-14		*/
						| ($2 << 15)		/*	rs1,	bit 15-19		*/
						| ($4 << 20)		/*	rs2,	bit 20-24		*/
						| (imm5 << 25)		/*	imm5-10,bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
		;

xor_riscv_instr	: T_XOR reg ',' reg ',' reg
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_xor(yyengine, yyengine->cp, $4, $6, $2);
				}

				uint32_t tmp = (B00110011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0100 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	rs2,	bit 20-24		*/
						| (B00000000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			 }
		}
		;

xori_riscv_instr	: T_XORI reg ',' reg ',' simm
		{
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_xori(yyengine, yyengine->cp, $4, $2, $6);
				}

				uint32_t tmp = (B00010011 << 0)		/*	opcode,	bit 0-6			*/
						| ($2 << 7)		/*	rd,	bit 7-11		*/
						| (B0100 << 12)		/*	funct3,	bit 12-14		*/
						| ($4 << 15)		/*	rs1,	bit 15-19		*/
						| ($6 << 20)		/*	imm,	bit 20-31		*/
						;

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
				yyengine->cp->PC += 4;
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


reg		: T_X0 {$$ = 0;}
		| T_X1 {$$ = 1;}
		| T_X2 {$$ = 2;}
		| T_X3 {$$ = 3;}
		| T_X4 {$$ = 4;}
		| T_X5 {$$ = 5;}
		| T_X6 {$$ = 6;}
		| T_X7 {$$ = 7;}
		| T_X8 {$$ = 8;}
		| T_X9 {$$ = 9;}
		| T_X10 {$$ = 10;}
		| T_X11 {$$ = 11;}
		| T_X12 {$$ = 12;}
		| T_X13 {$$ = 13;}
		| T_X14 {$$ = 14;}
		| T_X15 {$$ = 15;}
		| T_X16 {$$ = 16;}
		| T_X17 {$$ = 17;}
		| T_X18 {$$ = 18;}
		| T_X19 {$$ = 19;}
		| T_X20 {$$ = 20;}
		| T_X21 {$$ = 21;}
		| T_X22 {$$ = 22;}
		| T_X23 {$$ = 23;}
		| T_X24 {$$ = 24;}
		| T_X25 {$$ = 25;}
		| T_X26 {$$ = 26;}
		| T_X27 {$$ = 27;}
		| T_X28 {$$ = 28;}
		| T_X29 {$$ = 29;}
		| T_X30 {$$ = 30;}
		| T_X31 {$$ = 31;}
;

freg		: T_F0 {$$ = 0;}
		| T_F1 {$$ = 1;}
		| T_F2 {$$ = 2;}
		| T_F3 {$$ = 3;}
		| T_F4 {$$ = 4;}
		| T_F5 {$$ = 5;}
		| T_F6 {$$ = 6;}
		| T_F7 {$$ = 7;}
		| T_F8 {$$ = 8;}
		| T_F9 {$$ = 9;}
		| T_F10 {$$ = 10;}
		| T_F11 {$$ = 11;}
		| T_F12 {$$ = 12;}
		| T_F13 {$$ = 13;}
		| T_F14 {$$ = 14;}
		| T_F15 {$$ = 15;}
		| T_F16 {$$ = 16;}
		| T_F17 {$$ = 17;}
		| T_F18 {$$ = 18;}
		| T_F19 {$$ = 19;}
		| T_F20 {$$ = 20;}
		| T_F21 {$$ = 21;}
		| T_F22 {$$ = 22;}
		| T_F23 {$$ = 23;}
		| T_F24 {$$ = 24;}
		| T_F25 {$$ = 25;}
		| T_F26 {$$ = 26;}
		| T_F27 {$$ = 27;}
		| T_F28 {$$ = 28;}
		| T_F29 {$$ = 29;}
		| T_F30 {$$ = 30;}
		| T_F31 {$$ = 31;}
;
%%

#include "lex-riscv.c"

int
yyerror(char *err)
{
	merror(yyengine, "Invalid command! (for riscv)");
	clearistream(yyengine);

	return 0;
}
