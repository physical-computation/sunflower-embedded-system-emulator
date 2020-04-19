typedef enum
{
//BUG/TODO: rename with a SMAC_ prefix
	/*	Frame Types	*/
	T_SYNC,
	T_RTS,
	T_CTS,
	T_DATA,
	T_ACK,
	T_SLP,
}FrameType;


typedef enum
{
	/*	Node Flags	*/
	F_SYNCER = 1 << 0,
	F_FLLWER = 1 << 1,
} Flag;


typedef struct SchedEntry SchedEntry;
struct SchedEntry
{
	int		creation_time;
	int		framestart_usecs;
	int		sync_lastsent;
	uchar		originator[2];

	SchedEntry	*prev;
	SchedEntry	*next;
};


typedef struct
{
	uchar		src[2];
	uchar		dst[2];
	uchar		proto;
	int		duration;
	int		len;
	uchar		nxthdr;
} SMACheader;


typedef struct SMACframe SMACframe;
struct SMACframe
{
	uchar		src[2];
	uchar		dst[2];
	int		proto;
	int		len;
	int		nxthdr;
	uchar		seqno;
	uchar		*data;

	SMACframe	*prev;
	SMACframe	*next;
};


typedef struct
{
	char		addr[2];
	int		which_ifc;
	char		flag;


	/*	S-MAC per-instance params.	*/
	int		listen_usecs;
	int		sleep_usecs;
	int		sync_usecs;
	int		extension_usecs;
	int		max_initsleep_slots;
	int		max_syncsleep_slots;

	int		slot_usecs;
	int		sync_slots;
	int		data_slots;


	/*	    Cached quantities		*/
	int		byte_delay_usecs;
	int		header_delay_usecs;


	/*					*/
	/*	Set to the usecs since epoch	*/
	/*	each time a frame begins, to	*/
	/*	enable us to determine our	*/
	/*	offset into the frame.		*/
	/*					*/
	int		framestart_usec;
	int		sync_lastsent;
	int		vcsense_usecs;
	int		vcsense_lastupdate;
	int		rtsclear_time;
	SchedEntry	*sched_hd;


	/*		Receive queue		*/
	SMACframe	*dataq_hd;
	
	/*	Various Counters		*/
	int		rcvd_scheds;
	uchar		seqno;

	ulong		lock;
} SMACstate;

extern uchar	SMAC_BCAST_ADDR[2];

enum
{
	SMAC_ADDR_LEN			= 2,
	SMAC_CHUNK_SIZE			= 72,
	SMAC_SYNC_PAYLOAD_NBYTES	= 6,
};



int		smac_transmit(SMACstate *SM, uchar *dst, uchar *data, int len, int max_retries, int nxthdr);
SMACstate *	smac_init(uchar *addr, int whichifc, int listen_usecs, int sleep_usecs, int sync_usecs,
			int extension_usecs, int max_initsleep, int max_syncsleep,
			int slot_usecs, int sync_slots, int data_slots);
int		smac_virtcsense(SMACstate *SM);
SMACframe *	smac_receive(SMACstate *SM, int proto);
void		smac_timerhdlr(SMACstate *SM);
void		smac_nichdlr(SMACstate *SM, int whichifc);
void		smac_reset(SMACstate *);
void		smac_freeframe(SMACframe *chunk);
