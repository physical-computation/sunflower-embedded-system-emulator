#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sf-types.h"
#include "errors.h"
#include "sf-includes.h"
#include "sift-mac.h"


#ifdef SIFT_RANDTABLE
#	include "randtable.h"
#else
	int	*sift_randtable;
	enum {SIFT_RANDTABLE_LEN = 0};
#endif


uchar			SIFT_BCAST_ADDR[2] = {~0, ~0};

static void		send(SiftState *S, uchar *data, uchar *dst, int proto, int len, int nxthdr);
static int		cntq(SiftState *, int);
static SiftFrame*	deqframe(SiftState *, int);
static void		enqframe(SiftState *S, uchar *src, uchar *dst, int len,
				int proto, int nxthdr, uchar *data);
static int		netint2int(uchar *);
static int		netbyte2int(uchar *);
static ulong		uniform(ulong min, ulong max);
static void		udelay(ulong);

static void		lock(ulong *l);
static void		unlock(ulong *l);

extern void		fatal(char *);
extern void		splhi(void);
extern void		spldone(ulong);
extern ulong		getpsr(void);


//BUG: 	a problem here (and also in our S-MAC implementation), is that
//	we do not lock the data structures, even though they are accessed
//	asynchronously by intr hdlr, while main code might also be accessing structures


/*									*/
/*	If we used spin locks for the locking routines, we could	*/
/*	easily deadlock, e.g., when intr hdlr prempts app, but ends	*/
/*	up waiting for lock held by app (which won't get run till	*/
/*	hdlr is done.  This is ugly, but is the way things are since	*/
/*	we don't have a real OS.					*/
/*									*/
static void
lock(ulong *l)
{
	*l = getpsr();
	splhi();
}

static void
unlock(ulong *l)
{
	spldone(*l);
}

static void
enqframe(SiftState *S, uchar *src, uchar *dst, int len, int proto, int nxthdr, uchar *data)
{
	SiftFrame	*chunk;


	chunk = (SiftFrame *)malloc(sizeof(SiftFrame));
	if (chunk == NULL)
	{
		fatal(Emalloc);
	}
	chunk->data = NULL;

	if (len > 0)
	{
		chunk->data = (uchar *)malloc(len);
		if (chunk->data == NULL)
		{
			fatal(Emalloc);
		}

		memmove(chunk->data, data, len);
	}
	
	memmove(chunk->src, src, 2);
	memmove(chunk->dst, dst, 2);
	chunk->next = chunk->prev = NULL;
	chunk->proto = proto;
	chunk->len = len;
	chunk->nxthdr = nxthdr;
	
	lock(&S->lock);
	if (S->dataq_hd == NULL)
	{
		S->dataq_hd = chunk;
		unlock(&S->lock);

		return;
	}
	
	S->dataq_hd->prev = chunk;
	chunk->next = S->dataq_hd;
	S->dataq_hd = chunk;
	unlock(&S->lock);


	return;
}

static SiftFrame *
deqframe(SiftState *S, int proto)
{
	SiftFrame	*p;

	lock(&S->lock);
	p = S->dataq_hd;
	while (p != NULL)
	{
		if (p->proto == proto || proto == -1)
		{
			if (p == S->dataq_hd)
			{
				S->dataq_hd = p->next;
				unlock(&S->lock);

				return p;
			}

			p->prev->next = p->next;
			unlock(&S->lock);

			return p;
		}

		p = p->next;
	}
	unlock(&S->lock);


	return NULL;
}

static int
cntq(SiftState *S, int chkproto)
{
	SiftFrame	*p;
	int		n = 0;


	/*						*/
	/*	If chkproto is -1, match all types.	*/
	/*						*/
	lock(&S->lock);
	p = S->dataq_hd;
	while (p != NULL)
	{
		if (p->proto == chkproto || chkproto == -1)
		{
			n++;
		}
		p = p->next;
	}
	unlock(&S->lock);


	return n;
}

static int
netbyte2int(uchar *data)
{
	return	(int) data[0];
}

static int
netint2int(uchar *data)
{
	return (int)((data[0] << 0) |
		(data[1] << 8) |
		(data[2] << 16) |
		(data[3] << 24));
}

static ulong
uniform(ulong min, ulong max)
{
	return min + (devrand_getrand() % ((max-min)+1));
}


#ifdef SIFT_RANDTABLE
static int
pickslot(SiftState *S)
{
	/*									*/
	/*	As an alternative to accept/reject method, we can also		*/
	/*	populate a table with distribution based on selected alpha	*/
	/*									*/
	return sift_randtable[uniform(0, SIFT_RANDTABLE_LEN - 1)];
}
#else
static int
pickslot(SiftState *S)
{
	float	test, pr, qr;
	int	r, c;


	/*								*/
	/*	Here, we need to generate a random variable from the	*/
	/*	Sift-specified truncated geometric distribution.	*/
	/*								*/
	/*	This is an example of a situation where an algorithm	*/
	/*	has a component with a non-trivial computational cost	*/
	/*	which might not be taken into account when performing	*/
	/*	only network-level simulation.				*/
	/*								*/


	/*								*/
	/*	Truncated geometric distribution.			*/
	/*								*/
	/*	pr = ((1-alpha)*alpha^CW)*(alpha^(-r))/(1-alpha^CW) 	*/
	/*								*/

	/*					*/
	/*	Let fy(t) = 1, c = 1		*/
	/*	fx(t)/fy(t) <= c  for all t	*/
	/*					*/
	do
	{
		r = (double)uniform(0, S->CW - 1);
		pr = (1 - S->alpha)*pow(S->alpha, S->CW)*pow(S->alpha, 0 - r)
			/ (1 - pow(S->alpha, S->CW));
		qr = 1;
		c = 1;
		test = pr/(c*qr);
//@printf("\tpotential r = %d, S->CW = %d, S->alpha = %f, pr = %f\n", r, S->CW, S->alpha, pr);
	} while (uniform(0, (ulong)ceil(1.0/test)) != 1);

//@print("a/r, r = %d\n", r);

	return r;
}
#endif

static void
udelay(ulong usecs)
{
        volatile int    max, i;

        max = usecs/2;
        for (i = 0; i < max; i++)
        {
        }

	return;
}


static void
send(SiftState *S, uchar *data, uchar *dst, int proto, int len, int nxthdr)
{
	/*	OK to be on stack, since it is "copied" into network txbuf	*/
	uchar	header[SIFT_HDR_LEN];


	header[0] = S->addr[0];
	header[1] = S->addr[1];
	header[2] = dst[0];
	header[3] = dst[1]; 
	header[4] = proto;
	header[5] = (len >> 0) & 0xFF;
	header[6] = (len >> 8) & 0xFF;
	header[7] = (len >> 16) & 0xFF;
	header[8] = (len >> 24) & 0xFF;
	header[9] = nxthdr;

	/*	devmac_xmit takes both the MAC layer header and the payload	*/
	devmac_xmit(dst, SIFT_ADDR_LEN, &header[0], data, SIFT_HDR_LEN, len, S->which_ifc);


	return;
}

int
sift_xmit(SiftState *S, uchar *dst, uchar *data, int len, int nxthdr)
{
	int	qcnt, slot, bytes_left, waitperiods;


	//BUG: even though we do segmentation, sift_receive currently does not do reassembly.
	//	even though the S-MAC paper implies SAR, it is not fleshed out there, so
	//	we can choose our own impl.  To do SAR, we'd need a "fragment" bit in frame header,
	//	as well as a sequence ID.

	/*								*/
	/*	We segment large frames.  Recepient does reassembly	*/
	/*								*/
	bytes_left = len;
	while (bytes_left > 0)
	{
		slot = pickslot(S);

		/*							*/
		/*	While the Sift paper implies tdifs delay is	*/
		/*	fixed, it makes more sense to randomize the	*/
		/*	tdifs, to increase chance of not starting	*/
		/*	at same time as someone who chose same slot,	*/
		/*	increasing the chance of detecting carrier 	*/
		/*	(for non-hidden terminals), hence preventing	*/
		/*	a collision.					*/
		/*							*/
		udelay(uniform(0, S->tdifs) + slot*S->tslot);
		if (devmac_csense(S->which_ifc))
		{
			return -1;
		}

		send(S, &data[len - bytes_left], dst, FRAME_DATA,
			min(len, S->chunk_size), nxthdr);

		/*							*/
		/*	Sift does not retransmit a frame with a tardy	*/
		/*	ACK --- it just waits longer.  Though the 	*/
		/*	paper does not specify a max wait for ACK,	*/
		/*	it makes sense to limit our longing for ACKs	*/
		/*							*/
		waitperiods = 0;
		while (waitperiods < S->max_ackwaitperiods)
		{
			qcnt = cntq(S, FRAME_ACK);
			udelay(S->tACKTimeout);
			if ((cntq(S, FRAME_ACK) - qcnt) > 0)
			{
				while (cntq(S, FRAME_ACK))
				{
					sift_freeframe(deqframe(S, FRAME_ACK));
				}
				bytes_left -= min(len, S->chunk_size);

				break;
			}

			waitperiods++;
		}

		if (waitperiods == S->max_ackwaitperiods)
		{
			return -1;
		}
	}

	return 0;
}


void
sift_freeframe(SiftFrame *chunk)
{
int savedSR = getpsr();
splhi();
	if (chunk->data != NULL)
	{
		free(chunk->data);
	}
	free(chunk);
spldone(savedSR);

	return;
}


SiftFrame *
sift_receive(SiftState *S, int proto)
{
	return deqframe(S, proto);
}


void
sift_nichdlr(SiftState *S, int whichifc)
{
	/*									*/
	/*	Sift MAC layer frame format 9not specified in paper):		*/
	/*									*/
	/*	0	2	4	5      9         SIFT_HDR_LEN		*/
	/*	|  src	|  dst	| proto |  len  | nxthdr | data...		*/
	/*									*/
	/*	Fields src and dst are self explanatory.  Field proto is	*/
	/*	one of the SIFT frame types, and nxthdr is like the selfsame	*/
	/*	field in IP, i.e., signifies what the type of the payload is.	*/
	/*									*/
	uchar		*src, *dst, *data, *payload, *cksum, frame_cksum[2];
	int		proto, nxthdr, len, cur_usecs, rx_frame_size;


	rx_frame_size = devmac_getrxfsz(whichifc);
	payload = (uchar *)malloc(rx_frame_size);
	if (payload == NULL)
	{
		fatal(Emalloc);
	}

        devmac_recv(payload, rx_frame_size, whichifc);

	src 		= payload;
	dst 		= &payload[2];
	proto		= netbyte2int(&payload[4]);
	len		= netint2int(&payload[5]);
	nxthdr		= netbyte2int(&payload[9]);
	data		= &payload[SIFT_HDR_LEN];
	cksum		= &payload[rx_frame_size - SIFT_CKSUM_LEN];

//@printf("sift-mac::: got frame from [%d][%d], proto [%d], len [%d]\n", src[0], src[1], proto, len);

	devmac_cksum(&payload[0], &payload[SIFT_HDR_LEN], SIFT_HDR_LEN,
			rx_frame_size - SIFT_HDR_LEN - SIFT_CKSUM_LEN, &frame_cksum[0]);
	if ((cksum[0] != frame_cksum[0]) || (cksum[1] != frame_cksum[1]))
	{
		free(payload);
//@print("bad cksum...\n");

		return;
	}
//@print("OK cksum...\n");

	/*	Other Sanity checks on recieved frame	*/
	if (proto == FRAME_DATA && len == 0)
	{
		free(payload);

//@print("runt data frame...\n");
		return;
	}

	cur_usecs = devrtc_getusecs();
	switch(proto)
	{
		case FRAME_DATA:
		{
			/*							*/
			/*	Though not specified in Sift paper (?), makes	*/
			/*	sense to randomize the tsifs delay		*/
			/*							*/
			udelay(uniform(0, S->tsifs));

			/*							*/
			/*	Not explicit in Sift paper, but'd be silly	*/
			/*	not to sense for carrier.  Sift paper towards	*/
			/*	the end implies they also do vcsense.  If	*/
			/*	we implement that, we can get rid of this	*/
			/*	ugly busy poll					*/
			/*							*/
			while (devmac_csense(S->which_ifc))
			{
			}

			send(S, NULL, src, FRAME_ACK, 0, nxthdr);

			break;
		}

		case FRAME_ACK:
		{
			/*	Gets enqueued in rxqueue	*/
			break;
		}
	
		default: fatal(Ebadproto);
	}
	enqframe(S, src, dst, len, proto, nxthdr, data);
	free(payload);


	return;
}


SiftState *
sift_init(uchar *addr, int whichifc, float alpha, int CW,
		int tdifs, int tsifs, int tACKTimeout, int tslot,
		int chunk_size, int max_ackwaitperiods)
{
	SiftState	*S;

	S = malloc(sizeof(SiftState));
	if (S == NULL)
	{
		fatal(Emalloc);
	}

	S->addr[0]	= addr[0];
	S->addr[1]	= addr[1];
	S->which_ifc	= whichifc;
	S->alpha	= alpha;
	S->CW		= CW;
	S->tdifs	= tdifs;
	S->tsifs	= tsifs;
	S->tACKTimeout	= tACKTimeout;
	S->tslot	= tslot;
	S->chunk_size	= chunk_size;
	S->max_ackwaitperiods = max_ackwaitperiods;
	S->dataq_hd	= NULL;

	devmac_ctl(NIC_NCR_WRITE, NIC_CMD_POWERUP, S->which_ifc);


	return S;
}
