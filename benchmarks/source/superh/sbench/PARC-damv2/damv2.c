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
#include "../../../port/devnet.h"
#include "../../../port/devrtc.h"
#include "../../../port/devexcp.h"
#include "../../../port/devlog.h"
#include "../../../port/devloc.h"
#include "../../../port/devsensor.h"
#include "../../../port/misc.h"
#include "../../../port/superHspecific.h"
#include "fault.h"
#include "physics.h"
#include "interrupts-hitachi-sh.h"
#include "network-hitachi-sh.h"
#include "print.h"
#include "batt.h"

enum
{
	LIGHT_SENSOR = 0,
	TRUE = 1,
	FALSE = 0,
};

enum
{
	PROTO_DAM		= 0,
	DAM_PROTOCOL_PERIOD	= 2000,
	DAM_ID_LEN		= 16,
};

#define	DAM_THRESHOLD_ELECTION	50.0

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

static void		hdlr_install(void);
static void		dam_broadcast(Dampkt *p);
static void		dam_rcv_pkt(char *data);
static void		write_log(ulong period);

extern int		pow10(int y);

volatile float		dam_myPr, dam_maxPrHeard, dam_delta = 0.0;
volatile int		dam_participating;
volatile long		dam_period;
volatile uchar		dam_myID[DAM_ID_LEN], dam_leaderID[DAM_ID_LEN], dam_myParent[DAM_ID_LEN];

void
main(int argc, char *argv[])
{
	Dampkt		dam_p;
	char		tmp;
	char		*ep = &tmp;
	ulong		start = 0;
	long		sluimer;
	int		period = 0;


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

	printf("DAM node [%s] installing vector code...\n", dam_myID);


	dam_period = DAM_PROTOCOL_PERIOD;
	if (argc == 1)
	{
		int tmpperiod = strtol(argv[0], &ep, 0);
		if (*ep != '\0')
		{
			printf("Invalid DAM period supplied as argument.\n");
		}
		else
		{
			dam_period = tmpperiod;
			printf("Set  dam_period to [%ld] usecs\n", dam_period);
		}
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
	devnet_xmit("::1", PROTO_DAM, data, 4 + 2*(DAM_ID_LEN+1) + 2*8, 0);
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
	char	buf[1024];


	sprintf(buf, "\t\t@ %lu:\tdevrtc_getusecs = [%lu]\n", period, devrtc_getusecs());
	lprint(buf);
	sprintf(buf, "\t\t@ %lu:\tparticipating = [%d]\n", period, dam_participating);
	lprint(buf);
	sprintf(buf, "\t\t@ %lu:\tmyPr = [%E]\n", period, dam_myPr);
	lprint(buf);
	sprintf(buf, "\t\t@ %lu:\tmaxPrHeard = [%E]\n", period, dam_maxPrHeard);
	lprint(buf);
	sprintf(buf, "\t\t@ %lu:\tdelta = [%E]\n", period, dam_delta);
	lprint(buf);
	sprintf(buf, "\t\t@ %lu:\tmyID = [%s]\n", period, dam_myID);
	lprint(buf);
	sprintf(buf, "\t\t@ %lu:\tleaderID = [%s]\n", period, dam_leaderID);
	lprint(buf);
	sprintf(buf, "\t\t@ %lu:\tmyParent = [%s]\n", period, dam_myParent);
	lprint(buf);

	if (!strcmp(dam_myID, dam_leaderID))
	{
		sprintf(buf,
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
	int	whichifc;
	char	proto;
	uchar	src_addr[16+1];
	uchar	rxbuf[MAX_FRAMEBYTES];
	uchar	*tdataptr;
	ulong 	tframelen, timestamp, now;
	ulong   rx_frame_size;


        /*      Lower 12 bits of interrupt code specify IFC #   */
        whichifc = evt & 0xFFF;

NETTRACEMARK(8);
LOGMARK(0);
        rx_frame_size = devnet_getrxfsz(whichifc);
        devnet_recv(&rxbuf[0], rx_frame_size, whichifc);
	memmove(&src_addr, &rxbuf[0], 16);

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
		(proto == PROTO_DAM ? "DAM" : "UNKNOWN"));

	/*	There will always be some old frames, since broadcast leads to loops	*/
	memmove(&timestamp, tdataptr, 4);
	if ((now - timestamp) > dam_period)
	{
		printf("Frame is too old (timestamp  @ %d usecs) discarding...\n", timestamp);

		return;
	}
	
	if (proto == PROTO_DAM)
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

