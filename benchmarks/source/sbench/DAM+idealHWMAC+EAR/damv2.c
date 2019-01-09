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

#include "sf-types.h"
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

/*						*/
/*	The bcast addr for idealized MAC.  	*/
/*	Route layer uses this variable 		*/
/*	when it needs to			*/
/*						*/
const uchar		MAC_BCAST_ADDR[] = "::1";

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
main(int argc, char *argv[])
{
	Dampkt		dam_p;
	char		tmp;
	char		*ep = &tmp;
	ulong		start = 0;
	long		sluimer;
	int		period = 0;
	int		tmpperiod, tmp_alpha, tmp_beta, tmp_ftable_alpha,
			tmp_eij, tmp_maxhops;


	/*
	 *	Because the 16-byte ID is a sequence of bytes coming from the emulator
	 *	underneath and is not necessarily a null-terminated string, we need to
	 *	explicitly null-terminate. Second, we can't use strcpy/memmove anymore
	 *	with recent versions of the C library since they optimize by doing a
	 *	double-word access. We therefore have to manually read each of the
	 *	NIC_OUI bytes.
	 */
	dam_myID[0] = *(volatile uchar *)(NIC_OUI+0);
	dam_myID[1] = *(volatile uchar *)(NIC_OUI+1);
	dam_myID[2] = *(volatile uchar *)(NIC_OUI+2);
	dam_myID[3] = *(volatile uchar *)(NIC_OUI+3);
	dam_myID[4] = *(volatile uchar *)(NIC_OUI+4);
	dam_myID[5] = *(volatile uchar *)(NIC_OUI+5);
	dam_myID[6] = *(volatile uchar *)(NIC_OUI+6);
	dam_myID[7] = *(volatile uchar *)(NIC_OUI+7);
	dam_myID[8] = *(volatile uchar *)(NIC_OUI+8);
	dam_myID[9] = *(volatile uchar *)(NIC_OUI+9);
	dam_myID[10] = *(volatile uchar *)(NIC_OUI+10);
	dam_myID[11] = *(volatile uchar *)(NIC_OUI+11);
	dam_myID[12] = *(volatile uchar *)(NIC_OUI+12);
	dam_myID[13] = *(volatile uchar *)(NIC_OUI+13);
	dam_myID[14] = *(volatile uchar *)(NIC_OUI+14);
	dam_myID[15] = *(volatile uchar *)(NIC_OUI+15);
	dam_myID[DAM_ID_LEN] = '\0';
	hdlr_install();
	print("DAM node [%s] installed vector code.\n", dam_myID);

	devmac_ctl(NIC_NCR_WRITE, NIC_CMD_POWERUP, 0);
	print("DAM node [%s] powered up NIC.\n", dam_myID);

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

NETTRACEMARK(4);
LOGMARK(8);
	devnet_xmit("::1", PROTO_DAM_INTERNAL, data, 4 + 2*(DAM_ID_LEN+1) + 2*8, 0);
LOGMARK(9);
NETTRACEMARK(5);

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
	int			whichifc;
	char			proto;
	uchar			src_addr[16+1];
	uchar			rxbuf[MAX_FRAMEBYTES];
	uchar			*tdataptr;
	ulong 			tframelen, timestamp, now;
	ulong   		rx_frame_size;
	EARSrcPayloadTuple	*x;


        /*      Lower 12 bits of interrupt code specify IFC #   */
        whichifc = evt & 0xFFF;

NETTRACEMARK(8);
LOGMARK(0);
	rx_frame_size = devnet_getrxfsz(whichifc);
//printf("\tRX frame size is %d\n", rx_frame_size);


        devnet_recv(&rxbuf[0], rx_frame_size, whichifc);
	memmove(&src_addr[0], &rxbuf[0], 16);

	/*	Get payload and length		*/
	tdataptr = &rxbuf[0]+37;
	tframelen = 0;
	tframelen |= rxbuf[32] << 24;
	tframelen |= rxbuf[33] << 16;
	tframelen |= rxbuf[34] << 8;
	tframelen |= rxbuf[35];

	proto = rxbuf[36];
LOGMARK(1);
NETTRACEMARK(9);

	now = devrtc_getusecs();

	/*	DEBUG	*/
	printf("\t\tDAM node [%s]: Just got frame from node [%s], length [%lu], proto [%s]\n",
		dam_myID, &src_addr[0], tframelen,
		(proto == PROTO_DAM_INTERNAL ? "DAM" : (proto == PROTO_EAR ? "EAR" : "UNKNOWN")));

	if (proto == PROTO_EAR)
	{
NETTRACEMARK(12);
LOGMARK(18);
		x = ear_recv(Ear, tdataptr, tframelen);
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
		free(x);
LOGMARK(19);
NETTRACEMARK(13);

		return;
	}


	/*	There will always be some old frames, since broadcast leads to loops	*/
	memmove(&timestamp, tdataptr, 4);
	if ((now - timestamp) > dam_period)
	{
		printf("Frame is too old (timestamp  @ %d usecs) discarding...\n", timestamp);

		return;
	}
	
	if (proto == PROTO_DAM_INTERNAL)
	{		
LOGMARK(14);	
		dam_rcv_pkt(tdataptr);
LOGMARK(15);
	}

	return;
}

void
intr_hdlr(void)
{
	int	evt = devexcp_getintevt();


	/*	Only call nic_hdlr() for RX_OK interrupts	*/
	if ((evt >= NIC_RX_EXCP_CODE) && (evt < NIC_RX_EXCP_CODE_END))
	{
		nic_hdlr(evt);
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

	return devnet_xmit(ear_dst_addr, PROTO_EAR, ear_hdr, hdrlen, 0);
}


void
fatal(char *str)
{
	lprint("Node [%s] Fatal: %s\n", dam_myID, str);
	exit(-1);
}
