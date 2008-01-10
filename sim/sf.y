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
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <math.h>
#include "endian-hitachi-sh.h"
#include "sf.h"
#include "mmalloc.h"
#include "mextern.h"
%}


%union
{
	double	dval;
	ulong	uval;
	long	sval;
	char	*str;
}


/*	Myrmigki Commands	*/
%token	T_BATTALERTFRAC
%token	T_BATTLEAKCURRENT
%token	T_BATTCF
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
%token	T_LOAD
%token	T_LOCSTATS
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
%token	T_SETFREQ
%token	T_SETIFCOUI
%token	T_SETNODE
%token	T_SETPC
%token	T_SETPHYSICSPERIOD
%token	T_SETQUANTUM
%token	T_SETSCALEALPHA
%token	T_SETSCALEK
%token	T_SETSCALEVT
%token	T_SETTAG
%token	T_SETTIMERDELAY
%token	T_SETVDD
%token	T_SHAREBUS
%token	T_SHOWCLK
%token	T_SHOWPIPE
%token	T_SHOWTAGS
%token	T_SIGNALSRC
%token	T_SIGNALSUBSCRIBE
%token	T_SIZEMEM
%token	T_SIZEPAU
%token	T_SPLIT
%token	T_SRECL
%token	T_STEP
%token	T_STOP
%token	T_SFATAL
%token	T_TRACE
%token	T_THROTTLE
%token	T_THROTTLEWIN
%token	T_VERBOSE
%token	T_VERSION

/*	Assembler control instructions 		*/
%token	T_DOTALIGN
%token	T_DOTORG
%token	T_DOTDATA
%token	T_DOTTEXT
%token	T_DOTGLOBAL
%token	T_DOTFILE
%token	T_DOTLONG
%token	T_DOTCOMM

/*	Registers	*/
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

/*	Instructions	*/
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
%token	T_DIV0U	/*      TODO: This instr is not implemented in our interface assembler  	*/
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
		| myrmigki_cmd
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


myrmigki_cmd	: T_QUIT '\n'
		{
			if (!SCANNING)
			{
				mexit("Exiting as per user's request.", 0);
			}
		}
		| T_NEWNODE optstring dimm dimm dimm uimm dimm '\n'
		{
			if (!SCANNING)
			{
				m_newnode($2, $3, $4, $5, $6, $7);
			}
		}
		| T_BATTNODEATTACH uimm '\n'
		{
			if (!SCANNING)
			{
				batt_nodeattach(CUR_STATE, $2);
			}
		}
		| T_BATTSTATS uimm '\n'
		{
			if (!SCANNING)
			{
				batt_printstats(CUR_STATE, $2);
			}
		}
		| T_NEWBATT uimm dimm '\n'
		{
			if (!SCANNING)
			{
				batt_newbatt($2, $3);
			}
		}
		| T_BATTCF dimm '\n'
		{
			if (!SCANNING)
			{
				mprint(NULL, siminfo, 
						"Setting SIM_BATTERIES[%d].Cf to %f\n",
						CUR_BATT_ID, $2);
				SIM_BATTERIES[CUR_BATT_ID].Cf = $2;
			}
		}
		| T_BATTINOMINAL dimm '\n'
		{
			if (!SCANNING)
			{
				mprint(NULL, siminfo, 
					"Setting SIM_BATTERIES[%d].Inominal to %f\n",
					CUR_BATT_ID, $2);
				SIM_BATTERIES[CUR_BATT_ID].Inominal = $2;
			}
		}
		| T_BATTRF dimm '\n'
		{
			if (!SCANNING)
			{
				mprint(NULL, siminfo, 
						"Setting SIM_BATTERIES[%d].Rf to %f\n",
						CUR_BATT_ID, $2);
				SIM_BATTERIES[CUR_BATT_ID].Rf = $2;
			}
		}
		| T_BATTETALUT uimm dimm '\n'
		{
			if (!SCANNING)
			{
				if ($2 < SIM_BATTERIES[CUR_BATT_ID].etaLUTnentries)
				{
					SIM_BATTERIES[CUR_BATT_ID].etaLUT[$2] = $3;
				}
				else
				{
					merror("Invalid etaLUT index.");
				}
			}
		}
		| T_BATTETALUTNENTRIES uimm '\n'
		{
			if (!SCANNING)
			{
				double *tmp = (double *)mrealloc(SIM_BATTERIES[CUR_BATT_ID].etaLUT,
						$2*sizeof(double), 
						"shasm.y, (double *) for T_BATTETALUTNENTRIES");
				if (tmp == NULL)
				{
					merror("Could not resize etaLUT.");
				}
				else
				{
					SIM_BATTERIES[CUR_BATT_ID].etaLUT = tmp;
					SIM_BATTERIES[CUR_BATT_ID].etaLUTnentries = $2;
				}
			}
		}
		| T_BATTVBATTLUT uimm dimm '\n'
		{
			if (!SCANNING)
			{
				if ($2 < SIM_BATTERIES[CUR_BATT_ID].VbattLUTnentries)
				{
					SIM_BATTERIES[CUR_BATT_ID].VbattLUT[$2] = $3;
				}
				else
				{
					merror("Invalid VbattLUT index.");
				}
			}
		}
		| T_BATTVBATTLUTNENTRIES uimm '\n'
		{
			if (!SCANNING)
			{
				double *tmp = (double *)mrealloc(SIM_BATTERIES[CUR_BATT_ID].VbattLUT,
						$2*sizeof(double), 
						"shasm.y, (double *) for T_BATTVBATTLUTNENTRIES");
				if (tmp == NULL)
				{
					merror("Could not resize VbattLUT.");
				}
				else
				{
					SIM_BATTERIES[CUR_BATT_ID].VbattLUT = tmp;
					SIM_BATTERIES[CUR_BATT_ID].VbattLUTnentries = $2;
				}
			}
		}
		| T_BATTVLOSTLUT uimm dimm '\n'
		{
			if (!SCANNING)
			{
				if ($2 < SIM_BATTERIES[CUR_BATT_ID].VlostLUTnentries)
				{
					SIM_BATTERIES[CUR_BATT_ID].VlostLUT[$2] = $3;
				}
				else
				{
					merror("Invalid VlostLUT index.");
				}
			}
		}
		| T_BATTVLOSTLUTNENTRIES uimm '\n'
		{
			if (!SCANNING)
			{
				double *tmp = (double *)mrealloc(SIM_BATTERIES[CUR_BATT_ID].VlostLUT,
						$2*sizeof(double), 
						"shasm.y, (double *) for T_BATTVLOSTLUTNENTRIES");
				if (tmp == NULL)
				{
					merror("Could not resize Vlost.");
				}
				else
				{
					SIM_BATTERIES[CUR_BATT_ID].VlostLUT = tmp;
					SIM_BATTERIES[CUR_BATT_ID].VlostLUTnentries = $2;
				}
			}
		}
		| T_SETBATT uimm '\n'
		{
			if (!SCANNING)
			{
				if (($2 >= SIM_NUM_BATTERIES) || ($2 < 0))
				{
					merror("Battery ID out of range.");
				}
				else
				{
					CUR_BATT_ID = $2;
				}
			}
		}
		| T_POWERTOTAL '\n'
		{
			if (!SCANNING)
			{
				m_powertotal();
			}
		}
		| T_RETRYALG uimm T_STRING '\n'
		{
			if (!SCANNING)
			{
				network_setretryalg(CUR_STATE, $2, $3);
			}
		}
		| T_IGNORENODEDEATHS uimm '\n'
		{
			if (!SCANNING)
			{
				SIM_IGNORE_NODEDEATHS = $2;
			}
		}
		| T_SETSCALEALPHA dimm '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->voltscale_alpha = $2;
			}
		}
		| T_SETSCALEK dimm '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->voltscale_K = $2;
			}
		}
		| T_SETSCALEVT dimm '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->voltscale_Vt = $2;
			}
		}
		| T_SETQUANTUM uimm '\n'
		{
			if (!SCANNING)
			{
				SIM_QUANTUM = $2;
			}
		}
		| T_SETBASENODEID uimm '\n'
		{
			if (!SCANNING)
			{
				SIM_BASENODEID = $2;
			}
		}
		| T_RENUMBERNODES '\n'
		{
			if (!SCANNING)
			{
				m_renumbernodes();
			}
		}
		| T_RESETNODECTRS '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->trip_ustart = musercputimeusecs();
				CUR_STATE->trip_startclk = CUR_STATE->ICLK;
			}
		}
		| T_RESETALLCTRS '\n'
		{
			if (!SCANNING)
			{
				int i;

				for (i = 0; i < SIM_NUM_NODES; i++)
				{
					SIM_STATE_PTRS[i]->trip_ustart = musercputimeusecs();
					SIM_STATE_PTRS[i]->trip_startclk = SIM_STATE_PTRS[i]->ICLK;
				}
			}
		}
		| T_NETSEG2FILE uimm T_STRING '\n'
		{
			if (!SCANNING)
			{
				network_netseg2file($2, $3);
			}
		}
		| T_FILE2NETSEG T_STRING uimm '\n'
		{
			if (!SCANNING)
			{
				network_file2netseg($2, $3);
			}
		}
		| T_CD T_STRING '\n'
		{
			if (!SCANNING)
			{
				int	n = mchdir($2);

				if (n < 0)
				{
					merror("Could not change directory to \"%s\".", $2);
				}
			}
		}
		| T_SETTIMERDELAY dimm
		{
			if (!SCANNING)
			{
				CUR_STATE->settimerintrdelay(CUR_STATE, $2);
			}
		}
		| T_SETPHYSICSPERIOD dimm
		{
			if (!SCANNING)
			{
				SIM_PHYSICS_PERIOD = $2 * 1E-6;
			}
		}
		| T_SETBATTFEEDPERIOD dimm
		{
			if (!SCANNING)
			{
				SIM_BATTFEED_PERIOD = $2 * 1E-6;
			}
		}
		| T_FORCEAVGPWR dimm dimm
		{
			if (!SCANNING)
			{
				CUR_STATE->force_avgpwr = $2;
				CUR_STATE->force_sleeppwr = $3;
			}
		}
		| T_NETSEGPROPMODEL uimm uimm dimm
		{
			if (!SCANNING)
			{
				network_netsegpropmodel($2, $3, $4);
			}
		}
		| T_SETDUMPPWRPERIOD dimm
		{
			if (!SCANNING)
			{
				SIM_DUMPPWR_PERIOD = $2 * 1E-6;
			}
		}
		| T_VERSION
		{
			if (!SCANNING)
			{
				m_version();
			}
		}
		| T_SENSORSDEBUG
		{
			if (!SCANNING)
			{
				physics_sensorsdbg();
			}
		}

		| T_SIGNALSUBSCRIBE uimm uimm
		{
			if (!SCANNING)
			{
				physics_sigsubscr(CUR_STATE, $2, $3);
			}
		}
		| T_SIGNALSRC uimm optstring 
			dimm dimm 	dimm dimm dimm dimm dimm dimm dimm dimm dimm 	
			dimm 	dimm dimm dimm dimm dimm dimm dimm dimm 	
			optstring uimm dimm dimm dimm uimm		
			optstring uimm dimm uimm '\n'
		{
			if (!SCANNING)
			{
				physics_newsigsrc($2, $3, $4, $5, $6, $7, $8, $9, $10,
					$11, $12, $13, $14, $15, $16, $17, $18, $19, $20,
					$21, $22, $23, $24, $25, $26, $27, $28, $29, $30,
					$31, $32, $33);
			}
		}
		| T_PWD '\n'
		{
			if (!SCANNING)
			{
				mprint(NULL, siminfo,
					"Current directory: %s\n", mgetpwd());
			}
		}
		| T_PARSEOBJDUMP T_STRING '\n'
		{
			if (!SCANNING)
			{
				m_parseobjdump(CUR_STATE, $2);
			}
		}
		| T_DUMPALL T_STRING '\n'
		{
			if (!SCANNING)
			{
				m_dumpall($2);
			}
		}
		| T_SETNODE uimm '\n'
		{
			if (!SCANNING)
			{
				if ($2 >= SIM_NUM_NODES)
				{
					merror("Node ID out of range.");
				}
				else
				{
					m_setnode($2);
				}
			}
		}
		| T_MMAP uimm uimm '\n'
		{
			if (!SCANNING)
			{
				if (($2 > SIM_NUM_NODES) || ($2 < 0)\
					|| ($3 > SIM_NUM_NODES) || ($3 < 0))
				{
					merror("Node indeces out of range in call to MMAP!");
				}
				else
				{
					SIM_STATE_PTRS[$3]->MEM  = SIM_STATE_PTRS[$2]->MEM;
					mprint(NULL, siminfo,
						"Mapped mem of Node " ULONGFMT " into Node " ULONGFMT "\n", $2, $3);
				}
			}
		}
		| T_MODECA '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->step = CUR_STATE->cyclestep;
			}
		}
		| T_MODEFF '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->step = CUR_STATE->faststep;
			}
		}
		| T_CACHEINIT uimm uimm uimm '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->cache_init(CUR_STATE, $2, $3, $4);
			}
		}
		| T_CACHEOFF '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->cache_deactivate(CUR_STATE);
			}
		}
		| T_CACHESTATS '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->cache_printstats(CUR_STATE);
			}
		}
		| T_DUMPREGS '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->dumpregs(CUR_STATE);
			}
		}
		| T_DUMPSYSREGS '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->dumpsysregs(CUR_STATE);
			}
		}
		| T_DUMPPIPE '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->dumppipe(CUR_STATE);
			}
		}
		| T_RESETCPU '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->resetcpu(CUR_STATE);
			}
		}
		| T_DYNINSTR '\n'
		{
			if (!SCANNING)
			{
				mprint(CUR_STATE, nodeinfo,
					"Dynamic Instruction Count = [" UVLONGFMT "]\n",
					CUR_STATE->dyncnt);	
			}
		}
		| T_NODETACH uimm '\n'
		{
			if (!SCANNING)
			{
				SIM_NODETACH = $2;
			}
		}
		| T_ADDVALUETRACE T_STRING uimm uimm uimm uimm uimm uimm '\n'
		{
			if (!SCANNING)
			{
				m_addvaluetrace(CUR_STATE, $2, $3, $4, $5, $6, $7, $8);
			}
		}
		| T_DELVALUETRACE T_STRING uimm uimm uimm uimm uimm uimm '\n'
		{
			if (!SCANNING)
			{
				m_delvaluetrace(CUR_STATE, $2, $3, $4, $5, $6, $7, $8);
			}
		}
		| T_REGISTERSTABS T_STRING '\n'
		{
			if (!SCANNING)
			{
				m_readstabs(CUR_STATE, $2);
			}
		}
		| T_VALUESTATS '\n'
		{
			if (!SCANNING)
			{
				m_valuestats(CUR_STATE);
			}
		}
		| T_NUMAREGION T_STRING uimm uimm simm simm simm simm uimm uimm uimm '\n'
		{
			if (!SCANNING)
			{
				m_numaregion($2, $3, $4, $5, $6, $7, $8, $9, $10, $11);
			}
		}
		| T_NUMASTATS '\n'
		{
			if (!SCANNING)
			{
				m_numastats(CUR_STATE);
			}
		}
		| T_NUMASTATSALL '\n'
		{
			if (!SCANNING)
			{
				m_numastatsall();
			}
		}
		| T_NUMASETMAPID uimm uimm '\n'
		{
			if (!SCANNING)
			{
				m_numasetmapid($2, $3);
			}
		}
		| T_DUMPTLB '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->dumptlb(CUR_STATE);
			}
		}
		| T_RATIO '\n'
		{
			if (!SCANNING)
			{
				if (CUR_STATE->ICLK > 0)
				{
					mprint(CUR_STATE, nodeinfo,
						"Ratio of active/sleep cycles = %.6f\n",
						(float)CUR_STATE->CLK/(float)CUR_STATE->ICLK);
				}
				else
				{
					merror("We don't stand a chance.");
				}
			}
		}
		| T_EBATTINTR uimm '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->ENABLE_BATT_LOW_INTR = $2;
			}
		}
		| T_BATTLEAKCURRENT dimm '\n'
		{
			if (!SCANNING)
			{
				SIM_BATT_LEAK_CURRENT = $2;
			}
		}
		| T_BATTALERTFRAC dimm '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->battery_alert_frac = $2;
			}
		}
		| T_NODEFAILPROB dimm '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->fail_prob = $2;
			}
		}
		| T_NODEFAILDURMAX uimm '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->failure_duration_max = $2;
			}
		}
		| T_NETSEGFAILDURMAX uimm uimm '\n'
		{
			if (!SCANNING)
			{
				if ($2 >= MAX_NETSEGMENTS)
				{
					merror("Segment # > max. number of network segments.");
				}
				else
				{
					SIM_NET_SEGMENTS[$2].failure_duration_max = $3;
				}
			}
		}
		| T_NETNEWSEG uimm uimm uimm uimm uimm uimm dimm dimm dimm 
			uimm dimm dimm dimm '\n'
		{
			if (!SCANNING)
			{
				network_netnewseg($2, $3, $4, $5, $6, $7,
					$8, $9, $10, $11, $12, $13, $14);
			}
		}
		| T_NETSEGFAILPROB uimm dimm '\n'
		{
			if (!SCANNING)
			{
				if ($2 >= SIM_NUM_NET_SEGMENTS)
				{
					merror("NETSEGMENT out of range.");
				}
				else
				{
					SIM_NET_SEGMENTS[$2].fail_prob = $3;
				}
			}
		}
		| T_NETSEGNICATTACH uimm uimm '\n'
		{
			if (!SCANNING)
			{
				network_netsegnicattach(CUR_STATE, $2, $3);
			}
		}
		| T_NETCORREL uimm uimm dimm '\n'
		{
			if (!SCANNING)
			{
				if ($2 >= MAX_NETSEGMENTS)
				{
					merror("Segment # > max. number of network segments.");
				}
				else if ($3 >= SIM_NUM_NODES)
				{
					merror("Invalid node number.");
				}
				else
				{
					SIM_NET_SEGMENTS[$2].correl_coeffs[$3] = $4;
				}
			}
		}
		| T_NETNODENEWIFC uimm dimm dimm dimm uimm dimm dimm dimm uimm uimm'\n'
		{
			if (!SCANNING)
			{
				network_netnodenewifc(CUR_STATE, $2, $3, $4, $5,
					$6, $7, $8, $9, $10, $11);
			}
		}
		| T_NETSEGDELETE uimm '\n'
		{
			if (!SCANNING)
			{
				network_netsegdelete($2);
			}
		}
		| T_NODEFAILPROBFN T_STRING '\n'
		{
			if (!SCANNING)
			{
				merror("Command \"NODEFAILPROBFN\" unimplemented.");
			}
		}
		| T_NETSEGFAILPROBFN T_STRING '\n'
		{
			if (!SCANNING)
			{
				merror("Command \"NETSEGFAILPROBFN\" unimplemented.");
			}
		}
		| T_SIZEMEM uimm '\n'
		{
			if (!SCANNING)
			{
				m_sizemem(CUR_STATE, $2);
			}
		}
		| T_SIZEPAU uimm '\n'
		{
			if (!SCANNING)
			{
				pau_init(CUR_STATE, $2);
			}
		}
		| T_SPLIT uimm  uimm uimm T_STRING '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->split(CUR_STATE, $2, $3, $4, $5);
			}
		}
		| T_DUMPMEM uimm uimm '\n'
		{
			if (!SCANNING)
			{
				m_dumpmem(CUR_STATE, $2, $3);
			}
		}
		| T_DISABLEPIPELINE '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->pipelined = 0;
			}
		}
		| T_ENABLEPIPELINE '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->pipelined = 1;
			}
		}
		| T_CLOCKINTR uimm '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->superH->ENABLE_CLK_INTR = $2;
			}
		}
		| T_STOP '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->runnable = 0;
			}
		}
		| T_SFATAL T_STRING '\n'
		{
			if (!SCANNING)
			{
				sfatal(CUR_STATE, $2);
			}
		}
		| T_VERBOSE '\n'
		{
			if (!SCANNING)
			{
				SIM_VERBOSE ^= 1;
			}
		}
		| T_RUN optstring '\n'
		{
			if (!SCANNING)
			{
				m_run(CUR_STATE, $2);
			}

			/*	The 'string' is dynamically allocated, in lex.c		*/
			free($2);
		}
		| T_STEP '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->step(CUR_STATE, 0);
			}
		}
		| T_STEP uimm '\n'
		{
			if (!SCANNING)
			{
				go(CUR_STATE, $2);
			}
		}
		| T_LOAD T_STRING '\n'
		{
			if (!SCANNING)
			{
				loadcmds($2);
			}
		}
		| T_SETPC uimm '\n'
		{	
			if (!SCANNING)
			{
				CUR_STATE->PC = $2;
			}
		}
		| T_SETVDD dimm '\n'
		{
			if (!SCANNING)
			{
				/*	Scale frequency accordingly for provided Vdd	*/
				power_scaledelay(CUR_STATE, $2);
			}
		}
		| T_SETFREQ dimm '\n'
		{
			if (!SCANNING)
			{
				/*	Scale Vdd accordingly for provided frequency	*/
				power_scalevdd(CUR_STATE, $2);
			}
		}
		| T_HELP '\n'
		{
			if (!SCANNING)
			{
				help();
			}
		}
		| T_MAN '\n'
		{
			if (!SCANNING)
			{
				man($1);
			}
		}
		| T_PIPEFLUSH '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->pipeflush(CUR_STATE);
			}
		}
		| T_SHOWPIPE '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->pipeshow = !CUR_STATE->pipeshow;
			}
		}
		| T_SAVE uimm uimm T_STRING '\n'
		{
			if (!SCANNING)
			{
				savemem(CUR_STATE, $2, $3, $4);
			}
		}
		| T_SRECL T_STRING '\n'
		{
			if (!SCANNING)
			{
				load_srec(CUR_STATE, $2);
			}
		}
		| T_CONT uimm '\n'
		{
			if (!SCANNING)
			{
				cont(CUR_STATE, $2);
			}
		}
		| T_SHAREBUS uimm '\n'
		{
			if (!SCANNING)
			{
				m_sharebus(CUR_STATE, $2);
			}
		}
		| T_SHOWCLK '\n'
		{
			if (!SCANNING)
			{
				mprint(NULL, siminfo,
					"CLK = " UVLONGFMT ", ICLK = " UVLONGFMT ", TIME = %E, CYCLETIME = %E\n",
					CUR_STATE->CLK, CUR_STATE->ICLK, CUR_STATE->TIME, 
					CUR_STATE->CYCLETIME);
			}
		}
		| T_POWERSTATS '\n'
		{
			if (!SCANNING)
			{
				power_printstats(CUR_STATE);
			}
		}
		| T_LOCSTATS '\n'
		{
			if (!SCANNING)
			{
				mprint(NULL, siminfo,
					"Location  = [%E][%E][%E]\n",
					CUR_STATE->xloc, CUR_STATE->yloc, CUR_STATE->zloc);
			}
		}
		| T_ON '\n'
		{
			if (!SCANNING)
			{
				m_on(CUR_STATE);
			}
		}
		| T_OFF '\n'
		{
			if (!SCANNING)
			{
				m_off(CUR_STATE);
			}
		}
		| T_NETDEBUG '\n'
		{
			if (!SCANNING)
			{
				network_netdebug(CUR_STATE);
			}
		}
		| T_TRACE uimm '\n'
		{
			if (!SCANNING)
			{
				mprint(NULL, siminfo, 
					"WARNING: Trace only works if compiled w/ -MYRMIGKI_DEBUG\n\n");
				CUR_STATE->trace = $2;
			}
		}
		| T_THROTTLE uimm '\n'
		{
			if (!SCANNING)
			{
				SIM_THROTTLE_NANOSEC = $2;
			}
		}
		| T_THROTTLEWIN uimm '\n'
		{
			if (!SCANNING)
			{
				SIM_THROTTLE_WINDOW = $2;
			}
		}
		| T_FLTTHRESH uimm '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->faultthreshold = $2;
			}
		}
		| T_EFAULTS uimm '\n'
		{
			if (!SCANNING)
			{
				CUR_STATE->ENABLE_TOO_MANY_FAULTS = $2;
			}
		}
		| T_PFUN T_STRING '\n'
		{
			if (!SCANNING)
			{
				fault_setnodepfun(CUR_STATE, $2);
			}
		}
		| T_SETIFCOUI uimm T_STRING '\n'
		{
			if (!SCANNING)
			{
				if ($2 < CUR_STATE->superH->NIC_NUM_IFCS)
				{
					sprintf(&CUR_STATE->superH->NIC_IFCS[$2].IFC_OUI[0],
						"%s", $3);
				}
				else
				{
					merror("Network interface [%d] out of range.", $2);
				}
			}
		}
		| T_MALLOCDEBUG '\n'
		{
			if (!SCANNING)
			{
				mmblocksdisplay();
			}
		}
		| T_PAUINFO '\n'
		{
			if (!SCANNING)
			{
				pau_printstats(CUR_STATE);
			}
		}
		| T_NANOPAUSE uimm '\n'
		{
			if (!SCANNING)
			{
				// TODO: we should account for the cost of the m_on and m_off

				m_off(CUR_STATE);
				mnsleep($2);
				m_on(CUR_STATE);
			}
		}
		| T_PAUSE uimm '\n'
		{
			if (!SCANNING)
			{
				// TODO: we should account for the cost of the m_on and m_off

				m_off(CUR_STATE);
				mnsleep($2 * 1000000000);
				m_on(CUR_STATE);
			}
		}
		| T_COMMENT '\n'
		{
			if (!SCANNING)
			{
				/*	For now, nothing fun is done with comments	*/
			}
		}
		| '\n'
		{
			if (!SCANNING)
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
			/*	Whether SCANNING or not, forcefully align PC 	*/
			/*							*/
			ulong align = $2;

			if (align == 2)
			{
				mprint(NULL, siminfo, "adjusting PC from " ULONGFMT " to " ULONGFMT "\n",
					CUR_STATE->PC, (CUR_STATE->PC+16-(CUR_STATE->PC&0xF)));
				CUR_STATE->PC += 16 - (CUR_STATE->PC & 0xF);
			}
			else if (align == 4)
			{
				mprint(NULL, siminfo, "adjusting PC from " ULONGFMT " to " ULONGFMT "\n",
					CUR_STATE->PC, (CUR_STATE->PC+16-(CUR_STATE->PC&0xF)));
				CUR_STATE->PC += 256 - (CUR_STATE->PC & 0xFF);
			}
			else
			{
				merror(".align for arbitrary alignment not implemented !!!");
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
			long absdisp = ($2 << 1) + CUR_STATE->PC + 4;

			if (SCANNING)
			{
				CUR_STATE->PC += 4;
			}
			else
			{
				mprint(NULL, siminfo,
					"Laying down raw data, val = [" ULONGFMT "]\n", absdisp);
				memmove(&CUR_STATE->MEM[CUR_STATE->PC], &absdisp, sizeof(absdisp));

				/*	.long : 32 bits == 4 bytes	*/
				CUR_STATE->PC += 4;
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
			if (SCANNING)
			{
				char tmp;
				char *ep = &tmp;

				CUR_STATE->PC += strtol($4, &ep, 0);
				
				if (*ep != '\0')
				{
					mprint(NULL, siminfo, "invalid size for .comm variable");
				}	
			}
		}
		;

		
add_instr	: T_ADD reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)	
				{
					superH_add(CUR_STATE, $2, $4);
				}
		
				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1100;
				tmp.code_hi = B0011;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

addi_instr	: T_ADD '#' simm ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_ni tmp;

				if (!CUR_STATE->pipelined)	
				{
					superH_addi(CUR_STATE, $3, $5);
				}
		
				tmp.imm = ($3&B11111111);
				tmp.dst = ($5&B1111);
				tmp.code = B0111;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
				
			}
		}
		;

addc_instr	: T_ADDC reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_addc(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1110;
				tmp.code_hi = B0011;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

addv_instr	: T_ADDV reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_addv(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1111;
				tmp.code_hi = B0011;
			
				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

and_instr	: T_AND reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_and(CUR_STATE, $2, $4);
				}
	
				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1001;
				tmp.code_hi = B0010;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

andi_instr	: T_AND '#' simm ',' T_R0
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_andi(CUR_STATE, $3);
				}
	
				tmp.imm = ($3&B11111111);
				tmp.code = B11001001;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

andm_instr	: T_ANDB '#' simm ',' '@''(' T_R0 ',' T_GBR ')'
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_andm(CUR_STATE, $3);
				}
			
				tmp.imm = ($3&B11111111);
				tmp.code = B11001101;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

bf_instr	: T_BF disp
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
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

				if (!CUR_STATE->pipelined)
				{
					superH_bf(CUR_STATE, $2 - CUR_STATE->PC);
				}

				tmp.disp = ($2&B11111111);
				tmp.code = B10001011;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

bfs_instr	: T_BFS disp
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
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

				if (!CUR_STATE->pipelined)
				{
					superH_bfs(CUR_STATE, $2 - CUR_STATE->PC);
				}
	
				tmp.disp = ($2&B11111111);
				tmp.code = B10001111;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

bra_instr	: T_BRA disp
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_d12 tmp;

				if (!CUR_STATE->pipelined)
				{
					/*						*/
					/*	The disp is absolute, and we have to	*/
					/*	pass PC relative to bra(). No need to	*/
					/*	PC-2 as in main.c, since no pipelining	*/
					/*						*/
					superH_bra(CUR_STATE, $2 - CUR_STATE->PC);
				}

				tmp.disp = $2;
				tmp.code = B1010;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

braf_instr	: T_BRA reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;
	
				if (!CUR_STATE->pipelined)
				{
					superH_braf(CUR_STATE, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00100011;
				tmp.code_hi = B0000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

bsr_instr	: T_BSR disp
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d12 tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_bsr(CUR_STATE, $2 - CUR_STATE->PC);
				}

				tmp.disp = ($2&B111111111111);
				tmp.code = B1011;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

bsrf_instr	: T_BSRF reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_bsrf(CUR_STATE, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00000011;
				tmp.code_hi = B0000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

bt_instr	: T_BT disp
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
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

				if (!CUR_STATE->pipelined)
				{
					superH_bt(CUR_STATE, $2 - CUR_STATE->PC);
				}

				tmp.disp = ($2&B11111111);
				tmp.code = B10001001;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

bts_instr:	T_BTS disp
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
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

				if (!CUR_STATE->pipelined)
				{
					superH_bts(CUR_STATE, $2 - CUR_STATE->PC);
				}

				tmp.disp = ($2&B11111111);
				tmp.code = B10001101;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

clrmac_instr	: T_CLRMAC
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_clrmac(CUR_STATE);
				}

				tmp.code = B0000000000101000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

clrs_instr	: T_CLRS
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_clrs(CUR_STATE);
				}

				tmp.code = B0000000001001000;
			
				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

clrt_instr	: T_CLRT
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_clrt(CUR_STATE);
				}

				tmp.code = B0000000000001000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

cmpeq_instr	: T_CMPEQ reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_cmpeq(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B0000;
				tmp.code_hi = B0011;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

cmpge_instr	: T_CMPGE reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_cmpge(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B0011;
				tmp.code_hi = B0011;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

cmpgt_instr	: T_CMPGT reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_cmpgt(CUR_STATE, $2, $4);
				}
	
				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B0111;
				tmp.code_hi = B0011;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

cmphi_instr	: T_CMPHI reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_cmphi(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B0110; 
				tmp.code_hi = B0011;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

cmphs_instr	: T_CMPHS reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_cmphs(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B0010;
				tmp.code_hi = B0011;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

cmppl_instr	: T_CMPPL reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_cmppl(CUR_STATE, $2);
				}
				tmp.dst = ($2&B111);
				tmp.code_lo = B00010101;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

cmppz_instr	: T_CMPPZ reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_cmppz(CUR_STATE, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00010001;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

cmpstr_instr	: T_CMPSTR reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_cmpstr(CUR_STATE, $2, $4);
				}
		
				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1100;
				tmp.code_hi = B0010;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

cmpim_instr	: T_CMPEQ '#' simm ',' T_R0
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_cmpim(CUR_STATE, $3);
				}

				tmp.imm = ($3&B11111111);
				tmp.code = B10001000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

div0s_instr	: T_DIV0S reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_div0s(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B0111;
				tmp.code_hi = B0010;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

div1_instr	: T_DIV1 reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_div1(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B0100;
				tmp.code_hi = B0011;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

dmuls_instr	: T_DMULSL reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_dmuls(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1101;
				tmp.code_hi = B0011;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

dmulu_instr	: T_DMULUL reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_dmulu(CUR_STATE, $2, $4);
				}
			
				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B0101;
				tmp.code_hi = B0011;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

dt_instr	: T_DT reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_dt(CUR_STATE, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00010000;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

extsb_instr	: T_EXTSB reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_extsb(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1110;
				tmp.code_hi = B0110;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

extsw_instr	: T_EXTSW reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_extsw(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1111;
				tmp.code_hi = B0110;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

extub_instr	: T_EXTUB reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_extub(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1100;
				tmp.code_hi = B0110;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

extuw_instr	: T_EXTUW reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_extuw(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1101;
				tmp.code_hi = B0110;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

jmp_instr	: T_JMP '@' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_jmp(CUR_STATE, $3);
				}

				tmp.dst = ($3&B1111);
				tmp.code_lo = B00101011;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

jsr_instr	: T_JSR '@' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_jsr(CUR_STATE, $3);
				}

				tmp.dst = ($3&B1111);
				tmp.code_lo = B00001011;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

ldcsr_instr	: T_LDC reg ',' T_SR
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_ldcsr(CUR_STATE, $2);
				}

				tmp.src = ($2&B1111);
				tmp.code_lo = B00001110;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

ldcgbr_instr	: T_LDC reg ',' T_GBR
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_ldcgbr(CUR_STATE, $2);
				}

				tmp.src = ($2&B1111);
				tmp.code_lo = B00011110;
				tmp.code_hi = B0100;
			
				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

ldcvbr_instr	: T_LDC reg ',' T_VBR
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_ldcvbr(CUR_STATE, $2);
				}

				tmp.src = ($2&B1111);
				tmp.code_lo = B00101110;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

ldcssr_instr	: T_LDC reg ',' T_SSR
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_ldcssr(CUR_STATE, $2);
				}

				tmp.src = ($2&B1111);
				tmp.code_lo = B00111110;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

ldcspc_instr	: T_LDC reg ',' T_SPC
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_ldcspc(CUR_STATE, $2);
				}

				tmp.src = ($2&B1111);
				tmp.code_lo = B01001110;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

ldcr_bank_instr	: T_LDC reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_mbank tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_ldcr_bank(CUR_STATE, $4, $2);
				}
		
				tmp.code_lo = B1110;
				tmp.reg = ($4&B111);
				tmp.fill = 1;
				tmp.src = ($2&B1111);
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

ldcmsr_instr	: T_LDCL '@'reg'+'',' T_SR
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_ldcmsr(CUR_STATE, $3);
				}

				tmp.src = ($3&B1111);
				tmp.code_lo = B00000111;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

ldcmgbr_instr	: T_LDCL '@'reg'+'',' T_GBR
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else
			{
				instr_m tmp;
	
				if (!CUR_STATE->pipelined)
				{
					superH_ldcmgbr(CUR_STATE, $3);
				}

				tmp.src = ($3&B1111);
				tmp.code_lo = B00010111;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

ldcmvbr_instr	: T_LDCL '@'reg'+'',' T_VBR
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_ldcmvbr(CUR_STATE, $3);
				}

				tmp.src = ($3&B1111);
				tmp.code_lo = B00100111;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

ldcmssr_instr	: T_LDCL '@'reg'+'',' T_SSR
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m  tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_ldcmssr(CUR_STATE, $3);
				}

				tmp.src = ($3&B1111);
				tmp.code_lo = B00110111;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

ldcmspc_instr	: T_LDCL '@'reg'+'',' T_SPC
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_ldcmspc(CUR_STATE, $3);
				}

				tmp.src = ($3&B1111);
				tmp.code_lo = B01000111;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

ldcmr_bank_instr: T_LDCL '@' reg '+' ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_mbank tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_ldcmr_bank(CUR_STATE, $6, $3);
				}

				tmp.code_lo = B0111;
				tmp.reg = ($6&B111);
				tmp.fill = 1;
				tmp.src = ($3&B1111);
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

ldsmach_instr	: T_LDS reg ',' T_MACH
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_ldsmach(CUR_STATE, $2);
				}

				tmp.src = ($2&B1111);
				tmp.code_lo = B00001010;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

ldsmacl_instr	: T_LDS reg ',' T_MACL
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_ldsmacl(CUR_STATE, $2);
				}
	
				tmp.src = ($2&B1111);
				tmp.code_lo = B00011010;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

ldspr_instr	: T_LDS reg ',' T_PR
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_ldspr(CUR_STATE, $2);
				}

				tmp.src = ($2&B1111);
				tmp.code_lo = B00101010;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

ldsmmach_instr	: T_LDSL '@' reg '+' ',' T_MACH
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else
			{
				instr_m tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_ldsmmach(CUR_STATE, $3);
				}

				tmp.src = ($3&B1111);
				tmp.code_lo = B00000110;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

ldsmmacl_instr	: T_LDSL '@' reg '+' ',' T_MACL
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_ldsmmacl(CUR_STATE, $3);
				}

				tmp.src = ($3&B1111);
				tmp.code_lo = B00010110;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

ldsmpr_instr	: T_LDSL '@' reg '+' ',' T_PR
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_ldsmpr(CUR_STATE, $3);
				}

				tmp.src = ($3&B1111);
				tmp.code_lo = B00100110;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

ldtlb_instr	: T_LDTLB
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_ldtlb(CUR_STATE);
				}

				tmp.code = B0000000000111000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

macl_instr	: T_MACL '@' reg '+' ',' '@' reg '+'
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_macl(CUR_STATE, $3, $7);
				}

				tmp.src = ($3&B1111);
				tmp.dst = ($7&B1111);
				tmp.code_lo = B1111;
				tmp.code_hi = B0000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

macw_instr	: T_MACW '@' reg '+' ',' '@' reg '+'
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_macw(CUR_STATE, $3, $7);
				}

				tmp.src = ($3&B1111);
				tmp.dst = ($7&B1111);
				tmp.code_lo = B1111;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

mov_instr	: T_MOV reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_mov(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B0011;
				tmp.code_hi = B0110;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movbs_instr	: T_MOVB reg ',' '@' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_movbs(CUR_STATE, $2, $5);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($5&B1111);
				tmp.code_lo = B0000;
				tmp.code_hi = B0010;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movws_instr	: T_MOVW reg ',' '@' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_movws(CUR_STATE, $2, $5);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($5&B1111);
				tmp.code_lo = B0001;
				tmp.code_hi = B0010;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movls_instr	: T_MOVL reg ',' '@' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_movls(CUR_STATE, $2, $5);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($5&B1111);
				tmp.code_lo = B0010;
				tmp.code_hi = B0010;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movbl_instr	: T_MOVB '@' reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_movbl(CUR_STATE, $3, $5);
				}

				tmp.src = ($3&B1111);
				tmp.dst = ($5&B1111);
				tmp.code_lo = B0000;
				tmp.code_hi = B0110;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movwl_instr	: T_MOVW '@' reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_movwl(CUR_STATE, $3, $5);
				}

				tmp.src = ($3&B1111);
				tmp.dst = ($5&B1111);
				tmp.code_lo = B0001;
				tmp.code_hi = B0110;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movll_instr	: T_MOVL '@' reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_movll(CUR_STATE, $3, $5);
				}

				tmp.src = ($3&B1111);
				tmp.dst = ($5&B1111);
					tmp.code_lo = B0010;
				tmp.code_hi = B0110;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movbm_instr	: T_MOVB reg ',' '@' '-' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_movbm(CUR_STATE, $2, $6);
				}
	
				tmp.src = ($2&B1111);
				tmp.dst = ($6&B1111);
				tmp.code_lo = B0100;
				tmp.code_hi = B0010;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movwm_instr	: T_MOVW reg ',' '@' '-' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_movwm(CUR_STATE, $2, $6);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($6&B1111);
				tmp.code_lo = B0101;
				tmp.code_hi = B0010;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movlm_instr	: T_MOVL reg ',' '@' '-' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_movlm(CUR_STATE, $2, $6);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($6&B1111);
				tmp.code_lo = B0110;
				tmp.code_hi = B0010;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movbp_instr	: T_MOVB '@' reg '+' ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_movbp(CUR_STATE, $3, $6);
				}

				tmp.src = ($3&B1111);
				tmp.dst = ($6&B1111);
				tmp.code_lo = B0100;
				tmp.code_hi = B0110;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movwp_instr	: T_MOVW '@' reg '+' ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_movwp(CUR_STATE, $3, $6);
				}

				tmp.src = ($3&B1111);
				tmp.dst = ($6&B1111);
				tmp.code_lo = B0101;
				tmp.code_hi = B0110;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movlp_instr	: T_MOVL '@' reg '+' ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_movlp(CUR_STATE, $3, $6);
				}

				tmp.src = ($3&B1111);
				tmp.dst = ($6&B1111);
				tmp.code_lo = B0110;
				tmp.code_hi = B0110;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movbs0_instr	: T_MOVB reg ',' '@' '(' T_R0 ',' reg ')'
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_movbs0(CUR_STATE, $2, $8);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($8&B1111);
				tmp.code_lo = B0100;
				tmp.code_hi = B0000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movws0_instr	: T_MOVW reg ',' '@' '(' T_R0 ',' reg ')'
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_movws0(CUR_STATE, $2, $8);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($8&B1111);
				tmp.code_lo = B0101;
				tmp.code_hi = B0000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movls0_instr	: T_MOVL reg ',' '@' '(' T_R0 ',' reg ')'
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_movls0(CUR_STATE, $2, $8);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($8&B1111);
				tmp.code_lo = B0110;
				tmp.code_hi = B0000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movbl0_instr	: T_MOVB '@' '(' T_R0 ',' reg ')' ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_movbl0(CUR_STATE, $6, $9);
				}
	
				tmp.src = ($6&B1111);
				tmp.dst = ($9&B1111);
				tmp.code_lo = B1100;
				tmp.code_hi = B0000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movwl0_instr	: T_MOVW '@' '(' T_R0 ',' reg ')' ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_movwl0(CUR_STATE, $6, $9);
				}

				tmp.src = ($6&B1111);
				tmp.dst = ($9&B1111);
				tmp.code_lo = B1101;
				tmp.code_hi = B0000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}

		}
		;

movll0_instr	: T_MOVL '@' '(' T_R0 ',' reg ')' ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_movll0(CUR_STATE, $6, $9);
				}

				tmp.src = ($6&B1111);
				tmp.dst = ($9&B1111);
				tmp.code_lo = B1110;
				tmp.code_hi = B0000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movi_instr	: T_MOV '#' simm ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_ni tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_movi(CUR_STATE, $3, $5);
				}

				tmp.imm = ($3&B11111111);
				tmp.dst = ($5&B1111);
				tmp.code = B1110;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movwi_instr	: T_MOVW '@' '(' disp ',' T_PC ')' ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
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

				if (!CUR_STATE->pipelined)
				{
					superH_movwi(CUR_STATE, $4 - CUR_STATE->PC, $9);
				}

				tmp.dst = ($9&B11111111);
				tmp.disp = ($4&B11111111);
				tmp.code = B1001;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;


		/*	Original version from spec	*/
movli_instr	: T_MOVL '@' '(' disp ',' T_PC ')' ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
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

				if (!CUR_STATE->pipelined)
				{
					superH_movli(CUR_STATE, $4 - CUR_STATE->PC, $9);
				}

				tmp.dst = ($9&B11111111);
				tmp.disp = ($4&B11111111);
				tmp.code = B1101;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		/*	Added to handle "mov.l LABEL, reg" that gcc emits	*/
		| T_MOVL disp ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
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

				if (!CUR_STATE->pipelined)
				{
					superH_movli(CUR_STATE, $2 - CUR_STATE->PC, $4);
				}

				tmp.dst = ($4&B11111111);
				tmp.disp = ($2&B11111111);
				tmp.code = B1101;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movblg_instr	: T_MOVB '@' '(' disp ',' T_GBR ')' ',' T_R0
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
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

				if (!CUR_STATE->pipelined)
				{
					superH_movblg(CUR_STATE, $4 - CUR_STATE->PC);
				}

				tmp.disp = ($4&B11111111);
				tmp.code = B11000100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movwlg_instr	: T_MOVW '@' '(' disp ',' T_GBR ')' ',' T_R0
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
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

				if (!CUR_STATE->pipelined)
				{
					superH_movwlg(CUR_STATE, $4 - CUR_STATE->PC);
				}

				tmp.disp = ($4&B11111111);
				tmp.code = B11000101;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movllg_instr	: T_MOVL '@' '(' disp ',' T_GBR ')' ',' T_R0
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
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

				if (!CUR_STATE->pipelined)
				{
					superH_movllg(CUR_STATE, $4 - CUR_STATE->PC);
				}

				tmp.disp = ($4&B11111111);
				tmp.code = B11000110;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movbsg_instr	: T_MOVB T_R0 ',' '@' '(' disp ',' T_GBR ')'
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
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

				if (!CUR_STATE->pipelined)
				{
					superH_movbsg(CUR_STATE, $6 - CUR_STATE->PC);
				}

				tmp.disp = ($6&B11111111);
				tmp.code = B11000000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movwsg_instr	: T_MOVW T_R0 ',' '@' '(' disp ',' T_GBR ')'
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
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

				if (!CUR_STATE->pipelined)
				{
					superH_movwsg(CUR_STATE, $6 - CUR_STATE->PC);
				}

				tmp.disp = ($6&B11111111);
				tmp.code = B11000001;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movlsg_instr	: T_MOVL T_R0 ',' '@' '(' disp ',' T_GBR ')'
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
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

				if (!CUR_STATE->pipelined)
				{
					superH_movlsg(CUR_STATE, $6 - CUR_STATE->PC);
				}

				tmp.disp = ($6&B11111111);
				tmp.code = B11000010;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movbs4_instr	: T_MOVB T_R0 ',' '@' '(' disp ',' reg ')'
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
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

				if (!CUR_STATE->pipelined)
				{
					superH_movbs4(CUR_STATE, $6 - CUR_STATE->PC, $8);
				}
	
				tmp.dst = ($8&B1111);
				tmp.disp = ($6&B1111);
				tmp.code = B10000000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movws4_instr	: T_MOVW T_R0 ',' '@' '(' disp ',' reg ')'
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
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

				if (!CUR_STATE->pipelined)
				{
					superH_movws4(CUR_STATE, $6 - CUR_STATE->PC, $8);
				}

				tmp.dst = ($8&B1111);
				tmp.disp = ($6&B1111);
				tmp.code = B10000001;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movls4_instr	: T_MOVL reg ',' '@' '(' disp ',' reg ')'
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
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

				if (!CUR_STATE->pipelined)
				{
					superH_movls4(CUR_STATE, $2, $6 - CUR_STATE->PC, $8);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($8&B1111);
				tmp.disp = ($6&B1111);
				tmp.code = B0001;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movbl4_instr	: T_MOVB '@' '(' disp ',' reg ')' ',' T_R0
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
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

				if (!CUR_STATE->pipelined)
				{
					superH_movbl4(CUR_STATE, $4 - CUR_STATE->PC, $6);
				}

				tmp.src = ($6&B1111);
				tmp.disp = ($4&B1111);
				tmp.code = B10000100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movwl4_instr	: T_MOVW '@' '(' disp ',' reg ')' ',' T_R0
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
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

				if (!CUR_STATE->pipelined)
				{
					superH_movwl4(CUR_STATE, $4 - CUR_STATE->PC, $6);
				}

				tmp.src = ($6&B1111);
				tmp.disp = ($4&B1111);
				tmp.code = B10000101;
	
				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movll4_instr	: T_MOVL '@' '(' disp ',' reg ')' ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
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

				if (!CUR_STATE->pipelined)
				{
					superH_movll4(CUR_STATE, $6, $4 - CUR_STATE->PC, $9);
				}

				tmp.src = ($6&B1111);
				tmp.dst = ($9&B1111);
				tmp.disp = ($4&B1111);
				tmp.code = B0101;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

mova_instr	: T_MOVA '@' '(' disp ',' T_PC ')' ',' T_R0
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
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

				if (!CUR_STATE->pipelined)
				{
					superH_mova(CUR_STATE, $4 - CUR_STATE->PC);
				}

				tmp.disp = ($4&B11111111);
				tmp.code = B11000111;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

movt_instr	: T_MOVT reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_movt(CUR_STATE, $2);
				}
		
				tmp.dst = ($2&B1111);
				tmp.code_lo = B00101001;
				tmp.code_hi = B0000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

mull_instr	: T_MULL reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_mull(CUR_STATE, $2, $4);
				}
		
				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B0111;
				tmp.code_hi = B0000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

muls_instr	: T_MULS reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_muls(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1111;
				tmp.code_hi = B0010;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

muls_instr	: T_MULSW reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_muls(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1111;
				tmp.code_hi = B0010;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

mulu_instr	: T_MULU reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_mulu(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1110;
				tmp.code_hi = B0010;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

muluw_instr	: T_MULUW reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_mulu(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1110;
				tmp.code_hi = B0010;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

neg_instr	: T_NEG reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_neg(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1011;
				tmp.code_hi = B0110;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

negc_instr	: T_NEGC reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_negc(CUR_STATE, $2, $4);
				}
			
				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1010;
				tmp.code_hi = B0110;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

nop_instr	: T_NOP
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_nop(CUR_STATE);
				}

				tmp.code = B0000000000001001;
			
				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

not_instr	: T_NOT reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_not(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B0111;
				tmp.code_hi = B0110;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

or_instr	: T_OR reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_or(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1011;
				tmp.code_hi = B0010;
	
				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

ori_instr	: T_OR '#' simm ',' T_R0
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_ori(CUR_STATE, $3);
				}

				tmp.imm = ($3&B11111111);
				tmp.code = B11001011;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

orm_instr	: T_ORB '#' simm ',' '@' '(' T_R0 ',' T_GBR ')'
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_orm(CUR_STATE, $3);
				}

				tmp.imm = ($3&B11111111);
				tmp.code = B11001111;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

pref_instr	: T_PREF '@' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_pref(CUR_STATE, $3);
				}

				tmp.dst = ($3&B1111);
				tmp.code_lo = B10000011;
				tmp.code_hi = B0000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

rfg_instr	: T_RFG '#' simm
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_rfg(CUR_STATE, $3);
				}

				tmp.imm = ($3&B11111111);
				tmp.code = B11110000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;
rotcl_instr	: T_ROTCL reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_rotcl(CUR_STATE, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00100100;
				tmp.code_hi =  B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

rotcr_instr	: T_ROTCR reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_rotcr(CUR_STATE, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00100101;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

rotl_instr	: T_ROTL reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_rotl(CUR_STATE, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00000100;
				tmp.code_hi = B0100;
	
				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

rotr_instr	: T_ROTR reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_rotr(CUR_STATE, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00000101;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

rte_instr	: T_RTE
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_rte(CUR_STATE);
				}

				tmp.code = B0000000000101011;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

rts_instr	: T_RTS
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_rts(CUR_STATE);
				}

				tmp.code = B0000000000001011;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

sets_instr	: T_SETS
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_sets(CUR_STATE);
				}

				tmp.code = B0000000001011000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

sett_instr	: T_SETT
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_sett(CUR_STATE);
				}

				tmp.code = B0000000000011000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

shad_instr	: T_SHAD reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_shad(CUR_STATE, $2, $4);
				}
	
				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1100; 
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

shal_instr	: T_SHAL reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_shal(CUR_STATE, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00100000;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

shar_instr	: T_SHAR reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_shar(CUR_STATE, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00100001;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;	
			}
		}
		;

shld_instr	: T_SHLD reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_shld(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1101;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

shll_instr	: T_SHLL reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_shll(CUR_STATE, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00000000;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

shll2_instr	: T_SHLL2 reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_shll2(CUR_STATE, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00001000;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

shll8_instr	: T_SHLL8 reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_shll8(CUR_STATE, $2);
				}
		
				tmp.dst = ($2&B1111);
				tmp.code_lo = B00011000;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

shll16_instr	: T_SHLL16 reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_shll16(CUR_STATE, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00101000;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

shlr_instr	: T_SHLR reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_shlr(CUR_STATE, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00000001;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

shlr2_instr	: T_SHLR2 reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_shlr2(CUR_STATE, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00001001;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

shlr8_instr	: T_SHLR8 reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_shlr8(CUR_STATE, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00011001;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

shlr16_instr	: T_SHLR16 reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_shlr16(CUR_STATE, $2);
				}

				tmp.dst = ($2&B1111);
				tmp.code_lo = B00101001;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

sleep_instr	: T_SLEEP
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_sleep(CUR_STATE);
				}

				tmp.code = B0000000000011011;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

stcsr_instr	: T_STC T_SR ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_stcsr(CUR_STATE, $4);
				}

				tmp.dst = ($4&B1111);
				tmp.code_lo = B00000010;
				tmp.code_hi = B0000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

stcgbr_instr	: T_STC T_GBR ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_stcgbr(CUR_STATE, $4);
				}
			
				tmp.dst = ($4&B1111);
				tmp.code_lo = B00010010;
				tmp.code_hi = B0000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

stcvbr_instr	: T_STC T_VBR ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_stcvbr(CUR_STATE, $4);
				}

				tmp.dst = ($4&B1111);
				tmp.code_lo = B00100010;
				tmp.code_hi = B0000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

stcssr_instr	: T_STC T_SSR ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_stcssr(CUR_STATE, $4);
				}

				tmp.dst = ($4&B1111);
				tmp.code_lo = B00110010;
				tmp.code_hi = B0000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

stcspc_instr	: T_STC T_SPC ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_stcspc(CUR_STATE, $4);
				}

				tmp.dst = ($4&B1111);
				tmp.code_lo = B01000010;
				tmp.code_hi = B0000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

stcr_bank_instr	: T_STC reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nbank tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_stcr_bank(CUR_STATE, $2, $4);
				}

				tmp.code_lo = B0010;
				tmp.reg = ($2&B111);
				tmp.fill = 1;
				tmp.dst = ($4&B1111);
				tmp.code_hi = B0000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

stcmsr_instr	: T_STCL T_SR ',' '@' '-' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_stcmsr(CUR_STATE, $6);
				}

				tmp.dst = ($6&B1111);
				tmp.code_lo = B00000011;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

stcmgbr_instr	: T_STCL T_GBR ',' '@' '-' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_stcmgbr(CUR_STATE, $6);
				}

				tmp.dst = ($6&B1111);
				tmp.code_lo = B00010011;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

stcmvbr_instr	: T_STCL T_VBR ',' '@' '-' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_stcmvbr(CUR_STATE, $6);
				}

				tmp.dst = ($6&B1111);
				tmp.code_lo = B00100011;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

stcmssr_instr	: T_STCL T_SSR ',' '@' '-' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_stcmssr(CUR_STATE, $6);
				}

				tmp.dst = ($6&B1111);
				tmp.code_lo = B00110011;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

stcmspc_instr	: T_STCL T_SPC ',' '@' '-' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_stcmspc(CUR_STATE, $6);
				}

				tmp.dst = ($6&B1111);
				tmp.code_lo = B01000011;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

stcmr_bank_instr: T_STCL reg ',' '@' '-' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nbank tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_stcmr_bank(CUR_STATE, $2, $6);
				}

				tmp.code_lo = B0011;
				tmp.reg = ($2&B111);
				tmp.fill = 1;
				tmp.dst = ($6&B1111);
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

stsmach_instr	: T_STS T_MACH ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_stsmach(CUR_STATE, $4);
				}

				tmp.dst = ($4&B1111);
				tmp.code_lo = B00001010;
				tmp.code_hi = B0000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

stsmacl_instr	: T_STS T_MACL ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_stsmacl(CUR_STATE, $4);
				}

				tmp.dst = ($4&B1111);
				tmp.code_lo = B00011010;
				tmp.code_hi = B0000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

stspr_instr	: T_STS T_PR ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_stspr(CUR_STATE, $4);
				}

				tmp.dst = ($4&B1111);
				tmp.code_lo = B00101010;
				tmp.code_hi = B0000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

stsmmach_instr	: T_STSL T_MACH ',' '@' '-' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_stsmmach(CUR_STATE, $6);
				}

				tmp.dst = ($6&B1111);
				tmp.code_lo = B00000010;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

stsmmacl_instr	: T_STSL T_MACL ',' '@' '-' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_stsmmacl(CUR_STATE, $6);
				}

				tmp.dst = ($6&B1111);
				tmp.code_lo = B00010010;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

stsmpr_instr	: T_STSL T_PR ',' '@' '-' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_stsmpr(CUR_STATE, $6);
				}

				tmp.dst = ($6&B1111);
				tmp.code_lo = B00100010;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

sub_instr	: T_SUB reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_sub(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1000;
				tmp.code_hi = B0011;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

subc_instr	: T_SUBC reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_subc(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1010;
				tmp.code_hi = B0011;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

subv_instr	: T_SUBV reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_subv(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1011;
				tmp.code_hi = B0011;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

swapb_instr	: T_SWAPB reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_swapb(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1000;
				tmp.code_hi = B0110;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

swapw_instr	: T_SWAPW reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_swapw(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1001;
				tmp.code_hi = B0110;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

tas_instr	: T_TASB '@' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_tas(CUR_STATE, $3);
				}

				tmp.dst = ($3&B1111);
				tmp.code_lo = B00011011;
				tmp.code_hi = B0100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

trapa_instr	: T_TRAPA '#' simm
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_trapa(CUR_STATE, $3);
				}

				tmp.imm = ($3&B11111111);
				tmp.code = B11000011;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

tst_instr	: T_TST reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_tst(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1000;
				tmp.code_hi = B0010;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

tsti_instr	: T_TST '#' simm ',' T_R0
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_tsti(CUR_STATE, $3);
				}

				tmp.imm = ($3&B11111111);
				tmp.code = B11001000;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

tstb_instr	: T_TSTB '#' simm ',''@''('T_R0','T_GBR')'
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_tstm(CUR_STATE, $3);
				}

				tmp.imm = ($3&B11111111);
				tmp.code = B11001100;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

xor_instr	: T_XOR reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_xor(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1010;
				tmp.code_hi = B0010;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

xori_instr	: T_XOR '#' simm ',' T_R0
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_xori(CUR_STATE, $3);
				}

				tmp.imm = ($3&B11111111);
				tmp.code = B11001010;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

xorm_instr	: T_XORB '#' simm ',''@''('T_R0','T_GBR')'
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_xorm(CUR_STATE, $3);
				}

				tmp.imm = ($3&B11111111);
				tmp.code = B11001110;
	
				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;

xtrct_instr	: T_XTRCT reg ',' reg
		{
			if (SCANNING)
			{
				CUR_STATE->PC += 2;
			}
			else if (CUR_STATE->machinetype != MACHINE_SUPERH)
			{
				mprint(NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!CUR_STATE->pipelined)
				{
					superH_xtrct(CUR_STATE, $2, $4);
				}

				tmp.src = ($2&B1111);
				tmp.dst = ($4&B1111);
				tmp.code_lo = B1101;
				tmp.code_hi = B0010;

				memmove(&CUR_STATE->MEM[CUR_STATE->PC - CUR_STATE->MEMBASE],
					&tmp, sizeof(tmp));
				CUR_STATE->PC += 2;
			}
		}
		;


uimm		: T_STRING
		{
			char tmp;
			char *ep = &tmp;

			$$ = strtoul($1, &ep, 0);
			
			if (*ep != '\0')
			{
				merror("Invalid unsigned immediate data %s.", $1);
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
				merror("Invalid signed immediate data %s.", $1);
			}
		}
		| '-' T_STRING
		{
			char tmp;
			char *ep = &tmp;

			$$ = -(strtol($2, &ep, 0));
				
			if (*ep != '\0')
			{
				merror("Invalid signed immediate data %s.", $2);
			}
		}
		| '+' T_STRING
		{
			char tmp;
			char *ep = &tmp;

			$$ = +(strtol($2, &ep, 0));
				
			if (*ep != '\0')
			{
				merror("Invalid signed immediate data %s.", $2);
			}
		}
		;

dimm	: T_STRING
		{
			char tmp;
			char *ep = &tmp;

			$$ = strtod($1, &ep);
			
			if (*ep != '\0')
			{
				merror("Invalid double immediate data %s.", $1);
			}
		}
		| '-' T_STRING
		{
			char tmp;
			char *ep = &tmp;

			$$ = -(strtod($2, &ep));
				
			if (*ep != '\0')
			{
				merror("Invalid double immediate data %s.", $2);
			}
		}
		| '+' T_STRING
		{
			char tmp;
			char *ep = &tmp;

			$$ = +(strtod($2, &ep));
				
			if (*ep != '\0')
			{
				merror("Invalid double immediate data %s.", $2);
			}
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
			$$ = ($1 - (CUR_STATE->PC+4)) >> 1;

			/*	Warn if the PC calculated displacement was not a multiple of 2 */
			if (($1 - (CUR_STATE->PC+4)) & 0x1)
			{
				merror("Disp in terms of mem addr was not on word boundary.");
			}
		}
		| T_STRING
		{
			char tmp;
			char *ep = &tmp;


			$$ = strtol($1, &ep, 0);
				
			if (*ep != '\0')
			{
				merror("Invalid DISP ([%s]). Possibly due to a .comm.", $1);
			}
		}
		| '+' T_STRING
		{
			char tmp;
			char *ep = &tmp;


			$$ = +(strtol($2, &ep, 0));
				
			if (*ep != '\0')
			{
				merror("Invalid DISP ([%s]). Possibly due to a .comm.", $2);
			}
		}
		| '-' T_STRING
		{
			char tmp;
			char *ep = &tmp;


			$$ = -(strtol($2, &ep, 0));
				
			if (*ep != '\0')
			{
				merror("Invalid DISP ([%s]). Possibly due to a .comm.", $2);
			}
		}
		;


optstring	: {
			/*								*/
			/*	To make handling of empty string in T_RUN rule 		*/
			/*	uniform, we malloc a place holder.			*/
			/*								*/
			char *tmp = mmalloc(sizeof(char), "opstring rule in shasm.y");
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

#include "lex.c"

int
yyerror(char *err)
{
	merror("Invalid command!");
	clearistream();
	
	return 0;
}
