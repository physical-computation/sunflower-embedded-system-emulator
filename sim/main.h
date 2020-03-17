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

#define MAXCLOCK		(~(vlong)0)

#ifndef M_E
#	define	M_E		2.7182818284590452354
#endif
#ifndef MAXFLOAT
#	define MAXFLOAT		((float)3.40282346638528860e+38)
#endif
#ifndef DBL_MIN
#	define DBL_MIN		((double)2.2250738585072014E-308)
#endif
#ifndef DBL_MAX
#	define DBL_MAX		((double)1.7976931348623157e+308)
#endif
#ifndef DBL_EPSILON
#	define DBL_EPSILON	((double)2.2204460492503131e-16)
#endif

/*
	We are yet to make the switch to picoseconds
	#define	PICOSEC_MAX		(~(Picosec)0)
*/
#define	PICOSEC_MAX		DBL_MAX


/*	WIN32 compiler doesn't always know 'inline', and already has min/max */
#ifdef	_WIN32_WINNT
#	define	tuck
#else
#	define	tuck		inline
#	define	max(x,y)	((x) > (y) ? (x) : (y))
#	define	min(x,y)	((x) < (y) ? (x) : (y))
#endif


#define eventready(curtime, lastactivate, period)\
				((((curtime) - (lastactivate)) >= (period)) ? 1 : 0)

#define	SF_LOGFILENAME		"sunflower.out"

#define SF_FILE_MACRO		__FILE__

#ifdef LIBSF
#	define DBLFMT		"%E"
#	define INTFMT		"%d"
#	define UINTFMT		"%ud"
#	define UVLONGFMT	"%llud"
#	define ULONGFMT		"%lud"
#	define UHLONGFMT	"%lux"
#	define UH8LONGFMT	"%08lux"
#	define UH2XFMT		"%02X"
#else
#	define DBLFMT		"%E"
#	define INTFMT		"%d"
#	define UINTFMT		"%u"
#	define UVLONGFMT	"%llu"
#	define ULONGFMT		"%lu"
#	define UHLONGFMT	"%lx"
#	define UH8LONGFMT	"%08lx"
#	define UH2XFMT		"%02X"
#endif


/*
 *	NOTE: The code base currently violates our coding convention
 *	but this will be fixed soon. Some new constants (e.g., kUncertainAluHistogramBins)
 *	are defined according to the convention.
 */
enum
{
	DEFAULT_MEMREAD_LATENCY		= 0,
	DEFAULT_MEMWRITE_LATENCY	= 0,
	DEFAULT_FLASHREAD_LATENCY	= 0,
	DEFAULT_FLASHWRITE_LATENCY	= 0,
	DEFLT_MEMSIZE			= 1<<24,
	DEFLT_FLASHSIZE			= 1<<4,
};

enum
{
	MEM_ACCESS_NIL = 0,
	MEM_ACCESS_IFETCH,
	MEM_ACCESS_FAST_IFETCH,
	MEM_ACCESS_FAST_DELAYSLOT,
};

enum
{
	MEM_READ_BYTE,
	MEM_READ_WORD,
	MEM_READ_LONG,
	MEM_WRITE_BYTE,
	MEM_WRITE_WORD,
	MEM_WRITE_LONG,

	MEM_READ_STALL,
	MEM_WRITE_STALL,
};

enum
{
	MAX_SIMNODES			= 32,
	MAX_NODE_DIGITS			= 8,
	MAX_RUN_ARGS			= 64,
	MAX_SREC_LINELEN		= 1024,
	MAX_NAMELEN			= 128,
	MAX_NUMA_REGIONS		= 8192,
	MAX_REG_TRACERS			= 8192,
	MAX_CMD_LEN			= 128,
	MAX_NUMAREGION_NAMELEN		= 64,
	MAX_REGTRACER_NAMELEN		= 64,
	BASE_NUMAREGION_VALUETRACE	= 65535,
	BASE_REGTRACER_VALUETRACE	= 65535,
	BASE_REGTRACER_DEVNTRACE	= 65535,
	MAX_NUMAREGION_VALUETRACE	= 1048576,
	MAX_REGTRACER_VALUETRACE	= 1048576,
	MAX_PCSTACK_HEIGHT		= 8192,
	MAX_FPSTACK_HEIGHT		= 8192,
	MAX_RANDTABLEENTRIES		= 8192,
	MAX_RVARENTRIES			= 128,
	MAX_NUM_ENGINES			= 4,
	MAX_BREAKPOINTS			= 32,

	/*	Keep sorted in order, so last entry is max	*/
	MAX_NODESTDOUT_BUFSZ		= 8192,
	MAX_NODESTDERR_BUFSZ		= 8192,
	MAX_NODEINFO_BUFSZ		= 8192,
	MAX_SIM_INFO_BUFSZ		= 8192,
	MAX_MIO_BUFSZ,
};

typedef enum
{
	kSunflowerConstantsPropulsionModelCoefficientCount = 6,
} SunflowerConstants;

typedef enum
{
	SchedRoundRobin,
	SchedRandom,
} SchedType;

typedef enum
{
	Big,
	Little,
} Endianness;

typedef enum
{
	MACHINE_SUPERH,
	MACHINE_MSP430,
	MACHINE_RISCV,
} Machinetype;

enum
{
	nodestdout,
	nodestderr,
	nodeinfo,
	siminfo,
};

enum
{
	M_OREAD		= 1 << 0,
	M_OWRITE	= 1 << 1,
	M_OTRUNCATE	= 1 << 2,
};

typedef enum
{
	BPT_CYCLES,
	BPT_GLOBALTIME,
	BPT_INSTRS,
	BPT_SENSORREADING,
} Breaktype;

typedef enum
{
	kSunflowerTaintModeLiberal,
	kSunflowerTaintModeConservative,
} SunflowerTaintMode;





typedef struct State State;
typedef struct Engine Engine;
typedef struct SEEstruct SEEstruct;






typedef struct
{
	char	*cmd;
	char	*intro;
	char	*args;
} Help;

/*		Translated virtual address		*/
typedef struct
{
	ulong	paddr;
	ulong	vaddr;
	int	cacheable;

	int	error;
} TransAddr;

/*	Holds parsed output of objdump ease debugging	*/
typedef struct
{
	ulong	addr;
	char	*bytesstr;
	char	*asmstr;

	int	lineno;
} DisasmAddr;

typedef struct
{
	ulong		startaddr;
	ulong		endaddr;

	char		*filename;
	char		*fxnname;

	DisasmAddr	*disaddrs;
} DisasmRegion;

/*	Non-uniform memory access properties		*/
typedef struct
{
	/*			Boundary.		*/
	ulong		startaddr;
	ulong		endaddr;

	/*	For addresses on specific fn stacks	*/
	int		onstack;
	int		frameoffset;
	ulong		pcstart;

	/*						*/
	/*	Private entries only match when mapid	*/
	/*	is same as CPU ID			*/
	/*						*/
	int		private;

	/*	Actual accesses to this CPU's mem	*/
	int		map_id;
	ulong		map_offset;


	ulong		local_read_latency;
	ulong		local_write_latency;
	ulong		remote_read_latency;
	ulong		remote_write_latency;

	/*	Variable value traces			*/
	int		ispointer;
	int		valuetrace;
	int		validx;
	ulong		*values;

	/*		Resized at runtime		*/
	int		nvalues;


	/*	Access Statistics			*/
	ulong		nreads;
	ulong		nwrites;

	/*	Useful when analyzing statistics	*/
	char		name[MAX_NUMAREGION_NAMELEN];
} Numaregion;

typedef struct
{
	Numaregion	**regions;
	int		count;
} Numa;

typedef struct
{
	int		ispointer;
	int		size;
	char		name[MAX_REGTRACER_NAMELEN];
	int		regnum;
	int		validx;
	ulong		*values;
	ulong		*devns;

	/*	May be resized at runtime		*/
	int		nvalues;
	int		ndevns;

	ulong		pcstart;
} Regvt;

typedef struct
{
	Regvt		**regvts;
	int		count;
} Regtraces;

typedef struct
{
	int	rlat;
	int	wlat;
	uvlong	size;
	uvlong	base;
	uvlong	end;

	uchar	*data;
} Mem;

typedef struct
{
	union
	{
		struct	/*	dval	*/
		{
			double	dval;
		};

		struct	/*	uval	*/
		{
			ulong	uval;
		};

		struct	/*	sval	*/
		{
			long	sval;
		};

		struct	/*	str	*/
		{
			char	*str;
		};
	};

	struct
	{
		int	pfunid;
		int	disttabid;

		double	p1;
		double	p2;
		double	p3;
		double	p4;
	} rv;
} Rval;

typedef struct
{
	char	*name;
	double	*table;
	int	size;
} RandTable;

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

	/*	parameters for a particular instance of a pfun	*/
	double	min;
	double	max;
	double	p1;
	double	p2;
	double	p3;
	double	p4;

	char	*name;
	void	*addr;

	Picosec	nextupdate;
} Rvar;

struct SEEstruct
{
	uchar*		hw;

	/*	Size of structure, in bits	*/
	int		actual_bits;

	/*					*/
	/*	Number of machine bits that	*/
	/*	structure logically represents	*/
	/*					*/
	int		logical_bits;

	/*					*/
	/*	Location of struct in space	*/
	/*	of logical bits.		*/
	/*					*/
	int		logical_offset;


	/*					*/
	/*	To enable treating different	*/
	/*	portions of a hw structure	*/
	/*	differently.			*/
	/*					*/
	int		bit_offset;


	SEEstruct	*next;
};

typedef struct
{
	SEEstruct	*hd;
	SEEstruct	*tl;

	int		logical_bits;
	int		actual_bits;

	double		(*loc_pfun)(State *S, double min, double max,
					double p1, double p2, double p3, double p4);
	double		loc_pfun_p1;
	double		loc_pfun_p2;
	double		loc_pfun_p3;
	double		loc_pfun_p4;

	double		(*bit_pfun)(State *S, double min, double max,
					double p1, double p2, double p3, double p4);
	double		bit_pfun_p1;
	double		bit_pfun_p2;
	double		bit_pfun_p3;
	double		bit_pfun_p4;

	double		(*dur_pfun)(State *S, double min, double max,
					double p1, double p2, double p3, double p4);
	double		dur_pfun_p1;
	double		dur_pfun_p2;
	double		dur_pfun_p3;
	double		dur_pfun_p4;
} SEEstate;

typedef struct
{
	double		*xloc;
	double		*yloc;
	double		*zloc;

	double		*rho;
	double		*theta;
	double		*phi;

	int		nlocations;
	int		trajectory_rate;
	int		looptrajectory;
} Path;

struct State
{
	/*								*/
	/*			Machine Specific blocks			*/
	/*								*/
	SuperHState	*superH;
	MSP430State	*msp430;
	RiscvState	*riscv;


	Machinetype	machinetype;
	Endianness	endian;


	uchar		*MEM;
	int		mem_r_latency;
	int		mem_w_latency;
	int		flash_r_latency;
	int		flash_w_latency;
	int		MEMSIZE;
	int		MEMBASE;
	int		MEMEND;
	int		MEM_DATA_SEGMENT_END;

	/*
	*	Shadow Memory structure allocation
	*/
	ShadowMem	*TAINTMEM;
	int		TAINTMEMSIZE;
	int		TAINTMEMBASE;
	int		TAINTMEMEND;


	//	TODO:
	Mem		*M;
	int		nmems;

	/*								*/
	/*	Each State * CPU structure maintains a coroutine	*/
	/*	'continuation'.  This is used to interrupt the		*/
	/*	simulation of CPU at points where we need to stop	*/
	/*	doing the current cycle's duties and go simulate	*/
	/*	other processors. E.g., if in the middle of simulating	*/
	/*	an instruction (say, TAS), we must take a cache miss	*/
	/*	before finishing the rest of the instruction, we	*/
	/*	park the coroutine in the rack and return to that	*/
	/*	position when approapriate.				*/
	/*								*/
	jmp_buf		rack;


	/*	Signal transition counting		*/
	ulong		regfile_port;
	uvlong		Cycletrans;


	/*	Instruction-level power analysis	*/
	EnergyInfo	energyinfo;
	double		scaledcurrents[SUPERH_OP_MAX];/*	Currently, power data is only for superH	*/

	/*		The operating voltage.		*/
	double		VDD;
	/*	The default low operating voltage.	*/

	double		LOWVDD;
	/*		Saved VDD. see pau.c		*/
	double		SVDD;

	double		temperature;
	double		voltscale_alpha;
	double		voltscale_Vt;
	double		voltscale_K;


	int		ENABLE_BATT_LOW_INTR;
	double		battery_alert_frac;
	void		*BATT;

	/*		Sensors for sensing physical phenomena		*/
	Sensor		sensors[MAX_NODE_SENSORS];


	/*						*/
	/*	Override average power, and allow	*/
	/*	independing vdd/freq			*/
	/*						*/
	double		force_avgpwr;
	double		force_sleeppwr;


	/*	Non-uniform memory access ref		*/
	Numa		*N;
	Numa		*Nstack;


	/*	Register value tracing			*/
	Regtraces	*RT;


	uvlong		ICLK;
	uvlong		CLK;
	uvlong		last_stepclks;
	double		TIME;
	double		CYCLETIME;
	uvlong		dyncnt;
	uvlong		nfetched;


	InterruptQ	*intrQ;

	uint32_t		PC;
	ulong		STARTPC;
	ulong		PCSTACK[MAX_PCSTACK_HEIGHT];
	int		pcstackheight;
	ulong		FPSTACK[MAX_FPSTACK_HEIGHT];
	int		fpstackheight;


	int		NODE_ID;
	char		idstr[MAX_NAMELEN];


	/*			Failure Stuff				*/
	double		fail_prob;
	Pdist		failure_dist;
	Pdist		failure_duration_dist;
	uvlong		failure_duration_max;
	uvlong		fail_clocks_left;
	int		got_correlated_failure;
	uvlong		nfaults;
	uvlong		faultthreshold;
	int		ENABLE_TOO_MANY_FAULTS;
	SEEstate*	SEEmodeling;

	/*			Division off SIM_GLOBAL_CLOCK		*/
	int		clock_modulus;


	/*				Logging				*/
	int		logfd;
	int		trace;


	/*			Per node stdout/stderr buffers		*/
	char		stdout_buf[MAX_NODESTDOUT_BUFSZ];
	int		stdout_h2o;
	char		stderr_buf[MAX_NODESTDERR_BUFSZ];
	int		stderr_h2o;
	char		nodeinfo_buf[MAX_NODEINFO_BUFSZ];
	int		nodeinfo_h2o;



	int		runnable;
	int		pipelined;
	int		pipeshow;
	int		sleep;

	/*			Timing stuff				*/
	ulong		ustart, ufinish;
	uvlong		startclk, finishclk;
	uvlong		trip_ustart, trip_startclk;
	uvlong		num_cycles_waiting;

	/*			Topology Information			*/
	double		xloc;
	double		yloc;
	double		zloc;
	double		rho;
	double		theta;
	double		phi;
	Path		path;

	/*			Trajectory/headings input		*/
	char		*trajfilename;


	/*			Mass					*/
	double		nodeMass;


	/*			Propulsion model coefficients		*/
	double		propulsionXvectorCoefficients[kSunflowerConstantsPropulsionModelCoefficientCount];
	double		propulsionYvectorCoefficients[kSunflowerConstantsPropulsionModelCoefficientCount];
	double		propulsionZvectorCoefficients[kSunflowerConstantsPropulsionModelCoefficientCount];


	/*	Set if acting as a dummy from remote_seg_enqueue()	*/
	int		from_remote;


	/*			Buffer for rabbit hole			*/
	char		cmdbuf[MAX_CMD_LEN];
	int		cmdbuf_nbytes;


	/*	Pointer to function to relevant step() routine		*/
	int		(*step)(Engine *, State *, int);
	int		(*cyclestep)(Engine *, State *, int);
	int		(*faststep)(Engine *, State *, int);

	/*	Pointer to function for failure prob dist		*/
	uvlong		(*pfun)(void *, void *, char *, uvlong);

	/*	The routines to handle the different interrupts		*/
	int		(*take_timer_intr)(Engine *, State *S);
	int		(*take_nic_intr)(Engine *, State *S);
	int		(*take_batt_intr)(Engine *, State *S);
	int		(*check_nic_intr)(Engine *, State *S);
	int		(*check_batt_intr)(Engine *, State *S);

	/*		Other misc machine specific actions		*/
	void		(*fatalaction)(Engine *, State *S);
	void		(*dumpregs)(Engine *, State *S);
	void		(*dumpsysregs)(Engine *, State *S);
	void		(*resetcpu)(Engine *, State *S);
	void		(*dumppipe)(Engine *, State *S);
	void		(*dumpdistribution)(Engine *, State *S);
	void		(*flushpipe)(State *S);

	/*
	 *	Histogram-specific actions
	 */
	void		(*dumphist)(Engine *, State *S, int histogram_id);
	void		(*dumphistpretty)(Engine *, State *S, int histogram_id);
	void		(*ldhistrandom)(Engine *, State *S, int histogram_id);
	void		(*addhist)(Engine *, State *S, int histogram_id0, int histogram_id1, int histogram_id2);

	/*	Memory mapped device register read/write functions	*/
	uchar		(*devreadbyte)(Engine *, State *S, ulong addr);
	ushort		(*devreadword)(Engine *, State *S, ulong addr);
	ulong		(*devreadlong)(Engine *, State *S, ulong addr);
	void		(*devwritebyte)(Engine *, State *S, ulong addr, uchar data);
	void		(*devwriteword)(Engine *, State *S, ulong addr, ushort data);
	void		(*devwritelong)(Engine *, State *S, ulong addr, ulong data);

	/*		Split CPU into 2 shared memory CPUs		*/
	void		(*split)(Engine *, State *S, ulong startpc, ulong stackaddr, ulong argaddr, char *idstr);

	/*		Address translation / MMU routines		*/
	void		(*vmtranslate)(Engine *, State *S, int op, TransAddr *transladdr);

	/*	This is the function called on a cache miss		*/
	void		(*stallaction)(Engine *, State *S, ulong addr, int type, int latency);

	void		(*settimerintrdelay)(Engine *, State *S, int delay);

	int		(*cache_init)(Engine *, State *S, int size, int blocksize, int assoc);
	void		(*cache_deactivate)(Engine *, State *S);
	void		(*cache_printstats)(Engine *, State *S);
	void		(*dumptlb)(Engine *, State *S);

	void		(*writebyte)(Engine *, State *S, ulong addr, ulong data);
};

typedef struct
{
	Breaktype	type;
	int		valid;

	union
	{
		struct
		{
			int	nodeid;
			uvlong	dyncnt;
		} instrsbpt;

		struct
		{
			int	nodeid;
			uvlong	cycles;
		} cyclesbpt;

		struct
		{
			int	nodeid;
			int	sensor;
			double	value;
		} sensorreadingbpt;

		Picosec	globaltime;
	};
} Breakpoint;

struct Engine
{
	/*		String supplied in devsunflower attach		*/
	char		*attachspec;


	/*			Parsed input stream			*/
	Input		istream;
	Labels		labellist;

	/*				Decode caches			*/
	SuperHDCEntry	superHDC[1<<16];
	MSP430DCEntry	msp430DC[1<<16];


	/*		Do not spawn new thread on 'ON' command		*/
	int		nodetach;


	/*	Throttling simulation speed in distributed sim.		*/
	ulong		throttlensec;
	ulong		throttlewin;

	/*		Random number generator per-node state		*/
	uvlong		*randgen_mt;
	int		randgen_mti;

	/*		Table-based rnum generation			*/
	RandTable	randtabs[MAX_RANDTABLEENTRIES];
	int		nrandtabs;
	Picosec		rvarsnextpsec;
	Rvar*		rvars[MAX_RVARENTRIES];
	int		validrvars[MAX_RVARENTRIES];
	int		nvalidrvars;
	int		randsched[MAX_SIMNODES];


	/*	The collection of modeled processors/systems		*/
	int		baseid;			/*	base id for numbering	*/
	State		*sp[MAX_SIMNODES];	/*	array of node ptrs	*/
	State		*cp;			/*	pointer to current	*/
	int		cn;			/*	current node id		*/

	/*		Miscellaneous whole-simulation state		*/
	int		quantum;		/*	sim quantum		*/
	int		scanning;
	int		nnodes;
	int		on;
	jmp_buf		jmpbuf;			/*	for managing main loop	*/
	char		infobuf[MAX_SIM_INFO_BUFSZ];
	int		infoh2o;
	Picosec		globaltimepsec;
	Picosec		mincycpsec;
	Picosec		maxcycpsec;
	uvlong		randseed;

	/*				Behavior settings		*/
	int		ignoredeaths;
	SchedType	schedtype;


	/*				Failure				*/
	Picosec		fperiodpsec;
	Picosec		flastpsec;

	/*				Network				*/
	Netsegment	netsegs[MAX_NETSEGMENTS];
	int		nnetsegs;
	int		activensegs[MAX_NETSEGMENTS];
	int		nactivensegs;
	uvlong		nicsimbytes;
	char		netiobuf[MAX_NETIO_NBUFS][MAX_SEGBUF_TEXT];
	int		netioh2o;

	/*								*/
	/*	At 8 Mb/s, each byte is transferred in 1 us, thus	*/
	/*	period of < 1us is OK for simulating network speeds	*/
	/*	which are < 8 Mb/s. The period is set automatically	*/
	/*	in network.c, based on the min netseg speed (that's	*/
	/*	why we initialize to PICOSEC_MAX).			*/
	/*								*/
	Picosec		netperiodpsec;
	Picosec		netlastpsec;

	/*			Physical Phenomena			*/
	Signalsrc	sigsrcs[MAX_SIGNAL_SRCS];
	int		nsigsrcs;
	Picosec		phylastpsec;
	Picosec		phyperiodpsec;


	/*			Node trajectory kickers			*/
	Picosec		trajperiodpsec;
	Picosec		trajlastpsec;

	/*				Power/Batt.			*/
	Batt		batts[MAX_BATTERIES];
	int		nbatts;
	int		curbatt;
	Batt*		activebatts[MAX_BATTERIES];
	int		nactivebatts;
	int		ndepletedbatts;
	int		verbose;
	Picosec		battlastpsec;
	Picosec		battperiodpsec;
	Picosec		dumplastpsec;
	Picosec		dumpperiodpsec;



#ifdef LIBSF
	int		sched_pid;
#else
	pthread_t	sched_handle;
#endif


	/*		Malloc accounting		*/
	MMemblock	memblocks[MMALLOC_MAXALLOCBLOCKS];
	int		nmemblocks;


	/*		Breakpoint			*/
	Breakpoint	bpts[MAX_BREAKPOINTS];
	int		validbpts[MAX_BREAKPOINTS];
	int		nvalidbpts;

	char		*logfilename;
};

