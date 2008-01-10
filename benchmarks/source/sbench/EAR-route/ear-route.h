enum
{
	PROTO_EAR		= 2,

	/*			Protocol phases				*/
	EAR_PHASE_SETUP		= 0,
	EAR_PHASE_COMMS		= 1,

	EAR_ADDR_LEN		= 16,

	/*		Modify to fit needs of situation		*/
	EAR_MAX_NEIGHBORS	= 16,
	EAR_MAX_FTABLE		= 16,
	EAR_RANDTABLE_SIZE	= 1024,
};

typedef struct
{
	uchar		x;
	uchar		y;
	uchar		z;
} EARLocation;


typedef struct
{
	uchar		proto;
	uchar		phase;
	uchar		hopcount;
	uint		len;
	uint		nxthdr;
	uint		cost;

	uchar		final_dst[EAR_ADDR_LEN+1];
	uchar		orig_src[EAR_ADDR_LEN+1];

	uchar		src[EAR_ADDR_LEN+1];
	uchar		src_loc_x;
	uchar		src_loc_y;
	uchar		src_loc_z;
} EARHeader;


typedef struct
{
	uchar		addr[EAR_ADDR_LEN+1];
	EARLocation	location;
} EARNeighbor;


typedef struct
{
	uchar		addr[EAR_ADDR_LEN+1];
	uint		cost;

	uint		prob_num;
	uint		prob_denom;
} EARFTableEntry;

typedef struct
{
	uchar		addr[EAR_ADDR_LEN+1];
	EARLocation	location;
	int		which_ifc;

	/*	EAR per-instance params.	*/
	int		ftable_alpha;
	int		alpha;
	int		beta;
	int		eij;
	int		maxhops;

	EARNeighbor	ntable[EAR_MAX_NEIGHBORS];
	uint		numntable;
	EARFTableEntry	ftable[EAR_MAX_FTABLE];
	uint		numftable;
	uchar		randtable[EAR_RANDTABLE_SIZE];

	uchar		lock;
} EARState;

typedef struct
{
	uchar		src[EAR_ADDR_LEN+1];
	uchar		*payload;
	uint		nxthdr;
} EARSrcPayloadTuple;


EARState *		ear_init(uchar *addr, int which_ifc, int alpha, int beta, int ftable_alpha,
				int eij, int maxhops, int x, int y, int z);
int			ear_response(EARState *S, uchar *dst, uchar *data, int len, int nxthdr);
int			ear_request(EARState *S, uchar *src, uchar *data, int len, int nxthdr);
EARSrcPayloadTuple *	ear_recv(EARState *S, uchar *payload, int len);
