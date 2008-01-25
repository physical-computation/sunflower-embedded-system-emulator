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
enum
{
	MAX_NETSEGMENTS		= 64,
	MAX_SEG2FILES		= MAX_NETSEGMENTS,
	MAX_SEGNODES		= 256,
	MAX_FRAMEBYTES		= 200000,
	MAX_SEGBUF_TEXT 	= 3*MAX_FRAMEBYTES + 1024,
	MAX_NETIO_NBUFS		= 32,
};

enum
{
	NIC_MAX_IFCS		= 16,
	NIC_ADDR_LEN		= 16+1,	/*  +1 for NULL byte; we'll often sprint in	*/
	NIC_HDRLEN		= 37,
	NIC_NCR_READ		= 0,
	NIC_NCR_WRITE		= 1,

	NIC_CMD_POWERUP		= 0,
	NIC_CMD_POWERDN		= 1,
	NIC_CMD_TRANSMIT	= 2,

	NIC_STATE_IDLE		= 1<<0,
	NIC_STATE_LISTEN	= 1<<1,
	NIC_STATE_TX		= 1<<2,
	NIC_STATE_RX		= 1<<3,
};

enum
{
	NETOK			= 0,
	NETFRAMERR,
	NETCOLLERR,
	NETADDRERR,
	NETCARRIERSENSERR,
};

typedef enum
{
	TX_FIFO,
	RX_FIFO,
} Fifo;

typedef struct
{
	int	valid;

	/*						*/
	/*	Transmit power consumption (Watts)	*/
	/*						*/
	double		tx_pwr;

	/*						*/
	/*	Receive power consumption (Watts)	*/
	/*						*/
	double		rx_pwr;

	/*						*/
	/*	Idle power consumption (Watts)		*/
	/*						*/
	double		idle_pwr;

	/*						*/
	/*	Listen power consumption (Watts)	*/
	/*						*/
	double		listen_pwr;

	double		fail_prob;
	Pdist		failure_dist;
	Pdist		failure_duration_dist;
	uvlong		failure_duration_max;
	uvlong		fail_clocks_left;

	int		frame_bits;
	int		segno;


	/*	Each TX/RX FIFO entry is an array of bytes	*/
	uchar		**rx_fifo;
	uchar		**tx_fifo;

	/*							*/
	/*	The FIFO entries are each sized for maximum	*/
	/*	frame size.  We maintain the actual frame	*/
	/*	sizes separately.				*/
	/*							*/
	int		*rx_fifo_framesizes;
	int		*tx_fifo_framesizes;

	
	/*	For receive only, a further level of buffering	*/
	uchar		*rx_localbuf;
	int		rx_localbuf_h2o;
	int		rx_localbuf_framesize;


	/*	Interface can hold multiple incoming frames	*/
	uint		tx_fifo_size;
	uint		rx_fifo_size;


	/*			Max occupancy of FIFOs		*/
	uint		tx_fifo_maxoccupancy;
	uint		rx_fifo_maxoccupancy;


	/*	The FIFO entry that is currently being filled	*/
	uint		tx_fifo_curidx;
	uint		rx_fifo_curidx;

	/*	Oldest entry in the circular FIFO		*/
	uint		tx_fifo_oldestidx;
	uint		rx_fifo_oldestidx;


	/*	# bytes in FIFO entry currently being filled	*/
	/*	Seg->bits_left is used in lieu of rx_fifo_h20	*/
	uint		tx_fifo_h2o;


	/*	Number of retries since last sucessful xmit	*/
	uint		tx_alg_retries;
	double		fifo_nextretry_time;

	/*	    	  Memory Mapped registers		*/
	ulong		IFC_FSZ;
	ulong		IFC_RDR;
	ulong		IFC_NSR;
	ulong		IFC_TDR;
	ulong		IFC_NCR;
	ulong		IFC_BRR;
	ulong		IFC_NMR;
	uchar		IFC_OUI[NIC_ADDR_LEN+1];
	uchar		IFC_DST[NIC_ADDR_LEN+1];
		/*	Counter regs		*/
	ulong		IFC_CNTR_TXOK;
	ulong		IFC_CNTR_RXOK;
	ulong		IFC_CNTR_ADDR_ERR;
	ulong		IFC_CNTR_FRAME_ERR;
	ulong		IFC_CNTR_COLLS_ERR;
	ulong		IFC_CNTR_CSENSE_ERR;
	ulong		IFC_CNTR_RXOVRRUN_ERR;
	ulong		IFC_CNTR_RXUNDRRUN_ERR;
	ulong		IFC_CNTR_TXOVRRUN_ERR;
	ulong		IFC_CNTR_TXUNDRRUN_ERR;
	ulong		IFC_CNTR_CSUM_ERR;

		/*	TX/RX FIFO fill		*/
	ulong		IFC_TXFIFO_LEVEL;
	ulong		IFC_RXFIFO_LEVEL;

		/*	Track idle/listen/rx/tx	*/
	ulong		IFC_STATE;

	/*	Pointer to function for failure prob dist	*/
	uvlong 		(*pfun)(void *, void *, char *, uvlong);

	/*	Pointer to function for retry algorithm.	*/
	void		(*tx_fifo_retry_fxn)(void *, void *, int);
} Ifc;


typedef struct Segbuf Segbuf;
struct Segbuf
{
	/*	Timestamp in terms of global time	*/
	double	timestamp;

	/*						*/
	/*	Pointer to data from NIC_TX_BUF	plus	*/
	/*	the checksum. Others depend on size	*/
	/*	of MAXFRAMEBYTES to not inlcude cksum.	*/
	/*						*/
	uchar	data[MAX_FRAMEBYTES + 4];
	int	actual_nbytes;

	/*	How many bits remain to be emptied	*/
	int	bits_left;

	/*	From whence it cometh ?			*/
	void	*src_node;

	/*	Tither it goeth.			*/
	void	*dst_node;

	/*	Ist thou a broadcast packeth ?		*/
	int	bcast;

	/*	What was the source's IFC @ TX time	*/
	int	src_ifc;

	/*      On what network segment are we linked   */
	int     parent_netsegid;

	/*						*/
	/*	Whether this frame was forwarded from	*/
	/*	a remote simulation host.		*/
	/*						*/
	int	from_remote;
};


typedef struct
{
	int		valid;
	int		NETSEG_ID;

	/*							*/
	/*	This is the size of the quanta which we put	*/
	/*	on segment. For example, it is 1514*8 bits 	*/
	/*	for Ethernet, but 1 bit for CAN. NOTE that	*/
	/*	the framesize is in bits, not bytes!		*/
	/*							*/
	int		frame_bits;

	/*							*/
	/*	Media propagation speed in (m/s)		*/
	/*							*/
	ulong		propagation_speed;

	/*							*/
	/*	Physical Layer protocol bitrate	(bits/s)	*/
	/*							*/
	ulong		bitrate;

	/*							*/
	/*	Max width, > 0 => CDMA-like, 0 => CSMA-like	*/
	/*							*/
	int		queue_max_width;
	int		cur_queue_width;



	/*							*/
	double		lastactivate;
	double		bytedelay;


	/*							*/
	/*			Failure Modeling		*/
	/*							*/
	double		fail_prob;
	Pdist		failure_dist;
	Pdist		failure_duration_dist;
	uvlong		failure_duration_max;
	uvlong		fail_clocks_left;

	double		correl_coeffs[MAX_SEGNODES];

	int		busy;

	//TODO: should just hold pointers to nodes directly, instead of indeces
	int		node_ids[MAX_SEGNODES];
	int		num_attached;

	char		*seg2filenames[MAX_SEG2FILES];
	int		num_seg2files;

	Segbuf		*segbufs;


	/*	Association with a signal source/propagation model	*/
	Signalsrc	*sigsrc;
	double		minsnr;


	/*	Pointer to function for failure prob dist	*/
	uvlong 		(*pfun)(void *, void *, char *, uvlong);
} Netsegment;


/*	MAC layer trace frame from parsed libpcap format dump (from Ethereal)	*/
typedef struct
{
	ulong	timedelta;
	ulong	timezerooffset;

	int	len;

	char	src[NIC_ADDR_LEN];
	char	dst[NIC_ADDR_LEN];

	char	*data;
} TraceMACframe;
