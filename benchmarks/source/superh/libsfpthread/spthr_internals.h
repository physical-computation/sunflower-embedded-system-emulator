#ifdef SUPERH
enum
{
	NSAVEREGS	= 9,
	BYTES_PER_REG	= 4,
	NAMELEN		= 128,
	STKSIZE		= 8192,
	MAX_NTHREADS	= 1024,

	/*						*/
	/*	Fixed size;  If more than this		*/
	/*	number is detected when we probe	*/
	/*	hardware for count, kill thread.	*/
	/*						*/
	MAX_MAPCOUNT	= 8192,
};
#endif

typedef enum
{
	SPTHR_LIB_STATIC,

	SPTHR_LIB_DYNAMIC_1,
	SPTHR_LIB_DYNAMIC_2,
	SPTHR_LIB_DYNAMIC_3,
} LibMode;

enum
{
	SPTHR_MAP_PRIVATE,
	SPTHR_MAP_PUBLIC,
};

enum
{
	STATUS_ACTIVE,
	STATUS_MORIBUND,
};

enum
{
	SPTHR_MAP_LOCAL		= -1,
	SPTHR_MAP_INVALID	= -2,
	SPTHR_MAP_END		= -3,
};

enum
{
	SPTHR_EINVAL		= -1,
	SPTHR_EPERM		= -2,
};

enum
{
	SPTHR_MAPUPDATE_USECS	= 10000000,
};

typedef struct
{
	int		mapsize;

	ulong		nreads;
	ulong		nwrites;

	ulong		prednreads;
	ulong		prednwrites;

	ulong		maxnreads;
	ulong		maxnwrites;
	ulong		minnreads;
	ulong		minnwrites;
} MapStat;

typedef struct ThreadState ThreadState;
struct ThreadState
{
	uchar		registers[NSAVEREGS * BYTES_PER_REG];
	char		name[NAMELEN];
	int		remote;
	int		id;
	int		status;
	int		stksize;
	Maddr		stkaddr;
	Maddr		fnaddr;
	Maddr		fnargaddr;
	Maddr		privaddr;
	MapStat		mapstats[MAX_MAPCOUNT];
	ulong		mapstats_timestamp;

	ThreadState	*next;

	/*							*/
	/*	Reentrancy Struct. Needed for the reentrancy 	*/
	/*	of some C Library functions (e.g. strtok_r).	*/
	/*	See  http://www.objsw.com/docs/libc_127.html	*/
	/*							*/
	/*	Dealing with the global struct _reent (which	*/
	/*	happens to be called _impure_ptr) is tricky	*/
	/*	when our threads execute concurrently:  we	*/
	/*	can't swap what _impure_ptr points to as we	*/
	/*	switch b/n threads. Even though all threads	*/
	/*	share same address space, the _impure_ptr	*/
	/*	referred to by each thread should be unique.	*/
	/*	Ways to do this: (1) Let _impure_ptr be on	*/
	/*	a page of its own and keep a unique mapping	*/
	/*	of phy-virt for each thread. (2) Numaregion	*/
	/*							*/
	struct _reent	rs;
};

typedef struct RegionMap RegionMap;
struct RegionMap
{
	/*							*/
	/*	Actual access counts are maintained by HW.	*/
	/*	We only maintain these structures to know	*/
	/*	what IDs to ask HW for.				*/
	/*							*/
	ulong		addr;
	ulong		size;
	int		hostid;
	int		id;

	RegionMap	*next;
};

typedef volatile struct Sleepq Sleepq;
struct Sleepq
{
	int		rdy;
	Sleepq		* volatile next;

	/*	Debug	*/
	int		threadid;
};

typedef struct
{
	char	*name;
	int	homenode;
} StaticMap;

#define	min(a, b)	((a) < (b) ? (a) : (b))
#define	max(a, b)	((a) > (b) ? (a) : (b))
