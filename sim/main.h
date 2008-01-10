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
#define MAXCLOCK		(~(vlong)0)

#ifndef M_E
#	define	M_E		2.7182818284590452354
#endif
#ifndef MAXFLOAT
#	define MAXFLOAT        	((float)3.40282346638528860e+38)
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


#ifdef	_WIN32_WINNT
#	define	tuck
#	define	snprintf snprint
#	define	max(x,y)	((x) > (y) ? (x) : (y))
#	define	min(x,y)	((x) < (y) ? (x) : (y))
#else
#	define	tuck		inline
#	define	min(x,y)	((x) < (y) ? (x) : (y))
#	define	max(x,y)	((x) > (y) ? (x) : (y))
#endif

#define eventready(curtime, lastactivate, period)\
				((((curtime) - (lastactivate)) >= (period)) ? 1 : 0)

#ifdef LIBSF
#	define UVLONGFMT	"%llud"
#	define ULONGFMT		"%lud"
#	define UHLONGFMT	"%lux"
#else
#	define UVLONGFMT	"%llu"
#	define ULONGFMT		"%lu"
#	define UHLONGFMT	"%lx"
#endif


enum
{
	DEFAULT_MEMREAD_LATENCY		= 100,
	DEFAULT_MEMWRITE_LATENCY	= 100,
	DEFAULT_FLASHREAD_LATENCY	= 1,
	DEFAULT_FLASHWRITE_LATENCY	= 100,
	DEFLT_MEMSIZE			= 1<<20,
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
	MAX_SIMNODES			= 1024,		
	MAX_NODE_DIGITS			= 8,
	MAX_RUN_ARGS			= 64,		
	MAX_SREC_LINELEN		= 1024,
	MAX_NAMELEN			= 128,
	MAX_NUMA_REGIONS		= 8192,
	MAX_REG_TRACERS			= 8192,
	MAX_CMD_LEN			= 128,
	MAX_NUMAREGION_NAMELEN		= 128,
	MAX_REGTRACER_NAMELEN		= 128,
	BASE_NUMAREGION_VALUETRACE	= 1024,
	BASE_REGTRACER_VALUETRACE	= 1024,
	MAX_NUMAREGION_VALUETRACE	= 1048576,
	MAX_REGTRACER_VALUETRACE	= 1048576,
	MAX_PCSTACK_HEIGHT		= 1024,
	MAX_FPSTACK_HEIGHT		= 1024,

	/*	Maximum freq modeled is 1 Terahertz. Yeah.	*/
	MAX_FREQ_MHZ = 1000000,

	/*	Keep sorted in order, so last entry is max	*/
	MAX_NODESTDOUT_BUFSZ		= 8192,
	MAX_NODESTDERR_BUFSZ		= 8192,
	MAX_NODEINFO_BUFSZ		= 8192,
	MAX_SIM_INFO_BUFSZ		= 8192,
	MAX_MIO_BUFSZ,
};

enum
{
	LOG_STEP	= 10,
};

enum
{
	MOBILITY_RANDOM,
};

enum
{
	SIM_LOC_XMAX = 1000,
	SIM_LOC_YMAX = 1000,
	SIM_LOC_ZMAX = 1000,
};

typedef struct
{
	char	*cmd;
	char	*intro;
	char	*args;
} Help;

typedef enum
{
	Big,
	Little,
} Endianness;

typedef enum
{
	MACHINE_SUPERH,
	MACHINE_MSP430,
} Machinetype;

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

	/*	May be resized at runtime		*/
	int		nvalues;

	ulong		pcstart;
} Regvt;

typedef struct
{
	Regvt		**regvts;
	int		count;
} Regtraces;

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


typedef struct
{
	int	rlat;
	int	wlat;
	uvlong	size;
	uvlong	base;
	uvlong	end;

	uchar	*data;
} Mem;

typedef struct State State;
struct State
{
	/*								*/
	/*			Machine Specific blocks			*/
	/*								*/
	SuperHState	*superH;
	MSP430State	*msp430;


	Machinetype	machinetype;
	Endianness	endian;

	
	uchar		*MEM;
	int		mem_r_latency;
	int		mem_w_latency;
	int		MEMSIZE;
	int		MEMBASE;
	int		MEMEND;


	/*	TODO: replace MEM array with these	*/
	Mem		*M;
	int		nmems;

	jmp_buf		rack;


	/*	Signal transition counting		*/
	ulong		regfile_port;
	uvlong		Cycletrans;


	/*	Instruction-level power analysis	*/
	EnergyInfo	E;
	double		scaledcurrents[OP_MAX];

	double		VDD;
	double		LOWVDD;			
	double		SVDD;			
	double		temperature;
	double		voltscale_alpha;
	double		voltscale_Vt;
	double		voltscale_K;

	/*		Sensors for sensing physical phenomena		*/
	Sensor		sensors[MAX_NODE_SENSORS];


	/*						*/
	/*	Override average power, and allow 	*/
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

	
	InterruptQ	*intrQ;

	ulong 		PC;
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

	/*			Division off SIM_GLOBAL_CLOCK		*/
	int		clock_modulus;

	
	/*			Whether we accept low batt interrupts	*/
	int		ENABLE_BATT_LOW_INTR;

	/*			When are we scared ?			*/
	double		battery_alert_frac;

	/*			Our battery				*/
	void		*BATT;


	/*				Logging				*/
	int 		logfd;
	int		trace;


	/*			Per node stdout/stderr buffers		*/
        char            stdout_buf[MAX_NODESTDOUT_BUFSZ];
        int             stdout_h2o;
        char            stderr_buf[MAX_NODESTDERR_BUFSZ];
        int             stderr_h2o;
        char            nodeinfo_buf[MAX_NODEINFO_BUFSZ];
        int             nodeinfo_h2o;



	int		runnable;
	int		pipelined;
	int		pipeshow;
	int		sleep;

	/*			Timing stuff				*/
	ulong		ustart, ufinish;
	uvlong		startclk, finishclk;
	uvlong		trip_ustart, trip_startclk;


	/*	    		Topology Information			*/
	int		orbit;
	double		velocity;
	double		xloc;
	double		yloc;
	double		zloc;


	/*	Set if acting as a dummy from remote_seg_enqueue()	*/
	int		from_remote;


	/*	   		Buffer for rabbit hole			*/
	char		cmdbuf[MAX_CMD_LEN];
	int		cmdbuf_nbytes;


	/*	Pointer to function to relevant step() routine		*/
	int 		(*step)(State *, int);
	int 		(*cyclestep)(State *, int);
	int 		(*faststep)(State *, int);

	/*	Pointer to function for failure prob dist		*/
	uvlong 		(*pfun)(void *, char *, uvlong);

	/*	The routines to handle the different interrupts		*/
	int 		(*take_timer_intr)(State *S);
	int 		(*take_nic_intr)(State *S);
	int 		(*take_batt_intr)(State *S);
	int 		(*check_nic_intr)(State *S);
	int 		(*check_batt_intr)(State *S);

	/*		Other misc machine specific actions		*/
	void 		(*fatalaction)(State *S);
	void 		(*dumpregs)(State *S);
	void 		(*dumpsysregs)(State *S);
	void 		(*resetcpu)(State *S);
	void 		(*dumppipe)(State *S);
	void 		(*pipeflush)(State *S);

	/*	Memory mapped device register read/write functions	*/
	uchar		(*devreadbyte)(State *S, ulong addr);
	ushort		(*devreadword)(State *S, ulong addr);
	ulong		(*devreadlong)(State *S, ulong addr);
	void		(*devwritebyte)(State *S, ulong addr, uchar data);
	void		(*devwriteword)(State *S, ulong addr, ushort data);
	void		(*devwritelong)(State *S, ulong addr, ulong data);

	/*		Split CPU into 2 shared memory CPUs		*/
	void		(*split)(State *S, ulong startpc, ulong stackaddr, ulong argaddr, char *idstr);

	/*		Address translation / MMU routines		*/
	void		(*vmtranslate)(State *S, int op, TransAddr *transladdr);

	/*	This is the function called on a cache miss		*/
	void 		(*stallaction)(State *S, ulong addr, int type, int latency);

	void 		(*settimerintrdelay)(State *S, int delay);

	int		(*cache_init)(State *S, int size, int blocksize, int assoc);
	void 		(*cache_deactivate)(State *S);
	void 		(*cache_printstats)(State *S);
	void 		(*dumptlb)(State *S);

	void 		(*writebyte)(State *S, ulong addr, ulong data);
};	
