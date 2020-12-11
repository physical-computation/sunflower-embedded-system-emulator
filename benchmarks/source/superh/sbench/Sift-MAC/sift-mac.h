typedef enum
{
	STATE_IDLE,
	STATE_CONTEND,
	STATE_ACKWAIT,
	STATE_RECEIVE,
} State;


typedef enum
{
	/*	Frame Types	*/
	FRAME_DATA,
	FRAME_ACK,
}FrameType;


typedef struct
{
	uchar		src[2];
	uchar		dst[2];
	uchar		proto;
	int		duration;
	int		len;
	uchar		nxthdr;
} SiftHeader;


typedef struct SiftFrame SiftFrame;
struct SiftFrame
{
	uchar		src[2];
	uchar		dst[2];
	int		proto;
	int		len;
	int		nxthdr;
	uchar		*data;

	SiftFrame	*prev;
	SiftFrame	*next;
};


typedef struct
{
	char		addr[2];
	int		which_ifc;


	/*			S-MAC per-instance params.		*/
	float		alpha;
	int		CW;


	/*	Additional delay b/4 backoff (Sift paper, Figure 2)	*/
	int		tdifs;

	/*	Delay b/4 ACK (Sift paper, pseudocode, Section 2.3)	*/
	int		tsifs;
	int		tACKTimeout;
	int		tslot;


	/*	Not defined in Sift; the maximum chunk size for data	*/
	int		chunk_size;


	/*	Not defined in Sift: maximum wait time for an ACK	*/
	int		max_ackwaitperiods;


	/*		Receive queue		*/
	SiftFrame	*dataq_hd;


	ulong		lock;
} SiftState;


extern uchar	SIFT_BCAST_ADDR[2];


enum
{
	SIFT_ADDR_LEN			= 2,
	SIFT_CKSUM_LEN			= 2,
	SIFT_HDR_LEN			= 10,

	/*							*/
	/*	Contention window size is fixed at 32 slots	*/
	/*	(Sift paper, Section 2, paragraph 3)		*/
	/*							*/
	SIFT_FIXED_CW			= 32,

	/*							*/
	/*	Suggested slot duration in microseconds		*/
	/*	(Sift paper, Section 2, paragraph 3)		*/
	/*							*/
	SIFT_FIXED_SLOTUSECS		= 10,
};

typedef enum
{
	SIFT_RAND_ACCEPTREJECT,
	SIFT_RAND_TABLE,
} RandGenType;

int		sift_xmit(SiftState *S, uchar *dst, uchar *data, int len, int nxthdr);
SiftState *	sift_init(uchar *addr, int whichifc, float alpha, int CW,
				int tdifs, int tsifs, int tACKTimeout, int tslot,
				int chunk_size, int max_ackwaitpreriods);
int		sift_vcsense(SiftState *S);
SiftFrame *	sift_receive(SiftState *S, int proto);
void		sift_nichdlr(SiftState *S, int whichifc);
void		sift_freeframe(SiftFrame *chunk);
