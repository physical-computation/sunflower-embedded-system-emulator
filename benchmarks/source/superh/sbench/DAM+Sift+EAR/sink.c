/*										*/
/*	EAR-Sink.  Periodically query a random node in the network.		*/
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
#include "../../../port/devmac.h"
#include "../../../port/devrtc.h"
#include "../../../port/devexcp.h"
#include "../../../port/devlog.h"
#include "../../../port/devloc.h"
#include "../../../port/devrand.h"
#include "../../../port/devsensor.h"
#include "../../../port/misc.h"
#include "../../../port/superHspecific.c"
#include "fault.h"
#include "physics.h"
#include "interrupts-hitachi-sh.h"
#include "network-hitachi-sh.h"
#include "print.h"
#include "batt.h"
#include "errors.h"
#include "ear-route.h"
#include "sift-mac.h"

enum
{
	ID_LEN 			= 16,
	EAR_SINK_PERIOD		= 1000000,
	NETWORK_SIZE 		= 25,
	PROTO_DAM_INTERNAL	= 0,
};

#ifdef LOGMARKERS
#	include "logmarkers.h"
#endif

/*	    8 GPRs + PR		*/
uchar			REGSAVESTACK[36];
EARState		*Ear;
SiftState		*SIFT_MAC;
volatile uchar		myID[ID_LEN+1];
char			src[ID_LEN+1];
int			period = 0;


/*						*/
/*	The bcast addr for idealized MAC.  	*/
/*	Route layer uses this variable 		*/
/*	when it needs to			*/
/*						*/
uchar			MAC_BCAST_ADDR[2] = {~0, ~0};

static void		hdlr_install(void);
static void		write_log(ulong period, char *, int);
static ulong		uniform(ulong min, ulong max);

void			fatal(char *str);

extern int		pow10(int y);



static ulong
uniform(ulong min, ulong max)
{
	return min + (devrand_getrand() % ((max-min)+1));
}

void
main(int argc, char *argv[])
{
	char		tmp;
	char		*ep = &tmp;
	ulong		start = 0;
	long		sluimer;
	int		ear_sink_period;
	int		tmpperiod, tmp_alpha, tmp_beta, tmp_ftable_alpha,
			tmp_eij, tmp_maxhops;


	SIFT_MAC = NULL;
	strncpy(myID, (char *)NIC_OUI, ID_LEN);
	hdlr_install();

	print("EAR Sink node [%s] done installing vector code...\n", myID);
	print("EAR Sink node [%s] initializing Sift-MAC... ", myID);

	/*									*/
	/*	alpha = 0.138 for CW = 4 gives approx. the same prob of 	*/
	/*	picking slot 0, as well as same ballpark of ration of prob	*/
	/*	of picking last slot versus first slot, as alpha = 0.82 and 	*/
	/*	CW = 32 used as an example in the Sift paper.			*/
	/*									*/
	SIFT_MAC = sift_init(&myID[0], 0,	/*	ID and IFC #0	*/
			0.138,			/*	alpha		*/
			4,			/*	CW		*/
			1000,			/*	tdifs		*/
			50000,			/*	tsifs 		*/
			50000,			/*	tACKTimeout	*/
			15000,			/*	tslot		*/
			72,			/*	chunk size here is EAR payload-less pkt 	*/
			1			/*	# of ACK timeouts to wait for single ACK	*/
			);
	print("done.\n");

	ear_sink_period = EAR_SINK_PERIOD;
	if (argc == 6)
	{
		tmpperiod = strtol(argv[0], &ep, 0);
		if (*ep != '\0')
		{
			printf("Invalid EAR-sink period (\"%s\") supplied as argument.\n",
				argv[0]);
		}
		else
		{
			ear_sink_period = tmpperiod;
			printf("Set ear_sink_period to [%d] usecs\n", ear_sink_period);
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

		/*								*/
		/*		Initalize, EAR routing state			*/
		/*								*/
		Ear = ear_init(myID, 0, tmp_alpha, tmp_beta, tmp_ftable_alpha,
				tmp_eij, tmp_maxhops,
			(uchar)devloc_getxloc(), (uchar)devloc_getyloc(), (uchar)devloc_getzloc());
		fprintf(stderr, "EAR Sink node [%s] completed EAR init with runtime args:\n"
			"\talpha=%d\n\tbeta=%d\n\tftable_alpha=%d\n\teij=%d\n\tmaxhops=%d\n\n\n\n", 
			myID, tmp_alpha, tmp_beta, tmp_ftable_alpha, tmp_eij, tmp_maxhops);

	
	}
	else
	{
		/*								*/
		/*		Initalize, EAR routing state			*/
		/*								*/
		Ear = ear_init(myID, 0, 1, 50, 1, 1 /* eij */, 8 /* maxhops */,
			(uchar)devloc_getxloc(), (uchar)devloc_getyloc(), (uchar)devloc_getzloc());
		print("EAR Sink node [%s] completed EAR init using defaults.\n\n\n", myID);
	}

	while (1)
	{
		start = devrtc_getusecs();

		snprintf(src, ID_LEN, "%d", uniform(0, NETWORK_SIZE-1));
		printf("\t\tSending out \"pull\" request to node [%s]\n", src);

NETTRACEMARK(14);
LOGMARK(22);
		ear_request(Ear, src, NULL, 0, 0);
LOGMARK(23);
NETTRACEMARK(15);

		sluimer = ear_sink_period - (devrtc_getusecs() - start);
		sluimer = max(sluimer, 0);
LOGMARK(2);
		xusleep(sluimer);
LOGMARK(3);
		period++;
	}

	return;		
}

void
write_log(ulong period, char *src, int leader)
{
#	define	buflen	1024
	char	buf[buflen];


	snprintf(buf, buflen,
		"\t\t@ %lu:\tdevrtc_getusecs = [%lu]\n", period, devrtc_getusecs());
	lprint(buf);
	snprintf(buf, buflen,
		"\t\t@ %lu:\tPeak source = [%s], Peak's leader = [%d]\n", period, src, leader);
	lprint(buf);
	snprintf(buf, buflen, "\n");
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
		printf("\t\tEAR Sink node [%s]: Just got non-data frame\n", myID);

		return;
	}

	now = devrtc_getusecs();

	/*	DEBUG	*/
	printf("\t\tEAR Sink node [%s]: Just got frame from node [%s], length [%d], proto [%s]\n",
		myID, payload->src, payload->len,
		(payload->nxthdr == PROTO_DAM_INTERNAL ? "DAM" : (payload->nxthdr == PROTO_EAR ? "EAR" : "UNKNOWN")));

	if (payload->nxthdr == PROTO_EAR)
	{
NETTRACEMARK(12);
LOGMARK(18);
		x = ear_recv(Ear, payload->data, payload->len);
LOGMARK(19);
NETTRACEMARK(13);

		if (x != NULL)
		{
			write_log(period, x->src, x->nxthdr);
		}
		free(x);
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
	lprint("Node [%s] Fatal: %s\n", myID, str);
	exit(-1);
}
