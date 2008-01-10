/*										*/
/*	This is damv2, fixing the unrealistic location of write_log, added 	*/
/*	timestamps and discarding of frames from prior periods.			*/
/*										*/
/*		Implementation of DAM protocol from the paper:			*/
/*			"Lightweight Sensing and Communication Protocols	*/
/*			for Target Enumeration and Aggregation"			*/
/*										*/
/*	Format of a DAM packet is as follows:					*/
/*										*/
/*	0		17		34		42		50	*/
/*	|    transID	|     maxID	|     maxPr	|    transPr	|	*/
/*										*/
/*	Each individual field is in Big-endian byte order.			*/
/*										*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "e-types.h"
#include "tag.h"
#include "devsim7708.h"
#include "sh7708.h"
#include "devscc.h"
#include "devnet.h"
#include "devmac.h"
#include "devrtc.h"
#include "devexcp.h"
#include "devlog.h"
#include "devloc.h"
#include "devsensor.h"
#include "misc.h"
#include "fault.h"
#include "physics.h"
#include "interrupts-hitachi-sh.h"
#include "network-hitachi-sh.h"
#include "print.h"
#include "batt.h"
#include "ear-route.h"
#include "sift-mac.h"

enum
{
	LIGHT_SENSOR = 0,
	TRUE = 1,
	FALSE = 0,
};

enum
{
	PROTO_DAM_INTERNAL	= 0,
	DAM_PROTOCOL_PERIOD	= 2000,
	DAM_ID_LEN		= 16,
};

#define	DAM_THRESHOLD_ELECTION	10.0

#ifdef LOGMARKERS
#	include "logmarkers.h"
#endif

typedef struct Dampkt Dampkt;
struct Dampkt
{
	ulong	timestamp;

	char	transID[DAM_ID_LEN + 1];
	char	maxID[DAM_ID_LEN + 1];
	float	maxPr;
	float	transPr;
};

/*	    8 GPRs + PR		*/
uchar		REGSAVESTACK[36];
EARState	*Ear;
SiftState	*SIFT_MAC;

/*						*/
/*	The bcast addr for idealized MAC.  	*/
/*	Route layer uses this variable 		*/
/*	when it needs to			*/
/*						*/
uchar			MAC_BCAST_ADDR[2] = {~0, ~0};

static void		hdlr_install(void);
static void		dam_broadcast(Dampkt *p);
static void		dam_rcv_pkt(char *data);
static void		write_log(ulong period);

void			fatal(char *str);

extern int		pow10(int y);

volatile float		dam_myPr, dam_maxPrHeard, dam_delta = 0.0;
volatile int		dam_participating;
volatile long		dam_period;
volatile uchar		dam_myID[DAM_ID_LEN], dam_leaderID[DAM_ID_LEN],
			dam_myParent[DAM_ID_LEN], dam_sink[DAM_ID_LEN];

void
startup(int argc, char *argv[])
{
	Dampkt		dam_p;
	char		tmp;
	char		*ep = &tmp;
	ulong		start = 0;
	long		sluimer;
	int		period = 0;
	int		tmpperiod, tmp_alpha, tmp_beta, tmp_ftable_alpha,
			tmp_eij, tmp_maxhops;


	SIFT_MAC = NULL;
	strncpy(dam_myID, (char *)NIC_OUI, DAM_ID_LEN);
	hdlr_install();

	print("DAM node [%s] done installing vector code...\n", dam_myID);
	print("DAM node [%s] initializing Sift-MAC... ", dam_myID);

	/*									*/
	/*	alpha = 0.138 for CW = 4 gives approx. the same prob of 	*/
	/*	picking slot 0, as well as same ballpark of ration of prob	*/
	/*	of picking last slot versus first slot, as alpha = 0.82 and 	*/
	/*	CW = 32 used as an example in the Sift paper.			*/
	/*									*/
	SIFT_MAC = sift_init(&dam_myID[0], 0,	/*	ID and IFC #0	*/
			0.138,			/*	alpha		*/
			4,			/*	CW		*/
			1000,			/*	tdifs		*/
			50000,			/*	tsifs 		*/
			50000,			/*	tACKTimeout	*/
			15000,			/*	tslot		*/
			72,			/*	chunk size here is EAR payload-less pkt  */
			1			/*	# of ACK timeouts to wait for single ACK	*/
			);
	print("done.\n");

	dam_period = DAM_PROTOCOL_PERIOD;
	if (argc == 6)
	{
		tmpperiod = strtol(argv[0], &ep, 0);
		if (*ep != '\0')
		{
			printf("Invalid DAM period supplied as argument.\n");
		}
		else
		{
			dam_period = tmpperiod;
			printf("Set dam_period to [%d] usecs\n", dam_period);
		}

		tmp_alpha = strtol(argv[1], &ep, 0);
		if (*ep != '\0')
		{
			printf("Invalid EAR alpha supplied as argument.\n");
		}
		
		tmp_beta = strtol(argv[2], &ep, 0);
		if (*ep != '\0')
		{
			printf("Invalid EAR beta supplied as argument.\n");
		}

		tmp_ftable_alpha = strtol(argv[3], &ep, 0);
		if (*ep != '\0')
		{
			printf("Invalid EAR ftable_alpha supplied as argument.\n");
		}

		tmp_eij = strtol(argv[4], &ep, 0);
		if (*ep != '\0')
		{
			printf("Invalid EAR eij supplied as argument.\n");
		}

		tmp_maxhops = strtol(argv[5], &ep, 0);
		if (*ep != '\0')
		{
			printf("Invalid EAR maxhops supplied as argument.\n");
		}

		Ear = ear_init(dam_myID, 0, tmp_alpha, tmp_beta, tmp_ftable_alpha,
				tmp_eij, tmp_maxhops,
			(uchar)devloc_getxloc(), (uchar)devloc_getyloc(), (uchar)devloc_getzloc());
		fprintf(stderr, "DAM node [%s] completed EAR init with runtime args:\n"
			"\talpha=%d\n\tbeta=%d\n\tftable_alpha=%d\n\teij=%d\n\tmaxhops=%d\n\n\n\n", 
			dam_myID, tmp_alpha, tmp_beta, tmp_ftable_alpha, tmp_eij, tmp_maxhops);

	}
	else
	{
		Ear = ear_init(dam_myID, 0, 1, 50, 1, 1 /* eij */, 8 /* maxhops */,
			(uchar)devloc_getxloc(), (uchar)devloc_getyloc(), (uchar)devloc_getzloc());
		print("DAM node [%s] completed EAR init using defaults.\n\n\n", dam_myID);
	}


	/*							*/
	/*	Implemented to mirror description in paper	*/
	/*	All variables beginning w/ dam_ correspond	*/
	/*	to variables in the paper's algorithm descr.	*/
	/*							*/
	while (1)
	{
		start = devrtc_getusecs();
		dam_p.timestamp = start;

		/*									*/
		/*	Write the log for the previous period. We want all actions to	*/
		/*	be in the timed loop, and though this log writing may seem to	*/
		/*	be not inherent to application, you can think of it as some	*/
		/*	post-peak detection actions that the algorithm must perform.	*/
		/*									*/
		if ((period > 0) && dam_participating)
		{
			if (strlen(dam_sink) > 0)
			{
				int		id;
				char		tmp;
				char		*ep = &tmp;

				id = strtol(dam_leaderID, &ep, 0);
				ear_response(Ear, dam_sink, NULL, 0, id);
			}
//print("\n\ndam_sink = [%s]\n\n\n", dam_sink);
		
			write_log(period - 1);
		}

		/*								*/
		/*	The values in this case are in Lux (see test.m)		*/
		/*	Noise floor is 0.1 Lux. DAM_THRESHOLD_ELECTION is	*/
		/*	thus set to 10 Lux.					*/
		/*								*/
		dam_myPr = devsignal_read(LIGHT_SENSOR);

		/*								*/
		/*	Algorithm description in PARC paper does not reset	*/
		/*	maxPrHeard. For each DAM period, until a packet is	*/
		/*	received, or our local reading is > threshold, the	*/
		/*	maxPrHeard should be 0.					*/
		/*								*/
		dam_maxPrHeard = 0;

		if (dam_myPr > DAM_THRESHOLD_ELECTION)
		{
LOGMARK(12);
			dam_participating = TRUE;
			dam_maxPrHeard = dam_myPr;
			strncpy(dam_leaderID, dam_myID, DAM_ID_LEN);
			dam_p.maxPr = dam_p.transPr = dam_myPr;
			strncpy((char *)dam_p.transID, dam_myID, DAM_ID_LEN);
			strncpy((char *)dam_p.maxID, dam_myID, DAM_ID_LEN);

			dam_broadcast(&dam_p);
LOGMARK(13);
		}
		else
		{
			dam_participating = FALSE;
		}

		sluimer = dam_period - (devrtc_getusecs() - start);
		sluimer = max(sluimer, 0);
LOGMARK(2);
		xusleep(sluimer);
LOGMARK(3);
		period++;
	}

	return;		
}

void
dam_broadcast(Dampkt *p)
{
	uchar	data[4 + 2*(DAM_ID_LEN+1) + 2*8];

	memmove(&data[0],  &(p->timestamp), 4);
	memmove(&data[4], p->transID, DAM_ID_LEN+1);
	memmove(&data[4 + DAM_ID_LEN+1],  p->maxID, DAM_ID_LEN+1);
	memmove(&data[4 + 2*(DAM_ID_LEN+1)],  &(p->maxPr), 8);
	memmove(&data[4 + 2*(DAM_ID_LEN+1) + 8],  &(p->transPr), 8);

NETTRACEMARK(2);
LOGMARK(6);
	sift_xmit(SIFT_MAC, SIFT_BCAST_ADDR, data,
		4 + 2*(DAM_ID_LEN+1) + 2*8, PROTO_DAM_INTERNAL);
LOGMARK(7);
NETTRACEMARK(3);

	return;
}

void
dam_rcv_pkt(char *data)
{
	Dampkt	p;
	
	memmove(&p.timestamp, &data[0], 4);
	strncpy(p.transID, &data[4], DAM_ID_LEN);
	strncpy(p.maxID, &data[4 + DAM_ID_LEN+1], DAM_ID_LEN);

	p.maxPr = *((float *)&data[4 + 2*(DAM_ID_LEN+1)]);
	p.transPr = *((float *)&data[4 + 2*(DAM_ID_LEN+1) + 8]);

printf(">>> node %s (w/ myPr = %E, maxPrHeard = %E), got p.transID = [%s], p.maxID = [%s], p.maxPr = [%E], p.transPr = [%E] <<<\n",
	dam_myID, dam_myPr, dam_maxPrHeard, p.transID, p.maxID, p.maxPr, p.transPr);

	if ((p.maxPr > dam_maxPrHeard) && ((p.transPr + dam_delta) > dam_myPr))
	{
		dam_maxPrHeard = p.maxPr;
		strncpy(dam_leaderID, p.maxID, DAM_ID_LEN);
		strncpy(dam_myParent, p.transID, DAM_ID_LEN);
		p.transPr = dam_myPr;
		strncpy(p.transID, dam_myID, DAM_ID_LEN);
		dam_broadcast(&p);
	}

	return;
}

void
write_log(ulong period)
{
#	define	buflen	1024
	char	buf[buflen];


	snprintf(buf, buflen, "\t\t@ %lu:\tdevrtc_getusecs = [%lu]\n", period, devrtc_getusecs());
	lprint(buf);
	snprintf(buf, buflen, "\t\t@ %lu:\tparticipating = [%d]\n", period, dam_participating);
	lprint(buf);
	snprintf(buf, buflen, "\t\t@ %lu:\tmyPr = [%E]\n", period, dam_myPr);
	lprint(buf);
	snprintf(buf, buflen, "\t\t@ %lu:\tmaxPrHeard = [%E]\n", period, dam_maxPrHeard);
	lprint(buf);
	snprintf(buf, buflen, "\t\t@ %lu:\tdelta = [%E]\n", period, dam_delta);
	lprint(buf);
	snprintf(buf, buflen, "\t\t@ %lu:\tmyID = [%s]\n", period, dam_myID);
	lprint(buf);
	snprintf(buf, buflen, "\t\t@ %lu:\tleaderID = [%s]\n", period, dam_leaderID);
	lprint(buf);
	snprintf(buf, buflen, "\t\t@ %lu:\tmyParent = [%s]\n", period, dam_myParent);
	lprint(buf);

	if (!strcmp(dam_myID, dam_leaderID))
	{
		snprintf(buf, buflen,
			"\n%lu\t%lu\tpeak @ location\t%E %E %E\tnode %s\n",
			period, devrtc_getusecs(), devloc_getxloc(), devloc_getyloc(),
			devloc_getzloc(), dam_myID);
		lprint(buf);
	}

	sprintf(buf, "\n");
	lprint(buf);

	return;
}

void
nic_hdlr(int evt)
{
	int		whichifc;
	SiftFrame	*payload;
	ulong 		timestamp, now;
	EARSrcPayloadTuple	*x;


        /*      Lower 12 bits of interrupt code specify IFC #   	*/
        whichifc = evt & 0xFFF;
	
NETTRACEMARK(8);
LOGMARK(0);
	/*	Let the MAC layer take care of whatever it needs to	*/
	sift_nichdlr(SIFT_MAC, whichifc);
LOGMARK(1);
NETTRACEMARK(9);

	/*								*/
	/*	Now, retrieve the payload from the MAC layer, if any	*/
	/*								*/
	payload = sift_receive(SIFT_MAC, FRAME_DATA);
	if (payload == NULL)
	{
		printf("\t\tDAM node [%s]: Just got non-data frame\n", dam_myID);

		return;
	}

	now = devrtc_getusecs();

	/*	DEBUG	*/
	printf("\t\tDAM node [%s]: Just got frame from node [%s], length [%d], proto [%s]\n",
		dam_myID, payload->src, payload->len,
		(payload->nxthdr == PROTO_DAM_INTERNAL ? "DAM" : (payload->nxthdr == PROTO_EAR ? "EAR" : "UNKNOWN")));

	if (payload->nxthdr == PROTO_EAR)
	{
NETTRACEMARK(12);
LOGMARK(18);
		x = ear_recv(Ear, payload->data, payload->len);
		if (x != NULL)
		{
			int		id;
			char		tmp;
			char		*ep = &tmp;


			id = strtol(dam_leaderID, &ep, 0);

			/*							*/
			/*	Immediately send response w/ no payload		*/
			/*	but with DAM leader ID in nxthdr field 		*/
			/*	(we ignore incoming payload which is in 	*/
			/*	x->payload). Cache sink for use whenever	*/
			/*	we are in DAM cluster				*/
			/*							*/
			strncpy(dam_sink, x->src, DAM_ID_LEN);
			if (dam_participating)
			{
				ear_response(Ear, dam_sink, NULL, 0, id);
			}

//printf("\n\n\nsent EAR response = [%d] (dam_leader = [%s])\n\n\n",
//	(dam_leaderID[0] - '0')*10 + (dam_leaderID[1]-'0'), dam_leaderID);
		}
		sift_freeframe(payload);
		free(x);
LOGMARK(19);
NETTRACEMARK(13);

		return;
	}

	/*	There will always be some old frames, since broadcast leads to loops	*/
	memmove(&timestamp, payload->data, 4);
	if ((now - timestamp) > dam_period)
	{
		printf("Frame is too old (timestamp  @ %lu usecs) discarding...\n", timestamp);
		sift_freeframe(payload);

		return;
	}
	
	if (payload->nxthdr == PROTO_DAM_INTERNAL)
	{	
LOGMARK(14);	
		dam_rcv_pkt(payload->data);
LOGMARK(15);
	}
	sift_freeframe(payload);


	return;
}

void
intr_hdlr(void)
{
	int	evt = devexcp_getintevt();


	/*	Only call nic_hdlr() for RX_OK interrupts	*/
	if ((evt >= NIC_RX_EXCP_CODE) && (evt < NIC_RX_EXCP_CODE_END))
	{
		/*	Only begin triggering when sift_init is done	*/
		if (SIFT_MAC != NULL)
		{
			nic_hdlr(evt);
		}
	}
	else if (evt == TMU0_TUNI0_EXCP_CODE)
	{
	}

	return;
}

void
hdlr_install(void)
{
	extern	uchar	vec_stub_begin, vec_stub_end;
	uchar	*dstptr = (uchar *)0x8000600;
	uchar	*srcptr = &vec_stub_begin;


	/*	Copy the vector instructions to vector base	*/
	while (srcptr < &vec_stub_end)
	{
		*dstptr++ = *srcptr++;
	}

	return;
}

//TODO: move this into an "ideal-mac" directory, and use the
//	same interfaces we use for sift/smac/etc. to channel
//	data over devnet_xmit, using the idealized MAC layer
int
mac_xmit(uchar *ear_dst_addr, uchar *ear_hdr, int hdrlen, uchar *data, int datalen)
{
//fprintf(stderr, "mac_xmit(), ear_dst_addr=[%s]\n", ear_dst_addr);

//	return devnet_xmit(ear_dst_addr, PROTO_EAR, ear_hdr, hdrlen, 0);

	return sift_xmit(SIFT_MAC, ear_dst_addr, ear_hdr, hdrlen, PROTO_EAR);
}


void
fatal(char *str)
{
	lprint("Node [%s] Fatal: %s\n", dam_myID, str);
	exit(-1);
}
