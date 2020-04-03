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

int	yylex(void);
int	yyerror(char *);
int	sf_superh_parse(void);
int	sf_riscv_parse(void);


/*											*/
/*					Network modeling				*/
/*											*/
void	m_taintmem(Engine *E, State *S, uint64_t addr, uint32_t taintstartPC, uint32_t taintendPC, uint64_t taintCol, uint64_t taintLength);
void	m_taintreg(Engine *E, State *S, uint64_t addr, uint32_t taintstartPC, uint32_t taintendPC, uint64_t taintCol);
void	m_ftaintreg(Engine *E, State *S, uint64_t addr, uint32_t taintstartPC, uint32_t taintendPC, uint64_t taintCol);
void	m_riscvdumptaintdistr(Engine *E, State *S);
void	taintprop(Engine *E, State *S, uint64_t immtaint1, uint64_t immtaint2, uint64_t AddrOut, SunflowerTaintMemType memType);
uint64_t taintretmems(Engine *E, State *S, uint64_t Addr1, int NumBytes);
uint64_t taintretreg(Engine *E, State *S, uint64_t rs1);
uint64_t ftaintretreg(Engine *E, State *S, uint64_t rs1);
void	taintclear(Engine *E, State *S,uint64_t addr, SunflowerTaintMemType memType);
void	network_netseg2file(Engine *E, int which, char *filename);
void	network_file2netseg(Engine *E, char *file, int whichseg);
void	network_netsegpropmodel(Engine *, int whichseg, int whichmodel, double minsnr);
void	network_netnewseg(Engine *, int which, int framebits, int pgnspeed, int bitrate, int width,
		int fpdist, double fpdistmu, double fpdistsigma, double fpdistlambda,
		int fddist, double fddistmu, double fddistsigma, double fddistlambda);
void	network_netsegnicattach(Engine *, State *S, int whichifc, int whichseg);
void	network_netnodenewifc(
		Engine *, State *S, int which, double txpwr, double rxpwr, double idlepwr, double listenpwr,
		int fpdist, int fpdistmu, int fpdistsigma, int fpdistlambda,
		int txfifosz, int rxfifosz);
void	network_netsegdelete(Engine *, int whichseg);
void	network_netdebug(Engine *, State *S);
void	network_setretryalg(Engine *, State *S, int which, char *alg);
void	tx_retryalg_asap(void *, void *S, int whichifc);
void	tx_retryalg_binexp(void *, void *S, int whichifc);
void	tx_retryalg_linear(void *, void *S, int whichifc);
void	tx_retryalg_random(void *, void *S, int whichifc);
void	tx_retryalg_none(void *, void *S, int whichifc);
int	fifo_enqueue(Engine *, State *S, Fifo fifo_name, int whichifc);
void	netsegdump(Engine *, char *dumpname, Segbuf *segbuf);
void	network_clock(Engine *);
uchar	nic_rx_dequeue(Engine *, State *S, int whichifc);
void	nic_tx_enqueue(Engine *, State *S, uchar, int whichifc);
double	check_snr(Engine *, Netsegment *curseg, State *src_node, State *dst_node);
void	remote_seg_enqueue(Engine *, Segbuf *segbuf);
int	parsenetsegdump(Engine *, char *buf, Segbuf *segbuf);





/*											*/
/*					Emulated system calls				*/
/*											*/
int	mchdir(char *);
int	mclose(int);
int	mcreate(char *path, int mode);
char*	mfgets(char *buf, int len, int fd);
int	mfsize(int fd);
int	mopen(char *path, int mode);
int	mread(int fd, char* buf, int len);
int	mwrite(int fd, char* buf, int len);
char*	mgetpwd(void);
ulong	sim_syscall(Engine *, State *, ulong, ulong, ulong, ulong);
ulong 	riscv_sim_syscall(Engine *, State *, ulong, ulong, ulong, ulong);


/*											*/
/*				Miscellaneous helper routines				*/
/*											*/
void	mexit(Engine *, char *, int)  __attribute__((noreturn));
void	marchinit();
void	mlog(Engine *, State *S, char *fmt, ...);
void	merror(Engine *, char *fmt, ...);
void	mprintfd(int fd, char* buf);
int	msnprint(char *dst, int size, char *fmt, ...);
int	mkillscheduler(Engine *);
int	mspawnscheduler(Engine *);
uvlong	mrandom(Engine *);
uvlong	mrandominit(Engine *, uvlong);
ulong	mcputimeusecs(void);
ulong	musercputimeusecs(void);
void	mnsleep(ulong);
ulong	mwallclockusecs(void);
void	m_dumpall(Engine *, char *filename, int mode, char *tag, char *pre);
void	m_dumpnode(Engine *, int i, char *filename, int mode, char *tag, char *pre);
void	m_version(Engine *E);
void	m_newnode(Engine *E, char *type, double x, double y, double z, char *trajfilename, int looptrajectory, int trajectoryrate);
void	m_powertotal(Engine *);
void	m_renumbernodes(Engine *);
void	m_run(Engine *, State *S, char *args);
void	m_on(Engine *, State *S);
void	m_off(Engine *, State *S);
void	m_sizemem(Engine *, State *S, int size);
void	m_numaregion(Engine *, char *name, ulong start, ulong end,
		long lrlat, long lwlat,
		long rrlat, long rwlat,
		int mapid, ulong mapbase, int private);
void	m_numastats(Engine *, State *S);
void	m_numastatsall(Engine *);
void	m_numasetmapid(Engine *, int whichmap, int cpuid);
void	m_parseobjdump(Engine *, State *S, char *filename);
void	m_sharebus(Engine *, State *S, int donorid);
void	m_addvaluetrace(Engine *E, State *S, char *tag, ulong addr, int size, int onstack, ulong pcstart, int frameoffset, int ispointer);
void	m_delvaluetrace(Engine *E, State *S, char *tag, ulong addr, int size, int onstack, ulong pcstart, int frameoffset, int ispointer);
void	m_valuestats(Engine *E, State *s);
void	m_readstabs(Engine *E, State *S, char *filename);
void	m_regtracer(Engine *E, State *S, char *name, ulong pcstart, int regnum, int size, int ispointer);
void	m_regtracerstats(Engine *E, State *S);
int	m_sort_numaregions_comp(const void *a, const void *b);
int	m_sort_regtracers_comp(const void *a, const void *b);
int	m_find_numa(ulong vaddr, Numa *N, int start, int count);
int 	m_find_numastack(ulong curfn, ulong curframe, ulong vaddr,
			Numa *N, int start, int count);
void	m_pcbacktrace(Engine *E, State *S);
void	input(Engine *E, char *);
void	load(Engine *E, char *);
void	scan_labels_and_globalvars(Engine *E);
void	streamchk(Engine *);
void	mfree(Engine *E, void *ptr, char *ID);
void	mmblocksdisplay(Engine *E);
void*	mcalloc(Engine *E, int nelem, int size, char *ID);
void*	mmalloc(Engine *E, int size, char *ID);
void*	mrealloc(Engine *E, void *oldptr, int size, char *ID);
void	mstatelock();
void	mstateunlock();
void	run(Engine *, State *S);
void	savemem(Engine *, State *, ulong, ulong, char *);
void	scheduler(Engine *);
void	m_setnode(Engine *, int);
void	sfatal(Engine *E, State *, char *);
void	clearistream(Engine *);
void	dbg(char *);
void	m_dumpmem(Engine *, State *S, long, long);
void	error(char *);
void	help(Engine *);
void	load_srec(Engine *, State *S, char *);
void	load_mapfile(Engine *E, State *S, char *filename);
void	loadcmds(Engine *, char *filename);
void	man(Engine *, char *cmd);
void	mbitprint(Engine *, State *S, short, ulong);
void	mbit64print(Engine *E, State *S, short, uvlong);
void	mfloatprint(Engine *E, State *S, uvlong);
void	mprint(Engine *, State *S, int out, char *fmt, ...);
void	mprintout(char *s);
void	munchinput(Engine *, char *buf);
char*	mstrsep(char **, const char *);
ulong	checksum(uchar *data, int datalen);
void	m_setloc(Engine *, State *, double, double, double);
void	m_locstats(Engine *E, State *S);
Engine*	m_allocengine(uvlong);
Engine* m_lookupengine(uvlong);
void	traj_feed(Engine *E);



/*												*/
/*					Physical signal modeling				*/
/*												*/
void	physics_newsigsrc(Engine *, int type, char* descr, double tau, double propspeed,
		double A, double B, double C, double D, double E, double F,
		double G, double H, double I, double K, double m, double n,
		double o, double p, double q, double r, double s, double t,
		char * trajectoryfile, int trajectoryrate, double fixedx,
		double fixedy, double fixedz, int looptrajectory, char *samplesfile,
		int samplerate, double fixedsampleval, int loopsamples);
void	physics_sensorsdbg(Engine *);
void	physics_sigsubscr(Engine *, State *S, int whichsensor, int whichsignal);
void	physics_feed(Engine *);
double	physics_propagation(Signalsrc *s, double xloc, double yloc, double zloc);




/*
 *					Propulsion power modeling
 */
void
propulsionSetPropulsionCoeffs(Engine *  E, State *  S,
	double xk1, double xk2, double xk3, double xk4, double xk5, double xk6,
	double yk1, double yk2, double yk3, double yk4, double yk5, double yk6,
	double zk1, double zk2, double zk3, double zk4, double zk5, double zk6);



/*
 *					Mass modeling
 */
void	massSetNodeMass(Engine *  E, State *  S, double mass);


/*											*/
/*						PAU					*/
/*											*/
void	pau_printstats(Engine *, State *S);
void	pau_clk(Engine *, State *S);
void	pau_feed(Engine *, State *S, int type, ulong addr);
void	pau_lvdd(Engine *, State *S, int pauentry);
void	pau_init(Engine *, State *S, int nentries);



/*											*/
/*				Power estimation nd batteries				*/
/*											*/
void	power_printstats(Engine *, State *S);
void	power_scaledelay(Engine *, State *S, double Vdd);
void	power_scalevdd(Engine *, State *S, double freq);
int	bit_flips_32(ulong w1, ulong w2);
int	bit_flips_n(int n, ulong w1, ulong w2);
void	newbatt(int type, double capacity);
void	batt_newbatt(Engine *, int ID, double capacity_mAh);
void	batt_nodeattach(Engine *, State *S, int which);
void	batt_printstats(Engine *, State *S, int which);
void	battery_dumpall(Engine *, State *);
void	battery_feed(Engine *, double);


/*				Breakpoint management					*/
void	m_setbptglobaltime(Engine *, Picosec);
void	m_setbptcycles(Engine *, State *, uvlong);
void	m_setbptinstrs(Engine *, State *, uvlong);
void	m_setbptsensorreading(Engine *, State *, int, double);
void	m_bptls(Engine *);
void	m_bptdel(Engine *, int);




/*											*/
/*				Old "Tag" infrastructure				*/
/*											*/
void	tag_settag(Engine *, State *S, int whichtag, char *tag, char *whohas, int confidence, ulong ttl);
void	tag_showtags(Engine *E, State *S);



/*											*/
/*				Old fault modeling infrastructure			*/
/*											*/
void	fault_setnodepfun(Engine *E, State *S, char *alg);
void	fault_setnetsegpfun(Engine *E, Netsegment *tptr, char *alg);
//uvlong	pfunexp(void *ptr, char *type, uvlong modulo);
//uvlong	pfunrnd(void *ptr, char *type, uvlong modulo);
void	fault_feed(Engine *E);
uvlong	exponential(void *, char *, uvlong);
uvlong	uniform_random(void *, char *, uvlong);










/*											*/
/*				Handling probability distributions			*/
/*											*/
void	m_defndist(Engine *, char *distname, DoubleList *support, DoubleList *prob);
void	m_initrandtable(Engine *, char *distname, char *pfunname, double basis_start,
		double basis_end, double granularity, double p1, double p2, double p3, double p4);
double	m_randgen(Engine *, char *distname, double min, double max, double p1, double p2, double p3, double p4);
void	m_randprint(Engine *, char *distname, double min, double max, double p1, double p2, double p3, double p4);
void	m_registerrvar(Engine *, State *, char *name, int idx,
		char *valdistname, double v_p1, double v_p2, double v_p3, double v_p4,
		char *durdistname, double d_p1, double d_p2, double d_p3, double d_p4,
		char *mode);
void	m_updatervars(Engine *);
void	m_listrvars(Engine *E);

double	m_pfun_bathtub(Engine*, double, double, double, double, double, double);
double	m_pfun_beta(Engine*, double, double, double, double, double, double);
double	m_pfun_betaprime(Engine*, double, double, double, double, double, double);
double	m_pfun_cauchy(Engine*, double, double, double, double, double, double);
double	m_pfun_erlang(Engine*, double, double, double, double, double, double);
double	m_pfun_exp(Engine*, double, double, double, double, double, double);
double	m_pfun_extremeval(Engine*, double, double, double, double, double, double);
double	m_pfun_f(Engine*, double, double, double, double, double, double);
double	m_pfun_fermidirac(Engine*, double, double, double, double, double, double);
double	m_pfun_fisherz(Engine*, double, double, double, double, double, double);
double	m_pfun_gamma(Engine*, double, double, double, double, double, double);
double	m_pfun_gauss(Engine*, double, double, double, double, double, double);
double	m_pfun_gibrat(Engine*, double, double, double, double, double, double);
double	m_pfun_gumbel(Engine*, double, double, double, double, double, double);
double	m_pfun_halfnormal(Engine*, double, double, double, double, double, double);
double	m_pfun_laplace(Engine*, double, double, double, double, double, double);
double	m_pfun_logistic(Engine*, double, double, double, double, double, double);
double	m_pfun_lognorm(Engine*, double, double, double, double, double, double);
double	m_pfun_logseries(Engine*, double, double, double, double, double, double);
double	m_pfun_maxwell(Engine*, double, double, double, double, double, double);
double	m_pfun_negbinomial(Engine*, double, double, double, double, double, double);
double	m_pfun_pareto(Engine*, double, double, double, double, double, double);
double	m_pfun_pearsontype3(Engine*, double, double, double, double, double, double);
double	m_pfun_poisson(Engine*, double, double, double, double, double, double);
double	m_pfun_rayleigh(Engine*, double, double, double, double, double, double);
double	m_pfun_studentst(Engine*, double, double, double, double, double, double);
double	m_pfun_studentsz(Engine*, double, double, double, double, double, double);
double	m_pfun_uniform(Engine*, double, double, double, double, double, double);
double	m_pfun_weibull(Engine*, double, double, double, double, double, double);
double	m_pfun_xi(Engine*, double, double, double, double, double, double);
double	m_pfun_xi2(Engine*, double, double, double, double, double, double);

double	m_pr_bathtub(Engine*, double, double, double, double, double);
double	m_pr_beta(Engine*, double, double, double, double, double);
double	m_pr_betaprime(Engine*, double, double, double, double, double);
double	m_pr_cauchy(Engine*, double, double, double, double, double);
double	m_pr_erlang(Engine*, double, double, double, double, double);
double	m_pr_exp(Engine*, double, double, double, double, double);
double	m_pr_extremeval(Engine*, double, double, double, double, double);
double	m_pr_f(Engine*, double, double, double, double, double);
double	m_pr_fermidirac(Engine*, double, double, double, double, double);
double	m_pr_fisherz(Engine*, double, double, double, double, double);
double	m_pr_gamma(Engine*, double, double, double, double, double);
double	m_pr_gauss(Engine*, double, double, double, double, double);
double	m_pr_gibrat(Engine*, double, double, double, double, double);
double	m_pr_gumbel(Engine*, double, double, double, double, double);
double	m_pr_halfnormal(Engine*, double, double, double, double, double);
double	m_pr_laplace(Engine*, double, double, double, double, double);
double	m_pr_logistic(Engine*, double, double, double, double, double);
double	m_pr_lognorm(Engine*, double, double, double, double, double);
double	m_pr_logseries(Engine*, double, double, double, double, double);
double	m_pr_maxwell(Engine*, double, double, double, double, double);
double	m_pr_negbinomial(Engine*, double, double, double, double, double);
double	m_pr_pareto(Engine*, double, double, double, double, double);
double	m_pr_pearsontype3(Engine*, double, double, double, double, double);
double	m_pr_poisson(Engine*, double, double, double, double, double);
double	m_pr_rayleigh(Engine*, double, double, double, double, double);
double	m_pr_studentst(Engine*, double, double, double, double, double);
double	m_pr_studentsz(Engine*, double, double, double, double, double);
double	m_pr_uniform(Engine*, double, double, double, double, double);
double	m_pr_weibull(Engine*, double, double, double, double, double);
double	m_pr_xi(Engine*, double, double, double, double, double);
double	m_pr_xi2(Engine*, double, double, double, double, double);



/*											*/
/*			Microarchitecture  modeling: general				*/
/*											*/
ulong	reg_read(Engine *, State *S, int n);
void	reg_set(Engine *, State *S, int n, ulong data);
void	sched_step(Engine *);
void	Delay_Slot(State *S, ulong);
void	go(Engine *, State *S, int);
void	cont(Engine *, State *S, ulong);
void	pic_intr_clear(Engine *, State *S, InterruptQ *q, int type, int clear_all);
void	pic_intr_enqueue(Engine *, State *dptr, InterruptQ *q, int type, int value, int misc);
void* 	pic_intr_dequeue(Engine *, State *S, InterruptQ *q);




/*											*/
/*			Microarchitecture  modeling: Hitachi SH				*/
/*											*/
void	superHdecode(Engine *, ushort, SuperHPipestage *);
void	superHdumppipe(Engine *, State *S);
void	superHdumpregs(Engine *E, State *S);
void	superHdumpsysregs(Engine *E, State *S);
int	superHfaststep(Engine *, State *S, int drain_pipeline);
void	superHfatalaction(Engine *, State *S);
void	superHIFIDflush(State *S);
State*	superHnewstate(Engine *E, double, double, double, char *);
void	superHflushpipe(State *S);
void	superHresetcpu(Engine *, State *S);
void	superHsetfreq(State *, int);
void	superHsetvdd(State *, double);
void	superHstallaction(Engine *, State *S, ulong addr, int type, int latency);
int	superHstep(Engine *, State *S, int drain_pipeline);
int	superHtake_batt_intr(Engine *, State *S);
int	superHtake_nic_intr(Engine *, State *S);
int	superHtake_timer_intr(Engine *, State *S);
void	superHtake_exception(Engine *, State *S);
void	superHsplit(Engine *E, State *S, ulong startpc, ulong stackaddr, ulong argaddr, char *idstr);
int	superHtouchesmem(int op);
int	superHEXtouchesmem(State *S);
void	superHsettimerintrdelay(Engine *, State *S, int delay);
void	superHcache_deactivate(Engine *, State *S);
void	superHcache_printstats(Engine *E, State *S);
int	superHcache_init(Engine *, State *S, int size, int blocksize, int assoc);
void	superHdumpall(char *filename);

uchar 	superHreadbyte(Engine *, State *S, ulong addr);
ulong 	superHreadlong(Engine *, State *S, ulong addr);
ushort 	superHreadword(Engine *, State *S, ulong addr);
void 	superHwritelong(Engine *, State *S, ulong addr, ulong data);
void 	superHwriteword(Engine *, State *S, ulong addr, ulong data);
void	superHwritebyte(Engine *, State *S, ulong addr, ulong data);

uchar 	riscVreadbyte(Engine *, State *S, ulong addr);
ulong 	riscVreadlong(Engine *, State *S, ulong addr);
ushort 	riscVreadword(Engine *, State *S, ulong addr);
void 	riscVwritelong(Engine *, State *S, ulong addr, ulong data);
void 	riscVwriteword(Engine *, State *S, ulong addr, ulong data);
void	riscVwritebyte(Engine *, State *S, ulong addr, ulong data);
void    riscVcache_deactivate(Engine *, State *S);
int     riscVcache_init(Engine *, State *S, int size, int blocksize, int assoc);
void	riscVvmtranslate(Engine *E, State *S, int op, TransAddr *tr);

ulong	devportreadlong(Engine *E, State *S, ulong addr);
ushort	devportreadword(Engine *E, State *S, ulong addr);
uchar	devportreadbyte(Engine *E, State *S, ulong addr);
void	devportwritelong(Engine *E, State *S, ulong addr, ulong data);
void	devportwriteword(Engine *E, State *S, ulong addr, ushort data);
void	devportwritebyte(Engine *E, State *S, ulong addr, uchar data);

ulong	dev7708readlong(Engine *E, State *S, ulong addr);
ushort	dev7708readword(Engine *E, State *S, ulong addr);
uchar	dev7708readbyte(Engine *E, State *S, ulong addr);
void	dev7708writelong(Engine *E, State *S, ulong addr, ulong data);
void	dev7708writeword(Engine *E, State *S, ulong addr, ushort data);
void	dev7708writebyte(Engine *E, State *S, ulong addr, uchar data);
//void	dev7708write(State *S, ulong, uchar);
//uchar	dev7708read(State *S, ulong);


int	superHtlb_init(Engine *, State *S, int size, int blocksize, int assoc);
void	superHvmtranslate(Engine *, State *S, int op, TransAddr *transladdr);
void	superHtlb_exception(State *S, int code, ulong vaddr);
ulong	superHtlb_addrarray_read(State *S, ulong addr);
void	superHtlb_addrarray_write(State *S, ulong addr, ulong data);
ulong	superHtlb_dataarray_read(State *S, ulong addr);
void	superHtlb_dataarray_write(State *S, ulong addr, ulong data);
void	superHdumptlb(Engine *, State *S);
void	superHtlbflush(Engine *, State *S);
int	superHcheck_batt_intr(Engine *, State *S);
int	superHcheck_nic_intr(Engine *, State *S);


/*									*/
/*			SuperH instruction functions			*/
/*									*/
void	superH_add(Engine *E, State *S, ulong m, ulong n);
void	superH_addc(Engine *E, State *S, ulong m, ulong n);
void	superH_addi(Engine *E, State *S, long i, ulong n);
void	superH_addv(Engine *E, State *S, ulong m, ulong n);
void	superH_and(Engine *E, State *S, ulong m, ulong n);
void	superH_andi(Engine *E, State *S, long i);
void	superH_andm(Engine *E, State *S, long i);
void	superH_bf(Engine *E, State *S, long d);
void	superH_bfs(Engine *E, State *S, long d);
void	superH_bra(Engine *E, State *S, long d);
void	superH_braf(Engine *E, State *S, ulong n);
void	superH_bsr(Engine *E, State *S, long d);
void	superH_bsrf(Engine *E, State *S, ulong n);
void	superH_bt(Engine *E, State *S, long d);
void	superH_bts(Engine *E, State *S, long d);
void	superH_clrmac(Engine *E, State *S);
void	superH_clrs(Engine *E, State *S);
void	superH_clrt(Engine *E, State *S);
void	superH_cmpeq(Engine *E, State *S, ulong m, ulong n);
void	superH_cmpge(Engine *E, State *S, ulong m, ulong n);
void	superH_cmpgt(Engine *E, State *S, ulong m, ulong n);
void	superH_cmphi(Engine *E, State *S, ulong m, ulong n);
void	superH_cmphs(Engine *E, State *S, ulong m, ulong n);
void	superH_cmpim(Engine *E, State *S, long i);
void	superH_cmppl(Engine *E, State *S, ulong n);
void	superH_cmppz(Engine *E, State *S, ulong n);
void	superH_cmpstr(Engine *E, State *S, ulong m,ulong n);
void	superH_div0s(Engine *E, State *S, ulong m,ulong n);
void	superH_div0u(Engine *E, State *S);
void	superH_div1(Engine *E, State *S, ulong m,ulong n);
void	superH_dmuls(Engine *E, State *S, ulong m,ulong n);
void	superH_dmulu(Engine *E, State *S, ulong m,ulong n);
void	superH_dt(Engine *E, State *S, ulong n);
void	superH_extsb(Engine *E, State *S, ulong m,ulong n);
void	superH_extsw(Engine *E, State *S, ulong m,ulong n);
void	superH_extub(Engine *E, State *S, ulong m, ulong n);
void	superH_extuw(Engine *E, State *S, ulong m, ulong n);
void	superH_jmp(Engine *E, State *S, ulong n);
void	superH_jsr(Engine *E, State *S, ulong n);
void	superH_ldcgbr(Engine *E, State *S, ulong m);
void	superH_ldcmgbr(Engine *E, State *S, ulong m);
void	superH_ldcmr_bank(Engine *E, State *S, int q, ulong m);
void	superH_ldcmspc(Engine *E, State *S, ulong m);
void	superH_ldcmsr(Engine *E, State *S, ulong m);
void	superH_ldcmssr(Engine *E, State *S, ulong m);
void	superH_ldcmvbr(Engine *E, State *S, ulong m);
void	superH_ldcr_bank(Engine *E, State *S, int q, ulong m);
void	superH_ldcspc(Engine *E, State *S, ulong m);
void	superH_ldcsr(Engine *E, State *S, ulong m);
void	superH_ldcssr(Engine *E, State *S, ulong m);
void	superH_ldcvbr(Engine *E, State *S, ulong m);
void	superH_ldsmach(Engine *E, State *S, ulong m);
void	superH_ldsmacl(Engine *E, State *S, ulong m);
void	superH_ldsmmach(Engine *E, State *S, ulong m);
void	superH_ldsmmacl(Engine *E, State *S, ulong m);
void	superH_ldsmpr(Engine *E, State *S, ulong m);
void	superH_ldspr(Engine *E, State *S, ulong m);
void	superH_ldtlb(Engine *E, State *S);
void	superH_macl(Engine *E, State *S, ulong m,ulong n);
void	superH_macw(Engine *E, State *S, ulong m,ulong n);
void	superH_mov(Engine *E, State *S, ulong m,ulong n);
void	superH_mova(Engine *E, State *S, long d);
void	superH_movbl(Engine *E, State *S, ulong m, ulong n);
void	superH_movbl0(Engine *E, State *S, ulong m,ulong n);
void	superH_movbl4(Engine *E, State *S, ulong m, long d);
void	superH_movblg(Engine *E, State *S, long d);
void	superH_movbm(Engine *E, State *S, ulong m, ulong n);
void	superH_movbp(Engine *E, State *S, ulong m, ulong n);
void	superH_movbs(Engine *E, State *S, ulong m,ulong n);
void	superH_movbs0(Engine *E, State *S, ulong m, ulong n);
void	superH_movbs4(Engine *E, State *S, long d, ulong n);
void	superH_movbsg(Engine *E, State *S, long d);
void	superH_movi(Engine *E, State *S, long i, ulong n);
void	superH_movli(Engine *E, State *S, long d, ulong n);
void	superH_movll(Engine *E, State *S, ulong m, ulong n);
void	superH_movll0(Engine *E, State *S, ulong m, ulong n);
void	superH_movll4(Engine *E, State *S, ulong m, long d, ulong n);
void	superH_movllg(Engine *E, State *S, long d);
void	superH_movlm(Engine *E, State *S, ulong m, ulong n);
void	superH_movlp(Engine *E, State *S, ulong m,ulong n);
void	superH_movls(Engine *E, State *S, ulong m, ulong n);
void	superH_movls0(Engine *E, State *S, ulong m, ulong n);
void	superH_movls4(Engine *E, State *S, ulong m, long d,ulong n);
void	superH_movlsg(Engine *E, State *S, long d);
void	superH_movt(Engine *E, State *S, ulong n);
void	superH_movwi(Engine *E, State *S, long d, ulong n);
void	superH_movwl(Engine *E, State *S, ulong m, ulong n);
void	superH_movwl0(Engine *E, State *S, ulong m, ulong n);
void	superH_movwl4(Engine *E, State *S, ulong m, long d);
void	superH_movwlg(Engine *E, State *S, long d);
void	superH_movwm(Engine *E, State *S, ulong m, ulong n);
void	superH_movwp(Engine *E, State *S, ulong m, ulong n);
void	superH_movws(Engine *E, State *S, ulong m, ulong n);
void	superH_movws0(Engine *E, State *S, ulong m, ulong n);
void	superH_movws4(Engine *E, State *S, long d, ulong n);
void	superH_movwsg(Engine *E, State *S, long d);
void	superH_mull(Engine *E, State *S, ulong m,ulong n);
void	superH_muls(Engine *E, State *S, ulong m, ulong n);
void	superH_mulu(Engine *E, State *S, ulong m,ulong n);
void	superH_neg(Engine *E, State *S, ulong m,ulong n);
void	superH_negc(Engine *E, State *S, ulong m, ulong n);
void	superH_nop(Engine *E, State *S);
void	superH_not(Engine *E, State *S, ulong m,ulong n);
void	superH_or(Engine *E, State *S, ulong m,ulong n);
void	superH_ori(Engine *E, State *S, long i);
void	superH_orm(Engine *E, State *S, long i);
void	superH_sleep(Engine *E, State *S);
void	superH_pref(Engine *E, State *S, ulong n);
void	superH_rfg(Engine *E, State *S, long i);
void	superH_rotcl(Engine *E, State *S, ulong n);
void	superH_rotcr(Engine *E, State *S, ulong n);
void	superH_rotl(Engine *E, State *S, ulong n);
void	superH_rotr(Engine *E, State *S, ulong n);
void	superH_rte(Engine *E, State *S);
void	superH_rts(Engine *E, State *S);
void	superH_sets(Engine *E, State *S);
void	superH_sett(Engine *E, State *S);
void	superH_shad(Engine *E, State *S, ulong m, ulong n);
void	superH_shal(Engine *E, State *S, ulong n);
void	superH_shar(Engine *E, State *S, ulong n);
void	superH_shld(Engine *E, State *S, ulong m, ulong n);
void	superH_shll(Engine *E, State *S, ulong n);
void	superH_shll16(Engine *E, State *S, ulong n);
void	superH_shll2(Engine *E, State *S, ulong n);
void	superH_shll8(Engine *E, State *S, ulong n);
void	superH_shlr(Engine *E, State *S, ulong n);
void	superH_shlr16(Engine *E, State *S, ulong n);
void	superH_shlr2(Engine *E, State *S, ulong n);
void	superH_shlr8(Engine *E, State *S, ulong n);
void	superH_stcgbr(Engine *E, State *S, ulong n);
void	superH_stcmgbr(Engine *E, State *S, ulong n);
void	superH_stcmr_bank(Engine *E, State *S, int q, ulong m);
void	superH_stcmspc(Engine *E, State *S, ulong n);
void	superH_stcmsr(Engine *E, State *S, ulong n);
void	superH_stcmssr(Engine *E, State *S, ulong n);
void	superH_stcmvbr(Engine *E, State *S, ulong n);
void	superH_stcr_bank(Engine *E, State *S, int q, ulong m);
void	superH_stcspc(Engine *E, State *S, ulong n);
void	superH_stcsr(Engine *E, State *S, ulong n);
void	superH_stcssr(Engine *E, State *S, ulong n);
void	superH_stcvbr(Engine *E, State *S, ulong n);
void	superH_stsmach(Engine *E, State *S, ulong n);
void	superH_stsmacl(Engine *E, State *S, ulong n);
void	superH_stsmmach(Engine *E, State *S, ulong n);
void	superH_stsmmacl(Engine *E, State *S, ulong n);
void	superH_stsmpr(Engine *E, State *S, ulong n);
void	superH_stspr(Engine *E, State *S, ulong n);
void	superH_sub(Engine *E, State *S, ulong m,ulong n);
void	superH_subc(Engine *E, State *S, ulong m, ulong n);
void	superH_subv(Engine *E, State *S, ulong m,ulong n);
void	superH_swapb(Engine *E, State *S, ulong m, ulong n);
void	superH_swapw(Engine *E, State *S, ulong m, ulong n);
void	superH_tas(Engine *E, State *S, ulong n);
void	superH_trapa(Engine *E, State *S, long i);
void	superH_tst(Engine *E, State *S, ulong m, ulong n);
void	superH_tsti(Engine *E, State *S, long i);
void	superH_tstm(Engine *E, State *S, long i);
void	superH_xor(Engine *E, State *S, ulong m,ulong n);
void	superH_xori(Engine *E, State *S, long i);
void	superH_xorm(Engine *E, State *S, long i);
void	superH_xtrct(Engine *E, State *S, ulong m, ulong n);



/*											*/
/*			Microarchitecture  modeling: TI MSP430				*/
/*											*/
void	msp430decode(Engine *, State *S, ushort instr, MSP430Pipestage *p);
void	msp430dumpregs(Engine *, State *S);
void	msp430dumpsysregs(Engine *, State *S);
void	msp430fatalaction(Engine *, State *S);
void	msp430stallaction(Engine *, State *S);
void	msp430take_timer_intr(Engine *, State *S);
void	msp430resetcpu(Engine *, State *S);
State*	msp430newstate(Engine *E, double xloc, double yloc, double zloc, char *trajfilename);
ushort	msp430regread(Engine *E, State *S, int n, MSP430Pipestage *p);
void	msp430regset(Engine *E, State *S, int n, ushort data);
void	msp430writebyte(Engine *, State *S, ushort addr, uchar data);
void	msp430peripheralwritebyte(Engine *,State *S, ushort addr, uchar data);
void	msp430writeword(Engine *,State *S, ushort addr, ushort data);
void	msp430peripheralwriteword(Engine *,State *S, ushort addr, ushort data);
uchar	msp430readbyte(Engine *,State *S, ushort addr);
uchar	msp430peripheralreadbyte(Engine *,State *S, ushort addr);
ushort	msp430readword(Engine *,State *S, ushort addr);
ushort	msp430peripheralreadword(Engine *,State *S, ushort addr);
void	msp430flushpipe(State *S);
int	msp430step(Engine *, State *S, int);
int	msp430faststep(Engine *, State *S, int);
void	msp430dumppipe(Engine *, State *S);
void	msp430split(Engine *E, State *S, ulong startpc, ulong stackptr, ulong argaddr, char *idstr);

void	dev430x1xxreset(State *S, int type);
uchar	dev430x1xxreadbyte(Engine *, State *S, ushort addr);
void	dev430x1xxwritebyte(Engine *, State *S, ushort addr, uchar data);
ushort	dev430x1xxreadword(Engine *, State *S, ushort addr);
void	dev430x1xxwriteword(Engine *, State *S, ushort addr, ushort data);
void	dev430x1xxsetpin(State *S, int pin, double voltage);
double	dev430x1xxgetpin(State *S, int pin);
void	dev430x1xxPORreset(State *S);
void	dev430x1xxPUCreset(State *S);
void	dev430x1xxNMIinterrupt(State *S, int type);
void	dev430x1xxinterrupt(State *S, int priority);
void	dev430x1xxflashaccessviolation(State *S);
void	dev430x1xxflashreadbyte(State *S);
void	dev430x1xxflashwritebyte(State *S);
void	dev430x1xxoscfault(State *S);

/*									*/
/*			MSP430 instruction functions			*/
/*									*/
void	msp430_mov(Engine *E, State *S, ushort m, ushort n, MSP430Pipestage *p);
void	msp430_movb(Engine *E, State *S, ushort m, ushort n, MSP430Pipestage *p);
void	msp430_add(Engine *E, State *S, ushort m, ushort n, MSP430Pipestage *p);
void	msp430_addb(Engine *E, State *S, int m, int n, MSP430Pipestage *p);
void	msp430_addc(Engine *E, State *S, int m, int n, MSP430Pipestage *p);
void	msp430_addcb(Engine *E, State *S, int m, int n, MSP430Pipestage *p);
void	msp430_sub(Engine *E, State *S, int m, int n, MSP430Pipestage *p);
void	msp430_subb(Engine *E, State *S, int m, int n, MSP430Pipestage *p);
void	msp430_subc(Engine *E, State *S, int m, int n, MSP430Pipestage *p);
void	msp430_subcb(Engine *E, State *S, int m, int n, MSP430Pipestage *p);
void	msp430_cmp(Engine *E, State *S, int m, int n, MSP430Pipestage *p);
void	msp430_cmpb(Engine *E, State *S, int m, int n, MSP430Pipestage *p);
void	msp430_dadd(Engine *E, State *S, int m, int n, MSP430Pipestage *p);
void	msp430_daddb(Engine *E, State *S, int m, int n, MSP430Pipestage *p);
void	msp430_bit(Engine *E, State *S, int m, int n, MSP430Pipestage *p);
void	msp430_bitb(Engine *E, State *S, int m, int n, MSP430Pipestage *p);
void	msp430_bic(Engine *E, State *S, int m, int n, MSP430Pipestage *p);
void	msp430_bicb(Engine *E, State *S, int m, int n, MSP430Pipestage *p);
void	msp430_bis(Engine *E, State *S, int m, int n, MSP430Pipestage *p);
void	msp430_bisb(Engine *E, State *S, int m, int n, MSP430Pipestage *p);
void	msp430_xor(Engine *E, State *S, int m, int n, MSP430Pipestage *p);
void	msp430_xorb(Engine *E, State *S, int m, int n, MSP430Pipestage *p);
void	msp430_and(Engine *E, State *S, int m, int n, MSP430Pipestage *p);
void	msp430_andb(Engine *E, State *S, int m, int n, MSP430Pipestage *p);
void	msp430_rrc(Engine *E, State *S, ushort n, MSP430Pipestage *p);
void	msp430_rrcb(Engine *E, State *S, ushort n, MSP430Pipestage *p);
void	msp430_rra(Engine *E, State *S, ushort n, MSP430Pipestage *p);
void	msp430_rrab(Engine *E, State *S, ushort n, MSP430Pipestage *p);
void	msp430_push(Engine *E, State *S, ushort n, MSP430Pipestage *p);
void	msp430_pushb(Engine *E, State *S, ushort n, MSP430Pipestage *p);
void	msp430_swpb(Engine *E, State *S, ushort n, MSP430Pipestage *p);
void	msp430_call(Engine *E, State *S, ushort n, MSP430Pipestage *p);
void	msp430_reti(Engine *E, State *S, MSP430Pipestage *p);
void	msp430_sxt(Engine *E, State *S, ushort n, MSP430Pipestage *p);
void	msp430_jeqjz(Engine *E, State *S, short offset, MSP430Pipestage *p);
void	msp430_jnejnz(Engine *E, State *S, short offset, MSP430Pipestage *p);
void	msp430_jc(Engine *E, State *S, short offset, MSP430Pipestage *p);
void	msp430_jnc(Engine *E, State *S, short offset, MSP430Pipestage *p);
void	msp430_jn(Engine *E, State *S, short offset, MSP430Pipestage *p);
void	msp430_jge(Engine *E, State *S, short offset, MSP430Pipestage *p);
void	msp430_jl(Engine *E, State *S, short offset, MSP430Pipestage *p);
void	msp430_jmp(Engine *E, State *S, short offset, MSP430Pipestage *p);

/*									*/
/*	Microarchitecture and misc RISC-V  functions			*/
/*									*/
State*	riscvnewstate(Engine *E, double xloc, double yloc, double zloc, char *trajfilename);
void	riscvstallaction(Engine *, State *S, ulong addr, int type, int latency);
void    riscvdumpregs(Engine *E, State *S);
void	riscvdumppipe(Engine *E, State *S);
void	riscvflushpipe(State *S);
void	riscvIFflush(State *S);
void	riscvIFIDflush(State *S);
int	riscvstep(Engine *E, State *S, int drain_pipe);
int	riscvfaststep(Engine *E, State *S, int drain_pipe);
void	riscvdumphist(Engine *E, State *S, int histogram_id);
void	riscvdumphistpretty(Engine *E, State *S, int histogram_id);
void	riscvdumpdistribution(Engine *E, State *S);
void	riscvdecode(Engine *E, uint32_t instr, RiscvPipestage *stage);
uint32_t reg_read_riscv(Engine *E, State *S, uint8_t n);
void	reg_set_riscv(Engine *E, State *S, uint8_t n, uint32_t data);

/*									*/
/*			RISC-V instruction functions			*/
/*									*/
void 	riscv_nop(Engine *E, State *S);
void 	riscv_add(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void 	riscv_sub(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void 	riscv_slt(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void 	riscv_sltu(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void 	riscv_and(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void 	riscv_or(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void 	riscv_xor(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void 	riscv_sll(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void	riscv_srl(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void 	riscv_sra(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void 	riscv_addi(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t imm0);
void 	riscv_slti(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t imm0);
void 	riscv_sltiu(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t imm0);
void 	riscv_andi(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t imm0);
void 	riscv_ori(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t imm0);
void 	riscv_xori(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t imm0);
void	riscv_slli(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t imm0);
void	riscv_srli(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t imm0);
void	riscv_srai(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint32_t imm0);
void 	riscv_lui(Engine *E, State *S, uint8_t rd, uint32_t imm0);
void 	riscv_auipc(Engine *E, State *S, uint8_t rd, uint32_t imm0);
void 	riscv_jal(Engine *E, State *S, uint8_t rd, uint16_t imm1, uint8_t imm11, uint8_t imm12, uint8_t imm20);
void 	riscv_jalr(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0);
void 	riscv_beq(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t imm1, uint8_t imm5, uint8_t imm11, uint8_t imm12);
void 	riscv_bne(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t imm1, uint8_t imm5, uint8_t imm11, uint8_t imm12);
void 	riscv_blt(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t imm1, uint8_t imm5, uint8_t imm11, uint8_t imm12);
void 	riscv_bltu(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t imm1, uint8_t imm5, uint8_t imm11, uint8_t imm12);
void 	riscv_bge(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t imm1, uint8_t imm5, uint8_t imm11, uint8_t imm12);
void 	riscv_bgeu(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t imm1, uint8_t imm5, uint8_t imm11, uint8_t imm12);
void 	riscv_lw(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0);
void 	riscv_lh(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0);
void 	riscv_lhu(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0);
void 	riscv_lb(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0);
void 	riscv_lbu(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0);
void 	riscv_sw(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint16_t imm0, uint16_t imm5);
void 	riscv_sh(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint16_t imm0, uint16_t imm5);
void 	riscv_sb(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint16_t imm0, uint16_t imm5);
void 	riscv_fence(Engine *E, State *S);
void 	riscv_fence_i(Engine *E, State *S);
void 	riscv_cor(Engine *E, State *S);
void 	riscv_ecall(Engine *E, State *S);
void 	riscv_ebreak(Engine *E, State *S);
void 	riscv_csrrw(Engine *E, State *S);
void 	riscv_csrrs(Engine *E, State *S);
void 	riscv_csrrc(Engine *E, State *S);
void 	riscv_csrrwi(Engine *E, State *S);
void 	riscv_csrrsi(Engine *E, State *S);
void 	riscv_csrrci(Engine *E, State *S);

/*													*/
/*			RISC-V RV32M additional functions		*/
/*													*/
void riscv_mul(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void riscv_mulh(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void riscv_mulhsu(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void riscv_mulhu(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void riscv_div(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void riscv_divu(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void riscv_rem(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void riscv_remu(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);

/*									*/
/*			RISC-V RV32F additional functions		*/
/*									*/
uint64_t freg_read_riscv(Engine *E, State *S, uint8_t n);
void	freg_set_riscv(Engine *E, State *S, uint8_t n, uint64_t data);
void	rv32f_flw(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0);
void	rv32f_fsw(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint16_t imm0, uint16_t imm5);
void	rv32f_fmadd_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rs3, uint8_t rm, uint8_t rd);
void	rv32f_fmsub_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rs3, uint8_t rm, uint8_t rd);
void	rv32f_fnmsub_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rs3, uint8_t rm, uint8_t rd);
void	rv32f_fnmadd_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rs3, uint8_t rm, uint8_t rd);
void	rv32f_fadd_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void	rv32f_fsub_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void	rv32f_fmul_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void	rv32f_fdiv_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void	rv32f_fsqrt_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void	rv32f_fsgnj_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void	rv32f_fsgnjn_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void	rv32f_fsgnjx_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void	rv32f_fmin_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void	rv32f_fmax_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void	rv32f_fcvt_w_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void	rv32f_fcvt_wu_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void	rv32f_fmv_x_w(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void	rv32f_feq_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void	rv32f_flt_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void	rv32f_fle_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void	rv32f_fclass_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void	rv32f_fcvt_s_w(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void	rv32f_fcvt_s_wu(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void	rv32f_fmv_w_x(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);

/*									*/
/*			RISC-V RV32D additional functions		*/
/*									*/
void rv32d_fld(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0);
void rv32d_fsd(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint16_t imm0, uint16_t imm5);
void rv32d_fmadd_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rs3, uint8_t rm, uint8_t rd);
void rv32d_fmsub_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rs3, uint8_t rm, uint8_t rd);
void rv32d_fnmsub_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rs3, uint8_t rm, uint8_t rd);
void rv32d_fnmadd_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rs3, uint8_t rm, uint8_t rd);
void rv32d_fadd_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void rv32d_fsub_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void rv32d_fmul_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void rv32d_fdiv_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void rv32d_fsqrt_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void rv32d_fsgnj_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void rv32d_fsgnjn_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void rv32d_fsgnjx_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void rv32d_fmin_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void rv32d_fmax_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void rv32d_fcvt_s_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void rv32d_fcvt_d_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void rv32d_feq_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void rv32d_flt_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void rv32d_fle_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void rv32d_fclass_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void rv32d_fcvt_w_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void rv32d_fcvt_wu_d(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void rv32d_fcvt_d_w(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void rv32d_fcvt_d_wu(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);

/*									*/
/*			RISC-V uncertain additional functions		*/
/*									*/
void rv32un_unupg_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void rv32un_ungcov_s(Engine *E, State *S, uint8_t rs1, uint8_t rs2, uint8_t rd);
void rv32un_unsvar_s(Engine *E, State *S, uint8_t rs1, uint8_t _rs2, uint8_t rd);
void rv32un_unclvar_s(Engine *E, State *S, uint8_t _rs1, uint8_t _rs2, uint8_t rd);
void rv32un_uncpvar_s(Engine *E, State *S, uint8_t _rs1, uint8_t _rs2, uint8_t rd);
void rv32un_un_part1(Engine *E, State *S, uint8_t rs1, uint8_t rd, uint16_t imm0);


/*
 *		Histogram arithmetic.
 */
void		Histogram_AddDist(Engine *E, State *S, Histogram *hist1, Histogram *hist2, Histogram *histDest);
void		Histogram_ScalarMultiply(Engine *E, State *S, Histogram *hist, HistogramBinDatatype scalar);
void		Histogram_SubDist(Engine *E, State *S, Histogram *hist1, Histogram *hist2, Histogram *histDest);
void		Histogram_CombDist(Engine *E, State *S, Histogram *hist1, Histogram *hist2, Histogram *histDest);
int		Histogram_LowerBound(Engine *E, State *S, Histogram *hist);
int		Histogram_UpperBound(Engine *E, State *S, Histogram *hist);
void		Histogram_DistLShift(Engine *E, State *S, Histogram *hist1, uint8_t Rs2, Histogram *histDest);
void		Histogram_DistRShift(Engine *E, State *S, Histogram *hist1, uint8_t Rs2, Histogram *histDest);
uint8_t		Histogram_ExpectedValue(Engine *E, State *S, Histogram *hist);
uint32_t	Histogram_DistLess(Engine *E, State *S, Histogram *hist, uint32_t Rs2);
uint32_t	Histogram_DistGrt(Engine *E, State *S, Histogram *hist, uint32_t Rs2);
void		Histogram_LDDist(Engine *E, State *S, Histogram *histogram, HistogramBinDatatype bins[kUncertainAluHistogramBins]);
void 		Histogram_LDRandom(Engine *E, State *S, Histogram *histogram);
double		Histogram_Mean(Engine *E, State *S, Histogram *histogram);
void		Histogram_PrettyPrint(Engine *E, State *S, Histogram *histogram);
double		Histogram_MeanFrequency(Engine *E, State *S, Histogram *histogram);

/*
 *	Uncertainty propagation equation arithmetic
 */
int		uncertain_print_system(UncertainState * state, FILE *stream);
void		uncertain_sizemem(Engine *E, State *S, int size);
void		uncertain_inst_lr(UncertainState * state, int ud, int location);
void		uncertain_inst_sr(UncertainState * state, int us1, int location);
void		uncertain_inst_mv(UncertainState * state, int ud, int us1);
void		uncertain_inst_up1(UncertainState * state, int ud, int us1, float g1);
void		uncertain_inst_up2(UncertainState * state, int ud, int us1, int us2, float g1, float g2);
float		uncertain_inst_gv(UncertainState * state, int us1);
float		uncertain_inst_gcov(UncertainState * state, int us1, int us2);
void		uncertain_inst_sv(UncertainState * state, int ud, float variance);
