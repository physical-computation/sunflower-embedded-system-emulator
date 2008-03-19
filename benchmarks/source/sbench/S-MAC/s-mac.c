#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include "sf-types.h"
#include "errors.h"
#include "sf-includes.h"
#include "s-mac.h"

/*										*/
/*			A (portable?) S-MAC implementation			*/
/*										*/
/*	Implementation performed initially for the Sunflower simulator		*/
/*	(http://www.sunflowersim.org), and described in the following		*/
/*	papers:									*/
/*		Full-system simulation for sensor networks			*/
/*		(proc SenSys '05, pg. 317)					*/
/*										*/
/*		Sunflower --- Full-system Embedded Microarchitecture		*/
/*		evaluation.  Proc HiPEAC 2007					*/
/*										*/
/*	Spring 2007, phillip stanley-marbell <p.stanley-marbell@tue.nl>		*/
/*										*/
/*	Requires the following routines for accessing an underlying radio,	*/
/*	and other hardware resources:						*/
/*		devmac_xmit():		send (modulate and transmit) a set of	*/
/*					bytes over the radio interface		*/
/*										*/
/*		devmac_ctl():		control the state of the radio		*/
/*					interface (listen versus shutdown)	*/
/*										*/
/*		devmac_csense():	Carrier detection / RSSI		*/
/*										*/
/*		devmac_getspeed():	Obtain network interface speed		*/
/*										*/
/*		devmac_getrxfsz():	Obtain size of received frame		*/
/*										*/
/*		devmac_recv():		Retrieve a received sequence of		*/
/*					bytes from radio interface		*/
/*										*/
/*		devmac_cksum():		Compute CRC				*/
/*										*/
/*	and									*/
/*		xusleep():		Sleep for a number of usecs		*/
/*										*/
/*		devrand_getrand():	Obtain value from an entropy source	*/
/*										*/
/*		devrtc_getusecs():	Sample a free-running usec timer	*/


/*
	NOTE: S-MAC inherently depends on the ability to put the
	interface into promiscuous mode, because nodes snoop unicast
	RTS/CTS frames to do virtual csense,


	NOTE:	Data taken off the rxqueue is already allocated, so no
		need to re-allocate it, in, say, addsched.

	TODO: make sure that mac_powerdown() always has a matching
	mac_powerup() and vice versa, and that mac_powerup() is
	always accompanied by mac_promiscuous() or something similar


	TODO:
		We currently use routines like sleep() when we
		need to wait, e.g., until the beginning of a
		receipients time window.  This currently means
		that synchronous calls into smac_init(), smac_xmit(), 
		etc., may block for quite some time, possibly for the
		duration of a whole sleep frame.  This is inevitable
		if we are not truly multiplexing processes (?),
		but eventually, we'd like to have ways, e.g., to
		yield() for x seconds, etc.

	TODO:
		Check ordering of arguments on all calls to functions;
		we may have interchanged args of the same type in some
		calls.

	TODO:
		What should we do with received frames that we are not
		expecting ? We currently put them on the recieve queue,
		and we should purge them periodically ?

	TODO:
		I think S-MAC would benefit significnatly from a TTL
		field in  all frames, whther data or control.  Then,
		we can periodically scan rx queue and purge old entries

	TODO:
		Also, the addition of sequence numbers would make it
		possible to match unicast requests to their responses,
		e.g., an RTS to a CTS response, a DATA to an ACK response
		etc.
*/

uchar	SMAC_BCAST_ADDR[2] = {~0, ~0};

static void		vcsense_update(SMACstate *SM);
static int		addreq(uchar *, uchar *);
static void		addsched(SMACstate *, int, uchar *originator);
void			delsched(SMACstate *SM, uchar *src);
static int		cntq(SMACstate *, int, int);
static SMACframe*	deqframe(SMACstate *, int, int);
static void		enqframe(SMACstate *SM, uchar *src, uchar *dst, int len, int proto, int nxthdr, uchar seqno, uchar *data);
static int		netint2int(uchar *);
static int		netbyte2int(uchar *);
static uchar		send(SMACstate *, uchar *data, uchar *dst, int proto, int len, int stream_duration, int nxthdr, int seqno);
static uchar		send0(SMACstate *, uchar *data, uchar *dst, int proto, int len, int stream_duration, int nxthdr, uchar seqno);
static int		sendchunks(SMACstate *, uchar *dst, uchar *data, int len, int max_tries, int nxthdr);
int			schedexists(SMACstate *SM, uchar *originator);

extern void		fatal(char *);
extern void		xusleep(ulong);



/*										*/
/*	A calibrated "busy" usec delay loop;  for use where interrupts		*/
/*	are disabled (e.g., within a handler), and thus can't use xusleep()	*/
/*										*/
static void
udelay(ulong usecs)
{
        volatile int    max, i;

        max = usecs/2;
        for (i = 0; i < max; i++)
        {
        }
}

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
//	*l = getpsr();
//	splhi();
}

static void
unlock(ulong *l)
{
//	spldone(*l);
}


void
enqframe(SMACstate *SM, uchar *src, uchar *dst, int len, int proto, int nxthdr, uchar seqno, uchar *data)
{
	SMACframe	*chunk;


	chunk = (SMACframe *)malloc(sizeof(SMACframe));
	if (chunk == NULL)
	{
		fatal(Emalloc);
	}

	/*								*/
	/*	We NULL check this in some places, so need to make	*/
	/*	sure it really is, when not alloc (save calloc)		*/
	/*								*/
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
	chunk->seqno = seqno;
	
	lock(&SM->lock);
	if (SM->dataq_hd == NULL)
	{
		SM->dataq_hd = chunk;
		unlock(&SM->lock);

		return;
	}
	
	SM->dataq_hd->prev = chunk;
	chunk->next = SM->dataq_hd;
	SM->dataq_hd = chunk;
	unlock(&SM->lock);


	return;
}

SMACframe *
deqframe(SMACstate *SM, int proto, int seqno)
{
	SMACframe	*p;


	lock(&SM->lock);
	p = SM->dataq_hd;
	while (p != NULL)
	{
		if (	(p->proto == proto || proto == -1) &&
			(p->seqno == seqno || seqno == -1))
		{
			if (p == SM->dataq_hd)
			{
				SM->dataq_hd = p->next;
				unlock(&SM->lock);

				return p;
			}

			p->prev->next = p->next;
			unlock(&SM->lock);

			return p;
		}

		p = p->next;
	}
	unlock(&SM->lock);

	return NULL;
}

int
cntq(SMACstate *SM, int chkproto, int seqno)
{
	SMACframe	*p = SM->dataq_hd;
	int		n = 0;


	/*						*/
	/*	If chktype is -1, match all types.	*/
	/*						*/
	lock(&SM->lock);
	while (p != NULL)
	{
		if (	(p->proto == chkproto || chkproto == -1) &&
			(p->seqno == seqno || seqno == -1))
		{
			n++;
		}
		p = p->next;
	}
	unlock(&SM->lock);

	return n;
}

int
netbyte2int(uchar *data)
{
	return	(int) data[0];
}

int
netint2int(uchar *data)
{
	return (int)((data[0] << 0) |
		(data[1] << 8) |
		(data[2] << 16) |
		(data[3] << 24));
}

int
addreq(uchar *a, uchar *b)
{
	if ((a[0] != b[0]) || (a[1] != b[1]))
	{
		return 0;
	}
	
	return 1;
}

int
schedexists(SMACstate *SM, uchar *originator)
{
	SchedEntry *p;

return 0;

//BUG: for some reason, an item on the list is getting corrupted...
//printf("in schedexists, originator = [%s]\n", originator);

	lock(&SM->lock);
	p = SM->sched_hd;
	while (p != NULL)
	{
//printf("p = 0x%lx, originator = 0x%lx\n", p, originator);
		if (addreq(p->originator, originator))
		{
//printf("match on originator [%s]\n", originator);
			return 1;
		}

		p = p->next;
	}
	unlock(&SM->lock);

	return 0;
}

void
addsched(SMACstate *SM, int start, uchar *originator)
{
	SchedEntry	*ssched;


	/*						*/
	/*	Not explicit in paper, but makes sense	*/
	/*	not to add multiple items for same	*/
	/*	originator				*/
	/*						*/
	if (schedexists(SM, originator))
	{
		return;
	}

	ssched = (SchedEntry*)malloc(sizeof(SchedEntry));
	if (ssched == NULL)
	{
		fatal(Emalloc);
	}

	ssched->framestart_usecs = start;
	memmove(ssched->originator, originator, 2);

	/*						*/
	/*	Specific to our implementation, to	*/
	/*	enable purging of old entries.  Could	*/
	/*	also have set it to cur_usecs:		*/
	/*						*/
	ssched->creation_time = start;

	ssched->next = ssched->prev = NULL;

	lock(&SM->lock);
	if (SM->sched_hd == NULL)
	{
		SM->sched_hd = ssched;
		unlock(&SM->lock);

		return;
	}

	SM->sched_hd->prev = ssched;
	ssched->next = SM->sched_hd;
	SM->sched_hd = ssched;
	unlock(&SM->lock);


	return;	
}

void
delsched(SMACstate *SM, uchar *src)
{
	SchedEntry *p;


	lock(&SM->lock);
	p = SM->sched_hd;
	while (p != NULL)
	{
//@@print("\t\tin delsched, p->originator = [%s]¸ src = [%s]\n", p->originator, src);
		if (addreq(p->originator, src))
		{
			if (p == SM->sched_hd)
			{
				SM->sched_hd = p->next;
				free(p);
				unlock(&SM->lock);

				return;
			}
				
			p->prev->next = p->next;
			free(p);
			unlock(&SM->lock);

			return;
		}

		p = p->next;
	}
	unlock(&SM->lock);

	return;
}


uchar
send0(SMACstate *SM, uchar *data, uchar *dst, int proto, int len, int stream_duration, int nxthdr, uchar seqno)
{
	/*	OK to be on stack, since it is "copied" into network txbuf	*/
	uchar	header[15];

//@@printf("send0(): proto=%d seqno=%d\n", proto, seqno);

	header[0] = SM->addr[0];
	header[1] = SM->addr[1];
	header[2] = dst[0];
	header[3] = dst[1]; 
	header[4] = proto;
	header[5] = (stream_duration >> 0) & 0xFF;
	header[6] = (stream_duration >> 8) & 0xFF;
	header[7] = (stream_duration >> 16) & 0xFF;
	header[8] = (stream_duration >> 24) & 0xFF;
	header[9] = (len >> 0) & 0xFF;
	header[10] = (len >> 8) & 0xFF;
	header[11] = (len >> 16) & 0xFF;
	header[12] = (len >> 24) & 0xFF;
	header[13] = seqno;
	header[14] = nxthdr;

	/*	devmac_xmit takes both the MAC layer header and the payload	*/
	devmac_xmit(dst, SMAC_ADDR_LEN, &header[0], data, 15, len, SM->which_ifc);


	return seqno;
}

uchar
send(SMACstate *SM, uchar *data, uchar *dst, int proto, int len, int stream_duration,
	int nxthdr, int arg_seqno)
{
	uchar		seqno;
	SchedEntry	*p;
	int		qcnt, sched_match;
	int		window_start_off = 0, window_end_off = 0;

//@@printf("send(): proto=%d seqno=%d\n", proto, arg_seqno);

	/*	There is no existing sleep schedule.  Send immediately.		*/
	if (SM->sched_hd == NULL)
	{
//@@print("send():  no existing sleep schedule, sending immediately...\n");
		if (arg_seqno < 0)
		{
			seqno = send0(SM, data, dst, proto, len, stream_duration,
				nxthdr, SM->seqno++);
		}
		else
		{
			seqno = send0(SM, data, dst, proto, len, stream_duration,
				nxthdr, arg_seqno);
		}


		return seqno;
	}

	/*									*/
	/*	This is used both in the initial schedule setup, as well as	*/
	/*	in subsequent synchronization maintenance.			*/
	/*									*/
	/*	SYNCs are sent in the first half of the listen period (i.e.,	*/
	/*	if we already know a sleep/listen schedule.			*/
	/*									*/
	/*		(We repeat for all the schedules we follow:)		*/
	/*	First, sleep until beginning of recipients listen schedule:	*/
	/*									*/
//	lock(&SM->lock);
	p = SM->sched_hd;
//@@printf("looping over scheds...\n");

//	sched_match = schedexists(SM, dst);
	while (p != NULL)
	{
		int	cur_usecs = devrtc_getusecs();
		int	cur_off = cur_usecs - p->framestart_usecs;
		
//@@printf("Sending on sleep schedule, originator = [%s], dst = [%s]...\n", p->originator, dst);
		
		/*								*/
		/*	Not explicit in S-MAC paper, but it maked sense to	*/
		/*	only use the sleep schedule of the destination (if	*/
		/*	it exists, sched_match = 1).  For T_SYNC messages 	*/
		/*	though, we need to send.  Likewise, for broadcast,	*/
		/*	we need to send to all schedules.			*/
		/*								*/
//TODO: ideally, we want to check is there's a match for sched, and if not, to
//	send to all sched, but our sched_exists is currently broken...
//		if (sched_match && proto != T_SYNC && !addreq(dst, p->originator))
		if (	proto != T_SYNC &&
			proto != T_SLP &&
			!addreq(SMAC_BCAST_ADDR, p->originator) &&
			!addreq(dst, p->originator))
		{
//@@printf("Skipping originator [%s]...\n", p->originator);
			p = p->next;
			continue;
		}


		cur_off = cur_off % (SM->listen_usecs + SM->sleep_usecs);

		/*								*/
		/*	Only T_SYNC, T_DATA (in case of broadcasts, where	*/
		/*	no preceding RTS is sent) and T_RTS are delayed.  	*/
		/*	All other types are sent asap (windows_*_off = 0)	*/
		/*								*/
		switch (proto)
		{
			case T_SYNC:
			{
//@@print("Sending on sleep schedule, proto = T_SYNC...\n");
				window_start_off = 0;
				window_end_off = (devrand_getrand() % SM->sync_slots) * SM->slot_usecs;

				break;
			}

			case T_RTS:
			{
//@@print("Sending on sleep schedule, proto = T_RTS...\n");
				window_start_off = SM->sync_slots * SM->slot_usecs;
				window_end_off = window_start_off +
						(devrand_getrand() % SM->data_slots) * SM->slot_usecs;

				break;
			}
		}

		/*								*/
		/*	Except for broadcast data, all other frame types will	*/
		/*	be sent right away.					*/
		/*								*/
		if ((proto == T_DATA) && addreq(SMAC_BCAST_ADDR, dst))
		{
			window_start_off = SM->sync_slots * SM->slot_usecs;
			window_end_off = window_start_off +
					(devrand_getrand() % SM->data_slots) * SM->slot_usecs;
		}

		if (cur_usecs < (p->framestart_usecs + window_start_off))
		{
			/*						*/
			/*	Wait till window. Can't use usleep,	*/
			/*	since we might be in intr hdlr		*/
			/*						*/
			udelay(window_start_off + p->framestart_usecs - cur_usecs);
		}
		else if (cur_usecs >= (p->framestart_usecs + window_end_off))
		{
			/*						*/
			/*	Wait till next sleep frame. Can't use	*/
			/*	usleep, since we might be in intr hdlr	*/
			/*						*/
			udelay(SM->listen_usecs + SM->sleep_usecs - cur_off + window_start_off);
		}

		qcnt = cntq(SM, -1, -1);
		devmac_ctl(NIC_NCR_WRITE, NIC_CMD_POWERUP, SM->which_ifc);

		/*	Can't use sleep; might be in a hdlr	*/
		udelay(window_end_off);

		/*	If no activity on medium, we've got it	*/
		if (!(cntq(SM, -1, -1) - qcnt) && !smac_virtcsense(SM) && !devmac_csense(SM->which_ifc))
		{
			if (arg_seqno < 0)
			{
				seqno = send0(SM, data, dst, proto, len, stream_duration,
						nxthdr, SM->seqno);
			}
			else
			{
				seqno = send0(SM, data, dst, proto, len, stream_duration,
						nxthdr, arg_seqno);
			}
		}
		else
		{
			/*							*/
			/*	We'd recurse if we had nice tail recursion	*/
			/*	in C, but we don't, and we'd rather not risk	*/
			/*	ending up with too deep a stack.		*/
			/*							*/
//@@printf("\tconitnuing (cntq(SM, -1, -1) = %d, smac_virtcsense(SM) = %d, devmac_csense(SM->which_ifc) = %d, SM->vcsense_usecs = %d)...\n",
//@@	cntq(SM, -1, -1), smac_virtcsense(SM), devmac_csense(SM->which_ifc), SM->vcsense_usecs);

			/*							*/
			/*	Need to do this, since if running in a hdlr, 	*/
			/*	timer_hdlr() wont get run.			*/
			/*							*/
			vcsense_update(SM);

			continue;
		}

		if (proto == T_SYNC)
		{
			p->sync_lastsent = devrtc_getusecs();
		}

		p = p->next;
	}
	if (arg_seqno < 0)
	{
		SM->seqno++;
	}

//	unlock(&SM->lock);

//@@print("done sending on sleep schedule.\n");


	return seqno;
}

int
sendchunks(SMACstate *SM, uchar *dst, uchar *data, int len, int max_tries, int nxthdr)
{
	int	qcnt, duration, trycount = 0;
	int	bytes_remain = len;
	int	seqno;



	//BUG: even though we do segmentation, smac_receive currently does not do reassembly.
	//	even though the S-MAC paper implies SAR, it is not fleshed out there, so
	//	we can choose our own impl.  To do SAR, we'd need a "fragment" bit in frame header,
	//	as well as a sequence ID.

	/*									*/
	/*	For broadcast DATA, we don't wait for ACK.  This makes sense,	*/
	/*	alongside our refusal to send ACKs for received broadcasts	*/
	/*	but is a deviation from S-MAC (though probably implied).	*/
	/*									*/
	if (addreq(SMAC_BCAST_ADDR, dst))
	{
int i = 0;
		while (bytes_remain > 0)
		{
			/*							*/
			/*	Make sure vcsense up to date b/4 use. Might	*/
			/*	be in intr hdlr, so timer no trigg'g updates	*/
			/*							*/
			vcsense_update(SM);

			/*	Sleep for duration of virtual csense occupancy	*/
			udelay(SM->vcsense_usecs);

			/*	S-MAC paper, page 6, section D, paragraph 3.	*/
			duration = SM->header_delay_usecs + SM->byte_delay_usecs*bytes_remain;

			send(SM, &data[len - bytes_remain], dst, T_DATA,
				min(bytes_remain, SMAC_CHUNK_SIZE), duration, nxthdr, -1);

			bytes_remain -= min(len, SMAC_CHUNK_SIZE);
//printf("\t\t\tsending chunk %d (broadcast)\n", i++);
		}

		return 0;
	}

	while (bytes_remain > 0)
	{
		seqno = -1;
int i = 0;
		/*							*/
		/*	Make sure vcsense up to date b/4 use. Might	*/
		/*	be in intr hdlr, so timer no trigg'g updates	*/
		/*							*/
		vcsense_update(SM);

		/*	Sleep for duration of virtual csense occupancy	*/
		udelay(SM->vcsense_usecs);

		/*	S-MAC paper, page 6, section D, paragraph 3.	*/
		duration = SM->header_delay_usecs + SM->byte_delay_usecs*bytes_remain;

//TODO: now that we have seqno, we can get rid of the whole qcnt business
		do
		{
//printf("\t\t\tsending chunk %d, %dth/rd try\n", i++, trycount);

//TODO: next 2 statements should be locked to ensure we count before first response
			/*	First seqno auto determined, subsequently identical	*/
			seqno = send(SM, &data[len - bytes_remain], dst, T_DATA,
				min(bytes_remain, SMAC_CHUNK_SIZE), duration, nxthdr, seqno);
			qcnt = cntq(SM, T_ACK, seqno);

			/*								*/
			/*	Wait for ACK.  As per S-MAC paper, increase duration	*/
			/*	estimate.						*/
			/*								*/
			udelay(SM->extension_usecs);
			duration += SM->extension_usecs;
			trycount++;
		} while (trycount < max_tries && ((cntq(SM, T_ACK, seqno) - qcnt) == 0));

//@@printf("getpsr() = 0x%lx\n", getpsr());
//@@printf("cntq(SM, T_ACK, %d) = %d\n", seqno, cntq(SM, T_ACK, seqno));
//@@printf("cntq(SM, -1, %d) = %d\n", seqno, cntq(SM, -1, seqno));

		if (cntq(SM, T_ACK, seqno) == 0)
		{
			return -1;
		}

		/*	purge any ACKs in queue		*/
		while (cntq(SM, T_ACK, seqno))
		{
			smac_freeframe(deqframe(SM, T_ACK, seqno));
		}


		bytes_remain -= min(len, SMAC_CHUNK_SIZE);
		trycount = 0;
	}


	return 0;
}


void
smac_reset(SMACstate *SM)
{
	int		start_offset, initsleep;
	uchar		sync_payload[SMAC_SYNC_PAYLOAD_NBYTES];


	/*									*/
	/*	Sleep for a random period (0 -- max_initsleep). Make this	*/
	/*	initial period large, so that if there's an already existing	*/
	/*	network and we are joining, we inherit their schedule.		*/
	/*									*/
	devmac_ctl(NIC_NCR_WRITE, NIC_CMD_POWERUP, SM->which_ifc);
	initsleep = devrand_getrand() % SM->max_initsleep_slots;
	initsleep *= SM->slot_usecs;
	xusleep(initsleep);

	
	/*								*/
	/*		Have we received any SYNC frames ?		*/
	/*	If we received a SYNC, the addition of it to our	*/
	/*	sleep schedule table, as well as the rebroadcast	*/
	/*	after a random delay sleep, is handled in the		*/
	/*	smac_nichdlr()						*/
	/*								*/
	if (cntq(SM, T_SYNC, -1) == 0)
	{
		/*							*/
		/*	If not, choose a random delay mydelay (0 -- 	*/
		/*	max_syncsleep) to sleep and tell neighbors 	*/
		/*	our sleep schedule				*/
		/*							*/
		start_offset = (devrand_getrand() % SM->max_syncsleep_slots)*SM->slot_usecs;
		addsched(SM, devrtc_getusecs() + start_offset, SM->addr);
		SM->flag |= F_SYNCER;

		sync_payload[0] = (start_offset >> 0) & 0xFF;
		sync_payload[1] = (start_offset >> 8) & 0xFF;
		sync_payload[2] = (start_offset >> 16) & 0xFF;
		sync_payload[3] = (start_offset >> 24) & 0xFF;
		sync_payload[4] = SM->addr[0];
		sync_payload[5] = SM->addr[1];
		send(SM, &sync_payload[0], SMAC_BCAST_ADDR, T_SYNC,
			SMAC_SYNC_PAYLOAD_NBYTES,
			SM->header_delay_usecs + SM->byte_delay_usecs*SMAC_SYNC_PAYLOAD_NBYTES,
			0, -1);
	}

	/*									*/
	/*	If during all of the above, we've recieved a new sleep		*/
	/*	schedule, it is added to sleep schedule list by the		*/
	/*	smac_nichdlr.  An item is also added to the dataq so we		*/
	/*	can check to see what frames we've received when we wake up;	*/
	/*	Dequeue all such SYNC frames since we're done with them		*/
	/*									*/
	while (cntq(SM, T_SYNC, -1))
	{
		smac_freeframe(deqframe(SM, T_SYNC, -1));
	}


	return;
}


void
smac_freeframe(SMACframe *chunk)
{
	if (chunk->data != NULL)
	{
		free(chunk->data);
	}
	free(chunk);

	return;
}


int
smac_transmit(SMACstate *SM, uchar *dst, uchar *data, int len, int max_tries, int nxthdr)
{
	int	seqno = -1, duration, qcnt, trycount = 0;
	int	cur_usecs = devrtc_getusecs();
	int	init_delay;

	/*								*/
	/*	If we are during initial sync window, wait...		*/
	/*								*/
	init_delay = max(
		SM->slot_usecs*(SM->max_initsleep_slots+SM->max_syncsleep_slots) - cur_usecs, 0);
	udelay(init_delay);

	
	/*								*/
	/*	If it is a broadcast, in S-MAC no RTS/CTS is done:	*/
	/*								*/
	if (addreq(dst, SMAC_BCAST_ADDR))
	{
		return sendchunks(SM, dst, data, len, max_tries, nxthdr);
	}

	/*								*/
	/*	In SMAC, only one RTS and CTS pair are sent for the	*/
	/*	burst of "fragments":					*/
	/*								*/
	duration = SM->header_delay_usecs + SM->byte_delay_usecs*len;
//TODO: now that we have seqno, we can get rid of the whole qcnt business
	do
	{
		/*	Send out RTS.  We set duration to entire eventual stream	*/
//TODO: next 2 statements should be locked to ensure we count before first response

		/*	First seqno auto determined, subsequently identical	*/
		seqno = send(SM, NULL, dst, T_RTS, 0, duration, nxthdr, seqno);
		qcnt = cntq(SM, T_CTS, seqno);

		/*							*/
		/*	Wait for CTS.  Specific to our implementation,	*/
		/*	(i.e., not explicitly defined in S-MAC paper,)	*/
		/*	we wait for same delay as for T_DATA/T_ACK.	*/
		/*							*/
		udelay(SM->extension_usecs);
		trycount++;
	} while ((trycount < max_tries) && ((cntq(SM, T_CTS, seqno) - qcnt) == 0));

//@@printf("getpsr() = 0x%lx\n", getpsr());
//@@printf("cntq(SM, T_CTS, %d) = %d\n", seqno, cntq(SM, T_CTS, seqno));
//@@printf("cntq(SM, -1, %d) = %d\n", seqno, cntq(SM, -1, seqno));

	if (cntq(SM, T_CTS, seqno) == 0)
	{
		/*	Got no CTS. Sadly, we have to exit	*/
		return -1;
	}

	while (cntq(SM, T_CTS, seqno))
	{
		smac_freeframe(deqframe(SM, T_CTS, seqno));
	}


	/*	Send short in-band broadcast, which neighbors snoop for vcsense, for duration of eventual stream	*/
	send(SM, NULL, SMAC_BCAST_ADDR, T_SLP, 0, duration, nxthdr, -1);

	/*								*/
	/*		Specific to our implementation:			*/
	/*	Give recipients some time to process the T_SLP.		*/
	/*								*/
	udelay(SM->extension_usecs);

	return sendchunks(SM, dst, data, len, max_tries, nxthdr);
}


SMACstate *
smac_init(uchar *addr, int whichifc, int listen_usecs, int sleep_usecs, int sync_usecs,
	int extension_usecs, int max_initsleep_slots, int max_syncsleep_slots,
	int slot_usecs, int sync_slots, int data_slots)
{
	SMACstate	*SM;


	SM = (SMACstate*)malloc(sizeof(SMACstate));
	if (SM == NULL)
	{
		fatal(Emalloc);
	}

	SM->addr[0]		= addr[0];
	SM->addr[1]		= addr[1];
	SM->listen_usecs	= listen_usecs;
	SM->sleep_usecs		= sleep_usecs;
	SM->sync_usecs		= sync_usecs;
	SM->extension_usecs	= extension_usecs;
	SM->max_initsleep_slots	= max_initsleep_slots;
	SM->max_syncsleep_slots	= max_syncsleep_slots;
	SM->slot_usecs		= slot_usecs;
	SM->sync_slots		= sync_slots;
	SM->data_slots		= data_slots;
	SM->which_ifc		= whichifc;

	/*		Cache the tranmission times for all frame times		*/
	SM->byte_delay_usecs	= 1000000/(1024 * devmac_getspeed(SM->which_ifc) / 8);
	SM->header_delay_usecs	= sizeof(SMACheader) * SM->byte_delay_usecs;

	SM->vcsense_usecs	= 0;
	SM->rtsclear_time	= 0;
	SM->rcvd_scheds		= 0;
	SM->framestart_usec	= 0;
	SM->sync_lastsent	= 0;
	SM->vcsense_lastupdate	= 0;

	SM->dataq_hd = NULL;
	SM->sched_hd = NULL;
	SM->flag = 0;


	return SM;
}

int
smac_virtcsense(SMACstate *SM)
{
	return (SM->vcsense_usecs > 0);
}

SMACframe *
smac_receive(SMACstate *SM, int proto)
{
	return deqframe(SM, proto, -1);
}

void
vcsense_update(SMACstate *SM)
{
	int	diff, cur_usecs = devrtc_getusecs();

	/*	Update the virtual csense occupancy time if necessary	*/
	diff = cur_usecs - SM->vcsense_lastupdate;
	SM->vcsense_usecs -= min(SM->vcsense_usecs, diff);
	SM->vcsense_lastupdate = cur_usecs;

	return;
}

void
smac_timerhdlr(SMACstate *SM)
{
	/*								*/
	/*	This routine is called from the outside by the timer	*/
	/*	interrupt hdlr. 					*/
	/*								*/
	SchedEntry	*p;
	int		frame_length;
	int		cur_usecs;
	int		start_offset, diff, nlisten;
	uchar		seqno, sync_payload[SMAC_SYNC_PAYLOAD_NBYTES];


	vcsense_update(SM);

	frame_length = SM->listen_usecs + SM->sleep_usecs;

	nlisten = 0;

	p = SM->sched_hd;
	while (p != NULL)
	{
		cur_usecs = devrtc_getusecs();
		if (cur_usecs - p->framestart_usecs > frame_length)
		{
			p->framestart_usecs += frame_length;
		}

		start_offset = frame_length - (cur_usecs - p->framestart_usecs);
	
		/*	We only refresh schedules for which we are the originator	*/
		if ((cur_usecs - p->sync_lastsent >= SM->sync_usecs) &&
			addreq(p->originator, SM->addr))
		{
			sync_payload[0] = (start_offset >> 0) & 0xFF;
			sync_payload[1] = (start_offset >> 8) & 0xFF;
			sync_payload[2] = (start_offset >> 16) & 0xFF;
			sync_payload[3] = (start_offset >> 24) & 0xFF;
			sync_payload[4] = SM->addr[0];
			sync_payload[5] = SM->addr[1];
			send(SM, &sync_payload[0], SMAC_BCAST_ADDR, T_SYNC,
				SMAC_SYNC_PAYLOAD_NBYTES,
				SM->header_delay_usecs + 
					SM->byte_delay_usecs*SMAC_SYNC_PAYLOAD_NBYTES,
				0, -1);
		}

		if ((cur_usecs - p->framestart_usecs) <= SM->listen_usecs)
		{
			nlisten++;	
		}

		p = p->next;
	}
	
	/*									*/
	/*	If any current schedule requires listening, power-up the	*/
	/*	interface.							*/
	/*									*/
	if (nlisten > 0)
	{
		devmac_ctl(NIC_NCR_WRITE, NIC_CMD_POWERUP, SM->which_ifc);
	}
	else if (SM->sched_hd != NULL)
	{
		/*							*/
		/*	Avoid shutting down IFC if don't (yet) have	*/
		/*	a sched table					*/
		/*							*/
		devmac_ctl(NIC_NCR_WRITE, NIC_CMD_POWERDN, SM->which_ifc);
	}


	return;
}


void
smac_nichdlr(SMACstate *SM, int whichifc)
{
	/*										*/
	/*	S-MAC MAC layer frame format:						*/
	/*										*/
	/*	0	2	4	5	   9	   13       14	    15		*/
	/*	|  src	|  dst	| proto | duration |  len  | nxthdr | seqno | data...	*/
	/*										*/
	/*	Fields src and dst are self explanatory.  Field proto is		*/
	/*	one of the SMAC frame types, and nxthdr is like the selfsame		*/
	/*	field in IP, i.e., signifies what the type of the payload is.		*/
	/*										*/
	uchar		*src, *dst, *data, *payload, *cksum, frame_cksum[2];
	int		seqno, proto, nxthdr, stream_duration, len, cur_usecs, rx_frame_size;


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
	stream_duration = netint2int(&payload[5]);
	len		= netint2int(&payload[9]);
	seqno		= payload[13];
	nxthdr		= netbyte2int(&payload[14]);
	data		= &payload[15];
	cksum		= &payload[rx_frame_size - 2];

//print("payload [5 -- 9] = [%d][%d][%d][%d], stream_duration = %d", 
//payload[5], payload[6], payload[7], payload[8], stream_duration);
//
//print("payload [9 -- 12] = [%d][%d][%d][%d], len = %d", 
//payload[9], payload[10], payload[11], payload[12], len);

	devmac_cksum(&payload[0], &payload[15], 15, rx_frame_size - 15 - 2, &frame_cksum[0]);
	if ((cksum[0] != frame_cksum[0]) || (cksum[1] != frame_cksum[1]))
	{
//@@printf("smac_nichdlr(): checksum failed\n");

		free(payload);

		return;
	}
	
	cur_usecs = devrtc_getusecs();
	switch(proto)
	{
		case T_SYNC:
		{
//@@printf("smac_nichdlr(): got T_SYNC, seqno = %d\n", seqno);
			SM->rcvd_scheds++;
			
			/*	If we're the originator of schedule, ignore	*/
			if (addreq(SM->addr, &data[4]))
			{
				free(payload);

				return;
			}

			SM->flag |= F_FLLWER;

			/*							*/
			/*	Sleep for random delay, "td" in S-MAC paper,	*/
			/*	then re-broadcast our sleep schedule.		*/
			/*							*/
			/*	Specific to our implementation (check paper?)	*/
			/*	--- we only re-broadcast SYNC if the sender	*/
			/*	was the original synchronizer node, so the	*/
			/*	SYNCs don't plutify unduly.			*/
			/*							*/
			if (addreq(&data[4], src))
			{
				uchar	sync_payload[SMAC_SYNC_PAYLOAD_NBYTES];
				int	sleep_delta, start_offset;


				start_offset = netint2int(data);	
				sleep_delta = min((devrand_getrand() % SM->max_syncsleep_slots)*SM->slot_usecs,
							start_offset);
				udelay(sleep_delta);
				start_offset -= sleep_delta;

				sync_payload[0] = (start_offset >> 0) & 0xFF;
				sync_payload[1] = (start_offset >> 8) & 0xFF;
				sync_payload[2] = (start_offset >> 16) & 0xFF;
				sync_payload[3] = (start_offset >> 24) & 0xFF;
				sync_payload[4] = src[0];
				sync_payload[5] = src[1];
				send(SM, &sync_payload[0], SMAC_BCAST_ADDR, T_SYNC,
					SMAC_SYNC_PAYLOAD_NBYTES,
					SM->header_delay_usecs +
					SM->byte_delay_usecs*SMAC_SYNC_PAYLOAD_NBYTES,
					0, -1);
			}

			/*								*/
			/*	True originator of schedule may be a node other than	*/
			/*	sender, and this info is embedded in T_SYNC payload	*/
			/*								*/
			/*	Delete any exiting schedule from the same originator,	*/
			/*	add new one.						*/
			/*								*/
			delsched(SM, &data[4]);
			addsched(SM, cur_usecs + netint2int(data), &data[4]);
			break;
		}

		case T_RTS:
		{
//@@printf("smac_nichdlr(): got T_RTS, seqno = %d\n", seqno);
			/*	If this is a snooped ACK, update our vcsense	*/
			if (!addreq(SM->addr, dst))
			{
				SM->vcsense_usecs = max(SM->vcsense_usecs, stream_duration);
				SM->vcsense_lastupdate = cur_usecs;
				free(payload);

				return;
			}

			if (cur_usecs > SM->rtsclear_time)
			{
				SM->rtsclear_time = cur_usecs + stream_duration;
				send(SM, NULL, src, T_CTS, 0, stream_duration, 0, seqno);
			}

			/*								*/
			/*	Does not go into receive queue since S-MAC never	*/
			/*	checks for it directly; skip out.			*/
			/*								*/
			free(payload);

			return;
		}

		case T_CTS:
		{
//@@printf("smac_nichdlr(): got T_CTS, seqno = %d\n", seqno);
			if (!addreq(SM->addr, dst))
			{
				SM->vcsense_usecs = max(SM->vcsense_usecs, stream_duration);
				SM->vcsense_lastupdate = cur_usecs;
				free(payload);

				return;
			}

			/*	Gets enqueued in rxqueue	*/
			break;
		}

		case T_DATA:
		{
//@@printf("smac_nichdlr(): got T_DATA, seqno = %d\n", seqno);
			// TODO: need to implement promiscuous mode state for NIC in simulator

			/*	If this is a snooped DATA, update our vcsense	*/
			if (!addreq(SM->addr, dst) && !addreq(SMAC_BCAST_ADDR, dst))
			{
//@@print("smac_nichdlr(): data is a snooped packet. free()ing payload and skipping out\n");
				SM->vcsense_usecs = max(SM->vcsense_usecs, stream_duration);
				SM->vcsense_lastupdate = cur_usecs;
				free(payload);

				return;
			}

			/*	I don't think this is explicitly specified in S-MAC	*/
			/*	paper, but it makes sense not to ACK broadcasts, as	*/
			/*	that would create an "ACK storm" for each broadcast	*/
			/*	also, in smac_xmit, we don't wait for ACK if its a	*/
			/*	bcast (I don't think that is specified in S-MAC paper	*/
			/*	either)							*/
			if (addreq(SMAC_BCAST_ADDR, dst))
			{
				break;
			}

			/*								*/
			/*	In S-MAC, ack is snooped, and contains duration, 	*/
			/*	to let hidden terminals snoop, for their vcsense.	*/
			/*								*/
//@@print("smac_nichdlr(): got T_DATA,sending T_ACK\n");
			send(SM, NULL, src, T_ACK, 0, stream_duration, nxthdr, seqno);

			break;
		}

		case T_ACK:
		{
//@@printf("smac_nichdlr(): got T_ACK, seqno = %d\n", seqno);
			/*	If this is a snooped ACK, update our vcsense	*/
			if (!addreq(SM->addr, dst))
			{
				SM->vcsense_usecs = max(SM->vcsense_usecs, stream_duration);
				SM->vcsense_lastupdate = cur_usecs;
				free(payload);

				return;
			}

			/*	Gets enqueued in rxqueue	*/
			break;
		}

		case T_SLP:
		{
//@@printf("smac_nichdlr(): got T_SLP, seqno = %d\n", seqno);
			SM->vcsense_usecs = max(SM->vcsense_usecs, stream_duration);
			SM->vcsense_lastupdate = cur_usecs;

			/*								*/
			/*	Does not go into receive queue since S-MAC never	*/
			/*	checks for it directly; skip out.			*/
			/*								*/
			free(payload);

			return;
		}
	
		default: fatal(Ebadproto);
	}
//@@print("smac_nichdlr(): enqueueing received frame...\n");

	enqframe(SM, src, dst, len, proto, nxthdr, seqno, data);
	free(payload);


	return;
}
