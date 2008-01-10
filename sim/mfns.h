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
int	yylex(void);
int	yyerror(char *);
int	yyparse(void);

ulong	reg_read(State *S, int n);
void	reg_set(State *S, int n, ulong data);

void	mfree(void *ptr, char *ID);
void	mmblocksdisplay(void);
void*	mcalloc(int nelem, int size, char *ID);
void*	mmalloc(int size, char *ID);
void*	mrealloc(void *oldptr, int size, char *ID);
void	mstatelock(void);
void	mstateunlock(void);

void	network_netseg2file(int which, char *filename);
void	network_file2netseg(char *file, int whichseg);
void	network_netsegpropmodel(int whichseg, int whichmodel, double minsnr);
void	network_netnewseg(int which, int framebits, int pgnspeed, int bitrate, int width, 
		int fpdist, double fpdistmu, double fpdistsigma, double fpdistlambda,
		int fddist, double fddistmu, double fddistsigma, double fddistlambda);
void	network_netsegnicattach(State *S, int whichifc, int whichseg);
void	network_netnodenewifc(
	State *S, int which, double txpwr, double rxpwr, double idlepwr,
	int fpdist, int fpdistmu, int fpdistsigma, int fpdistlambda,
	int txfifosz, int rxfifosz);
void	network_netsegdelete(int whichseg);
void	network_netdebug(State *S);
void	network_setretryalg(State *S, int which, char *alg);

void	m_init(void);
void	m_dumpall(char *filename);
void	m_version(void);
void	m_newnode(char *type, double x, double y, double z, double orbit, double velocity);
void	m_powertotal(void);
void	m_renumbernodes(void);
void	m_run(State *S, char *args);
void	m_on(State *S);
void	m_off(State *S);
void	m_sizemem(State *S, int size);
void	m_numaregion(char *name, ulong start, ulong end, 
		long lrlat, long lwlat,
		long rrlat, long rwlat,
		int mapid, ulong mapbase, int private);
void	m_numastats(State *S);
void	m_numastatsall(void);
void	m_numasetmapid(int whichmap, int cpuid);
void	m_parseobjdump(State *S, char *filename);
void	m_sharebus(State *S, int donorid);
void	m_addvaluetrace(State *S, char *tag, ulong addr, int size, int onstack, ulong pcstart, int frameoffset, int ispointer);
void	m_delvaluetrace(State *S, char *tag, ulong addr, int size, int onstack, ulong pcstart, int frameoffset, int ispointer);
void	m_valuestats(State *s);
void	m_readstabs(State *S, char *filename);
void	m_regtracer(State *S, char *name, ulong pcstart, int regnum, int size, int ispointer);
void	m_regtracerstats(State *S);
int	m_sort_numaregions_comp(const void *a, const void *b);
int	m_sort_regtracers_comp(const void *a, const void *b);
int	m_find_numa(ulong vaddr, Numa *N, int start, int count);
int 	m_find_numastack(ulong curfn, ulong curframe, ulong vaddr,
			Numa *N, int start, int count);


void	physics_newsigsrc(int type, char* descr, double tau, double propspeed,
	double A, double B, double C, double D, double E, double F,
	double G, double H, double I, double K, double m, double n,
	double o, double p, double q, double r, double s, double t,
	char * trajectoryfile, int trajectoryrate, double fixedx,
	double fixedy, double fixedz, int looptrajectory, char *samplesfile,
	int samplerate, double fixedsampleval, int loopsamples);
void	physics_sensorsdbg(void);
void	physics_sigsubscr(State *S, int whichsensor, int whichsignal);


void	pau_printstats(State *S);
void	power_printstats(State *S);
void	tag_settag(State *S, int whichtag, char *tag, char *whohas, int confidence, ulong ttl);
void	tag_showtags(State *S);


void	fault_setnodepfun(State *S, char *alg);
void	fault_setnetsegpfun(Netsegment *tptr, char *alg);

void	batt_newbatt(int ID, double capacity_mAh);
void	batt_nodeattach(State *S, int which);
void	batt_printstats(State *S, int which);
char*	mstrsep(char **, const char *);
int	bit_flips_32(ulong w1, ulong w2);
int	bit_flips_n(int n, ulong w1, ulong w2);
int	parsenetsegdump(char *buf, Segbuf *segbuf);
int	superHcheck_batt_intr(State *S);
int	superHcheck_nic_intr(State *S);
uchar	dev7708read(State *S, ulong);
uchar	nic_rx_dequeue(State *S, int whichifc);

ulong	checksum(uchar *data, int datalen);
uvlong	exponential(void *, char *, uvlong);
uvlong	uniform_random(void *, char *, uvlong);
void	Delay_Slot(State *S, ulong);
void	battery_dumpall(State *);
void	battery_feed(double);
void	clearistream(void);
void	cont(State *S, ulong);
void	dbg(char *);
void	dev7708write(State *S, ulong, uchar);
void	m_dumpmem(State *S, long, long);
void	error(char *);
void	fault_feed(void);
void	go(State *S, int);
void	help(void);
void	load_srec(State *S, char *);
void	loadcmds(char *filename);
void	man(char *cmd);
void	mbitprint(State *S, short, ulong);
void	mprint(State *S, int out, char *fmt, ...);
void	mprintout(char *s);
void	munchinput(char *buf);
void	myrmigkiinit(void);
void	netsegdump(char *dumpname, Segbuf *segbuf);
void	network_clock(void);
void	newbatt(int type, double capacity);
void	nic_tx_enqueue(State *S, uchar, int whichifc);
void	pau_clk(State *S);
void	pau_feed(State *S, int type, ulong addr);
void	pau_lvdd(State *S, int pauentry);
void	pau_init(State *S, int nentries);
void	physics_feed(void);
double	physics_propagation(Signalsrc *s, double xloc, double yloc, double zloc);
void	pic_intr_clear(State *S, InterruptQ *q, int type, int clear_all);
void	pic_intr_enqueue(State *dptr, InterruptQ *q, int type, int value, int misc);
void* 	pic_intr_dequeue(State *S, InterruptQ *q);
void	power_scaledelay(State *S, double Vdd);
void	power_scalevdd(State *S, double freq);
void	remote_seg_enqueue(Segbuf *segbuf);
void	run(State *S);
void	savemem(State *, ulong, ulong, char *);
void	sched_step(void);
void	scheduler(void*);
void	m_setnode(int);
void	sfatal(State *, char *);
void	streamchk(void);
void	superHdecode(ushort, SuperHPipestage *);
void	superHdumppipe(State *S);
void	superHdumpregs(State *S);
void	superHdumpsysregs(State *S);
int	superHfaststep(State *S, int drain_pipeline);
void	superHfatalaction(State *S);
void	superHifidflush(State *S);
State*	superHnewstate(double, double, double, int, double);
void	superHpipeflush(State *S);
void	superHresetcpu(State *S);
void	superHsetfreq(State *, int);
void	superHsetvdd(State *, double);
void	superHstallaction(State *S, ulong addr, int type, int latency);
int	superHstep(State *S, int drain_pipeline);
int	superHtake_batt_intr(State *S);
int	superHtake_nic_intr(State *S);
int	superHtake_timer_intr(State *S);
void	superHtake_exception(State *S);
void	superHsplit(State *S, ulong startpc, ulong stackaddr, ulong argaddr, char *idstr);
int	superHtouchesmem(int op);
int	superHEXtouchesmem(State *S);
void	superHsettimerintrdelay(State *S, int delay);
void	superHcache_deactivate(State *S);
void	superHcache_printstats(State *S);
int	superHcache_init(State *S, int size, int blocksize, int assoc);
void	superHdumpall(char *filename);

uchar 	superHreadbyte(State *S, ulong addr);
ulong 	superHreadlong(State *S, ulong addr);
ushort 	superHreadword(State *S, ulong addr);
void 	superHwritelong(State *S, ulong addr, ulong data);
void 	superHwriteword(State *S, ulong addr, ulong data);
void	superHwritebyte(State *S, ulong addr, ulong data);

void	tx_retryalg_asap(void *S, int whichifc);
void	tx_retryalg_binexp(void *S, int whichifc);
void	tx_retryalg_linear(void *S, int whichifc);
void	tx_retryalg_random(void *S, int whichifc);

ulong	devportreadlong(State *S, ulong addr);
ushort	devportreadword(State *S, ulong addr);
uchar	devportreadbyte(State *S, ulong addr);
void	devportwritelong(State *S, ulong addr, ulong data);
void	devportwriteword(State *S, ulong addr, ushort data);
void	devportwritebyte(State *S, ulong addr, uchar data);

ulong	dev7708readlong(State *S, ulong addr);
ushort	dev7708readword(State *S, ulong addr);
uchar	dev7708readbyte(State *S, ulong addr);
void	dev7708writelong(State *S, ulong addr, ulong data);
void	dev7708writeword(State *S, ulong addr, ushort data);
void	dev7708writebyte(State *S, ulong addr, uchar data);

int	superHtlb_init(State *S, int size, int blocksize, int assoc);
void	superHvmtranslate(State *S, int op, TransAddr *transladdr);
void	superHtlb_exception(State *S, int code, ulong vaddr);
ulong	superHtlb_addrarray_read(State *S, ulong addr);
void	superHtlb_addrarray_write(State *S, ulong addr, ulong data);
ulong	superHtlb_dataarray_read(State *S, ulong addr);
void	superHtlb_dataarray_write(State *S, ulong addr, ulong data);
void	superHdumptlb(State *S);
void	superHtlbflush(State *S);

void	msp430decode(State *S, ushort instr, MSP430Pipestage *p);
void	msp430dumpregs(State *S);
void	msp430dumpsysregs(State *S);
void	msp430fatalaction(State *S);
void	msp430stallaction(State *S);
void	msp430take_timer_intr(State *S);
void	msp430resetcpu(State *S);
State*	msp430newstate(double xloc, double yloc, double zloc, int orbit, double velocity);
ushort	msp430regread(State *S, int n, MSP430Pipestage *p);
void	msp430regset(State *S, int n, ushort data);
void	msp430writebyte(State *S, ushort addr, uchar data);
void	msp430peripheralwritebyte(State *S, ushort addr, uchar data);
void	msp430writeword(State *S, ushort addr, ushort data);
void	msp430peripheralwriteword(State *S, ushort addr, ushort data);
uchar	msp430readbyte(State *S, ushort addr);
uchar	msp430peripheralreadbyte(State *S, ushort addr);
ushort	msp430readword(State *S, ushort addr);
ushort	msp430peripheralreadword(State *S, ushort addr);
void	msp430pipeflush(State *S);
int	msp430step(State *S);
uchar	dev430x1xxreadbyte(State *S, ushort addr);
void	dev430x1xxwritebyte(State *S, ushort addr, uchar data);
ushort	dev430x1xxreadword(State *S, ushort addr);
void	dev430x1xxwriteword(State *S, ushort addr, ushort data);
int	msp430step(State *S);
int	msp430faststep(State *S);
void	msp430pipeflush(State *S);
void	msp430dumppipe(State *S);
void	msp430split(State *S, ulong startpc, ulong stackptr, ulong argaddr, char *idstr);

int	mchdir(char *);
int	mclose(int);
int	mcreate(char *path, int mode);
char*	mfgets(char *buf, int len, int fd);
int	mfsize(int fd);
int	mkillscheduler(void);
int	mopen(char *path, int mode);
int	mread(int fd, char* buf, int len);
int	mspawnscheduler(void);
int	mwrite(int fd, char* buf, int len);
long	mrandom(void);
ulong	mcputimeusecs(void);
ulong	musercputimeusecs(void);
void	mnsleep(ulong);
ulong	mwallclockusecs(void);
ulong	sim_syscall(State *, ulong, ulong, ulong, ulong);
void	mexit(char *, int);
void	marchinit(void);
void	mlog(State *S, char *fmt, ...);
void	merror(char *fmt, ...);
void	mprintfd(int fd, char* buf);
char*	mgetpwd(void);
long	mrandominit(void);

/*									*/
/*			SuperH instruction functions			*/
/*									*/
void	superH_add(State *S, ulong m, ulong n);
void	superH_addc(State *S, ulong m, ulong n);
void	superH_addi(State *S, long i, ulong n);
void	superH_addv(State *S, ulong m, ulong n);
void	superH_and(State *S, ulong m, ulong n);
void	superH_andi(State *S, long i);
void	superH_andm(State *S, long i);
void	superH_bf(State *S, long d);
void	superH_bfs(State *S, long d);
void	superH_bra(State *S, long d);
void	superH_braf(State *S, ulong n);
void	superH_bsr(State *S, long d);
void	superH_bsrf(State *S, ulong n);
void	superH_bt(State *S, long d);
void	superH_bts(State *S, long d);
void	superH_clrmac(State *S);
void	superH_clrs(State *S);
void	superH_clrt(State *S);
void	superH_cmpeq(State *S, ulong m, ulong n);
void	superH_cmpge(State *S, ulong m, ulong n);
void	superH_cmpgt(State *S, ulong m, ulong n);
void	superH_cmphi(State *S, ulong m, ulong n);
void	superH_cmphs(State *S, ulong m, ulong n);
void	superH_cmpim(State *S, long i);
void	superH_cmppl(State *S, ulong n);
void	superH_cmppz(State *S, ulong n);
void	superH_cmpstr(State *S, ulong m,ulong n);
void	superH_div0s(State *S, ulong m,ulong n);
void	superH_div0u(State *S);
void	superH_div1(State *S, ulong m,ulong n);
void	superH_dmuls(State *S, ulong m,ulong n);
void	superH_dmulu(State *S, ulong m,ulong n);
void	superH_dt(State *S, ulong n);
void	superH_extsb(State *S, ulong m,ulong n);
void	superH_extsw(State *S, ulong m,ulong n);
void	superH_extub(State *S, ulong m, ulong n);
void	superH_extuw(State *S, ulong m, ulong n);
void	superH_jmp(State *S, ulong n);
void	superH_jsr(State *S, ulong n);
void	superH_ldcgbr(State *S, ulong m);
void	superH_ldcmgbr(State *S, ulong m);
void	superH_ldcmr_bank(State *S, int q, ulong m);
void	superH_ldcmspc(State *S, ulong m);
void	superH_ldcmsr(State *S, ulong m);
void	superH_ldcmssr(State *S, ulong m);
void	superH_ldcmvbr(State *S, ulong m);
void	superH_ldcr_bank(State *S, int q, ulong m);
void	superH_ldcspc(State *S, ulong m);
void	superH_ldcsr(State *S, ulong m);
void	superH_ldcssr(State *S, ulong m);
void	superH_ldcvbr(State *S, ulong m);
void	superH_ldsmach(State *S, ulong m);
void	superH_ldsmacl(State *S, ulong m);
void	superH_ldsmmach(State *S, ulong m);
void	superH_ldsmmacl(State *S, ulong m);
void	superH_ldsmpr(State *S, ulong m);
void	superH_ldspr(State *S, ulong m);
void	superH_ldtlb(State *S);
void	superH_macl(State *S, ulong m,ulong n);
void	superH_macw(State *S, ulong m,ulong n);
void	superH_mov(State *S, ulong m,ulong n);
void	superH_mova(State *S, long d);
void	superH_movbl(State *S, ulong m, ulong n);
void	superH_movbl0(State *S, ulong m,ulong n);
void	superH_movbl4(State *S, ulong m, long d);
void	superH_movblg(State *S, long d);
void	superH_movbm(State *S, ulong m, ulong n);
void	superH_movbp(State *S, ulong m, ulong n);
void	superH_movbs(State *S, ulong m,ulong n);
void	superH_movbs0(State *S, ulong m, ulong n);
void	superH_movbs4(State *S, long d, ulong n);
void	superH_movbsg(State *S, long d);
void	superH_movi(State *S, long i, ulong n);
void	superH_movli(State *S, long d, ulong n);
void	superH_movll(State *S, ulong m, ulong n);
void	superH_movll0(State *S, ulong m, ulong n);
void	superH_movll4(State *S, ulong m, long d, ulong n);
void	superH_movllg(State *S, long d);
void	superH_movlm(State *S, ulong m, ulong n);
void	superH_movlp(State *S, ulong m,ulong n);
void	superH_movls(State *S, ulong m, ulong n);
void	superH_movls0(State *S, ulong m, ulong n);
void	superH_movls4(State *S, ulong m, long d,ulong n);
void	superH_movlsg(State *S, long d);
void	superH_movt(State *S, ulong n);
void	superH_movwi(State *S, long d, ulong n);
void	superH_movwl(State *S, ulong m, ulong n);
void	superH_movwl0(State *S, ulong m, ulong n);
void	superH_movwl4(State *S, ulong m, long d);
void	superH_movwlg(State *S, long d);
void	superH_movwm(State *S, ulong m, ulong n);
void	superH_movwp(State *S, ulong m, ulong n);
void	superH_movws(State *S, ulong m, ulong n);
void	superH_movws0(State *S, ulong m, ulong n);
void	superH_movws4(State *S, long d, ulong n);
void	superH_movwsg(State *S, long d);
void	superH_mull(State *S, ulong m,ulong n);
void	superH_muls(State *S, ulong m, ulong n);
void	superH_mulu(State *S, ulong m,ulong n);
void	superH_neg(State *S, ulong m,ulong n);
void	superH_negc(State *S, ulong m, ulong n);
void	superH_nop(State *S);
void	superH_not(State *S, ulong m,ulong n);
void	superH_or(State *S, ulong m,ulong n);
void	superH_ori(State *S, long i);
void	superH_orm(State *S, long i);
void	superH_sleep(State *S);
void	superH_pref(State *S, ulong n);
void	superH_rfg(State *S, long i);
void	superH_rotcl(State *S, ulong n);
void	superH_rotcr(State *S, ulong n);
void	superH_rotl(State *S, ulong n);
void	superH_rotr(State *S, ulong n);
void	superH_rte(State *S);
void	superH_rts(State *S);
void	superH_sets(State *S);
void	superH_sett(State *S);
void	superH_shad(State *S, ulong m, ulong n);
void	superH_shal(State *S, ulong n);
void	superH_shar(State *S, ulong n);
void	superH_shld(State *S, ulong m, ulong n);
void	superH_shll(State *S, ulong n);
void	superH_shll16(State *S, ulong n);
void	superH_shll2(State *S, ulong n);
void	superH_shll8(State *S, ulong n);
void	superH_shlr(State *S, ulong n);
void	superH_shlr16(State *S, ulong n);
void	superH_shlr2(State *S, ulong n);
void	superH_shlr8(State *S, ulong n);
void	superH_stcgbr(State *S, ulong n);
void	superH_stcmgbr(State *S, ulong n);
void	superH_stcmr_bank(State *S, int q, ulong m);
void	superH_stcmspc(State *S, ulong n);
void	superH_stcmsr(State *S, ulong n);
void	superH_stcmssr(State *S, ulong n);
void	superH_stcmvbr(State *S, ulong n);
void	superH_stcr_bank(State *S, int q, ulong m);
void	superH_stcspc(State *S, ulong n);
void	superH_stcsr(State *S, ulong n);
void	superH_stcssr(State *S, ulong n);
void	superH_stcvbr(State *S, ulong n);
void	superH_stsmach(State *S, ulong n);
void	superH_stsmacl(State *S, ulong n);
void	superH_stsmmach(State *S, ulong n);
void	superH_stsmmacl(State *S, ulong n);
void	superH_stsmpr(State *S, ulong n);
void	superH_stspr(State *S, ulong n);
void	superH_sub(State *S, ulong m,ulong n);
void	superH_subc(State *S, ulong m, ulong n);
void	superH_subv(State *S, ulong m,ulong n);
void	superH_swapb(State *S, ulong m, ulong n);
void	superH_swapw(State *S, ulong m, ulong n);
void	superH_tas(State *S, ulong n);
void	superH_trapa(State *S, long i);
void	superH_tst(State *S, ulong m, ulong n);
void	superH_tsti(State *S, long i);
void	superH_tstm(State *S, long i);
void	superH_xor(State *S, ulong m,ulong n);
void	superH_xori(State *S, long i);
void	superH_xorm(State *S, long i);
void	superH_xtrct(State *S, ulong m, ulong n);


/*									*/
/*			MSP430 instruction functions			*/
/*									*/		
void	msp430_mov(State *S, ushort m, ushort n, MSP430Pipestage *p);
void	msp430_movb(State *S, ushort m, ushort n, MSP430Pipestage *p);
void	msp430_add(State *S, ushort m, ushort n, MSP430Pipestage *p);
void	msp430_addb(State *S, int m, int n, MSP430Pipestage *p);
void	msp430_addc(State *S, int m, int n, MSP430Pipestage *p);
void	msp430_addcb(State *S, int m, int n, MSP430Pipestage *p);
void	msp430_sub(State *S, int m, int n, MSP430Pipestage *p);
void	msp430_subb(State *S, int m, int n, MSP430Pipestage *p);
void	msp430_subc(State *S, int m, int n, MSP430Pipestage *p);
void	msp430_subcb(State *S, int m, int n, MSP430Pipestage *p);
void	msp430_cmp(State *S, int m, int n, MSP430Pipestage *p);
void	msp430_cmpb(State *S, int m, int n, MSP430Pipestage *p);
void	msp430_dadd(State *S, int m, int n, MSP430Pipestage *p);
void	msp430_daddb(State *S, int m, int n, MSP430Pipestage *p);
void	msp430_bit(State *S, int m, int n, MSP430Pipestage *p);
void	msp430_bitb(State *S, int m, int n, MSP430Pipestage *p);
void	msp430_bic(State *S, int m, int n, MSP430Pipestage *p);
void	msp430_bicb(State *S, int m, int n, MSP430Pipestage *p);
void	msp430_bis(State *S, int m, int n, MSP430Pipestage *p);
void	msp430_bisb(State *S, int m, int n, MSP430Pipestage *p);
void	msp430_xor(State *S, int m, int n, MSP430Pipestage *p);
void	msp430_xorb(State *S, int m, int n, MSP430Pipestage *p);
void	msp430_and(State *S, int m, int n, MSP430Pipestage *p);
void	msp430_andb(State *S, int m, int n, MSP430Pipestage *p);
void	msp430_rrc(State *S, ushort n, MSP430Pipestage *p);
void	msp430_rrcb(State *S, ushort n, MSP430Pipestage *p);
void	msp430_rra(State *S, ushort n, MSP430Pipestage *p);
void	msp430_rrab(State *S, ushort n, MSP430Pipestage *p);
void	msp430_push(State *S, ushort n, MSP430Pipestage *p);
void	msp430_pushb(State *S, ushort n, MSP430Pipestage *p);
void	msp430_swpb(State *S, ushort n, MSP430Pipestage *p);
void	msp430_call(State *S, ushort n, MSP430Pipestage *p);
void	msp430_reti(State *S, MSP430Pipestage *p);
void	msp430_sxt(State *S, ushort n, MSP430Pipestage *p);
void	msp430_jeqjz(State *S, short offset, MSP430Pipestage *p);
void	msp430_jnejnz(State *S, short offset, MSP430Pipestage *p);
void	msp430_jc(State *S, short offset, MSP430Pipestage *p);
void	msp430_jnc(State *S, short offset, MSP430Pipestage *p);
void	msp430_jn(State *S, short offset, MSP430Pipestage *p);
void	msp430_jge(State *S, short offset, MSP430Pipestage *p);
void	msp430_jl(State *S, short offset, MSP430Pipestage *p);
void	msp430_jmp(State *S, short offset, MSP430Pipestage *p);
